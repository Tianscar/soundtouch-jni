# SoundTouch4Android

The JNI-packaged SoundTouch repository which can be used on the Android platform.

The basic functionality is fully encapsulated and PCM data can be processed directly.

SoundTouch version: 2.2

SoundTouch WWW: http://www.surina.net/soundtouch

To get a Git project into your build (gradle):

Step 1. Add the JitPack repository to your build file
Add it in your root build.gradle at the end of repositories:
allprojects {
	repositories {
		...
		maven { url 'https://jitpack.io' }
	}
}

Step 2. Add the dependency
dependencies {
	...
	implementation 'com.github.Tianscar:SoundTouch4Android:1.0.2'
}
