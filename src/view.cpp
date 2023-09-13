//
// Created by STK on 2023-09-08.
//

#include "view.h"
#include "dynamic_type_receiver.h"
#include "reader_creation_wizard_dialog.h"

View::View() = default;

bool View::ShowMainForm()
{
	m_pMainFrame = new(std::nothrow) MainForm(this);
	if (m_pMainFrame == nullptr) return false;

	m_pMainFrame->Show();
	return true;
}

void View::OnCommandRun(wxCommandEvent&)
{
	ReaderCreationWizard wizard(this, m_pMainFrame);
	auto finishHandler = [&wizard](wxWizardEvent& evt)
	{
		auto pXmlTxtCtrl = wxStaticCast(wxWindow::FindWindowById(ID_TXT_XML, &wizard), wxStyledTextCtrl);
		auto domainId = wizard.GetDomainId();
		auto xmlStr = pXmlTxtCtrl->GetText();
		auto topicName = wizard.GetTopicName();
		auto typeName = wizard.GetTypeName();
		auto topicTypeName = wizard.GetRegisterTypeName();
		auto ownership = wizard.GetOwnershipKind();
		auto reliability = wizard.GetReliabilityKind();
		if (topicName.IsEmpty())
		{
			wxMessageBox(wxS("Topic name is empty"), wxS("Receiver"), wxOK | wxCENTRE, &wizard);
			evt.Veto();
			return;
		}

		if (typeName.IsEmpty())
		{
			wxMessageBox(wxS("Type name is empty"), wxS("Receiver"), wxOK | wxCENTRE, &wizard);
			evt.Veto();
			return;
		}

		if (xmlStr.IsEmpty())
		{
			wxMessageBox(wxS("idl code is empty"), wxS("Receiver"), wxOK | wxCENTRE, &wizard);
			evt.Veto();
			return;
		}

		if (topicTypeName.IsEmpty()) topicTypeName = typeName;

		auto& app = wxGetApp();
		if (reliability)
		{
			app.SetReliabilityKind(*reliability);
		}

		if (ownership)
		{
			app.SetOwnershipKind(*ownership);
		}

		app.SetRegisterTypeName(topicTypeName);
		app.SetTopicName(topicName);
		app.SetDomainId(domainId);
		app.SetXml(xmlStr);
		app.SetTypeName(typeName);
		auto ret = app.RunGenerateTypeInfo();
		if (ret == ReturnCode::SUCCEED) return;

		evt.Veto();
		switch (ret)
		{
		case ReturnCode::FAILED_TO_INIT_RESOURCE:
			wxMessageBox(wxS("Failed to create type"), wxS("Receiver"), wxOK | wxCENTRE, &wizard);
			break;

		case ReturnCode::FAILED_TO_INIT_ENTITY:
			wxMessageBox(wxS("Failed to create entity"), wxS("Receiver"), wxOK | wxCENTRE, &wizard);
			break;
		}
	};

	wizard.Bind(wxEVT_WIZARD_FINISHED, finishHandler);
	if (!wizard.RunWizard(wizard.m_pages.front()))
	{
		return;
	}

	auto& app = wxGetApp();
	if(app.GetStatus() == StatusKind::RECEIVING)
	{
		SubscriptionConfig config{};
		config.topicName = app.GetTopicName();
		config.registeredTypeName = app.GetRegisteredTypeName();
		config.domainId = app.GetDomainId();
		config.ownership = app.GetOwnershipKind();
		config.reliability = app.GetReliabilityKind();
		m_pMainFrame->SetConfiguration(config);
		m_pMainFrame->SetAppState(true);
	}
}

void View::Update()
{
	auto& app = wxGetApp();
	m_pMainFrame->SetDataIdList(app.GetDataIdList());
}

void View::OnCommandDClickedListBox(wxCommandEvent&)
{
	auto it = m_pMainFrame->GetSelection();
	auto& app = wxGetApp();
	auto sample = app.GetData(it);
	m_pMainFrame->ShowData(sample);
}

BEGIN_EVENT_TABLE(View, wxEvtHandler)
	EVT_TOOL(ID_PLAY, View::OnCommandRun)
	EVT_LISTBOX_DCLICK(ID_LIST_DATA, View::OnCommandDClickedListBox)
END_EVENT_TABLE()
