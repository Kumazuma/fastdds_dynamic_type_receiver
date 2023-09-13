#pragma once

#include <wx/wx.h>
#include "../read_only_sequence.h"

class HexViewCtrl : public wxControl
{
	wxDECLARE_EVENT_TABLE();

public:
	HexViewCtrl();
	HexViewCtrl(wxWindow* parent, wxWindowID id,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize, long style = 0,
				const wxValidator& validator = wxDefaultValidator,
				const wxString& name = wxASCII_STR(wxControlNameStr));

	bool Create(wxWindow* parent, wxWindowID id,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize, long style = 0,
				const wxValidator& validator = wxDefaultValidator,
				const wxString& name = wxASCII_STR(wxControlNameStr));

	void SetValue(const ReadOnlySequence<uint8_t>& value);
	void DoUpdateWindowUI(wxUpdateUIEvent& event) override;
	bool SetFont(const wxFont& font) override;
	void Refresh(bool eraseBackground = true, const wxRect* rect = nullptr) override;

protected:
	wxSize DoGetBestSize() const override;

protected:
	void OnPaint(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	void OnThumbTracked(wxScrollWinEvent&);
	void OnScrollWinPageUp(wxScrollWinEvent&);
	void OnScrollWinPageDown(wxScrollWinEvent&);
	void OnScrollWinLineUp(wxScrollWinEvent&);
	void OnScrollWinLineDown(wxScrollWinEvent&);
	void Render(wxDC& dc);

private:
	ReadOnlySequence<uint8_t> m_value;
	int m_offset;
	int m_numOfLines;
	int m_numOfLinesViewing;
#if defined(__WXMSW__)
	wxBitmap m_backBuffer;
#endif
};
