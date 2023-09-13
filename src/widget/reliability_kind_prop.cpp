#include "reliability_kind_prop.h"

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(ReliabilityKind)

ReliabilityKindProperty::ReliabilityKindProperty(const wxString& label, const wxString& name,
												const ReliabilityKind& value):
wxPGProperty(label, name)
{
	m_value << value;
	m_choices.Add(wxS(""), 0);
	m_choices.Add(wxS("BEST_EFFORT"), 1);
	m_choices.Add(wxS("RELIABLE"), 2);
}

void ReliabilityKindProperty::OnSetValue() {}

wxString ReliabilityKindProperty::ValueToString(wxVariant& value, int argFlags) const
{
	auto& kind = ReliabilityKindRefFromVariant(value);
	if (kind) return m_choices.GetLabel(*kind);

	return wxS("");
}

bool ReliabilityKindProperty::StringToValue(wxVariant& variant, const wxString& text, int argFlags) const
{
	if (text == wxS("BEST_EFFORT"))
	{
		variant << static_cast<ReliabilityKind>(
			eprosima::fastdds::dds::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS);
	}
	else if (text == wxS("RELIABLE"))
	{
		variant << static_cast<ReliabilityKind>(
			eprosima::fastdds::dds::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS);
	}
	else if (text.IsEmpty()) { variant << std::nullopt; }
	else { return false; }

	return true;
}

bool ReliabilityKindProperty::IntToValue(wxVariant& variant, int number, int argFlags) const
{
	if (number == 0) { variant << std::nullopt; }
	else if (number == 1)
	{
		variant << static_cast<ReliabilityKind>(
			eprosima::fastdds::dds::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS);
	}
	else if (number == 2)
	{
		variant << static_cast<ReliabilityKind>(
			eprosima::fastdds::dds::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS);
	}
	else { return false; }

	return true;
}

int ReliabilityKindProperty::GetChoiceSelection() const
{
	auto& kind = ReliabilityKindRefFromVariant(m_value);
	if (!kind) return 0;

	return *kind;
}
