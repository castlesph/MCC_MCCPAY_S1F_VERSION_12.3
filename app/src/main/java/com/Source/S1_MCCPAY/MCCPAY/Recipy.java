package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

import java.io.File;
import java.io.IOException;

import CTOS.CtPrint;

public class Recipy extends DemoAppActivity {
//    static Activity ActivityA;

    CTOS_Printer Printer;
    ImageView recipy;
    int total_price;
    String time_temp;
    String card_number;
    String pay_number;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
//        ActivityA=this;
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);

//        requestWindowFeature(Window.FEATURE_NO_TITLE);
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
        setContentView(R.layout.recipy_main);
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen

        recipy = (ImageView) findViewById(R.id.recipy);
        final Bitmap[] temp = {getBitmapFromSDCard("/mnt/sdcard/DCIM/gaozhen_08189.jpg")};
        recipy.setImageBitmap(temp[0]);

        Bundle bundle = getIntent().getExtras();
        total_price = bundle.getInt("total_price");
        time_temp = bundle.getString("time_temp");
        card_number = bundle.getString("card_number");
        pay_number = bundle.getString("pay_number");

        Printer = new CTOS_Printer();
        Printer.Init();

        ImageView check = (ImageView) findViewById(R.id.check);
        check.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                if (MainActivity.Chinese == true) {
                    Intent intent = new Intent();
                    intent.setClass(Recipy.this, Recipy2.class);
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                    Bundle bundle = new Bundle();
                    bundle.putInt("total_price", total_price);
                    bundle.putString("time_temp", time_temp);
                    bundle.putString("card_number", card_number);
                    bundle.putString("pay_number", pay_number);
                    intent.putExtras(bundle);
                    Recipy.this.startActivity(intent);
//                    overridePendingTransition(0, 0);
                    ((BitmapDrawable) recipy.getDrawable()).getBitmap().recycle();
                    finish();
                } else {
                    temp[0].recycle();
                    temp[0] = null;
                    recipy = null;
                    Intent intent = new Intent();
                    intent.setClass(Recipy.this, MainActivity.class);
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                    //intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
                    Recipy.this.startActivity(intent);
//                    overridePendingTransition(0, 0);

                    CloseActivityClass.exitClient();
                    finish();
                }
            }
        });
        ImageView print = (ImageView) findViewById(R.id.print);
        print.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                try {
                    if (MainActivity.Finish_Printf == 0 || MainActivity.Finish_Printf == 1) {
                        Printer.goprintf2();
                        if (MainActivity.Finish_Printf == 0)
                            MainActivity.Finish_Printf = 2;
                        else
                            MainActivity.Finish_Printf = 3;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }


    public static Bitmap getBitmapFromSDCard(String file) {
        try {
            Bitmap bitmap = BitmapFactory.decodeFile(file);
            return bitmap;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public class CTOS_Printer {
        int ret = 0;
        CtPrint Print;

        public void Init() {
            Print = new CtPrint();
        }

        public int goprintf2() throws IOException {

            File file = new File("/mnt/sdcard/DCIM/gaozhen_08189.jpg");
            Bitmap panel = BitmapFactory.decodeFile(file.getAbsolutePath());
            Print.initPage(panel.getHeight() + 100);
            System.out.println("Set panel initPage is" + Integer.toString(panel.getHeight() + 300));
            Print.drawImage(panel, 0, 0);
            Print.printPage();

            panel.recycle();
            panel = null;
            return ret;
        }
    }

    //    @Override
//    public void onDestroy() {
//        super.onDestroy();
//        Runtime.getRuntime().gc();
//    }
}
