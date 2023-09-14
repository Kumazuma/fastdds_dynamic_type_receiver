#include "hex_view_ctrl.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

BEGIN_EVENT_TABLE(HexViewCtrl, wxScrolled<wxWindow>)
	EVT_PAINT(HexViewCtrl::OnPaint)
	EVT_UPDATE_UI(wxID_ANY, HexViewCtrl::DoUpdateWindowUI)
	EVT_SIZE(HexViewCtrl::OnSize)
	EVT_SCROLLWIN(HexViewCtrl::OnScrollWinEvent)
END_EVENT_TABLE()

HexViewCtrl::HexViewCtrl()
: wxScrolled<wxWindow>()
, m_numOfLines(0) {}

HexViewCtrl::HexViewCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: HexViewCtrl()
{
	if (!Create(parent, id, pos, size, style, name))
	{
		return;
	}
}

bool HexViewCtrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
#if defined(__WXMSW__)
	auto pRenderer = wxGraphicsRenderer::GetDirect2DRenderer();
#else
	auto pRenderer = wxGraphicsRenderer::GetDefaultRenderer();
#endif
	m_blackPen = pRenderer->CreatePen(wxGraphicsPenInfo{wxColour(0, 0, 0, 1)});
	m_transparentPen = pRenderer->CreatePen(wxGraphicsPenInfo{wxColour(0, 0, 0, 0)});
	m_grayBrush = pRenderer->CreateBrush(*wxGREY_BRUSH);
	m_whiteBrush = pRenderer->CreateBrush(*wxWHITE_BRUSH);
	m_font = pRenderer->CreateFont(GetFont());

	SetBackgroundStyle(wxBG_STYLE_PAINT);
	if (!wxScrolled<wxWindow>::Create(parent, id, pos, size, style | WS_VSCROLL | WS_HSCROLL, name))
	{
		return false;
	}



	return true;
}

void HexViewCtrl::SetValue(const ReadOnlySequence<uint8_t>& value)
{
	m_value = value;
	UpdateWindowUI();
}

void HexViewCtrl::DoUpdateWindowUI(wxUpdateUIEvent& event)
{
	const auto octetCount = static_cast<int>(m_value.size());
	if (octetCount != 0)
	{
		wxClientDC dc(this);
		wxSize fontSize;
		dc.GetTextExtent(wxS("0"), &fontSize.x, &fontSize.y);
		const auto clientSize = GetClientSize();
		auto numOfLines = (octetCount + 15) / 16;
		wxSize virtualSize{};
		virtualSize.y = numOfLines * fontSize.y;
		virtualSize.x = 16 * fontSize.x * 3 + fontSize.x * 12;
		bool needSetVirtualSize = m_numOfLines != numOfLines;
		auto oldVirtualSize = GetVirtualSize();
		needSetVirtualSize = needSetVirtualSize || (oldVirtualSize != virtualSize);
		m_numOfLines = numOfLines;
		if(needSetVirtualSize)
		{
			SetScrollRate(1, fontSize.y);
			SetVirtualSize(virtualSize);
		}
	}
}

void HexViewCtrl::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	
#if defined(__WXMSW__)
	Render(dc);
	 /*wxMemoryDC memDC(m_backBuffer);
	 dc.Blit(wxPoint(0, 0), m_backBuffer.GetSize(), &memDC, wxPoint(0, 0));*/
#else
	Render(dc);
#endif
}

void HexViewCtrl::OnSize(wxSizeEvent& evt)
{
	
	m_backBuffer = wxBitmap(GetClientSize());
	wxMemoryDC dc(m_backBuffer);
	Render(dc);
}

bool HexViewCtrl::SetFont(const wxFont& font)
{
	auto prevFont = GetFont();
	bool ret = wxScrolled<wxWindow>::SetFont(font);
	if (prevFont == font) return ret;
#if defined(__WXMSW__)
	auto pRenderer = wxGraphicsRenderer::GetDirect2DRenderer();
#else
	auto pRenderer = wxGraphicsRenderer::GetDefaultRenderer();
#endif

	m_font = pRenderer->CreateFont(font);
	UpdateWindowUI();
	return ret;
}

void HexViewCtrl::Render(wxClientDC& dc)
{
#if defined(__WXMSW__)
	auto pContext = wxGraphicsRenderer::GetDirect2DRenderer()->CreateContext(dc);
#else
	auto pContext = wxGraphicsRenderer::GetDefaultRenderer()->CreateContext(dc);
#endif

	Render(pContext);
	delete pContext;
}

void HexViewCtrl::Render(wxMemoryDC& dc)
{
#if defined(__WXMSW__)
	auto pContext = wxGraphicsRenderer::GetDirect2DRenderer()->CreateContext(dc);
#else
	auto pContext = wxGraphicsRenderer::GetDefaultRenderer()->CreateContext(dc);
#endif

	Render(pContext);
	delete pContext;
}

void HexViewCtrl::Render(wxGraphicsContext* pContext)
{
	auto currentFont = GetFont();
	wxClientDC dc(this);
	wxSize fontSize;
	dc.GetTextExtent(wxS("0"), &fontSize.x, &fontSize.y);

	pContext->SetContentScaleFactor(GetDPIScaleFactor());
	pContext->SetFont(m_font);
	auto clientSize = GetClientSize();
	pContext->SetBrush(m_whiteBrush);
	pContext->DrawRectangle(0, 0, clientSize.GetWidth(), clientSize.GetHeight());
	
	pContext->SetPen(m_transparentPen);
	pContext->SetBrush(m_grayBrush);
	pContext->DrawRectangle(0, 0, fontSize.x * 10, clientSize.GetHeight());
	pContext->SetPen(m_blackPen);
	pContext->SetBrush(m_whiteBrush);
	double y = 0;
	int offset = GetScrollPos(wxVERTICAL);
	
	auto viewOfLines = (clientSize.GetHeight() + ((int)floor(fontSize.y)) - 1) / ((int)floor(fontSize.y));

	const auto end =
	std::min((viewOfLines + offset > m_numOfLines
				? m_numOfLines
				: viewOfLines + offset) * 16, static_cast<int>(m_value.size()));

	pContext->DrawText(wxString::Format(wxS("%08X"), offset * 16), fontSize.x * 0.5, y);

	for (int i = offset * 16; i < end; ++i)
	{
		int x = (i % 16) * fontSize.x * 3 + fontSize.x * 11;
		pContext->DrawText(wxString::Format(wxS("%02X "), m_value[i]), x, y);
		if (i % 16 == 15)
		{
			y += fontSize.y;
			pContext->DrawText(wxString::Format(wxS("%08X"), (i + 1)), fontSize.x * 0.5, y);
		}
	}
}

void HexViewCtrl::OnScrollWinEvent(wxScrollWinEvent& evt)
{
	evt.Skip();
	wxMemoryDC dc(m_backBuffer);
	Render(dc);
}

//void HexViewCtrl::RecalcScrollBar()
//{
//	const auto octetCount = static_cast<int>(m_value.size());
//	if (octetCount != 0)
//	{
//		const auto clientSize = GetClientSize();
//		wxClientDC dc(const_cast<HexViewCtrl*>(this));
//		auto s = dc.GetTextExtent(wxS("0"));
//		auto currentFont = GetFont();
//		auto numOfLines = (octetCount + 15) / 16;
//		auto viewOfLines = (clientSize.GetHeight() + s.y - 1) / s.y;
//		if (viewOfLines > numOfLines) viewOfLines = numOfLines;
//
//		bool needUpdate = m_numOfLines != numOfLines || viewOfLines != m_numOfLinesViewing;
//		SetScrollbar(wxVERTICAL, m_offset, viewOfLines, numOfLines, false);
//		if (needUpdate)
//		{
//			m_numOfLines = numOfLines;
//			m_numOfLinesViewing = viewOfLines;
//			Refresh(false);
//			Update();
//		}
//
//	}
//}

void HexViewCtrl::Refresh(bool eraseBackground, const wxRect* rect)
{
#if defined(__WXMSW__)
	// wxMemoryDC dc(m_backBuffer);
	// Render(dc);
#endif
	wxScrolled<wxWindow>::Refresh(eraseBackground, rect);
}
