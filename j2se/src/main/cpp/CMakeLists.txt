cmake_minimum_required(VERSION 3.22)
project(soundtouchjni)

# JNI
find_package(JNI)
include_directories(${JNI_INCLUDE_DIRS})

# SoundTouch
include_directories(../../../../core/src/main/cpp/include)
if (${TARGET_ARCH} MATCHES "X64")
    if(${TARGET_SYSTEM} MATCHES "Windows")
        find_library(SoundTouch SoundTouchDLL.dll ../../../src/main/resources/windows-x86_64)
    elseif(${TARGET_SYSTEM} MATCHES "Mac")
        find_library(SoundTouch libSoundTouchDLL.dylib ../../../src/main/resources/macos-x86_64)
    elseif(${TARGET_SYSTEM} MATCHES "Linux")
        find_library(SoundTouch libSoundTouchDLL.so ../../../src/main/resources/linux-amd64)
    endif()
elseif(${TARGET_ARCH} MATCHES "X86")
    if(${TARGET_SYSTEM} MATCHES "Windows")
        find_library(SoundTouch SoundTouchDLL.dll ../../../src/main/resources/windows-x86)
    elseif(${TARGET_SYSTEM} MATCHES "Linux")
        find_library(SoundTouch libSoundTouchDLL.so ../../../src/main/resources/linux-i386)
    endif()
elseif(${TARGET_ARCH} MATCHES "ARM64")
    if(${TARGET_SYSTEM} MATCHES "Windows")
        find_library(SoundTouch SoundTouchDLL.dll ../../../src/main/resources/windows-aarch64)
    elseif(${TARGET_SYSTEM} MATCHES "Mac")
        find_library(SoundTouch libSoundTouchDLL.dylib ../../../src/main/resources/macos-arm64)
    endif()
elseif(${TARGET_ARCH} MATCHES "ARM")
endif()

# Source code
add_library(soundtouchjni SHARED ../../../../core/src/main/cpp/soundtouchjni.cpp ../../../../core/src/main/cpp/bpmdetectorjni.cpp)

# MSVC link static libs
if(${TARGET_SYSTEM} MATCHES "Windows")
    if (${TARGET_ARCH} MATCHES "X64")
        target_link_directories(soundtouchjni PUBLIC libs/windows-x86_64)
    elseif(${TARGET_ARCH} MATCHES "X86")
        target_link_directories(soundtouchjni PUBLIC libs/windows-x86)
    elseif(${TARGET_ARCH} MATCHES "ARM64")
        target_link_directories(soundtouchjni PUBLIC libs/windows-aarch64)
    endif()
endif()

# Dynamic link library
target_link_libraries(soundtouchjni PUBLIC ${SoundTouch})