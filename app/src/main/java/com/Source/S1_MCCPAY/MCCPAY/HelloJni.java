/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.Source.S1_MCCPAY.MCCPAY;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TableLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;

import CTOS.*;

import android.content.pm.PackageManager;

import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.bumptech.glide.Glide;
import com.bumptech.glide.request.target.GlideDrawableImageViewTarget;
import com.google.zxing2.BarcodeFormat;
import com.google.zxing2.WriterException;
import com.google.zxing2.common.BitMatrix;
import com.google.zxing2.qrcode.QRCodeWriter;
import com.squareup.okhttp.MediaType;
import com.squareup.okhttp.MultipartBuilder;
import com.squareup.okhttp.OkHttpClient;
import com.squareup.okhttp.RequestBody;

import static com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity.Chinese;


public class HelloJni extends DemoAppActivity {

    //    static Activity ActivityA;
    //Return Code
    public int temp_total_price;
    private boolean touch = true;
    private boolean Enable_printer = false;
    private boolean test_enter_keyin = false;

    final byte d_SC_USER = 0;
    byte d_OK = 0;
    final byte d_ON = 1;
    final byte d_OFF = 0;
    final byte d_PIN_FAIL = 0x20;
    final byte d_SIGN_FAIL = 0x30;
    final int d_EMVCL_PENDING = 0x80000020;
    final int d_EMVCL_INIT_TAGSETTING_ERROR = 0x80000022;
    final int d_EMVCL_TX_CANCEL = 0x80000021;
    final int d_EMVCL_RC_DATA = 0xA0000001;
    final int d_EMVCL_RC_NO_CARD = 0xA00000F2;
    final int d_EMVCL_RC_FAILURE = 0xA00000FF;

    final byte d_MK_SC_PRESENT = 1;
    final byte d_MK_SC_ACTIVE = 2;

    final byte d_EMVCL_ACT_DATA_START_A = 0x00;

    private CtCL cltest = new CtCL();

    CTOS_LCD LCD;
    private TextView uprowText;
    private TextView bottomrowText;
    CTOS_LED LED;
    CTOS_KBD KBD;
    CTOS_MSR MSR;
    CTOS_ICC ICC;


    boolean toTransaction;


    private ImageButton Button00;
    private ImageButton Button01;
    private ImageButton Button02;
    private ImageButton Button03;
    private ImageButton Button04;
    private ImageButton Button05;
    private ImageButton Button06;
    private ImageButton Button07;
    private ImageButton Button08;
    private ImageButton Button09;
    private ImageButton Buttondown;
    private ImageButton Buttonup;
    private ImageButton Buttoncancel;
    private ImageButton Buttonclear;
    private ImageButton Buttonenter;
    private ImageButton Buttonf;

    final int d_KBD_0 = 7;
    final int d_KBD_1 = 8;
    final int d_KBD_2 = 9;
    final int d_KBD_3 = 10;

    final int d_KBD_4 = 11;
    final int d_KBD_5 = 12;
    final int d_KBD_6 = 13;
    final int d_KBD_7 = 14;

    final int d_KBD_8 = 15;
    final int d_KBD_9 = 16;
    final int d_KBD_down = 0;
    final int d_KBD_up = 0;

    final int d_KBD_cancel = 28;
    final int d_KBD_clear = 67;
    final int d_KBD_f = 62;
    final int d_KBD_enter = 66;


    private ImageView led01;
    private ImageView led02;
    private ImageView led03;
    private ImageView led04;

    private ImageView pic;
    private ImageView draw_canvas;
    private ImageView cancle;
    Canvas canvas;
    Bitmap panel = null;
    Paint paint;
    private int downX;
    private int downY;
    private boolean is_touch = false;

    Handler draw_Handler;


    private boolean user_cancel;

    private SoundPool sp_beep;
    private int music_beep;
    private SoundPool sp_buttonbeep;
    private int music_buttonbeep;
    Handler aHandler;
    String time_temp;
    String card_number;
    String link;
    CTOS_Printer Printer;
    CTOS_Printer Printer2;
    int pay_number;


    boolean wait_temp = false;
    int food_1_count;
    int food_2_count;
    int food_3_count;
    int food_4_count;
    int food_5_count;
    int food_6_count;


    GlideDrawableImageViewTarget imageViewTarget;

    public void DEBUG(String str) {
        System.out.println(str);
    }

    @Override

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // TODO Auto-generated method stub


        if (keyCode == d_KBD_1)
            Button01.performClick();
        if (keyCode == d_KBD_2)
            Button02.performClick();
        if (keyCode == d_KBD_3)
            Button03.performClick();
        if (keyCode == d_KBD_4)
            Button04.performClick();
        if (keyCode == d_KBD_5)
            Button05.performClick();
        if (keyCode == d_KBD_6)
            Button06.performClick();
        if (keyCode == d_KBD_7)
            Button07.performClick();
        if (keyCode == d_KBD_8)
            Button08.performClick();
        if (keyCode == d_KBD_9)
            Button09.performClick();
        if (keyCode == d_KBD_0)
            Button00.performClick();


        if (keyCode == d_KBD_cancel)
            Buttoncancel.performClick();
        if (keyCode == d_KBD_clear)
            Buttonclear.performClick();
        if (keyCode == d_KBD_f)
            Buttonf.performClick();
        if (keyCode == d_KBD_enter)
            Buttonenter.performClick();

        DEBUG(" onKeyDown " + keyCode);
        return super.onKeyDown(keyCode, event);
    }


    /**
     * Called when the activity is first created.
     */

    final Runnable draw_runnable = new Runnable() {
        public void run() {
            InitPanel();
        }
    };


    // LCD
    public class CTOS_LCD {
        Boolean godraw = false;
        Boolean pwdmode = false;
        Boolean key_pin = false;
        String result;
        String savelcd;
        int amout_len = 0;
        float[] save_amout = new float[7];
        int pwd_len = 0;
        int[] save_pwd;
        double number = 0.00f;

        public void Init() {
            uprowText = (TextView) findViewById(R.id.up_row_view);
            bottomrowText = (TextView) findViewById(R.id.bottom_row_view);
            number = temp_total_price;
            result = String.format("$ %15.2f", number);
            Printbottomrow(result);
            pwdmode = false;
            draw_canvas = (ImageView) findViewById(R.id.canvas_space);
            pic = (ImageView) findViewById(R.id.show_pic);

        }

        public void drawtest() {


            if (godraw == false) {
                LED.EndIdleLEDBehavior();
                draw_seton();
                godraw = true;
                draw_canvas.setOnTouchListener(new MyOnTouchListener());
            } else {
                LED.StartIdleLEDBehavior();
                draw_setoff();
                godraw = false;
                draw_canvas.setVisibility(View.GONE);

                //canvas.drawBitmap(draw_canvas, 0, 0, null);
                //canvas.drawBitmap(draw_canvas, firstBitmap.getWidth(),0, null);
                Paint mypaint = new Paint();
                mypaint.setTextSize(45);
                mypaint.setColor(Color.LTGRAY);
                canvas.drawText("test test", 0, 40, mypaint);
                //panel =mergeBitmap(panel,panel);
                save_tupian();
                panel = null;
                canvas.drawColor(Color.WHITE);
                canvas.drawColor(R.drawable.background_radius);
            }
        }


        public void drawtest2() {
            CtPrint p;
            p = new CtPrint();
            p.initPage(1200);
            if (godraw == false) {
                LED.EndIdleLEDBehavior();
                draw_seton();
                godraw = true;
                draw_Handler = new Handler();
                draw_Handler.post(draw_runnable);
                draw_canvas.setOnTouchListener(new MyOnTouchListener());


            } else {
                if (is_touch == false) {
                    Message msg = new Message();
                    msg.what = 1;
                    mHandler.sendMessage(msg);
                    drawtest3();
                    LED.StartIdleLEDBehavior();
                    draw_setoff();
                    godraw = false;
                    panel = null;
                    return;
                }

                bottomrowText.setText(savelcd);
                String print_font;
                int print_x = 0;
                int print_y = 16;
                int Currently_high = 50;

                int print_liftx = 50;

                print_font = "SALE TESTING";
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                p.drawText(print_x, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "CASTLES TECHNOLOGY";
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                p.drawText(print_x, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "HAPPY RETAIL";
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                p.drawText(print_x, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "BANK CARDS";
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                p.drawText(print_x, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y * 3;

                print_font = "DEMO";
                print_y = 36;
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                p.drawText(print_x, print_y + Currently_high, print_font, print_y);
                print_y = 16;
                Currently_high += print_y * 3 + 36;

                print_font = "Date/Time";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "MERCHANT ID: 000009876543210";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "BATCH NUM  : 000001";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "INVOICE NUM: 00001";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y * 3;

                print_font = "WAVE SALE";
                print_y = 36;
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3 + 40;
                p.drawText(print_x, print_y + Currently_high, print_font, print_y);
                print_y = 16;
                Currently_high += print_y * 3 + 36;

                print_font = "CARD NUM: 4563 0181 0005 1401";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "EXP DATE: 09/10";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "APPR CODE: 111";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "CARD TYPE: BANK";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "REF NUM: 010203040506";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y * 4;

                print_font = "AMOUNT:  EURO  " + String.format("%.2f", LCD.number);
                print_y = 24;
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                print_y = 16;
                Currently_high += print_y * 2;

                print_font = "_______________________";
                print_y = 24;
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                print_y = 16;
                Currently_high += print_y * 2;

				  /*
                 Bitmap b = Bitmap.createBitmap(384, print_y+4, Bitmap.Config.RGB_565);
		         b.eraseColor(Color.BLACK);


		         Rect src = new Rect(0, 0, 354, print_y+4);
		         Rect dst = new Rect(15, Currently_high, 354, print_y+4+Currently_high);
		         p.drawImage(b, src, dst);
				  */

                //print_font = "NO SIGNATURE REQUIRED";
                print_font = "";
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                p.drawText(print_x, print_y + Currently_high, print_font, print_y, 1);
                Currently_high += print_y * 4 + 10;

                print_font = "SIGN: ______________________________";
                p.drawText(print_liftx, print_y + Currently_high + 15, print_font, print_y);
                Currently_high -= (print_y * 4 - 15);


                //p.drawText(0, 12, "TEST", 12*3);
                // Bitmap b = Bitmap.createBitmap(30, 30, Bitmap.Config.RGB_565);
                // b.eraseColor(Color.BLACK);


                //print_font = "          X   ";
                //p.drawText(print_liftx, print_y+Currently_high+55, print_font, print_y*3);


                Currently_high += (print_y * 4 - 15);
                Currently_high += print_y * 3;

                print_font = "I AGREE TO PAY THE ABOVE TOTAL";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "AMOUNT, ACCORDING TO THE CARD";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "ISSUER AGREEMENT";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y * 2;

                print_font = "CUSTOMER COPY";
                p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y * 2;


                p.saveJPEG("/mnt/sdcard/DCIM/gaozhen_0818.jpg");
                LED.StartIdleLEDBehavior();
                draw_setoff();
                godraw = false;
                panel = null;

                canvas.drawColor(Color.WHITE);
                canvas.drawColor(R.drawable.background_radius);
                draw_canvas.setVisibility(View.GONE);

                p.printPage();


            }
        }

        public void drawtest3() {


            CtPrint p = new CtPrint();
            p.initPage(1200);
            String print_font;
            int print_x = 0;
            int print_y = 16;
            int Currently_high = 50;

            int print_liftx = 50;

            print_font = "SALE TESTING";
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            p.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "CASTLES TECHNOLOGY";
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            p.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;
            print_font = "HAPPY RETAIL";
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            p.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "BANK CARDS";
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            p.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y * 3;

            print_font = "DEMO";
            print_y = 36;
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            p.drawText(print_x, print_y + Currently_high, print_font, print_y);
            print_y = 16;
            Currently_high += print_y * 3 + 36;

            print_font = "Date/Time";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "MERCHANT ID: 000009876543210";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "BATCH NUM  : 000001";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "INVOICE NUM: 00001";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y * 3;

            print_font = "WAVE SALE";
            print_y = 36;
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3 + 40;
            p.drawText(print_x, print_y + Currently_high, print_font, print_y);
            print_y = 16;
            Currently_high += print_y * 3 + 36;

            print_font = "CARD NUM: 4563 0181 0005 1401";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "EXP DATE: 09/10";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "APPR CODE: 111";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "CARD TYPE: BANK";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "REF NUM: 010203040506";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y * 4;

            print_font = "AMOUNT:  EURO  " + String.format("%.2f", LCD.number);
            print_y = 24;
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            print_y = 16;
            Currently_high += print_y * 2;

            print_font = "_______________________";
            print_y = 24;
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            print_y = 16;
            Currently_high += print_y * 2;

			 /*
             Bitmap b = Bitmap.createBitmap(384, print_y+4, Bitmap.Config.RGB_565);
	         b.eraseColor(Color.BLACK);

	         Rect src = new Rect(0, 0, 354, print_y+4);
	         Rect dst = new Rect(15, Currently_high, 354, print_y+4+Currently_high);
	         p.drawImage(b, src, dst);
	         */

            //print_font = "NO SIGNATURE REQUIRED";
            print_font = "";
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            p.drawText(print_x, print_y + Currently_high, print_font, print_y, 1);
            Currently_high += print_y * 4 + 10;

            print_font = "SIGN: ______________________________";
            p.drawText(print_liftx, print_y + Currently_high + 15, print_font, print_y);
            Currently_high -= (print_y * 4 - 15);


            print_font = "          X   ";
            p.drawText(print_liftx, print_y + Currently_high + 55, print_font, print_y * 3);


            Currently_high += (print_y * 4 - 15);
            Currently_high += print_y * 3;

            print_font = "I AGREE TO PAY THE ABOVE TOTAL";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "AMOUNT, ACCORDING TO THE CARD";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "ISSUER AGREEMENT";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y * 2;

            print_font = "CUSTOMER COPY";
            p.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y * 2;


            p.saveJPEG("/mnt/sdcard/DCIM/gaozhen_0818.jpg");


            p.printPage();

        }


        public void Setpwdmode(byte onoff) {
            if (onoff == d_ON) {
                pwd_len = 0;
                save_pwd = new int[12];
                pwdmode = true;
                key_pin = false;
            } else if (onoff == d_OFF) {
                pwdmode = false;
            }

        }


        public void showpic(int id) {
            pic.setImageResource(id);
        }


        public void refresh() {

            result = String.format("$ %15.2f", number);
            Printbottomrow(result);


        }


        public void pic_setoff() {
            uprowText.setVisibility(View.VISIBLE);
            bottomrowText.setVisibility(View.VISIBLE);
            pic.setVisibility(View.GONE);
            cancle.setVisibility(View.VISIBLE);
        }

        public void pic_seton() {
            uprowText.setVisibility(View.GONE);
            bottomrowText.setVisibility(View.GONE);
            pic.setVisibility(View.VISIBLE);
            cancle.setVisibility(View.GONE);
        }

        public void draw_setoff() {
            uprowText.setVisibility(View.VISIBLE);
            bottomrowText.setVisibility(View.VISIBLE);
            draw_canvas.setVisibility(View.GONE);
        }

        public void draw_seton() {
            uprowText.setVisibility(View.GONE);
            bottomrowText.setVisibility(View.GONE);
            draw_canvas.setVisibility(View.VISIBLE);
        }

        public void Printonrow(String string) {

            uprowText.setText(string);
        }

        public void Printbottomrow(String string) {

            bottomrowText.setText(string);
        }


    }


    final int waiting_for_trade = 0;
    final int Processing = 1;
    final int trade_success = 2;
    final int trade_error = 3;
    final int trade_stop = 4;

    // LED
    public class CTOS_LED {

        int status = waiting_for_trade;

        public void Init() {
            ;
        }

        public void StartIdleLEDBehavior() {
            led_Handler.postDelayed(led_runnable, 10);
            ;
        }

        public void EndIdleLEDBehavior() {
            led_Handler.removeCallbacks(led_runnable);
            ;
        }
    }

    int Reverse = 0, Flicker = 0, LED_count = 0;
    Handler led_Handler = new Handler();
    Runnable led_runnable = new Runnable() {
        @Override
        public void run() {
            // TODO Auto-generated method stub


            if (LED.status == waiting_for_trade) {
                led_Handler.postDelayed(this, 10);

                if (LED_count >= 100) {
                    if (Reverse == 1) {
                        Reverse = 0;
                    } else {
                        Reverse = 1;
                    }
                    LED_count = 0;
                } else
                    LED_count++;
            } else if (LED.status == Processing) {
                led_Handler.postDelayed(this, 10);

                if (LED_count >= 30) {
                    if (Reverse == 1) {
                        Reverse = 0;
                    } else {
                        Reverse = 1;
                    }
                    LED_count = 0;
                } else
                    LED_count++;
            } else if (LED.status == trade_success) {
                if (Flicker == 0) {
                    led_Handler.postDelayed(this, 250);
                    Flicker++;

                } else if (Flicker == 1) {
                    led_Handler.postDelayed(this, 1000);
                    Flicker++;
                    sp_beep.play(music_beep, 1, 1, 0, 0, 1);
                } else if (Flicker == 2) {
                    led_Handler.postDelayed(this, 250);
                    LED.status = waiting_for_trade;


                    LED_count = 0;
                    Flicker = 0;
                }
            } else if (LED.status == trade_error) {
                led_Handler.postDelayed(this, 1000);
                if (Flicker == 0) {
                    Flicker++;
                } else if (Flicker == 1) {
                    LED.status = waiting_for_trade;
                    LED_count = 0;
                    Flicker = 0;
                }
            } else if (LED.status == trade_stop) {
                led_Handler.postDelayed(this, 10);
            }

        }

    };


    // ICC
    public class CTOS_ICC {
        byte[] ICCPIN;
        byte[] ICCStar;
        int ICCPINLen;
        int ICCStarLen;
        String result;
        int ret = 0x0000;
        int card_status = 0;
        CtSC sc;
        boolean wait;
        int test_card = 0;
        int ATRLen = 0;
        byte baATR[] = new byte[128];
        byte baRBuf[] = new byte[128];
        byte baSBuf[] = new byte[5];

        public void Init() {
            sc = new CtSC();
        }


        public int IccTXNProcess() {


            System.out.println("do TxnAppSelect");
            byte buf[] = new byte[128];
            char bID = 0;

            sc.resetEMV(bID, 1, buf);
            sc.status(test_card);
            card_status = sc.getStatus();
            if ((card_status & d_MK_SC_ACTIVE) == d_MK_SC_ACTIVE) {
                ret = sc.resetISO(test_card, 1, baATR);
                if (ret == d_OK) {
                    LED.status = trade_stop;
                    Message msg = new Message();
                    msg.what = 4;
                    mHandler.sendMessage(msg);
                    wait = false;
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                Thread.sleep(1000);
                            } catch (Exception e) {
                                e.printStackTrace();
                            } finally {
                                wait = true;
                            }
                        }
                    }).start();
                    while (wait == false) ;

                    //msg = new Message();
                    //msg.what = 3;
                    //mHandler.sendMessage(msg);
                    try {
                        Thread.sleep(2000);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    while (!wait_temp) ;

                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(HelloJni.this, ICcard.class);
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                    bundle.putInt("total_price", temp_total_price);
                    bundle.putString("time_temp", time_temp);
                    bundle.putString("card_number", card_number);
                    //bundle.putString("link", link);
                    bundle.putString("pay_number", String.valueOf(pay_number));
                    bundle.putInt("food_1_count", food_1_count);
                    bundle.putInt("food_2_count", food_2_count);
                    bundle.putInt("food_3_count", food_3_count);
                    bundle.putInt("food_4_count", food_4_count);
                    bundle.putInt("food_5_count", food_5_count);
                    bundle.putInt("food_6_count", food_6_count);
                    intent.putExtras(bundle);
                    HelloJni.this.startActivity(intent);
//                    overridePendingTransition(0, 0);
                    wait = false;
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                Thread.sleep(1000);
                                while (LCD.godraw == true) ;
                            } catch (Exception e) {
                                e.printStackTrace();
                            } finally {
                                wait = true;
                            }
                        }
                    }).start();
                    while (wait == false) ;
                    HelloJni.this.finish();
                    return d_OK;

                }
                return ret;
            } else {
                System.out.println("nonononono!!!");
                Message msg = new Message();
                msg.what = 4;
                mHandler.sendMessage(msg);
                wait = false;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Thread.sleep(1000);
                        } catch (Exception e) {
                            e.printStackTrace();
                        } finally {
                            wait = true;
                        }
                    }
                }).start();
                while (wait == false) ;

                msg = new Message();
                msg.what = 1;
                mHandler.sendMessage(msg);
                return 0x00000001;
            }
        }

        public int Status() {
            ICCPIN = new byte[8];
            ICCStar = new byte[8];
            ICCStarLen = 0;
            ICCPINLen = 0;

            byte buf[] = new byte[128];
            char bID = 0;

            sc.resetEMV(bID, 1, buf);
            sc.status(test_card);
            card_status = sc.getStatus();
            System.out.println(String.format("Status  = %d", card_status));


            if ((card_status & (d_MK_SC_PRESENT | d_MK_SC_ACTIVE)) == (d_MK_SC_PRESENT | d_MK_SC_ACTIVE)) {
                ret = IccTXNProcess();
                return ret;
            }
            return 0x00000001;
        }


    }


    // MSR
    public class CTOS_MSR {
        int ret = 0x0000;
        CtMSR msr;
        int len = 0, err = 0;
        boolean wait;
        byte[] CardExpDatebuf2 = new byte[128];

        public void Init() {
            msr = new CtMSR();
        }

        public int MSRRead() {
            byte baTk1Buf[] = new byte[256];
            byte baTk2Buf[] = new byte[256];
            byte baTk3Buf[] = new byte[256];
            ret = msr.read(baTk1Buf, baTk2Buf, baTk3Buf);
            //String result = String.format("MSRRead is 0x%04x",ret);
            //System.out.println(result);
            if (ret == 0) {
                len = msr.getTk1Len();
                err = msr.getTk1Err();
                System.out.println(String.format("\n===TK1===%03d[0x02%x]\n", len, err));


                for (int i = 0; i < len; i++) {
                    System.out.println(String.format("%d", baTk1Buf[i]));
                }

                len = msr.getTk2Len();
                err = msr.getTk2Err();
                System.out.println(String.format("\n===TK2===%03d[0x02%x]\n", len, err));

                for (int i = 0; i < len; i++) {
                    System.out.println(String.format("%d", baTk2Buf[i]));
                }

                len = msr.getTk3Len();
                err = msr.getTk3Err();
                System.out.println(String.format("\n===TK3===%03d[0x02%x]\n", len, err));

                for (int i = 0; i < len; i++) {
                    System.out.println(String.format("%d", baTk3Buf[i]));
                }
            }
            //if(ret == 0 && baTk2Buf.length != 0)
            if (ret == 0 && baTk2Buf.length != 0) {
                System.out.println("Do MSR!");
                //ret = MSRTXNProcess(msr.baTk1Buf, msr.pusTk1Len, msr.baTk2Buf, msr.pusTk2Len, msr.baTk3Buf, msr.pusTk3Len);
                ret = d_OK;
                if (ret == d_OK) {
                    LED.status = trade_stop;
                    System.out.println("MSRTXNProcess OK!");
                    user_cancel = false;
                    Message msg = new Message();
                    msg.what = 6;
                    mHandler.sendMessage(msg);

                    wait = false;
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                Thread.sleep(1000);
                                while (LCD.godraw == true) ;
                            } catch (Exception e) {
                                e.printStackTrace();
                            } finally {
                                wait = true;
                            }
                        }
                    }).start();
                    while (wait == false) ;

                    if (user_cancel == false) {
                        msg = new Message();
                        msg.what = 5;
                        mHandler.sendMessage(msg);
                        wait = false;
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    Thread.sleep(1000);
                                } catch (Exception e) {
                                    e.printStackTrace();
                                } finally {
                                    wait = true;
                                }
                            }
                        }).start();

                        while (wait == false) ;
                    }
                    try {
                        Thread.sleep(2000);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    while (!wait_temp) ;
                    Intent intent = new Intent();
                    intent.setClass(HelloJni.this, MSRcard.class);
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                    Bundle bundle = new Bundle();
                    bundle.putInt("total_price", temp_total_price);
                    bundle.putString("time_temp", time_temp);
                    bundle.putString("card_number", card_number);
                    bundle.putInt("pay_number", pay_number);
                    //bundle.putString("link", link);
                    bundle.putInt("pay_number", pay_number);
                    bundle.putInt("food_1_count", food_1_count);
                    bundle.putInt("food_2_count", food_2_count);
                    bundle.putInt("food_3_count", food_3_count);
                    bundle.putInt("food_4_count", food_4_count);
                    bundle.putInt("food_5_count", food_5_count);
                    bundle.putInt("food_6_count", food_6_count);
                    intent.putExtras(bundle);
                    HelloJni.this.startActivity(intent);
//                    overridePendingTransition(0, 0);
//                    new Thread(new Runnable() {
//                        @Override
//                        public void run() {
//                            try {
//                                Thread.sleep(1000);
                    finish();
//                            } catch (Exception e) {
//                                e.printStackTrace();
//                            }
//                        }
//                    }).start();
                    return ret;

                }
                /*try{
                    Thread.sleep(2000);
                }
                catch(Exception e){
                    e.printStackTrace();
                }
                while(!wait_temp);
                Intent intent = new Intent();
                intent.setClass(HelloJni.this, PayApprove.class);
                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                bundle.putInt("total_price",temp_total_price);
                bundle.putString("time_temp",time_temp);
                bundle.putString("card_number",card_number);
                bundle.putString("link",link);
                intent.putExtras(bundle);
                HelloJni.this.startActivity(intent);*/
                return ret;
            } else
                return 0x00000001;

        }


        public int MSRTXNProcess(byte[] baTK1, char TK1Len, byte[] baTK2, char TK2Len, byte[] baTK3, char TK3Len) {
            byte[] baPAN = new byte[32];
            int i = 1;
            boolean CardExpFlag = false;
            int j = 0;

            while (true) {
                if (CardExpFlag == true) {
                    CardExpDatebuf2[0] = baTK2[j];
                    CardExpDatebuf2[1] = baTK2[j + 1];
                    CardExpDatebuf2[2] = baTK2[j + 2];
                    CardExpDatebuf2[3] = baTK2[j + 3];

                    CardExpFlag = false;
                    break;
                }
                if (j > TK2Len)
                    break;

                if (baTK2[j] == '=')
                    CardExpFlag = true;

                j++;
            }

            if (baTK2[0] == ';') {
                while (baTK2[i] != '=' || i > TK2Len) {
                    baPAN[i - 1] = baTK2[i];
                    i++;
                }
                Message msg = new Message();
                msg.what = 4;
                mHandler.sendMessage(msg);
                wait = false;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Thread.sleep(1000);
                        } catch (Exception e) {
                            e.printStackTrace();
                        } finally {
                            wait = true;
                        }
                    }
                }).start();
                while (wait == false) ;

                return d_OK;
            }
            return 0x00000001;
        }
    }

    // KBD
    public class CTOS_KBD {
        public void Init() {
            Button00 = (ImageButton) findViewById(R.id.ImageButton00);
            Button00.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7 && LCD.number != 0) {
                            LCD.save_amout[LCD.amout_len] = 0.0f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 0;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });


            Button01 = (ImageButton) findViewById(R.id.ImageButton01);
            Button01.setOnKeyListener(new OnKeyListener() {
                public boolean onKey(View v, int keyCode, KeyEvent event) {
                    System.out.println("keyCode = " + keyCode);
                    if (keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
                        Buttonenter.performClick();
                        test_enter_keyin = true;
                    }
                    return false;
                }
            });
            Button01.setOnClickListener(new Button.OnClickListener() {

                public void onClick(View arg0) {

                    if (test_enter_keyin == true) {
                        test_enter_keyin = false;
                        return;
                    }
                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.01f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.01;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 1;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button02 = (ImageButton) findViewById(R.id.ImageButton02);
            Button02.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.02f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.02;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 2;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button03 = (ImageButton) findViewById(R.id.ImageButton03);
            Button03.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.03f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.03;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 3;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button04 = (ImageButton) findViewById(R.id.ImageButton04);
            Button04.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);


                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.04f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.04;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 4;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button05 = (ImageButton) findViewById(R.id.ImageButton05);
            Button05.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);


                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.05f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.05;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 5;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button06 = (ImageButton) findViewById(R.id.ImageButton06);
            Button06.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.06f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.06;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 6;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button07 = (ImageButton) findViewById(R.id.ImageButton07);
            Button07.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.07f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.07;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 7;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button08 = (ImageButton) findViewById(R.id.ImageButton08);
            Button08.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.08f;
                            // number_result =
                            // String.format("save_amout[%d] is %1.2f",amout_len,
                            // save_amout[amout_len]);
                            // System.out.println(number_result);
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.08;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 8;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Button09 = (ImageButton) findViewById(R.id.ImageButton09);
            Button09.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len < 7) {
                            LCD.save_amout[LCD.amout_len] = 0.09f;
                            LCD.amout_len = LCD.amout_len + 1;
                            LCD.number = LCD.number * 10;
                            LCD.number += 0.09;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len < 12) {
                            LCD.save_pwd[LCD.pwd_len] = 9;
                            LCD.pwd_len = LCD.pwd_len + 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                }
            });

            Buttoncancel = (ImageButton) findViewById(R.id.ImageButtoncancel);
            Buttoncancel.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false) {
                        if (LCD.amout_len > 0) {
                            LCD.amout_len = 0;
                            LCD.number = 0;
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }


                        if (LCD.godraw == true) {
                            LCD.godraw = false;
                            LCD.draw_setoff();
                            panel = null;
                            canvas.drawColor(Color.WHITE);
                            canvas.drawColor(R.drawable.background_radius);
                            LED.StartIdleLEDBehavior();
                            user_cancel = true;

                        }


                        if (toTransaction == true) {
                            toTransaction = false;
                            LCD.number = 0.00f;
                            LCD.refresh();
                        }


                    }
                    if (LCD.pwdmode == true) {
                        LCD.pwd_len = 1;
                        LCD.save_pwd[0] = d_PIN_FAIL;
                        if (LCD.key_pin == false)
                            LCD.key_pin = true;

                    }
                }
            });

            Buttonclear = (ImageButton) findViewById(R.id.ImageButtonclear);
            Buttonclear.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    if (LCD.pwdmode == false && toTransaction == false) {
                        if (LCD.amout_len > 0) {
                            LCD.amout_len = LCD.amout_len - 1;
                            LCD.number = 0;
                            for (int i = 0; i < LCD.amout_len; i++) {
                                LCD.number = LCD.number * 10;
                                LCD.number += LCD.save_amout[i];
                            }
                            LCD.result = String.format("$ %15.2f", LCD.number);
                            LCD.Printbottomrow(LCD.result);
                        }
                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.pwd_len > 0) {
                            LCD.pwd_len = LCD.pwd_len - 1;
                            LCD.result = "";
                            for (int len = 0; len < LCD.pwd_len; len++)
                                LCD.result += "*";
                            LCD.Printbottomrow(LCD.result);
                        }

                    }
                }
            });

            Buttonenter = (ImageButton) findViewById(R.id.ImageButtonenter);
            Buttonenter.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);


                    if (LCD.pwdmode == false && toTransaction == false) {

                        double number = temp_total_price;
                        String result = String.format("$ %15.2f", number);
                        LCD.Printonrow(result + " \n Please Swipe");
                        LCD.Printbottomrow("\n Insert or Tap Card");
                        toTransaction = true;
                        LED.status = Processing;
                        byte baTk1Buf[] = new byte[256];
                        byte baTk2Buf[] = new byte[256];
                        byte baTk3Buf[] = new byte[256];
                        MSR.msr.read(baTk1Buf, baTk2Buf, baTk3Buf);
                        new TransactionTask().execute();

                    }
                    if (LCD.pwdmode == true) {
                        if (LCD.key_pin == false)
                            LCD.key_pin = true;
                    }

                    if (LCD.godraw == true) {
                        LCD.drawtest2();
                    }

                }
            });

            Buttonf = (ImageButton) findViewById(R.id.ImageButtonf);
            Buttonf.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {
                    //LCD.drawtest();
                    //LCD.drawtest2();

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);
					/*
					if(Barcode.scan==false)
					{
						Barcode.StartIdleBehavior();
						Barcode.scan=true;
						Toast.makeText(HelloJni.this, "Open Barcode!", Toast.LENGTH_SHORT).show();
					}
					else
					{
						Barcode.EndIdleBehavior();
						Barcode.scan=false;
						Toast.makeText(HelloJni.this, "Close Barcode!", Toast.LENGTH_SHORT).show();
					}
					*/
                    Intent intent = new Intent("com.google.zxing.client.android.SCAN");
                    if (getPackageManager().queryIntentActivities(intent, PackageManager.MATCH_DEFAULT_ONLY).size() == 0)
                        Toast.makeText(HelloJni.this, "Plz  Play Store DL ZXing Barcode Scanner!", Toast.LENGTH_LONG).show();
                    else {
                        intent.putExtra("SCAN_MODE", "SCAN_MODE");

                        // 呼叫ZXing Scanner，完成動作後回傳 1 給 onActivityResult 的 requestCode 參數
                        startActivityForResult(intent, 1);
                    }

                }
            });


            Buttondown = (ImageButton) findViewById(R.id.ImageButtondown);
            Buttondown.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    //LCD.drawtest2();
                }
            });

            Buttonup = (ImageButton) findViewById(R.id.ImageButtonup);
            Buttonup.setOnClickListener(new Button.OnClickListener() {
                public void onClick(View arg0) {

                    sp_beep.play(music_buttonbeep, 1, 1, 0, 0, 1);

                    //LCD.drawtest2();
                }
            });
        }
    }

    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
        if (requestCode == 1) {
            if (resultCode == RESULT_OK) {
                // ZXing回傳的內容
                String contents = intent.getStringExtra("SCAN_RESULT");

                int Barcodeuart_len = contents.toString().length();
                System.out.println("Barcode len: " + Integer.toString(Barcodeuart_len));
                //System.out.println( "Barcode len: "+Barcode.Barcodeuart.len);
                if (Barcodeuart_len > 0 && toTransaction == false) {
                    LCD.amout_len = 3;

                    if (contents.toString().length() > 3) {
                        //int intValue = Integer.valueOf(contents.toString().substring(contents.toString().length()-3, contents.toString().length()));
                        int intValue = 0;
                        if (contents.toString().charAt(contents.toString().length() - 3) >= 0x30 && contents.toString().charAt(contents.toString().length() - 3) <= 0x39)
                            intValue += contents.toString().charAt(contents.toString().length() - 3) - 0x30;
                        else
                            intValue += 1;
                        intValue = intValue * 10;
                        if (contents.toString().charAt(contents.toString().length() - 2) >= 0x30 && contents.toString().charAt(contents.toString().length() - 2) <= 0x39)
                            intValue += contents.toString().charAt(contents.toString().length() - 2) - 0x30;
                        else
                            intValue += 2;
                        intValue = intValue * 10;
                        if (contents.toString().charAt(contents.toString().length() - 1) >= 0x30 && contents.toString().charAt(contents.toString().length() - 1) <= 0x39)
                            intValue += contents.toString().charAt(contents.toString().length() - 1) - 0x30;
                        else
                            intValue += 3;


                        LCD.save_amout[2] = (intValue % 10) * 0.01f;
                        intValue = intValue / 10;
                        LCD.save_amout[1] = (intValue % 10) * 0.01f;
                        intValue = intValue / 10;
                        LCD.save_amout[0] = (intValue % 10) * 0.01f;
                        intValue = intValue / 10;
                    } else {
                        LCD.save_amout[0] = 0.01f;
                        LCD.save_amout[1] = 0.02f;
                        LCD.save_amout[2] = 0.03f;
                    }
                    LCD.number = 0;
                    for (int i = 0; i < LCD.amout_len; i++) {
                        LCD.number = LCD.number * 10;
                        LCD.number += LCD.save_amout[i];
                    }
                    LCD.result = String.format("$ %15.2f", LCD.number);
                    LCD.Printbottomrow(LCD.result);
                    sp_beep.play(music_beep, 1, 1, 0, 0, 1);
                }

            } else if (resultCode == RESULT_CANCELED) {
                Toast.makeText(this, "Cancel Scanner", Toast.LENGTH_LONG).show();
            }
        }
    }

    final Runnable Musicrunnable = new Runnable() {
        public void run() {
            //sp.play(music, 1, 1, 0, 1, 1);
        }
    };


    private Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 0:
                    LCD.pic_setoff();
                    LCD.Printonrow("Please Enter PIN");
                    LCD.Printbottomrow("");
                    LCD.Setpwdmode(d_ON);
                    break;
                case 1:
                    LCD.pic_setoff();
                    LCD.amout_len = 0;
                    LCD.number = 0;
                    LCD.refresh();
                    LCD.Setpwdmode(d_OFF);


                    break;
                case 2:
                    LCD.Printbottomrow("Wrong PIN!!");
                    LCD.pwd_len = 0;
                    LCD.save_pwd = new int[12];
                    LCD.key_pin = false;
                    System.out.println("PIN FAIL!!");
                    break;
                case 3:
                    LCD.pic_seton();
                    break;
                case 4:
                    LCD.pic_seton();
                    ///ProgressDialog dialog = ProgressDialog.show(HelloJni.this, "","Loading. Please wait...", true);
                    //dialog.show();

                    ImageView imageView = (ImageView) findViewById(R.id.qr_code);
                    imageViewTarget = new GlideDrawableImageViewTarget(imageView);
                    Glide.with(getApplicationContext()).load(R.raw.image).into(imageViewTarget);
                    //LCD.showpic(R.drawable.processing);
                    break;
                case 5:
                    LCD.pic_seton();
                    //ProgressDialog dialog2 = ProgressDialog.show(HelloJni.this, "","Loading. Please wait...", true);
                    //dialog2.show();

                    ImageView imageView2 = (ImageView) findViewById(R.id.qr_code);
                    imageViewTarget = new GlideDrawableImageViewTarget(imageView2);
                    Glide.with(HelloJni.this).load(R.raw.image).into(imageViewTarget);
                    //LCD.showpic(R.drawable.processing);
                    break;
                case 6:
                    LCD.pic_setoff();
                    if (Enable_printer == true) {
                        if (touch == true)
                            LCD.drawtest2();
                        else
                            LCD.drawtest3();
                    }
                    break;
                case 7:
                    if (Enable_printer == true) {
                        LCD.drawtest3();
                    }
                    break;


            }


        }
    };

    private int EMVPolling() {
        int ret = 0;

        //System.out.println("in ICC");
        ret = ICC.Status();

        if (ret == 1) {
            //System.out.println("in MSRRead");
            ret = MSR.MSRRead();
        }
        return ret;
    }


    private boolean Transactionwait = false;
    private boolean cl_wait = false;

    private class TransactionTask extends AsyncTask<String, Void, Integer> {
        protected Integer doInBackground(String... urls) {

            int ret = 0;

            byte[] temp;
            int len;

            //ulAmt = ((float) (Math.round((LCD.number *100* 100)) / 100));


            len = LCD.amout_len / 2;
            len += LCD.amout_len % 2;
            temp = new byte[len];
            if ((LCD.amout_len % 2) == 1) {
                temp[0] = (byte) ((float) (Math.round((LCD.save_amout[0] * 100 * 100)) / 100));
                for (int i = 1; i < len; i++) {
                    temp[i] += (byte) ((float) (Math.round((LCD.save_amout[i * 2 - 1] * 10 * 100 + LCD.save_amout[i * 2] * 100) * 100)) / 100);
                }
            } else {
                for (int i = 0; i < len; i++) {
                    temp[i] += (byte) ((float) (Math.round((LCD.save_amout[i * 2] * 10 * 100 + LCD.save_amout[i * 2 + 1] * 100) * 100)) / 100);
                }
            }


            while (toTransaction == true) {


                ret = EMVPolling();

                if (ret == d_OK) {
                    LED.status = waiting_for_trade;
                    System.out.println("finish Transaction!!");
                    toTransaction = false;
                    //Message msg = new Message();
                    //msg.what = 1;
                    //mHandler.sendMessage(msg);
                    return 0;
                } else if (ret == d_PIN_FAIL) {
                    toTransaction = false;
                    //EMVCL.CancelTransaction();
                    Message msg = new Message();
                    msg.what = 1;
                    mHandler.sendMessage(msg);
                    return -1;
                } else if (ret == d_SIGN_FAIL) {
                    toTransaction = false;
                    return -1;
                }

                //bEMVCL.CLPowerOff();
                cl_wait = false;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Thread.sleep(10);
                        } catch (Exception e) {
                            e.printStackTrace();
                        } finally {
                            cl_wait = true;
                        }
                    }
                }).start();
                while (cl_wait == false) ;


                byte baCSN[] = new byte[10];
                ret = cltest.ppPolling(baCSN, 1);
                if (ret == d_OK) {
                    LED.status = trade_success;
                    Transactionwait = false;


                    Message msg = new Message();
                    msg.what = 4;
                    mHandler.sendMessage(msg);
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {

                                Thread.sleep(1000);
                            } catch (Exception e) {
                                e.printStackTrace();
                            } finally {
                                Transactionwait = true;
                            }
                        }
                    }).start();
                    while (Transactionwait == false) ;


                    Transactionwait = false;
                    msg = new Message();
                    msg.what = 5;
                    mHandler.sendMessage(msg);


                    try {
                        Thread.sleep(2000);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    while (!wait_temp) ;
                    Intent intent = new Intent();
                    intent.setClass(HelloJni.this, PayApprove.class);
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                    Bundle bundle = new Bundle();
                    bundle.putInt("total_price", temp_total_price);
                    bundle.putString("time_temp", time_temp);
                    bundle.putString("card_number", card_number);
                    bundle.putString("pay_number", String.valueOf(pay_number));
                    //bundle.putString("link", link);
                    bundle.putInt("food_1_count", food_1_count);
                    bundle.putInt("food_2_count", food_2_count);
                    bundle.putInt("food_3_count", food_3_count);
                    bundle.putInt("food_4_count", food_4_count);
                    bundle.putInt("food_5_count", food_5_count);
                    bundle.putInt("food_6_count", food_6_count);
                    intent.putExtras(bundle);
                    HelloJni.this.startActivity(intent);
//                    overridePendingTransition(0, 0);
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                System.out.println("finish Transaction!!");
                                Message msg = new Message();
                                msg.what = 7;
                                mHandler.sendMessage(msg);
                                Thread.sleep(1000);
                            } catch (Exception e) {
                                e.printStackTrace();
                            } finally {
                                Transactionwait = true;
                            }
                        }
                    }).start();

                    while (Transactionwait == false) ;


                    toTransaction = false;
                    msg = new Message();
                    msg.what = 1;
                    mHandler.sendMessage(msg);
                    finish();
                    return 0;
                }


    			/*
    			ret = EMVCL.PerformTransactionEx(EMVCL.stRCDataEx);
    			if (ret == d_EMVCL_TX_CANCEL) {
    				toTransaction = false;
                    return -1;
                } else if (ret == 0xA00000E0) {
                } else if (ret == d_EMVCL_RC_FAILURE) {
                	System.out.println("Read Card fail!");
                } else if (ret == d_EMVCL_RC_NO_CARD) {
                	System.out.println("no CL Card!");
                } else if (ret != d_EMVCL_PENDING) {

                	String RRR = "";
                	RRR = String.format("ret is 0x%08x",ret);
                	System.out.println(RRR);

                	System.out.println("read CL Card 1!");

                	if(ret != d_EMVCL_RC_DATA)
                		continue;

        			System.out.println("read CL Card 2!");
                    Transactionwait = false;
        			Message msg = new Message();
           	        msg.what = 4;
           	        mHandler.sendMessage(msg);
        	   	     new Thread(new Runnable(){
        	             @Override
        	             public void run() {
        	                 try{
        	                     Thread.sleep(2000);
        	                 }
        	                 catch(Exception e){
        	                     e.printStackTrace();
        	                 }
        	                 finally{
        	                	 Transactionwait = true;
        	                 }
        	             }
        	        }).start();
        	   	    while(Transactionwait==false);

        	   	 Transactionwait = false;
        	   	 msg = new Message();
        	        msg.what = 5;
        	        mHandler.sendMessage(msg);
           	     new Thread(new Runnable(){
                     @Override
                     public void run() {
                         try{
                             Thread.sleep(2000);
                         }
                         catch(Exception e){
                             e.printStackTrace();
                         }
                         finally{
                        	 Transactionwait = true;
                         }
                     }
                }).start();
           	    while(Transactionwait==false);


           	       toTransaction = false;
        		   msg = new Message();
        	        msg.what = 1;
        	        mHandler.sendMessage(msg);
        	        return 0;


                }
    			*/

            }
            ;


            return 0;

        }

    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
//        ActivityA=this;
        SysApplication.getInstance().addActivity(this);

		/*
		 * Create a TextView and set its content. the text is retrieved by
		 * calling a native function.
		 */
        // requestWindowFeature(Window.FEATURE_NO_TITLE);
/*
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        setContentView(R.layout.main);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE,R.layout.titlebar);
*/
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
//        ActionBar actionBar = getSupportActionBar();
//        actionBar.hide();
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen

        Bundle bundle = getIntent().getExtras();
        temp_total_price = bundle.getInt("total_price");
        time_temp = bundle.getString("time_temp");
        food_1_count = bundle.getInt("food_1_count");
        food_2_count = bundle.getInt("food_2_count");
        food_3_count = bundle.getInt("food_3_count");
        food_4_count = bundle.getInt("food_4_count");
        food_5_count = bundle.getInt("food_5_count");
        food_6_count = bundle.getInt("food_6_count");
        toTransaction = false;

        LCD = new CTOS_LCD();
        LED = new CTOS_LED();
        KBD = new CTOS_KBD();

        MSR = new CTOS_MSR();
        ICC = new CTOS_ICC();


        LCD.Init();
        LCD.draw_setoff();
        LED.Init();
        KBD.Init();
        MSR.Init();
        ICC.Init();


        LED.StartIdleLEDBehavior();

        int card_number_1 = (int) (Math.random() * 8999 + 1000);
        int card_number_2 = (int) (Math.random() * 8999 + 1000);
        int card_number_3 = (int) (Math.random() * 8999 + 1000);
        final int card_number_4 = (int) (Math.random() * 8999 + 1000);
        card_number = String.valueOf(card_number_1) + "-" + String.valueOf(card_number_2) + "-" + String.valueOf(card_number_3) + "-" + String.valueOf(card_number_4);


        sp_beep = new SoundPool(1, AudioManager.STREAM_MUSIC, 1);
        music_beep = sp_beep.load(HelloJni.this, R.raw.beep_success, 1);

        sp_buttonbeep = new SoundPool(1, AudioManager.STREAM_MUSIC, 1);
        music_buttonbeep = sp_beep.load(HelloJni.this, R.raw.buttonbeep, 1);

        ImageView LCD_welcome = (ImageView) findViewById(R.id.imageview_welcome);
        TableLayout LCD_touch = (TableLayout) findViewById(R.id.tablelayout_touch);
        if (touch == false) {
            LCD_welcome.setVisibility(View.VISIBLE);
            LCD_touch.setVisibility(View.GONE);
        } else {
            LCD_welcome.setVisibility(View.GONE);
            LCD_touch.setVisibility(View.VISIBLE);
        }


        System.out.println("Android Verson 17/05/04 15:23  V1.8");
        ImageButton temp = (ImageButton) findViewById(R.id.ImageButtonenter);
        temp.callOnClick();
        final boolean[] cancel_check = {false};
        cancle = (ImageView) findViewById(R.id.cancle);
        cancle.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                cancel_check[0] = true;
                Intent intent = new Intent();
                intent.setClass(HelloJni.this, PayCheck.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                bundle.putInt("total_price", temp_total_price);
                bundle.putInt("food_1_count", food_1_count);
                bundle.putInt("food_2_count", food_2_count);
                bundle.putInt("food_3_count", food_3_count);
                bundle.putInt("food_4_count", food_4_count);
                bundle.putInt("food_5_count", food_5_count);
                bundle.putInt("food_6_count", food_6_count);
                intent.putExtras(bundle);
                HelloJni.this.startActivity(intent);
//                overridePendingTransition(0, 0);
                HelloJni.this.finish();
                toTransaction = false;
//                Message msg = new Message();
//                msg.what = 1;
//                mHandler.sendMessage(msg);
            }
        });
        //LED.Approved=true;
        // TextView tv = new TextView(this);
        // tv.setText( stringFromJNI() );
        // tv.setText( TESTmsr);
        // setContentView(tv);
        new Thread(new Runnable() {
            @Override
            public void run() {
                pay_number = (int) (Math.random() * 1000000000);

                try {

                    Bitmap bmp = Bitmap.createBitmap(800
                            , 600
                            , Bitmap.Config.RGB_565
                    );
                    bmp.eraseColor(Color.WHITE);
                    FileOutputStream fos = new FileOutputStream("/mnt/sdcard/DCIM/sign_ok.jpg");
                    bmp.compress(Bitmap.CompressFormat.PNG, 100, fos);
                    bmp.recycle();
                    bmp = null;
                    fos.close();
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                Printer = new CTOS_Printer();
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
                OkHttpClient client = new OkHttpClient();
                MultipartBuilder builder = new MultipartBuilder();
                builder.type(MultipartBuilder.FORM);
                builder.addFormDataPart("transaction_id", String.valueOf(pay_number));
                builder.addFormDataPart("merchant_id", String.valueOf(pay_number));
                String item_id = "", item_name = "", item_count = "", item_unit = "", item_price = "";
                boolean check = false;
                if (food_1_count > 0) {
                    item_id += "1";
                    item_name += "hamburger";
                    item_count += food_1_count;
                    item_unit += "Pcs";
                    item_price += "45";
                    check = true;
                }
                if (check) {
                    item_id += "|";
                    item_name += "|";
                    item_count += "|";
                    item_unit += "|";
                    item_price += "|";
                    check = false;
                }
                if (food_2_count > 0) {
                    item_id += "2";
                    item_name += "hot dog";
                    item_count += food_2_count;
                    item_unit += "Pcs";
                    item_price += "35";
                    check = true;
                }
                if (check) {
                    item_id += "|";
                    item_name += "|";
                    item_count += "|";
                    item_unit += "|";
                    item_price += "|";
                    check = false;
                }
                if (food_3_count > 0) {
                    item_id += "3";
                    item_name += "chicken";
                    item_count += food_3_count;
                    item_unit += "Pcs";
                    item_price += "160";
                    check = true;
                }
                if (check) {
                    item_id += "|";
                    item_name += "|";
                    item_count += "|";
                    item_unit += "|";
                    item_price += "|";
                    check = false;
                }
                if (food_4_count > 0) {
                    item_id += "4";
                    item_name += "fries";
                    item_count += food_4_count;
                    item_unit += "Pcs";
                    item_price += "30";
                    check = true;
                }
                if (check) {
                    item_id += "|";
                    item_name += "|";
                    item_count += "|";
                    item_unit += "|";
                    item_price += "|";
                    check = false;
                }
                if (food_5_count > 0) {
                    item_id += "5";
                    item_name += "ice cream";
                    item_count += food_5_count;
                    item_unit += "Pcs";
                    item_price += "40";
                    check = true;
                }
                if (check) {
                    item_id += "|";
                    item_name += "|";
                    item_count += "|";
                    item_unit += "|";
                    item_price += "|";
                    check = false;
                }
                if (food_6_count > 0) {
                    item_id += "6";
                    item_name += "cola";
                    item_count += food_6_count;
                    item_unit += "Pcs";
                    item_price += "25";
                    check = true;
                }
                if (check) {
                    item_id += "|";
                    item_name += "|";
                    item_count += "|";
                    item_unit += "|";
                    item_price += "|";
                    check = false;
                }
                builder.addFormDataPart("item_id", item_id);
                builder.addFormDataPart("item_name", item_name);
                builder.addFormDataPart("item_count", item_count);
                builder.addFormDataPart("item_unit", item_unit);
                builder.addFormDataPart("item_price", item_price);
                builder.addFormDataPart("invoice_amt", String.valueOf(temp_total_price));
                builder.addFormDataPart("invoice_number", "11223344");
                builder.addFormDataPart("invoice_random_num", "9453");
                builder.addFormDataPart("invoice_create_time", time_temp);
                builder.addFormDataPart("invoice_barcode", "102030405060708090");
                builder.addFormDataPart("invoice_qrcode_lfet", "http://castles.life9453.com/payment/info/" + pay_number);
                builder.addFormDataPart("invoice_qrcode_right", "http://castles.life9453.com/payment/info/" + pay_number);
                builder.addFormDataPart("payment_merchant_id", "4201605361");
                builder.addFormDataPart("payment_terminal_id", "63513242");
                builder.addFormDataPart("payment_cardtype", card_number);
                builder.addFormDataPart("payment_batch_num", "379");
                builder.addFormDataPart("payment_auth_code", "047364");
                builder.addFormDataPart("payment_time", time_temp);
                builder.addFormDataPart("payment_card_last4", String.valueOf(card_number_4));
                builder.addFormDataPart("payment_total_amt", String.valueOf(temp_total_price));
                RequestBody fileBody = RequestBody.create(MediaType.parse("image/jpg"), new File("/mnt/sdcard/DCIM/gaozhen_08189.jpg"));
                builder.addFormDataPart("payment_img", "/mnt/sdcard/DCIM/gaozhen_08189.jpg", fileBody);
                RequestBody fileBody2 = RequestBody.create(MediaType.parse("image/jpg"), new File("/mnt/sdcard/DCIM/gaozhen_08188.jpg"));
                builder.addFormDataPart("invoice_img", "/mnt/sdcard/DCIM/gaozhen_08188.jpg", fileBody2);

                wait_temp = true;
                /*
                RequestBody body = builder.build();
                Request request = new Request.Builder().url("http://castles.life9453.com/api/webservice").post(body).build();
                try {
                    Response response = client.newCall(request).execute();
                    String temp = response.body().string();
                    String[] temp_2 = temp.split("\"qrcode_link\": \"");
                    String[] temp_3 = temp_2[1].split("\"");
                    link = temp_3[0];
                    wait_temp = true;
                } catch (IOException e) {
                    if (!cancel_check[0]) {
                        runOnUiThread(new Runnable() {
                            public void run() {
                                Toast.makeText(HelloJni.this, "Socket time out", Toast.LENGTH_SHORT).show();
                            }
                        });
                        Intent intent = new Intent();
                        intent.setClass(HelloJni.this, PayCheck.class);
//                        intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                        Bundle bundle = new Bundle();
                        bundle.putInt("total_price", temp_total_price);
                        bundle.putInt("food_1_count", food_1_count);
                        bundle.putInt("food_2_count", food_2_count);
                        bundle.putInt("food_3_count", food_3_count);
                        bundle.putInt("food_4_count", food_4_count);
                        bundle.putInt("food_5_count", food_5_count);
                        bundle.putInt("food_6_count", food_6_count);
                        intent.putExtras(bundle);
                        HelloJni.this.startActivity(intent);
//                        overridePendingTransition(0, 0);
                        HelloJni.this.finish();
                        toTransaction = false;
                        Message msg = new Message();
                        msg.what = 1;
                        mHandler.sendMessage(msg);
                        e.printStackTrace();
                    }

                }
                */


            }
        }).start();


    }

    public void DisplayAlertDialog_fail(String str_Result) {
        LayoutInflater inflater = LayoutInflater.from(HelloJni.this);
        View AlertDialog_view = inflater.inflate(R.layout.dialog_view_fail, null);

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("System fail");
        builder.setIcon(android.R.drawable.ic_delete);
        // builder.setIcon(android.R.drawable.star_big_on);

        // EditText Result_Edit = (EditText)
        // AlertDialog_view.findViewById(R.id.EditResult);
        TextView Result_View = (TextView) AlertDialog_view
                .findViewById(R.id.Result_fail);
        Result_View.setText(str_Result);

        // Result_View.settext
        builder.setView(AlertDialog_view);
        AlertDialog dialog = builder.create();

        dialog.setButton("OK", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {

                android.os.Process.killProcess(android.os.Process.myPid());

            }
        });

        dialog.show();

    }


    class MyOnTouchListener implements OnTouchListener {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            bottomrowText.setVisibility(View.GONE);
            int action = event.getAction();
            switch (action) {
                case MotionEvent.ACTION_DOWN:
                    is_touch = true;
                    System.out.println("down");
                    downX = (int) event.getX();
                    downY = (int) event.getY();
                    break;
                case MotionEvent.ACTION_MOVE:
                    System.out.println("move");
                    int moveX = (int) event.getX();
                    int moveY = (int) event.getY();
                    canvas.drawLine(downX, downY, moveX, moveY, paint);
                    draw_canvas.setImageBitmap(panel);
                    downX = moveX;
                    downY = moveY;
                    break;
                case MotionEvent.ACTION_UP:
                    System.out.println("up");
                    break;
                default:
                    break;
            }
            return true;
        }

    }

    private Bitmap mergeBitmap(Bitmap firstBitmap, Bitmap secondBitmap) {
        int newWidth = firstBitmap.getWidth() + secondBitmap.getWidth();
        Bitmap bitmap = Bitmap.createBitmap(newWidth, firstBitmap.getHeight(),
                firstBitmap.getConfig());
        Canvas tmpcanvas = new Canvas(bitmap);
        tmpcanvas.drawBitmap(firstBitmap, 0, 0, null);
        tmpcanvas.drawBitmap(secondBitmap, firstBitmap.getWidth(), 0, null);
        return bitmap;
    }

    private void InitPanel() {
        if (panel == null) {
            panel = Bitmap.createBitmap(draw_canvas.getWidth(), draw_canvas.getHeight(), Config.ARGB_8888);

            System.out.println(String.format("getWidth %d", draw_canvas.getWidth()));
            System.out.println(String.format("getHeight %d",
                    draw_canvas.getHeight()));
            canvas = new Canvas(panel);
            paint = new Paint();
            paint.setColor(Color.BLACK);
            //paint.setStyle(Paint.Style.FILL);
            paint.setStrokeWidth(5);
            draw_canvas.setImageBitmap(panel);
            bottomrowText.setVisibility(View.VISIBLE);
            LCD.savelcd = (String) bottomrowText.getText();
            bottomrowText.setText("Sign here!");
            is_touch = false;
        }
    }

    private void save_tupian() {

        // ??�画?��?��??�图??��?��?�起?��
        File cacheDir = Environment.getExternalStorageDirectory();
        File cacheFile = new File(cacheDir, "/DCIM/gaozhen_0819.png");
        System.out.println("rute:" + cacheFile.getPath());
        try {
            // FileOutputStream out=new FileOutputStream(cacheFile);
            // boolean isSuccess=panel.compress(CompressFormat.JPEG, 100, out);
            FileOutputStream fos = new FileOutputStream(cacheFile);
            boolean isSuccess = panel.compress(CompressFormat.PNG, 100, fos);
            if (isSuccess) {
                Toast.makeText(this, "save ok", 0).show();
                Toast.makeText(this, cacheFile.getPath(), 0).show();
                // MediaScannerConnection.scanFile(this, new
                // String[]{Environment
                // .getExternalStorageDirectory().getPath()+"/DCIM/gaozhen_0819.png"},
                // null, null);
            } else {
                Toast.makeText(this, "save fail", 0).show();
            }
        } catch (FileNotFoundException e) {

            e.printStackTrace();
        }
    }

    public class CTOS_Printer {
        int ret = 0;
        CtPrint Print;

        public void Init() {
            Print = new CtPrint();
        }

        public int goprintf() throws IOException {
            if (Chinese == true) {
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

                print_font = "BANK    00 一般交易 SALE";
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

                print_font = "總計(Total):     NT$" + temp_total_price;
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
                Print.drawText(print_liftx, print_y + Currently_high + 15, print_font, print_y);
                Currently_high += print_y - 70;


                print_font = "        X        ";
                Print.drawText(print_liftx, print_y + Currently_high + 55, print_font, print_y * 3);


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
            } else {
                int page_len = 920;
                if (food_1_count > 0)
                    page_len += 20;
                if (food_2_count > 0)
                    page_len += 20;
                if (food_3_count > 0)
                    page_len += 20;
                if (food_4_count > 0)
                    page_len += 20;
                if (food_5_count > 0)
                    page_len += 20;
                if (food_6_count > 0)
                    page_len += 20;

                Print.initPage(page_len);
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
                print_font = "...............................................";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 15;

                print_y = 16;
                print_font = "STORE: 0003         REGISTER: 001";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 3;

                print_font = "CASHIER: KATIE";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 3;

                print_font = "ASSOCIATE: 0000000";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_y = 26;
                print_font = "...............................................";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;


                print_y = 16;
                print_font = "CUSTOMER RECEIPT COPY";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 15;


                print_font = "";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "Card Type ";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "  MASTERCARD";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "Chech No. ";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "  90119";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "Card No. ";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = card_number + "(M)";
                print_y = 16;
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3 + 40;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "Host/Irans. Type";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "  BANK   00 GENERAL CONDITION  SALE";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "Batch No.";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "  379";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "Auth Code";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "  047364";
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;


                print_font = "DATE";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;
                print_font = "  " + time_temp;
                print_y = 16;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 10;

                print_y = 26;
                print_font = "...............................................";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y + 15;

                if (food_1_count > 0) {
                    print_font = String.format("HAMBURGER %3d*%2d ", 45, food_1_count);
                    print_y = 20;
                    Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);

                    print_font = String.format("%5d", 45 * food_1_count);
                    Print.drawText(300, print_y + Currently_high, print_font, print_y);
                    Currently_high += print_y;

                }

                if (food_2_count > 0) {
                    print_font = String.format("HOT DOG %3d*%2d ", 35, food_2_count);
                    print_y = 20;
                    Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);

                    print_font = String.format("%5d", 35 * food_2_count);
                    Print.drawText(300, print_y + Currently_high, print_font, print_y);
                    Currently_high += print_y;

                }

                if (food_3_count > 0) {
                    print_font = String.format("CHICKEN %3d*%2d ", 160, food_3_count);
                    print_y = 20;
                    Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);

                    print_font = String.format("%5d", 160 * food_3_count);
                    Print.drawText(300, print_y + Currently_high, print_font, print_y);
                    Currently_high += print_y;


                }

                if (food_4_count > 0) {
                    print_font = String.format("FRIES %3d*%2d ", 30, food_4_count);
                    print_y = 20;
                    Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);

                    print_font = String.format("%5d", 30 * food_4_count);
                    Print.drawText(300, print_y + Currently_high, print_font, print_y);
                    Currently_high += print_y;

                }

                if (food_5_count > 0) {
                    print_font = String.format("ICE CREAM %3d*%2d ", 40, food_5_count);
                    print_y = 20;
                    Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);

                    print_font = String.format("%5d", 40 * food_5_count);
                    Print.drawText(300, print_y + Currently_high, print_font, print_y);
                    Currently_high += print_y;

                }

                if (food_6_count > 0) {
                    print_font = String.format("COLA %3d*%2d ", 25, food_6_count);
                    print_y = 20;
                    Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);

                    print_font = String.format("%5d", 25 * food_6_count);
                    Print.drawText(300, print_y + Currently_high, print_font, print_y);
                    Currently_high += print_y;

                }


                Currently_high += print_y;
                print_font = "TOTAL AMOUNT";
                print_y = 20;
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);

                print_font = String.format("%5d", temp_total_price);
                Print.drawText(300, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;


                print_y = 26;
                print_font = "...............................................";
                Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
                Currently_high += print_y;

                print_font = "";
                print_y = 24;
                print_x = (384 - print_font.length() * print_y) / 2 + 1;
                print_x += print_font.length() * 3;
                Print.drawText(print_x, print_y + Currently_high, print_font, print_y, 1);
                Currently_high += print_y * 3 + 10;

                print_font = "Sign: _______________________";
                Print.drawText(print_liftx, print_y + Currently_high + 15, print_font, print_y);
                Currently_high += print_y - 70;


                print_font = "        X        ";
                Print.drawText(print_liftx, print_y + Currently_high + 55, print_font, print_y * 3);


                Currently_high += (print_y * 5);
                print_y = 16;
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
            print_x = (384 - print_font.length() * print_y) / 2 + 120;
            print_x += print_font.length() * 3;
            Print.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "電子發票證明聯";
            print_y = 32;
            print_x = (384 - print_font.length() * print_y) / 2 - 15;
            print_x += print_font.length() * 3;
            Print.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;
            print_font = "106年07-08月";
            print_x = (384 - print_font.length() * print_y) / 2 + 45;
            print_x += print_font.length() * 3;
            Print.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = "AB-11223344";
            print_x = (384 - print_font.length() * print_y) / 2 + 55;
            print_x += print_font.length() * 3;
            Print.drawText(print_x, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            print_font = time_temp;
            print_y = 20;
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;
            print_font = "隨機碼 : 9999　　　總計 $" + temp_total_price;
            print_x = (384 - print_font.length() * print_y) / 2 + 1;
            print_x += print_font.length() * 3;
            Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;
            print_font = "賣方23060248";
            Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y + 10;

            Rect src = new Rect(0, 0, 368, 1200);
            Rect dst = new Rect(40, Currently_high, 368 - 40, 1200 / 3 + Currently_high);
            Resources res = getResources();
            panel = BitmapFactory.decodeResource(res, R.drawable.recipy_pic);
            Print.drawImage(panel, src, dst);
            Currently_high += print_y + 50;

            String link_temp = "http://castles.life9453.com/payment/info/" + pay_number;
            QRCodeWriter writer = new QRCodeWriter();
            BitMatrix bitMatrix = null;
            try {
                bitMatrix = writer.encode(link_temp, BarcodeFormat.QR_CODE, 512, 512);
            } catch (WriterException e) {
                e.printStackTrace();
            }
            int width = bitMatrix.getWidth();
            int height = bitMatrix.getHeight();
            Bitmap bmp = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    bmp.setPixel(x, y, bitMatrix.get(x, y) ? Color.BLACK : Color.WHITE);
                }
            }

            Rect src2 = new Rect(0, 0, width, height);  //之前畫的圖
            Rect dst2 = new Rect(30, Currently_high, width / 3 + 30, height / 3 + Currently_high); //簽單
            Print.drawImage(bmp, src2, dst2);
            Rect dst3 = new Rect(190, Currently_high, width / 3 + 190, height / 3 + Currently_high); //簽單
            Print.drawImage(bmp, src2, dst3);
            //Rect  dst_3 = new Rect(40+100+40, Currently_high, 100, 1200/3+Currently_high);
            //Print.drawImage(bmp, src, dst_3);
            Currently_high += print_y + 150;


            print_font = "店號009999-機02-序999999999";
            Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y + 10;

            Rect src4 = new Rect(0, 0, 368, 1200);
            Rect dst4 = new Rect(40, Currently_high, 368 - 40, 1200 / 3 + Currently_high);
            Print.drawImage(panel, src4, dst4);
            Currently_high += print_y + 60;

            print_font = "* *退貨時請攜帶電子發票證明聯";
            Print.drawText(print_liftx, print_y + Currently_high, print_font, print_y);
            Currently_high += print_y;

            Print.saveJPEG("/mnt/sdcard/DCIM/gaozhen_08188.jpg");

            //Print.printPage();
            bmp.recycle();
            bmp = null;
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
