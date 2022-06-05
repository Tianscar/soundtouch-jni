package com.tianscar.soundtouch4j;

final class Util {

    private Util(){}

    public static int UNSIGNED_MIN = 0;
    public static long UINT_MAX = 4294967295L;

    public static void checkUnsignedInt(long value) {
        if (value < UNSIGNED_MIN) throw new IllegalArgumentException("Unsigned int cannot be < " + UNSIGNED_MIN + ".");
        else if (value > UINT_MAX) throw new IllegalArgumentException("Unsigned int cannot be > " + UINT_MAX + ".");
    }

    public static void loadLibrary() {
    }

}
