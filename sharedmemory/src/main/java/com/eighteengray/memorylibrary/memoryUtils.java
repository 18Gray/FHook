package com.eighteengray.memorylibrary;

public class memoryUtils {

    static {
        System.loadLibrary("native-lib");
    }

    public native String stringFromJNI();
}
