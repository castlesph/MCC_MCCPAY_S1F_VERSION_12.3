package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.Source.S1_MCCPAY.MCCPAY.zbar.CaptureActivity;

public class ScanQRCode extends DemoAppActivity {

    private static final String TAG = "tagScanQRCode";
    Context context;

    TextView textView_txn;
    private Button btn_can;
    private Button btn_scan;
    private TextView tv_scanResult;

    public static String final_string = "";
    private int inTimeOut = 0;
    private static final int REQUEST_CODE_SCAN = 0x0000;

    public static String sQRCodeHeader = "";
    private static String sFunction = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.activity_scanqrcode);
        Intent intent = getIntent();
        String dispmsg = intent.getStringExtra("pass_in_string");

        textView_txn = (TextView) findViewById(R.id.textView_txn);
        btn_can = (Button) findViewById(R.id.btn_can);
        btn_scan = (Button) findViewById(R.id.btn_scan);
        tv_scanResult = (TextView) findViewById(R.id.tv_scanResult);

        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            //System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    sQRCodeHeader = textView_txn.getText().toString();
                    break;
                case 1://Add function
                    sFunction = dispmsginfo[inIdx];
                    break;
                case 2:
                    break;
            }
        }

        btn_can.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                //getTimerCancel();

                final_string = "0|000000|CANCEL";
                com.Source.S1_MCCPAY.MCCPAY.Trans.ScanQRCode.this.finish();

                Log.i("TINE", "CANCEL BUTTON");
                if(sFunction.equals("MANUAL EXPENSE")){

                }
                else {
                    // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                    synchronized (MainActivity.LOCK) {
                        MainActivity.LOCK.setCondition(true);
                        MainActivity.LOCK.notifyAll();

                    }
                }
            }
        });

        btn_scan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                switch (view.getId()) {
                    case R.id.btn_scan:
                        if (ContextCompat.checkSelfPermission(com.Source.S1_MCCPAY.MCCPAY.Trans.ScanQRCode.this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                            ActivityCompat.requestPermissions(com.Source.S1_MCCPAY.MCCPAY.Trans.ScanQRCode.this, new String[]{Manifest.permission.CAMERA}, 1);
                        } else {
                            goScan();
                        }
                        break;
                    default:
                        break;
                }
            }
        });


    }

    private void goScan() {
        Log.i(TAG, "goScan: run");
        Intent intent = new Intent(this, CaptureActivity.class);
        startActivityForResult(intent, REQUEST_CODE_SCAN);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        Log.i(TAG, "onRequestPermissionsResult: run");
        switch (requestCode) {
            case 1:
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    goScan();
                } else {
                    Toast.makeText(this, "Unable to scan QRCode！", Toast.LENGTH_SHORT).show();
                }
                break;
            default:
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        boolean fDoneScan = false;

        Log.i(TAG, "onActivityResult: run");
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case REQUEST_CODE_SCAN:
                if (resultCode == RESULT_OK) {
                    if (data != null) {
                        Bundle bundle = data.getExtras();
                        String result = bundle.getString(CaptureActivity.EXTRA_STRING);
                        tv_scanResult.setText(result);
                        fDoneScan = true;
                    }
                }
                break;
            default:
                break;
        }

        // Scan Complete
        if (fDoneScan)
        {
            Log.i(TAG, "onActivityResult: Scan complete");

            final_string = "1" + "|"  + tv_scanResult.getText().toString() + "|" + "CONFIRM";

            Log.i(TAG, "sFunction= "+sFunction);

            com.Source.S1_MCCPAY.MCCPAY.Trans.ScanQRCode.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            if(sFunction.equals("MANUAL EXPENSE")){

            }
            else
            {
                MainActivity.LOCK.setCondition(false);
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }

        }
    }

    public void getTimerCancel(){
        timer.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart()
    {
        timer.start();
    }

    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "Timer onFinish");

            //etInputStr.clear();
            final_string = "TO";

            //startActivity(new Intent(ScanQRCode.this, ItemDetailListView.class));

            Log.i("PATRICK", "Timeout ScanQRCode");
            com.Source.S1_MCCPAY.MCCPAY.Trans.ScanQRCode.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            MainActivity.LOCK.setCondition(false);
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {
        com.Source.S1_MCCPAY.MCCPAY.Trans.ScanQRCode.this.finish();

        Log.i("PATRICK", "Back");
    }

    @Override
    protected void onResume() {
        super.onResume();

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        final_string = null;
        context = null;
        SysApplication.getInstance().removeActivity(this);
    }

}
