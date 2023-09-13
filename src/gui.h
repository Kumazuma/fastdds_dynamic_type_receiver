///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class AuiPanel;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/stc/stc.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/wizard.h>
#include <wx/dynarray.h>
WX_DEFINE_ARRAY_PTR( wxWizardPageSimple*, WizardPages );
#include <wx/listbox.h>
#include <wx/splitter.h>
#include <wx/aui/aui.h>

///////////////////////////////////////////////////////////////////////////

#define ID_PLAY 1000
#define ID_STOP 1001
#define ID_WIZARD_FIRST_PAGE 1002
#define ID_TXT_XML 1003
#define ID_TYPE_NAME 1004
#define ID_PROP_CONFIG 1005
#define ID_LIST_DATA 1006
#define ID_PROP_DATA 1007

///////////////////////////////////////////////////////////////////////////////
/// Class MainFormBase
///////////////////////////////////////////////////////////////////////////////
class MainFormBase : public wxFrame
{
	private:

	protected:
		AuiPanel* m_panel9;
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_tool1;
		wxToolBarToolBase* m_tool2;

	public:

		MainFormBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,393 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainFormBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MyWizard1
///////////////////////////////////////////////////////////////////////////////
class MyWizard1 : public wxWizard
{
	private:

	protected:
		wxButton* m_button4;
		wxStyledTextCtrl* m_scintilla2;
		wxTextCtrl* m_textCtrl1;
		wxPropertyGridManager* m_propertyGridManager2;
		wxPropertyGridPage* m_propertyGridPage1;

	public:

		MyWizard1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxBitmap& bitmap = wxNullBitmap, const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		WizardPages m_pages;

		~MyWizard1();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AuiPanel
///////////////////////////////////////////////////////////////////////////////
class AuiPanel : public wxPanel
{
	private:

	protected:
		wxPanel* m_panel5;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel10;
		wxListBox* m_listBox1;
		wxPanel* m_panel11;
		wxPropertyGrid* m_propertyGrid1;
		wxPanel* m_panel7;
		wxPropertyGridManager* m_propertyGridManager1;
		wxPropertyGridPage* m_propertyGridPage2;

	public:

		AuiPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		wxAuiManager m_mgr;

		~AuiPanel();

		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 0 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( AuiPanel::m_splitter2OnIdle ), NULL, this );
		}

};

