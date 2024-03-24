package com.tianscar.soundtouch.test;

import com.tianscar.soundtouch.SoundTouch;
import org.junit.jupiter.api.Test;

public class LoadlibraryTest {

    @Test
    public void loadLibraryTest() throws ClassNotFoundException {
        Class.forName("com.tianscar.soundtouch.SoundTouch");
    }

    @Test
    public void samplesTest() {
        SoundTouch soundTouch = new SoundTouch();

        float[] samples = new float[44100];
        soundTouch.setSampleRate(44100);
        soundTouch.setChannels(1);
        soundTouch.setRate(2.f);
        soundTouch.putSamples(samples, samples.length);
        soundTouch.flush();
        soundTouch.receiveSamples(samples, (int) soundTouch.numSamples());
        soundTouch.dispose();
    }

}
