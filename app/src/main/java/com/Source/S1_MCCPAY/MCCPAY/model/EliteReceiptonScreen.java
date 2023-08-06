package com.Source.S1_MCCPAY.MCCPAY.model;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;

import java.io.File;


public class EliteReceiptonScreen extends DemoAppActivity {

    Button btn_proceed;
    ImageView imageView_receipt_bmp;
    ImageView imageView_receipt_logo;

    public static String final_string;

    private int inTimeOut = 30;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        //| View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        //| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        //| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        //| View.SYSTEM_UI_FLAG_FULLSCREEN);

        setContentView(R.layout.elite_receipt);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:EliteReceiptonScreen>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent = getIntent();
        String dispmsg = intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        //textView01 = (TextView) findViewById(R.id.textView_issuer);
        //textView02 = (TextView) findViewById(R.id.textView_title);
        //textView03 = (TextView) findViewById(R.id.textView_PAN);
        //textView04 = (TextView) findViewById(R.id.textView_cardnum);
        //imageView = (ImageView) findViewById(R.id.issuer_logo);

        btn_proceed = (Button) findViewById(R.id.btn_proceed);


        File receipt_bmp_file = new File("/data/data/pub/Print_BMP.bmp");

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    //textView_txn.setText(dispmsginfo[inIdx]);
                    File logo_bmp_file = new File(dispmsginfo[inIdx]);
                    Bitmap mylogo = BitmapFactory.decodeFile(logo_bmp_file.getAbsolutePath());
                    imageView_receipt_logo = (ImageView) findViewById(R.id.receipt_logo);
                    imageView_receipt_logo.setImageBitmap(mylogo);
                    break;
                case 1:
                    //textView02.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    //textView03.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    //textView04.setText(dispmsginfo[inIdx]);
                    break;
            }
        }

        getTimerRestart();

        Bitmap myBitmap = BitmapFactory.decodeFile(receipt_bmp_file.getAbsolutePath());
        imageView_receipt_bmp = (ImageView) findViewById(R.id.receipt_bmp);
        imageView_receipt_bmp.setImageBitmap(myBitmap);


        btn_proceed.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "CONFIRM";
                EliteReceiptonScreen.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

    }

    public void getTimerCancel(){
        timer.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart()
    {
        timer.start();
    }

    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "EliteReceiptonScreen Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "EliteReceiptonScreen Timer onFinish");

            //etInputStr.clear();
            final_string = "TO";

            startActivity(new Intent(EliteReceiptonScreen.this, MainActivity.class));

            Log.i("TINE", "Timeout PrintFirstReceiptonScreen");
            EliteReceiptonScreen.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        final_string = null;
        SysApplication.getInstance().removeActivity(this);
        CloseActivityClass.activityList.remove(this);
    }

}

