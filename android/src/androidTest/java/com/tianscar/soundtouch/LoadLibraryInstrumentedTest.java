package com.tianscar.soundtouch;

import android.support.test.runner.AndroidJUnit4;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class LoadLibraryInstrumentedTest {

    @Test
    public void loadLibraryTest() throws ClassNotFoundException {
        Class.forName("com.tianscar.soundtouch.SoundTouch");
    }

}