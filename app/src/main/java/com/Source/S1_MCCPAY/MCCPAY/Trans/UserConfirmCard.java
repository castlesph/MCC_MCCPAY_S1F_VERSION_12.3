package com.Source.S1_MCCPAY.MCCPAY.Trans;

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
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

import java.io.File;

public class UserConfirmCard extends DemoAppActivity {

    String issuerlogo_path;

    TextView textView_txn;
    TextView textView_cardnum;
    TextView textView_expirydate;
    TextView textView_customername;
    TextView textView_total;
    ImageView imageView;
    Button btn_can;
    Button btn_proceed;

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

        setContentView(R.layout.confirm_card_details);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-UserConfirmCard>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent = getIntent();
        String dispmsg = intent.getStringExtra("pass_in_string");
        //Log.i("dispmsg", dispmsg);

        textView_cardnum = (TextView) findViewById(R.id.textView_cardnum);
        textView_expirydate = (TextView) findViewById(R.id.textView_expirydate);
        textView_customername = (TextView) findViewById(R.id.textView_customername);
        textView_total = (TextView) findViewById(R.id.textView_total);
        textView_txn = (TextView) findViewById(R.id.textView_txn);
        imageView = (ImageView) findViewById(R.id.issuer_logo);

        btn_can = (Button) findViewById(R.id.btn_can);
        btn_proceed = (Button) findViewById(R.id.btn_proceed);

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        //System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            //System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 1:
                    textView_cardnum.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    textView_expirydate.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    textView_customername.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textView_total.setText(dispmsginfo[inIdx]);
                    break;
                case 5:
                    issuerlogo_path = "/data/data/pub/"+dispmsginfo[inIdx];
                    File issuerlogo_bmp_file = new File(issuerlogo_path);
                    Bitmap txnlogo = BitmapFactory.decodeFile(issuerlogo_bmp_file.getAbsolutePath());

                    imageView.setImageBitmap(txnlogo);
                    break;
            }
        }

        /*Start timer*/
        getTimerRestart();

        btn_proceed.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "CONFIRM";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(GetAmount.this,MainActivity.class));
                UserConfirmCard.this.finish();

                //Log.i("TINE", "UserConfirmCard Proceed");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        btn_can.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getTimerCancel();

                final_string = "CANCEL";
                //Tine:  try to disable going back to MainActivity
                //startActivity(new Intent(UserConfirmCard.this,MainActivity.class));
                UserConfirmCard.this.finish();

                //Log.i("PATRICK", "Get Amount KeyBoard btn_can");

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
                Log.i("Timer", "Timer onTick");
            }

            @Override
            public void onFinish() {
                //Log.i("Timer", "Timer onFinish");

                //etInputStr.clear();
                final_string = "TIME OUT";

                //startActivity(new Intent(UserConfirmCard.this, MainActivity.class));

                //Log.i("PATRICK", "Timeout UserInputString");
                UserConfirmCard.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }
        };

        @Override
        public void onBackPressed() {
            getTimerCancel();
            startActivity(new Intent(UserConfirmCard.this, MainActivity.class));
            UserConfirmCard.this.finish();

            Log.i("PATRICK", "Back");

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }

        }

        @Override
        protected void onDestroy() {
            this.mWakeLock.release();
            super.onDestroy();
            //final_string = null;
            SysApplication.getInstance().removeActivity(this);
            CloseActivityClass.activityList.remove(this);
        }

    }

