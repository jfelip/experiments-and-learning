cmake_minimum_required(VERSION 3.7)
project(PBD)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES
        include/Common.h
        include/CVector3.hpp
        include/physics/CParticle.hpp
        include/physics/CParticleSystem.h
        include/physics/CPositionBasedDynamics.h
        include/physics/CConstraint.hpp
        include/physics/CWorld.h
        src/main.cpp)

add_executable(PBD ${SOURCE_FILES})
