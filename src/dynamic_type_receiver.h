#pragma once

#include <wx/wx.h>
#include <fastdds/dds/core/policy/QosPolicies.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastrtps/types/DynamicType.h>
#include <fastrtps/types/DynamicTypePtr.h>
#include "view.h"
#include "utils.h"
#include "data_id.h"

enum class ReturnCode
{
	SUCCEED,
	FAILED_TO_INIT_RESOURCE,
	FAILED_TO_INIT_ENTITY
};

enum class StatusKind
{
	READY,
	RECEIVING
};

class DynamicTypeReceiverApp : public wxApp, public eprosima::fastdds::dds::DataReaderListener
{
public:
	DynamicTypeReceiverApp();
	bool OnInit() override;
	int OnRun() override;
	void SetDomainId(int domainId);
	void SetXml(const wxString& text);
	void SetTypeName(const wxString& typeName);
	void SetIncludeDirectory(const wxString& includeDirectory);
	void SetTopicName(const wxString& topicName);
	void SetRegisterTypeName(const wxString& registerTypeName);
	ReturnCode RunGenerateTypeInfo();
	std::vector<DataId> GetDataIdList() const;
	std::any GetData(const DataId& key) const;
	int GetDomainId() const;
	const wxString& GetTopicName() const;
	const wxString& GetRegisteredTypeName() const;
	eprosima::fastdds::dds::ReliabilityQosPolicyKind GetReliabilityKind() const;
	eprosima::fastdds::dds::OwnershipQosPolicyKind GetOwnershipKind() const;
	StatusKind GetStatus() const;
	void SetReliabilityKind(eprosima::fastdds::dds::ReliabilityQosPolicyKind value);
	void SetOwnershipKind(eprosima::fastdds::dds::OwnershipQosPolicyKind value);

protected:
	void on_data_available(eprosima::fastdds::dds::DataReader* aReader) override;
	void UpdateUi();

private:
	StatusKind m_currentState;
	int m_domainId;
	wxString m_xmlText;
	wxString m_typeName;
	wxString m_topicName;
	wxString m_registeredTypeName;
	View m_view;

	eprosima::fastdds::dds::TypeSupport m_typeSubpport;
	eprosima::fastdds::dds::DomainParticipant* m_pParticipant;
	eprosima::fastdds::dds::Subscriber* m_pSubscriber;

	eprosima::fastdds::dds::DataReader* m_pReader;
	eprosima::fastrtps::types::DynamicType_ptr m_dynamicType;
	mutable wxCriticalSection m_lockDataList;
	std::vector<std::tuple<DataId, std::any>> m_dataList;
	eprosima::fastdds::dds::ReliabilityQosPolicyKind m_reliability;
	eprosima::fastdds::dds::OwnershipQosPolicyKind m_ownership;
};

wxDECLARE_APP(DynamicTypeReceiverApp);
