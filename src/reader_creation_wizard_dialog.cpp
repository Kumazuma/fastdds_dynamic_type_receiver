#include "reader_creation_wizard_dialog.h"
#include "gui.h"
#include "widget/reliability_kind_prop.h"
#include "widget/ownership_kind_prop.h"
#include <fastrtps/rtps/participant/RTPSParticipant.h>

ReaderCreationWizard::ReaderCreationWizard(wxEvtHandler* parentView, wxWindow* parent)
: MyWizard1(parent, wxID_ANY, wxS("Create DataReader"))
, m_pView(parentView)
{
	auto pStyledTextCtrl = wxStaticCast(wxWindow::FindWindowById(ID_TXT_XML, this), wxStyledTextCtrl);
	pStyledTextCtrl->SetLexer(wxSTC_LEX_XML);
	pStyledTextCtrl->StyleSetForeground(wxSTC_H_NUMBER, wxColour(0, 150, 0));
	pStyledTextCtrl->StyleSetForeground(wxSTC_H_SINGLESTRING, wxColour(150, 0, 0));
	pStyledTextCtrl->StyleSetForeground(wxSTC_H_DOUBLESTRING, wxColour(150, 0, 0));
	pStyledTextCtrl->StyleSetForeground(wxSTC_H_TAG, wxColour(0, 0, 150));
	pStyledTextCtrl->StyleSetForeground(wxSTC_H_ATTRIBUTE, wxColour(0, 150, 0));
	pStyledTextCtrl->StyleSetForeground(wxSTC_H_COMMENT, wxColour(150, 150, 150));
	m_pConfigPropGrid = wxStaticCast(wxWindow::FindWindowById(ID_PROP_CONFIG, this), wxPropertyGridManager);
	m_pConfigPropGrid->Append(new wxPropertyCategory(wxS("DomainParticipant"), wxS("domain_participant")));
	auto pDomainIdProp =
		m_pConfigPropGrid->Append(new wxUIntProperty(wxS("Domain ID"), wxS("domain_id")));
	pDomainIdProp->SetAttribute(wxS("Min"), 0);
	pDomainIdProp->SetAttribute(wxS("Max"), 233);
	pDomainIdProp->SetHelpString(
		wxS("a domain id which DomainParticipant will be created in. it should be ranged in 0 ~ 233."));
	m_pConfigPropGrid->Append(new wxPropertyCategory(wxS("Topic Configuration")));
	m_pConfigPropGrid->Append(new wxStringProperty(wxS("Topic name"), wxS("topic_name")))->SetHelpString(
		wxS("a name of topic what a DataReader subscribe in."));
	m_pConfigPropGrid->Append(new wxStringProperty(wxS("Type name for register"), wxS("type_name")))->SetHelpString(
		wxS("a type name that uses registering to a DomainParticipant. if it is empty, use default type name."));
	m_pConfigPropGrid->Append(new wxPropertyCategory(wxS("DataReader QoS")));
	m_pConfigPropGrid->Append(new ReliabilityKindProperty(wxS("Reliability"), wxS("reliability")));
	m_pConfigPropGrid->Append(new OwnershipKindProperty(wxS("Ownership"), wxS("ownership")));

}

bool ReaderCreationWizard::ProcessEvent(wxEvent& event)
{
	bool processed = wxEvtHandler::ProcessEvent(event);
	if (!processed || event.GetSkipped())
	{
		processed = m_pView->ProcessEvent(event);
	}

	return processed;
}

void ReaderCreationWizard::OnCommandOpen(wxCommandEvent& event)
{
	auto pStyledTextCtrl = wxStaticCast(wxWindow::FindWindowById(ID_TXT_XML, this), wxStyledTextCtrl);
	wxFileDialog fileDialog(this, wxS("Choose idl file"), {}, {}, wxS("idl file|*.idl"));
	if (fileDialog.ShowModal() != wxID_OK)
	{
		return;
	}

	auto path = fileDialog.GetPath();
	wxFile file;
	if (!file.Open(path))
	{
		return;
	}

	wxString text;
	if (!file.ReadAll(&text, wxConvUTF8))
	{
		return;
	}

	pStyledTextCtrl->SetText(text);
}

wxString ReaderCreationWizard::GetTopicName() const
{
	return m_pConfigPropGrid->GetProperty(wxS("topic_name"))->GetValueAsString();
}

wxString ReaderCreationWizard::GetRegisterTypeName() const
{
	return m_pConfigPropGrid->GetProperty(wxS("type_name"))->GetValueAsString();
}

wxString ReaderCreationWizard::GetTypeName() const
{
	auto targetTypeNameCtrl = wxStaticCast(wxWindow::FindWindowById(ID_TYPE_NAME, this), wxTextCtrl);
	return targetTypeNameCtrl->GetValue();
}

int ReaderCreationWizard::GetDomainId() const
{
	return m_pConfigPropGrid->GetProperty(wxS("domain_id"))->GetValue().GetULongLong().GetValue();
}

std::optional<eprosima::fastdds::dds::ReliabilityQosPolicyKind> ReaderCreationWizard::GetReliabilityKind() const
{
	return ReliabilityKindRefFromVariant(m_pConfigPropGrid->GetProperty(wxS("reliability"))->GetValue());
}

std::optional<eprosima::fastdds::dds::OwnershipQosPolicyKind> ReaderCreationWizard::GetOwnershipKind() const
{
	return OwnershipKindRefFromVariant(m_pConfigPropGrid->GetProperty(wxS("ownership"))->GetValue());
}

BEGIN_EVENT_TABLE(ReaderCreationWizard, wxWizard)
	EVT_BUTTON(wxID_OPEN, ReaderCreationWizard::OnCommandOpen)
END_EVENT_TABLE()
