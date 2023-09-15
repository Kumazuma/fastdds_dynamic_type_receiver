#include "hex_view_ctrl.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

BEGIN_EVENT_TABLE(HexViewCtrl, wxScrolled<wxWindow>)
	EVT_PAINT(HexViewCtrl::OnPaint)
	EVT_UPDATE_UI(wxID_ANY, HexViewCtrl::DoUpdateWindowUI)
	EVT_SIZE(HexViewCtrl::OnSize)
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
	m_pRenderer = wxGraphicsRenderer::GetDirect2DRenderer();


	SetBackgroundStyle(wxBG_STYLE_PAINT);
	if (!wxScrolled<wxWindow>::Create(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name))
	{
		return false;
	}

	m_blackPen = m_pRenderer->CreatePen(wxGraphicsPenInfo{wxColour(0, 0, 0, 1)});
	m_transparentPen = m_pRenderer->CreatePen(wxGraphicsPenInfo{wxColour(0, 0, 0, 0)});
	m_grayBrush = m_pRenderer->CreateBrush(*wxGREY_BRUSH);
	m_whiteBrush = m_pRenderer->CreateBrush(*wxWHITE_BRUSH);
	m_font = m_pRenderer->CreateFont(GetFont());

	wxClientDC dc(this);
	dc.SetFont(GetFont());
	dc.GetTextExtent(wxS("0"), &m_widthOfChar, &m_lineHeight);
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
		dc.SetFont(GetFont());
		wxSize fontSize;
		dc.GetTextExtent(wxS("0"), &fontSize.x, &fontSize.y);
		m_lineHeight = fontSize.y;
		m_widthOfChar = fontSize.x;
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
			SetScrollRate(fontSize.x, fontSize.y);
			SetVirtualSize(virtualSize);
		}
	}
}

void HexViewCtrl::OnPaint(wxPaintEvent& evt)
{
#if defined(__WXMSW__)
	wxBufferedPaintDC dc(this, m_backBuffer);
#else
	wxPaintDC dc(this);
#endif
	dc.Clear();
	PrepareDC(dc);
	auto context = m_pRenderer->CreateContext(dc);
	Render(context, wxPoint{});
	delete context;
}

void HexViewCtrl::OnSize(wxSizeEvent& evt)
{
#if defined(__WXMSW__)
	wxClientDC dc(this);
	auto size = evt.GetSize();
	m_backBuffer = wxBitmap(size.x, size.y, dc);
#endif
}

wxSize HexViewCtrl::DoGetBestClientSize() const
{
	wxSize virtualSize{};
	virtualSize.y = 16 * m_lineHeight;
	virtualSize.x = 16 * 3 * m_widthOfChar + m_widthOfChar * 12;
	return virtualSize;
}

bool HexViewCtrl::SetFont(const wxFont& font)
{
	auto prevFont = GetFont();
	bool ret = wxScrolled<wxWindow>::SetFont(font);
	if (prevFont == font) return ret;
	UpdateWindowUI();
	m_font = m_pRenderer->CreateFont(font);

	return ret;
}

void HexViewCtrl::Render(wxMemoryDC& dc)
{
	auto currentFont = GetFont();
	wxSize fontSize;
	dc.Clear();
	dc.SetFont(currentFont);
	dc.GetTextExtent(wxS("0"), &fontSize.x, &fontSize.y);
	auto clientSize = GetClientSize();
	dc.SetPen(*wxTRANSPARENT_PEN);
	auto origin = dc.GetDeviceOrigin();
	int offset = GetScrollPos(wxVERTICAL);
	int y = offset * fontSize.y;
	dc.SetBrush(*wxGREY_BRUSH);
	dc.DrawRectangle(0, y, fontSize.x * 10, clientSize.GetHeight());
	dc.SetPen(*wxBLACK_PEN);
	auto viewOfLines = (clientSize.GetHeight() + fontSize.y - 1) /fontSize.y;

	const auto end =
	std::min((viewOfLines + offset > m_numOfLines
				? m_numOfLines
				: viewOfLines + offset) * 16, static_cast<int>(m_value.size()));

	dc.DrawText(wxString::Format(wxS("%08X"), offset * 16), 5, y);

	for (int i = offset * 16; i < end; ++i)
	{
		int x = (i % 16) * fontSize.x * 3 + fontSize.x * 11;
		dc.DrawText(wxString::Format(wxS("%02X "), m_value[i]), x, y);
		if (i % 16 == 15)
		{
			y += fontSize.y;
			dc.DrawText(wxString::Format(wxS("%08X"), (i + 1)), 5, y);
		}
	}
}

void HexViewCtrl::Render(wxGraphicsContext* context, const wxPoint& origin)
{
	auto virtualSize = GetVirtualSize();
	context->SetFont(m_font);
	auto clientSize = GetClientSize();
	context->SetPen(m_transparentPen);
	int offset = GetScrollPos(wxVERTICAL);
	int y = 0;
	context->SetBrush(m_grayBrush);
	context->DrawRectangle(0, y, m_widthOfChar * 10, virtualSize.GetHeight() + m_lineHeight);
	context->SetPen(m_blackPen);
	const auto viewOfLines = (clientSize.GetHeight() + m_lineHeight - 1) / m_lineHeight;

	const auto end =
		std::min((viewOfLines + offset + 1 > m_numOfLines
			? m_numOfLines
			: viewOfLines + offset + 1) * 16, static_cast<int>(m_value.size()));

	context->DrawText(wxString::Format(wxS("%08X"), offset * 16), 5, 0);

	for (int i = std::max(0, (offset - 1)* 16); i < end; ++i)
	{
		y = (i / 16) * m_lineHeight;
		int x = (i % 16) * m_widthOfChar * 3 + m_widthOfChar * 11;
		context->DrawText(wxString::Format(wxS("%02X "), m_value[i]), x, y);
		if (i % 16 == 15 && (i + 1) != end)
		{
			context->DrawText(wxString::Format(wxS("%08X"), (i + 1)), 5, (1 + i / 16) * m_lineHeight);
		}
	}
}

void HexViewCtrl::Refresh(bool eraseBackground, const wxRect* rect)
{
	wxScrolled<wxWindow>::Refresh(eraseBackground, rect);
}
