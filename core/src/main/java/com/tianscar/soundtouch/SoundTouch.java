/*
 * MIT License
 *
 * Copyright (c) 2021-2022 Tianscar
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

import static com.tianscar.soundtouch.Util.checkUnsignedInt;
import static com.tianscar.soundtouch.Util.loadLibrary;
import static java.util.Objects.requireNonNull;

/**
 * The SoundTouch class that invokes native SoundTouch routines through the JNI
 * interface.
 * <br><br>
 * <b>
 * SoundTouch - main class for tempo/pitch/rate adjusting routines.
 * </b><br>
 * Notes:
 * <ul><li>
 * Initialize the SoundTouch object instance by setting up the sound stream
 *   parameters with functions 'setSampleRate' and 'setChannels', then set
 *   desired tempo/pitch/rate settings with the corresponding functions.</li>
 * <li>
 * The SoundTouch class behaves like a first-in-first-out pipeline: The
 *   samples that are to be processed are fed into one of the pipe by calling
 *   function 'putSamples', while the ready processed samples can be read
 *   from the other end of the pipeline with function 'receiveSamples'.</li>
 * <li>
 * The SoundTouch processing classes require certain sized 'batches' of
 *   samples in order to process the sound. For this reason the classes buffer
 *   incoming samples until there are enough of samples available for
 *   processing, then they carry out the processing step and consequently
 *   make the processed samples available for outputting.</li>
 * <li>
 * For the above reason, the processing routines introduce a certain
 *   'latency' between the input and output, so that the samples input to
 *   SoundTouch may not be immediately available in the output, and neither
 *   the amount of outputtable samples may not immediately be in direct
 *   relationship with the amount of previously input samples.</li>
 * <li>
 * The tempo/pitch/rate control parameters can be altered during processing.
 *   Please notice though that they aren't currently protected by semaphores,
 *   so in multi-thread application external semaphore protection may be
 *   required.</li>
 * <li>
 * This class utilizes classes 'TDStretch' for tempo change (without modifying
 *   pitch) and 'RateTransposer' for changing the playback rate (that is, both
 *   tempo and pitch in the same ratio) of the sound. The third available control
 *   'pitch' (change pitch but maintain tempo) is produced by a combination of
 *   combining the two other controls.</li></ul>
 * <a href="https://www.surina.net/soundtouch">SoundTouch WWW</a>
 *
 * @author Tianscar
 */
public final class SoundTouch {

    /** Soundtouch library version string */
    public static final String SOUNDTOUCH_VERSION = "2.3.2";

    /** SoundTouch library version id */
    public static final long SOUNDTOUCH_VERSION_ID = 20302L;

    // Available setting IDs for the 'setSetting' & 'getSetting' functions:

    /** Enable/disable anti-alias filter in pitch transposer (0 = disable) */
    public static final int SETTING_USE_AA_FILTER = 0;

    /** Pitch transposer anti-alias filter length (8 .. 128 taps, default = 32) */
    public static final int SETTING_AA_FILTER_LENGTH = 1;

    /** Enable/disable quick seeking algorithm in tempo changer routine
     * (enabling quick seeking lowers CPU utilization but causes a minor sound
     * quality compromising) */
    public static final int SETTING_USE_QUICKSEEK = 2;

    /** Time-stretch algorithm single processing sequence length in milliseconds. This determines
     * to how long sequences the original sound is chopped in the time-stretch algorithm.
     */
    public static final int SETTING_SEQUENCE_MS = 3;

    /** Time-stretch algorithm seeking window length in milliseconds for algorithm that finds the
     * best possible overlapping location. This determines from how wide window the algorithm
     * may look for an optimal joining location when mixing the sound sequences back together.
     */
    public static final int SETTING_SEEKWINDOW_MS = 4;

    /** Time-stretch algorithm overlap length in milliseconds. When the chopped sound sequences
     * are mixed back together, to form a continuous sound stream, this parameter defines over
     * how long period the two consecutive sequences are let to overlap each other.
     */
    public static final int SETTING_OVERLAP_MS = 5;

    /** Call "getSetting" with this ID to query processing sequence size in samples.
     * This value gives approximate value of how many input samples you'll need to
     * feed into SoundTouch after initial buffering to get out a new batch of
     * output samples.
     * <br><br>
     * This value does not include initial buffering at beginning of a new processing
     * stream, use SETTING_INITIAL_LATENCY to get the initial buffering size.
     * <br><br>
     * Notices:
     * - This is read-only parameter, i.e. setSetting ignores this parameter
     * - This parameter value is not constant but change depending on
     *   tempo/pitch/rate/samplerate settings.
     */
    public static final int SETTING_NOMINAL_INPUT_SEQUENCE = 6;

    /** Call "getSetting" with this ID to query nominal average processing output
     * size in samples. This value tells approcimate value how many output samples
     * SoundTouch outputs once it does DSP processing run for a batch of input samples.
     * <br><br>
     * Notices:
     * - This is read-only parameter, i.e. setSetting ignores this parameter
     * - This parameter value is not constant but change depending on
     *   tempo/pitch/rate/samplerate settings. */
    public static final int SETTING_NOMINAL_OUTPUT_SEQUENCE = 7;

    /** Call "getSetting" with this ID to query initial processing latency, i.e.
     * approx. how many samples you'll need to enter to SoundTouch pipeline before
     * you can expect to get first batch of ready output samples out.
     * <br><br>
     * After the first output batch, you can then expect to get approx.
     * SETTING_NOMINAL_OUTPUT_SEQUENCE ready samples out for every
     * SETTING_NOMINAL_INPUT_SEQUENCE samples that you enter into SoundTouch.
     * <br><br>
     * Example:<br>
     *     processing with parameter -tempo=5<br>
     *     =&gt; initial latency = 5509 samples<br>
     *        input sequence  = 4167 samples<br>
     *        output sequence = 3969 samples
     * <br><br>
     * Accordingly, you can expect to feed in approx. 5509 samples at beginning of
     * the stream, and then you'll get out the first 3969 samples. After that, for
     * every approx. 4167 samples that you'll put in, you'll receive again approx.
     * 3969 samples out.
     * <br><br>
     * This also means that average latency during stream processing is
     * INITIAL_LATENCY-OUTPUT_SEQUENCE/2, in the above example case 5509-3969/2
     * = 3524 samples
     * <br><br>
     * Notices:
     * - This is read-only parameter, i.e. setSetting ignores this parameter
     * - This parameter value is not constant but change depending on
     *   tempo/pitch/rate/samplerate settings. */
    public static final int SETTING_INITIAL_LATENCY = 8;

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

    /** Create a new instance of SoundTouch processor. */
    public SoundTouch() {
        handle = createInstance();
        disposed = false;
    }
    private static native long createInstance();

    /** Destroys a SoundTouch processor instance. */
    public void dispose() {
        checkDisposed();
        disposed = true;
        destroyInstance(handle);
        handle = 0;
    }
    private static native void destroyInstance(long h);

    /** Get SoundTouch library version string.
     *
     * @return the SoundTouch library version string
     */
    public static String getVersionString() {
        return SOUNDTOUCH_VERSION;
    }

    /** Get SoundTouch library version id.
     *
     * @return the SoundTouch library version id
     */
    public static long getVersionId() {
        return SOUNDTOUCH_VERSION_ID;
    }

    /** Sets new rate control value. Normal rate = 1.0, smaller values
     * represent slower rate, larger faster rates.
     *
     * @param newRate the new rate
     */
    public void setRate(float newRate) {
        setRate(handle, newRate);
    }
    private static native void setRate(long h, float newRate);

    /** Sets new tempo control value. Normal tempo = 1.0, smaller values
     * represent slower tempo, larger faster tempo.
     *
     * @param newTempo the new tempo
     */
    public void setTempo(float newTempo) {
        setTempo(handle, newTempo);
    }
    private static native void setTempo(long h, float newTempo);

    /** Sets new rate control value as a difference in percents compared
     * to the original rate (-50 .. +100 %);
     *
     * @param newRate the new rate change radio
     */
    public void setRateChange(float newRate) {
        setRateChange(handle, newRate);
    }
    private static native void setRateChange(long h, float newRate);

    /** Sets new tempo control value as a difference in percents compared
     * to the original tempo (-50 .. +100 %);
     *
     * @param newTempo the new tempo change radio
     */
    public void setTempoChange(float newTempo) {
        setTempoChange(handle, newTempo);
    }
    private static native void setTempoChange(long h, float newTempo);

    /** Sets new pitch control value. Original pitch = 1.0, smaller values
     * represent lower pitches, larger values higher pitch.
     *
     * @param newPitch the new pitch
     */
    public void setPitch(float newPitch) {
        setPitch(handle, newPitch);
    }
    private static native void setPitch(long h, float newPitch);

    /** Sets pitch change in octaves compared to the original pitch
     * (-1.00 .. +1.00);
     *
     * @param newPitch the new pitch octaves
     */
    public void setPitchOctaves(float newPitch) {
        setPitchOctaves(handle, newPitch);
    }
    private static native void setPitchOctaves(long h, float newPitch);

    /** Sets pitch change in semi-tones compared to the original pitch
     * (-12 .. +12);
     *
     * @param newPitch the new pitch semi-tones
     */
    public void setPitchSemiTones(float newPitch) {
        setPitchSemiTones(handle, newPitch);
    }
    private static native void setPitchSemiTones(long h, float newPitch);

    /** Sets the number of channels, 1 = mono, 2 = stereo, n = multichannel
     *
     * @param numChannels Number of channels.
     */
    public void setChannels(long numChannels) {
        checkUnsignedInt(numChannels);
        setChannels(handle, numChannels);
    }
    private static native void setChannels(long h, long numChannels);

    /** Sets sample rate.
     *
     * @param srate Sample rate in Hz.
     */
    public void setSampleRate(long srate) {
        checkUnsignedInt(srate);
        setSampleRate(handle, srate);
    }
    private static native void setSampleRate(long h, long srate);

    /** Flushes the last samples from the processing pipeline to the output.
     * Clears also the internal processing buffers.
     * <br><br>
     * Note: This function is meant for extracting the last samples of a sound
     * stream. This function may introduce additional blank samples in the end
     * of the sound stream, and thus it's not recommended to call this function
     * in the middle of a sound stream.
     */
    public void flush() {
        flush(handle);
    }
    private static native void flush(long h);

    /** Adds 'numSamples' pcs of samples from the 'samples' memory position into
     * the input of the object. Notice that sample rate _has_to_ be set before
     * calling this function, otherwise throws a runtime_error exception.
     *
     * @param samples Sample buffer array.
     * @param numSamples Number of sample frames in buffer. Notice that in case of multi-channel sound a single
     *                   sample frame contains data for all channels.
     */
    public void putSamples(final float[] samples, int numSamples) {
        requireNonNull(samples, "samples cannot be null.");
        checkUnsignedInt(numSamples);
        putSamples(handle, samples, numSamples);
    }
    private static native void putSamples(long h, final float[] samples, int numSamples);

    /** int16 version of putSamples(): This accept int16 (short) sample data
     * and internally converts it to float format before processing.
     *
     * @param samples Sample buffer array.
     * @param numSamples Number of sample frames in buffer. Notice that in case of multi-channel sound a single
     *                   sample frame contains data for all channels.
     */
    public void putSamplesI16(final short[] samples, int numSamples) {
        requireNonNull(samples, "samples cannot be null.");
        checkUnsignedInt(numSamples);
        putSamples_i16(handle, samples, numSamples);
    }
    private static native void putSamples_i16(long h, final short[] samples, int numSamples);


    /** Clears all the samples in the object's output and internal processing
     * buffers.
     */
    public void clear() {
        clear(handle);
    }
    private static native void clear(long h);

    /** Changes a setting controlling the processing system behaviour. See the
     * 'SETTING_...' constants for available setting ID's.
     *
     * @param settingId Setting ID number. see SETTING_... defines.
     * @param value New setting value.
     *
     * @return true if the setting was successfully changed, otherwise false
     */
    public boolean setSetting(int settingId, int value) {
        return setSetting(handle, settingId, value) != 0;
    }
    private static native int setSetting(long h, int settingId, int value);

    /** Reads a setting controlling the processing system behaviour. See the
     * 'SETTING_...' constants for available setting ID's.
     *
     * @param settingId Setting ID number. see SETTING_... constants.
     *
     * @return the setting value.
     */
    public int getSetting(int settingId) {
        return getSetting(handle, settingId);
    }
    private static native int getSetting(long h, int settingId);


    /** Returns number of samples currently unprocessed.
     *
     * @return number of samples currently unprocessed
     */
    public long numUncompressedSamples() {
        return numUnprocessedSamples(handle);
    }
    private static native long numUnprocessedSamples(long h);

    /** Adjusts book-keeping so that given number of samples are removed from beginning of the
     * sample buffer without copying them anywhere.
     * <br><br>
     * Used to reduce the number of samples in the buffer when accessing the sample buffer directly
     * with 'ptrBegin' function.
     *
     * @param outBuffer Buffer where to copy output samples.
     * @param maxSamples How many samples to receive at max.
     *
     * @return the number of samples returned
     */
    public int receiveSamples(float[] outBuffer, int maxSamples) {
        requireNonNull(outBuffer, "outBuffer cannot be null.");
        checkUnsignedInt(maxSamples);
        return receiveSamples(handle, outBuffer, maxSamples);
    }
    private static native int receiveSamples(long h, float[] outBuffer, int maxSamples);


    /** int16 version of receiveSamples(): This converts internal float samples
     * into int16 (short) return data type.
     *
     * @param outBuffer Buffer where to copy output samples.
     * @param maxSamples How many samples to receive at max.
     *
     * @return the number of samples returned
     */
    public int receiveSamplesI16(short[] outBuffer, int maxSamples) {
        requireNonNull(outBuffer, "outBuffer cannot be null.");
        checkUnsignedInt(maxSamples);
        return receiveSamples_i16(handle, outBuffer, maxSamples);
    }
    private static native int receiveSamples_i16(long h, short[] outBuffer, int maxSamples);

    /** Returns number of samples currently available.
     *
     * @return number of samples currently available
     */
    public long numSamples() {
        return numSamples(handle);
    }
    private static native long numSamples(long h);

    /** Returns true if there aren't any samples available for outputting.
     *
     * @return whether there aren't any samples available for outputting
     */
    public boolean isEmpty() {
        return isEmpty(handle) != 0;
    }
    private static native int isEmpty(long h);

}
