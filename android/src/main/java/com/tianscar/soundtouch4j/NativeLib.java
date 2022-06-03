package com.tianscar.soundtouch4j;

public class NativeLib {

    // Used to load the 'soundtouch4j' library on application startup.
    static {
        System.loadLibrary("soundtouchjni");
    }

    /**
     * A native method that is implemented by the 'soundtouchjni' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}