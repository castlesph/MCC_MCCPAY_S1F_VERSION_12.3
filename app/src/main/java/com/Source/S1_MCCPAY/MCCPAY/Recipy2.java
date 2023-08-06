package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.google.zxing2.BarcodeFormat;
import com.google.zxing2.WriterException;
import com.google.zxing2.common.BitMatrix;
import com.google.zxing2.qrcode.QRCodeWriter;

import java.io.IOException;
import java.util.Date;

import CTOS.CtPrint;


public class Recipy2 extends DemoAppActivity {
    Handler RTC_Handler;
    TextView time;
    CTOS_Printer Printer;
    ImageView recipy;
    int total_price;
    String time_temp;
    String card_number;
    String pay_number;
    Bitmap panel = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.recipy2_main);
//        ActionBar actionBar = getSupportActionBar();
//        actionBar.hide();
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen

        recipy=(ImageView)findViewById(R.id.recipy);
        recipy.setImageBitmap(getBitmapFromSDCard("/mnt/sdcard/DCIM/gaozhen_08188.jpg"));

        Bundle bundle = getIntent().getExtras();
        total_price = bundle.getInt("total_price");
        time_temp = bundle.getString("time_temp");
        card_number = bundle.getString("card_number");
        pay_number = bundle.getString("pay_number");

        Printer = new CTOS_Printer();
        Printer.Init();

        ImageView check = (ImageView)findViewById(R.id.check);
        check.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                Intent intent = new Intent();
                intent.setClass(Recipy2.this, MainActivity.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                //intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
                Recipy2.this.startActivity(intent);
//                overridePendingTransition(0, 0);
                finish();
            }
        });
        ImageView print = (ImageView)findViewById(R.id.print);
        print.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                try {
                    Printer.goprintf();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    final Runnable RTC_runnable = new Runnable() {
        public void run() {

            new Thread(new Runnable(){
                public void run()
                {
                    RTC_Handler.postDelayed(RTC_runnable, 500);
                    RTCHandler.sendMessage(RTCHandler.obtainMessage());
                }
            }).start();

        }};


    Handler		RTCHandler	= new Handler()
    {
        public void handleMessage(Message msg)
        {
            super.handleMessage(msg);
            time.setText(new Date().toString());
        }
    };

    public static Bitmap getBitmapFromSDCard(String file)
    {
        try
        {
            Bitmap bitmap = BitmapFactory.decodeFile(file);
            return bitmap;
        }
        catch (Exception e)
        {
            e.printStackTrace();
            return null;
        }
    }

    public class CTOS_Printer {
        int ret = 0;
        CtPrint Print;
        public void Init()
        {
            Print = new CtPrint();
        }

        public int goprintf() throws IOException {

            Print.initPage(1000);
            String print_font;
            int print_x = 0;
            int print_y = 16;
            int Currently_high = 50;

            int print_liftx = 25;

            print_font = "Retail Merchant";
            print_y = 40;
            print_x = (384 - print_font.length()*print_y)/2+120;
            print_x += print_font.length()*3;
            Print.drawText(print_x, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y;

            print_font = "電子發票證明聯";
            print_y = 32;
            print_x = (384 - print_font.length()*print_y)/2-15;
            print_x += print_font.length()*3;
            Print.drawText(print_x, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y;
            print_font = "106年07-08月";
            print_x = (384 - print_font.length()*print_y)/2+45;
            print_x += print_font.length()*3;
            Print.drawText(print_x, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y;

            print_font = "AB-11223344";
            print_x = (384 - print_font.length()*print_y)/2+55;
            print_x += print_font.length()*3;
            Print.drawText(print_x, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y;

            print_font = time_temp;
            print_y = 20;
            print_x = (384 - print_font.length()*print_y)/2+1;
            print_x += print_font.length()*3;
            Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y;
            print_font = "隨機碼 : 9999　　　總計 $"+total_price;
            print_x = (384 - print_font.length()*print_y)/2+1;
            print_x += print_font.length()*3;
            Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y;
            print_font = "賣方23060248";
            Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y+10;

            Rect src = new Rect(0, 0, 368, 1200);
            Rect  dst = new Rect(40, Currently_high, 368-40, 1200/3+Currently_high);
            Resources res = getResources();
            panel=BitmapFactory.decodeResource(res, R.drawable.recipy_pic);
            Print.drawImage(panel, src, dst);
            Currently_high +=  print_y+50;

            String link_temp="http://castles.life9453.com/payment/info/"+pay_number;
            QRCodeWriter writer = new QRCodeWriter();
            BitMatrix bitMatrix = null;
            try {
                bitMatrix = writer.encode(link_temp, BarcodeFormat.QR_CODE, 512, 512);
            } catch (WriterException e) {
                e.printStackTrace();
            }
            int width = bitMatrix.getWidth();
            int height = bitMatrix.getHeight();
            final Bitmap bmp = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    bmp.setPixel(x, y, bitMatrix.get(x, y) ? Color.BLACK : Color.WHITE);
                }
            }

            Rect src2 = new Rect(0, 0, width, height);  //之前畫的圖
            Rect  dst2 = new Rect(30, Currently_high, width/3+30, height/3+Currently_high); //簽單
            Print.drawImage(bmp, src2, dst2);
            Rect  dst3 = new Rect(190, Currently_high, width/3+190, height/3+Currently_high); //簽單
            Print.drawImage(bmp, src2, dst3);
            //Rect  dst_3 = new Rect(40+100+40, Currently_high, 100, 1200/3+Currently_high);
            //Print.drawImage(bmp, src, dst_3);
            Currently_high +=  print_y+150;


            print_font = "店號009999-機02-序999999999";
            Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y+10;

            Rect src4 = new Rect(0, 0, 368, 1200);
            Rect  dst4 = new Rect(40, Currently_high, 368-40, 1200/3+Currently_high);
            Print.drawImage(panel, src4, dst4);
            Currently_high +=  print_y+60;

            print_font = "* *退貨時請攜帶電子發票證明聯";
            Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
            Currently_high +=  print_y;

            //Print.save("/mnt/sdcard/DCIM/gaozhen_08188.jpg");

            Print.printPage();

            return ret;
        }
    }

}
