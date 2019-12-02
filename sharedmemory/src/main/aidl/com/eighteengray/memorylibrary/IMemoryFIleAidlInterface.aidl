package com.eighteengray.memorylibrary;
import android.os.ParcelFileDescriptor;

interface IMemoryFIleAidlInterface {
    void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,
            double aDouble, String aString);

    ParcelFileDescriptor getParcelFileDescriptor();

}
