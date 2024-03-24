# SoundTouch JNI

JNI bindings for [SoundTouch](https://www.surina.net/soundtouch). Can be used on both Java SE and Android.

SoundTouch version: 2.3.2

## Add the library to your project (gradle)
1. Add the Maven Central repository (if not exist) to your build file:
```groovy
repositories {
    ...
    mavenCentral()
}
```

2. Add the dependency:
```groovy
dependencies {
    ...
    implementation 'com.tianscar.soundtouch:soundtouch-jni-core:1.0.7'    // core
    implementation 'com.tianscar.soundtouch:soundtouch-jni-android:1.0.7' // Android
    implementation 'com.tianscar.soundtouch:soundtouch-jni-javase:1.0.7'  // Java SE
}
```

## Notes
### For Java SE
It is recommended to put the native binaries to the `java.library.path`, instead of extract from the jar at runtime.
If you already put the binaries to `java.library.path`, the jar for Java SE no longer required, and can be removed from the dependencies.

## Usage
[JavaDoc](https://docs.tianscar.com/soundtouch-jni)

## License
[MIT](https://github.com/Tianscar/soundtouchjni/blob/main/LICENSE) (c) Karstian Lee  

### Dependencies
[LGPL-2.1](https://codeberg.org/soundtouch/soundtouch/src/branch/master/COPYING.TXT) [SoundTouch](https://codeberg.org/soundtouch/soundtouch)
