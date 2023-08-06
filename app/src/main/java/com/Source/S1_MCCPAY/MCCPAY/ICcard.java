package com.Source.S1_MCCPAY.MCCPAY;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;


public class ICcard extends DemoAppActivity {
    static Activity ActivityA;
    Handler RTC_Handler;
    TextView time;
    int total_price;
    String time_temp;
    String card_number;
    String pay_number;
    String link;
    int count=1;
    private int food_1_count;
    private int food_2_count;
    private int food_3_count;
    private  int food_4_count;
    private int food_5_count;
    private int food_6_count;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
		ActivityA=this;
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.iccrad_main);
//        ActionBar actionBar = getSupportActionBar();
//        actionBar.hide();
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
        Bundle bundle = getIntent().getExtras();
        total_price = bundle.getInt("total_price");
        time_temp = bundle.getString("time_temp");
        card_number = bundle.getString("card_number");
        pay_number = bundle.getString("pay_number");
        link = "http://castles.life9453.com/payment/info/"+pay_number;
        //link = bundle.getString("link");
        food_1_count=bundle.getInt("food_1_count");
        food_2_count=bundle.getInt("food_2_count");
        food_3_count=bundle.getInt("food_3_count");
        food_4_count=bundle.getInt("food_4_count");
        food_5_count=bundle.getInt("food_5_count");
        food_6_count=bundle.getInt("food_6_count");

        Button button1 = (Button)findViewById(R.id.button1);
        button1.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(1);
            }
        });
        Button button2 = (Button)findViewById(R.id.button2);
        button2.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(2);
            }
        });
        Button button3 = (Button)findViewById(R.id.button3);
        button3.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(3);
            }
        });
        Button button4 = (Button)findViewById(R.id.button4);
        button4.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(4);
            }
        });
        Button button5 = (Button)findViewById(R.id.button5);
        button5.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(5);
            }
        });
        Button button6 = (Button)findViewById(R.id.button6);
        button6.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(6);
            }
        });
        Button button7 = (Button)findViewById(R.id.button7);
        button7.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(7);
            }
        });
        Button button8 = (Button)findViewById(R.id.button8);
        button8.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(8);
            }
        });
        Button button9 = (Button)findViewById(R.id.button9);
        button9.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(9);
            }
        });
        Button button0 = (Button)findViewById(R.id.button0);
        button0.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                input_number(0);
            }
        });
        Button cancel = (Button)findViewById(R.id.cancel);
        cancel.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                Intent intent = new Intent();
                //intent.setClass(ICcard.this, HelloJni.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                bundle.putInt("total_price",total_price);
                bundle.putString("time_temp",time_temp);
                bundle.putString("pay_number",pay_number);
                bundle.putInt("food_1_count",food_1_count);
                bundle.putInt("food_2_count",food_2_count);
                bundle.putInt("food_3_count",food_3_count);
                bundle.putInt("food_4_count",food_4_count);
                bundle.putInt("food_5_count",food_5_count);
                bundle.putInt("food_6_count",food_6_count);
                intent.putExtras(bundle);
                ICcard.this.startActivity(intent);
//                overridePendingTransition(0, 0);
                ICcard.this.finish();
            }
        });
        Button clear = (Button)findViewById(R.id.clear);
        clear.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                count=1;
                TextView pin1 = (TextView)findViewById(R.id.pin1);
                TextView pin2 = (TextView)findViewById(R.id.pin2);
                TextView pin3 = (TextView)findViewById(R.id.pin3);
                TextView pin4 = (TextView)findViewById(R.id.pin4);
                pin1.setText("");
                pin2.setText("");
                pin3.setText("");
                pin4.setText("");
            }
        });
        Button enter = (Button)findViewById(R.id.enter);
        enter.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                if(count==5)
                {
                    //Toast.makeText(ICcard.this,"Free signature",Toast.LENGTH_SHORT).show();
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(ICcard.this, PayApprove.class);
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                    bundle.putInt("total_price",total_price);
                    bundle.putString("time_temp",time_temp);
                    bundle.putString("card_number",card_number);
                    bundle.putString("pay_number",pay_number);
                    bundle.putString("link",link);
                    intent.putExtras(bundle);
                    ICcard.this.startActivity(intent);
//                    overridePendingTransition(0, 0);
                    finish();
                }
                else
                {
                    Toast.makeText(ICcard.this,"請輸入完整密碼",Toast.LENGTH_SHORT).show();
                }
            }
        });

    }
    public void input_number(int number)
    {
        TextView pin1 = (TextView)findViewById(R.id.pin1);
        TextView pin2 = (TextView)findViewById(R.id.pin2);
        TextView pin3 = (TextView)findViewById(R.id.pin3);
        TextView pin4 = (TextView)findViewById(R.id.pin4);
        switch (count) {
            case 1:
                pin1.setText(String.valueOf(number));
                count++;
                break;
            case 2:
                pin1.setText("*");
                pin2.setText(String.valueOf(number));
                count++;
                break;
            case 3:
                pin1.setText("*");
                pin2.setText("*");
                pin3.setText(String.valueOf(number));
                count++;
                break;
            case 4:
                pin1.setText("*");
                pin2.setText("*");
                pin3.setText("*");
                pin4.setText(String.valueOf(number));
                count++;
                break;
            default:
                break;
        }
    }

}
