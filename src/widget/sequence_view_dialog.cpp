#include "sequence_view_dialog.h"
#include <wx/artprov.h>
#include <wx/file.h>
#include <wx/propgrid/propgrid.h>

BEGIN_EVENT_TABLE(SequenceViewDialog<uint8_t>, wxDialog)
	EVT_TOOL(wxID_SAVE, SequenceViewDialog::OnCommandSaveFile)
	EVT_TOOL(wxID_OPEN, SequenceViewDialog::OnCommandOpenAsImage)
	EVT_COMMAND(wxID_ANY, wxEVT_HEX_VIEW_SELECTION_CHANGED, SequenceViewDialog::OnCommandHexViewSelectionChanged)
	EVT_PG_CHANGED(wxID_ANY, SequenceViewDialog::OnPropGridValueChanged)
END_EVENT_TABLE()

SequenceViewDialog<uint8_t>::SequenceViewDialog(wxWindow* parent, wxWindowID id, const wxString& title)
: wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
, m_showAsBigEndian(false)
, m_latestIndex(-1)
{
	m_pHexViewCtrl = new HexViewCtrl(this, wxID_ANY);
	wxFont font;
	font = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
	font.SetPointSize(font.GetPointSize() + font.GetPointSize() / 2);

	assert(font.IsFixedWidth());
#if defined(__WXMSW__)
	font = wxFont(font.GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxS("Consolas"));
#else

#endif
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
	auto bsizer2 = new wxBoxSizer(wxHORIZONTAL);
	bsizer->Add(m_pToolbar, 0, wxEXPAND);

	bsizer2->Add(m_pHexViewCtrl, 1, wxEXPAND | wxALL, 5);
	m_pPropGrid = new wxPropertyGrid(this, wxID_ANY);
	m_pPropGrid->SetMinSize(FromDIP(wxSize(180, 180)));
	wxPGChoices choice;
	choice.Add(wxS("Little endian"), 0);
	choice.Add(wxS("Big endian"), 1);
	m_pPropGrid->Append(new wxEnumProperty(wxS("Endian"), wxS("endian"), choice, 0));
	m_pPropGrid->Append(new wxStringProperty(wxS("int8"), wxS("s8")));
	m_pPropGrid->Append(new wxStringProperty(wxS("int16"), wxS("s16")));
	m_pPropGrid->Append(new wxStringProperty(wxS("int32"), wxS("s32")));
	m_pPropGrid->Append(new wxStringProperty(wxS("int64"), wxS("s64")));
	m_pPropGrid->Append(new wxStringProperty(wxS("uint8"), wxS("u8")));
	m_pPropGrid->Append(new wxStringProperty(wxS("uint16"), wxS("u16")));
	m_pPropGrid->Append(new wxStringProperty(wxS("uint32"), wxS("u32")));
	m_pPropGrid->Append(new wxStringProperty(wxS("uint64"), wxS("u64")));
	m_pPropGrid->Append(new wxStringProperty(wxS("float"), wxS("f32")));
	m_pPropGrid->Append(new wxStringProperty(wxS("double"), wxS("f64")));
	bsizer2->Add(m_pPropGrid, 0, wxEXPAND | wxALL);
	bsizer->Add(bsizer2, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(bsizer);
}

void SequenceViewDialog<uint8_t>::SetValue(const ReadOnlySequence<uint8_t>& value)
{
	m_value = value;
	m_pHexViewCtrl->SetValue(value);
}

void SequenceViewDialog<uint8_t>::OnCommandSaveFile(wxCommandEvent& evt)
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

void SequenceViewDialog<uint8_t>::OnCommandOpenAsImage(wxCommandEvent& evt)
{
	
}

void SequenceViewDialog<uint8_t>::OnCommandHexViewSelectionChanged(wxCommandEvent& evt)
{
	auto index = evt.GetInt();
	m_latestIndex = index;
	ShowProp(index);
}

void SequenceViewDialog<uint8_t>::OnPropGridValueChanged(wxPropertyGridEvent& evt)
{
	if(evt.GetPropertyName() == wxS("endian"))
	{
		m_showAsBigEndian = evt.GetPropertyValue().GetInteger() == 1;
		if(m_latestIndex != -1)
		{
			ShowProp(m_latestIndex);
		}
	}
}

void SequenceViewDialog<unsigned char>::ShowProp(int index)
{
	auto& vector = m_value.Get();
	m_pPropGrid
	->GetPropertyByName(wxS("s8"))
	->SetValue(wxString::Format(wxS("%d"), (int)*(int8_t*)(vector.data() + index)));
	m_pPropGrid
	->GetPropertyByName(wxS("u8"))
	->SetValue(wxString::Format(wxS("%u"), (unsigned)*(uint8_t*)(vector.data() + index)));
	if(index + 1 < m_value.size())
	{
		int16_t s16 = *(int16_t*)(vector.data() + index);
		if(m_showAsBigEndian)
		{
			s16 = wxINT16_SWAP_ON_LE(s16);
		}
		else
		{
			s16 = wxINT16_SWAP_ON_BE(s16);
		}

		m_pPropGrid
		->GetPropertyByName(wxS("s16"))
		->SetValue(wxString::Format(wxS("%d"), (int)s16));

		m_pPropGrid
		->GetPropertyByName(wxS("u16"))
		->SetValue(wxString::Format(wxS("%u"), (unsigned)s16));
	}
	else
	{
		m_pPropGrid
		->GetPropertyByName(wxS("s16"))
		->SetValue(wxEmptyString);

		m_pPropGrid
		->GetPropertyByName(wxS("u16"))
		->SetValue(wxEmptyString);
	}

	if(index + 3 < m_value.size())
	{
		int32_t s32 = *(int32_t*)(vector.data() + index);
		if(m_showAsBigEndian)
		{
			s32 = wxINT32_SWAP_ON_LE(s32);
		}
		else
		{
			s32 = wxINT32_SWAP_ON_BE(s32);
		}

		m_pPropGrid
		->GetPropertyByName(wxS("s32"))
		->SetValue(wxString::Format(wxS("%d"), (int)s32));

		m_pPropGrid
		->GetPropertyByName(wxS("u32"))
		->SetValue(wxString::Format(wxS("%u"), (unsigned)s32));

		m_pPropGrid
		->GetPropertyByName(wxS("f32"))
		->SetValue(wxString::Format(wxS("%f"), reinterpret_cast<float&>(s32)));
	}
	else
	{
		m_pPropGrid
		->GetPropertyByName(wxS("s32"))
		->SetValue(wxEmptyString);

		m_pPropGrid
		->GetPropertyByName(wxS("u32"))
		->SetValue(wxEmptyString);
		
		m_pPropGrid
		->GetPropertyByName(wxS("f32"))
		->SetValue(wxEmptyString);
	}
	
	if(index + 7 < m_value.size())
	{
		int64_t s64 = *(int64_t*)(vector.data() + index);
		if(m_showAsBigEndian)
		{
			s64 = wxINT64_SWAP_ON_LE(s64);
		}
		else
		{
			s64 = wxINT64_SWAP_ON_BE(s64);
		}

		m_pPropGrid
		->GetPropertyByName(wxS("s64"))
		->SetValue(wxString::Format(wxS("%lld"), s64));

		m_pPropGrid
		->GetPropertyByName(wxS("u64"))
		->SetValue(wxString::Format(wxS("%llu"), (uint64_t)s64));

		m_pPropGrid
		->GetPropertyByName(wxS("f64"))
		->SetValue(wxString::Format(wxS("%f"), reinterpret_cast<double&>(s64)));
	}
	else
	{
		m_pPropGrid
		->GetPropertyByName(wxS("s64"))
		->SetValue(wxEmptyString);

		m_pPropGrid
		->GetPropertyByName(wxS("u64"))
		->SetValue(wxEmptyString);
		
		m_pPropGrid
		->GetPropertyByName(wxS("f64"))
		->SetValue(wxEmptyString);
	}
}
