#pragma once

#include <fastrtps/types/DynamicType.h>
#include <fastrtps/types/DynamicTypePtr.h>
#include <any>

bool convert_dynamic_data_to_std_data(const eprosima::fastrtps::types::DynamicType_ptr& ddsDynamicType,
									const eprosima::fastrtps::types::DynamicData& src, std::any* dst);
