package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

public class PayChoose extends DemoAppActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.paychoose_main);


        Bundle bundle = getIntent().getExtras();
        final int total_price=bundle.getInt("total_price");
        final int food_1_count=bundle.getInt("food_1_count");
        final int food_2_count=bundle.getInt("food_2_count");
        final int food_3_count=bundle.getInt("food_3_count");
        final int food_4_count=bundle.getInt("food_4_count");
        final int food_5_count=bundle.getInt("food_5_count");
        final int food_6_count=bundle.getInt("food_6_count");

        ImageView food_1 = (ImageView)findViewById(R.id.food_1);
        food_1.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                Intent intent = new Intent();
                intent.setClass(PayChoose.this, PayCheck.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                bundle.putInt("total_price",total_price);
                bundle.putInt("food_1_count",food_1_count);
                bundle.putInt("food_2_count",food_2_count);
                bundle.putInt("food_3_count",food_3_count);
                bundle.putInt("food_4_count",food_4_count);
                bundle.putInt("food_5_count",food_5_count);
                bundle.putInt("food_6_count",food_6_count);
                intent.putExtras(bundle);
                PayChoose.this.startActivity(intent);
//                overridePendingTransition(0, 0);
                finish();
            }
        });
        ImageView cancel = (ImageView)findViewById(R.id.cancel);
        cancel.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                startActivity(new Intent(PayChoose.this,MainActivity.class));
                PayChoose.this.finish();
            }
        });
    }

    @Override
    public void onBackPressed() {

    }
}
