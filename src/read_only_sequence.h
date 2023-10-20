#pragma once

#include <any>
#include <memory>
#include <wx/propgrid/propgriddefs.h>
#include <wx/variant.h>

template <typename T>
class ReadOnlySequence
{
public:
	ReadOnlySequence()
	: m_length(0) {};

	ReadOnlySequence(std::vector<T>&& sequence)
	: m_vector(new std::vector(std::move(sequence))) { m_length = m_vector->size(); }

	ReadOnlySequence(ReadOnlySequence&& rhs) noexcept
	: m_vector(std::move(rhs.m_vector))
	, m_length(rhs.m_length) { rhs.m_length = 0; }

	ReadOnlySequence(const ReadOnlySequence& rhs)
	: m_vector(rhs.m_vector)
	, m_length(rhs.m_length) { }

	const T& operator[](size_t index) const
	{
		assert(index < m_length);
		return m_vector->at(index);
	}

	void operator =(const ReadOnlySequence& rhs)
	{
		if (rhs.m_vector != m_vector)
		{
			m_vector = rhs.m_vector;
			m_length = rhs.m_length;
		}
	}

	void operator =(ReadOnlySequence&& rhs) noexcept
	{
		if (rhs.m_vector != m_vector)
		{
			m_vector = std::move(rhs.m_vector);
			m_length = rhs.m_length;
			rhs.m_length = 0;
		}
	}

	size_t size() const { return m_length; }
	const std::vector<T>& Get() const { return *m_vector; }
private:
	std::shared_ptr<std::vector<T>> m_vector;
	size_t m_length;
};

using ReadOnlyInt8Sequence = ReadOnlySequence<int8_t>;
using ReadOnlyInt16Sequence = ReadOnlySequence<int16_t>;
using ReadOnlyInt32Sequence = ReadOnlySequence<int32_t>;
using ReadOnlyInt64Sequence = ReadOnlySequence<int64_t>;
using ReadOnlyUint8Sequence = ReadOnlySequence<uint8_t>;
using ReadOnlyUint16Sequence = ReadOnlySequence<uint16_t>;
using ReadOnlyUint32Sequence = ReadOnlySequence<uint32_t>;
using ReadOnlyUint64Sequence = ReadOnlySequence<uint64_t>;
using ReadOnlyFloatSequence = ReadOnlySequence<float>;
using ReadOnlyDoubleSequence = ReadOnlySequence<double>;
using ReadOnlyStringSequence = ReadOnlySequence<std::string>;
using ReadOnlyWstringSequence = ReadOnlySequence<std::wstring>;
using ReadOnlyAnySequence = ReadOnlySequence<std::any>;

WX_PG_DECLARE_VARIANT_DATA(ReadOnlyInt8Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyInt16Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyInt32Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyInt64Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyUint8Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyUint16Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyUint32Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyUint64Sequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyFloatSequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyDoubleSequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyStringSequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyWstringSequence)
WX_PG_DECLARE_VARIANT_DATA(ReadOnlyAnySequence)
