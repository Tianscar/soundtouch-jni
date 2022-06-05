cmake_minimum_required(VERSION 3.22)
project(soundtouchjni C)

# C standard
set(CMAKE_C_STANDARD 11)

# JNI
find_package(JNI)
include_directories(${JNI_INCLUDE_DIRS})

# SoundTouch
include_directories(../../../../core/src/main/cpp/include)
if (WIN32)
    find_library(SoundTouch SoundTouchDLL ../../../src/main/resources/windows-x86
            ../../../src/main/resources/windows-x86_64 ../../../src/main/resources/windows-aarch64)
elseif(APPLE)
    find_library(SoundTouch SoundTouchDLL ../../../src/main/resources/macos-x86_64
            ../../../src/main/resources/macos-arm64)
elseif(UNIX)
    find_library(SoundTouch SoundTouchDLL ../../../src/main/resources/linux-amd64
            ../../../src/main/resources/linux-i386)
endif()

# Source code
add_library(soundtouchjni SHARED ../../../../core/src/main/cpp/soundtouchjni.c ../../../../core/src/main/cpp/bpmdetectorjni.c)

# Dynamic links
target_link_libraries(soundtouchjni ${SoundTouch})