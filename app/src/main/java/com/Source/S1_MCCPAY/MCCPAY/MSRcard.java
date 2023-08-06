package com.Source.S1_MCCPAY.MCCPAY;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

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
import com.squareup.okhttp.Request;
import com.squareup.okhttp.RequestBody;
import com.squareup.okhttp.Response;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


import CTOS.CtPrint;

import static com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity.Chinese;


public class MSRcard extends DemoAppActivity {

    static Activity ActivityA;
    Handler RTC_Handler;
    TextView time;
    int total_price;
    int pay_number;
    String time_temp;
    String card_number;
    String link;
    int count = 1;
    CTOS_Printer Printer;
    CTOS_Printer Printer2;

    Canvas canvas;
    Bitmap panel = null;
    boolean wait_temp = false;
    int food_1_count;
    int food_2_count;
    int food_3_count;
    int food_4_count;
    int food_5_count;
    int food_6_count;

    List<PointF> points = new ArrayList<PointF>();
    Handler Touch_Handler;
    Panel mPanel;
    private Paint mPaint = null;
    Bitmap bmp = Bitmap.createBitmap(800
            , 600
            , Bitmap.Config.RGB_565
    );

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        ActivityA = this;
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.msrcrad_main);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-MSRcard>>WAKE_LOCK");
        this.mWakeLock.acquire();

//        ActionBar actionBar = getSupportActionBar();
//        actionBar.hide();
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
        Bundle bundle = getIntent().getExtras();
        total_price = bundle.getInt("total_price");
        time_temp = bundle.getString("time_temp");
        card_number = bundle.getString("card_number");
        pay_number = (int) (Math.random() * 1000000000);
        link = "http://castles.life9453.com/payment/info/"+pay_number;
        //link = bundle.getString("link");
        food_1_count = bundle.getInt("food_1_count");
        food_2_count = bundle.getInt("food_2_count");
        food_3_count = bundle.getInt("food_3_count");
        food_4_count = bundle.getInt("food_4_count");
        food_5_count = bundle.getInt("food_5_count");
        food_6_count = bundle.getInt("food_6_count");

        mPanel = new Panel(this);
        Touch_Handler = new Handler();
        bmp.eraseColor(Color.WHITE);
        final LinearLayout linearLayout = (LinearLayout) findViewById(R.id.sign);
        linearLayout.addView(mPanel);
        final ImageView check = (ImageView) findViewById(R.id.check);

        final String[] item_id = {""};
        final String[] item_name = {""};
        final String[] item_count = {""};
        final String[] item_unit = {""};
        final String[] item_price = {""};
        new Thread(new Runnable() {
            @Override
            public void run() {
                Printer2 = new CTOS_Printer();
                Printer2.Init();
                try {
                    Printer2.goprintf2();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                boolean check = false;
                if (food_1_count > 0) {
                    item_id[0] += "1";
                    item_name[0] += "hamburger";
                    item_count[0] += food_1_count;
                    item_unit[0] += "Pcs";
                    item_price[0] += "45";
                    check = true;
                }
                if (check) {
                    item_id[0] += "|";
                    item_name[0] += "|";
                    item_count[0] += "|";
                    item_unit[0] += "|";
                    item_price[0] += "|";
                    check = false;
                }
                if (food_2_count > 0) {
                    item_id[0] += "2";
                    item_name[0] += "hot dog";
                    item_count[0] += food_2_count;
                    item_unit[0] += "Pcs";
                    item_price[0] += "35";
                    check = true;
                }
                if (check) {
                    item_id[0] += "|";
                    item_name[0] += "|";
                    item_count[0] += "|";
                    item_unit[0] += "|";
                    item_price[0] += "|";
                    check = false;
                }
                if (food_3_count > 0) {
                    item_id[0] += "3";
                    item_name[0] += "chicken";
                    item_count[0] += food_3_count;
                    item_unit[0] += "Pcs";
                    item_price[0] += "160";
                    check = true;
                }
                if (check) {
                    item_id[0] += "|";
                    item_name[0] += "|";
                    item_count[0] += "|";
                    item_unit[0] += "|";
                    item_price[0] += "|";
                    check = false;
                }
                if (food_4_count > 0) {
                    item_id[0] += "4";
                    item_name[0] += "fries";
                    item_count[0] += food_4_count;
                    item_unit[0] += "Pcs";
                    item_price[0] += "30";
                    check = true;
                }
                if (check) {
                    item_id[0] += "|";
                    item_name[0] += "|";
                    item_count[0] += "|";
                    item_unit[0] += "|";
                    item_price[0] += "|";
                    check = false;
                }
                if (food_5_count > 0) {
                    item_id[0] += "5";
                    item_name[0] += "ice cream";
                    item_count[0] += food_5_count;
                    item_unit[0] += "Pcs";
                    item_price[0] += "40";
                    check = true;
                }
                if (check) {
                    item_id[0] += "|";
                    item_name[0] += "|";
                    item_count[0] += "|";
                    item_unit[0] += "|";
                    item_price[0] += "|";
                    check = false;
                }
                if (food_6_count > 0) {
                    item_id[0] += "6";
                    item_name[0] += "cola";
                    item_count[0] += food_6_count;
                    item_unit[0] += "Pcs";
                    item_price[0] += "25";
                    check = true;
                }
                if (check) {
                    item_id[0] += "|";
                    item_name[0] += "|";
                    item_count[0] += "|";
                    item_unit[0] += "|";
                    item_price[0] += "|";
                    check = false;
                }

            }
        }).start();

        final boolean[] check_only_once = {true};
        check.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                if (check_only_once[0] == true) {
                    check_only_once[0] = false;
                    ImageView imageView = (ImageView) findViewById(R.id.qr_code);
                    GlideDrawableImageViewTarget imageViewTarget = new GlideDrawableImageViewTarget(imageView);
                    Glide.with(MSRcard.this).load(R.raw.image).into(imageViewTarget);
                    try {
                        FileOutputStream fos = new FileOutputStream("/mnt/sdcard/DCIM/sign_ok.jpg");
                        bmp.compress(Bitmap.CompressFormat.PNG, 100, fos);
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

                    final OkHttpClient client = new OkHttpClient();
                    MultipartBuilder builder = new MultipartBuilder();
                    builder.type(MultipartBuilder.FORM);
                    builder.addFormDataPart("transaction_id", String.valueOf(pay_number));
                    builder.addFormDataPart("merchant_id", String.valueOf(pay_number));
                    builder.addFormDataPart("item_id", item_id[0]);
                    builder.addFormDataPart("item_name", item_name[0]);
                    builder.addFormDataPart("item_count", item_count[0]);
                    builder.addFormDataPart("item_unit", item_unit[0]);
                    builder.addFormDataPart("item_price", item_price[0]);
                    builder.addFormDataPart("invoice_amt", String.valueOf(total_price));
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
                    builder.addFormDataPart("payment_card_last4", card_number.substring(16));
                    builder.addFormDataPart("payment_total_amt", String.valueOf(total_price));
                    RequestBody fileBody = RequestBody.create(MediaType.parse("image/jpg"), new File("/mnt/sdcard/DCIM/gaozhen_08189.jpg"));
                    builder.addFormDataPart("payment_img", "/mnt/sdcard/DCIM/gaozhen_08189.jpg", fileBody);
                    RequestBody fileBody2 = RequestBody.create(MediaType.parse("image/jpg"), new File("/mnt/sdcard/DCIM/gaozhen_08188.jpg"));
                    builder.addFormDataPart("invoice_img", "/mnt/sdcard/DCIM/gaozhen_08188.jpg", fileBody2);

                    RequestBody body = builder.build();
                    final Request request = new Request.Builder().url("http://castles.life9453.com/api/webservice").post(body).build();
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                Response response = client.newCall(request).execute();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();
                    link = "http://castles.life9453.com/payment/info/" + pay_number;
                    //ProgressDialog dialog = ProgressDialog.show(MSRcard.this, "","Loading. Please wait...", true);
                    //dialog.show();
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MSRcard.this, PayApprove.class);
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                    bundle.putInt("total_price", total_price);
                    bundle.putString("time_temp", time_temp);
                    bundle.putString("card_number", card_number);
                    bundle.putString("pay_number", String.valueOf(pay_number));
                    bundle.putString("link", link);
                    bundle.putInt("food_1_count", food_1_count);
                    bundle.putInt("food_2_count", food_2_count);
                    bundle.putInt("food_3_count", food_3_count);
                    bundle.putInt("food_4_count", food_4_count);
                    bundle.putInt("food_5_count", food_5_count);
                    bundle.putInt("food_6_count", food_6_count);
                    intent.putExtras(bundle);
                    MSRcard.this.startActivity(intent);
                    finish();
//                    overridePendingTransition(0, 0);
                }
            }
        });

        ImageView refresh = (ImageView) findViewById(R.id.refresh);
        refresh.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
//                Toast.makeText(MSRcard.this, "Please Sign First !", Toast.LENGTH_SHORT).show();
                mPanel.cleanCanvas();
//                Intent intent = new Intent();
//                Bundle bundle = new Bundle();
//                intent.setClass(MSRcard.this, MSRcard.class);
////                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
//                bundle.putInt("total_price", total_price);
//                bundle.putString("time_temp", time_temp);
//                bundle.putString("card_number", card_number);
//                bundle.putString("link", link);
//                bundle.putInt("food_1_count", food_1_count);
//                bundle.putInt("food_2_count", food_2_count);
//                bundle.putInt("food_3_count", food_3_count);
//                bundle.putInt("food_4_count", food_4_count);
//                bundle.putInt("food_5_count", food_5_count);
//                bundle.putInt("food_6_count", food_6_count);
//                intent.putExtras(bundle);
//                MSRcard.this.startActivity(intent);
////                overridePendingTransition(0, 0);
//                MSRcard.this.finish();
//                overridePendingTransition(0, 0);
            }
        });
    }

    class Panel extends View {

        private int mov_x;
        private int mov_y;
        Canvas vBitmapCanvas = new Canvas(bmp);

        public Panel(Context context) {
            super(context);
            mPaint = new Paint();
            mPaint.setColor(Color.BLUE);
            mPaint.setStyle(Paint.Style.FILL);
            mPaint.setAntiAlias(true);
            mPaint.setStrokeJoin(Paint.Join.ROUND);    // set the join to round you want
            mPaint.setStrokeCap(Paint.Cap.ROUND);      // set the paint cap to round too
            mPaint.setStrokeWidth(15);  //粗度
        }

        @Override
        public void onDraw(Canvas canvas) {

            super.onDraw(canvas);
            canvas.drawBitmap(bmp, 0, 0, null);
            vBitmapCanvas.drawBitmap(bmp, 0, 0, null);
        }

        final Runnable Touch_runnable = new Runnable() {
            public void run() {

                new Thread(new Runnable() {
                    public void run() {
                        try {
                            //Thread.sleep(100);
                            DisplayHandler.sendMessage(DisplayHandler.obtainMessage());
                        } catch (Exception e) {
                            e.printStackTrace();
                        }

                    }
                }).start();

            }
        };


        Handler DisplayHandler = new Handler() {
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                setTitle("Wait touch...");
            }
        };


        @Override
        public boolean onTouchEvent(android.view.MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                mov_x = (int) event.getX();
                mov_y = (int) event.getY();
                // 画点
                vBitmapCanvas.drawPoint(mov_x, mov_y, mPaint);
                invalidate();
            }

            // 如果拖动
            if (event.getAction() == MotionEvent.ACTION_MOVE) {
                vBitmapCanvas.drawLine(mov_x, mov_y, event.getX(), event.getY(), mPaint);
                invalidate();
            }
            mov_x = (int) event.getX();
            mov_y = (int) event.getY();

            // return值必须为true，才能画连续曲线
            return true;
        }

        public void cleanCanvas() {
            bmp = Bitmap.createBitmap(800
                    , 600
                    , Bitmap.Config.RGB_565
            );

            bmp.eraseColor(Color.WHITE);

            vBitmapCanvas = new Canvas(bmp);

            mPaint = new Paint();
            mPaint.setColor(Color.BLUE);
            mPaint.setStyle(Paint.Style.FILL);
            mPaint.setAntiAlias(true);
            mPaint.setStrokeJoin(Paint.Join.ROUND);    // set the join to round you want
            mPaint.setStrokeCap(Paint.Cap.ROUND);      // set the paint cap to round too
            mPaint.setStrokeWidth(15);  //粗度
            invalidate();
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
                Print.drawText(print_liftx, print_y + Currently_high + 15, print_font, print_y);
                Currently_high += print_y - 90;

                File file = new File("/mnt/sdcard/DCIM/sign_ok.jpg");
                Bitmap panel = BitmapFactory.decodeFile(file.getAbsolutePath());
                Rect src = new Rect(0, 0, panel.getWidth(), panel.getHeight());  //之前畫的圖
                Rect dst = new Rect(70, Currently_high, panel.getWidth() / 3 * 3 / 4 + 70, panel.getHeight() / 3 / 2 + Currently_high); //簽單
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
            } else {
                int page_len = 940;
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

                print_font = String.format("%5d", total_price);
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
                Currently_high += print_y - 90;

                File file = new File("/mnt/sdcard/DCIM/sign_ok.jpg");
                Bitmap panel = BitmapFactory.decodeFile(file.getAbsolutePath());
                Rect src = new Rect(0, 0, panel.getWidth(), panel.getHeight());  //之前畫的圖
                Rect dst = new Rect(70, Currently_high, panel.getWidth() / 3 * 3 / 4 + 70, panel.getHeight() / 3 / 2 + Currently_high); //簽單
                Print.drawImage(panel, src, dst);  //把之前畫的圖和簽單合再一起
                Currently_high += (print_y * 5);

                panel.recycle();
                panel = null;
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
            print_font = "隨機碼 : 9999　　　總計 $" + total_price;
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

    @Override
    public void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
//        Runtime.getRuntime().gc();
        bmp.recycle();
        bmp = null;
        SysApplication.getInstance().removeActivity(this);
    }
}
