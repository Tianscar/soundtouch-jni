package com.tianscar.soundtouch4j.test;

import com.tianscar.soundtouch4j.BPMDetector;
import com.tianscar.soundtouch4j.SoundTouch;

public class J2SETest {

    public static void main(String[] args) {
        SoundTouch soundTouch = new SoundTouch();
        soundTouch.dispose();
        BPMDetector bpmDetector = new BPMDetector(1, 22400);
        bpmDetector.dispose();
    }

}
