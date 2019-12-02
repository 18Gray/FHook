package com.eighteengray.elfhook;

import android.app.Activity;
import android.os.Bundle;
import com.eighteengray.nativehooklibrary.R;


public class XHookActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        XHook.getInstance().init(this.getApplicationContext());
        if(!XHook.getInstance().isInited()) {
            return;
        }

        XHookApply.getInstance().init();
        XHookApply.getInstance().start();

        XHook.getInstance().refresh(false);

    }


}
