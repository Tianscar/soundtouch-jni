package com.tianscar.soundtouch4j;

public final class SoundTouch {

    private long handle;
    private boolean disposed;

    private static native long createInstance();
    private static native long destroyInstance(long handle);

}
