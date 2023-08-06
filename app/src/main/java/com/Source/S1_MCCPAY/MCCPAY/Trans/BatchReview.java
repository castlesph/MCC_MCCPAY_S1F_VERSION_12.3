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
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

public class BatchReview extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    Button btn_prev;
    Button btn_next;
    Button btn_can;

	TextView batchreview_title;
	TextView textView_cur;
	TextView batchreview_datetime;
    TextView batchreview_pan;
    TextView batchreview_amount;
    TextView batchreview_inv;
    TextView textView_txn;

    public static String final_string;

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

        setContentView(R.layout.batchreview_ui);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-BatchReview>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        textView_txn = (TextView) findViewById(R.id.textView_txn);
        textView_cur = (TextView) findViewById(R.id.textView_cur);
        batchreview_title = (TextView) findViewById(R.id.batchreview_title);
        batchreview_datetime = (TextView) findViewById(R.id.batchreview_datetime);
        batchreview_pan = (TextView) findViewById(R.id.batchreview_pan);
        batchreview_amount = (TextView) findViewById(R.id.batchreview_amount);
        batchreview_inv = (TextView) findViewById(R.id.batchreview_inv);

        btn_can = (Button) findViewById(R.id.btn_can);
        btn_next = (Button) findViewById(R.id.btn_next);
        btn_prev = (Button) findViewById(R.id.btn_prev);

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0: // Trans Title
					textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 1: // Trans Type
                    batchreview_title.setText(dispmsginfo[inIdx]);
                    break;
				case 2: // Issuer
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
                    else
                        textView_cur.setBackground(getResources().getDrawable(R.drawable.icon_none));
					break;
                case 3: // DateTime
                    batchreview_datetime.setText(dispmsginfo[inIdx]);
                    break;
                case 4: // Order ID
                    batchreview_pan.setText(dispmsginfo[inIdx]);
                    break;
                case 5: // Amount
                    batchreview_amount.setText(dispmsginfo[inIdx]);
                    break;
                case 6: // InvoiceNo
                    batchreview_inv.setText(dispmsginfo[inIdx]);
                    break;
                case 7: // sidumili: Record navigation result
                    int inNavRecType = Integer.parseInt(dispmsginfo[inIdx]);
                    switch (inNavRecType)
                    {
                        case 0: // Beginning of Record
                            // do nothing...
                            break;
                        case 1: // First of Record
                            Toast.makeText(getApplicationContext(), "First of record", Toast.LENGTH_SHORT).show();
                            break;
                        case 2: // End of Record
                            Toast.makeText(getApplicationContext(), "End of record", Toast.LENGTH_SHORT).show();
                            break;
                    }
                    break;
            }
        }

        /*Start timer*/
        getTimerRestart();

        btn_prev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "PREVIOUS";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                BatchReview.this.finish();

                Log.i("TINE", "PREVIOUS BUTTON");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        btn_next.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "NEXT";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                BatchReview.this.finish();

                Log.i("TINE", "NEXT BUTTON");

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

                /*cancel timer first*/
                getTimerCancel();

                final_string = "CANCEL";

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                BatchReview.this.finish();

                Log.i("TINE", "EXIT BUTTON");

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
            Log.i("BatchReview Timer", "Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "BatchReview Timer onFinish");

            //etInputStr.clear();
            final_string = "TIME OUT";

            //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

            Log.i("PATRICK", "Timeout UserInputString");
            BatchReview.this.finish();

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
