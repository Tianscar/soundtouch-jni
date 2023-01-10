package com.tianscar.soundtouch.test;

import org.junit.jupiter.api.Test;

public class LoadlibraryTest {

    @Test
    public void loadLibraryTest() throws ClassNotFoundException {
        Class.forName("com.tianscar.soundtouch.SoundTouch");
    }

}
