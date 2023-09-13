#include "dynamic_data_to_std_data.h"
#include "enum.h"
#include "read_only_sequence.h"
#include <cstddef>
#include <fastrtps/types/TypeDescriptor.h>
#include <fastrtps/types/DynamicTypeMember.h>
#include <fastrtps/types/MemberDescriptor.h>
#include <fastrtps/types/DynamicDataPtr.h>
#include <fastrtps/types/DynamicData.h>

template <typename T>
struct SeqTrait {};

#define DEFINE_SEQ_TRAIT(_T, _E) \
template<> struct SeqTrait<_T> {   \
	static _T Get(const eprosima::fastrtps::types::DynamicData& src, uint32_t index) {\
	return src.get_##_E##_value(index);    \
	}\
};

DEFINE_SEQ_TRAIT(int64_t, int64)

DEFINE_SEQ_TRAIT(int32_t, int32)

DEFINE_SEQ_TRAIT(int16_t, int16)

DEFINE_SEQ_TRAIT(int8_t, int8)

DEFINE_SEQ_TRAIT(bool, bool)

DEFINE_SEQ_TRAIT(uint64_t, uint64)

DEFINE_SEQ_TRAIT(uint32_t, uint32)

DEFINE_SEQ_TRAIT(uint16_t, uint16)

DEFINE_SEQ_TRAIT(uint8_t, uint8)

DEFINE_SEQ_TRAIT(float, float32)

DEFINE_SEQ_TRAIT(double, float64)

DEFINE_SEQ_TRAIT(char, char8)

DEFINE_SEQ_TRAIT(wchar_t, char16)

DEFINE_SEQ_TRAIT(std::string, string)

DEFINE_SEQ_TRAIT(std::wstring, wstring)

template <typename T>
bool convert_string_sequence_to_std_vector(const eprosima::fastrtps::types::DynamicData& src,
											const eprosima::fastrtps::types::TypeDescriptor& memberTypeDesc,
											eprosima::fastrtps::types::MemberId memberId, std::any* const dst)
{
	using Trait = SeqTrait<T>;
	eprosima::fastrtps::types::DynamicData* sequence;
	src.get_complex_value(&sequence, memberId);
	const auto length = sequence->get_item_count();
	std::vector<T> list;
	list.reserve(length);
	for (uint32_t i = 0; i < length; ++i) { list.emplace_back(Trait::Get(*sequence, i)); }

	*dst = ReadOnlySequence<T>(std::move(list));
	return true;
}

bool convert_sequence_to_std_vector(const eprosima::fastrtps::types::DynamicData& src,
									const eprosima::fastrtps::types::TypeDescriptor& memberTypeDesc,
									eprosima::fastrtps::types::MemberId memberId, std::any* const dst)
{
	bool ret = true;
	switch (static_cast<TypeKind>(memberTypeDesc.get_element_type()->get_kind()))
	{
	case TypeKind::INT64:
		ret = convert_string_sequence_to_std_vector<int64_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::INT32:
		ret = convert_string_sequence_to_std_vector<int32_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::INT16:
		ret = convert_string_sequence_to_std_vector<int16_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::INT8:
		ret = convert_string_sequence_to_std_vector<int8_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::UINT64:
		ret = convert_string_sequence_to_std_vector<uint64_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::UINT32:
		ret = convert_string_sequence_to_std_vector<uint32_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::UINT16:
		ret = convert_string_sequence_to_std_vector<uint16_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::UINT8:
		ret = convert_string_sequence_to_std_vector<uint8_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::FLOAT64:
		ret = convert_string_sequence_to_std_vector<double>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::FLOAT32:
		ret = convert_string_sequence_to_std_vector<float>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::CHAR8:
		ret = convert_string_sequence_to_std_vector<char>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::CHAR16:
		ret = convert_string_sequence_to_std_vector<wchar_t>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::BOOLEAN:
		ret = convert_string_sequence_to_std_vector<bool>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::STRING8:
		ret = convert_string_sequence_to_std_vector<std::string>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::STRING16:
		ret = convert_string_sequence_to_std_vector<std::wstring>(src, memberTypeDesc, memberId, dst);
		break;
	case TypeKind::BYTE:
		// BYTE is not supported
		ret = false;
		break;
	case TypeKind::FLOAT128:
		// FLOAT128 is not supported
		ret = false;
		break;
	case TypeKind::ALIAS:
		ret = false;
		break;
	case TypeKind::ENUM:
		// TODO: Not implemented yet!
		ret = false;
		break;
	case TypeKind::BITMASK:
		// TODO: Not implemeneted yet!
		ret = false;
		break;
	case TypeKind::STRUCTURE:
		// TODO: Not implemented yet!
		break;
	case TypeKind::UNION:
		// TODO: Not implemented yet!
		ret = false;
		break;
	case TypeKind::BITSET:
		// BITSET is not supported
		ret = false;
		break;
	case TypeKind::SEQUENCE:
		// Nested sequence or array not support
		ret = false;
		break;
	case TypeKind::ARRAY:
		// Nested sequence or array not support
		ret = false;
		break;
	case TypeKind::MAP:
		ret = false;
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

bool convert_dynamic_struct_data_to_std_data(const eprosima::fastrtps::types::DynamicType_ptr& ddsDynamicType,
											const eprosima::fastrtps::types::DynamicData& src, std::any* const dst)
{
	std::vector<std::pair<std::string, std::any>> structure;
	const auto count = ddsDynamicType->get_members_count();
	std::map<eprosima::fastrtps::types::MemberId, eprosima::fastrtps::types::DynamicTypeMember*> members;
	ddsDynamicType->get_all_members(members);
	for (auto pair : members)
	{
		auto member = pair.second;
		auto memberDesc = member->get_descriptor();
		std::pair<std::string, std::any> field(memberDesc->get_name(), std::any{});
		auto& data = field.second;
		bool ret = true;
		switch (static_cast<TypeKind>(memberDesc->get_type()->get_kind()))
		{
		case TypeKind::INT64:
			data = src.get_int64_value(pair.first);
			break;

		case TypeKind::INT32:
			data = src.get_int32_value(pair.first);
			break;

		case TypeKind::INT16:
			data = src.get_int16_value(pair.first);
			break;

		case TypeKind::INT8:
			data = src.get_int8_value(pair.first);
			break;

		case TypeKind::UINT64:
			data = src.get_uint64_value(pair.first);
			break;

		case TypeKind::UINT32:
			data = src.get_uint32_value(pair.first);
			break;

		case TypeKind::UINT16:
			data = src.get_uint16_value(pair.first);
			break;

		case TypeKind::UINT8:
			data = src.get_uint8_value(pair.first);
			break;

		case TypeKind::BOOLEAN:
			data = src.get_bool_value(pair.first);
			break;

		case TypeKind::FLOAT32:
			data = src.get_float32_value(pair.first);
			break;

		case TypeKind::FLOAT64:
			data = src.get_float64_value(pair.first);
			break;

		case TypeKind::STRUCTURE:
		{
			eprosima::fastrtps::types::DynamicData* memberData = nullptr;
			src.get_complex_value(&memberData, pair.first);
			ret = convert_dynamic_data_to_std_data(memberDesc->get_type(), *memberData, &data);
			break;
		}

		case TypeKind::STRING8:
			data = src.get_string_value(pair.first);
			break;

		case TypeKind::NONE:
			ret = false;
			break;
		case TypeKind::BYTE:
			ret = false;
			break;
		case TypeKind::FLOAT128:
			ret = false;
			break;
		case TypeKind::CHAR8:
			// Single char type is not supported
			break;

		case TypeKind::CHAR16:
			// Single char16 type is not supported
			break;

		case TypeKind::STRING16:
			data = src.get_wstring_value(pair.first);
			break;

		case TypeKind::ENUM:
			// TODO: Not implemented yet
			ret = false;
			break;

		case TypeKind::BITMASK:
			// TODO: Not implemented yet
			ret = false;
			break;

		case TypeKind::UNION:
			// TODO: Not implemented yet
			ret = false;
			break;

		case TypeKind::BITSET:
			// Not Support
			break;

		case TypeKind::SEQUENCE:
		{
			auto typeDescriptor = memberDesc->get_type()->get_descriptor();
			ret = convert_sequence_to_std_vector(src, typeDescriptor, pair.first, &data);
			break;
		}

		case TypeKind::ARRAY:
		{
			auto typeDescriptor = memberDesc->get_type()->get_descriptor();
			ret = convert_sequence_to_std_vector(src, typeDescriptor, pair.first, &data);
			break;
		}

		case TypeKind::MAP:
			// Not Support
			ret = false;
			break;

		default:
			ret = false;
			break;
		}

		if (!ret) continue;

		structure.emplace_back(std::move(field));
	}

	*dst = std::move(structure);
	return true;
}

bool convert_dynamic_data_to_std_data(const eprosima::fastrtps::types::DynamicType_ptr& ddsDynamicType,
									const eprosima::fastrtps::types::DynamicData& src, std::any* const dst)
{
	assert(ddsDynamicType != nullptr && dst != nullptr);
	if (ddsDynamicType == nullptr || dst == nullptr) return false;

	bool ret = true;
	auto desc = ddsDynamicType->get_type_descriptor();
	switch (static_cast<TypeKind>(desc->get_kind()))
	{
	case TypeKind::ENUM:
		// TODO: Not implemented yet!
		ret = false;
	case TypeKind::BITMASK:
		// TODO: Not implemented yet!
		ret = false;
		break;
	case TypeKind::STRUCTURE:
		ret = convert_dynamic_struct_data_to_std_data(ddsDynamicType, src, dst);
		break;
	case TypeKind::UNION:
		// TODO: Not implemented yet!
		ret = false;
	case TypeKind::BITSET:
		// BITSET is not supported
		ret = false;
		break;
	case TypeKind::MAP:
		// MAP is not supported
		ret = false;
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}
