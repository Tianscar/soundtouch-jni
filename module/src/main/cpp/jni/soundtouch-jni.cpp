/*
 * MIT License
 *
 * Copyright (c) 2021 Tianscar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <jni.h>
#include <android/log.h>
#include <stdexcept>
#include <string>

using namespace std;

#include "com_tianscar_soundtouch4android_SoundTouch.h"
#include "SoundTouch.h"

using namespace soundtouch;

// JNI methods

extern "C"
JNIEXPORT jlong JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_newInstance(JNIEnv *env, jclass clazz)
{
	return (jlong)(new SoundTouch());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_deleteInstance(JNIEnv *env, jobject thiz, jlong handle)
{
	auto *ptr = (SoundTouch*)handle;
	delete ptr;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getChannels(JNIEnv *env, jobject thiz, jlong handle) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->numChannels();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setChannels(JNIEnv *env, jobject thiz, jlong handle,
                                                  jint channels) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setChannels(channels);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setSampleRate(JNIEnv *env, jobject thiz, jlong handle,
                                                    jint srate) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setSampleRate(srate);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getInputOutputSampleRatio(JNIEnv *env, jobject thiz,
                                                                jlong handle) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->getInputOutputSampleRatio();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getUnprocessedSampleCount(JNIEnv *env, jobject thiz,
                                                                 jlong handle) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->numUnprocessedSamples();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getSampleCount(JNIEnv *env, jobject thiz, jlong handle) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->numSamples();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_adjustAmountOfSamples(JNIEnv *env, jobject thiz, jlong handle,
                                                            jint samples_count) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->adjustAmountOfSamples(samples_count);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_putSamples(JNIEnv *env, jobject thiz, jlong handle,
                                                 jshortArray samples, jint sample_count) {
    auto *ptr = (SoundTouch*)handle;
    jshort *temp;
    jboolean *isCopy = JNI_FALSE;
    temp = env->GetShortArrayElements(samples, isCopy);
    ptr->putSamples(temp, sample_count);
    env->ReleaseShortArrayElements(samples, temp, 0);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_receiveSamples1(JNIEnv *env, jobject thiz, jlong handle,
                                                      jshortArray sample_buffer, jint max_samples) {
    auto *ptr = (SoundTouch*)handle;
    jshort *temp;
    jboolean *isCopy = JNI_FALSE;
    temp = env->GetShortArrayElements(sample_buffer, isCopy);
    ptr->putSamples(temp, max_samples);
    jint result = ptr->receiveSamples(temp, max_samples);
    env->ReleaseShortArrayElements(sample_buffer, temp, 0);
    return result;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_receiveSamples2(JNIEnv *env, jobject thiz, jlong handle,
                                                      jint max_samples) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->receiveSamples(max_samples);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_flush(JNIEnv *env, jobject thiz, jlong handle) {
    auto *ptr = (SoundTouch*)handle;
    ptr->flush();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_clear(JNIEnv *env, jobject thiz, jlong handle) {
    auto *ptr = (SoundTouch*)handle;
    ptr->clear();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_isEmpty(JNIEnv *env, jobject thiz, jlong handle) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->isEmpty();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setRate(JNIEnv *env, jobject thiz, jlong handle,
                                              jdouble rate) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setRate(rate);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setTempo(JNIEnv *env, jobject thiz, jlong handle,
                                               jdouble tempo) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setTempo(tempo);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setRateChange(JNIEnv *env, jobject thiz, jlong handle,
                                                    jdouble rate) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setRateChange(rate);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setTempoChange(JNIEnv *env, jobject thiz, jlong handle,
                                                     jdouble tempo) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setTempoChange(tempo);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitch(JNIEnv *env, jobject thiz, jlong handle,
                                               jdouble pitch) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setPitch(pitch);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitchOctaves(JNIEnv *env, jobject thiz, jlong handle,
                                                      jdouble pitch) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setPitchOctaves(pitch);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitchSemiTones1(JNIEnv *env, jobject thiz, jlong handle,
                                                         jint pitch) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setPitchSemiTones(pitch);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitchSemiTones2(JNIEnv *env, jobject thiz, jlong handle,
                                                         jdouble pitch) {
    auto *ptr = (SoundTouch*)handle;
    ptr->setPitchSemiTones(pitch);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setSettingValue(JNIEnv *env, jobject thiz, jlong handle,
                                                      jint setting_id, jint value) {
    auto *ptr = (SoundTouch*)handle;
    if (ptr->setSetting(setting_id, value)) {
        return 0;
    }
    else {
        return -1;
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getSettingValue(JNIEnv *env, jobject thiz, jlong handle,
                                                      jint setting_id) {
    auto *ptr = (SoundTouch*)handle;
    return ptr->getSetting(setting_id);
}