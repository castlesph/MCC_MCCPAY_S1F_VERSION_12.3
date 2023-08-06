package com.Source.S1_MCCPAY.MCCPAY.model;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;

public class ConfirmMenu2 extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    EditText etInputStr;
    Button buOK;
    Button buCancel;

    TextView textView01;
    TextView textViewCount01;
    TextView textViewDetail01;

    TextView textView02;
    TextView textViewCount02;
    TextView textViewDetail02;

    TextView textView03;
    TextView textViewCount03;
    TextView textViewDetail03;

    TextView textView_txn;

    TextView textView_cur;

    Double dAmount;
    int inResult;
    String stResult;

    boolean androidThinking;
    public static String final_string;
    public static String input_type;

    private int inTimeOut = 1;

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

        setContentView(R.layout.confirm_menu2);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:ConfirmMenu2>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        textView_txn = (TextView) findViewById(R.id.textView_txn);
        textView_cur = (TextView) findViewById(R.id.textView_cur);

        textView01 = (TextView) findViewById(R.id.msg_text_01);
        textViewCount01 = (TextView) findViewById(R.id.msg_count_01);
        textViewDetail01 = (TextView) findViewById(R.id.msg_detail_01);

        textView02 = (TextView) findViewById(R.id.msg_text_02);
        textViewCount02 = (TextView) findViewById(R.id.msg_count_02);
        textViewDetail02 = (TextView) findViewById(R.id.msg_detail_02);

        textView03 = (TextView) findViewById(R.id.msg_text_03);
        textViewCount03 = (TextView) findViewById(R.id.msg_count_03);
        textViewDetail03 = (TextView) findViewById(R.id.msg_detail_03);

        String[] dispmsginfo = dispmsg.split("\\|");
		int msgcnt = dispmsginfo.length;

		System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 1:
                    textView01.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    textViewCount01.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    textViewDetail01.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textView02.setText(dispmsginfo[inIdx]);
                    break;
                case 5:
                    textViewCount02.setText(dispmsginfo[inIdx]);
                    break;
                case 6:
                    textViewDetail02.setText(dispmsginfo[inIdx]);
                    break;
                case 7:
                    textView03.setText(dispmsginfo[inIdx]);
                    break;
                case 8:
                    textViewCount03.setText(dispmsginfo[inIdx]);
                    break;
                case 9:
                    textViewDetail03.setText(dispmsginfo[inIdx]);
                    break;

            }
        }

		Log.i("saturn gettimer", "saturn before get timer");


        /*Start timer*/
        getTimerRestart();
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
            Log.i("saturn Timer", "saturn Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "Timer onFinish");
            //etInputStr.clear();
            
			getTimerCancel();
            final_string = "TIME OUT";

            //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

            Log.i("saturn PATRICK", "saturn Timeout UserInputString");
            ConfirmMenu2.this.finish();

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
        mContext = null;
        SysApplication.getInstance().removeActivity(this);
        CloseActivityClass.activityList.remove(this);
    }

}

