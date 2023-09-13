#include "hex_view_ctrl.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

BEGIN_EVENT_TABLE(HexViewCtrl, wxControl)
	EVT_PAINT(HexViewCtrl::OnPaint)
	EVT_SIZE(HexViewCtrl::OnSize)
	EVT_UPDATE_UI(wxID_ANY, HexViewCtrl::DoUpdateWindowUI)
	EVT_SCROLLWIN_THUMBTRACK(HexViewCtrl::OnThumbTracked)
	EVT_SCROLLWIN_THUMBRELEASE(HexViewCtrl::OnThumbTracked)
	EVT_SCROLLWIN_PAGEUP(HexViewCtrl::OnScrollWinPageUp)
	EVT_SCROLLWIN_PAGEDOWN(HexViewCtrl::OnScrollWinPageDown)
	EVT_SCROLLWIN_TOP(HexViewCtrl::OnScrollWinPageUp)
	EVT_SCROLLWIN_BOTTOM(HexViewCtrl::OnScrollWinPageDown)
	EVT_SCROLLWIN_LINEUP(HexViewCtrl::OnScrollWinLineUp)
	EVT_SCROLLWIN_LINEDOWN(HexViewCtrl::OnScrollWinLineDown)
END_EVENT_TABLE()

HexViewCtrl::HexViewCtrl()
: wxControl()
, m_offset(0)
, m_numOfLines(0)
, m_numOfLinesViewing(0) {}

HexViewCtrl::HexViewCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
						const wxValidator& validator, const wxString& name)
: HexViewCtrl()
{
	if (!Create(parent, id, pos, size, style, validator, name))
	{
		return;
	}
}

bool HexViewCtrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
						const wxValidator& validator, const wxString& name)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	if (!wxControl::Create(parent, id, pos, size, style | WS_VSCROLL | WS_HSCROLL, validator, name))
	{
		return false;
	}
	else
	{
		return true;
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
	wxControlBase::DoUpdateWindowUI(event);
	const auto octetCount = static_cast<int>(m_value.size());
	if (octetCount != 0)
	{
		const auto clientSize = GetClientSize();
		auto currentFont = GetFont();
		auto fontPixelSize = currentFont.GetPixelSize();
		auto numOfLines = (octetCount + 15) / 16;
		auto viewOfLines = clientSize.GetHeight() / fontPixelSize.GetHeight();
		if (viewOfLines > numOfLines) viewOfLines = numOfLines;

		bool needUpdate = m_numOfLines != numOfLines || viewOfLines != m_numOfLinesViewing;
		if (needUpdate)
		{
			m_numOfLines = numOfLines;
			m_numOfLinesViewing = viewOfLines;
		}

		SetScrollbar(wxVERTICAL, m_offset, viewOfLines, numOfLines, false);
	}

	Refresh(false);
}

void HexViewCtrl::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
#if defined(__WXMSW__)
	dc.DrawBitmap(m_backBuffer, 0, 0);
#else
	Render(dc);
#endif
}

void HexViewCtrl::OnSize(wxSizeEvent& evt)
{
#if defined(__WXMSW__)
	m_backBuffer = wxBitmap(GetClientSize());
#endif

	UpdateWindowUI();
}

void HexViewCtrl::OnThumbTracked(wxScrollWinEvent& evt)
{
	if (evt.GetOrientation() == wxVERTICAL)
	{
		m_offset = evt.GetPosition();
		Refresh();
		// SetScrollbar(wxVERTICAL, m_offset, m_numOfLinesViewing, m_numOfLines);
	}
}

bool HexViewCtrl::SetFont(const wxFont& font)
{
	auto prevFont = GetFont();
	bool ret = wxControlBase::SetFont(font);
	if (prevFont == font) return ret;

	InvalidateBestSize();
	return ret;
}

void HexViewCtrl::OnScrollWinPageUp(wxScrollWinEvent& evt)
{
	if (evt.GetOrientation() == wxVERTICAL)
	{
		const auto oldOffset = m_offset;
		m_offset = std::max(m_offset - m_numOfLinesViewing, 0);
		if (oldOffset != m_offset) wxWindow::Refresh(false);
	}
}

void HexViewCtrl::OnScrollWinPageDown(wxScrollWinEvent& evt)
{
	if (evt.GetOrientation() == wxVERTICAL)
	{
		const auto oldOffset = m_offset;
		m_offset = std::min(m_offset + m_numOfLinesViewing, m_numOfLines - m_numOfLinesViewing);
		if (oldOffset != m_offset) Refresh(false);
	}
}

void HexViewCtrl::OnScrollWinLineUp(wxScrollWinEvent& evt)
{
	if (evt.GetOrientation() == wxVERTICAL)
	{
		const auto oldOffset = m_offset;
		m_offset = std::max(m_offset - 16, 0);
		if (oldOffset != m_offset) Refresh(false);
	}
}

void HexViewCtrl::OnScrollWinLineDown(wxScrollWinEvent& evt)
{
	if (evt.GetOrientation() == wxVERTICAL)
	{
		const auto oldOffset = m_offset;
		m_offset = std::min(m_offset + 16, m_numOfLines - m_numOfLinesViewing);
		if (oldOffset != m_offset) Refresh(false);
	}
}

wxSize HexViewCtrl::DoGetBestSize() const
{
	auto currentFont = GetFont();
	auto fontPixelSize = currentFont.GetPixelSize();
	if (fontPixelSize.x == 0) fontPixelSize.x = fontPixelSize.y / 2;

	return {16 * fontPixelSize.GetWidth() * 3 + fontPixelSize.GetWidth() * 12, fontPixelSize.GetHeight() * 16};
}

void HexViewCtrl::Render(wxDC& dc)
{
	auto pContext = wxGraphicsRenderer::GetDefaultRenderer()->CreateContextFromUnknownDC(dc);
	const auto end =
		std::min((m_numOfLinesViewing + m_offset > m_numOfLines
					? m_numOfLines
					: m_numOfLinesViewing + m_offset) * 16, static_cast<int>(m_value.size()));

	auto currentFont = GetFont();
	auto fontPixelSize = currentFont.GetPixelSize();
	if (fontPixelSize.x == 0) fontPixelSize.x = fontPixelSize.y / 2;

	wxBrush brush(*wxGREY_BRUSH);
	auto clientSize = GetClientSize();
	auto oldBrush = dc.GetBrush();
	auto oldPen = dc.GetPen();
	pContext->SetBrush(*wxWHITE_BRUSH);
	pContext->DrawRectangle(0, 0, clientSize.GetWidth(), clientSize.GetHeight());
	pContext->SetFont(GetFont(), wxColour(0, 0, 0, 255));
	pContext->SetPen(*wxTRANSPARENT_PEN);
	pContext->SetBrush(brush);
	pContext->DrawRectangle(0, 0, fontPixelSize.GetWidth() * 10, clientSize.GetHeight());
	pContext->SetPen(oldPen);
	pContext->SetBrush(*wxWHITE_BRUSH);
	int y = 0;
	pContext->DrawText(wxString::Format(wxS("%08X"), m_offset * 16), fontPixelSize.GetWidth() >> 1, y);
	for (int i = m_offset * 16; i < end; ++i)
	{
		int x = (i % 16) * fontPixelSize.GetWidth() * 3 + fontPixelSize.GetWidth() * 11;
		pContext->DrawText(wxString::Format(wxS("%02X "), m_value[i]), x, y);
		if (i % 16 == 15)
		{
			y += fontPixelSize.GetHeight();
			pContext->DrawText(wxString::Format(wxS("%08X"), (i + 1)), fontPixelSize.GetWidth() >> 1, y);
		}
	}

	delete pContext;
}

void HexViewCtrl::Refresh(bool eraseBackground, const wxRect* rect)
{
	wxWindow::Refresh(eraseBackground, rect);
#if defined(__WXMSW__)
	wxMemoryDC dc(m_backBuffer);
	Render(dc);
#endif
}
