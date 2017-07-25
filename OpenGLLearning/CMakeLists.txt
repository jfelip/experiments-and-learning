cmake_minimum_required(VERSION 3.7)
project(OpenGLLearning)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/OpenGLLearning.cpp)

include_directories(include)

link_libraries(glfw GLEW GL)
add_executable(OpenGLLearning ${SOURCE_FILES})