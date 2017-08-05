cmake_minimum_required(VERSION 3.7)
project(VectorTests)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp)

include_directories(include ../OpenGLLearning/include)

add_executable(VectorTests ${SOURCE_FILES})
