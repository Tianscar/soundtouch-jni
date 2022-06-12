package com.tianscar.soundtouchjni.test;

import com.tianscar.soundtouchjni.BPMDetector;
import com.tianscar.soundtouchjni.SoundTouch;

public class J2SETest {

    public static void main(String[] args) {
        SoundTouch soundTouch = new SoundTouch();
        soundTouch.dispose();
        BPMDetector bpmDetector = new BPMDetector(1, 22400);
        bpmDetector.dispose();
    }

}
