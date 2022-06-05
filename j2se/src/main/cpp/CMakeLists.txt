cmake_minimum_required(VERSION 3.22)
project(soundtouchjni C)

# C standard
set(CMAKE_C_STANDARD 11)

# JNI
find_package(JNI)
include_directories(${JNI_INCLUDE_DIRS})

# SoundTouch
include_directories(${CMAKE_SOURCE_DIR}/../../../../core/src/main/cpp/include)
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "X64" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        find_library(SoundTouch SoundTouchDLL ${CMAKE_SOURCE_DIR}/../../../src/main/resources/windows-x86_64)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Mac")
        find_library(SoundTouch SoundTouchDLL ${CMAKE_SOURCE_DIR}/../../../src/main/resources/macos-x86_64)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        find_library(SoundTouch SoundTouchDLL ${CMAKE_SOURCE_DIR}/../../../src/main/resources/linux-amd64)
    endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "X86")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        find_library(SoundTouch SoundTouchDLL ${CMAKE_SOURCE_DIR}/../../../src/main/resources/windows-x86)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        find_library(SoundTouch SoundTouchDLL ${CMAKE_SOURCE_DIR}/../../../src/main/resources/linux-i386)
    endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM64" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "AARCH64")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        find_library(SoundTouch SoundTouchDLL ${CMAKE_SOURCE_DIR}/../../../src/main/resources/windows-aarch64)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Mac")
        find_library(SoundTouch SoundTouchDLL ${CMAKE_SOURCE_DIR}/../../../src/main/resources/macos-arm64)
    endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM")
endif()

# Source code
add_library(soundtouchjni SHARED ${CMAKE_SOURCE_DIR}/../../../../core/src/main/cpp/soundtouchjni.c ${CMAKE_SOURCE_DIR}/../../../../core/src/main/cpp/bpmdetectorjni.c)

# Dynamic links
target_link_libraries(soundtouchjni ${SoundTouch})