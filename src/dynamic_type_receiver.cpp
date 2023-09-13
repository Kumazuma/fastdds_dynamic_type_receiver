#include "dynamic_type_receiver.h"
#include "enum.h"
#include "dynamic_data_to_std_data.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastrtps/xmlparser/XMLProfileManager.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicDataHelper.hpp>

IMPLEMENT_APP(DynamicTypeReceiverApp)

DynamicTypeReceiverApp::DynamicTypeReceiverApp()
: wxApp()
, m_currentState(StatusKind::READY)
, m_typeSubpport()
, m_pParticipant()
, m_pSubscriber()
, m_pReader()
, m_dynamicType() {}

bool DynamicTypeReceiverApp::OnInit()
{
	if (!wxApp::OnInit()) return false;

	eprosima::fastrtps::xmlparser::XMLProfileManager::loadDefaultXMLFile();
	m_reliability = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT.reliability().kind;
	m_ownership = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT.ownership().kind;
	if (!m_view.ShowMainForm()) return false;

	return true;
}

void DynamicTypeReceiverApp::SetXml(const wxString& text) { m_xmlText = text; }

void DynamicTypeReceiverApp::SetTypeName(const wxString& typeName) { m_typeName = typeName; }

ReturnCode DynamicTypeReceiverApp::RunGenerateTypeInfo()
{
	auto xmlUtf8Str = m_xmlText.ToStdString(wxConvUTF8);
	auto typeNameUtf8Str = m_typeName.ToStdString(wxConvUTF8);
	auto pFactory = eprosima::fastdds::dds::DomainParticipantFactory::get_instance();
	if (auto ret = pFactory->load_XML_profiles_string(xmlUtf8Str.c_str(), xmlUtf8Str.size());
		ret != ReturnCode_t::RETCODE_OK) { return ReturnCode::FAILED_TO_INIT_RESOURCE; }

	auto builder = eprosima::fastrtps::xmlparser::XMLProfileManager::getDynamicTypeByName(typeNameUtf8Str);
	if (builder == nullptr) { return ReturnCode::FAILED_TO_INIT_RESOURCE; }

	auto dynamicType = builder->build();
	auto pParticipant = pFactory->create_participant(m_domainId, eprosima::fastdds::dds::PARTICIPANT_QOS_DEFAULT);
	if (pParticipant == nullptr) { return ReturnCode::FAILED_TO_INIT_ENTITY; }

	auto typeSupport = eprosima::fastdds::dds::TypeSupport(dynamicType);
	if (typeSupport == nullptr)
	{
		pFactory->delete_participant(pParticipant);
		return ReturnCode::FAILED_TO_INIT_RESOURCE;
	}

	auto pSubscriber = pParticipant->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT);
	if (pSubscriber == nullptr)
	{
		pFactory->delete_participant(pParticipant);
		return ReturnCode::FAILED_TO_INIT_ENTITY;
	}

	auto registerTypeNameUtf8 = m_registeredTypeName.ToStdString(wxConvUTF8);
	pParticipant->register_type(typeSupport, registerTypeNameUtf8);

	auto topicNameUtf8 = m_topicName.ToStdString(wxConvUTF8);
	auto pTopic = pParticipant->create_topic(topicNameUtf8, registerTypeNameUtf8,
											eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
	if (pTopic == nullptr)
	{
		pParticipant->delete_subscriber(pSubscriber);
		pFactory->delete_participant(pParticipant);
		return ReturnCode::FAILED_TO_INIT_ENTITY;
	}

	eprosima::fastdds::dds::DataReaderQos readerQos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT;
	readerQos.reliability().kind = m_reliability;
	readerQos.ownership().kind = m_ownership;
	auto reader = pSubscriber->create_datareader(pTopic, readerQos, this,
												eprosima::fastdds::dds::StatusMask::data_available());
	if (reader == nullptr)
	{
		pParticipant->delete_topic(pTopic);
		pParticipant->delete_subscriber(pSubscriber);
		pFactory->delete_participant(pParticipant);
		return ReturnCode::FAILED_TO_INIT_ENTITY;
	}

	m_pParticipant = pParticipant;
	m_pSubscriber = pSubscriber;
	m_pReader = reader;
	m_typeSubpport = typeSupport;
	m_dynamicType = dynamicType;
	m_currentState = StatusKind::RECEIVING;
	if (!m_pParticipant->is_enabled()) { m_pParticipant->enable(); }

	return ReturnCode::SUCCEED;
}

int DynamicTypeReceiverApp::OnRun()
{
	int ret = wxApp::OnRun();
	auto dtbf = eprosima::fastdds::dds::DomainParticipantFactory::get_instance();
	if (m_pSubscriber != nullptr) { m_pParticipant->delete_subscriber(m_pSubscriber); }

	m_pSubscriber = nullptr;
	if (m_pParticipant != nullptr) { dtbf->delete_participant(m_pParticipant); }

	return ret;
}

std::vector<DataId> DynamicTypeReceiverApp::GetDataIdList() const
{
	wxCriticalSectionLocker locker(m_lockDataList);
	std::vector<DataId> list;
	list.reserve(m_dataList.size());
	for (auto& it : m_dataList) { list.emplace_back(std::get<0>(it)); }

	return list;
}

std::any DynamicTypeReceiverApp::GetData(const DataId& id) const
{
	wxCriticalSectionLocker locker(m_lockDataList);
	auto it = std::find_if(m_dataList.begin(), m_dataList.end(),
							[&id](const auto& it) { return std::get<0>(it) == id; });

	if (it == m_dataList.end()) return {};

	return std::get<1>(*it);
}

void DynamicTypeReceiverApp::SetDomainId(int domainId) { m_domainId = domainId; }

void DynamicTypeReceiverApp::SetTopicName(const wxString& topicName) { m_topicName = topicName; }

void DynamicTypeReceiverApp::SetRegisterTypeName(const wxString& registerTypeName)
{
	m_registeredTypeName = registerTypeName;
}

void DynamicTypeReceiverApp::on_data_available(eprosima::fastdds::dds::DataReader* reader)
{
	eprosima::fastrtps::types::DynamicData_ptr data(
		eprosima::fastrtps::types::DynamicDataFactory::get_instance()->create_data(m_dynamicType));
	eprosima::fastdds::dds::SampleInfo sampleInfo;

	while (reader->take_next_sample(data.get(), &sampleInfo) == ReturnCode_t::RETCODE_OK)
	{
		wxCriticalSectionLocker locker(m_lockDataList);
		std::any msg;
		if (convert_dynamic_data_to_std_data(m_dynamicType, *data, &msg))
		{
			Guid guid = iHandle2GUID(sampleInfo.publication_handle);
			m_dataList.emplace_back(DataId{guid, static_cast<uint64_t>(sampleInfo.source_timestamp.to_ns())},
									std::move(msg));
		}
	}

	CallAfter(&DynamicTypeReceiverApp::UpdateUi);
}

void DynamicTypeReceiverApp::UpdateUi() { m_view.Update(); }

eprosima::fastdds::dds::ReliabilityQosPolicyKind DynamicTypeReceiverApp::GetReliabilityKind() const
{
	return m_reliability;
}

eprosima::fastdds::dds::OwnershipQosPolicyKind DynamicTypeReceiverApp::GetOwnershipKind() const { return m_ownership; }

StatusKind DynamicTypeReceiverApp::GetStatus() const { return m_currentState; }

int DynamicTypeReceiverApp::GetDomainId() const { return m_domainId; }

const wxString& DynamicTypeReceiverApp::GetRegisteredTypeName() const { return m_registeredTypeName; }

const wxString& DynamicTypeReceiverApp::GetTopicName() const { return m_topicName; }

void DynamicTypeReceiverApp::SetReliabilityKind(eprosima::fastdds::dds::ReliabilityQosPolicyKind value)
{
	m_reliability = value;
}

void DynamicTypeReceiverApp::SetOwnershipKind(eprosima::fastdds::dds::OwnershipQosPolicyKind value)
{
	m_ownership = value;
}
