package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.support.annotation.RequiresApi;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.google.zxing2.common.BitMatrix;
import com.google.zxing2.BarcodeFormat;
import com.google.zxing2.WriterException;
import com.google.zxing2.qrcode.QRCodeWriter;
import java.io.File;

public class ConfirmQRCodePHQR extends DemoAppActivity {
    private Context mContext;
    TextView tv_transtitle;
    TextView tv_issuername;
    TextView tv_formattedamt;
    //TextView tv_convertedamt;
    //TextView tv_qrcodeurl;

    Button btn_confirm;
    Button btn_cancel;
	Button btn_print;
    ImageView imageView;
    Bitmap myBitmap = null;
    File qrcode_bmp_file = null;

    public static String final_string2 = "TIME OUT";

    private int inTimeOut = 0;

    protected PowerManager.WakeLock mWakeLock;

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        //| View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        //| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        //| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        //| View.SYSTEM_UI_FLAG_FULLSCREEN);

        setContentView(R.layout.confirm_phqr);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-ConfirmQRCode");
        this.mWakeLock.acquire();

	//commneted to eliminate hang
        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        //final PowerManager pwm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        //this.mWakeLock = pwm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:ConfirmQRCode>>MCCPAY");
        //this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("dispmsg", dispmsg);

        // TextView
        tv_transtitle = (TextView) findViewById(R.id.textView_txn);
        tv_issuername = (TextView) findViewById(R.id.tv_issuername);
        tv_formattedamt = (TextView) findViewById(R.id.tv_formattedamt);
        //tv_convertedamt = (TextView) findViewById(R.id.tv_convertedamt);
        //tv_qrcodeurl = (TextView)findViewById(R.id.tv_qrcodeurl);
        btn_confirm = (Button) findViewById(R.id.btn_confirm);
        btn_cancel = (Button) findViewById(R.id.btn_cancel);
		btn_print = (Button) findViewById(R.id.button_print);

		
        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        // Show status bar
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        for (int inIdx = 0; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    tv_transtitle.setText(dispmsginfo[inIdx]);
                    break;
                case 1:
                    tv_formattedamt.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    //tv_convertedamt.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    //tv_qrcodeurl.setText(dispmsginfo[inIdx]);
                    GenerateQRCode(dispmsginfo[inIdx]);
                    break;
                case 4:
                    //tv_issuername.setText(dispmsginfo[inIdx]);
                        tv_issuername.setBackground(getResources().getDrawable(R.drawable.phqr));
                    break;
				case 5:
                    inTimeOut=Integer.parseInt(dispmsginfo[inIdx]);
					break;
            }
        }

        /*Start timer*/
        System.out.println("inTimeOut:"+ inTimeOut);
        //getTimerCancel();
        if(inTimeOut > 0) {
            /*Start timer*/
            getTimerRestart();
        }
        //getTimerRestart();

        btn_confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string2 = "CONFIRM";
                ConfirmQRCodePHQR.this.finish();

        		Log.i("ConfirmQRCodePHQR", final_string2);

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

		
        btn_print.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string2 = "PRINT";
                ConfirmQRCodePHQR.this.finish();

				Log.i("ConfirmQRCodePHQR", final_string2);

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

        btn_cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string2 = "CANCEL";
                ConfirmQRCodePHQR.this.finish();

				Log.i("ConfirmQRCodePHQR", final_string2);

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });
    }

    // sidumili: QRCode generation
    private void GenerateQRCode(String sURL)
    {
        QRCodeWriter writer = new QRCodeWriter();
        try {
            String link_temp = sURL;
            BitMatrix bitMatrix = writer.encode(link_temp, BarcodeFormat.QR_CODE, 512, 512);
            int width = bitMatrix.getWidth();
            int height = bitMatrix.getHeight();
            Bitmap bmp = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    bmp.setPixel(x, y, bitMatrix.get(x, y) ? Color.BLACK : Color.WHITE);
                }
            }

			Bitmap myLogo = BitmapFactory.decodeResource(getResources(), R.drawable.phqrlogo);

			Bitmap bmOverlay = Bitmap.createBitmap(bmp.getWidth(), bmp.getHeight(), bmp.getConfig());
			Canvas canvas = new Canvas(bmOverlay);
			//canvas.drawBitmap(myLogo, (bmp.getWidth() - myLogo.getWidth()) / 2, (bmp.getHeight() - myLogo.getHeight()) / 2, null);
			//canvas.drawBitmap(bmp, new Matrix(), null);

			canvas.drawBitmap(bmp, new Matrix(), null);
			canvas.drawBitmap(myLogo, 200, 200, null);


			((ImageView) findViewById(R.id.img_qrcode)).setImageBitmap(bmOverlay);
            //((ImageView) findViewById(R.id.img_qrcode)).setImageBitmap(bmp);

            writer = null;
            bmp = null;
            bitMatrix = null;

        } catch (WriterException e) {
            e.printStackTrace();
        }

    }

    CountDownTimer timer = null;

    /**
     * 取消倒计时
     */
    public void getTimerCancel() {
        if(timer!=null)
            timer.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart()
    {
        timer = new CountDownTimer(inTimeOut*1000, 1000) {
            public void onTick(long millisUntilFinished) {
                Log.i("Timer", "Timer onTick");
                Log.i("Timer", "seconds remaining: " + millisUntilFinished / 1000);
            }
            public void onFinish() {
                Log.i("Timer", "Timer onFinish");

                //etInputStr.clear();
                final_string2 = "TIME OUT";

                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

                Log.i("PATRICK", "Timeout UserInputString");
                ConfirmQRCodePHQR.this.finish();

				Log.i("ConfirmQRCodePHQR", final_string2);

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }
        };
        timer.start();
    }

    /*
    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "Timer onTick");
            Log.i("Timer", "seconds remaining: " + millisUntilFinished / 1000);
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "Timer onFinish");

            //etInputStr.clear();
            final_string = "TIME OUT";

            //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

            Log.i("PATRICK", "Timeout UserInputString");
            ConfirmQRCode.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {

    }
    */

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        final_string2 = null;
        mContext = null;
        //SysApplication.getInstance().removeActivity(this); - fix hang
    }

}
