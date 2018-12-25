cmake_minimum_required (VERSION 3.0)
project (emp-ot)
set(NAME "emp-ot")
set (CMAKE_CXX_STANDARD 11) 

SET(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(EMP_TOOL_CMAKE ${CURRENT_DIR}/../emp-tool/cmake)

message(STATUS "emp-ot current directory: ${CURRENT_DIR}")

# will set on the upper cmakelist
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

#find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${EMP_TOOL_CMAKE}/common.cmake)
include(${EMP_TOOL_CMAKE}/source_of_randomness.cmake)
include(${EMP_TOOL_CMAKE}/threading.cmake)

# setup directory where we should look for cmake files
list(APPEND CMAKE_MODULE_PATH "${EMP_TOOL_CMAKE}")
list(APPEND CMAKE_MODULE_PATH ${CURRENT_DIR}/cmake)
message(STATUS "cmake path: ${CMAKE_MODULE_PATH}")
find_package(OpenSSL)

# Requirements for lattice
if (LATTICEOT)
  message(STATUS "Building with support for lattice OT")
  find_package(Eigen3 3.3 REQUIRED NO_MODULE)
  find_package(Boost 1.58 REQUIRED COMPONENTS random)
  include_directories(${EMP-TOOL_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
else (LATTICEOT)
  include_directories(${EMP-TOOL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
endif (LATTICEOT)


if(WIN32)
    link_directories(${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    # for windows socket
    link_libraries(Ws2_32)
else()
    link_directories(${CMAKE_BINARY_DIR}/lib)
endif()

# protobuf dependencies
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	if (WIN32)
		SET(LIB_PROTOBUF libprotobufd)
	else()
		SET(LIB_PROTOBUF protobufd)
	endif()
else()
	if (WIN32)
		SET(LIB_PROTOBUF libprotobuf)
	else()
		SET(LIB_PROTOBUF protobuf)
	endif()
endif()


if(OT_NP_USE_MIRACL)
	message(STATUS "Use MIRACL base libraries !")
	ADD_DEFINITIONS(-DOT_NP_USE_MIRACL=1)
	include_directories(../miracl/include)
	link_libraries(miracl)
else()
	message(STATUS "Use RELIC base libraries !")
	find_package(relic) # compile relic should refer to emp-readme github repository (https://github.com/emp-toolkit/emp-readme)
	#set(RELIC_LIBRARIES relic_s)
	include_directories(${RELIC_INCLUDE_DIR})
	message(STATUS "RELIC_INCLUDE_DIR: ${RELIC_INCLUDE_DIR}")
endif()

if (NOT WIN32)
	find_package(GMP)
	include_directories(${GMP_INCLUDE_DIR})
	link_libraries(${GMP_LIBRARIES})
endif()


# emp-tool include
include_directories(${CURRENT_DIR}/../emp-tool)
include_directories(${CURRENT_DIR}/../emp-tool/include)
include_directories(${CURRENT_DIR})

# Installation
install(FILES cmake/emp-ot-config.cmake DESTINATION cmake/)
install(DIRECTORY emp-ot DESTINATION include/)

message(STATUS "pb: ${LIB_PROTOBUF}")
# Test cases
macro (emp_ot_test _name)
	message(STATUS "emp ot test: ${_name}")

	aux_source_directory(./testutils EXE_SRC_LIST)
    add_executable(emp_ot_${_name} ./test/${_name}.cpp)

	if(NOT OT_NP_USE_MIRACL)
		if(WIN32)
			message(FATAL "Not support relic on Windows !")
		endif()

		target_link_libraries(emp_ot_${_name} ${OPENSSL_LIBRARIES} ${RELIC_LIBRARIES} ${GMP_LIBRARIES} ${LIB_PROTOBUF} emp-tool)
	else()
		if(NOT WIN32)
			target_link_libraries(emp_ot_${_name} ${OPENSSL_LIBRARIES} ${GMP_LIBRARIES} ${LIB_PROTOBUF} emp-tool)
		else()
			target_link_libraries(emp_ot_${_name} ${OPENSSL_LIBRARIES} ${LIB_PROTOBUF} emp-tool_static)
		endif()
	endif()
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
