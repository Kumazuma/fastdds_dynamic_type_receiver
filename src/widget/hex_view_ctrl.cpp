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
	if (!wxScrolled<wxWindow>::Create(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name))
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
			SetScrollRate(fontSize.x, fontSize.y);
			SetVirtualSize(virtualSize);
		}
	}
}

void HexViewCtrl::OnPaint(wxPaintEvent& evt)
{
#if defined(__WXMSW__)
	wxBufferedPaintDC dc(this, m_backBuffer, wxBUFFER_CLIENT_AREA);
#else
	wxPaintDC dc(this);
#endif
	PrepareDC(dc);
	Render(dc);
}

void HexViewCtrl::OnSize(wxSizeEvent& evt)
{
	wxClientDC dc(this);
	auto size = evt.GetSize();
	m_backBuffer = wxBitmap(size.x, size.y, dc);
}

bool HexViewCtrl::SetFont(const wxFont& font)
{
	auto prevFont = GetFont();
	bool ret = wxScrolled<wxWindow>::SetFont(font);
	if (prevFont == font) return ret;
	UpdateWindowUI();
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

void HexViewCtrl::Refresh(bool eraseBackground, const wxRect* rect)
{
	wxScrolled<wxWindow>::Refresh(eraseBackground, rect);
}
