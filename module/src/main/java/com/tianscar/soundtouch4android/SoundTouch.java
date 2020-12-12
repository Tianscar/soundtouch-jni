////////////////////////////////////////////////////////////////////////////////
///
/// Example class that invokes native SoundTouch routines through the JNI
/// interface.
///
/// Author        : Copyright (c) Olli Parviainen
/// Modifier      : Copyright (c) AnsdoShip Studio @ Tianscar
/// Author e-mail : oparviai 'at' iki.fi
/// WWW           : http://www.surina.net
///
////////////////////////////////////////////////////////////////////////////////

package com.tianscar.soundtouch4android;

public final class SoundTouch {

    // Channel IDs for the 'setChannels' & 'getChannels' functions:
    public static final int SOUNDTOUCH_CHANNEL_MONO = 1;
    public static final int SOUNDTOUCH_CHANNEL_STEREO = 2;
    public static final int SOUNDTOUCH_MAX_CHANNELS = 16;

    // Available setting IDs for the 'setSettingValue' & 'getSettingValue' functions:

    // Enable/disable anti-alias filter in pitch transposer (0 = disable)
    public static final int SETTING_USE_AA_FILTER = 0;
    // Pitch transposer anti-alias filter length (8 .. 128 taps, default = 32)
    public static final int SETTING_AA_FILTER_LENGTH = 1;
    // Enable/disable quick seeking algorithm in tempo changer routine
    // (enabling quick seeking lowers CPU utilization but causes a minor sound
    //  quality compromising)
    public static final int SETTING_USE_QUICKSEEK = 2;
    // Time-stretch algorithm single processing sequence length in milliseconds. This determines
    // to how long sequences the original sound is chopped in the time-stretch algorithm.
    public static final int SETTING_SEQUENCE_MS = 3;
    // Time-stretch algorithm seeking window length in milliseconds for algorithm that finds the
    // best possible overlapping location. This determines from how wide window the algorithm
    // may look for an optimal joining location when mixing the sound sequences back together.
    public static final int SETTING_SEEKWINDOW_MS = 4;
    // Time-stretch algorithm overlap length in milliseconds. When the chopped sound sequences
    // are mixed back together, to form a continuous sound stream, this parameter defines over
    // how long period the two consecutive sequences are let to overlap each other.
    public static final int SETTING_OVERLAP_MS = 5;
    // Call "getSetting" with this ID to query processing sequence size in samples.
    // This value gives approximate value of how many input samples you'll need to
    // feed into SoundTouch after initial buffering to get out a new batch of
    // output samples.
    //
    // This value does not include initial buffering at beginning of a new processing
    // stream, use SETTING_INITIAL_LATENCY to get the initial buffering size.
    //
    // Notices:
    // - This is read-only parameter, i.e. setSetting ignores this parameter
    // - This parameter value is not constant but change depending on
    //   tempo/pitch/rate/samplerate settings.
    public static final int SETTING_NOMINAL_INPUT_SEQUENCE = 6;
    // Call "getSetting" with this ID to query nominal average processing output
    // size in samples. This value tells approcimate value how many output samples
    // SoundTouch outputs once it does DSP processing run for a batch of input samples.
    //
    // Notices:
    // - This is read-only parameter, i.e. setSetting ignores this parameter
    // - This parameter value is not constant but change depending on
    //   tempo/pitch/rate/samplerate settings.
    public static final int SETTING_NOMINAL_OUTPUT_SEQUENCE = 7;
    // Call "getSetting" with this ID to query initial processing latency, i.e.
    // approx. how many samples you'll need to enter to SoundTouch pipeline before
    // you can expect to get first batch of ready output samples out.
    //
    // After the first output batch, you can then expect to get approx.
    // SETTING_NOMINAL_OUTPUT_SEQUENCE ready samples out for every
    // SETTING_NOMINAL_INPUT_SEQUENCE samples that you enter into SoundTouch.
    //
    // Example:
    //     processing with parameter -tempo=5
    //     => initial latency = 5509 samples
    //        input sequence  = 4167 samples
    //        output sequence = 3969 samples
    //
    // Accordingly, you can expect to feed in approx. 5509 samples at beginning of
    // the stream, and then you'll get out the first 3969 samples. After that, for
    // every approx. 4167 samples that you'll put in, you'll receive again approx.
    // 3969 samples out.
    //
    // This also means that average latency during stream processing is
    // INITIAL_LATENCY-OUTPUT_SEQUENCE/2, in the above example case 5509-3969/2
    // = 3524 samples
    //
    // Notices:
    // - This is read-only parameter, i.e. setSetting ignores this parameter
    // - This parameter value is not constant but change depending on
    //   tempo/pitch/rate/samplerate settings.
    public static final int SETTING_INITIAL_LATENCY = 8;

    // Native instance
    private long handle = 0;

    // Is native instance released
    private boolean released;

    // Native interface function that returns SoundTouch version string.
    // This invokes the native c++ routine defined in "soundtouch-jni.cpp".

    /* Instance */

    private native static long newInstance();
    private native void deleteInstance(long handle);

    /* Samples */

    private native int getChannels(long handle);
    private native void setChannels(long handle, int channels);
    private native void setSampleRate(long handle, int srate);
    private native int getUnprocessedSampleCount(long handle);
    private native int getSampleCount(long handle);
    private native int adjustAmountOfSamples(long handle, int amount);
    private native void flush(long handle);
    private native void clear(long handle);
    private native int isEmpty(long handle);
    private native double getInputOutputSampleRatio(long handle);
    private native void putSamples(long handle, float[] samples, int amount);
    private native int receiveSamples1(long handle, float[] sampleBuffer, int maxSamples);
    private native int receiveSamples2(long handle, int maxSamples);

    /* Rate */

    private native void setRate(long handle, double rate);
    private native void setTempo(long handle, double tempo);
    private native void setRateChange(long handle, double rate);
    private native void setTempoChange(long handle, double tempo);

    /* Pitch */

    private native void setPitch(long handle, double pitch);
    private native void setPitchOctaves(long handle, double pitch);
    private native void setPitchSemiTones1(long handle, int pitch);
    private native void setPitchSemiTones2(long handle, double pitch);

    /* Setting */

    private native int setSettingValue(long handle, int settingId, int value);
    private native int getSettingValue(long handle, int settingId);

    /* I/O */

    /*
    private native int processFile(long handle, String inputFilePath, String outputFilePath);

     */

    /* Others */

    // Get SoundTouch library version Id
    private native static int getVersionId();
    // Get SoundTouch library version string
    private native static String getVersionString();
    // Get native error string
    private native static String getErrorString();

    // Public API

    /* Instance */

    // Instantiation native instance
    public SoundTouch() {
    	handle = newInstance();
    	setChannels(handle, SOUNDTOUCH_CHANNEL_MONO);
    	released = false;
    }

    // Release native instance
    public void release() {
        checkReleased();
    	deleteInstance(handle);
    	handle = 0;
    	released = true;
    }

    /* Samples */

    // Return number of channels
    public int getChannels() {
        checkReleased();
        return getChannels(handle);
    }

    // Sets the number of channels, 1 = mono, 2 = stereo
    public void setChannels(int channels) {
        checkReleased();
        if (channels < SOUNDTOUCH_CHANNEL_MONO || channels > SOUNDTOUCH_MAX_CHANNELS) {
            throw new RuntimeException("Illegal channel amount, 1 = mono, 2 = stereo, the maximum value is 16");
        }
        setChannels(handle, channels);
    }

    // Sets sample rate.
    public void setSamplingRate(int srate) {
        checkReleased();
        if (srate < 0) {
            throw new RuntimeException("Illegal param, srate must be >= 0");
        }
        setSampleRate(handle, srate);
    }

    // Get ratio between input and output audio durations, useful for calculating
    // processed output duration: if you'll process a stream of N samples, then
    // you can expect to get out N * getInputOutputSampleRatio() samples.
    public double getInputOutputSampleRatio() {
        checkReleased();
        return getInputOutputSampleRatio(handle);
    }

    // Returns number of samples currently unprocessed.
    public int getUnprocessedSampleCount() {
        checkReleased();
        return getUnprocessedSampleCount(handle);
    }

    // Returns number of samples currently available.
    public int getSampleCount() {
        checkReleased();
        return getSampleCount(handle);
    }

    // allow trimming (downwards) amount of samples in pipeline.
    // Returns adjusted amount of samples
    public int adjustAmountOfSamples(int amount) {
        checkReleased();
        if (amount < 0) {
            throw new RuntimeException("Illegal param, amount must be >= 0");
        }
        return adjustAmountOfSamples(handle, amount);
    }

    // Adds 'amount' pcs of samples from the 'samples' memory position into
    // the input of the object.
    public void putSamples(float[] samples, int amount) {
        checkReleased();
        if (amount < 0) {
            throw new RuntimeException("Illegal param, amount must be >= 0");
        }
        putSamples(handle, samples, amount);
    }

    // Output samples from beginning of the sample buffer. Copies requested samples to
    // output buffer and removes them from the sample buffer. If there are less than
    // amount of samples in the buffer, returns all that available.
    //
    // return Number of samples returned.
    public int receiveSamples(float[] sampleBuffer, int maxSamples) {
        checkReleased();
        if (maxSamples < 0) {
            throw new RuntimeException("Illegal param, maxSamples must be >= 0");
        }
        return receiveSamples1(handle, sampleBuffer, maxSamples);
    }

    // Adjusts book-keeping so that given number of samples are removed from beginning of the
    // sample buffer without copying them anywhere.
    //
    // return Number of samples returned.
    public int receiveSamples(int maxSamples) {
        checkReleased();
        if (maxSamples < 0) {
            throw new RuntimeException("Illegal param, maxSamples must be >= 0");
        }
        return receiveSamples2(handle, maxSamples);
    }

    // Flushes the last samples from the processing pipeline to the output.
    // Clears also the internal processing buffers.
    //
    // Note: This function is meant for extracting the last samples of a sound
    // stream. This function may introduce additional blank samples in the end
    // of the sound stream, and thus it's not recommended to call this function
    // in the middle of a sound stream.
    public void flush() {
        checkReleased();
        flush(handle);
    }

    // Clears all the samples in the object's output and internal processing
    // buffers.
    public void clear() {
        checkReleased();
        clear(handle);
    }

    // Returns true if there aren't any samples available for outputting.
    public boolean isEmpty() {
        checkReleased();
        return isEmpty(handle) != 0;
    }

    /* Rate */

    // Sets new rate control value. Normal rate = 1.0, smaller values
    // represent slower rate, larger faster rates.
    public void setRate(double rate) {
        checkReleased();
        setRate(handle, rate);
    }

    // Sets new tempo control value. Normal tempo = 1.0, smaller values
    // represent slower tempo, larger faster tempo.
    public void setTempo(double tempo) {
        checkReleased();
    	setTempo(handle, tempo);
    }

    // Sets new rate control value as a difference in percents compared
    // to the original rate (-50 .. +100 %)
    public void setRateChange(double rate) {
        checkReleased();
        setRateChange(handle, rate);
    }

    // Sets new tempo control value as a difference in percents compared
    // to the original tempo (-50 .. +100 %)
    public void setTempoChange(double tempo) {
        checkReleased();
        setTempoChange(handle, tempo);
    }

    /* Pitch */

    // Sets new pitch control value. Original pitch = 1.0, smaller values
    // represent lower pitches, larger values higher pitch.
    public void setPitch(double pitch) {
        checkReleased();
        setPitch(handle, pitch);
    }

    // Sets pitch change in octaves compared to the original pitch
    // (-1.00 .. +1.00)
    public void setPitchOctaves(double pitch) {
        checkReleased();
        setPitchOctaves(handle, pitch);
    }

    // Sets pitch change in semi-tones compared to the original pitch
    // (-12 .. +12)
    public void setPitchSemiTones(int pitch) {
        checkReleased();
        setPitchSemiTones1(handle, pitch);
    }

    public void setPitchSemiTones(double pitch) {
        checkReleased();
    	setPitchSemiTones2(handle, pitch);
    }

    /* Setting */

    // Changes a setting controlling the processing system behaviour. See the
    // 'SETTING_...' defines for available setting ID's.
    public boolean setSettingValue(int settingId, int value) {
        checkReleased();
        int result = setSettingValue(handle, settingId, value);
        return result == 0;
    }

    public int getSettingValue(int settingId) {
        checkReleased();
        return getSettingValue(handle, settingId);
    }

    /* I/O */

    /*
    public void processFile(String inputFilePath, String outputFilePath) throws IOException {
        checkReleased();
        int result = processFile(handle, inputFilePath, outputFilePath);
        if (result == -1) {
            throw new IOException("JNI exception: " + getErrorString());
        }
    }

     */
    
    /* Others */

    // Get SoundTouch library version Id
    public static int getSoundTouchVersionId() {
        if (getVersionId() == -1) {
            throw new RuntimeException(getErrorString());
        }
        return getVersionId();
    }

    // Get SoundTouch library version string
    public static String getSoundTouchVersionString() {
        if (getVersionString() == null) {
            throw new RuntimeException(getErrorString());
        }
        return getVersionString();
    }

    // Returns false if has been released.
    public boolean isReleased() {
        return released;
    }

    // Private Java methods

    private void checkReleased() {
        if (isReleased()) {
            throw new RuntimeException("The native instance has been released. Please create a new SoundTouch object for use.");
        }
    }
    
    // Load the native library upon startup
    static {
        System.loadLibrary("soundtouch");
    }

}