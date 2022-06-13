package com.tianscar.soundtouchjni.test;

import com.tianscar.soundtouchjni.BPMDetector;
import com.tianscar.soundtouchjni.SoundTouch;

public class J2SETest {

    public static void main(String[] args) {
        System.load("/home/tianscar/扩展/文档/IdeaProjects/soundtouchjni/j2se/src/main/resources/linux-amd64/libSoundTouchDLL.so");
        System.load("/home/tianscar/扩展/文档/IdeaProjects/soundtouchjni/j2se/src/main/resources/linux-amd64/libsoundtouchjni.so");
        SoundTouch soundTouch = new SoundTouch();
        soundTouch.dispose();
        BPMDetector bpmDetector = new BPMDetector(1, 22400);
        bpmDetector.dispose();
    }

}
