cmake_minimum_required(VERSION 3.13)

project(gen_alg)

#include_directories(${cxxopts_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(hdrs
  )

set(srcs
  src/main.cpp
  )

add_executable(${PROJECT_NAME} ${srcs} ${hdrs})

#target_link_libraries(${PROJECT_NAME} cxxopts)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
