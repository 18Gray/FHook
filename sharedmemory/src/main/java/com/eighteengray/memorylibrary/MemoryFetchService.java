package com.eighteengray.memorylibrary;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.MemoryFile;
import android.os.ParcelFileDescriptor;
import android.os.RemoteException;

import java.io.FileDescriptor;
import java.lang.reflect.Method;


public class MemoryFetchService extends Service {

    @Override
    public IBinder onBind(Intent intent) {
        return new MemoryFetchStub();
    }

    static class MemoryFetchStub extends IMemoryFIleAidlInterface.Stub{

        @Override
        public void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat, double aDouble, String aString) throws RemoteException {

        }

        @Override
        public ParcelFileDescriptor getParcelFileDescriptor() throws RemoteException {
            MemoryFile memoryFile = null;
            try {
                memoryFile = new MemoryFile("test_memory", 1024);
                memoryFile.getOutputStream().write(new byte[]{1, 2, 3, 4, 5});
                Method method = MemoryFile.class.getDeclaredMethod("getFileDescriptor");
                FileDescriptor des = (FileDescriptor) method.invoke(memoryFile);
                return ParcelFileDescriptor.dup(des);
            } catch (Exception e) {

            }
            return null;
        }
    }



}
