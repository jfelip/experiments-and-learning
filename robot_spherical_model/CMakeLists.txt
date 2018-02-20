cmake_minimum_required(VERSION 2.8.9)
project(robot_spherical_model)
set(CMAKE_BUILD_TYPE Release)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


message(STATUS "SOURCE_DIRECTORY: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "BIN_DIRECTORY: ${CMAKE_CURRENT_BINARY_DIR}")

#Library generation
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include" "/opt/ros/$ENV{ROS_DISTRO}/include")
add_library(robot_spherical_model SHARED include/robot_spherical_model/RobotSphericalModel.h src/RobotSphericalModel.cpp)