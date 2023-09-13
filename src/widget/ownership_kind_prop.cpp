#include "ownership_kind_prop.h"

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(OwnershipKind)

OwnershipKindProperty::OwnershipKindProperty(const wxString& label, const wxString& name, const OwnershipKind& value)
: wxPGProperty(label, name)
{
	m_value << value;
	m_choices.Add(wxS(""), -1);
	m_choices.Add(wxS("SHARED"), 0);
	m_choices.Add(wxS("EXCLUSIVE"), 1);
}

void OwnershipKindProperty::OnSetValue() {}

wxString OwnershipKindProperty::ValueToString(wxVariant& value, int argFlags) const
{
	auto& kind = OwnershipKindRefFromVariant(value);
	if (kind) return m_choices.GetLabel(*kind + 1);

	return wxS("");
}

bool OwnershipKindProperty::StringToValue(wxVariant& variant, const wxString& text, int argFlags) const
{
	if (text == wxS("SHARED"))
	{
		variant << static_cast<OwnershipKind>(eprosima::fastdds::dds::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS);
	}
	else if (text == wxS("RELIABLE"))
	{
		variant << static_cast<OwnershipKind>(eprosima::fastdds::dds::OwnershipQosPolicyKind::EXCLUSIVE_OWNERSHIP_QOS);
	}
	else if (text.IsEmpty()) { variant << std::nullopt; }
	else { return false; }

	return true;
}

bool OwnershipKindProperty::IntToValue(wxVariant& variant, int number, int argFlags) const
{
	if (number == -1) { variant << std::nullopt; }
	else if (number == 0)
	{
		variant << static_cast<OwnershipKind>(eprosima::fastdds::dds::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS);
	}
	else if (number == 1)
	{
		variant << static_cast<OwnershipKind>(eprosima::fastdds::dds::OwnershipQosPolicyKind::EXCLUSIVE_OWNERSHIP_QOS);
	}
	else { return false; }

	return true;
}

int OwnershipKindProperty::GetChoiceSelection() const
{
	auto& kind = OwnershipKindRefFromVariant(m_value);
	if (!kind) return 0;

	return *kind + 1;
}
