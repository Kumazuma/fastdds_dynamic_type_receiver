#pragma once

#include <wx/wx.h>
#include <wx/propgrid/props.h>
#include "../read_only_sequence.h"
#include "sequence_view_dialog.h"
#include <wx/propgrid/propgrid.h>

template <typename T>
class ReadOnlySequenceProperty : public wxEditorDialogProperty
{
public:
	ReadOnlySequenceProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
							const ReadOnlySequence<T>& seq = {});
	wxString ValueToString(wxVariant& value, int argFlags) const override;
	bool StringToValue(wxVariant& variant, const wxString& text, int argFlags) const override { return false; }
	const wxPGEditor* DoGetEditorClass() const override { return wxPGEditor_TextCtrlAndButton; }

protected:
	bool DisplayEditorDialog(wxPropertyGrid* pg, wxVariant& value) override;
};

template <typename T>
ReadOnlySequenceProperty<T>::ReadOnlySequenceProperty(const wxString& label, const wxString& name,
													const ReadOnlySequence<T>& seq)
: wxEditorDialogProperty(label, name) { m_value << seq; }

template <typename T>
wxString ReadOnlySequenceProperty<T>::ValueToString(wxVariant& value, int argFlags) const
{
	ReadOnlySequence<T> seq;
	seq << m_value;
	return wxString::Format(wxS("{Sequence Length: %zu}"), seq.size());
}

template <typename T>
bool ReadOnlySequenceProperty<T>::DisplayEditorDialog(wxPropertyGrid* pg, wxVariant& value)
{
	SequenceViewDialog<T> dialog(pg, wxID_ANY, m_label);
	ReadOnlySequence<T> seq;
	seq << m_value;
	dialog.SetValue(seq);
	dialog.ShowModal();
	return false;
}
