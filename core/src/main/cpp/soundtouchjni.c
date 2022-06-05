#include "com_tianscar_soundtouch4j_SoundTouch.h"
#include "SoundTouchDLL.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_createInstance
        (JNIEnv *env, jclass clazz) {
    return (jlong) soundtouch_createInstance();
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_destroyInstance
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_destroyInstance((HANDLE) h);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setRate
        (JNIEnv *env, jclass clazz, jlong h, jfloat newRate) {
    soundtouch_setRate((HANDLE) h, newRate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setTempo
        (JNIEnv *env, jclass clazz, jlong h, jfloat newTempo) {
    soundtouch_setTempo((HANDLE) h, newTempo);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setRateChange
        (JNIEnv *env, jclass clazz, jlong h, jfloat newRate) {
    soundtouch_setRateChange((HANDLE) h, newRate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setTempoChange
        (JNIEnv *env, jclass clazz, jlong h, jfloat newTempo) {
    soundtouch_setTempoChange((HANDLE) h, newTempo);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setPitch
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitch((HANDLE) h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setPitchOctaves
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitchOctaves((HANDLE) h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setPitchSemiTones
        (JNIEnv *env, jclass clazz, jlong h, jfloat newPitch) {
    soundtouch_setPitchSemiTones((HANDLE) h, newPitch);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setChannels
        (JNIEnv *env, jclass clazz, jlong h, jlong numChannels) {
    soundtouch_setChannels((HANDLE) h, numChannels);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setSampleRate
        (JNIEnv *env, jclass clazz, jlong h, jlong srate) {
    soundtouch_setSampleRate((HANDLE) h, srate);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_flush
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_flush((HANDLE) h);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_putSamples
        (JNIEnv *env, jclass clazz, jlong h, jfloatArray samples, jint numSamples) {
    jfloat *temp;
    temp = (*env)->GetFloatArrayElements(env, samples, NULL);
    soundtouch_putSamples((HANDLE) h, temp, numSamples);
    (*env)->ReleaseFloatArrayElements(env, samples, temp, 0);
    (*env)->DeleteLocalRef(env, samples);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_putSamples_1i16
        (JNIEnv *env, jclass clazz, jlong h, jshortArray samples, jint numSamples) {
    jshort *temp;
    temp = (*env)->GetShortArrayElements(env, samples, NULL);
    soundtouch_putSamples_i16((HANDLE) h, temp, numSamples);
    (*env)->ReleaseShortArrayElements(env, samples, temp, 0);
    (*env)->DeleteLocalRef(env, samples);
}

JNIEXPORT void JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_clear
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_clear((HANDLE) h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_setSetting
        (JNIEnv *env, jclass clazz, jlong h, jint settingId, jint value) {
    soundtouch_setSetting((HANDLE) h, settingId, value);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_getSetting
        (JNIEnv *env, jclass clazz, jlong h, jint settingId) {
    return soundtouch_getSetting((HANDLE) h, settingId);
}

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_numUnprocessedSamples
        (JNIEnv *env, jclass clazz, jlong h) {
    return soundtouch_numUnprocessedSamples((HANDLE) h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_receiveSamples
        (JNIEnv *env, jclass clazz, jlong h, jfloatArray outBuffer, jint maxSamples) {
    jfloat *temp;
    jsize length = (jsize) soundtouch_receiveSamples((HANDLE) h, temp, maxSamples);
    (*env)->SetFloatArrayRegion(env, outBuffer, 0, length, temp);
    return length;
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_receiveSamples_1i16
        (JNIEnv *env, jclass clazz, jlong h, jshortArray outBuffer, jint maxSamples) {
    jshort *temp;
    jsize length = (jsize) soundtouch_receiveSamples_i16((HANDLE) h, temp, maxSamples);
    (*env)->SetShortArrayRegion(env, outBuffer, 0, length, temp);
    return length;
}

JNIEXPORT jlong JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_numSamples
        (JNIEnv *env, jclass clazz, jlong h) {
    return soundtouch_numSamples((HANDLE) h);
}

JNIEXPORT jint JNICALL Java_com_tianscar_soundtouch4j_SoundTouch_isEmpty
        (JNIEnv *env, jclass clazz, jlong h) {
    soundtouch_isEmpty((HANDLE) h);
}

#ifdef __cplusplus
}
#endif