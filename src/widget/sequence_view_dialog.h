#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include "../read_only_sequence.h"
#include "hex_view_ctrl.h"

template <typename T>
class SequenceViewDialog : public wxDialog
{
public:
	SequenceViewDialog(wxWindow* parent, wxWindowID id, const wxString& title);
	void SetValue(const ReadOnlySequence<T>& value);

private:
	wxGrid* m_pGridCtrl;
	ReadOnlySequence<T> m_value;
};

template <typename T>
class ReadOnlySequenceModel : public wxGridTableBase
{
public:
	explicit ReadOnlySequenceModel(const ReadOnlySequence<T>& sequence) :
	m_sequence(sequence) { }

	bool IsEmptyCell(int WXUNUSED(row), int WXUNUSED(col) ) override { return false; }

	wxString GetTypeName(int WXUNUSED(row), int WXUNUSED(col)) override
	{
		return wxString::Format("%s:40", wxGRID_VALUE_STRING);
	}

	int GetNumberRows() override { return static_cast<int>(m_sequence.size()); }
	int GetNumberCols() override { return 1; }
	wxString GetValue(int row, int col) override;
	wxString GetRowLabelValue(int row) override { return wxString::Format(wxS("%d"), row); };
	wxString GetColLabelValue(int col) override { return wxS("item"); }
	void SetValue(int row, int col, const wxString& value) override {};

	bool CanGetValueAs(int WXUNUSED(row), int col, const wxString& typeName) override
	{
		return typeName == wxGRID_VALUE_STRING;
	}

private:
	ReadOnlySequence<T> m_sequence;
};

template <typename T>
wxString ReadOnlySequenceModel<T>::GetValue(int row, int col) { return std::to_string(m_sequence[row]); }

template <>
inline wxString ReadOnlySequenceModel<std::string>::GetValue(int row, int col)
{
	return wxString::FromUTF8(m_sequence[row]);
}

template <typename T>
SequenceViewDialog<T>::SequenceViewDialog(wxWindow* parent, wxWindowID id, const wxString& title) :
wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	m_pGridCtrl = new wxGrid(this, wxID_ANY);
	auto bsizer = new wxBoxSizer(wxHORIZONTAL);
	bsizer->Add(m_pGridCtrl, 1, wxEXPAND | wxALL, 5);
	SetSizer(bsizer);
}

template <typename T>
void SequenceViewDialog<T>::SetValue(const ReadOnlySequence<T>& value)
{
	m_value = value;
	m_pGridCtrl->AssignTable(new ReadOnlySequenceModel(value));
}

// Specialization for octet sequence
template <>
class SequenceViewDialog<uint8_t> : public wxDialog
{
	DECLARE_EVENT_TABLE();

	enum class TextCtrlIndex;
public:
	SequenceViewDialog(wxWindow* parent, wxWindowID id, const wxString& title);
	void SetValue(const ReadOnlySequence<uint8_t>& value);

protected:
	void OnCommandSaveFile(wxCommandEvent& evt);
	void OnCommandOpenAsImage(wxCommandEvent& evt);
	void OnCommandHexViewSelectionChanged(wxCommandEvent& evt);
	void OnPropGridValueChanged(wxPropertyGridEvent& evt);

private:
	void ShowProp(int index);

private:
	wxToolBar* m_pToolbar;
	HexViewCtrl* m_pHexViewCtrl;
	ReadOnlySequence<uint8_t> m_value;
	wxPropertyGrid* m_pPropGrid;
	bool m_showAsBigEndian;
	int m_latestIndex;
};
