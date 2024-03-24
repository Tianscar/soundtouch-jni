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

#include "com_tianscar_soundtouch_SoundTouch.h"

#include <cstddef>

#if defined(_WIN32) || defined(WIN32)
#pragma comment(lib, "SoundTouchDLL.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "SoundTouchDLL.h"

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch_SoundTouch_createInstance
        (JNIEnv *env, jclass clazz) {
    return (jlong) soundtouch_createInstance();
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_destroyInstance
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_destroyInstance((HANDLE) h);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setRate
        (JNIEnv *env, jclass clazz, jlong h, jfloat newRate) {
    soundtouch_setRate((HANDLE) h, newRate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setTempo
        (JNIEnv *env, jclass clazz, jlong h, jfloat newTempo) {
    soundtouch_setTempo((HANDLE) h, newTempo);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setRateChange
        (JNIEnv *env, jclass clazz, jlong h, jfloat newRate) {
    soundtouch_setRateChange((HANDLE) h, newRate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setTempoChange
        (JNIEnv *env, jclass clazz, jlong h, jfloat newTempo) {
    soundtouch_setTempoChange((HANDLE) h, newTempo);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setPitch
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitch((HANDLE) h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setPitchOctaves
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitchOctaves((HANDLE) h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setPitchSemiTones
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitchSemiTones((HANDLE) h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setChannels
        (JNIEnv *env, jclass clazz, jlong h, jlong numChannels) {
    soundtouch_setChannels((HANDLE) h, numChannels);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_setSampleRate
        (JNIEnv *env, jclass clazz, jlong h, jlong srate) {
    soundtouch_setSampleRate((HANDLE) h, srate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_flush
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_flush((HANDLE) h);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_putSamples
        (JNIEnv *env, jclass clazz, jlong h, jfloatArray samples, jint offset, jint numSamples) {
    jfloat *temp;
    temp = env->GetFloatArrayElements(samples, nullptr);
    soundtouch_putSamples((HANDLE) h, (jfloat *) ((size_t) temp + offset), numSamples);
    env->ReleaseFloatArrayElements(samples, temp, 0);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_putSamples_1i16
        (JNIEnv *env, jclass clazz, jlong h, jshortArray samples, jint offset, jint numSamples) {
    jshort *temp;
    temp = env->GetShortArrayElements(samples, nullptr);
    soundtouch_putSamples_i16((HANDLE) h, (jshort *) ((size_t) temp + offset), numSamples);
    env->ReleaseShortArrayElements(samples, temp, 0);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch_SoundTouch_clear
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_clear((HANDLE) h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch_SoundTouch_setSetting
        (JNIEnv *env, jclass clazz, jlong h, jint settingId, jint value) {
    return soundtouch_setSetting((HANDLE) h, settingId, value);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch_SoundTouch_getSetting
        (JNIEnv *env, jclass clazz, jlong h, jint settingId) {
    return soundtouch_getSetting((HANDLE) h, settingId);
}

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch_SoundTouch_numUnprocessedSamples
        (JNIEnv *env, jclass clazz, jlong h) {
    return soundtouch_numUnprocessedSamples((HANDLE) h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch_SoundTouch_receiveSamples
        (JNIEnv *env, jclass clazz, jlong h, jfloatArray outBuffer, jint offset, jint maxSamples) {
    jfloat *temp;
    jboolean isCopy;
    temp = env->GetFloatArrayElements(outBuffer, &isCopy);
    auto length = (jsize) soundtouch_receiveSamples((HANDLE) h, (jfloat *) ((size_t) temp + offset), maxSamples);
    if (isCopy) env->SetFloatArrayRegion(outBuffer, offset, length, temp);
    env->ReleaseFloatArrayElements(outBuffer, temp, 0);
    return length;
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch_SoundTouch_receiveSamples_1i16
        (JNIEnv *env, jclass clazz, jlong h, jshortArray outBuffer, jint offset, jint maxSamples) {
    jshort *temp;
    jboolean isCopy;
    temp = env->GetShortArrayElements(outBuffer, &isCopy);
    auto length = (jsize) soundtouch_receiveSamples_i16((HANDLE) h, (jshort *) ((size_t) temp + offset), maxSamples);
    if (isCopy) env->SetShortArrayRegion(outBuffer, offset, length, temp);
    env->ReleaseShortArrayElements(outBuffer, temp, 0);
    return length;
}

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch_SoundTouch_numSamples
        (JNIEnv *env, jclass clazz, jlong h) {
    return soundtouch_numSamples((HANDLE) h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch_SoundTouch_isEmpty
        (JNIEnv *env, jclass clazz, jlong h) {
    return soundtouch_isEmpty((HANDLE) h);
}

#ifdef __cplusplus
}
#endif