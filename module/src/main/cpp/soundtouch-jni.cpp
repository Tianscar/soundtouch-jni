////////////////////////////////////////////////////////////////////////////////
///
/// Example Interface class for SoundTouch native compilation
///
/// Author        : Copyright (c) Olli Parviainen
/// Modifier      : Copyright (c) AnsdoShip Studio @ Tianscar
/// Author e-mail : oparviai 'at' iki.fi
/// WWW           : http://www.surina.net
///
////////////////////////////////////////////////////////////////////////////////

#include <jni.h>
#include <android/log.h>
#include <stdexcept>
#include <string>

using namespace std;

#include "com_tianscar_soundtouch4android_SoundTouch.h"
#include "SoundTouch.h"
#include "WavFile.h"

#define LOGV(...)   __android_log_print((int)ANDROID_LOG_INFO, "SOUNDTOUCH", __VA_ARGS__)
//#define LOGV(...)


// String for keeping possible c++ exception error messages. Notice that this isn't
// thread-safe but it's expected that exceptions are special situations that won't
// occur in several threads in parallel.
static string _errMsg = "";


#define DLL_PUBLIC __attribute__ ((visibility ("default")))
//#define BUFF_SIZE 4096


using namespace soundtouch;


// Set error message to return
static void _setErrmsg(const char *msg)
{
	_errMsg = msg;
}


#ifdef _OPENMP

#include <pthread.h>
extern pthread_key_t gomp_tls_key;
static void * _p_gomp_tls = NULL;

/// Function to initialize threading for OpenMP.
///
/// This is a workaround for bug in Android NDK v10 regarding OpenMP: OpenMP works only if
/// called from the Android App main thread because in the main thread the gomp_tls storage is
/// properly set, however, Android does not properly initialize gomp_tls storage for other threads.
/// Thus if OpenMP routines are invoked from some other thread than the main thread,
/// the OpenMP routine will crash the application due to NULL pointer access on uninitialized storage.
///
/// This workaround stores the gomp_tls storage from main thread, and copies to other threads.
/// In order this to work, the Application main thread needws to call at least "getVersionString"
/// routine.
static int _init_threading(bool warn)
{
	void *ptr = pthread_getspecific(gomp_tls_key);
	LOGV("JNI thread-specific TLS storage %ld", (long)ptr);
	if (ptr == NULL)
	{
		LOGV("JNI set missing TLS storage to %ld", (long)_p_gomp_tls);
		pthread_setspecific(gomp_tls_key, _p_gomp_tls);
	}
	else
	{
		LOGV("JNI store this TLS storage");
		_p_gomp_tls = ptr;
	}
	// Where critical, show warning if storage still not properly initialized
	if ((warn) && (_p_gomp_tls == NULL))
	{
		_setErrmsg("Error - OpenMP threading not properly initialized: Call this method from the App main thread!");
		return -1;
	}
	return 0;
}

#else
static int _init_threading(bool warn)
{
	// do nothing if not OpenMP build
	return 0;
}
#endif


// Processes the sound file

/*
static void _processFile(SoundTouch *pSoundTouch, const char *inFileName, const char *outFileName)
{
    int nSamples;
    int nChannels;
    int buffSizeSamples;
    SAMPLETYPE sampleBuffer[BUFF_SIZE];

    // open input file
    WavInFile inFile(inFileName);
    int sampleRate = inFile.getSampleRate();
    int bits = inFile.getNumBits();
    nChannels = inFile.getNumChannels();

    // create output file
    WavOutFile outFile(outFileName, sampleRate, bits, nChannels);

    pSoundTouch->setSampleRate(sampleRate);
    pSoundTouch->setChannels(nChannels);

    assert(nChannels > 0);
    buffSizeSamples = BUFF_SIZE / nChannels;

    // Process samples read from the input file
    while (inFile.eof() == 0)
    {
        int num;

        // Read a chunk of samples from the input file
        num = inFile.read(sampleBuffer, BUFF_SIZE);
        nSamples = num / nChannels;

        // Feed the samples into SoundTouch processor
        pSoundTouch->putSamples(sampleBuffer, nSamples);

        // Read ready samples from SoundTouch processor & write them output file.
        // NOTES:
        // - 'receiveSamples' doesn't necessarily return any samples at all
        //   during some rounds!
        // - On the other hand, during some round 'receiveSamples' may have more
        //   ready samples than would fit into 'sampleBuffer', and for this reason
        //   the 'receiveSamples' call is iterated for as many times as it
        //   outputs samples.
        do
        {
            nSamples = pSoundTouch->receiveSamples(sampleBuffer, buffSizeSamples);
            outFile.write(sampleBuffer, nSamples * nChannels);
        } while (nSamples != 0);
    }

    // Now the input file is processed, yet 'flush' few last samples that are
    // hiding in the SoundTouch's internal processing pipeline.
    pSoundTouch->flush();
    do
    {
        nSamples = pSoundTouch->receiveSamples(sampleBuffer, buffSizeSamples);
        outFile.write(sampleBuffer, nSamples * nChannels);
    } while (nSamples != 0);
}

 */

// JNI API

/* Instance */
extern "C" DLL_PUBLIC jlong Java_com_tianscar_soundtouch4android_SoundTouch_newInstance(JNIEnv *env,
                                                                                          jclass thiz)
{
	return (jlong)(new SoundTouch());
}
extern "C" DLL_PUBLIC void Java_com_tianscar_soundtouch4android_SoundTouch_deleteInstance(JNIEnv *env, jobject thiz, jlong handle)
{
	SoundTouch *ptr = (SoundTouch*)handle;
	delete ptr;
}

/* Sample */
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getChannels(JNIEnv *env, jobject thiz, jlong handle) {
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->numChannels();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setChannels(JNIEnv *env, jobject thiz, jlong handle,
                                                  jint channels) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setChannels(channels);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setSampleRate(JNIEnv *env, jobject thiz, jlong handle,
                                                    jint srate) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setSampleRate(srate);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getInputOutputSampleRatio(JNIEnv *env, jobject thiz,
                                                                jlong handle) {
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->getInputOutputSampleRatio();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getUnprocessedSampleCount(JNIEnv *env, jobject thiz,
                                                                 jlong handle) {
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->numUnprocessedSamples();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getSampleCount(JNIEnv *env, jobject thiz, jlong handle) {
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->numSamples();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_adjustAmountOfSamples(JNIEnv *env, jobject thiz, jlong handle,
                                                            jint samples_count) {
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->adjustAmountOfSamples(samples_count);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_putSamples(JNIEnv *env, jobject thiz, jlong handle,
                                                 jfloatArray samples, jint sample_count) {
    SoundTouch *ptr = (SoundTouch*)handle;
    jfloat *temp;
    temp = env->GetFloatArrayElements(samples, 0);
    ptr->putSamples(temp, sample_count);
    env->ReleaseFloatArrayElements(samples, temp, 0);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_receiveSamples1(JNIEnv *env, jobject thiz, jlong handle,
                                                      jfloatArray sample_buffer, jint max_samples) {
    SoundTouch *ptr = (SoundTouch*)handle;
    jfloat *temp;
    temp = env->GetFloatArrayElements(sample_buffer, 0);
    ptr->putSamples(temp, max_samples);
    jint result = ptr->receiveSamples(temp, max_samples);
    env->ReleaseFloatArrayElements(sample_buffer, temp, 0);
    return result;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_receiveSamples2(JNIEnv *env, jobject thiz, jlong handle,
                                                      jint max_samples) {
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->receiveSamples(max_samples);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_flush(JNIEnv *env, jobject thiz, jlong handle) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->flush();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_clear(JNIEnv *env, jobject thiz, jlong handle) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->clear();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_isEmpty(JNIEnv *env, jobject thiz, jlong handle) {
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->isEmpty();
}

/* Rate */
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setRate(JNIEnv *env, jobject thiz, jlong handle,
                                              jdouble rate) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setRate(rate);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setTempo(JNIEnv *env, jobject thiz, jlong handle,
                                               jdouble tempo) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setTempo(tempo);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setRateChange(JNIEnv *env, jobject thiz, jlong handle,
                                                    jdouble rate) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setRateChange(rate);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setTempoChange(JNIEnv *env, jobject thiz, jlong handle,
                                                     jdouble tempo) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setTempoChange(tempo);
}

/* Pitch */
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitch(JNIEnv *env, jobject thiz, jlong handle,
                                               jdouble pitch) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setPitch(pitch);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitchOctaves(JNIEnv *env, jobject thiz, jlong handle,
                                                      jdouble pitch) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setPitchOctaves(pitch);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitchSemiTones1(JNIEnv *env, jobject thiz, jlong handle,
                                                         jint pitch) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setPitchSemiTones(pitch);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setPitchSemiTones2(JNIEnv *env, jobject thiz, jlong handle,
                                                         jdouble pitch) {
    SoundTouch *ptr = (SoundTouch*)handle;
    ptr->setPitchSemiTones(pitch);
}

/* Setting */
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_setSettingValue(JNIEnv *env, jobject thiz, jlong handle,
                                                      jint setting_id, jint value) {
    SoundTouch *ptr = (SoundTouch*)handle;
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
    SoundTouch *ptr = (SoundTouch*)handle;
    return ptr->getSetting(setting_id);
}

/* I/O */
/*
extern "C" DLL_PUBLIC int Java_com_tianscar_soundtouch4android_SoundTouch_processFile(JNIEnv *env, jobject thiz, jlong handle, jstring jinputFile, jstring joutputFile)
{
    SoundTouch *ptr = (SoundTouch*)handle;

    const char *inputFile = env->GetStringUTFChars(jinputFile, 0);
    const char *outputFile = env->GetStringUTFChars(joutputFile, 0);

    LOGV("JNI process file %s", inputFile);

    /// gomp_tls storage bug workaround - see comments in _init_threading() function!
    if (_init_threading(true)) return -1;

    try
    {
        _processFile(ptr, inputFile, outputFile);
    }
    catch (const runtime_error &e)
    {
        const char *err = e.what();
        // An exception occurred during processing, return the error message
        LOGV("JNI exception in SoundTouch::processFile: %s", err);
        _setErrmsg(err);
        return -1;
    }


    env->ReleaseStringUTFChars(jinputFile, inputFile);
    env->ReleaseStringUTFChars(joutputFile, outputFile);

    return 0;
}

 */

/* Others */
extern "C"
JNIEXPORT jint JNICALL
Java_com_tianscar_soundtouch4android_SoundTouch_getVersionId(JNIEnv *env, jclass clazz) {

    LOGV("JNI call SoundTouch.getVersionId");

    /// gomp_tls storage bug workaround - see comments in _init_threading() function!
    if(_init_threading(false) == -1) {
        return -1;
    }

    int threads = 0;
#pragma omp parallel
    {
#pragma omp atomic
        threads ++;
    }
    LOGV("JNI thread count %d", threads);

    // return version id
    return SoundTouch::getVersionId();
}
extern "C" DLL_PUBLIC jstring Java_com_tianscar_soundtouch4android_SoundTouch_getVersionString(JNIEnv *env,
                                                                                     jclass thiz)
{
    const char *verStr;

    LOGV("JNI call SoundTouch.getVersionString");

    // Call example SoundTouch routine
    verStr = SoundTouch::getVersionString();

    /// gomp_tls storage bug workaround - see comments in _init_threading() function!
    if(_init_threading(false) == -1) {
        return NULL;
    }

    int threads = 0;
#pragma omp parallel
    {
#pragma omp atomic
        threads ++;
    }
    LOGV("JNI thread count %d", threads);

    // return version as string
    return env->NewStringUTF(verStr);
}
extern "C" DLL_PUBLIC jstring Java_com_tianscar_soundtouch4android_SoundTouch_getErrorString(JNIEnv *env,
                                                                                   jclass thiz)
{
    jstring result = env->NewStringUTF(_errMsg.c_str());
    _errMsg.clear();

    return result;
}