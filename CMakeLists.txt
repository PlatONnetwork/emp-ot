cmake_minimum_required (VERSION 3.0)
if(POLICY CMP0054)
	 cmake_policy(SET CMP0054 NEW)
endif()
project (emp-ot)
set(NAME "emp-ot")
set (CMAKE_CXX_STANDARD 11) 


SET(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(EMP_TOOL_CMAKE ${CURRENT_DIR}/../emp-tool/cmake)

# setup directory where we should look for cmake files
list(APPEND CMAKE_MODULE_PATH "${EMP_TOOL_CMAKE}")
list(APPEND CMAKE_MODULE_PATH ${CURRENT_DIR}/cmake)

#find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${EMP_TOOL_CMAKE}/common.cmake)
include(${EMP_TOOL_CMAKE}/source_of_randomness.cmake)
include(${EMP_TOOL_CMAKE}/threading.cmake)


###### includes here
# emp-tool include
include_directories(${CURRENT_DIR}/../emp-tool)
include_directories(${CURRENT_DIR}/../emp-tool/include)
include_directories(${CURRENT_DIR}/../emp-ot)
include_directories(${CURRENT_DIR})


###### libraries here
if(WIN32)
	link_libraries(Ws2_32)
endif()

link_relic_or_miracl() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
link_protobuf() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake

# Requirements for lattice
if (LATTICEOT)
    message(STATUS "Building with support for lattice OT")
    find_package(Eigen3 3.3 REQUIRED NO_MODULE)
    find_package(Boost 1.58 REQUIRED COMPONENTS random)
    include_directories(${EMP-TOOL_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
else (LATTICEOT)
    include_directories(${EMP-TOOL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
endif (LATTICEOT)


# Installation
install(FILES cmake/emp-ot-config.cmake DESTINATION cmake/)
install(DIRECTORY emp-ot DESTINATION include/)

# Test cases
macro (emp_ot_test _name)
	message(STATUS "emp ot test project name: ${_name}")

	aux_source_directory(./testutils EXE_SRC_LIST)
    add_executable(emp_ot_${_name} ./test/${_name}.cpp)
    target_link_libraries(emp_ot_${_name} emp-tool)
    
    #IF(NOT WIN32 OR OT_NP_USE_RELIC_WIN)
    #    target_link_libraries(emp_ot_${_name} ${GMP_LIBRARIES})
    #ENDIF()

	SET_TARGET_PROPERTIES(emp_ot_${_name} PROPERTIES FOLDER "emp-toolkit-test")
endmacro()


emp_ot_test(shot)
emp_ot_test(mot)
emp_ot_test(deltaot)
emp_ot_test(print_precomputation_table)
emp_ot_test(ideal)


if (LATTICEOT)
  set(INCLUDE_LATTICE_OT ON)
  emp_ot_test(lattice)
  target_link_libraries(lattice Eigen3::Eigen)
  SET_TARGET_PROPERTIES(lattice PROPERTIES COMPILE_DEFINITIONS "EMP_USE_RANDOM_DEVICE")
else(LATTICEOT)
  set(INCLUDE_LATTICE_OT OFF)

endif(LATTICEOT)

configure_file(${CURRENT_DIR}/emp-ot/latticeInclude.h.in ${CURRENT_DIR}/emp-ot/latticeInclude.h @ONLY)
