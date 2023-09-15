#include "hex_view_ctrl.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

wxDEFINE_EVENT(wxEVT_HEX_VIEW_SELECTION_CHANGED, wxCommandEvent);

BEGIN_EVENT_TABLE(HexViewCtrl, wxScrolled<wxWindow>)
	EVT_PAINT(HexViewCtrl::OnPaint)
	EVT_UPDATE_UI(wxID_ANY, HexViewCtrl::DoUpdateWindowUI)
	EVT_SIZE(HexViewCtrl::OnSize)
	EVT_LEFT_DOWN(HexViewCtrl::OnMouseEvent)
	EVT_MOTION(HexViewCtrl::OnMouseEvent)
	EVT_LEFT_UP(HexViewCtrl::OnMouseEvent)
	EVT_LEAVE_WINDOW(HexViewCtrl::OnMouseLeave)
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
	m_pRenderer = wxGraphicsRenderer::GetDefaultRenderer();


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
		m_fontMatrics = dc.GetFontMetrics();
		dc.GetTextExtent(wxS("0"), &fontSize.x, &fontSize.y);
		m_lineHeight = m_fontMatrics.height;
		if(m_fontMatrics.internalLeading < 4)
		{
			m_fontMatrics.internalLeading = 4;
			m_lineHeight += 4; 
		}

		m_widthOfChar = fontSize.x;
		if(m_widthOfChar < m_fontMatrics.averageWidth)
		{
			m_widthOfChar = m_fontMatrics.averageWidth;
		}

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
	wxRect box{};
#if defined(__WXMSW__)
	wxBufferedPaintDC dc(this, m_backBuffer);
#else
	wxPaintDC dc(this);
#endif
	PrepareDC(dc);
	dc.Clear();
	auto context = m_pRenderer->CreateContext(dc);
	Render(context, wxPoint{});

	if(m_selectionBegin != -1)
	{
		RenderSelectionOverlay(context);
	}

	delete context;
}

void HexViewCtrl::MSWUpdateFontOnDPIChange(const wxSize& newDPI)
{
	wxScrolled<wxWindow>::MSWUpdateFontOnDPIChange(newDPI);
	m_font = m_pRenderer->CreateFont(GetFont());
	UpdateWindowUI();
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
	context->SetContentScaleFactor(GetContentScaleFactor());
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
			context->DrawText(wxString::Format(wxS("%08X"), (i + 1)), 5, y + m_lineHeight);
		}
	}
}

void HexViewCtrl::RenderSelectionOverlay(wxGraphicsContext* context)
{
	auto offsetStart = m_selectionBegin;
	auto offsetEnd = m_selectionEnd == -1 ? m_selectionUnderCursor : m_selectionEnd;
	if(offsetStart > offsetEnd)
		std::swap(offsetStart, offsetEnd);

	const auto clientSize = GetClientSize();
	const auto offset = GetScrollPos(wxVERTICAL);
	const auto viewOfLines = (clientSize.GetHeight() + m_lineHeight - 1) / m_lineHeight;

	const auto beginRow = std::max(offsetStart / 16, offset - 1);
	const auto endRow = std::min(offsetEnd / 16 + 1, offset + viewOfLines + 1);
	auto b = context->CreateBrush(wxBrush(wxColor(100, 100, 255, 128)));
	context->SetBrush(b);
	for(int row = beginRow; row < endRow; ++row)
	{
		int colsStart = offsetStart <= row * 16 ? 0 : (offsetStart - row * 16);
		int colsEnd = (row + 1) * 16 <= offsetEnd ? 15 : (offsetEnd - row * 16);
		context->DrawRectangle(colsStart * m_widthOfChar * 3 + m_widthOfChar * 11, row * m_lineHeight, (colsEnd - colsStart + 1) * 3 * m_widthOfChar - m_widthOfChar, m_lineHeight + 1);
	}
}

void HexViewCtrl::OnMouseEvent(wxMouseEvent& evt)
{
	if(m_lineHeight == 0 && m_value.size() != 0)
	{
		return;
	}

	if(evt.LeftDown() || (evt.Dragging() && m_selectionBegin != -1))
	{
		auto logicalPosition = CalcUnscrolledPosition(evt.GetPosition());
		int row = logicalPosition.y / m_lineHeight;
		int cols = (logicalPosition.x - m_widthOfChar * 11) / (m_widthOfChar * 3);
		if(cols < 0 || cols > 15)
		{
			return;
		}

		int offset = row * 16 + cols;
		if(offset >= m_value.size())
		{
			return;
		}

		const bool needRepaint = m_selectionUnderCursor != offset;
		m_selectionUnderCursor = offset;
		if(m_selectionBegin == -1)
		{
			m_selectionBegin = m_selectionUnderCursor;
		}
		else if(m_selectionEnd != -1)
		{
			m_selectionBegin = m_selectionUnderCursor;
			m_selectionEnd = -1;
		}

		if(needRepaint)
		{
			Refresh(true);
			if(m_selectionUnderCursor != -1)
			{
				wxCommandEvent event(wxEVT_HEX_VIEW_SELECTION_CHANGED, GetId());
				event.SetEventObject(this);
				event.SetInt(m_selectionUnderCursor);
				HandleWindowEvent(event);
			}
		}
	}
	else if(m_selectionBegin != -1 && m_selectionEnd == -1)
	{
		// Finish selection
		auto logicalPosition = CalcUnscrolledPosition(evt.GetPosition());
		int row = logicalPosition.y / m_lineHeight;
		int cols = (logicalPosition.x - m_widthOfChar * 11) / (m_widthOfChar * 3);
		if(cols < 0 || cols > 15)
		{
			m_selectionEnd = m_selectionUnderCursor;
			m_selectionUnderCursor = -1;
			return;
		}

		int offset = row * 16 + cols;
		if(offset >= m_value.size())
		{
			offset = m_value.size() - 1;
		}

		const bool needRepaint = m_selectionUnderCursor != offset;
		m_selectionUnderCursor = offset;
		m_selectionEnd = m_selectionUnderCursor;
		if(needRepaint)
		{
			Refresh(true);
			if(m_selectionUnderCursor != -1)
			{
				wxCommandEvent event(wxEVT_HEX_VIEW_SELECTION_CHANGED, GetId());
				event.SetEventObject(this);
				event.SetInt(m_selectionUnderCursor);
				HandleWindowEvent(event);
			}
		}

		m_selectionUnderCursor = -1;
	}
}

void HexViewCtrl::OnMouseLeave(wxMouseEvent& evt)
{
	if(evt.LeftDown() && m_selectionBegin != -1 && m_selectionEnd == -1 && m_value.size() != 0)
	{
		// Finish selection
		auto logicalPosition = CalcUnscrolledPosition(evt.GetPosition());
		int row = logicalPosition.y / m_lineHeight;
		int cols = (logicalPosition.x - m_widthOfChar * 11) / (m_widthOfChar * 3);
		if(cols < 0 || cols > 15)
		{
			m_selectionEnd = m_selectionUnderCursor;
			m_selectionUnderCursor = -1;
			return;
		}

		int offset = row * 16 + cols;
		if(offset >= m_value.size())
		{
			offset = m_value.size() - 1;
		}

		const bool needRepaint = m_selectionUnderCursor != offset;
		m_selectionUnderCursor = offset;
		m_selectionEnd = m_selectionUnderCursor;
		if(needRepaint)
		{
			Refresh(true);
			if(m_selectionUnderCursor != -1)
			{
				wxCommandEvent event(wxEVT_HEX_VIEW_SELECTION_CHANGED, GetId());
				event.SetEventObject(this);
				event.SetInt(m_selectionUnderCursor);
				HandleWindowEvent(event);
			}
		}

		m_selectionUnderCursor = -1;
	}
}

void HexViewCtrl::Refresh(bool eraseBackground, const wxRect* rect)
{
	wxScrolled<wxWindow>::Refresh(eraseBackground, rect);
}
