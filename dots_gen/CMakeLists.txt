cmake_minimum_required(VERSION 3.13)

project(dots_gen)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${cxxopts_INCLUDE_DIRS})
#message("${cxxopts_INCLUDE_DIRS} asdasdasdasd")

set(hdrs
  gl_funcs.h
  structs.h
  )

add_executable(${PROJECT_NAME} main.cpp ${hdrs})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} cxxopts)
