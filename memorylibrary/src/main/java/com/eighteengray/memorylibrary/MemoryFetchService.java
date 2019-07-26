package com.eighteengray.memorylibrary;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;



public class MemoryFetchService extends Service {

    @Override
    public IBinder onBind(Intent intent) {
        return new MemoryFetchStub();
    }

    static class MemoryFetchStub extends IMemoryFIleAidlInterface.Stub{

        @Override
        public void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat, double aDouble, String aString) throws RemoteException {

        }
    }



}
