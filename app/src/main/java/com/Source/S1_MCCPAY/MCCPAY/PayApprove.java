package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.bumptech.glide.Glide;
import com.bumptech.glide.request.target.GlideDrawableImageViewTarget;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

public class PayApprove extends DemoAppActivity {
    Handler RTC_Handler;
    TextView time;
    int total_price;
    String time_temp;
    String card_number;
    String pay_number;
    String link;
    ImageView imageView;
    ImageView check;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);

//        requestWindowFeature(Window.FEATURE_NO_TITLE);
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
        setContentView(R.layout.payapprove_main);
//        ActionBar actionBar = getSupportActionBar();
//        actionBar.hide();
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
        Bundle bundle = getIntent().getExtras();
        total_price = bundle.getInt("total_price");
        time_temp = bundle.getString("time_temp");
        card_number = bundle.getString("card_number");
        pay_number = bundle.getString("pay_number");
        //link = bundle.getString("link");
        link = "http://castles.life9453.com/payment/info/" + pay_number;
        TextView price = (TextView) findViewById(R.id.price);
        TextView card_number_text = (TextView) findViewById(R.id.card_number);
        card_number_text.setText("Mask PAN:   　  " + card_number);
        //final ProgressDialog dialog = ProgressDialog.show(PayApprove.this, "","Loading. Please wait...", true);
        //dialog.show();
        imageView = (ImageView) findViewById(R.id.qr_code);
        GlideDrawableImageViewTarget imageViewTarget = new GlideDrawableImageViewTarget(imageView);
        Glide.with(this).load(R.raw.image).into(imageViewTarget);



        /*
        final Bitmap[] bmp = new Bitmap[1];
        final boolean[] bitmaprecycle = {false};
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    QRCodeWriter writer = new QRCodeWriter();
                    BitMatrix bitMatrix = writer.encode(link, BarcodeFormat.QR_CODE, 512, 512);
                    int width = bitMatrix.getWidth();
                    int height = bitMatrix.getHeight();
                    bmp[0] = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
                    for (int x = 0; x < width; x++) {
                        for (int y = 0; y < height; y++) {
                            bmp[0].setPixel(x, y, bitMatrix.get(x, y) ? Color.BLACK : Color.WHITE);
                        }
                    }
                    runOnUiThread(new Runnable() {
                        public void run() {
                            imageView.setImageBitmap(bmp[0]);
                            bitmaprecycle[0] = true;
                            check.setVisibility(View.VISIBLE);
                        }

                    });

                    //System.out.println(link_2[0]);
                } catch (WriterException e) {
                    e.printStackTrace();
                }

                //dialog.dismiss();
            }
        }).start();
        */


        price.setText("Amount:                                       $" + total_price);


        time = (TextView) findViewById(R.id.time);
        time.setText("Transaction time:" + time_temp);
        RTC_Handler = new Handler();
        RTC_Handler.postDelayed(RTC_runnable, 500);
        check = (ImageView) findViewById(R.id.check);
        check.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View arg0) {
                Intent intent = new Intent();
                intent.setClass(PayApprove.this, Recipy.class);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                Bundle bundle = new Bundle();
                bundle.putInt("total_price", total_price);
                bundle.putString("time_temp", time_temp);
                bundle.putString("card_number", card_number);
                bundle.putString("pay_number", pay_number);
                intent.putExtras(bundle);
                PayApprove.this.startActivity(intent);
                finish();
//                overridePendingTransition(0, 0);
//                new Thread(new Runnable(){
//                    @Override
//                    public void run() {
//                        try{
//                            while(true) {
//                                Thread.sleep(1000);
//                                if (bitmaprecycle[0]) {
//                                    bmp[0].recycle();
//                                    bmp[0]=null;
//                                    break;
//                                }
//                            }
//                        }
//                        catch(Exception e){
//                            e.printStackTrace();
//                        }
//                    }
//                }).start();
            }
        });
    }

    final Runnable RTC_runnable = new Runnable() {
        public void run() {

            new Thread(new Runnable() {
                public void run() {
                    //RTC_Handler.postDelayed(RTC_runnable, 500);
                    RTCHandler.sendMessage(RTCHandler.obtainMessage());
                }
            }).start();

        }
    };


    Handler RTCHandler = new Handler() {
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            //time.setText(new Date().toString());
            imageView.setImageResource(R.drawable.offline_qrcode);
            check.setVisibility(View.VISIBLE);
        }
    };


    @Override
    public void onBackPressed() {
        ConfirmExit(); //呼叫ConfirmExit()函數
    }

//    public boolean onKeyDown(int keyCode, KeyEvent event) {
//
//        if ((keyCode == KeyEvent.KEYCODE_BACK)) {   //確定按下退出鍵
//
//            ConfirmExit(); //呼叫ConfirmExit()函數
//
//            return true;
//
//        }
//
//        return super.onKeyDown(keyCode, event);
//
//    }


    public void ConfirmExit() {

        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(PayApprove.this, "The transaction has been completed. ", Toast.LENGTH_SHORT).show();
            }
        });
    }
}
