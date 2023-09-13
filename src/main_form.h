#pragma once

#include "gui.h"
#include "utils.h"
#include "data_id.h"
#include <fastdds/dds/core/policy/QosPolicies.hpp>
#include <any>

struct SubscriptionConfig
{
	int domainId;
	wxString topicName;
	wxString registeredTypeName;
	eprosima::fastdds::dds::ReliabilityQosPolicyKind reliability;
	eprosima::fastdds::dds::OwnershipQosPolicyKind ownership;
};

class MainForm : public MainFormBase
{
	DECLARE_EVENT_TABLE();

public:
	explicit MainForm(wxEvtHandler* view);
	bool ProcessEvent(wxEvent& evt) override;
	void ShowData(const std::any& data);
	void SetDataIdList(std::vector<DataId>&& list);
	DataId GetSelection() const;
	void SetConfiguration(const SubscriptionConfig& config);
	void SetAppState(bool running);
private:
	void ShowAny(const std::any& data, const wxString& name, wxPGProperty* parent);
	void ShowStruct(const std::vector<std::pair<std::string, std::any>>& data, const wxString& name,
					wxPGProperty* parent);

private:
	wxEvtHandler* view_;
	std::vector<DataId> m_dataIdList;
	wxPropertyGridManager* m_pConfigPropGrid;
};
