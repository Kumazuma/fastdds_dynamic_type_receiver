#pragma once

#include <wx/wx.h>
#include "main_form.h"

class View : public wxEvtHandler
{
	DECLARE_EVENT_TABLE();

public:
	View();
	bool ShowMainForm();
	void Update();

protected:
	void OnCommandRun(wxCommandEvent&);
	void OnCommandDClickedListBox(wxCommandEvent&);

private:
	MainForm* m_pMainFrame{};
};
