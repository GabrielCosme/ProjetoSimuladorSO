cmake_minimum_required(VERSION 3.22.1)
project(projeto_simulador_so)

# Set a default C++ compile flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 20)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)
include(cmake/Linter.cmake)

add_library(${PROJECT_NAME}_lib
  src/lib/interface.cpp
  src/lib/memory.cpp
  src/lib/process_manager.cpp
  src/lib/process.cpp
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/cfg
)

add_executable(${PROJECT_NAME}_bin
  src/bin/main.cpp
)

target_link_libraries(${PROJECT_NAME}_bin
  ${PROJECT_NAME}_lib
)
