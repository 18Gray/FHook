// IMemoryFIleAidlInterface.aidl
package com.eighteengray.memorylibrary;
import android.os.ParcelFileDescriptor;
// Declare any non-default types here with import statements

interface IMemoryFIleAidlInterface {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
    void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,
            double aDouble, String aString);

    ParcelFileDescriptor getParcelFileDescriptor();

}
