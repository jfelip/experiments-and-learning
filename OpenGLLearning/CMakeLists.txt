cmake_minimum_required(VERSION 3.7)
project(OpenGLLearning)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/Transform.h
        include/Material.hpp
        include/GLGeometry.h
        include/CVector3.hpp
        include/CReal.hpp
        include/CQuaternion.hpp
        include/Common.h
        include/CGLShader.hpp
        include/CGLLight.hpp
        include/CGLCamera.hpp
        include/CDualQuaternion.hpp
        include/primitives/GLPrimitives.h
        include/primitives/GLPointCloud.h
        include/primitives/CSolidSphere.h
        include/primitives/CSolidCylinder.h
        include/primitives/CSolidCone.h
        include/primitives/CSolidCapsule.h
        include/primitives/CSolidBox.h
        include/primitives/CSolidArrow.h
        include/primitives/CReferenceFrame.h
        include/primitives/CParticleSystem.h
        include/primitives/CParticleSystemSpheres.h
        src/OpenGLLearning.cpp)

include_directories(include ../PBD/include ../../HCD /usr/include/eigen3)

link_libraries(glfw GLEW GL)

add_executable(OpenGLLearning ${SOURCE_FILES})
