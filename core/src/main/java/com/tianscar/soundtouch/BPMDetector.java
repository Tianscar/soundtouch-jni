/*
 * MIT License
 *
 * Copyright (c) 2021-2023 Karstian Lee
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

package com.tianscar.soundtouch;

import java.io.Closeable;
import java.io.IOException;
import java.util.concurrent.atomic.AtomicBoolean;

import static com.tianscar.soundtouch.Util.checkUnsignedInt;
import static com.tianscar.soundtouch.Util.loadLibrary;
import static java.util.Objects.requireNonNull;

/**
 * The BPMDetector class that invokes native BPMDetector routines through the JNI
 * interface.
 * <br><br>
 * <b>
 * Beats-per-minute (BPM) detection routine.
 * </b><br>
 * The beat detection algorithm works as follows:
 * <ul>
 * <li>
 * Use function 'inputSamples' to input a chunks of samples to the class for
 *   analysis. It's a good idea to enter a large sound file or stream in smallish
 *   chunks of around few kilosamples in order not to extinguish too much RAM memory.</li>
 * <li>
 * Input sound data is decimated to approx 500 Hz to reduce calculation burden,
 *   which is basically ok as low (bass) frequencies mostly determine the beat rate.
 *   Simple averaging is used for anti-alias filtering because the resulting signal
 *   quality isn't of that high importance.</li>
 * <li>
 * Decimated sound data is enveloped, i.e. the amplitude shape is detected by
 *   taking absolute value that's smoothed by sliding average. Signal levels that
 *   are below a couple of times the general RMS amplitude level are cut away to
 *   leave only notable peaks there.</li>
 * <li>
 * Repeating sound patterns (e.g. beats) are detected by calculating short-term
 *   autocorrelation function of the enveloped signal.</li>
 * <li>
 * After whole sound data file has been analyzed as above, the bpm level is
 *   detected by function 'getBpm' that finds the highest peak of the autocorrelation
 *   function, calculates it's precise location and converts this reading to bpm's.</li></ul>
 *
 * @author Tianscar
 */
public final class BPMDetector implements Closeable {

    static {
        loadLibrary();
    }

    private volatile long handle;
    private final AtomicBoolean disposed;

    private void checkDisposed() {
        if (disposed.get()) throw new IllegalStateException("The native instance has been disposed. " +
                "Please create a new SoundTouch object for use.");
    }

    /**
     * Return true if the native instance has been destroyed.
     *
     * @return whether the native instance has been destroyed
     */
    public boolean isDisposed() {
        return disposed.get();
    }

    /** Create a new instance of BPM detector.
     *
     * @param numChannels Number of channels in sample data.
     * @param sampleRate Sample rate in Hz.
     */
    public BPMDetector(int numChannels, int sampleRate) {
        handle = createInstance(numChannels, sampleRate);
        disposed = new AtomicBoolean(false);
    }
    private static native long createInstance(int numChannels, int sampleRate);

    /** Destroys a BPM detector instance. */
    public void dispose() {
        if (disposed.compareAndSet(false, true)) {
            destroyInstance(handle);
            handle = 0;
        }
    }

    /** Destroys a BPM detector instance. */
    @Override
    public void close() throws IOException {
        dispose();
    }

    @SuppressWarnings("deprecation")
    @Override
    protected void finalize() throws Throwable {
        dispose();
    }

    private static native void destroyInstance(long h);

    /** Feed 'numSamples' sample frames from 'samples' into the BPM detector.
     *
     * @param samples Sample buffer array.
     * @param offset Offset of sample buffer array.
     * @param numSamples Number of samples in buffer. Notice that in case of stereo-sound a single sample
     *                   contains data for both channels.
     */
    public void putSamples(final float[] samples, int offset, int numSamples) {
        checkDisposed();
        requireNonNull(samples, "samples cannot be null.");
        checkUnsignedInt(numSamples);
        putSamples(handle, samples, offset, numSamples);
    }
    private static native void putSamples(long h, final float[] samples, int offset, int numSamples);

    /** Feed 'numSamples' sample frames from 'samples' into the BPM detector.
     * 16bit int sample format version.
     *
     * @param samples Sample buffer array.
     * @param offset Offset of sample buffer array.
     * @param numSamples Number of samples in buffer. Notice that in case of stereo-sound a single sample
     *                   contains data for both channels.
     */
    public void putSamples(final short[] samples, int offset, int numSamples) {
        checkDisposed();
        requireNonNull(samples, "samples cannot be null.");
        checkUnsignedInt(numSamples);
        putSamples_i16(handle, samples, offset, numSamples);
    }
    private static native void putSamples_i16(long h, final short[] samples, int offset, int numSamples);

    /** Analyzes the results and returns the BPM rate. Use this function to read result
     * after whole song data has been input to the class by consecutive calls of
     * 'inputSamples' function.
     *
     * @return Beats-per-minute rate, or zero if detection failed.
     */
    public float getBPM() {
        checkDisposed();
        return getBpm(handle);
    }
    private static native float getBpm(long h);

}
