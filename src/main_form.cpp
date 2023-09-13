#include "main_form.h"
#include "dynamic_data_to_std_data.h"
#include "widget/prop.h"
#include "widget/reliability_kind_prop.h"
#include "widget/ownership_kind_prop.h"

MainForm::MainForm(wxEvtHandler* view)
: MainFormBase(nullptr, wxID_ANY)
, view_(view)
{
	m_tool1->SetNormalBitmap(wxBitmapBundle::FromSVGResource(wxS("ID_SVG_PLAY"), FromDIP(wxSize(24, 24))));
	m_tool2->SetNormalBitmap(wxBitmapBundle::FromSVGResource(wxS("ID_SVG_STOP"), FromDIP(wxSize(24, 24))));
	m_toolBar1->Realize();

	m_tool2->Enable(false);
	m_toolBar1->Realize();

	wxPGProperty* pProp;
	m_pConfigPropGrid = wxStaticCast(FindWindowById(ID_PROP_CONFIG, this), wxPropertyGridManager);
	m_pConfigPropGrid->Append(new wxPropertyCategory(wxS("DomainParticipant"), wxS("domain_participant")));

	pProp = m_pConfigPropGrid->Append(new wxUIntProperty(wxS("Domain ID"), wxS("domain_id")));
	pProp->SetAttribute(wxS("Min"), 0);
	pProp->SetAttribute(wxS("Max"), 233);
	pProp->SetHelpString(
		wxS("a domain id which DomainParticipant will be created in. it should be ranged in 0 ~ 233."));
	pProp->ChangeFlag(wxPG_PROP_READONLY, true);

	m_pConfigPropGrid->Append(new wxPropertyCategory(wxS("Topic Configuration")));

	pProp = m_pConfigPropGrid->Append(new wxStringProperty(wxS("Topic name"), wxS("topic_name")));
	pProp->SetHelpString(wxS("a name of topic what a DataReader subscribe in."));
	pProp->ChangeFlag(wxPG_PROP_READONLY, true);

	pProp = m_pConfigPropGrid->Append(new wxStringProperty(wxS("Type name for register"), wxS("type_name")));
	pProp->SetHelpString(
		wxS("a type name that uses registering to a DomainParticipant. if it is empty, use default type name."));
	pProp->ChangeFlag(wxPG_PROP_READONLY, true);

	m_pConfigPropGrid->Append(new wxPropertyCategory(wxS("DataReader QoS")));

	pProp = m_pConfigPropGrid->Append(new ReliabilityKindProperty(wxS("Reliability"), wxS("reliability")));
	pProp->ChangeFlag(wxPG_PROP_READONLY, true);

	pProp = m_pConfigPropGrid->Append(new OwnershipKindProperty(wxS("Ownership"), wxS("ownership")));
	pProp->ChangeFlag(wxPG_PROP_READONLY, true);
	std::vector<uint8_t> bigPayload;
	bigPayload.resize(1024);
	std::any dummy(ReadOnlyUint8Sequence(std::move(bigPayload)));
	ShowData(dummy);
}

bool MainForm::ProcessEvent(wxEvent& evt)
{
	bool ret = wxFrame::ProcessEvent(evt);
	if (ret && evt.GetSkipped())
	{
		ret = false;
	}

	if (!ret)
	{
		evt.Skip(false);
		ret = view_->ProcessEvent(evt);
	}

	return ret;
}

void MainForm::ShowData(const std::any& data)
{
	auto propGrid = wxStaticCast(FindWindowById(ID_PROP_DATA, this), wxPropertyGrid);
	propGrid->Clear();

	wxString name = wxS("value");
	if(data.type() == typeid(std::vector<std::pair<std::string, std::any>>))
	{
		name.Clear();
	}

	ShowAny(data, name, nullptr);
}

void MainForm::ShowStruct(const std::vector<std::pair<std::string, std::any>>& data, const wxString& name,
						wxPGProperty* parent)
{
	auto pProp = !name.IsEmpty() ? new wxStringProperty(name, name) : nullptr;
	if (pProp != nullptr)
	{
		if (parent == nullptr)
		{
			auto propGrid = wxStaticCast(FindWindowById(ID_PROP_DATA, this), wxPropertyGrid);
			propGrid->Append(pProp);
		}
		else
		{
			parent->AppendChild(pProp);
		}
	}

	for (auto& it : data)
	{
		ShowAny(it.second, wxString::FromUTF8(it.first), pProp);
	}
}

void MainForm::ShowAny(const std::any& data, const wxString& name, wxPGProperty* parent)
{
	wxPGProperty* pProperty = nullptr;
	if (data.type() == typeid(std::vector<std::pair<std::string, std::any>>))
	{
		ShowStruct(std::any_cast<const std::vector<std::pair<std::string, std::any>>&>(data), name, parent);
		return;
	}
	if (data.type() == typeid(std::string))
	{
		pProperty = new wxStringProperty(name, name, wxString::FromUTF8(std::any_cast<const std::string&>(data)));
	}
	else if (data.type() == typeid(std::wstring))
	{
		auto& str = std::any_cast<const std::wstring&>(data);
		pProperty = new wxStringProperty(name, name, str);
	}
	else if (data.type() == typeid(int64_t))
	{
		pProperty = new wxIntProperty(name, name);
		pProperty->SetValue(wxVariant(static_cast<wxLongLong>(std::any_cast<int64_t>(data))));
	}
	else if (data.type() == typeid(int32_t))
	{
		pProperty = new wxIntProperty(name, name, std::any_cast<int32_t>(data));
	}
	else if (data.type() == typeid(int16_t))
	{
		pProperty = new wxIntProperty(name, name, std::any_cast<int16_t>(data));
	}
	else if (data.type() == typeid(int8_t))
	{
		pProperty = new wxIntProperty(name, name, std::any_cast<int8_t>(data));
	}
	else if (data.type() == typeid(uint64_t))
	{
		pProperty = new wxUIntProperty(name, name, std::any_cast<uint64_t>(data));
	}
	else if (data.type() == typeid(uint32_t))
	{
		pProperty = new wxUIntProperty(name, name, std::any_cast<uint32_t>(data));
	}
	else if (data.type() == typeid(uint16_t))
	{
		pProperty = new wxUIntProperty(name, name, std::any_cast<uint16_t>(data));
	}
	else if (data.type() == typeid(uint8_t))
	{
		pProperty = new wxUIntProperty(name, name, std::any_cast<uint8_t>(data));
	}
	else if (data.type() == typeid(float))
	{
		pProperty = new wxFloatProperty(name, name, std::any_cast<float>(data));
	}
	else if (data.type() == typeid(double))
	{
		pProperty = new wxFloatProperty(name, name, std::any_cast<double>(data));
	}
	else if (data.type() == typeid(bool))
	{
		pProperty = new wxBoolProperty(name, name, std::any_cast<bool>(data));
		pProperty->ChangeFlag(wxPG_PROP_USE_CHECKBOX, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<std::string>))
	{
		pProperty = new ReadOnlySequenceProperty<std::string>(
			name, name, std::any_cast<ReadOnlySequence<std::string>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<int64_t>))
	{
		pProperty = new ReadOnlySequenceProperty<int64_t>(name, name, std::any_cast<ReadOnlySequence<int64_t>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<int32_t>))
	{
		pProperty = new ReadOnlySequenceProperty<int32_t>(name, name, std::any_cast<ReadOnlySequence<int32_t>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<int16_t>))
	{
		pProperty = new ReadOnlySequenceProperty<int16_t>(name, name, std::any_cast<ReadOnlySequence<int16_t>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<int8_t>))
	{
		pProperty = new ReadOnlySequenceProperty<int8_t>(name, name, std::any_cast<ReadOnlySequence<int8_t>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<uint64_t>))
	{
		pProperty = new ReadOnlySequenceProperty<uint64_t>(name, name, std::any_cast<ReadOnlySequence<uint64_t>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<uint32_t>))
	{
		pProperty = new ReadOnlySequenceProperty<uint32_t>(name, name, std::any_cast<ReadOnlySequence<uint32_t>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<uint8_t>))
	{
		pProperty = new ReadOnlySequenceProperty<uint8_t>(name, name, std::any_cast<ReadOnlySequence<uint8_t>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<float>))
	{
		pProperty = new ReadOnlySequenceProperty<float>(name, name, std::any_cast<ReadOnlySequence<float>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<double>))
	{
		pProperty = new ReadOnlySequenceProperty<double>(name, name, std::any_cast<ReadOnlySequence<double>>(data));
		pProperty->ChangeFlag(wxPG_PROP_ACTIVE_BTN, true);
	}
	else if (data.type() == typeid(ReadOnlySequence<std::any>))
	{
		// TODO: Object List View
		return;
	}
	else { return; }

	pProperty->ChangeFlag(wxPG_PROP_READONLY, true);
	if (parent == nullptr)
	{
		auto propGrid = wxStaticCast(FindWindowById(ID_PROP_DATA, this), wxPropertyGrid);
		propGrid->Append(pProperty);
	}
	else
	{
		parent->AppendChild(pProperty);
	}
}

static constexpr char HEX_TABLE[] = "0123456789ABCDEF";

void MainForm::SetDataIdList(std::vector<DataId>&& list)
{
	auto listBox = wxStaticCast(FindWindowById(ID_LIST_DATA, this), wxListBox);
	m_dataIdList = std::move(list);
	listBox->Freeze();
	auto listBoxItemCount = listBox->GetCount();
	auto listItemCount = m_dataIdList.size();
	if (listBoxItemCount > listItemCount)
	{
		for (uint32_t i = listItemCount; i < listBoxItemCount; ++i) { listBox->Delete(listItemCount); }
	}
	else if (listBoxItemCount < listItemCount)
	{
		for (uint32_t i = listBoxItemCount; i < listItemCount; ++i) { listBox->Append(wxS("")); }
	}

	uintptr_t idx = 0;
	for (auto& it : m_dataIdList)
	{
		auto& guid = it.GetGuid();
		auto seq = it.GetSeq();

		wxString guidStr;
		for (int i = 0; i < 12; ++i)
		{
			guidStr += HEX_TABLE[(guid.guidPrefix.value[i] >> 4) & 0xF];
			guidStr += HEX_TABLE[guid.guidPrefix.value[i] & 0xF];
			if (i % 4 == 3) guidStr += wxS("-");
		}

		guidStr += wxString::Format(wxS("%08X"), (uint32_t&)guid.entityId);
		auto text = wxString::Format(wxS("guid: %s seq: %llu"), guidStr, seq);
		listBox->SetString(idx, text);
		idx += 1;
	}

	listBox->Thaw();
}

DataId MainForm::GetSelection() const
{
	auto listBox = wxStaticCast(FindWindowById(ID_LIST_DATA, this), wxListBox);
	auto currentIdx = listBox->GetSelection();
	if (currentIdx == -1) return {};

	return m_dataIdList[currentIdx];
}

void MainForm::SetConfiguration(const SubscriptionConfig& config)
{
	m_pConfigPropGrid->GetProperty(wxS("domain_id"))->SetValue(wxVariant(config.domainId));
	m_pConfigPropGrid->GetProperty(wxS("topic_name"))->SetValue(wxVariant(config.topicName));
	m_pConfigPropGrid->GetProperty(wxS("type_name"))->SetValue(wxVariant(config.registeredTypeName));
	m_pConfigPropGrid->GetProperty(wxS("reliability"))->SetValue(
		WXVARIANT(static_cast<ReliabilityKind>(config.reliability)));
	m_pConfigPropGrid->GetProperty(wxS("ownership"))->SetValue(WXVARIANT(static_cast<OwnershipKind>(config.ownership)));
}

void MainForm::SetAppState(bool running)
{
	if(running)
	{
		m_toolBar1->EnableTool(ID_PLAY, false);
		m_toolBar1->EnableTool(ID_STOP, true);
	}
	else
	{
		m_toolBar1->EnableTool(ID_PLAY, true);
		m_toolBar1->EnableTool(ID_STOP, false);
	}
}

BEGIN_EVENT_TABLE(MainForm, wxFrame)
	END_EVENT_TABLE()
