# SoundTouch4Android

The **JNI-packaged SoundTouch** repository which can be used on the **Android** platform.<br/>
The basic functions are **fully encapsulated** and **PCM data can be processed directly**.

JavaDoc: https://tianscar.github.io/SoundTouch4Android/

SoundTouch version: 2.2<br/>
SoundTouch website: http://www.surina.net/soundtouch

# To get a Git project into your build (gradle):

* Step 1. Add the JitPack repository to your build file<br/>
Add it in your root build.gradle at the end of repositories:<br/>
```
allprojects {
        repositories {
		...
		maven { url 'https://jitpack.io' }
	}
}
```

* Step 2. Add the dependency:<br/>
```
dependencies {
	...
	implementation 'com.github.Tianscar:SoundTouch4Android:1.0.3.1'
}
```


# License
[MIT](https://github.com/Tianscar/SoundTouch4Android/blob/master/LICENSE) © Tianscar
[LGPL-2.1](https://github.com/Tianscar/SoundTouch4Android/blob/master/SoundTouch-LICENSE) © Olli Parviainen
