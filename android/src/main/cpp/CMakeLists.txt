cmake_minimum_required(VERSION 3.22.1)

project(soundtouchjni C CXX)

file(GLOB SoundTouchFiles ${CMAKE_SOURCE_DIR}/SoundTouch/*.cpp)

add_library(SoundTouchDLL SHARED ${SoundTouchFiles})

# SoundTouch
include_directories(../../../../core/src/main/cpp/include)

# Android log lib
find_library(log-lib log)

add_library(soundtouchjni SHARED ../../../../core/src/main/cpp/soundtouchjni.cpp ../../../../core/src/main/cpp/bpmdetectorjni.cpp)

target_link_libraries(soundtouchjni ${log-lib} SoundTouchDLL)