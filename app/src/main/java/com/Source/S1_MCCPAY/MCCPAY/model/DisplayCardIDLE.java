package com.Source.S1_MCCPAY.MCCPAY.model;


import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;

import pl.droidsonroids.gif.GifDrawable;
import pl.droidsonroids.gif.GifImageView;

public class DisplayCardIDLE extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    EditText etInputStr;


    TextView textView01;
    TextView textView02;
    TextView textView03;




    Double dAmount;
    int inResult;
    String stResult;

    boolean androidThinking;
    public static String final_string;
    public static String input_type;

    private int inTimeOut = 60*60*24*365;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

        setContentView(R.layout.display_cardidle);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-DisplayCardIDLE");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        // Show status bar
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        GifImageView gifImageView1 = (GifImageView) findViewById(R.id.gif1);
        try {// 如果加载的是gif动图，第一步需要先将gif动图资源转化为GifDrawable
            // 将gif图资源转化为GifDrawable
            GifDrawable gifDrawable = new GifDrawable(getResources(), R.drawable.cardidle);// gif1加载一个动态图gif
            gifImageView1.setImageDrawable(gifDrawable);
            // 如果是普通的图片资源，就像Android的ImageView set图片资源一样简单设置进去即可。
        } catch(Exception e){
            e.printStackTrace();
        }


        //textView01 = (TextView) findViewById(R.id.msg_text_01);
        //textView02 = (TextView) findViewById(R.id.msg_text_02);
        //textView03 = (TextView) findViewById(R.id.msg_text_03);


        String[] dispmsginfo = dispmsg.split("\\|");
		int msgcnt = dispmsginfo.length;

		System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
//                case 0:
//                    textView01.setText(dispmsginfo[inIdx]);
//                    break;
//                case 1:
//                    textView02.setText(dispmsginfo[inIdx]);
//                    break;
//                case 2:
//                    textView03.setText(dispmsginfo[inIdx]);
//                    break;
                 default:
                     break;
            }
        }

        /*Start timer*/
        //getTimerRestart();

//
//        buOK.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//
//                /*cancel timer first*/
//                getTimerCancel();
//
//
//                final_string = "CONFIRM";
//
//
//                DisplayCardIDLE.this.finish();
//
//                Log.i("PATRICK", "Get Amount KeyBoard buOK");
//
//                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
//                synchronized (MainActivity.LOCK) {
//                    MainActivity.LOCK.setCondition(true);
//                    MainActivity.LOCK.notifyAll();
//
//                }
//
//            }
//        });
//
//        buCancel.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//
//                /*cancel timer first*/
//                getTimerCancel();
//
//                final_string = "CANCEL";
//                startActivity(new Intent(DisplayCardIDLE.this,MainActivity.class));
//                DisplayCardIDLE.this.finish();
//
//                Log.i("PATRICK", "Get Amount KeyBoard buCancel");
//
//                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
//                synchronized (MainActivity.LOCK) {
//                    MainActivity.LOCK.setCondition(true);
//                    MainActivity.LOCK.notifyAll();
//
//                }
//            }
//        });
        //finish();
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
            //startBeepTone("beep_beep_beep");
            //MainActivity.CTOS_Beep();
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "Timer onFinish");


            final_string = "TO";

            startActivity(new Intent(DisplayCardIDLE.this, MainActivity.class));

            Log.i("PATRICK", "Timeout UserInputString");
            DisplayCardIDLE.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
//            synchronized (MainActivity.LOCK) {
//                MainActivity.LOCK.setCondition(true);
//                MainActivity.LOCK.notifyAll();
//            }
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
        mContext = null;
        SysApplication.getInstance().removeActivity(this);

    }

    public String startBeepTone(String beepStr) {
        Log.i("beepStr", beepStr);

        return "OK";
    }

}

