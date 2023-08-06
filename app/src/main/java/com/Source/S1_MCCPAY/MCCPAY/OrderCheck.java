package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

public class OrderCheck extends DemoAppActivity {
//    static Activity ActivityA;
    private int food_1_count;
    private int food_2_count;
    private int food_3_count;
    private  int food_4_count;
    private int food_5_count;
    private int food_6_count;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
//        ActivityA=this;
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ordercheck_main);
//        ActionBar actionBar = getSupportActionBar();
//        actionBar.hide();
        food_1_count=0;food_2_count=0;food_3_count=0;food_4_count=0;food_5_count=0;food_6_count=0;
        Bundle bundle = getIntent().getExtras();
        if(bundle.getInt("choose")==1)
            food_1_count = 1;
        else if(bundle.getInt("choose")==2)
            food_2_count=1;
        else if(bundle.getInt("choose")==3)
            food_3_count=1;
        else if(bundle.getInt("choose")==4)
            food_4_count=1;
        else if(bundle.getInt("choose")==5)
            food_5_count=1;
        else if(bundle.getInt("choose")==6)
            food_6_count=1;
        refresh();
        ImageView food_1 = (ImageView)findViewById(R.id.food_1);
        food_1.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                food_1_count+=1;
                refresh();
            }
        });
        ImageView food_2 = (ImageView)findViewById(R.id.food_2);
        food_2.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                food_2_count+=1;
                refresh();
            }
        });
        ImageView food_3 = (ImageView)findViewById(R.id.food_3);
        food_3.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                food_3_count+=1;
                refresh();
            }
        });
        ImageView food_4 = (ImageView)findViewById(R.id.food_4);
        food_4.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                food_4_count+=1;
                refresh();
            }
        });
        ImageView food_5 = (ImageView)findViewById(R.id.food_5);
        food_5.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                food_5_count+=1;
                refresh();
            }
        });
        ImageView food_6 = (ImageView)findViewById(R.id.food_6);
        food_6.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                food_6_count+=1;
                refresh();
            }
        });
        ImageView refresh = (ImageView)findViewById(R.id.refresh);
        refresh.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {

                Intent intent = new Intent();
                intent.setClass(OrderCheck.this, MainActivity.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                OrderCheck.this.startActivity(intent);
//                overridePendingTransition(0, 0);
                finish();
//                new Thread(new Runnable(){
//                    @Override
//                    public void run() {
//                        try{
//                            Thread.sleep(1000);
//                            finish();
//                        }
//                        catch(Exception e){
//                            e.printStackTrace();
//                        }
//                    }
//                }).start();
            }
        });
        ImageView check = (ImageView)findViewById(R.id.check);
        check.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                Intent intent = new Intent();
                intent.setClass(OrderCheck.this, PayChoose.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                int total_price= food_1_count*45+food_2_count*35+food_3_count*160+food_4_count*30+food_5_count*40+food_6_count*25;
                bundle.putInt("total_price",total_price);
                bundle.putInt("food_1_count",food_1_count);
                bundle.putInt("food_2_count",food_2_count);
                bundle.putInt("food_3_count",food_3_count);
                bundle.putInt("food_4_count",food_4_count);
                bundle.putInt("food_5_count",food_5_count);
                bundle.putInt("food_6_count",food_6_count);
                intent.putExtras(bundle);
                OrderCheck.this.startActivity(intent);
//                overridePendingTransition(0, 0);
                finish();
            }
        });
    }
    private void refresh(){
        if(food_1_count>0) {
            TextView food_1_count_text = (TextView) findViewById(R.id.food_1_count);
            String temp = "x" + food_1_count;
            food_1_count_text.setText(temp);
        }
        if(food_2_count>0) {
            TextView food_2_count_text = (TextView) findViewById(R.id.food_2_count);
            String temp = "x" + food_2_count;
            food_2_count_text.setText(temp);
        }
        if(food_3_count>0) {
            TextView food_3_count_text = (TextView) findViewById(R.id.food_3_count);
            String temp = "x" + food_3_count;
            food_3_count_text.setText(temp);
        }
        if(food_4_count>0) {
            TextView food_4_count_text = (TextView) findViewById(R.id.food_4_count);
            String temp = "x" + food_4_count;
            food_4_count_text.setText(temp);
        }
        if(food_5_count>0) {
            TextView food_5_count_text = (TextView) findViewById(R.id.food_5_count);
            String temp = "x" + food_5_count;
            food_5_count_text.setText(temp);
        }
        if(food_6_count>0) {
            TextView food_6_count_text = (TextView) findViewById(R.id.food_6_count);
            String temp = "x" + food_6_count;
            food_6_count_text.setText(temp);
        }
        TextView result = (TextView) findViewById(R.id.result);
        int total_cat=0;
        if(food_1_count>0)
            total_cat++;
        if(food_2_count>0)
            total_cat++;
        if(food_3_count>0)
            total_cat++;
        if(food_4_count>0)
            total_cat++;
        if(food_5_count>0)
            total_cat++;
        if(food_6_count>0)
            total_cat++;
        int total_count= food_1_count+food_2_count+food_3_count+food_4_count+food_5_count+food_6_count;
        int total_price= food_1_count*45+food_2_count*35+food_3_count*160+food_4_count*30+food_5_count*40+food_6_count*25;
        String temp = total_cat+" Items / "+ total_count + " Pcs \n Total $" + total_price;
        result.setText(temp);
    }
//    @Override
//    public void onDestroy() {
//        super.onDestroy();
//        Runtime.getRuntime().gc();
//    }
}
