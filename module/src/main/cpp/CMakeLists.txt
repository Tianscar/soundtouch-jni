# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)

find_library(log-lib log)

set(SOUNDTOUCH ${CMAKE_SOURCE_DIR}/lib)
include_directories(${SOUNDTOUCH})
file(GLOB soundtouch_files ${SOUNDTOUCH}/*.cpp)
add_library(
        soundtouch
        SHARED
        ${soundtouch_files}
        jni/soundtouch-jni.cpp
)
target_link_libraries(
        soundtouch
        ${log-lib}
)