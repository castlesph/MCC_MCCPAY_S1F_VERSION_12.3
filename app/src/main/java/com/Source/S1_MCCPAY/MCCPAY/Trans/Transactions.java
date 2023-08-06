package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.OperatorHolder;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DPopupMenu;
import com.Source.S1_MCCPAY.MCCPAY.model.DPopupMenuActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.TxnPopupMenu;

import java.io.File;


public class Transactions extends DemoAppActivity {

    private boolean isFirstTrigger=true;
    private int flag=1, iHeader=0;
    private Button btn_title=null;
    private String TAG="MCC_Menu";

    private static Handler mHandler;

    public static String select_item;
    public String TrxList, AppHeaderBuff, TransLogoBuff, TransLogo, TrxListItems;
    public String[] transItems = new String[20];
    public int inCnt=0;


    private int inTimeOut = 60;

    private DPopupMenu mccMenu;
    private String MenuTitle, FullMenuItems;
    private String AppDetail1, AppDetail2, AppDetail, Networknm;
    private String[] TrxFunc = new String[100];
    private String[] AppHeader = new String[100];
    private String[] TrxMenuItems = new String[100];
    private String[] TrxTitle = new String[1000];

    private Button btn_can;

    TextView textView_appversion;
    TextView textView_merchant;
    TextView textView_tprofile;
    TextView textView_carrier_name;
    ImageView mcc_trans_logo;

    private String sLocalAppHeader;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i("SATURN Transactions", "Popup MCC Transactions Menu onCreate");

        overridePendingTransition(0, 0); // disable the animation, faster
        OperatorHolder operatorHolder = new OperatorHolder(this);
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo info = wifiManager.getConnectionInfo ();
        String ssid  = info.getSSID();

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
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        if(MainActivity.inTermType == 5)
            setContentView(R.layout.mcc_trans_menu_s1em);
        else
            setContentView(R.layout.mcc_trans_menu);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:Transactions>>WAKE_LOCK");
        this.mWakeLock.acquire();

        btn_can = (Button) findViewById(R.id.mcc_menu_can);

        textView_appversion = (TextView) findViewById(R.id.app_version);
        textView_merchant = (TextView) findViewById(R.id.merchant);
        textView_tprofile = (TextView) findViewById(R.id.tprofile);
        textView_carrier_name = (TextView) findViewById(R.id.NetworkCarrier);
        mcc_trans_logo = (ImageView) findViewById(R.id.mcc_trans_logo);

        //btn_can.setText("hello");
        Intent intent = getIntent();
        TrxList = intent.getStringExtra("pass_in_string");
        Log.i("main MenuBuff", TrxList);

        AppHeaderBuff = intent.getStringExtra("AppHeaderBuff");
        Log.i("main AppHeaderBuff", AppHeaderBuff);

        TransLogoBuff = TrxList.substring(0, TrxList.indexOf("|"));
        TrxListItems = TrxList.substring(TrxList.indexOf("|") + 1);
        Log.i("TransLogo", TransLogoBuff);
        Log.i("FullMenuTimes", TrxListItems);

        TransLogo = "/data/data/pub/"+TransLogoBuff;
        Log.i("main TransLogo", TransLogo);

        File txnlogo_bmp_file = new File(TransLogo);
        Bitmap txnlogo = BitmapFactory.decodeFile(txnlogo_bmp_file.getAbsolutePath());

        mcc_trans_logo.setImageBitmap(txnlogo);

        /*split out the menu items*/
        //MenuItems = FullMenuItems.split(" \n");
        TrxMenuItems = TrxListItems.split(" \n");
        AppHeader = AppHeaderBuff.split(" \n");
        iHeader = AppHeader.length;

        Log.i(TAG, "sLocalAppHeader: "+AppHeaderBuff);

        for(int indx=0; indx<iHeader; indx++)
        {
            switch (indx) {
                case 0:
                    textView_appversion.setText(AppHeader[indx]);
                    break;
                case 1:
                    textView_merchant.setText(AppHeader[indx]);
                    break;
                case 2:
                    AppDetail1 = "MID: "+AppHeader[indx];
                    break;
                case 3:
                    AppDetail2 = "TID: "+AppHeader[indx];
                    break;
            }
        }
        AppDetail = AppDetail1 + "  |  " + AppDetail2;
        Networknm = "Network: " + operatorHolder.getOperatorName();
        textView_tprofile.setText(AppDetail);

        if (ssid.equals("<unknown ssid>") || (ssid.length() <=0))
            Networknm = "Network: " + operatorHolder.getOperatorName();
        else
            Networknm = "Network: " + ssid;

        if (Networknm.equals("Network: "))
            Networknm = "Network: NOT CONNECTED";

        Log.i("AAA Networkname", "Networknm: " + Networknm);
        textView_carrier_name.setText(Networknm);

        btn_title = (Button) findViewById(R.id.mcc_menu_title);
        //btn_title.setText(MenuTitle);

        /*Start timer*/
        getTimerRestart();

        FuncKeyCancel();
/*
        btn_can.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("CANCEL", "CANCEL DPopupMenuActivity");
                getTimerCancel();
                popMenu.dismiss();
                select_item = "d_KBD_CANCEL";

                Log.i("TINE", "CANCEL DPopupMenuActivity");
                //startActivity(new Intent(DPopupMenuActivity.this, MainActivity.class));
                DPopupMenuActivity.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }
        });
*/

/*
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder builder = new AlertDialog.Builder(DPopupMenuActivity.this);
                builder.setTitle("Choose an animal");

                builder.setNegativeButton("cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                        //finish();
                        select_item = "TO";


                        DPopupMenuActivity.this.finish();

                        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                        synchronized (MainActivity.LOCK) {
                            MainActivity.LOCK.setCondition(true);
                            MainActivity.LOCK.notifyAll();
                        }
                    }
                });

                // add a list
                String[] animals = {"horse", "cow", "camel", "sheep", "goat"};
                builder.setItems(animals, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        switch (which) {
                            case 0: // horse
                            case 1: // cow
                            case 2: // camel
                            case 3: // sheep
                            case 4: // goat
                        }
                    }
                });

                AlertDialog dialog = builder.create();
                dialog.show();
            }
    });
*/


        btn_title.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                flag = 1;

                mccMenu = new DPopupMenu(getApplicationContext());


                //popMenu.addItem(new DPopupMenu.Item("选项一",0));
                //popMenu.addItem(new DPopupMenu.Item("选项二",0));
                //popMenu.addItem(new DPopupMenu.Item("选项三",0));
                //popMenu.addItem(new DPopupMenu.Item("选项四",0));

                Log.i("MCCPAY", "TrxMenuItems.length: " + TrxMenuItems.length);
                Log.i("MCCPAY", "TrxMenuItems: " + TrxMenuItems);

                for (int i = 0; i < TrxMenuItems.length; i++) {
                    System.out.println(TrxMenuItems[i]);
                    TrxTitle[i] = TrxMenuItems[i].substring(0,TrxMenuItems[i].indexOf("|"));
                    TrxFunc[i] = TrxMenuItems[i].substring(TrxMenuItems[i].indexOf("|") + 1);
                    Log.i("SATURN Trxn Title", TrxTitle[i]);
                    Log.i("SATURN Trxn Image ID", TrxFunc[i]);

                    mccMenu.addItem(new DPopupMenu.Item(TrxTitle[i], 0));

                }

                // 菜单项点击监听器
                mccMenu.setOnItemClickListener(popmenuItemClickListener);
                mccMenu.showAsDropDown(btn_title);
                //FuncKeyCancel();
                //popMenu.dismiss();
            }
        });

        //don't want change this sample code, so set auto click.
        new Handler().postDelayed(new Runnable() {

            public void run() {
                //button1.dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), MotionEvent.ACTION_BUTTON_PRESS , 0, 0, 0));
                //button1.dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), MotionEvent.ACTION_BUTTON_RELEASE , 0, 0, 0));
                Log.i("TINE", "Runnable DPopupMenuActivity");
                btn_title.callOnClick();
            }
        }, 100);

        //for testing only

    }

    private long mLastClickTime = 0;
    private AdapterView.OnItemClickListener popmenuItemClickListener = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            // mis-clicking prevention, using threshold of 1000 ms
            if (SystemClock.elapsedRealtime() - mLastClickTime < 1000){
                Log.i("PATRICK", "mLastClickTime < 1000");
                return;
            }
            mLastClickTime = SystemClock.elapsedRealtime();

			view.setClickable(false);
            /*
            switch (position) {
                case 0:
                    popMenu.dismiss();
                    break;
                case 1:
                    popMenu.dismiss();
                    break;
                case 2:
                    popMenu.dismiss();
                    break;
                case 3:
                    popMenu.dismiss();
                    break;
            }
            */
            getTimerCancel();
            mccMenu.dismiss();
            //select_item = String.valueOf(position+1);
            select_item = TrxFunc[position];
            Log.i("PATRICK", select_item);
            //startActivity(new Intent(DPopupMenuActivity.this,MainActivity.class));
            //use for s1f
            Transactions.this.finish();
			//use for s1f2
            //new Handler().postDelayed(new Runnable() {
            //@Override
            //    public void run() {
            //         Transactions.this.finish();
            //    }
            //}, 300);   
			//s1f2-end
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }


        }
    };


    public void FuncKeyCancel() {
        btn_can.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                getTimerCancel();
                mccMenu.dismiss();
                //Toast.makeText(getApplicationContext(), "Cancelling...", Toast.LENGTH_SHORT).show();

                select_item = "CANCEL";
                //startActivity(new Intent(AmountEntryActivity.this, MainActivity.class));
                Transactions.this.finish();

                Log.i("PATRICK", "DPopupMenuActivity KeyBoard btn_can");
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }

            }
        });
    }



    /**
     * 取消倒计时
     */
    public void getTimerCancel() {
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

            mccMenu.dismiss();
            select_item = "TO";

            //startActivity(new Intent(DPopupMenuActivity.this,MainActivity.class));


            Log.i("TINE", "Timeout DPopupMenuActivity");
            Transactions.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        getTimerCancel();
        mccMenu.dismiss();
        select_item = "CANCEL";
        //startActivity(new Intent(DPopupMenuActivity.this,MainActivity.class));
        Transactions.this.finish();
        Log.i("PATRICK", "BackPressed DPopupMenuActivity");

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }
    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        select_item = null;
        TrxList = null;
        AppHeaderBuff = null;
        TransLogoBuff = null;
        TransLogo = null;
        TrxListItems = null;
        mHandler = null;
        SysApplication.getInstance().removeActivity(this);



    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int ExeFunction(String szFuncName);
    public native int inCTOSS_Sale();
}
