package com.eighteengray.fhook;

import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.ParcelFileDescriptor;
import android.support.annotation.Nullable;
import android.support.v4.app.FragmentActivity;
import android.view.View;

import com.eighteengray.memorylibrary.IMemoryFIleAidlInterface;
import com.eighteengray.memorylibrary.MemoryFetchService;

import java.io.FileDescriptor;
import java.io.FileInputStream;


public class TestActivity extends FragmentActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        findViewById(R.id.sample_text).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(TestActivity.this, MemoryFetchService.class);
                bindService(intent, new ServiceConnection() {
                    @Override
                    public void onServiceConnected(ComponentName name, IBinder service) {
                        byte[] content = new byte[10];
                        IMemoryFIleAidlInterface iMemoryAidlInterface
                                = IMemoryFIleAidlInterface.Stub.asInterface(service);
                        try {
                            ParcelFileDescriptor parcelFileDescriptor = iMemoryAidlInterface.getParcelFileDescriptor();
                            FileDescriptor descriptor = parcelFileDescriptor.getFileDescriptor();
                            FileInputStream fileInputStream = new FileInputStream(descriptor);
                            fileInputStream.read(content);
                        } catch (Exception e) {
                        }}

                    @Override
                    public void onServiceDisconnected(ComponentName name) {

                    }
                }, Service.BIND_AUTO_CREATE);

            }
        });

    }
}
