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

#include "com_tianscar_soundtouch4j_SoundTouch.h"
#include "SoundTouchDLL.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_createInstance
        (JNIEnv *env, jclass clazz) {
    return soundtouch_createInstance();
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_destroyInstance
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_destroyInstance(h);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setRate
        (JNIEnv *env, jclass clazz, jlong h, jfloat newRate) {
    soundtouch_setRate(h, newRate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setTempo
        (JNIEnv *env, jclass clazz, jlong h, jfloat newTempo) {
    soundtouch_setTempo(h, newTempo);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setRateChange
        (JNIEnv *env, jclass clazz, jlong h, jfloat newRate) {
    soundtouch_setRateChange(h, newRate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setTempoChange
        (JNIEnv *env, jclass clazz, jlong h, jfloat newTempo) {
    soundtouch_setTempoChange(h, newTempo);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setPitch
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitch(h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setPitchOctaves
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitchOctaves(h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setPitchSemiTones
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitchSemiTones(h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setChannels
        (JNIEnv *env, jclass clazz, jlong h, jlong numChannels) {
    soundtouch_setChannels(h, numChannels);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setSampleRate
        (JNIEnv *env, jclass clazz, jlong h, jlong srate) {
    soundtouch_setSampleRate(h, srate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_flush
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_flush(h);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_putSamples
        (JNIEnv *env, jclass clazz, jlong h, jfloatArray samples, jint numSamples) {
    jfloat *temp;
    temp = (*env)->GetFloatArrayElements(env, samples, NULL);
    soundtouch_putSamples(h, temp, numSamples);
    (*env)->ReleaseFloatArrayElements(env, samples, temp, 0);
    (*env)->DeleteLocalRef(env, samples);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_putSamples_1i16
        (JNIEnv *env, jclass clazz, jlong h, jshortArray samples, jint numSamples) {
    jshort *temp;
    temp = (*env)->GetShortArrayElements(env, samples, NULL);
    soundtouch_putSamples_i16(h, temp, numSamples);
    (*env)->ReleaseShortArrayElements(env, samples, temp, 0);
    (*env)->DeleteLocalRef(env, samples);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_clear
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_clear(h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setSetting
        (JNIEnv *env, jclass clazz, jlong h, jint settingId, jint value) {
    soundtouch_setSetting(h, settingId, value);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_getSetting
        (JNIEnv *env, jclass clazz, jlong h, jint settingId) {
    return soundtouch_getSetting(h, settingId);
}

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_numUnprocessedSamples
        (JNIEnv *env, jclass clazz, jlong h) {
    return soundtouch_numUnprocessedSamples(h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_receiveSamples
        (JNIEnv *env, jclass clazz, jlong h, jfloatArray outBuffer, jint maxSamples) {
    jfloat *temp;
    jsize length = (jsize) soundtouch_receiveSamples(h, temp, maxSamples);
    (*env)->SetFloatArrayRegion(env, outBuffer, 0, length, temp);
    return length;
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_receiveSamples_1i16
        (JNIEnv *env, jclass clazz, jlong h, jshortArray outBuffer, jint maxSamples) {
    jshort *temp;
    jsize length = (jsize) soundtouch_receiveSamples_i16(h, temp, maxSamples);
    (*env)->SetShortArrayRegion(env, outBuffer, 0, length, temp);
    return length;
}

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_numSamples
        (JNIEnv *env, jclass clazz, jlong h) {
    return soundtouch_numSamples(h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_isEmpty
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_isEmpty(h);
}

#ifdef __cplusplus
}
#endif