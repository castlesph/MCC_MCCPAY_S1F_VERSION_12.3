package com.Source.S1_MCCPAY.MCCPAY.model;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;


import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;

public class UpDownMenu extends DemoAppActivity {

    Button button_up;
    Button button_down;
    Button button_exit;

    TextView textView01;
    TextView textView02;
    TextView textView03;
    TextView textView04;
    TextView textView05;
    TextView textView06;
    TextView textView07;
    TextView textView08;

    public static String final_string;


    private int inTimeOut = 60*8;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
       
        super.onCreate(savedInstanceState);

        overridePendingTransition(0, 0); // disable the animation, faster

        setContentView(R.layout.updown_menu);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-UpDownMenu>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);
       
        textView01 = (TextView) findViewById(R.id.msg_text_01);
        textView02 = (TextView) findViewById(R.id.msg_text_02);
        textView03 = (TextView) findViewById(R.id.msg_text_03);
        textView04 = (TextView) findViewById(R.id.msg_text_04);
        textView05 = (TextView) findViewById(R.id.msg_text_05);
        textView06 = (TextView) findViewById(R.id.msg_text_06);
        textView07 = (TextView) findViewById(R.id.msg_text_07);
        textView08 = (TextView) findViewById(R.id.msg_text_08);

        String[] dispmsginfo = dispmsg.split("\\|");
		int msgcnt = dispmsginfo.length;

		System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    textView01.setText(dispmsginfo[inIdx]);
                    break;
                case 1:
                    textView02.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    textView03.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    textView04.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textView05.setText(dispmsginfo[inIdx]);
                    break;
                case 5:
                    textView06.setText(dispmsginfo[inIdx]);
                    break;
                case 6:
                    textView07.setText(dispmsginfo[inIdx]);
                    break;
                case 7:
                    textView08.setText(dispmsginfo[inIdx]);
                    break;
            }
        }

        button_down = (Button) findViewById(R.id.button_down);
        button_up = (Button) findViewById(R.id.button_up);
        button_exit = (Button) findViewById(R.id.button_exit);
        /*Start timer*/
        getTimerRestart();


        button_down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string = "DOWN";

                UpDownMenu.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        button_up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string = "UP";
                //startActivity(new Intent(UpDownMenu.this,MainActivity.class));
                UpDownMenu.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

        button_exit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string = "EXIT";
                //startActivity(new Intent(UpDownMenu.this,MainActivity.class));
                UpDownMenu.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

    }

    /**
     * 取消倒计时
     */
    public void getTimerCancel() {
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
            Log.i("Timer", "Timer onFinish");

            final_string = "TO";

            UpDownMenu.this.finish();

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

		if (timer != null) {
			timer.cancel();
			timer = null;
		}
		
        final_string = null;
        SysApplication.getInstance().removeActivity(this);
    }

}

