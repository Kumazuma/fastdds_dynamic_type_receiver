#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include "../read_only_sequence.h"

class HexViewCtrl : public wxScrolled<wxWindow>
{
	wxDECLARE_EVENT_TABLE();

public:
	HexViewCtrl();
	HexViewCtrl(wxWindow* parent, wxWindowID id,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize, long style = wxScrolledWindowStyle,
				const wxString& name = wxASCII_STR(wxControlNameStr));

	bool Create(wxWindow* parent, wxWindowID id,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize, long style = wxScrolledWindowStyle,
				const wxString& name = wxASCII_STR(wxControlNameStr));

	void SetValue(const ReadOnlySequence<uint8_t>& value);
	void DoUpdateWindowUI(wxUpdateUIEvent& event) override;
	bool SetFont(const wxFont& font) override;
	void Refresh(bool eraseBackground = true, const wxRect* rect = nullptr) override;
protected:
	void OnPaint(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	void Render(wxMemoryDC& dc);
private:
	ReadOnlySequence<uint8_t> m_value;
	int m_numOfLines;
	wxGraphicsBrush m_whiteBrush;
	wxGraphicsBrush m_grayBrush;
	wxGraphicsPen m_blackPen;
	wxGraphicsPen m_transparentPen;
	wxGraphicsFont m_font;
#if defined(__WXMSW__)
	wxBitmap m_backBuffer;
#endif
};
