cmake_minimum_required(VERSION 3.6)

project(gl-test)

set(CMAKE_CXX_STANDARD 11)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

add_executable(gl-test main.cpp glad.c shader_s.h CWorld.h CWorld.cpp CCamera.h CCar.h CCar.cpp)

add_dependencies(gl-test OpenGL::OpenGL)

add_custom_command(
TARGET gl-test POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory              
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:gl-test>/shaders
COMMENT "Copying shaders" VERBATIM
)

add_custom_command(
TARGET gl-test POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory              
    ${CMAKE_SOURCE_DIR}/objects $<TARGET_FILE_DIR:gl-test>/objects
COMMENT "Copying objects" VERBATIM
)

target_link_libraries(gl-test OpenGL::OpenGL ${GLUT_LIBRARIES} ${OPENGL_glu_LIBRARY} glfw assimp)
target_include_directories(gl-test PRIVATE include ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIRS} glfw)

