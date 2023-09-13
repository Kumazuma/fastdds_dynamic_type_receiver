///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

MainFormBase::MainFormBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );

	m_panel9 = new AuiPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer16->Add( m_panel9, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL|wxTB_HORZ_TEXT, wxID_ANY );
	m_tool1 = m_toolBar1->AddTool( ID_PLAY, wxT("Play"), wxArtProvider::GetBitmap( wxART_NORMAL_FILE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_tool2 = m_toolBar1->AddTool( ID_STOP, wxT("Stop"), wxArtProvider::GetBitmap( wxART_NORMAL_FILE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->Realize();


	this->Centre( wxBOTH );
}

MainFormBase::~MainFormBase()
{
}

MyWizard1::MyWizard1( wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style )
{
	this->Create( parent, id, title, bitmap, pos, style );

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxWizardPageSimple* m_wizPage1 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage1 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_button4 = new wxButton( m_wizPage1, wxID_OPEN, wxT("Load from file"), wxDefaultPosition, wxDefaultSize, 0 );

	m_button4->SetBitmap( wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_BUTTON ) );
	bSizer2->Add( m_button4, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_wizPage1, wxID_ANY, wxT("XML") ), wxVERTICAL );

	m_scintilla2 = new wxStyledTextCtrl( sbSizer2->GetStaticBox(), ID_TXT_XML, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_scintilla2->SetUseTabs( true );
	m_scintilla2->SetTabWidth( 4 );
	m_scintilla2->SetIndent( 4 );
	m_scintilla2->SetTabIndents( true );
	m_scintilla2->SetBackSpaceUnIndents( true );
	m_scintilla2->SetViewEOL( false );
	m_scintilla2->SetViewWhiteSpace( false );
	m_scintilla2->SetMarginWidth( 2, 0 );
	m_scintilla2->SetIndentationGuides( true );
	m_scintilla2->SetReadOnly( false );
	m_scintilla2->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_scintilla2->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_scintilla2->SetMarginWidth( 1, 16);
	m_scintilla2->SetMarginSensitive( 1, true );
	m_scintilla2->SetProperty( wxT("fold"), wxT("1") );
	m_scintilla2->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_scintilla2->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_scintilla2->SetMarginWidth( 0, m_scintilla2->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_scintilla2->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_scintilla2->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	sbSizer2->Add( m_scintilla2, 1, wxEXPAND, 5 );


	bSizer2->Add( sbSizer2, 1, wxEXPAND|wxALL, 5 );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_wizPage1, wxID_ANY, wxT("Type name") ), wxVERTICAL );

	m_textCtrl1 = new wxTextCtrl( sbSizer3->GetStaticBox(), ID_TYPE_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textCtrl1->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrl1->SetMaxLength( 255 );
	}
	#else
	m_textCtrl1->SetMaxLength( 255 );
	#endif
	sbSizer3->Add( m_textCtrl1, 0, wxALL|wxEXPAND, 5 );


	bSizer2->Add( sbSizer3, 0, wxEXPAND|wxALL, 5 );


	m_wizPage1->SetSizer( bSizer2 );
	m_wizPage1->Layout();
	bSizer2->Fit( m_wizPage1 );
	wxWizardPageSimple* m_wizPage31 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage31 );

	wxStaticBoxSizer* sbSizer92;
	sbSizer92 = new wxStaticBoxSizer( new wxStaticBox( m_wizPage31, wxID_ANY, wxT("Configuration") ), wxVERTICAL );

	m_propertyGridManager2 = new wxPropertyGridManager(sbSizer92->GetStaticBox(), ID_PROP_CONFIG, wxDefaultPosition, wxDefaultSize, wxPGMAN_DEFAULT_STYLE|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER);
	m_propertyGridManager2->SetExtraStyle( wxPG_EX_MODE_BUTTONS );

	m_propertyGridPage1 = m_propertyGridManager2->AddPage( wxT("Page"), wxNullBitmap );
	sbSizer92->Add( m_propertyGridManager2, 1, wxALL|wxEXPAND, 5 );


	m_wizPage31->SetSizer( sbSizer92 );
	m_wizPage31->Layout();
	sbSizer92->Fit( m_wizPage31 );

	this->Centre( wxBOTH );

	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}
}

MyWizard1::~MyWizard1()
{
	m_pages.Clear();
}

AuiPanel::AuiPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_HINT_FADE|wxAUI_MGR_LIVE_RESIZE|wxAUI_MGR_TRANSPARENT_DRAG);

	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel5, wxAuiPaneInfo() .Center() .CaptionVisible( false ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_splitter2 = new wxSplitterWindow( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( AuiPanel::m_splitter2OnIdle ), NULL, this );
	m_splitter2->SetMinimumPaneSize( 100 );

	m_panel10 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_panel10, wxID_ANY, wxT("Samples") ), wxVERTICAL );

	m_listBox1 = new wxListBox( sbSizer10->GetStaticBox(), ID_LIST_DATA, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE );
	sbSizer10->Add( m_listBox1, 1, wxALL|wxEXPAND, 5 );


	m_panel10->SetSizer( sbSizer10 );
	m_panel10->Layout();
	sbSizer10->Fit( m_panel10 );
	m_panel11 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( m_panel11, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_propertyGrid1 = new wxPropertyGrid(sbSizer11->GetStaticBox(), ID_PROP_DATA, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	sbSizer11->Add( m_propertyGrid1, 1, wxEXPAND|wxALL, 5 );


	m_panel11->SetSizer( sbSizer11 );
	m_panel11->Layout();
	sbSizer11->Fit( m_panel11 );
	m_splitter2->SplitVertically( m_panel10, m_panel11, 0 );
	bSizer6->Add( m_splitter2, 1, wxEXPAND, 5 );


	m_panel5->SetSizer( bSizer6 );
	m_panel5->Layout();
	bSizer6->Fit( m_panel5 );
	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel7, wxAuiPaneInfo() .Left() .Caption( wxT("Subscription setting") ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).RightDockable( false ).Floatable( false ).MinSize( wxSize( 150,150 ) ) );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_propertyGridManager1 = new wxPropertyGridManager(m_panel7, ID_PROP_CONFIG, wxDefaultPosition, wxSize( 200,200 ), wxPGMAN_DEFAULT_STYLE|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER);
	m_propertyGridManager1->SetExtraStyle( wxPG_EX_HIDE_PAGE_BUTTONS|wxPG_EX_NATIVE_DOUBLE_BUFFERING );
	m_propertyGridManager1->SetMinSize( wxSize( 200,200 ) );


	m_propertyGridPage2 = m_propertyGridManager1->AddPage( wxT("Page"), wxNullBitmap );
	bSizer9->Add( m_propertyGridManager1, 1, wxALL|wxEXPAND, 5 );


	m_panel7->SetSizer( bSizer9 );
	m_panel7->Layout();
	bSizer9->Fit( m_panel7 );

	m_mgr.Update();
}

AuiPanel::~AuiPanel()
{
	m_mgr.UnInit();

}
