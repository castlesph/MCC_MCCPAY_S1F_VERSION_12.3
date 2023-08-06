package com.Source.S1_MCCPAY.MCCPAY.Trans;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.*;

// sidumili:
public class TransDetailListView extends DemoAppActivity {

    private Context mContext;
    final Context c = this;
    public static String final_string;

    public static int inTimeOut = 30;

    RecyclerView recyclerView;
    String s1[], s2[], s3[], s4[], s5[], s6[], s7[], s8[], s9[], s10[], s11[];
    public static int images[] = {R.drawable.visa, R.drawable.mc, R.drawable.amex, R.drawable.jcb, R.drawable.diners, R.drawable.bancnet2, R.drawable.shopcard,
                    R.drawable.up, R.drawable.security, R.drawable.wechat, R.drawable.alipay, R.drawable.gcash, R.drawable.grabpay, R.drawable.upipay, R.drawable.phqr};

    Button btn_cancel;
    Button btn_confirm;
    TextView tv_title;

    String final_selected_string;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {

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

        setContentView(R.layout.activity_transdetail);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:TransDetailListView>>WAKE_LOCK");
        this.mWakeLock.acquire();


        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("saturn dispmsg", dispmsg);

        btn_cancel = (Button) findViewById(R.id.btn_cancel);
        btn_confirm = (Button) findViewById(R.id.btn_confirm);
        tv_title = (TextView) findViewById(R.id.textView_txn);

        // Display Trxn Title
        String[] dispmsginfotitle = dispmsg.split("\\|");
        tv_title.setText(dispmsginfotitle[0]);

        ArrayList<String> lists1 = new ArrayList<String>();
        ArrayList<String> lists2 = new ArrayList<String>();
        ArrayList<String> lists3 = new ArrayList<String>();
        ArrayList<String> lists4 = new ArrayList<String>();
        ArrayList<String> lists5 = new ArrayList<String>();
        ArrayList<String> lists6 = new ArrayList<String>();
        ArrayList<String> lists7 = new ArrayList<String>();
        ArrayList<String> lists8 = new ArrayList<String>();

        ArrayList<String> lists9 = new ArrayList<String>();
        ArrayList<String> lists10 = new ArrayList<String>();
        ArrayList<String> lists11 = new ArrayList<String>();

        // Read file
        try {
            Scanner sFile = new Scanner(new File("/data/data/pub/transdetailfile.dat"));
            int x = 0;

            while (sFile.hasNext())
            {
                //Log.i("Data=", sFile.nextLine());

                String record = sFile.nextLine();

                if (record == null)
                {
                    // do nothing....
                }
                else
                {
                    String[] dispmsginfo = record.split("\\|");
                    System.out.println("TransDetailListView::record [" + record + "]");

                    // [21|11|ALIPAY|150560000001052000000016|1| |RETRIEVE| | |MAR.18,2021 05:20:03AM|PHP1.00|

                    String sHDTid, sIITid, sIssuerName, sCardNo, sExpDate, sInvoiceNo, sTransType, sDateTime, sAmount, sRefundNo, sQRPresented, sVoided;
                    sHDTid = dispmsginfo[0];
                    sIITid = dispmsginfo[1];
                    sIssuerName = dispmsginfo[2];
                    sCardNo = dispmsginfo[3];
                    sExpDate = dispmsginfo[4];
                    sInvoiceNo = dispmsginfo[5];
                    sTransType = dispmsginfo[6];
                    sRefundNo  = dispmsginfo[7];
                    sQRPresented  = dispmsginfo[8];
                    sDateTime = dispmsginfo[9];
                    sAmount = dispmsginfo[10];
                    sVoided = dispmsginfo[11];


                    System.out.println("TransDetailListView::x [" + x + "]");
                    System.out.println("TransDetailListView::sHDTid [" + sHDTid + "]");
                    System.out.println("TransDetailListView::sIITid [" + sIITid + "]");
                    System.out.println("TransDetailListView::sIssuerName [" + sIssuerName + "]");
                    System.out.println("TransDetailListView::sCardNo [" + sCardNo + "]");
                    System.out.println("TransDetailListView::sExpDate [" + sExpDate + "]");
                    System.out.println("TransDetailListView::sInvoiceNo [" + sInvoiceNo + "]");
                    System.out.println("TransDetailListView::sTransType [" + sTransType + "]");
                    System.out.println("TransDetailListView::sDateTime [" + sDateTime + "]");
                    System.out.println("TransDetailListView::sAmount [" + sAmount + "]");
                    System.out.println("TransDetailListView::sRefundNo [" + sRefundNo + "]");
                    System.out.println("TransDetailListView::sQRPresented [" + sQRPresented + "]");
                    System.out.println("TransDetailListView::sVoided [" + sVoided + "]");

                    lists1.add(sTransType);
                    lists2.add(sCardNo);
                    lists3.add(sRefundNo);
                    lists4.add(sIssuerName);
                    lists5.add(sDateTime);
                    lists6.add(sAmount);
                    lists7.add(sHDTid);
                    lists8.add(sIITid);
                    lists9.add(sInvoiceNo);
                    lists10.add(sQRPresented);
                    lists11.add(sVoided);
                }

                x++;

            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        recyclerView = (RecyclerView) findViewById(R.id.recyclerView);

        s1 = lists1.toArray(new String[lists1.size()]);
        s2 = lists2.toArray(new String[lists2.size()]);
        s3 = lists3.toArray(new String[lists3.size()]);
        s4 = lists4.toArray(new String[lists4.size()]);
        s5 = lists5.toArray(new String[lists5.size()]);
        s6 = lists6.toArray(new String[lists6.size()]);
        s7 = lists7.toArray(new String[lists7.size()]);
        s8 = lists8.toArray(new String[lists8.size()]);
        s9 = lists9.toArray(new String[lists9.size()]);
        s10 = lists10.toArray(new String[lists10.size()]);
        s11 = lists11.toArray(new String[lists11.size()]);
        /*
        System.out.println(Arrays.toString(s1));
        System.out.println(Arrays.toString(s2));
        System.out.println(Arrays.toString(s3));
        System.out.println(Arrays.toString(s4));
        System.out.println(Arrays.toString(s5));
        System.out.println(Arrays.toString(s6));
        System.out.println(Arrays.toString(s7));
        System.out.println(Arrays.toString(s8));
        */

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar

        decorView = getWindow().getDecorView();
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

        TransDetailAdapter transDetailAdapter = new TransDetailAdapter(this, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11);
        recyclerView.setAdapter(transDetailAdapter);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));

        /*Start timer*/
        //getTimerRestart();

        btn_cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                //getTimerCancel();

                final_string = "0|000000|CANCEL";
                //SetProcessDetailVariable();
                TransDetailListView.this.finish();

                Log.i("TINE", "CANCEL BUTTON");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

        btn_confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                //getTimerCancel();

                getData();

                final_string = "1" + "|" + final_selected_string + "|" + "CONFIRM";
                Log.i("sidumili", "onClick: Confirm, final_string="+final_string);
                //SetProcessDetailVariable();
                TransDetailListView.this.finish();

                Log.i("TINE", "CONFIRM BUTTON");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });
    }

    public static class Lock {
        private boolean condition;

        public boolean conditionMet() {
            return condition;
        }

        public void setCondition(boolean condition) {
            this.condition = condition;


        }

    }

    private void getData()
    {
        String sSelected =  TransDetailAdapter.final_rb_selected_string;
        Log.i("sidumili", "getData: sSelected="+sSelected);
        if (sSelected.length() > 0)
        {
            final_selected_string = sSelected;
        }
    }

    public static TransDetailListView.Lock LOCK = new TransDetailListView.Lock();

    public void getTimerCancel() {
        timer.cancel();
    }

    public void getTimerRestart()
    {
        timer.start();
    }

    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("EditListView Timer", "Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "TransDetailListView Timer onFinish");

            //etInputStr.clear();
            final_string = "0|000000|TIME OUT";

            Log.i("PATRICK", "Timeout TransDetailListView");
            TransDetailListView.this.finish();

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
        final_selected_string = null;
        mContext = null;
        recyclerView = null;
        SysApplication.getInstance().removeActivity(this);
    }
}
