package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.EditDatabaseActivity;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

// sidumili: Display selected item from recyclerview
public class ProcessTransDetailUI extends DemoAppActivity {

    Context context;
    TextView textView1, textView2, textView3, textView4, textView5, textView6;
    Button btn_proceed;
    Button btn_can;
    ImageView myImageView;

    public static String final_string = "";
    public static int final_rb_selected = 0;
    public static String final_rb_selected_string = "";
    public static String final_selected_value = "";

    private int inTimeOut = 20;

    String data1, data2, data3, data4, data5, data6, data7, data8;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

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

        setContentView(R.layout.confirm_retrieve_transdetail);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-ProcessTransDetailUI>>WAKE_LOCK");
        this.mWakeLock.acquire();

        myImageView = (ImageView) findViewById(R.id.myImageView);
        textView1 = (TextView) findViewById(R.id.textView1);
        textView2 = (TextView) findViewById(R.id.textView2);
        textView3 = (TextView) findViewById(R.id.textView3);
        textView4 = (TextView) findViewById(R.id.textView4);
        textView5 = (TextView) findViewById(R.id.textView5);
        textView6 = (TextView) findViewById(R.id.textView6);
        btn_can = (Button) findViewById(R.id.btn_can);
        btn_proceed = (Button) findViewById(R.id.btn_proceed);


        getData();
        setData();

        /*Start timer*/
        //getTimerRestart();

        btn_proceed.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Log.i("sidumili", "ProceedButton Before final_rb_selected=" + final_rb_selected + ",final_rb_selected_string=" + final_rb_selected_string);
                if (final_rb_selected > 0)
                {
                    //getTimerCancel();

                    final_string = "CONFIRM";

                    Log.i("sidumili", "ProceedButton final_string=" + final_string);
                    Log.i("sidumili", "ProceedButton final_rb_selected=" + final_rb_selected + ",final_rb_selected_string=" + final_rb_selected_string);
                    Log.i("sidumili", "ProceedButton final_selected_value=" + final_selected_value);

                    ProcessTransDetailUI.this.finish();
                    Intent intent = new Intent(context, MainActivity.class);
                    context.startActivity(intent);

                    //Intent intent = new Intent(getBaseContext(), MainActivity.class);
                    //context.startActivity(intent);
                    //ProcessTransDetailUI.this.finish();

                    //startActivity(new Intent(ProcessTransDetailUI.this,MainActivity.class));
                    //ProcessTransDetailUI.this.finish();

                    //TransDetailListView cls = new TransDetailListView();
                    //cls.checkProcess();

                    /*
                    // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                    MainActivity.LOCK.setCondition(false);
                    synchronized (MainActivity.LOCK) {
                        MainActivity.LOCK.setCondition(true);
                        MainActivity.LOCK.notifyAll();


                    }
                    */


                    /*
                    TransDetailListView.LOCK.setCondition(false);
                    // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                    synchronized (TransDetailListView.LOCK) {
                        TransDetailListView.LOCK.setCondition(true);
                        TransDetailListView.LOCK.notifyAll();
                        TransDetailListView.LOCK.checkProcess();
                    }
                     */
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "No transaction selected", Toast.LENGTH_SHORT).show();
                }
            }
        });

        btn_can.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //getTimerCancel();

                final_string = "CANCEL";

                //Tine:  try to disable going back to MainActivity
                ProcessTransDetailUI.this.finish();

                Log.i("PATRICK", "UserCancelUI btn_userCan");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (TransDetailListView.LOCK) {
                    TransDetailListView.LOCK.setCondition(true);
                    TransDetailListView.LOCK.notifyAll();
                }
            }

        });

    }

    private void getData()
    {
        if (getIntent().hasExtra("data1") && getIntent().hasExtra("data2") && getIntent().hasExtra("data3")
                && getIntent().hasExtra("data4") && getIntent().hasExtra("data5") && getIntent().hasExtra("data6")
                && getIntent().hasExtra("data7") && getIntent().hasExtra("data8"))
        {
            data1 = getIntent().getStringExtra("data1");
            data2 = getIntent().getStringExtra("data2");
            data3 = getIntent().getStringExtra("data3");
            data4 = getIntent().getStringExtra("data4");
            data5 = getIntent().getStringExtra("data5");
            data6 = getIntent().getStringExtra("data6");
            data7 = getIntent().getStringExtra("data7");
            data8 = getIntent().getStringExtra("data8");

            Log.i("sidumili", "getData: data1="+data1);
            Log.i("sidumili", "getData: data2="+data2);
            Log.i("sidumili", "getData: data3="+data3);
            Log.i("sidumili", "getData: data4="+data4);
            Log.i("sidumili", "getData: data5="+data5);
            Log.i("sidumili", "getData: data6="+data6);
            Log.i("sidumili", "getData: data7="+data7);
            Log.i("sidumili", "getData: data8="+data8);
        }
        else
        {
            Toast.makeText(this, "No data", Toast.LENGTH_SHORT).show();
        }
    }

    private void setData()
    {
        textView1.setText(data1);
        textView2.setText(data2);
        textView3.setText(data3);
        textView4.setText(data4);
        textView5.setText(data5);
        textView6.setText(data6);

        int iIndex = Integer.parseInt(data8) - 1; // start with index 0
        myImageView.setImageResource(TransDetailListView.images[iIndex]); // access public variable

        // Set Selected
        final_selected_value = textView3.getText().toString();

        final_rb_selected = data3.length();

        //
        final_rb_selected_string = "X";
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
            Log.i("Timer", "Timer onFinish");

            //etInputStr.clear();
            final_string = "TO";

            startActivity(new Intent(ProcessTransDetailUI.this, TransDetailListView.class));

            Log.i("PATRICK", "Timeout ProcessTransDetailUI");
            ProcessTransDetailUI.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (TransDetailListView.LOCK) {
                TransDetailListView.LOCK.setCondition(true);
                TransDetailListView.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {
        ProcessTransDetailUI.this.finish();

        Log.i("PATRICK", "Back");

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (TransDetailListView.LOCK) {
            TransDetailListView.LOCK.setCondition(true);
            TransDetailListView.LOCK.notifyAll();
        }
    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        final_string = null;
        SysApplication.getInstance().removeActivity(this);
    }

}
