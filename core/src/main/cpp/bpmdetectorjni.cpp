// MIT License
//
// Copyright (c) 2021-2022 Tianscar
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "com_tianscar_soundtouch4j_BPMDetector.h"
#include "SoundTouchDLL.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch4j_BPMDetector_createInstance
        (JNIEnv *env, jclass clazz, jint numChannels, jint sampleRate) {
    return (jlong) bpm_createInstance(numChannels, sampleRate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_BPMDetector_destroyInstance
(JNIEnv *env, jclass clazz, jlong h) {
    bpm_destroyInstance((HANDLE) h);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_BPMDetector_putSamples
(JNIEnv *env, jclass clazz, jlong h, jfloatArray samples, jint numSamples) {
    jfloat *temp;
    temp = env->GetFloatArrayElements(samples, nullptr);
    bpm_putSamples((HANDLE) h, temp, numSamples);
    env->ReleaseFloatArrayElements(samples, temp, 0);
    env->DeleteLocalRef(samples);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_BPMDetector_putSamples_1i16
(JNIEnv *env, jclass clazz, jlong h, jshortArray samples, jint numSamples) {
    jshort *temp;
    temp = env->GetShortArrayElements(samples, nullptr);
    bpm_putSamples_i16((HANDLE) h, temp, numSamples);
    env->ReleaseShortArrayElements(samples, temp, 0);
    env->DeleteLocalRef(samples);
}

JNIEXPORT jfloat JNICALL Java_com_tianscar_soundtouch4j_BPMDetector_getBpm
        (JNIEnv *env, jclass clazz, jlong h) {
    return bpm_getBpm((HANDLE) h);
}

#ifdef __cplusplus
}
#endif