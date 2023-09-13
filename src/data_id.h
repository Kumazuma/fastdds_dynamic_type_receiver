#pragma once
#include "utils.h"

struct DataId
{
public:
	DataId()
	: m_guid()
	, m_timestamp() {}

	DataId(const Guid& guid, uint64_t seq);
	const Guid& GetGuid() const { return m_guid; }
	uint64_t GetSeq() const { return m_timestamp; }
	bool operator ==(const DataId& rhs) const;

private:
	Guid m_guid;
	uint64_t m_timestamp;
};

inline DataId::DataId(const Guid& guid, uint64_t seq)
: m_guid(guid)
, m_timestamp(seq) {}

inline bool DataId::operator==(const DataId& rhs) const
{
	return m_guid == rhs.m_guid && m_timestamp == rhs.m_timestamp;
}
