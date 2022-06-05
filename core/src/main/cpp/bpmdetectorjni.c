#include "com_tianscar_soundtouch4j_SoundTouch.h"
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
    temp = (*env)->GetFloatArrayElements(env, samples, NULL);
    bpm_putSamples((HANDLE) h, temp, numSamples);
    (*env)->ReleaseFloatArrayElements(env, samples, temp, 0);
    (*env)->DeleteLocalRef(env, samples);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_BPMDetector_putSamples_1i16
(JNIEnv *env, jclass clazz, jlong h, jshortArray samples, jint numSamples) {
    jshort *temp;
    temp = (*env)->GetShortArrayElements(env, samples, NULL);
    bpm_putSamples_i16((HANDLE) h, temp, numSamples);
    (*env)->ReleaseShortArrayElements(env, samples, temp, 0);
    (*env)->DeleteLocalRef(env, samples);
}

JNIEXPORT jfloat JNICALL Java_com_tianscar_soundtouch4j_BPMDetector_getBpm
        (JNIEnv *env, jclass clazz, jlong h) {
    return bpm_getBpm((HANDLE) h);
}

#ifdef __cplusplus
}
#endif
