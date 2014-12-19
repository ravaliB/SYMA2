# FightInBar, implementation library -  #

cmake_minimum_required(VERSION 2.8)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug") # Debug is the default build type on single-configuration toolchains.
endif()
project(FIGHTINBAR)

# Retrieving dependencies
if (NOT DEFINED mlv_runtime_FOUND OR NOT ${mlv_runtime_FOUND})
    get_filename_component(mlv_runtime_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../cmake/vc100 ABSOLUTE)
    find_package(mlv_runtime REQUIRED)
endif()
include_directories(${MLV_RUNTIME_INCLUDE_DIRS})

# Setting up the directories
if(NOT FIGHTINBAR_SOURCES_DIR)
    get_filename_component(FIGHTINBAR_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
endif()
if(NOT FIGHTINBAR_LIB_OUTPUT_DIR_DEBUG)
    get_filename_component(FIGHTINBAR_LIB_OUTPUT_DIR_DEBUG ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)
endif()
if(NOT FIGHTINBAR_LIB_OUTPUT_DIR_RELEASE)
    get_filename_component(FIGHTINBAR_LIB_OUTPUT_DIR_RELEASE ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)
endif()

# Listing the source files
set(LIBRARY_SOURCES
    ${FIGHTINBAR_SOURCES_DIR}/ImplementationLibrary.h
    ${FIGHTINBAR_SOURCES_DIR}/ImplementationLibrary.cpp
)

set(ACTION_SOURCES
    ${FIGHTINBAR_SOURCES_DIR}/setEntityBooleanAction.cpp
    ${FIGHTINBAR_SOURCES_DIR}/setEntityBooleanAction.h
    ${FIGHTINBAR_SOURCES_DIR}/IncrementAction.h
    ${FIGHTINBAR_SOURCES_DIR}/IncrementAction.cpp
)

set(COMPONENT_SOURCES
    ${FIGHTINBAR_SOURCES_DIR}/ContactComponent.h
    ${FIGHTINBAR_SOURCES_DIR}/ContactComponent.cpp
)

set(SOURCES ${LIBRARY_SOURCES} ${ACTION_SOURCES} ${CUSTOM_BT_NODE_SOURCES} ${COMPONENT_SOURCES} CMakeLists.txt)

# Project layout (for visual ide such as Visual Studio)
source_group(cmake FILES CMakeLists.txt)
source_group(sources FILES ${LIBRARY_SOURCES})
source_group(sources\\actions FILES ${ACTION_SOURCES})
source_group(sources\\components FILES ${COMPONENT_SOURCES})
source_group(sources\\nodes FILES ${CUSTOM_BT_NODE_SOURCES})

# compilation settings
if(WIN32)
    set(CMAKE_CXX_FLAGS "/EHsc /GR- /W3")
endif()

# configuring the target
add_library(FIGHTINBAR_implementation_lib SHARED ${SOURCES})
target_link_libraries(FIGHTINBAR_implementation_lib
    mlv_allocator
    mlv_utils
    mlv_behavior
    mlv_simulation
    mlv_simulation_components
    mlv_simulation_configuration)

set_target_properties(FIGHTINBAR_implementation_lib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${FIGHTINBAR_LIB_OUTPUT_DIR_DEBUG}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${FIGHTINBAR_LIB_OUTPUT_DIR_RELEASE}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${FIGHTINBAR_LIB_OUTPUT_DIR_DEBUG}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${FIGHTINBAR_LIB_OUTPUT_DIR_RELEASE}
    DEBUG_POSTFIX _vc100-mt-gd
    RELEASE_POSTFIX _vc100-mt
)
