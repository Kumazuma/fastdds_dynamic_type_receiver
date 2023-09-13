cmake_minimum_required(VERSION 3.26)
project(fastdds_dynamic_type_receiver)

set(CMAKE_CXX_STANDARD 20)
include(${WX_LIB_PATH}/cmake/wxWidgets/wxWidgetsConfig.cmake)
find_package(fastrtps REQUIRED)
find_package(fastcdr REQUIRED)

get_target_property(WX_INCLUDE_DIRECTORIES wx::base INTERFACE_INCLUDE_DIRECTORIES)
find_file(WX_MANIFEST_PMV2_XML wx/msw/wx_dpi_aware_pmv2.manifest PATHS ${WX_INCLUDE_DIRECTORIES} REQUIRED)

add_executable(fastdds_dynamic_type_receiver WIN32
	src/event.cpp
	src/read_only_sequence.cpp
	src/widget/hex_view_ctrl.cpp
	src/widget/ownership_kind_prop.cpp
	src/widget/reliability_kind_prop.cpp
	src/widget/sequence_view_dialog.cpp
	src/widget/prop.cpp
	src/view.cpp
	src/gui.cpp
	src/main_form.cpp
	src/dynamic_data_to_std_data.cpp
	src/reader_creation_wizard_dialog.cpp
	src/dynamic_type_receiver.cpp
	src/resource.rc
	${WX_MANIFEST_PMV2_XML})

target_link_libraries(fastdds_dynamic_type_receiver PRIVATE wx::base wx::core wx::aui wx::propgrid wx::stc fastrtps fastcdr)

target_compile_definitions(fastdds_dynamic_type_receiver PRIVATE wxUSE_NO_MANIFEST)
set_target_properties(fastdds_dynamic_type_receiver PROPERTIES VS_DPI_AWARE "PerMonitor")

