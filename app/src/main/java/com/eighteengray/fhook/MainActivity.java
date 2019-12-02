package com.eighteengray.fhook;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.eighteengray.elfhook.XHookActivity;

public class MainActivity extends Activity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText("点击进入HookActivity");
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                /*Intent intent = new Intent(MainActivity.this, TestActivity.class);
                startActivity(intent);*/

                Intent intent = new Intent(MainActivity.this, XHookActivity.class);
                startActivity(intent);
            }
        });
    }

}
