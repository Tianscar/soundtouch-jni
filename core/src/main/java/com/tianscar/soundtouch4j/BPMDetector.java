package com.tianscar.soundtouch4j;

import static com.tianscar.soundtouch4j.Util.loadLibrary;

public final class BPMDetector {

    static {
        loadLibrary();
    }

    private volatile long handle;
    private volatile boolean disposed;

    private void checkDisposed() {
        if (disposed) throw new IllegalStateException("The native instance has been disposed. " +
                "Please create a new SoundTouch object for use.");
    }

    /**
     * Return true if the native instance has been destroyed.
     *
     * @return whether the native instance has been destroyed
     */
    public boolean isDisposed() {
        return disposed;
    }

    /** Create a new instance of BPM detector. */
    public BPMDetector(int numChannels, int sampleRate) {
        handle = createInstance(numChannels, sampleRate);
        disposed = false;
    }
    private static native long createInstance(int numChannels, int sampleRate);

    /** Destroys a BPM detector instance. */
    public void dispose() {
        checkDisposed();
        disposed = true;
        destroyInstance(handle);
        handle = 0;
    }
    private static native void destroyInstance(long h);

    /** Feed 'numSamples' sample frames from 'samples' into the BPM detector.
     *
     * @param samples Pointer to sample buffer.
     * @param numSamples Number of samples in buffer. Notice that in case of stereo-sound a single sample
     *                   contains data for both channels.
     */
    public void putSamples(final float[] samples, int numSamples) {
        putSamples(handle, samples, numSamples);
    }
    private static native void putSamples(long h, final float[] samples, int numSamples);

    /** Feed 'numSamples' sample frames from 'samples' into the BPM detector.
     * 16bit int sample format version.
     *
     * @param samples Pointer to sample buffer.
     * @param numSamples Number of samples in buffer. Notice that in case of stereo-sound a single sample
     *                   contains data for both channels.
     */
    public void putSamplesI16(final short[] samples, int numSamples) {
        putSamples_i16(handle, samples, numSamples);
    }
    private static native void putSamples_i16(long h, final short[] samples, int numSamples);

    /** Analyzes the results and returns the BPM rate. Use this function to read result
     * after whole song data has been input to the class by consecutive calls of
     * 'inputSamples' function.
     *
     * @return Beats-per-minute rate, or zero if detection failed.
     */
    public float getBPM() {
        return getBpm(handle);
    }
    private static native float getBpm(long h);

}
