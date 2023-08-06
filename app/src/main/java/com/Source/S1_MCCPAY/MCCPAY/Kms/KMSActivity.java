package com.Source.S1_MCCPAY.MCCPAY.Kms;
import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.os.HandlerThread;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.widget.EditText;

import com.Source.S1_MCCPAY.MCCPAY.R;


import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class KMSActivity extends AppCompatActivity {
    @Override
    public FileInputStream openFileInput(String name) throws FileNotFoundException {
        return super.openFileInput(name);
    }

    private static final String TAG_A = "KMS";
    private static final String TAG_C = "KMSActivity";

    private PINPADFIXED MovingEncrythread;
    private PINPADDUKPT ScramblingEncrythread;
    int inKeySet = 0;
    int inKeyIndex= 0;
    String StrInData= "";
    public static String final_string;
    public static String PinBlock;
    public static String KSN;

    Activity activity = this;
    EditText PinDigitET;


    private Handler background_ON_Handler;
    private HandlerThread background_ON_Thread;




    private Runnable background_on = new Runnable () {
        public void run() {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    PinDigitET.setEnabled(false);
                    PinDigitET.setAlpha(0.0f);
                }
            });
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_kms);

        Intent intent=getIntent();
        inKeySet=intent.getIntExtra("KeySet", 0);
        inKeyIndex=intent.getIntExtra("KeyIndex", 0);
        StrInData=intent.getStringExtra("StrInData");
        Log.i("inKeySet", inKeySet+"");
        Log.i("inKeyIndex", inKeyIndex+"");
        Log.i("StrInData", StrInData);

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        PinDigitET = (EditText) findViewById(R.id.PinDigitEditText);






        PinDigitET.setEnabled(false);
        PinDigitET.setAlpha(0.0f);

        background_ON_Thread = new HandlerThread("background_on");
        background_ON_Thread.start();
        background_ON_Handler = new Handler (background_ON_Thread.getLooper());
    }
    @Override
    protected void onStart() {
        super.onStart();


    }
    /** Called when the activity has become visible. */
    @Override
    protected void onResume() {
        super.onResume();
        background_ON_Handler.post(background_on);
    }

}
