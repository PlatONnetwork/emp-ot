find_package(emp-tool)

find_path(EMP-OT_INCLUDE_DIR emp-ot/emp-ot.h)
find_library(EMP-OT_LIBRARY NAMES emp-ot)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(EMP-OT DEFAULT_MSG EMP-OT_INCLUDE_DIR)

if(EMP-OT_FOUND)
	set(EMP-OT_INCLUDE_DIRS ${EMP-OT_INCLUDE_DIR} ${EMP-TOOL_INCLUDE_DIRS})
	set(EMP-OT_LIBRARIES ${EMP-TOOL_LIBRARIES} ${EMP-OT_LIBRARY})
endif()
