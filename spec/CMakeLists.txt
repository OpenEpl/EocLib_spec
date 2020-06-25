cmake_minimum_required(VERSION 3.8)

if(NOT DEFINED EOC_HOME)
	set(EOC_HOME $ENV{EOC_HOME})
endif()
include(${EOC_HOME}/EocBuildHelper.cmake)

project(EocSpec)
option(EocSpec_BUILD_TESTS "Build tests for EocSpec." OFF)
aux_source_directory(${EocSpec_SOURCE_DIR}/src/e/lib/spec EocSpec_DIR_SRCS_MAIN_MOUDLED)
add_library(EocSpec ${EocSpec_DIR_SRCS_MAIN_MOUDLED})
set_property(TARGET EocSpec PROPERTY CXX_STANDARD 17)
set_property(TARGET EocSpec PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_eoc_lib(EocSpec system EocSystem)
target_link_eoc_lib(EocSpec krnln EocKrnln)
target_include_directories(EocSpec PUBLIC ${EocSpec_SOURCE_DIR}/src)

if(EocSpec_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()