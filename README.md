# SoundTouch JNI

JNI bindings for [SoundTouch](https://www.surina.net/soundtouch). Can be used on both Java SE and Android.

SoundTouch version: 2.3.1

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
    implementation 'com.tianscar.soundtouch:soundtouch-jni-android:1.0.5' // Android
    implementation 'com.tianscar.soundtouch:soundtouch-jni-javase:1.0.5'  // Java SE
}
```

## Usage
[JavaDoc](https://docs.tianscar.com/soundtouch-jni)

## License
[MIT](https://github.com/Tianscar/soundtouchjni/blob/main/LICENSE) (c) Tianscar  

### Dependencies
[SoundTouch](https://codeberg.org/soundtouch/soundtouch) - [LGPL-2.1](https://codeberg.org/soundtouch/soundtouch/src/branch/master/COPYING.TXT) (c) Olli Parviainen
