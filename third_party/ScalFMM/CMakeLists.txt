# third_party/ScalFMM/CMakeLists.txt
cmake_minimum_required(VERSION 3.24)

project(ScalFmm
  VERSION 1.0
  DESCRIPTION "Scalable Fast Multipole Method"
  LANGUAGES CXX)

# set the library source files
set(scalFmm_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/FAlgorithmTimers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/FLog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/FMemStats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/FSignalHandler.cpp)

# set the library header files
file(GLOB scalFmm_include ${CMAKE_CURRENT_SOURCE_DIR}/include/ScalFMM/*)
set(scalFmm_headers ${scalFmm_include})

# create the library
add_library(ScalFmm ${scalFmm_sources} ${scalFmm_headers})
target_include_directories(ScalFmm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)