#include "sequence_view_dialog.h"
#include <wx/artprov.h>

SequenceViewDialog<uint8_t>::SequenceViewDialog(wxWindow* parent, wxWindowID id, const wxString& title) :
wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	m_pHexViewCtrl = new HexViewCtrl(this, wxID_ANY);
	auto font = m_pHexViewCtrl->GetFont();
	font.SetPointSize(font.GetPointSize() + font.GetPointSize() / 2);
	m_pHexViewCtrl->SetFont(font);

	m_pToolbar = new wxToolBar(this, wxID_ANY);
	auto pToolSave = m_pToolbar->AddTool(wxID_SAVE, wxS("Save as file"),
										wxArtProvider::GetBitmapBundle(wxART_FILE_SAVE, wxART_TOOLBAR));
	pToolSave->SetShortHelp(wxS("Save binary as file"));
	auto pToolOpenAs = m_pToolbar->AddTool(wxID_OPEN, wxS("Try show as a image"),
											wxArtProvider::GetBitmapBundle(wxART_FILE_OPEN, wxART_TOOLBAR));
	pToolOpenAs->SetShortHelp(wxS("Try decode a image and show it"));
	m_pToolbar->Realize();
	auto bsizer = new wxBoxSizer(wxVERTICAL);
	bsizer->Add(m_pToolbar, 0, wxEXPAND);
	bsizer->Add(m_pHexViewCtrl, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(bsizer);
}

void SequenceViewDialog<uint8_t>::SetValue(const ReadOnlySequence<uint8_t>& value)
{
	m_value = value;
	m_pHexViewCtrl->SetValue(value);
}
