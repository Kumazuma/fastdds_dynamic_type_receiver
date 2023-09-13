#pragma once

#include <wx/wx.h>
#include <wx/propgrid/props.h>
#include <fastdds/dds/core/policy/QosPolicies.hpp>
#include <optional>

using OwnershipKind = std::optional<eprosima::fastdds::dds::OwnershipQosPolicyKind>;

class OwnershipKindProperty : public wxPGProperty
{
public:
	OwnershipKindProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
						const OwnershipKind& value = std::nullopt);
	void OnSetValue() override;
	wxString ValueToString(wxVariant& value, int argFlags) const override;
	bool StringToValue(wxVariant& variant, const wxString& text, int argFlags) const override;
	const wxPGEditor* DoGetEditorClass() const override { return wxPGEditor_Choice; }
	bool IntToValue(wxVariant& variant, int number, int argFlags = 0) const override;
	int GetChoiceSelection() const override;
};

WX_PG_DECLARE_VARIANT_DATA(OwnershipKind);
