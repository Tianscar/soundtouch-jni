/*
 * MIT License
 *
 * Copyright (c) 2021-2022 Tianscar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

package com.tianscar.soundtouch;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.util.Objects;
import java.util.UUID;

final class Util {

    private Util(){}

    public static int UNSIGNED_MIN = 0;
    public static long UINT_MAX = 4294967295L;

    public static void checkUnsignedInt(long value) {
        if (value < UNSIGNED_MIN) throw new IllegalArgumentException("Unsigned int cannot be < " + UNSIGNED_MIN + ".");
        else if (value > UINT_MAX) throw new IllegalArgumentException("Unsigned int cannot be > " + UINT_MAX + ".");
    }

    private static volatile boolean librariesLoaded = false;

    @SuppressWarnings("UnsafeDynamicallyLoadedCode")
    public static void loadLibrary() {
        if (librariesLoaded) return;
        librariesLoaded = true;
        final String osName = System.getProperty("os.name");
        final String osArch = System.getProperty("os.arch");
        if (System.getProperty("java.runtime.name").equals("Android Runtime")) {
            System.loadLibrary("SoundTouchDLL");
            System.loadLibrary("soundtouchjni");
        }
        else {
            final String os = osName.toLowerCase(), arch = osArch.toLowerCase();
            final String resName, resArch;
            final String prefix, extension;
            if (os.contains("win") && arch.contains("aarch64")) {
                resName = "windows";
                resArch = "aarch64";
            }
            else if (os.contains("win") && (arch.contains("x86") || arch.contains("amd"))) {
                resName = "windows";
                if (arch.contains("64")) resArch = "x86_64";
                else resArch = "x86";
            }
            else if ((os.contains("mac") || os.contains("osx")) && (arch.contains("x86") || arch.contains("amd")) && arch.contains("64")) {
                resName = "macos";
                resArch = "x86_64";
            }
            else if ((os.contains("mac") || os.contains("osx")) && arch.contains("aarch64")) {
                resName = "macos";
                resArch = "arm64";
            }
            else if ((os.contains("nix") || os.contains("nux")) && (arch.contains("x86") || arch.contains("amd"))) {
                resName = "linux";
                if (arch.contains("64")) resArch = "amd64";
                else resArch = "i386";
            }
            else throw new UnsupportedOperationException("Unsupported platform: " + osName + " " + osArch);
            if (resName.equals("windows")) {
                prefix = "";
                extension = "dll";
            }
            else {
                prefix = "lib";
                if (resName.equals("macos")) {
                    extension = "dylib";
                }
                else extension = "so";
            }
            final String SoundTouchDLL = prefix + "SoundTouchDLL" + "." + extension, soundtouchjni = prefix + "soundtouchjni" + "." + extension;
            try {
                System.loadLibrary(SoundTouchDLL);
                System.loadLibrary(soundtouchjni);
            }
            catch (final UnsatisfiedLinkError e) {
                final String resDir = resName + "-" + resArch;
                load(resDir, SoundTouchDLL);
                load(resDir, soundtouchjni);
            }
        }
    }

    @SuppressWarnings("UnsafeDynamicallyLoadedCode")
    private static void load(final String resDir, final String libname) {
        try (final InputStream inputStream = SoundTouch.class.getResourceAsStream(resDir + "/" + libname);
             final BufferedInputStream source = new BufferedInputStream(Objects.requireNonNull(inputStream))) {
            final File outFile = new File(System.getProperty("java.io.tmpdir"), UUID.randomUUID().toString());
            if (!outFile.exists()) {
                if (!outFile.createNewFile()) throw new IllegalStateException("Cannot create output file!");
            }
            if (!outFile.canWrite()) throw new IllegalStateException("Cannot create output file!");
            try (final FileOutputStream outputStream = new FileOutputStream(outFile);
                 final BufferedOutputStream target = new BufferedOutputStream(outputStream)) {
                final byte[] buffer = new byte[8192];
                int length;
                while ((length = source.read(buffer)) != -1) {
                    target.write(buffer, 0, length);
                }
                System.load(outFile.getAbsolutePath());
            }
            finally {
                outFile.delete();
            }
        }
        catch (final IOException e) {
            throw new RuntimeException(e);
        }
    }

}