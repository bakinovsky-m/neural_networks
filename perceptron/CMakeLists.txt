cmake_minimum_required(VERSION 3.13)

project(perceptron)

include_directories(${cxxopts_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(hdrs
  neuron.h
  layer.h
  network.h
  )

set(srcs
  src/main.cpp
  src/neuron.cpp
  src/layer.cpp
  src/network.cpp
  )

add_executable(${PROJECT_NAME} ${srcs} ${hdrs})

target_link_libraries(${PROJECT_NAME} cxxopts)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
