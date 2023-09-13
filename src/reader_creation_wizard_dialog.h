#pragma once

#include <wx/wx.h>
#include <wx/wizard.h>
#include <fastdds/dds/core/policy/QosPolicies.hpp>
#include <optional>
#include "gui.h"

class MyWizard1;

class ReaderCreationWizard : public MyWizard1
{
	wxDECLARE_EVENT_TABLE();

public:
	ReaderCreationWizard(wxEvtHandler* parentView, wxWindow* parent);
	bool ProcessEvent(wxEvent& event) override;
	wxString GetTopicName() const;
	wxString GetRegisterTypeName() const;
	wxString GetTypeName() const;
	int GetDomainId() const;
	std::optional<eprosima::fastdds::dds::ReliabilityQosPolicyKind> GetReliabilityKind() const;
	std::optional<eprosima::fastdds::dds::OwnershipQosPolicyKind> GetOwnershipKind() const;

protected:
	void OnCommandOpen(wxCommandEvent& event);

private:
	wxEvtHandler* m_pView;
	wxPropertyGridManager* m_pConfigPropGrid;

};
