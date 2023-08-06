package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

import java.text.SimpleDateFormat;
import java.util.Date;

public class PayCheck extends DemoAppActivity {
    private int food_1_count;
    private int food_2_count;
    private int food_3_count;
    private int food_4_count;
    private int food_5_count;
    private int food_6_count;
    Handler RTC_Handler;
    TextView time;
    String time_temp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.paycheck_main);

        Bundle bundle = getIntent().getExtras();
        final int total_price = bundle.getInt("total_price");
        food_1_count = bundle.getInt("food_1_count");
        food_2_count = bundle.getInt("food_2_count");
        food_3_count = bundle.getInt("food_3_count");
        food_4_count = bundle.getInt("food_4_count");
        food_5_count = bundle.getInt("food_5_count");
        food_6_count = bundle.getInt("food_6_count");
        TextView price = (TextView) findViewById(R.id.price);
        price.setText("Amount:          $" + total_price/100 + "." + total_price%100);

        time = (TextView) findViewById(R.id.time);
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Date curDate = new Date(System.currentTimeMillis()); // 獲取當前時間
        String str = formatter.format(curDate);
        time_temp = str;
        time.setText("Transaction time:" + time_temp);
        RTC_Handler = new Handler();
        //RTC_Handler.postDelayed(RTC_runnable, 500);


        ImageView cancel = (ImageView) findViewById(R.id.cancel);
        cancel.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                startActivity(new Intent(PayCheck.this,MainActivity.class));
                PayCheck.this.finish();
            }
        });
        ImageView check = (ImageView) findViewById(R.id.check);
        check.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {

                Intent intent = new Intent();
                //intent.setClass(PayCheck.this, HelloJni.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                bundle.putInt("total_price", total_price);
                bundle.putString("time_temp", time_temp);
                bundle.putInt("food_1_count", food_1_count);
                bundle.putInt("food_2_count", food_2_count);
                bundle.putInt("food_3_count", food_3_count);
                bundle.putInt("food_4_count", food_4_count);
                bundle.putInt("food_5_count", food_5_count);
                bundle.putInt("food_6_count", food_6_count);
                intent.putExtras(bundle);
                PayCheck.this.startActivity(intent);
                finish();
//                overridePendingTransition(0, 0);

                /*
                Intent intent = new Intent();
                intent.setClass(PayCheck.this, PayApprove.class);
                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                bundle.putString("card_number","12345");
                bundle.putInt("pay_number",12345);
                bundle.putInt("total_price",total_price);
                bundle.putString("time_temp",time_temp);
                bundle.putInt("food_1_count",food_1_count);
                bundle.putInt("food_2_count",food_2_count);
                bundle.putInt("food_3_count",food_3_count);
                bundle.putInt("food_4_count",food_4_count);
                bundle.putInt("food_5_count",food_5_count);
                bundle.putInt("food_6_count",food_6_count);
                intent.putExtras(bundle);
                PayCheck.this.startActivity(intent);
                overridePendingTransition(0, 0);*/
            }
        });
    }

    final Runnable RTC_runnable = new Runnable() {
        public void run() {

            new Thread(new Runnable() {
                public void run() {
                    RTC_Handler.postDelayed(RTC_runnable, 500);
                    RTCHandler.sendMessage(RTCHandler.obtainMessage());
                }
            }).start();

        }
    };


    Handler RTCHandler = new Handler() {
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            time.setText(new Date().toString());
        }
    };
}
