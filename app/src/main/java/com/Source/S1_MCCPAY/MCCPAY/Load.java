package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.request.target.GlideDrawableImageViewTarget;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.google.zxing2.BarcodeFormat;
import com.google.zxing2.WriterException;
import com.google.zxing2.common.BitMatrix;
import com.google.zxing2.qrcode.QRCodeWriter;
import com.squareup.okhttp.MediaType;
import com.squareup.okhttp.MultipartBuilder;
import com.squareup.okhttp.OkHttpClient;
import com.squareup.okhttp.Request;
import com.squareup.okhttp.RequestBody;
import com.squareup.okhttp.Response;

import java.io.File;
import java.io.IOException;

import CTOS.CtPrint;

import static com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity.Chinese;

public class Load extends DemoAppActivity {
    Handler RTC_Handler;
    TextView time;
    int total_price;
    int pay_number;
    String time_temp;
    String card_number;
    String link;
    int count=1;
    CTOS_Printer Printer;
    CTOS_Printer Printer2;

    Canvas canvas;
    Bitmap panel = null;
    boolean wait_temp=false;
    int food_1_count;
    int food_2_count;
    int food_3_count;
    int food_4_count;
    int food_5_count;
    int food_6_count;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.msrcrad_main);
//        ActionBar actionBar = getSupportActionBar();
//        actionBar.hide();
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
        Bundle bundle = getIntent().getExtras();
        total_price = bundle.getInt("total_price");
        time_temp = bundle.getString("time_temp");
        card_number = bundle.getString("card_number");
        pay_number = (int)(Math.random()* 1000000000);
        link = bundle.getString("link");
        food_1_count=bundle.getInt("food_1_count");
        food_2_count=bundle.getInt("food_2_count");
        food_3_count=bundle.getInt("food_3_count");
        food_4_count=bundle.getInt("food_4_count");
        food_5_count=bundle.getInt("food_5_count");
        food_6_count=bundle.getInt("food_6_count");

        //ProgressDialog dialog = ProgressDialog.show(Load.this, "","Loading. Please wait...", true);
        //dialog.show();
        ImageView imageView = (ImageView) findViewById(R.id.qr_code);
        GlideDrawableImageViewTarget imageViewTarget = new GlideDrawableImageViewTarget(imageView);
        Glide.with(Load.this).load(R.raw.image).into(imageViewTarget);
        new Thread(new Runnable(){
            @Override
            public void run() {
                Printer = new Load.CTOS_Printer();
                Printer.Init();
                try {
                    Printer.goprintf();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                Printer2 = new CTOS_Printer();
                Printer2.Init();
                try {
                    Printer2.goprintf2();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                final OkHttpClient client = new OkHttpClient();
                MultipartBuilder builder = new MultipartBuilder();
                builder.type(MultipartBuilder.FORM);
                builder.addFormDataPart("transaction_id", String.valueOf(pay_number));
                builder.addFormDataPart("merchant_id", String.valueOf(pay_number));
                String item_id="",item_name="",item_count="",item_unit="",item_price="";
                boolean check =false;
                if(food_1_count>0)
                {
                    item_id+="1";
                    item_name+="hamburger";
                    item_count+=food_1_count;
                    item_unit+="Pcs";
                    item_price+="45";
                    check=true;
                }
                if(check)
                {
                    item_id+="|";
                    item_name+="|";
                    item_count+="|";
                    item_unit+="|";
                    item_price+="|";
                    check=false;
                }
                if(food_2_count>0)
                {
                    item_id+="2";
                    item_name+="hot dog";
                    item_count+=food_2_count;
                    item_unit+="Pcs";
                    item_price+="35";
                    check=true;
                }
                if(check)
                {
                    item_id+="|";
                    item_name+="|";
                    item_count+="|";
                    item_unit+="|";
                    item_price+="|";
                    check=false;
                }
                if(food_3_count>0)
                {
                    item_id+="3";
                    item_name+="chicken";
                    item_count+=food_3_count;
                    item_unit+="Pcs";
                    item_price+="160";
                    check=true;
                }
                if(check)
                {
                    item_id+="|";
                    item_name+="|";
                    item_count+="|";
                    item_unit+="|";
                    item_price+="|";
                    check=false;
                }
                if(food_4_count>0)
                {
                    item_id+="4";
                    item_name+="fries";
                    item_count+=food_4_count;
                    item_unit+="Pcs";
                    item_price+="30";
                    check=true;
                }
                if(check)
                {
                    item_id+="|";
                    item_name+="|";
                    item_count+="|";
                    item_unit+="|";
                    item_price+="|";
                    check=false;
                }
                if(food_5_count>0)
                {
                    item_id+="5";
                    item_name+="ice cream";
                    item_count+=food_5_count;
                    item_unit+="Pcs";
                    item_price+="40";
                    check=true;
                }
                if(check)
                {
                    item_id+="|";
                    item_name+="|";
                    item_count+="|";
                    item_unit+="|";
                    item_price+="|";
                    check=false;
                }
                if(food_6_count>0)
                {
                    item_id+="6";
                    item_name+="cola";
                    item_count+=food_6_count;
                    item_unit+="Pcs";
                    item_price+="25";
                    check=true;
                }
                if(check)
                {
                    item_id+="|";
                    item_name+="|";
                    item_count+="|";
                    item_unit+="|";
                    item_price+="|";
                    check=false;
                }
                builder.addFormDataPart("item_id", item_id);
                builder.addFormDataPart("item_name", item_name);
                builder.addFormDataPart("item_count", item_count);
                builder.addFormDataPart("item_unit", item_unit);
                builder.addFormDataPart("item_price", item_price);
                RequestBody fileBody = RequestBody.create(MediaType.parse("image/jpg"), new File("/mnt/sdcard/DCIM/gaozhen_08189.jpg"));
                builder.addFormDataPart("payment_img", "/mnt/sdcard/DCIM/gaozhen_08189.jpg", fileBody);
                RequestBody fileBody2 = RequestBody.create(MediaType.parse("image/jpg"), new File("/mnt/sdcard/DCIM/gaozhen_08188.jpg"));
                builder.addFormDataPart("invoice_img", "/mnt/sdcard/DCIM/gaozhen_08188.jpg", fileBody2);

                RequestBody body = builder.build();
                final Request request = new Request.Builder().url("http://castles.life9453.com/api/webservice").post(body).build();
                new Thread(new Runnable(){
                    @Override
                    public void run() {
                        try {
                            Response response = client.newCall(request).execute();
                            String temp = response.body().string();
                            String[] temp_2 = temp.split("\"qrcode_link\": \"");
                            String[] temp_3 = temp_2[1].split("\"");
                            link = temp_3[0];
                            Intent intent = new Intent();
                            Bundle bundle2 = new Bundle();
                            intent.setClass(Load.this, PayApprove.class);
//                            intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                            bundle2.putInt("total_price",total_price);
                            bundle2.putString("time_temp",time_temp);
                            bundle2.putString("card_number",card_number);
                            bundle2.putString("link",link);
                            intent.putExtras(bundle2);
                            Load.this.startActivity(intent);
//                            overridePendingTransition(0, 0);
                            finish();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }).start();



            }
            }).start();

    }

    public class CTOS_Printer {
        int ret = 0;
        CtPrint Print;
        public void Init()
        {
            Print = new CtPrint();
        }

        public int goprintf () throws IOException {
            if(Chinese==true)
            {
                Print.initPage(850);
                String print_font;
                int print_x = 0;
                int print_y = 16;
                int Currently_high = 50;

                int print_liftx = 25;

                print_font = "SAMPLE RECEIPT";
                print_y = 32;
                print_x = (384 - print_font.length() * print_y) / 2 + 52;
                print_x += print_font.length() * 3;
                Print.drawText(print_x, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "";
                print_y = 18;
                print_x = (384 - print_font.length() * print_y) / 2 + 55;
                print_x += print_font.length() * 3;
                Print.drawText(print_x, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 16;


                print_y = 26;

                print_font = "金融卡";
                print_x = (384 - print_font.length() * print_y) / 2 + 85;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "商店代號          4201605361";
                print_x = (384 - print_font.length() * print_y) / 2 + 85;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "端末機代號           63513242";
                print_x = (384 - print_font.length() * print_y) / 2 + 85;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;
                print_font = "========================";
                print_x = (384 - print_font.length() * print_y) / 2 + 55;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "城市別(City)";
                print_y = 16;
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;
                print_font = "TAICHUNG CITY";
                print_y = 24;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "卡別(Card Type)          檢查碼(Chech No. )";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "MASTERCARD            90119";
                print_y = 24;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "卡號(Card No. )";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = card_number + "(M)";
                print_y = 24;
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3 + 40;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "主機別/交易類別(Host/Irans. Type)";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "NCCC     00 一般交易 SALE";
                print_y = 24;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "批次號碼(Batch No.)     授權碼(Auth Code) ";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "379　　　　　　　　 047364";
                print_y = 24;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "日期/時間(Date/Time)";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;
                print_font = time_temp;
                print_y = 24;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;
                print_font = "序號(Ref No. )　　　發票號碼(Inv No. )　";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;
                print_font = "　　　　　　　91324279014";
                print_y = 24;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "總計(Total):     NT$" + total_price;
                print_y = 24;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                print_y = 16;
                Currently_high += print_y * 2;

                print_font = "";
                print_y = 24;
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                Print.drawText(print_x, print_y + Currently_high, print_font, print_y, 1);
                Currently_high += print_y * 3 + 10;

                print_font = "Sign: _______________________";
                Print.drawText(print_liftx, print_y+Currently_high+15, print_font, print_y);
                Currently_high +=  print_y-90;

                File file = new File("/mnt/sdcard/DCIM/sign_ok.jpg");
                Bitmap panel = BitmapFactory.decodeFile(file.getAbsolutePath());
                Rect src = new Rect(0, 0, panel.getWidth(), panel.getHeight());  //之前畫的圖
                Rect  dst = new Rect(70, Currently_high, panel.getWidth()/3*3/4+70, panel.getHeight()/3/2+Currently_high); //簽單
                Print.drawImage(panel, src, dst);  //把之前畫的圖和簽單合再一起
                Currently_high += (print_y * 5);


                print_font = "持卡人簽名";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 2;
                print_font = "I AGREE TO PAY THE ABOVE TOTAL";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 2;

                print_font = "AMOUNT, ACCORDING TO THE CARD";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 2;

                print_font = "ISSUER AGREEMENT";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y * 2;

                print_font = "CUSTOMER COPY";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y * 2;
            }
            else
            {
                Print.initPage(850);
                String print_font;
                int print_x = 0;
                int print_y = 16;
                int Currently_high = 50;

                int print_liftx = 25;

                print_font = "SAMPLE RECEIPT";
                print_y = 32;
                print_x = (384 - print_font.length()*print_y)/2+52;
                print_x += print_font.length()*3;
                Print.drawText(print_x, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "";
                print_y = 18;
                print_x = (384 - print_font.length()*print_y)/2+55;
                print_x += print_font.length()*3;
                Print.drawText(print_x, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y+16;


                print_y = 26;
                print_font = "...............................................";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y+15;

                print_y = 16;
                print_font = "STORE: 0003         REGISTER: 001";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y+3;

                print_font = "CASHIER: KATIE";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y+3;

                print_font = "ASSOCIATE: 0000000";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_y = 26;
                print_font = "...............................................";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;


                print_y = 16;
                print_font = "CUSTOMER RECEIPT COPY";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y+15;

                print_font = "城市別(City)";
                print_y = 16;
                print_x = (384 - print_font.length()*print_y)/2+1;
                print_x += print_font.length()*3;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;
                print_font = "TAICHUNG CITY";
                print_y = 24;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "卡別(Card Type)          檢查碼(Chech No. )";
                print_y = 16;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "MASTERCARD            90119";
                print_y = 24;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "卡號(Card No. )";
                print_y = 16;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = card_number+"(M)";
                print_y = 24;
                print_x = (384 - print_font.length()*print_y)/2+1;
                print_x += print_font.length()*3+40;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "主機別/交易類別(Host/Irans. Type)";
                print_y = 16;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "NCCC     00 一般交易 SALE";
                print_y = 24;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "批次號碼(Batch No.)     授權碼(Auth Code) ";
                print_y = 16;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "379　　　　　　　　 047364";
                print_y = 24;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "日期/時間(Date/Time)";
                print_y = 16;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;
                print_font = time_temp;
                print_y = 24;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;
                print_font = "序號(Ref No. )　　　發票號碼(Inv No. )　";
                print_y = 16;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;
                print_font = "　　　　　　　91324279014";
                print_y = 24;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y;

                print_font = "總計(Total):     NT$" + total_price;
                print_y = 24;
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                print_y = 16;
                Currently_high +=  print_y*2;

                print_font = "";
                print_y = 24;
                print_x = (384 - print_font.length()*print_y)/2+1;
                print_x += print_font.length()*3;
                Print.drawText(print_x, print_y+Currently_high, print_font, print_y,1);
                Currently_high +=  print_y*3+10;

                File file = new File("/mnt/sdcard/DCIM/sign_ok.jpg");
                Bitmap panel = BitmapFactory.decodeFile(file.getAbsolutePath());
                Rect src = new Rect(0, 0, panel.getWidth(), panel.getHeight());  //之前畫的圖
                Rect  dst = new Rect(70, Currently_high, panel.getWidth()/3*3/4+70, panel.getHeight()/3/2+Currently_high); //簽單
                Print.drawImage(panel, src, dst);  //把之前畫的圖和簽單合再一起
                Currently_high += (print_y * 5);



                Currently_high +=  (print_y*5);
                print_y = 16;
                print_font = "I AGREE TO PAY THE ABOVE TOTAL";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y+2;

                print_font = "AMOUNT, ACCORDING TO THE CARD";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y+2;

                print_font = "ISSUER AGREEMENT";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y*2;

                print_font = "CUSTOMER COPY";
                Print.drawText(print_liftx, print_y+Currently_high, print_font, print_y);
                Currently_high +=  print_y*2;


            }
            Print.saveJPEG("/mnt/sdcard/DCIM/gaozhen_08189.jpg");
            return ret;
        }

        public int goprintf2() throws IOException {
            Print.initPage(800);
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

            Print.saveJPEG("/mnt/sdcard/DCIM/gaozhen_08188.jpg");

            //Print.printPage();

            return ret;
        }
    }

}
