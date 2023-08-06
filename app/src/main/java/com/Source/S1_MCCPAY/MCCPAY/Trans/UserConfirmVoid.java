package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.support.annotation.RequiresApi;
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
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

public class UserConfirmVoid extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    EditText etInputStr;
    Button buOK;
    Button buCancel;


    TextView textView_txn;
    TextView msg_01, msg_02, msg_03, msg_05, msg_06, msg_07, msg_08;
    TextView msg_detail_01, msg_detail_02, msg_detail_03, msg_detail_05, msg_detail_06, msg_detail_07, msg_detail_08;

    Double dAmount;
    public static String final_string;
    public static String input_type;

    private int inTimeOut = 30;

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

        setContentView(R.layout.confirm_void);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-UserConfirmVoid>>UserConfirmVoid");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        textView_txn = (TextView) findViewById(R.id.textView_txn);
        msg_01 = (TextView) findViewById(R.id.msg_01);
        msg_02 = (TextView) findViewById(R.id.msg_02);
        msg_03 = (TextView) findViewById(R.id.msg_03);
        msg_05 = (TextView) findViewById(R.id.msg_05);
        msg_06 = (TextView) findViewById(R.id.msg_06);
        msg_07 = (TextView) findViewById(R.id.msg_07);
        msg_08 = (TextView) findViewById(R.id.msg_08);

        msg_detail_01 = (TextView) findViewById(R.id.msg_detail_01);
        msg_detail_02 = (TextView) findViewById(R.id.msg_detail_02);
        msg_detail_03 = (TextView) findViewById(R.id.msg_detail_03);
        msg_detail_05 = (TextView) findViewById(R.id.msg_detail_05);
        msg_detail_06 = (TextView) findViewById(R.id.msg_detail_06);
        msg_detail_07 = (TextView) findViewById(R.id.msg_detail_07);
        msg_detail_08 = (TextView) findViewById(R.id.msg_detail_08);

        msg_01.setText("Host:");
        msg_02.setText("Transaction Type:");
        msg_03.setText("Amount:");
        msg_05.setText("Conversion:");
        msg_06.setText("Order ID:");
        msg_07.setText("Trace Number:");
        msg_08.setText("Date / Time:");

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    textView_txn.setText(dispmsginfo[inIdx]); // Trxn Name
                    break;
                case 1:
                    msg_detail_02.setText(dispmsginfo[inIdx]); // Trans Type

                case 2:
                    msg_detail_03.setText(dispmsginfo[inIdx]); // Amount
                    break;
                case 3:
                    msg_detail_05.setText(dispmsginfo[inIdx]); // Conversion Amt
                    break;
                case 4:
                    msg_detail_06.setText(dispmsginfo[inIdx]); // Order ID
                    break;
                case 5:
                    msg_detail_07.setText(dispmsginfo[inIdx]); // Invoice No
                    break;
                case 6:
                    msg_detail_08.setText(dispmsginfo[inIdx]); // Date / Time
                    break;
                case 7:
                    if (dispmsginfo[inIdx].toLowerCase().compareTo("wechat") == 0)
                        msg_detail_01.setBackground(getResources().getDrawable(R.drawable.wechat));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("alipay") == 0)
                        msg_detail_01.setBackground(getResources().getDrawable(R.drawable.alipay));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("gcash") == 0)
                        msg_detail_01.setBackground(getResources().getDrawable(R.drawable.gcash));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("grab pay") == 0)
                        msg_detail_01.setBackground(getResources().getDrawable(R.drawable.grabpay));
                    else if (dispmsginfo[inIdx].toLowerCase().compareTo("upi") == 0)
                        msg_detail_01.setBackground(getResources().getDrawable(R.drawable.upipay));
                    else
                        msg_detail_01.setBackground(getResources().getDrawable(R.drawable.icon_none));
                    break;
            }
        }

        buOK = (Button) findViewById(R.id.IPT_OKButton);
        buCancel = (Button) findViewById(R.id.IPT_CancelButton);

        /*Start timer*/
        getTimerRestart();

//

        buOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "CONFIRM";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                UserConfirmVoid.this.finish();

                Log.i("UserConfirmVoid", "Button buOK");

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
                UserConfirmVoid.this.finish();

                Log.i("UserConfirmVoid", "Button buCancel");

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

            //etInputStr.clear();
            final_string = "TIME OUT";

            //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

            Log.i("UserConfirmVoid", "Timeout");
            UserConfirmVoid.this.finish();

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
    }

}
