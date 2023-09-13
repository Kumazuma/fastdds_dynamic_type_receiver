#include "sequence_view_dialog.h"
#include <wx/artprov.h>
#include <wx/file.h>

BEGIN_EVENT_TABLE(SequenceViewDialog<uint8_t>, wxDialog)
	EVT_TOOL(wxID_SAVE, SequenceViewDialog::OnCommandSaveFile)
	EVT_TOOL(wxID_OPEN, SequenceViewDialog::OnCommandOpenAsImage)
END_EVENT_TABLE()

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
											wxArtProvider::GetBitmapBundle(wxART_NORMAL_FILE, wxART_TOOLBAR));
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

void SequenceViewDialog<unsigned char>::OnCommandSaveFile(wxCommandEvent& evt)
{
	if(m_value.size() == 0)
	{
		return;
	}

	wxFileDialog fileDialog(this, wxS("Save binary as a file"), wxEmptyString, wxEmptyString, wxS("*.*|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	auto ret = fileDialog.ShowModal();
	if(ret != wxID_OK)
	{
		return;
	}

	auto path = fileDialog.GetPath();
	wxFile file;
	if(!file.Open(path, wxFile::write))
	{
		wxMessageDialog diloag(this, wxS("Failed to open file"), wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR);
		diloag.CenterOnParent();
		diloag.ShowModal();
		return;
	}

	auto& binary = m_value.Get();
	size_t offset = 0;
	while(binary.size() != offset)
	{
		auto written = file.Write(binary.data() + offset, binary.size() - offset);
		if(file.Error())
		{
			wxMessageDialog diloag(this, wxS("Failed to write to file"), wxMessageBoxCaptionStr, wxOK | wxCENTER | wxICON_ERROR);
			diloag.CenterOnParent();
			diloag.ShowModal();
			return;
		}

		offset += written;
	}
}

void SequenceViewDialog<unsigned char>::OnCommandOpenAsImage(wxCommandEvent& evt)
{
	
}
