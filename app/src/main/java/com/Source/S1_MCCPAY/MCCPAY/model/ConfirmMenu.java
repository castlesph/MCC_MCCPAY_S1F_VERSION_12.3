package com.Source.S1_MCCPAY.MCCPAY.model;

import android.content.Context;
import android.content.Intent;
import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.support.annotation.RequiresApi;
import android.support.constraint.ConstraintLayout;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;

public class ConfirmMenu extends DemoAppActivity {

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

    ConstraintLayout clayout_button;

    Double dAmount;
    int inResult;
    String stResult;

    boolean androidThinking;
    public static String final_string;
    public static String input_type;

    private int isButtonVisible = 0;

    protected PowerManager.WakeLock mWakeLock;

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
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

        setContentView(R.layout.confirm_menu);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-ConfirmMenu>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("saturn dispmsg", dispmsg);

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

        clayout_button = (ConstraintLayout) findViewById(R.id.clayout_button);

        String[] dispmsginfo = dispmsg.split("\\|");
		int msgcnt = dispmsginfo.length;

		System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0: // Check for button visibility
                    if (dispmsginfo[inIdx].compareTo("1") == 0) {
                        isButtonVisible = 1;
                        clayout_button.setVisibility(View.VISIBLE);
                    }
                    else {
                        isButtonVisible = 0;
                        clayout_button.setVisibility(View.INVISIBLE);
                    }
                    break;
                case 1: // Issuer Name
                    if (dispmsginfo[inIdx].toLowerCase().compareTo("wechat") == 0)
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.wechat));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("alipay") == 0)
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.alipay));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("gcash") == 0)
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.gcash));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("grab pay") == 0)
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.grabpay));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("upi") == 0)
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.upipay));
					else if (dispmsginfo[inIdx].toLowerCase().compareTo("phqr") == 0)
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.phqr));
                    else
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.icon_none));
                    break;
                case 2:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    textView01.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textViewCount01.setText(dispmsginfo[inIdx]);
                    break;
                case 5:
                    textViewDetail01.setText(dispmsginfo[inIdx]);
                    break;
                case 6:
                    textView02.setText(dispmsginfo[inIdx]);
                    break;
                case 7:
                    textViewCount02.setText(dispmsginfo[inIdx]);
                    break;
                case 8:
                    textViewDetail02.setText(dispmsginfo[inIdx]);
                    break;
                case 9:
                    textView03.setText(dispmsginfo[inIdx]);
                    textView03.setTypeface(null, Typeface.BOLD);
                    break;
                case 10:
                    textViewCount03.setText(dispmsginfo[inIdx]);
                    textViewCount03.setTypeface(null, Typeface.BOLD);
                    break;
                case 11:
                    textViewDetail03.setText(dispmsginfo[inIdx]);
                    textViewDetail03.setTypeface(null, Typeface.BOLD);
                    break;
            }
        }

        buOK = (Button) findViewById(R.id.IPT_OKButton);
        buCancel = (Button) findViewById(R.id.IPT_CancelButton);

        // sidumili: added checking
        ConstraintLayout.LayoutParams params = (ConstraintLayout.LayoutParams) buOK.getLayoutParams(); // cancel button
        ConstraintLayout.LayoutParams params1 = (ConstraintLayout.LayoutParams) buCancel.getLayoutParams(); // ok button

        String sTitle = (String) textView_txn.getText();
        Log.i("sidumili", "ConfirmMenu: sTitle="+sTitle);

        if (isButtonVisible > 0)
        {
            if (sTitle.compareTo("SETTLEMENT") == 0)
            {
                params.width = 330;
                buOK.setLayoutParams(params);

                params1.width = 330;
                buCancel.setLayoutParams(params1);
            }
            else
            {
                buCancel.setVisibility(View.INVISIBLE);
                params.width = 680;
                buOK.setLayoutParams(params);
            }
        }

        Log.i("sidumili", "isButtonVisible=" + isButtonVisible + ",inTimeOut:="+inTimeOut);

        /*Start timer*/
        getTimerRestart();

        buOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "CONFIRM";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(GetAmount.this,MainActivity.class));
                ConfirmMenu.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buOK");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        buCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string = "CANCEL";
                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                ConfirmMenu.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buCancel");

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

        if (isButtonVisible > 0) // sidumili: button is visible
            timer.cancel();
        else
            timer2.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart()
    {
        if (isButtonVisible > 0) // sidumili: button is visible
            timer.start();
        else
            timer2.start();
    }

    // sidumili: display for 30secs
    private int inTimeOut = 30;
    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "saturn Timer onTick ConfirmMenu, inTimeOut="+inTimeOut);
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "saturn Timer onFinish ConfirmMenu, inTimeOut="+inTimeOut);

            //etInputStr.clear();

            onTimeOut(); // sidumili: timeout occured
        }
    };

    // sidumili: display for 3secs, applied in batch total display during autosettlement
    private int inTimeOut2 = 3;
    private CountDownTimer timer2 = new CountDownTimer(inTimeOut2*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "saturn Timer onTick ConfirmMenu, inTimeOut2="+inTimeOut2);
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "saturn Timer onFinish ConfirmMenu, inTimeOut2="+inTimeOut2);

            //etInputStr.clear();

            onTimeOut(); // sidumili: timeout occured
        }
    };

    private void onTimeOut()
    {
        if (isButtonVisible == 0)
            final_string = "CONFIRM";
        else
            final_string = "TIMEOUT";

        //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

        Log.i("PATRICK", "Timeout saturn ConfirmMenu");
		
		getTimerCancel();
        ConfirmMenu.this.finish();

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }
    }

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

