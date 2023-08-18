package com.Source.S1_MCCPAY.MCCPAY.Main;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.AnimationDrawable;
import android.graphics.drawable.BitmapDrawable;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkInfo;
import android.net.NetworkRequest;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.PowerManager;
import android.preference.PreferenceManager;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.RequiresApi;
import android.support.constraint.ConstraintLayout;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.DMenu.Model;
import com.Source.S1_MCCPAY.MCCPAY.DMenu.GridViewAdapter;
import com.Source.S1_MCCPAY.MCCPAY.DMenu.ViewPagerAdapter;
import com.Source.S1_MCCPAY.MCCPAY.EditDatabaseActivity;

import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.Trans.ProcessTransDetailUI;
import com.Source.S1_MCCPAY.MCCPAY.Trans.S1InputString;
import com.Source.S1_MCCPAY.MCCPAY.Trans.ScanQRCode;
import com.Source.S1_MCCPAY.MCCPAY.Trans.TransDetailAdapter;
import com.Source.S1_MCCPAY.MCCPAY.Trans.TransDetailListView;
import com.Source.S1_MCCPAY.MCCPAY.Trans.UserConfirmCard;
import com.Source.S1_MCCPAY.MCCPAY.Trans.UserConfirmVoid;
import com.Source.S1_MCCPAY.MCCPAY.model.ConfirmMenu;
import com.Source.S1_MCCPAY.MCCPAY.model.DOptionMenu;
import com.Source.S1_MCCPAY.MCCPAY.model.DPopupMenuActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.Source.S1_MCCPAY.MCCPAY.model.EliteReceiptonScreen;
import com.Source.S1_MCCPAY.MCCPAY.model.NormalKeyBoard;
import com.Source.S1_MCCPAY.MCCPAY.model.DPopupMenu;
import com.Source.S1_MCCPAY.MCCPAY.model.UserInputString;
import com.Source.S1_MCCPAY.MCCPAY.model.UpDownMenu;
import com.Source.S1_MCCPAY.MCCPAY.utility.GetAppInfor;
import com.Source.S1_MCCPAY.MCCPAY.utility.TemporaryData;
import com.bumptech.glide.gifencoder.AnimatedGifEncoder;
import com.Source.S1_MCCPAY.MCCPAY.Trans.AmountEntryActivity;
import com.Source.S1_MCCPAY.MCCPAY.Trans.BatchReview;
import com.Source.S1_MCCPAY.MCCPAY.model.ConfirmOKMenu;
import com.Source.S1_MCCPAY.MCCPAY.model.PrintReceiptonScreen;
import com.Source.S1_MCCPAY.MCCPAY.model.ConfirmMenu2;
import com.Source.S1_MCCPAY.MCCPAY.Trans.Transactions;
import com.Source.S1_MCCPAY.MCCPAY.OperatorHolder;
import com.Source.S1_MCCPAY.MCCPAY.Trans.ConfirmQRCode;
import com.Source.S1_MCCPAY.MCCPAY.Trans.ConfirmQRCodePHQR;


import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

import android.os.CountDownTimer;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import CTOS.CtPrint;
import pl.droidsonroids.gif.GifDrawable;
import pl.droidsonroids.gif.GifImageView;

import static android.content.ContentValues.TAG;
import static android.provider.ContactsContract.Intents.Insert.ACTION;
//import splashapp.android.nttd.cas.com.mylibrary.CtLed;

import java.io.FileInputStream;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import com.Source.S1_MCCPAY.MCCPAY.Kms.KMSUI;
import com.Source.S1_MCCPAY.MCCPAY.wub_lib;

import CTOS.CtSystem;
import CTOS.CtSystemException; 

import java.security.KeyFactory;
import java.security.Signature;
import java.security.spec.PKCS8EncodedKeySpec;
import java.util.Base64;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import com.Source.S1_MCCPAY.MCCPAY.Trans.BatchReview2;





public class MainActivity extends DemoAppActivity implements View.OnClickListener {
    public static boolean Chinese = false;
    public static int Finish_Printf = 0;


    private static Handler mHandler;
    private static Context mContext;

    private TextView title, result;
    private ImageView refresh, check;
    private ImageView[] food = new ImageView[6];
    private TextView[] food_count = new TextView[6];

    private int[] foodPickMount = new int[6];
    private int[] foodPrice = {45, 35, 160, 30, 40, 25};
    private int[] inPaddingLeft = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    private int total_price = 0;
    int ctlsdemo = 0;
    private String in_string="";

	public static final String ALGORITHM = "HmacSHA256";

	
    TextView app_version;
    TextView merchant;
    TextView tprofile;
    TextView carrier_name;
    Button btn_start;
	
    public String AppHeaderBuff;
	public static boolean fSTART = false;

    public String sLocalAppHeaderDetails;
    public String[] sLocalAppHeader = new String[100];

    @Override
    public FileInputStream openFileInput(String name) throws FileNotFoundException {
        return super.openFileInput(name);
    }


 
    int inKeySet = 0;
    int inKeyIndex= 0;
    int inKeyType = 0;
    int inPinBypassAllow = 0;
    String StrInData= "";
    public static String final_string;
    public static int Enter_Press;
    public static int inRet;
    public static String PinBlock;
    public static String KSN;

    public static String sPinBlock = "";
    public static String sKSN = "";
    public static String OutputStr = "";

    Activity activity = this;
    Button startButton;
    Button startButton2;
    EditText PinDigitET;
    final KMSUI kmsui = new KMSUI();

    TextView tv;
    EditText edtLog;
    Thread test;
    String list[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    ListView lsv_LCDTPrint;
    ArrayAdapter adapter;
    String displayMode[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    int addPadding[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    boolean isInitialize = false;

    Double dAmount;
    EditText etAmount;
    EditText etAmount1;
    private String amt_string="";

    private int uiFlag = 0;

    private String pass_in_string="";
    private DPopupMenu popMenu;
    AdapterView mAdapter[];

    /* Database */
//    private Cursor employees;
//    private MyDatabase db;

    /*
    SD or flash card write
     */
    private EditText editname;
    private EditText editdetail;
    private Button btnsave;
    private Button btnclean;
    private Button btnread;
    private Button btnauto;

    int inGravity;


    TextView tvMsgLine01;
    TextView tvMsgLine02;
    TextView tvMsgLine03;
    TextView tvMsgLine04;
    TextView tvMsgLine05;
    TextView tvMsgLine06;
    TextView tvMsgLine07;
    TextView tvMsgLine08;
    TextView tvMsgLine09;
    TextView tvMsgLine10;
    TextView tvMsgLine11;
    TextView tvMsgLine12;
    TextView tvMsgLine13;
    TextView tvMsgLine14;
    TextView tvMsgLine15;
    TextView tvMsgLine16;

    ImageView imageView;


    /* Dmenu */
    private String[] titles = {"INIT WAVE", "EditDB", "Credit Sale", "Void Sale", "Offline", "Refund", "Tip Adjust", "Authorize", "Sale Complete", "EPP Sale", "EPP Void", "SETTLEMENT",
             "Clear Reversal", "Last Receipt", "Last Settle",
		"ANY RECEIPT", "DETAIL REPORT", "SUMMARY REPORT", "SETTLE ALL", "VOID PREAUTH", "BATCH TOTAL", "BATCH REVIEW", "OFFICIAL RECEIPT"};

    private ViewPager mPager;
    private List<View> mPagerList;
    private List<Model> mDatas;
    private LinearLayout mLlDot;
    private LayoutInflater inflater;
    Button button;
    int intransCount = 0;
    /**
     * 总的页数
     */
    private int pageCount;
    /**
     * 每一页显示的个数
     */
    private int pageSize = 35;
    /**
     * 当前显示的是第几页
     */
    private int curIndex = 0;

    private boolean isHide = false;

    boolean isFirstRun;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("crypto");
    }

    static {
        System.loadLibrary("ssl");
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    private JNI_Lib jni_api = new JNI_Lib();
    private JNICB_implements_CTOS jni_cb_ctos = new JNICB_implements_CTOS(this);
    private JNICB_implements_IPT jni_cb_ipt = new JNICB_implements_IPT(this);

    public	static int inTermType;
    public static int inPrintConfirmTimeOut=10;
	
	public void vdClearFile(String fileName) {
	
		Log.i(TAG, "saturn mccpay delete file" + fileName);
	//			  String fileName = "/data/data/pub/TERMINAL.S3DB";
		try {
			File file = new File(fileName);
			if (!file.exists()) {
				Log.i(TAG,	"["  + fileName + "]" + "  Not exist");
			} else {
				if (file.isFile()) {
					Log.i(TAG, "file exist: " + fileName + "  Deleting...");
					boolean ret = false;
					ret = file.delete();
					Log.i(TAG, "SATURN del result:: " + ret);
	//											  file.deleteOnExit();
					File file1 = new File(fileName);
					if (!file1.exists()) {
						Log.i(TAG, "Del file:" +  "Not exist" + "  expected!!!");
					}
	//											  ret = deleteFile(fileName);
					Log.i(TAG, "del result: " + ret);
				}
			}
		} catch (Exception e) {
			Log.e(TAG, "exception: " + e.getMessage());
			e.printStackTrace();
		}
	
	}

    private void copyFileOrDir(String path) {
        AssetManager assetManager = this.getAssets();
        String assets[] = null;
        try {
            Log.i("SATURN", "MCCPAY copyFileOrDir() " + path);
            assets = assetManager.list(path);
            if (assets.length == 0) {
                copyFile(path);
            } else {
                for (int i = 0; i < assets.length; ++i) {
                    String p;
                    if (path.equals(""))
                        p = "";
                    else
                        p = path + "/";

                    if (!path.startsWith("images") && !path.startsWith("sounds") && !path.startsWith("webkit"))
                        copyFileOrDir(p + assets[i]);
                }
            }
        } catch (IOException ex) {
            Log.e("Castles", "I/O Exception", ex);
        }
    }

    public int chmod(File path, int mode) throws Exception {
        Class fileUtils = Class.forName("android.os.FileUtils");
        Method setPermissions =
                fileUtils.getMethod("setPermissions", String.class, int.class, int.class, int.class);
        return (Integer) setPermissions.invoke(null, path.getAbsolutePath(), mode, -1, -1);
    }

	private void copyFile(String filename) {
		AssetManager assetManager = this.getAssets();
		int inFlagFs_dataSkip = 0;

		Log.i(TAG, "copyFile: run");
		InputStream in = null;
		OutputStream out = null;
		String newFileName = null;

		String strFilename = filename.substring(filename.lastIndexOf("/") + 1);
		Log.i("Castles", "strFilename() " + strFilename);

		try {
			Log.i("Castles", "copyFile() " + filename);

			if (filename.toLowerCase().startsWith("21"))
			{
				newFileName = "/data/data/pub/" + strFilename;
			}
			else if (filename.toLowerCase().startsWith("11"))
			{
				newFileName = "/data/data/font/" + strFilename;
			}
			else if (filename.toLowerCase().startsWith("55"))
			{
				newFileName = "/data/data/pub/" + strFilename;
			}
			else if (filename.toLowerCase().startsWith("56"))
			{
				newFileName = "/data/data/pub/" + strFilename;		
			}
			else if (filename.toLowerCase().startsWith("04"))
			{
				inCTOSS_CreatefsdataFolder();

				newFileName = "/data/data/" + this.getPackageName() + "/fs_data/" + strFilename;
				Log.i(TAG, "copyFile: strFilename " + strFilename  +  " newFileName: " + newFileName);

				//if (inCTOSS_fGetECRFlg() != 1)
				//{
				/*
					if(strFilename.equals("castleslogo.bmp"))
					{
						newFileName = "/data/data/" + this.getPackageName() + "/fs_data/" + "logo.bmp";
						Log.i(TAG, "copyFile: strFilename " + strFilename  +  " newFileName: " + newFileName);
					}
					else if(strFilename.equals("logo.bmp"))
					{
						Log.i(TAG, "copyFile: return strFilename " + strFilename  +  " newFileName: " + newFileName);
						return;
					}
				*/
				//}

				inFlagFs_dataSkip = 0;
			}
			else
			{
				 newFileName = "/data/data/" + this.getPackageName() + "/" + strFilename;
			}
			
			Log.i("Castles", "copyFile() Path " + newFileName);
			
			Log.i("Castles", "copyFile() Path " + newFileName);

			Log.i(TAG, "inFlagFs_dataSkip="+inFlagFs_dataSkip);
			File dbFile = new File(newFileName);

			// patrick testing code start 20190703
			/*
			if (dbFile.exists())
			{
				Log.i(TAG, "File: " + newFileName + " already exist!!!");
				if (inFlagFs_dataSkip == 0)
					dbFile.delete();
				else
					inFlagFs_dataSkip = 0;
			}
			*/
			// patrick testing code end 20190703

			if (!dbFile.exists()) {
				//Log.i(TAG, "File: " + newFileName + " not exist!!!");
				//Log.i("Castles", "copyFile() !exists");
				in = assetManager.open(filename);
				out = new FileOutputStream(newFileName);

				byte[] buffer = new byte[2048];
				int read;
				while ((read = in.read(buffer)) != -1) {
					out.write(buffer, 0, read);
				}
				in.close();
				in = null;
				out.flush();
				out.close();
				out = null;
				chmod(new File(newFileName), 0777);

				Log.i(TAG, "File: " + newFileName + " copied complete");
			}
		} catch (Exception e) {
			Log.e("Castles", "Exception in copyFile() of " + newFileName);
			Log.e("Castles", "Exception in copyFile() " + e.toString());
		}
	}


    public static class Lock {
        private boolean condition;

        public boolean conditionMet() {
            Log.i("saturn", "conditionMet");
            return condition;
        }

        public void setCondition(boolean condition) {
            Log.i("saturn", "setCondition");
            this.condition = condition;
        }
    }

    public final static Lock LOCK = new Lock();

    final Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg){
            super.handleMessage(msg);
            Log.i("handleMessage", "Process");
            if(msg.what == 1) {
                    GetMenu();
            }
        }
    };

    protected PowerManager.WakeLock mWakeLock;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TemporaryData.init(this);
        overridePendingTransition(0, 0);

        mHandler = new Handler(getMainLooper());
        mContext = this;

/*
        SharedPreferences.Editor editorctls = TemporaryData.getSharedPreferences().edit();
        editorctls.putInt(TemporaryData.CTLSDEMO, 0);
        editorctls.apply();
*/
/*
        SharedPreferences sharedPreferences2 = PreferenceManager.getDefaultSharedPreferences(this);

        SharedPreferences.Editor editor2 = sharedPreferences2.edit();
        editor2.putInt("UIFLAG", 0);
        editor2.apply();

        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
        uiFlag = sharedPreferences.getInt("UIFLAG", 0);

        Log.d("onCreate", uiFlag+"");
*/
        // Show status bar
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.dmenu_activity_main);

	//commented to eliminate hang
        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-MainActivity>>WAKE_LOCK");
        this.mWakeLock.acquire();

        final SharedPreferences wmbPreference = PreferenceManager.getDefaultSharedPreferences(this);
        //final boolean isFirstRun = wmbPreference.getBoolean("FIRSTRUN", true);
        //Log.i("MCCPAY", "-isFirstRun1 =" + isFirstRun);


// Patrick here check is that sub application and return finish start
// Patrick need handle block finish subAP task then call finish.
	if (inCTOSS_MultiAP_CheckMainAPStatus() == 0)
	{
	
	   Log.i("saturn", "saturn test1");
	
		jni_api.REG_CB_CTOS(jni_cb_ctos);	
		
		Log.i("saturn", "saturn after REG_CB_CTOS");
		//test
		//setContentView(R.layout.activity_disp);

		
		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
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
		setContentView(R.layout.lcddisplay_message);
		
		TextView textViewmsg;

		textViewmsg = (TextView) findViewById(R.id.textView_msg);
		//textViewmsg.setText("PROCESSING...");

        try {
            DisplayStatusBox("Processing|Please wait...|process");
			inTermType=inCTOSS_GetEnvInt("TERM_TYPE");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        //test
/*
		lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
		edtLog = (EditText) findViewById(R.id.edtLog);
		adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {
		
			@NonNull
			@Override
			public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
				View view = super.getView(position, convertView, parent);
		
				TextView textView = (TextView) view;
		
				if (addPadding[position] != 0)
					textView.setPadding(36 * addPadding[position], 0, 0, 0);
		
		
				if (displayMode[position].equals("0")) {
					textView.setGravity(Gravity.LEFT);
				} else if (displayMode[position].equals("1")) {
					textView.setGravity(Gravity.CENTER);
				} else if (displayMode[position].equals("2")) {
					textView.setGravity(Gravity.RIGHT);
				}
		
		
				return view;
		
			}
		};
*/
        //ctms prm file exist

        if(isCTMSParameterFileExist()) {
            Log.i(TAG, "reset FIRSTRUN: ");
            wmbPreference.edit().remove("FIRSTRUN").commit();
        }

        int firstInit = inCTOSS_Get1stInitFlag();
        int DLNotFinished = inCTOSS_GetDLNotFinishedFlag();
        Log.i(TAG, "-inCTOSS_Get1stInitFlag: " + firstInit + " DLNotFinished:" + DLNotFinished);
        isFirstRun = wmbPreference.getBoolean("FIRSTRUN", true);
        Log.i(TAG, "-isFirstRun2 =" + isFirstRun);

        //DLNotFinished = 0;
        if(DLNotFinished == 1)
        {
            //terminal need to restart after call ctms in this case
            ScheduledExecutorService scheduledExecutorService = new ScheduledThreadPoolExecutor(1);
            scheduledExecutorService.schedule(new Runnable() {
                @Override
                public void run() {
                    Log.i(TAG, "run: ctms call");
                    Intent intent = new Intent();
                    intent.putExtra("REBOOT", true);
                    intent.setClass(MainActivity.this, Activity_ctms_update.class);
                    startActivity(intent);
                    Log.i(TAG, "run: ctms done");
                }
            }, 1000/2, TimeUnit.MILLISECONDS);
            Log.i(TAG, "return oncreate: ");
            return;
        }
	        new Thread() {
	            @Override
	            public void run() {
	                try {

						
						int firstInit = inCTOSS_Get1stInitFlag();
						int inAlreadyInitialized = inCTOSS_GetPHQRInit();
                        DisplayStatusBox("Initializing|Please wait...|wait");
                        if (isFirstRun) {
                        // if(firstInit == 1){
							if (inAlreadyInitialized != 1)
								vdClearFile("/data/data/pub/MCCPAY.S3DB");
							
							vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/QRPAYMENU.S3DB");
                            vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/QRPAYMENU.S3DB");
                            vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/S1_MCCPAY.S3DB");

                            vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/wechat_footer.bmp");
                            vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/alipay_footer.bmp");
                            vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/gcash_footer.bmp");
                            vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/grabpay_footer.bmp");
                            vdClearFile("/data/data/com.Source.S1_MCCPAY.MCCPAY/upipay_footer.bmp");



                            Log.i("saturn", "saturn run: Initializing...");
                            //copy file or Dir
                            Log.d("inCTOSS_SubAPMain", "MCCPAY START 2");
                            copyFileOrDir("");
                            inCTOSS_ProcessCfgExpress();
                            Log.d("inCTOSS_SubAPMain", "MCCPAY END 2");

                            SharedPreferences.Editor editor = wmbPreference.edit();
                            editor.putBoolean("FIRSTRUN", false);
                            editor.commit();


                        }
                        Log.i("saturn", "saturn inCTOSS_SubAPMain 1");
	                    inCTOSS_SubAPMain();
	                    finish();

	        	        Log.i("Castles", "Amex finish()");

	                    // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity

	                } catch (Exception e) {
                        Log.d("inCTOSS_SubAPMain", "Encountered ERROR: MCCPAY 2 ");
	                }
	            }
	        }.start();

	}
// Patrick here check is that sub application and return finish end	
	else
	{
				mPager = (ViewPager) findViewById(R.id.viewpager);
				mLlDot = (LinearLayout) findViewById(R.id.ll_dot);
				tvMsgLine01 = (TextView) findViewById(R.id.msg_text_01);
		//		  btnauto = (Button) findViewById(R.id.btn_auto);
				/*make it no sleep*/
				getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
				// SharedPreferences wmbPreference = PreferenceManager.getDefaultSharedPreferences(this);
		
				// boolean isFirstRun = wmbPreference.getBoolean("FIRSTRUN", true);

				if (isFirstRun)
				{
					jni_api.REG_CB_CTOS(jni_cb_ctos);
					setContentView(R.layout.activity_disp);
					lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
					edtLog = (EditText) findViewById(R.id.edtLog);
					adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {
		
						@NonNull
						@Override
						public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
							View view = super.getView(position, convertView, parent);
		
							TextView textView = (TextView) view;
		
							if (addPadding[position] != 0)
								textView.setPadding(36 * addPadding[position], 0, 0, 0);
		
		
							if (displayMode[position].equals("0")) {
								textView.setGravity(Gravity.LEFT);
							} else if (displayMode[position].equals("1")) {
								textView.setGravity(Gravity.CENTER);
							} else if (displayMode[position].equals("2")) {
								textView.setGravity(Gravity.RIGHT);
							}
		
		
							return view;
		
						}
					};
		
		
		
					{
						//setContentView(R.layout.activity_keypad);
						Log.i("Castles", "INIT");
						new Thread() {
							@Override
							public void run() {
								try {
		
									//copy file or Dir
                                    Log.d("inCTOSS_SubAPMain", "MCCPAY START4");
									copyFileOrDir("");
									// inCTOSS_InitWaveData();
                                    Log.d("inCTOSS_SubAPMain", "MCCPAY END4");
									final Thread thread = new Thread(new Runnable(){
		
										@Override
										public void run() {
											Message message = new Message();
											message.what = 1;
											Log.d("handler.sendMessage",  "test");
											handler.sendMessageDelayed(message, 100);
										}
		
									});
		
									thread.start();
								} catch (Exception e) {
		
								}
							}
						}.start();
					}
		
		
					SharedPreferences.Editor editor = wmbPreference.edit();
					editor.putBoolean("FIRSTRUN", false);
					editor.commit();
				}

	
				//初始化数据源
				initDatas();
				inflater = LayoutInflater.from(this);
				//总的页数=总数/每页数量，并取整
				pageCount = (int) Math.ceil(mDatas.size() * 1.0 / pageSize);
				mPagerList = new ArrayList<View>();
				for (int i = 0; i < pageCount; i++) {
					//每个页面都是inflate出一个新实例
					GridView gridView = (GridView) inflater.inflate(R.layout.gridview, mPager, false);
					gridView.setAdapter(new GridViewAdapter(this, mDatas, i, pageSize));
					mPagerList.add(gridView);
		
					gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
						@Override
						public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
							int pos = position + curIndex * pageSize;
							Toast.makeText(MainActivity.this, mDatas.get(pos).getName(), Toast.LENGTH_SHORT).show();
							
							jni_api.REG_CB_CTOS(jni_cb_ctos);
							setContentView(R.layout.activity_disp);
							lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
							edtLog = (EditText) findViewById(R.id.edtLog);
							adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list){
		
								@NonNull
								@Override
								public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
									View view =  super.getView(position, convertView, parent);
		
		
									TextView textView = (TextView)view;
		
		//							  Log.d("Position", String.valueOf(position));
		//							  Log.d("Padding", String.valueOf(addPadding[position]));
		
									if(addPadding[position] != 0)
										textView.setPadding(36*addPadding[position],0,0,0);
		
		//							  Log.d("DisplayMode", displayMode[position]);
		
									if (displayMode[position].equals("0")) {
										textView.setGravity(Gravity.LEFT);
									} else if (displayMode[position].equals("1")) {
										textView.setGravity(Gravity.CENTER);
									} else if (displayMode[position].equals("2")) {
										textView.setGravity(Gravity.RIGHT);
									}
		
		
									return view;
		
								}
							};
		
						if (mDatas.get(pos).getName() == "INIT WAVE") {
							Log.i("Castles", "INIT WAVE");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_InitWaveData();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						if (mDatas.get(pos).getName() == "EditDB") {
							Log.i("Castles", "EditDB");
							Intent intent = new Intent();
							intent.setClass(MainActivity.this, EditDatabaseActivity.class);
							startActivity(intent);
						
						}
						
						if (mDatas.get(pos).getName() == "Credit Sale") {
							Log.i("Castles", "SALE");
							new Thread() {
								@Override
								public void run() {
									try {				
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_WAVE_SALE();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Void Sale") {
							Log.i("Castles", "Void Sale");
							new Thread() {
								@Override
								public void run() {
									try {

										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_VOID();

										final Thread thread = new Thread(new Runnable(){

											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Offline") {
							Log.i("Castles", "Offline");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_SALE_OFFLINE();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
												//handler.sendMessage(message);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Refund") {
							Log.i("Castles", "Refund");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_REFUND();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
												//handler.sendMessage(message);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Tip Adjust") {
							Log.i("Castles", "Tip Adjust");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_TIPADJUST();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Authorize") {
							//setContentView(R.layout.activity_keypad);
							Log.i("Castles", "Authorize");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_PREAUTH();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Sale Complete") {
							Log.i("Castles", "Sale Complete");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_SALECOMPLETE();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "EPP Sale") {
							Log.i("Castles", "EPP Sale");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_EPP_SALE();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "EPP Void") {
							Log.i("Castles", "EPP Void");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_EPP_VOID();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "SETTLEMENT") {
							Log.i("Castles", "SETTLEMENT");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_SETTLEMENT();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Clear Reversal") {
							Log.i("Castles", "Clear Reversal");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										vdCTOSS_DeleteReversal();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Last Receipt") {
							Log.i("Castles", "Last Receipt");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_REPRINT_LAST();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
						
						if (mDatas.get(pos).getName() == "Last Settle") {
							Log.i("Castles", "Last Settle");
							new Thread() {
								@Override
								public void run() {
									try {
		
										jni_api.REG_CB_CTOS(jni_cb_ctos);
										inCTOSS_REPRINTF_LAST_SETTLEMENT();
						
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
		
							if (mDatas.get(pos).getName() == "ANY RECEIPT") {
							Log.i("Castles", "ANY RECEIPT");
							new Thread() {
								@Override
								public void run() {
									try {
		
													jni_api.REG_CB_CTOS(jni_cb_ctos);
													inCTOSS_REPRINT_ANY();
		
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
		
							if (mDatas.get(pos).getName() == "DETAIL REPORT") {
							Log.i("Castles", "DETAIL REPORT");
							new Thread() {
								@Override
								public void run() {
									try {
		
													jni_api.REG_CB_CTOS(jni_cb_ctos);
													inCTOSS_PRINTF_DETAIL();
		
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
		
							if (mDatas.get(pos).getName() == "SUMMARY REPORT") {
							Log.i("Castles", "SUMMARY REPORT");
							new Thread() {
								@Override
								public void run() {
									try {
		
													jni_api.REG_CB_CTOS(jni_cb_ctos);
													inCTOSS_PRINTF_SUMMARY();
		
										final Thread thread = new Thread(new Runnable(){
						
											@Override
											public void run() {
												Message message = new Message();
												message.what = 1;
												Log.d("handler.sendMessage",  "test");
												handler.sendMessageDelayed(message, 100);
											}
						
										});
						
										thread.start();
									} catch (Exception e) {
						
									}
								}
							}.start();
						}
		
						if (mDatas.get(pos).getName() == "SETTLE ALL") {
								Log.i("Castles", "SETTLE ALL");
								new Thread() {
									@Override
									public void run() {
										try {
		
											jni_api.REG_CB_CTOS(jni_cb_ctos);
											inCTOSS_SETTLE_ALL();
		
											final Thread thread = new Thread(new Runnable(){
							
												@Override
												public void run() {
													Message message = new Message();
													message.what = 1;
													Log.d("handler.sendMessage",  "test");
													handler.sendMessageDelayed(message, 100);
												}
							
											});
							
											thread.start();
										} catch (Exception e) {
							
										}
									}
								}.start();
							}
		
		
							if (mDatas.get(pos).getName() == "VOID PREAUTH") {
								Log.i("Castles", "VOID PREAUTH");
								new Thread() {
									@Override
									public void run() {
										try {
		
											jni_api.REG_CB_CTOS(jni_cb_ctos);
											inCTOSS_VOIDPREAUTH();
					
												final Thread thread = new Thread(new Runnable(){
												@Override
												public void run() {
													Message message = new Message();
													message.what = 1;
													Log.d("handler.sendMessage",  "test");
													handler.sendMessageDelayed(message, 100);
												}
							
											});
							
											thread.start();
										} catch (Exception e) {
							
										}
									}
								}.start();
							}
		
		
							if (mDatas.get(pos).getName() == "BATCH TOTAL") {
								Log.i("Castles", "BATCH TOTAL");
								new Thread() {
									@Override
									public void run() {
										try {
							
											jni_api.REG_CB_CTOS(jni_cb_ctos);
											inCTOSS_BATCH_TOTAL();
		
												final Thread thread = new Thread(new Runnable(){
												@Override
												public void run() {
													Message message = new Message();
													message.what = 1;
													Log.d("handler.sendMessage",  "test");
													handler.sendMessageDelayed(message, 100);
												}
							
											});
							
											thread.start();
										} catch (Exception e) {
							
										}
									}
								}.start();
							}
		
							if (mDatas.get(pos).getName() == "BATCH REVIEW") {
								Log.i("Castles", "BATCH REVIEW");
								new Thread() {
									@Override
									public void run() {
										try {
							
											jni_api.REG_CB_CTOS(jni_cb_ctos);
											inCTOSS_BATCH_REVIEW();
							
												final Thread thread = new Thread(new Runnable(){
												@Override
												public void run() {
													Message message = new Message();
													message.what = 1;
													Log.d("handler.sendMessage",  "test");
													handler.sendMessageDelayed(message, 100);
												}
							
											});
							
											thread.start();
										} catch (Exception e) {
							
										}
									}
								}.start();
							}
		
								if (mDatas.get(pos).getName() == "OFFICIAL RECEIPT") {
								Log.i("Castles", "OFFICIAL RECEIPT");
								new Thread() {
									@Override
									public void run() {
										try {
							
											jni_api.REG_CB_CTOS(jni_cb_ctos);
											inCTOSS_PAYCASH();
							
												final Thread thread = new Thread(new Runnable(){
												@Override
												public void run() {
													Message message = new Message();
													message.what = 1;
													Log.d("handler.sendMessage",  "test");
													handler.sendMessageDelayed(message, 100);
												}
							
											});
							
											thread.start();
										} catch (Exception e) {
							
										}
									}
								}.start();
							}
		
		
		
							
						}
					});
		
				}
		
		
		
				//设置适配器
				mPager.setAdapter(new ViewPagerAdapter(mPagerList));
		/*
			   ctlsdemo = TemporaryData.getSharedPreferences().getInt(TemporaryData.CTLSDEMO, 0);
			   if(ctlsdemo == 1) {
				   mPager.setOnClickListener(this);
		
				   mPager.performClick();
			   }
		*/		 
				//设置圆点
				setOvalLayout();
		//		 btnauto.performClick();

	}

    }

    /**
     * 初始化数据源
     */
    private void initDatas() {
        mDatas = new ArrayList<Model>();
        for (int i = 0; i < titles.length; i++) {
            //动态获取资源ID，第一个参数是资源名，第二个参数是资源类型例如drawable，string等，第三个参数包名
            int imageId = getResources().getIdentifier("ic_category_" + i, "mipmap", getPackageName());
            mDatas.add(new Model(titles[i], imageId));
        }
    }

    // dmenu start code
    /**
     * 设置圆点
     */
    public void setOvalLayout() {
        for (int i = 0; i < pageCount; i++) {
            mLlDot.addView(inflater.inflate(R.layout.dot, null));
        }
        // 默认显示第一页
        mLlDot.getChildAt(0).findViewById(R.id.v_dot)
                .setBackgroundResource(R.drawable.dot_selected);
        mPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            public void onPageSelected(int position) {
                // 取消圆点选中
                mLlDot.getChildAt(curIndex)
                        .findViewById(R.id.v_dot)
                        .setBackgroundResource(R.drawable.dot_normal);
                // 圆点选中
                mLlDot.getChildAt(position)
                        .findViewById(R.id.v_dot)
                        .setBackgroundResource(R.drawable.dot_selected);
                curIndex = position;
            }

            public void onPageScrolled(int arg0, float arg1, int arg2) {
            }

            public void onPageScrollStateChanged(int arg0) {
            }
        });
    }
// dmenu end code

    private void bindViews() {
        editdetail = (EditText) findViewById(R.id.editdetail);
        editname = (EditText) findViewById(R.id.editname);
        btnclean = (Button) findViewById(R.id.btnclean);
        btnsave = (Button) findViewById(R.id.btnsave);
        btnread = (Button) findViewById(R.id.btnread);

        btnclean.setOnClickListener(this);
        btnsave.setOnClickListener(this);
        btnread.setOnClickListener(this);
    }

    @Override
    public void onClick(View v){
        switch (v.getId()) {
            case R.id.btnclean:
                editdetail.setText("");
                editname.setText("");
                break;
            case R.id.btnsave:
                break;
            case R.id.btnread:
                break;
            case R.id.viewpager:
                if(ctlsdemo == 1)
                {
                    intransCount++;
                Toast.makeText(mContext, "Transaction Count "+intransCount, Toast.LENGTH_SHORT).show();
                jni_api.REG_CB_CTOS(jni_cb_ctos);
                setContentView(R.layout.activity_disp);
                lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
                edtLog = (EditText) findViewById(R.id.edtLog);
                adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {

                    @NonNull
                    @Override
                    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
                        View view = super.getView(position, convertView, parent);


                        TextView textView = (TextView) view;

//                            Log.d("Position", String.valueOf(position));
//                            Log.d("Padding", String.valueOf(addPadding[position]));

                        if (addPadding[position] != 0)
                            textView.setPadding(36 * addPadding[position], 0, 0, 0);

//                            Log.d("DisplayMode", displayMode[position]);

                        if (displayMode[position].equals("0")) {
                            textView.setGravity(Gravity.LEFT);
                        } else if (displayMode[position].equals("1")) {
                            textView.setGravity(Gravity.CENTER);
                        } else if (displayMode[position].equals("2")) {
                            textView.setGravity(Gravity.RIGHT);
                        }


                        return view;

                    }
                };
            {
                Log.i("Castles", "SALE AUTO");
                new Thread() {
                    @Override
                    public void run() {
                        try {
                            inCTOSS_Sale();

                            final Thread thread = new Thread(new Runnable() {

                                @Override
                                public void run() {
                                    Message message = new Message();
                                    message.what = 1;
                                    Log.d("handler.sendMessage", "test");
                                    handler.sendMessageDelayed(message, 100);
                                    //handler.sendMessage(message);
                                }

                            });

                            thread.start();
                        } catch (Exception e) {

                        }
                    }
                }.start();
            }

        }
        }
    }

    public void showMsg( final String msg)
    {
        runOnUiThread(new Runnable()
                      {
                          @Override
                          public void run()
                          {
                              edtLog.append(msg + '\n');
                          }
                      }
        );
    }

    public void vdMCCPaysendBackEcrResponse(String str) {
        final String ACTION = "com.persistent.app.RECEIVER";

        Log.i(TAG, "vdMCCPaysendBackEcrResponse: " + str);
//        Intent intent = new Intent(Intent.ACTION_MAIN);
//        ComponentName componentName = new ComponentName(ECR_APPLICATION_ID, ECR_APPLICATION_ID+".RECEIVER");
//        intent.setComponent(componentName);
//        Intent intent = new Intent(ECR_APPLICATION_ID + ".RECEIVER");
        Intent intent = new Intent(ACTION);
        Log.i(TAG, "sendBackEcrResponse: " + Build.VERSION.SDK_INT);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N_MR1)
        {
            Log.i(TAG, "V8 and above: ");
            intent.addFlags(0x01000000);
//          intent.setComponent(new ComponentName(ECR_APPLICATION_ID, ECR_APPLICATION_ID+".RECEIVER"));
        }
        intent.putExtra("ECR_RESP", "SUCC");
        mContext.sendBroadcast(intent);

        Log.i(TAG, "vdMCCPaysendBackEcrResponse: done");
    }
	
    public String ForkSubAP(String msg)
    {
        int inMsgCnt = 0;
        int inIdx = 0;
//	 String szSubAP = ".Main.MultiAP_SubAP";
	 String szSubAP = ".Main.MainActivity";

        Log.i("Castles", msg);

	 ComponentName apk2Component = new ComponentName(msg, msg+szSubAP);
	 Log.i("Castles", msg+szSubAP);
     

	 Intent intent = new Intent();
        Bundle bundle = new Bundle();

        bundle.putString("subAP", "1");
        bundle.putString("subAP1", "0");

        intent.putExtras(bundle);
        intent.setComponent(apk2Component); 

        startActivity(intent);
        return "OK";
    }

    public String displayMultipleMsg(final String disp_msg)
    {
        mHandler.post(new Runnable() {
            @Override
            public void run() {

                setContentView(R.layout.displaytextmessage);

                tvMsgLine01 = (TextView) findViewById(R.id.msg_text_01);
                tvMsgLine02 = (TextView) findViewById(R.id.msg_text_02);
                tvMsgLine03 = (TextView) findViewById(R.id.msg_text_03);
                tvMsgLine04 = (TextView) findViewById(R.id.msg_text_04);
                tvMsgLine05 = (TextView) findViewById(R.id.msg_text_05);
                tvMsgLine06 = (TextView) findViewById(R.id.msg_text_06);
                tvMsgLine07 = (TextView) findViewById(R.id.msg_text_07);
                tvMsgLine08 = (TextView) findViewById(R.id.msg_text_08);
                tvMsgLine09 = (TextView) findViewById(R.id.msg_text_09);
                tvMsgLine10 = (TextView) findViewById(R.id.msg_text_10);
                tvMsgLine11 = (TextView) findViewById(R.id.msg_text_11);
                tvMsgLine12 = (TextView) findViewById(R.id.msg_text_12);
                tvMsgLine13 = (TextView) findViewById(R.id.msg_text_13);
                tvMsgLine14 = (TextView) findViewById(R.id.msg_text_14);
                tvMsgLine15 = (TextView) findViewById(R.id.msg_text_15);
                tvMsgLine16 = (TextView) findViewById(R.id.msg_text_16);

                tvMsgLine01.setTag(new Date().getTime());

                System.out.println("start displayMultipleMsg");
                System.out.println(disp_msg);

                {
                    getMsg();

                    String[] parsemsg = disp_msg.split("\\|");

                    System.out.println("line:"+parsemsg[1]+"msg:"+parsemsg[2]+"mode:"+parsemsg[3]);

                    int inmsgcnt = parsemsg.length;
                    int line = Integer.parseInt(parsemsg[1]);
                    String msg = parsemsg[2];
                    int dispmode = Integer.parseInt(parsemsg[3]);

                    System.out.println("line:"+line+msg+dispmode);

                    if(dispmode == 0)
                        inGravity = Gravity.LEFT;
                    else if(dispmode == 1)
                        inGravity = Gravity.CENTER;
                    else if(dispmode == 2)
                        inGravity = Gravity.RIGHT;

                    System.out.printf("inGravity[%d]\n", inGravity);

                    switch (line) {
                        case 1:
                            tvMsgLine01.setGravity(inGravity);
                            tvMsgLine01.setText(parsemsg[2]);

                            break;
                        case 2:
                            tvMsgLine02.setGravity(inGravity);
                            tvMsgLine02.setText(parsemsg[2]);

                            break;
                        case 3:
                            tvMsgLine03.setGravity(inGravity);
                            tvMsgLine03.setText(parsemsg[2]);
                            break;
                        case 4:
                            tvMsgLine04.setGravity(inGravity);
                            tvMsgLine04.setText(parsemsg[2]);
                            break;
                        case 5:
                            tvMsgLine05.setGravity(inGravity);
                            tvMsgLine05.setText(parsemsg[2]);
                            break;
                        case 6:
                            tvMsgLine06.setGravity(inGravity);
                            tvMsgLine06.setText(parsemsg[2]);
                            break;
                        case 7:
                            tvMsgLine07.setGravity(inGravity);
                            tvMsgLine07.setText(parsemsg[2]);
                            break;
                        case 8:
                            tvMsgLine08.setGravity(inGravity);
                            tvMsgLine08.setText(parsemsg[2]);
                            break;
                        case 9:
                            tvMsgLine09.setGravity(inGravity);
                            tvMsgLine09.setText(parsemsg[2]);
                            break;
                        case 10:
                            tvMsgLine10.setGravity(inGravity);
                            tvMsgLine10.setText(parsemsg[2]);
                            break;
                        case 11:
                            tvMsgLine11.setGravity(inGravity);
                            tvMsgLine11.setText(parsemsg[2]);
                            break;
                        case 12:
                            tvMsgLine12.setGravity(inGravity);
                            tvMsgLine12.setText(parsemsg[2]);
                            break;
                        case 13:
                            tvMsgLine13.setGravity(inGravity);
                            tvMsgLine13.setText(parsemsg[2]);
                            break;
                        case 14:
                            tvMsgLine14.setGravity(inGravity);
                            tvMsgLine14.setText(parsemsg[2]);
                            break;
                        case 15:
                            tvMsgLine15.setGravity(inGravity);
                            tvMsgLine15.setText(parsemsg[2]);
                            break;
                        case 16:
                            tvMsgLine16.setGravity(inGravity);
                            tvMsgLine16.setText(parsemsg[2]);
                            break;
                    }
                }

                setMsg();

                System.out.println("end displayMultipleMsg");
            }
        });


        return "OK";
    }


    public String printbyXY(int x, int y, String disp_msg)
    {
        setContentView(R.layout.displaytextmessage);

        tvMsgLine01 = (TextView) findViewById(R.id.msg_text_01);
        tvMsgLine02 = (TextView) findViewById(R.id.msg_text_02);
        tvMsgLine03 = (TextView) findViewById(R.id.msg_text_03);
        tvMsgLine04 = (TextView) findViewById(R.id.msg_text_04);
        tvMsgLine05 = (TextView) findViewById(R.id.msg_text_05);
        tvMsgLine06 = (TextView) findViewById(R.id.msg_text_06);
        tvMsgLine07 = (TextView) findViewById(R.id.msg_text_07);
        tvMsgLine08 = (TextView) findViewById(R.id.msg_text_08);
        tvMsgLine09 = (TextView) findViewById(R.id.msg_text_09);
        tvMsgLine10 = (TextView) findViewById(R.id.msg_text_10);
        tvMsgLine11 = (TextView) findViewById(R.id.msg_text_11);
        tvMsgLine12 = (TextView) findViewById(R.id.msg_text_12);
        tvMsgLine13 = (TextView) findViewById(R.id.msg_text_13);
        tvMsgLine14 = (TextView) findViewById(R.id.msg_text_14);
        tvMsgLine15 = (TextView) findViewById(R.id.msg_text_15);
        tvMsgLine16 = (TextView) findViewById(R.id.msg_text_16);

        System.out.println("start printbyXY");
        System.out.println(disp_msg);

        inGravity = Gravity.NO_GRAVITY;
        x=x-1;


        getMsg();
        System.out.printf("inGravity[%d]\n", inGravity);

        switch (y) {
            case 1:
                tvMsgLine01.setGravity(inGravity);
                tvMsgLine01.setPadding(36*x,0,0,0);
                tvMsgLine01.setText(disp_msg);
                inPaddingLeft[1]=36*x;

                break;
            case 2:
                tvMsgLine02.setGravity(inGravity);
                tvMsgLine02.setPadding(36*x,0,0,0);
                tvMsgLine02.setText(disp_msg);
                inPaddingLeft[2]=36*x;
                break;
            case 3:
                tvMsgLine03.setGravity(inGravity);
                tvMsgLine03.setPadding(36*x,0,0,0);
                tvMsgLine03.setText(disp_msg);
                inPaddingLeft[3]=36*x;
                break;
            case 4:
                tvMsgLine04.setGravity(inGravity);
                tvMsgLine04.setPadding(36*x,0,0,0);
                tvMsgLine04.setText(disp_msg);
                inPaddingLeft[4]=36*x;
                break;
            case 5:
                tvMsgLine05.setGravity(inGravity);
                tvMsgLine05.setPadding(36*x,0,0,0);
                tvMsgLine05.setText(disp_msg);
                inPaddingLeft[5]=36*x;
                break;
            case 6:
                tvMsgLine06.setGravity(inGravity);
                tvMsgLine06.setPadding(36*x,0,0,0);
                tvMsgLine06.setText(disp_msg);
                inPaddingLeft[6]=36*x;
                break;
            case 7:
                tvMsgLine07.setGravity(inGravity);
                tvMsgLine07.setPadding(36*x,0,0,0);
                tvMsgLine07.setText(disp_msg);
                inPaddingLeft[7]=36*x;
                break;
            case 8:
                tvMsgLine08.setGravity(inGravity);
                tvMsgLine08.setPadding(36*x,0,0,0);
                tvMsgLine08.setText(disp_msg);
                inPaddingLeft[8]=36*x;
                break;
            case 9:
                tvMsgLine09.setGravity(inGravity);
                tvMsgLine09.setPadding(36*x,0,0,0);
                tvMsgLine09.setText(disp_msg);
                inPaddingLeft[9]=36*x;
                break;
            case 10:
                tvMsgLine10.setGravity(inGravity);
                tvMsgLine10.setPadding(36*x,0,0,0);
                tvMsgLine10.setText(disp_msg);
                inPaddingLeft[10]=36*x;
                break;
            case 11:
                tvMsgLine11.setGravity(inGravity);
                tvMsgLine11.setPadding(36*x,0,0,0);
                tvMsgLine11.setText(disp_msg);
                inPaddingLeft[11]=36*x;
                break;
            case 12:
                tvMsgLine12.setGravity(inGravity);
                tvMsgLine12.setPadding(36*x,0,0,0);
                tvMsgLine12.setText(disp_msg);
                inPaddingLeft[12]=36*x;
                break;
            case 13:
                tvMsgLine13.setGravity(inGravity);
                tvMsgLine13.setPadding(36*x,0,0,0);
                tvMsgLine13.setText(disp_msg);
                inPaddingLeft[13]=36*x;
                break;
            case 14:
                tvMsgLine14.setGravity(inGravity);
                tvMsgLine14.setPadding(36*x,0,0,0);
                tvMsgLine14.setText(disp_msg);
                inPaddingLeft[14]=36*x;
                break;
            case 15:
                tvMsgLine15.setGravity(inGravity);
                tvMsgLine15.setPadding(36*x,0,0,0);
                tvMsgLine15.setText(disp_msg);
                inPaddingLeft[15]=36*x;
                break;
            case 16:
                tvMsgLine16.setGravity(inGravity);
                tvMsgLine16.setPadding(36*x,0,0,0);
                tvMsgLine16.setText(disp_msg);
                inPaddingLeft[16]=36*x;
                break;
        }

        setMsg();

        System.out.println("end displayMultipleMsg");
        return "DISP_MSG_OK";
    }


    public void GetMenu(){
        final String MenuBuff;//, AppHeaderBuff;
        String TrxMenuItems, AppDetail1="", AppDetail2="", TrxImageId, AppDetail, Networknm;
        String MainHeader;
        String[] TrxTitle = new String[100];
        String[] AppHeader = new String[100];
        int inLoop=0;
        String rstatus="0";
        String idleMode = "0";

        fSTART = false;
        

        Log.i("saturn ", "GetMenu - START");
        
        GetAppInfor getAppInfor = new GetAppInfor(mContext);
        String currentSignature = getAppInfor.getInstalledAppHashCode();
        Log.i("currentSignature", "" + currentSignature);
        //Toast.makeText(getApplicationContext(), "currentSignature " + currentSignature, Toast.LENGTH_SHORT).show();


        AppHeaderBuff = GetAppHeaderDetails();

        MainHeader = AppHeaderBuff;
        Log.i("GetMenu::", AppHeaderBuff);
        AppHeader = MainHeader.split(" \n");
        int msgcnt = AppHeader.length;

        OperatorHolder operatorHolder = new OperatorHolder(this);
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo info = wifiManager.getConnectionInfo ();
        String ssid  = info.getSSID();

        View decorView = getWindow().getDecorView();
        //decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);     
		decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        setContentView(R.layout.dmenu_activity_main);

        app_version = (TextView) findViewById(R.id.main_app_version);
        merchant = (TextView) findViewById(R.id.main_merchant);
        tprofile = (TextView) findViewById(R.id.main_tprofile);
        //btn_start = (Button) findViewById(R.id.btn_start);
        carrier_name = (TextView) findViewById(R.id.main_NetworkCarrier);


        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            //System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    app_version.setText(AppHeader[inIdx]);
                    break;
                case 1:
                    merchant.setText(AppHeader[inIdx]);
                    break;
                case 2:
                    AppDetail1 = "MID: "+AppHeader[inIdx];
                    Log.i("TINE", AppDetail1);
                    break;
                case 3:
                    AppDetail2 = "TID: "+AppHeader[inIdx];
                    Log.i("TINE", AppDetail2);
                    break;
                case 4:
                    rstatus = AppHeader[inIdx];
                    break;
                case 5:
                    idleMode = AppHeader[inIdx];
                    break;
            }
        }

        AppDetail = AppDetail1 + "  |  " + AppDetail2;
        tprofile.setText(AppDetail);

        if (ssid.equals("<unknown ssid>") || (ssid.length() <=0))
            Networknm = "Network: " + operatorHolder.getOperatorName();
        else
            Networknm = "Network: " + ssid;

        if (Networknm.equals("Network: "))
            Networknm = "Network: NOT CONNECTED";

        Log.i("AAA Networkname", "Networknm: " + Networknm);
        carrier_name.setText(Networknm);

        mPager = (ViewPager) findViewById(R.id.viewpager);

        //初始化数据源
        initDatas();
        inflater = LayoutInflater.from(MainActivity.this);
        //总的页数=总数/每页数量，并取整
        pageCount = (int) Math.ceil(mDatas.size() * 1.0 / pageSize);
        mPagerList = new ArrayList<View>();
        for (int i = 0; i < pageCount; i++) {
            //每个页面都是inflate出一个新实例
            GridView gridView = (GridView) inflater.inflate(R.layout.gridview, mPager, false);
            gridView.setAdapter(new GridViewAdapter(this, mDatas, i, pageSize));
            mPagerList.add(gridView);


            gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    int pos = position + curIndex * pageSize;
                    Toast.makeText(MainActivity.this, mDatas.get(pos).getName(), Toast.LENGTH_SHORT).show();

                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                    setContentView(R.layout.activity_disp);
                    lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
                    edtLog = (EditText) findViewById(R.id.edtLog);
                    adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list){

                        @NonNull
                        @Override
                        public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
                            View view =  super.getView(position, convertView, parent);


                            TextView textView = (TextView)view;

//                            Log.d("Position", String.valueOf(position));
//                            Log.d("Padding", String.valueOf(addPadding[position]));

                            if(addPadding[position] != 0)
                                textView.setPadding(36*addPadding[position],0,0,0);

//                            Log.d("DisplayMode", displayMode[position]);

                            if (displayMode[position].equals("0")) {
                                textView.setGravity(Gravity.LEFT);
                            } else if (displayMode[position].equals("1")) {
                                textView.setGravity(Gravity.CENTER);
                            } else if (displayMode[position].equals("2")) {
                                textView.setGravity(Gravity.RIGHT);
                            }


                            return view;

                        }
                    };
					
                    if (mDatas.get(pos).getName() == "INIT WAVE") {
                        Log.i("Castles", "INIT WAVE");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_InitWaveData();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }
					
                    if (mDatas.get(pos).getName() == "EditDB") {
                        Log.i("Castles", "EditDB");
                        Intent intent = new Intent();
                        intent.setClass(MainActivity.this, EditDatabaseActivity.class);
                        startActivity(intent);

                    }


                    if (mDatas.get(pos).getName() == "Credit Sale") {
                        Log.i("Castles", "SALE");
                        new Thread() {
                            @Override
                            public void run() {
                                try {				

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_WAVE_SALE();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Void Sale") {
                        Log.i("Castles", "Void Sale");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_VOID();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Offline") {
                        Log.i("Castles", "Offline");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_SALE_OFFLINE();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                            //handler.sendMessage(message);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Refund") {
                        Log.i("Castles", "Refund");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_REFUND();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                            //handler.sendMessage(message);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Tip Adjust") {
                        Log.i("Castles", "Tip Adjust");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_TIPADJUST();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Authorize") {
                        //setContentView(R.layout.activity_keypad);
                        Log.i("Castles", "Authorize");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_PREAUTH();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Sale Complete") {
                        Log.i("Castles", "Sale Complete");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_SALECOMPLETE();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "EPP Sale") {
                        Log.i("Castles", "EPP Sale");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_EPP_SALE();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "EPP Void") {
                        Log.i("Castles", "EPP Void");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_EPP_VOID();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "SETTLEMENT") {
                        Log.i("Castles", "SETTLEMENT");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_SETTLEMENT();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Clear Reversal") {
                        Log.i("Castles", "Clear Reversal");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    vdCTOSS_DeleteReversal();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Last Receipt") {
                        Log.i("Castles", "Last Receipt");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_REPRINT_LAST();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

                    if (mDatas.get(pos).getName() == "Last Settle") {
                        Log.i("Castles", "Last Settle");
                        new Thread() {
                            @Override
                            public void run() {
                                try {

                                    jni_api.REG_CB_CTOS(jni_cb_ctos);
                                    inCTOSS_REPRINTF_LAST_SETTLEMENT();

                                    final Thread thread = new Thread(new Runnable(){

                                        @Override
                                        public void run() {
                                            Message message = new Message();
                                            message.what = 1;
                                            Log.d("handler.sendMessage",  "test");
                                            handler.sendMessageDelayed(message, 100);
                                        }

                                    });

                                    thread.start();
                                } catch (Exception e) {

                                }
                            }
                        }.start();
                    }

					if (mDatas.get(pos).getName() == "ANY RECEIPT") {
					Log.i("Castles", "ANY RECEIPT");
					new Thread() {
						@Override
						public void run() {
							try {

			                                jni_api.REG_CB_CTOS(jni_cb_ctos);
			                                inCTOSS_REPRINT_ANY();

								final Thread thread = new Thread(new Runnable(){
				
									@Override
									public void run() {
										Message message = new Message();
										message.what = 1;
										Log.d("handler.sendMessage",  "test");
										handler.sendMessageDelayed(message, 100);
									}
				
								});
				
								thread.start();
							} catch (Exception e) {
				
							}
						}
					}.start();
				}

					if (mDatas.get(pos).getName() == "DETAIL REPORT") {
					Log.i("Castles", "DETAIL REPORT");
					new Thread() {
						@Override
						public void run() {
							try {

			                                jni_api.REG_CB_CTOS(jni_cb_ctos);
			                                inCTOSS_PRINTF_DETAIL();

								final Thread thread = new Thread(new Runnable(){
				
									@Override
									public void run() {
										Message message = new Message();
										message.what = 1;
										Log.d("handler.sendMessage",  "test");
										handler.sendMessageDelayed(message, 100);
									}
				
								});
				
								thread.start();
							} catch (Exception e) {
				
							}
						}
					}.start();
				}

					if (mDatas.get(pos).getName() == "SUMMARY REPORT") {
					Log.i("Castles", "SUMMARY REPORT");
					new Thread() {
						@Override
						public void run() {
							try {

			                                jni_api.REG_CB_CTOS(jni_cb_ctos);
			                                inCTOSS_PRINTF_SUMMARY();

								final Thread thread = new Thread(new Runnable(){
				
									@Override
									public void run() {
										Message message = new Message();
										message.what = 1;
										Log.d("handler.sendMessage",  "test");
										handler.sendMessageDelayed(message, 100);
									}
				
								});
				
								thread.start();
							} catch (Exception e) {
				
							}
						}
					}.start();
				}

				if (mDatas.get(pos).getName() == "SETTLE ALL") {
						Log.i("Castles", "SETTLE ALL");
						new Thread() {
							@Override
							public void run() {
								try {

									jni_api.REG_CB_CTOS(jni_cb_ctos);
									inCTOSS_SETTLE_ALL();

									final Thread thread = new Thread(new Runnable(){
					
										@Override
										public void run() {
											Message message = new Message();
											message.what = 1;
											Log.d("handler.sendMessage",  "test");
											handler.sendMessageDelayed(message, 100);
										}
					
									});
					
									thread.start();
								} catch (Exception e) {
					
								}
							}
						}.start();
					}


					if (mDatas.get(pos).getName() == "VOID PREAUTH") {
						Log.i("Castles", "VOID PREAUTH");
						new Thread() {
							@Override
							public void run() {
								try {

									jni_api.REG_CB_CTOS(jni_cb_ctos);
									inCTOSS_VOIDPREAUTH();
			
										final Thread thread = new Thread(new Runnable(){
										@Override
										public void run() {
											Message message = new Message();
											message.what = 1;
											Log.d("handler.sendMessage",  "test");
											handler.sendMessageDelayed(message, 100);
										}
					
									});
					
									thread.start();
								} catch (Exception e) {
					
								}
							}
						}.start();
					}


					if (mDatas.get(pos).getName() == "BATCH TOTAL") {
						Log.i("Castles", "BATCH TOTAL");
						new Thread() {
							@Override
							public void run() {
								try {
					
									jni_api.REG_CB_CTOS(jni_cb_ctos);
									inCTOSS_BATCH_TOTAL();

										final Thread thread = new Thread(new Runnable(){
										@Override
										public void run() {
											Message message = new Message();
											message.what = 1;
											Log.d("handler.sendMessage",  "test");
											handler.sendMessageDelayed(message, 100);
										}
					
									});
					
									thread.start();
								} catch (Exception e) {
					
								}
							}
						}.start();
					}

					if (mDatas.get(pos).getName() == "BATCH REVIEW") {
						Log.i("Castles", "BATCH REVIEW");
						new Thread() {
							@Override
							public void run() {
								try {
					
									jni_api.REG_CB_CTOS(jni_cb_ctos);
									inCTOSS_BATCH_REVIEW();
					
										final Thread thread = new Thread(new Runnable(){
										@Override
										public void run() {
											Message message = new Message();
											message.what = 1;
											Log.d("handler.sendMessage",  "test");
											handler.sendMessageDelayed(message, 100);
										}
					
									});
					
									thread.start();
								} catch (Exception e) {
					
								}
							}
						}.start();
					}

						if (mDatas.get(pos).getName() == "OFFICIAL RECEIPT") {
						Log.i("Castles", "OFFICIAL RECEIPT");
						new Thread() {
							@Override
							public void run() {
								try {
					
									jni_api.REG_CB_CTOS(jni_cb_ctos);
									inCTOSS_PAYCASH();
					
										final Thread thread = new Thread(new Runnable(){
										@Override
										public void run() {
											Message message = new Message();
											message.what = 1;
											Log.d("handler.sendMessage",  "test");
											handler.sendMessageDelayed(message, 100);
										}
					
									});
					
									thread.start();
								} catch (Exception e) {
					
								}
							}
						}.start();
					}






                }
            });


        }

        //设置适配器
        mPager.setAdapter(new ViewPagerAdapter(mPagerList));
        //设置圆点
        setOvalLayout();

    }


    @RequiresApi(api = Build.VERSION_CODES.M)
    public String displayImage(int x, int y, String msg_img)
    {
        setContentView(R.layout.displayimage);
        Log.i("Castles", "Start displayImage");
        Log.i("Castles", "x:"+x+"y:"+y+"name:"+msg_img);
        // System.out.println("Start displayImage");
        // System.out.println("x:"+x+"y:"+y+"name:"+msg_img);


        int resid = getResources().getIdentifier(msg_img,"drawable", getApplicationContext().getPackageName());

        imageView = (ImageView) findViewById(R.id.imageView);
        imageView.setImageResource(resid);

/*        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(400, 400);
        imageView.setLayoutParams(params);
        imageView.setBackgroundColor(getColor(R.color.colorAccent));*/
        ViewGroup.MarginLayoutParams margin = new ViewGroup.MarginLayoutParams(imageView.getLayoutParams());
        margin.leftMargin = x;
        margin.topMargin = y;
        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(margin);
//        layoutParams.height = 400;//set the height
//        layoutParams.width = 400; //set the width
        imageView.setLayoutParams(layoutParams);
        //imageView.setScaleType(ImageView.ScaleType.FIT_XY);

        Log.i("Castles", "End displayImage");

        //System.out.println("End displayImage");
        return "OK";
    }


    public void getMsg() {
        tvMsgLine01.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine01, null));
        tvMsgLine02.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine02, null));
        tvMsgLine03.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine03, null));
        tvMsgLine04.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine04, null));
        tvMsgLine05.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine05, null));
        tvMsgLine06.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine06, null));
        tvMsgLine07.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine07, null));
        tvMsgLine08.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine08, null));
        tvMsgLine09.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine09, null));
        tvMsgLine10.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine10, null));
        tvMsgLine11.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine11, null));
        tvMsgLine12.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine12, null));
        tvMsgLine13.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine13, null));
        tvMsgLine14.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine14, null));
        tvMsgLine15.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine15, null));
        tvMsgLine16.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine16, null));

        tvMsgLine01.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity01, 0));
        tvMsgLine02.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity02, 0));
        tvMsgLine03.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity03, 0));
        tvMsgLine04.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity04, 0));
        tvMsgLine05.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity05, 0));
        tvMsgLine06.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity06, 0));
        tvMsgLine07.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity07, 0));
        tvMsgLine08.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity08, 0));
        tvMsgLine09.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity09, 0));
        tvMsgLine10.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity10, 0));
        tvMsgLine11.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity11, 0));
        tvMsgLine12.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity12, 0));
        tvMsgLine13.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity13, 0));
        tvMsgLine14.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity14, 0));
        tvMsgLine15.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity15, 0));
        tvMsgLine16.setGravity(TemporaryData.getSharedPreferences().getInt(TemporaryData.Gravity16, 0));

        tvMsgLine01.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft01, 0), 0, 0, 0);
        tvMsgLine02.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft02, 0), 0, 0, 0);
        tvMsgLine03.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft03, 0), 0, 0, 0);
        tvMsgLine04.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft04, 0), 0, 0, 0);
        tvMsgLine05.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft05, 0), 0, 0, 0);
        tvMsgLine06.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft06, 0), 0, 0, 0);
        tvMsgLine07.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft07, 0), 0, 0, 0);
        tvMsgLine08.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft08, 0), 0, 0, 0);
        tvMsgLine09.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft09, 0), 0, 0, 0);
        tvMsgLine10.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft10, 0), 0, 0, 0);
        tvMsgLine11.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft11, 0), 0, 0, 0);
        tvMsgLine12.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft12, 0), 0, 0, 0);
        tvMsgLine13.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft13, 0), 0, 0, 0);
        tvMsgLine14.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft14, 0), 0, 0, 0);
        tvMsgLine15.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft15, 0), 0, 0, 0);
        tvMsgLine16.setPadding(TemporaryData.getSharedPreferences().getInt(TemporaryData.PaddingLeft16, 0), 0, 0, 0);

    }

    public void setMsg(){

        SharedPreferences.Editor editor = TemporaryData.getSharedPreferences().edit();
        editor.putString(TemporaryData.DispLine01, tvMsgLine01.getText().toString());
        editor.putString(TemporaryData.DispLine02, tvMsgLine02.getText().toString());
        editor.putString(TemporaryData.DispLine03, tvMsgLine03.getText().toString());
        editor.putString(TemporaryData.DispLine04, tvMsgLine04.getText().toString());
        editor.putString(TemporaryData.DispLine05, tvMsgLine05.getText().toString());
        editor.putString(TemporaryData.DispLine06, tvMsgLine06.getText().toString());
        editor.putString(TemporaryData.DispLine07, tvMsgLine07.getText().toString());
        editor.putString(TemporaryData.DispLine08, tvMsgLine08.getText().toString());
        editor.putString(TemporaryData.DispLine09, tvMsgLine09.getText().toString());
        editor.putString(TemporaryData.DispLine10, tvMsgLine10.getText().toString());
        editor.putString(TemporaryData.DispLine11, tvMsgLine11.getText().toString());
        editor.putString(TemporaryData.DispLine12, tvMsgLine12.getText().toString());
        editor.putString(TemporaryData.DispLine13, tvMsgLine13.getText().toString());
        editor.putString(TemporaryData.DispLine14, tvMsgLine14.getText().toString());
        editor.putString(TemporaryData.DispLine15, tvMsgLine15.getText().toString());
        editor.putString(TemporaryData.DispLine16, tvMsgLine16.getText().toString());

        editor.putInt(TemporaryData.Gravity01, tvMsgLine01.getGravity());
        editor.putInt(TemporaryData.Gravity02, tvMsgLine02.getGravity());
        editor.putInt(TemporaryData.Gravity03, tvMsgLine03.getGravity());
        editor.putInt(TemporaryData.Gravity04, tvMsgLine04.getGravity());
        editor.putInt(TemporaryData.Gravity05, tvMsgLine05.getGravity());
        editor.putInt(TemporaryData.Gravity06, tvMsgLine06.getGravity());
        editor.putInt(TemporaryData.Gravity07, tvMsgLine07.getGravity());
        editor.putInt(TemporaryData.Gravity08, tvMsgLine08.getGravity());
        editor.putInt(TemporaryData.Gravity09, tvMsgLine09.getGravity());
        editor.putInt(TemporaryData.Gravity10, tvMsgLine10.getGravity());
        editor.putInt(TemporaryData.Gravity11, tvMsgLine11.getGravity());
        editor.putInt(TemporaryData.Gravity12, tvMsgLine12.getGravity());
        editor.putInt(TemporaryData.Gravity13, tvMsgLine13.getGravity());
        editor.putInt(TemporaryData.Gravity14, tvMsgLine14.getGravity());
        editor.putInt(TemporaryData.Gravity15, tvMsgLine15.getGravity());
        editor.putInt(TemporaryData.Gravity16, tvMsgLine16.getGravity());

        editor.putInt(TemporaryData.PaddingLeft01, inPaddingLeft[1]);
        editor.putInt(TemporaryData.PaddingLeft02, inPaddingLeft[2]);
        editor.putInt(TemporaryData.PaddingLeft03, inPaddingLeft[3]);
        editor.putInt(TemporaryData.PaddingLeft04, inPaddingLeft[4]);
        editor.putInt(TemporaryData.PaddingLeft05, inPaddingLeft[5]);
        editor.putInt(TemporaryData.PaddingLeft06, inPaddingLeft[6]);
        editor.putInt(TemporaryData.PaddingLeft07, inPaddingLeft[7]);
        editor.putInt(TemporaryData.PaddingLeft08, inPaddingLeft[8]);
        editor.putInt(TemporaryData.PaddingLeft09, inPaddingLeft[9]);
        editor.putInt(TemporaryData.PaddingLeft10, inPaddingLeft[10]);
        editor.putInt(TemporaryData.PaddingLeft11, inPaddingLeft[11]);
        editor.putInt(TemporaryData.PaddingLeft12, inPaddingLeft[12]);
        editor.putInt(TemporaryData.PaddingLeft13, inPaddingLeft[13]);
        editor.putInt(TemporaryData.PaddingLeft14, inPaddingLeft[14]);
        editor.putInt(TemporaryData.PaddingLeft15, inPaddingLeft[15]);
        editor.putInt(TemporaryData.PaddingLeft16, inPaddingLeft[16]);
        editor.apply();

    }


    /**
     * 为了得到传回的数据，必须在前面的Activity中（指MainActivity类）重写onActivityResult方法
     *
     * requestCode 请求码，即调用startActivityForResult()传递过去的值
     * resultCode 结果码，结果码用于标识返回数据来自哪个新Activity
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        String result = data.getExtras().getString("result");//得到新Activity 关闭后返回的数据
        Log.i("onActivityResult", result);
    }

		public String GetAmountString(String text) throws InterruptedException {
	
			String user_str="";
			pass_in_string = text;
	
			new Thread()
			{
				public void run()
				{
					try
					{
						Intent intent = new Intent();
						Bundle bundle = new Bundle();
						//intent.setClass(MainActivity.this, GetAmount.class);
						intent.setClass(MainActivity.this, AmountEntryActivity.class);
	
						//bundle.putString("amt_string", amt_string);
						bundle.putString("pass_in_string", pass_in_string);
						//bundle.putString("display", "AMOUNT  (PHP)");
						bundle.putString("minlength", "1");
						bundle.putString("maxlength", "9");
						bundle.putString("type", "1");
						//bundle.putString("pass_in_string", pass_in_string);
						Log.i("pass_in_string", pass_in_string);
	
						intent.putExtras(bundle);
						startActivity(intent);
						//startActivityForResult(intent, 0);
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			}.start();
	
	
			/*
			Intent intent = new Intent();
			Bundle bundle = new Bundle();
			intent.setClass(MainActivity.this, GetAmount.class);
	
			bundle.putString("amt_string", amt_string);
			bundle.putString("amt_disp_msg", text);
			Log.i("amt_disp_msg", text);
	
			intent.putExtras(bundle);
			//startActivity(intent);
			startActivityForResult(intent, 0);
	
			Log.i("PATRICK", "startActivity");
			//finish();
			*/
	
	
			MainActivity.LOCK.setCondition(false);
	
			synchronized (LOCK) {
				while (!LOCK.conditionMet()) {
					try {
						LOCK.wait();
					} catch (InterruptedException e) {
						Log.e(TAG, "Exception when waiting for condition", e);
						return "Exception";
					}
				}
			}
	
			//user_str = GetAmount.final_amt_string;
			user_str = AmountEntryActivity.final_amt_string;
	
			Log.i("PATRICK", user_str);
			Log.i("PATRICK", "FINISH 456");
			//pin_num = pin_num + "XXX";
	//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
			return user_str;
	
		}

	
	
		/**
	
		 * 给图片添加文字到左上角
	
		 * @param context
	
		 * @param bitmap
	
		 * @param text
	
		 * @return
	
		 */
	
		public static Bitmap drawTextToLeftTop(Context context, Bitmap bitmap, String text,
	
											   int size, int color, int paddingLeft, int paddingTop) {
	
			Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	
			paint.setColor(color);
	
			paint.setTextSize(dp2px(context, size));
	
			Rect bounds = new Rect();
	
			paint.getTextBounds(text, 0, text.length(), bounds);
	
			return drawTextToBitmap(context, bitmap, text, paint, bounds,
	
					dp2px(context, paddingLeft),
	
					dp2px(context, paddingTop) + bounds.height());
	
		}
	
	
	
		/**
	
		 * 绘制文字到右下角
	
		 * @param context
	
		 * @param bitmap
	
		 * @param text
	
		 * @param size
	
		 * @param color
	
		 * @param paddingLeft
	
		 * @param paddingTop
	
		 * @return
	
		 */
	
		public static Bitmap drawTextToRightBottom(Context context, Bitmap bitmap, String text,
	
												   int size, int color, int paddingRight, int paddingBottom) {
	
			Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	
			paint.setColor(color);
	
			paint.setTextSize(dp2px(context, size));
	
			Rect bounds = new Rect();
	
			paint.getTextBounds(text, 0, text.length(), bounds);
	
			return drawTextToBitmap(context, bitmap, text, paint, bounds,
	
					bitmap.getWidth() - bounds.width() - dp2px(context, paddingRight),
	
					bitmap.getHeight() - dp2px(context, paddingBottom));
	
		}
	
	
	
		/**
	
		 * 绘制文字到右上方
	
		 * @param context
	
		 * @param bitmap
	
		 * @param text
	
		 * @param size
	
		 * @param color
	
		 * @param paddingRight
	
		 * @param paddingTop
	
		 * @return
	
		 */
	
		public static Bitmap drawTextToRightTop(Context context, Bitmap bitmap, String text,
	
												int size, int color, int paddingRight, int paddingTop) {
	
			Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	
			paint.setColor(color);
	
			paint.setTextSize(dp2px(context, size));
	
			Rect bounds = new Rect();
	
			paint.getTextBounds(text, 0, text.length(), bounds);
	
			return drawTextToBitmap(context, bitmap, text, paint, bounds,
	
					bitmap.getWidth() - bounds.width() - dp2px(context, paddingRight),
	
					dp2px(context, paddingTop) + bounds.height());
	
		}
	
	
	
		/**
	
		 * 绘制文字到左下方
	
		 * @param context
	
		 * @param bitmap
	
		 * @param text
	
		 * @param size
	
		 * @param color
	
		 * @param paddingLeft
	
		 * @param paddingBottom
	
		 * @return
	
		 */
	
		public static Bitmap drawTextToLeftBottom(Context context, Bitmap bitmap, String text,
	
												  int size, int color, int paddingLeft, int paddingBottom) {
	
			Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	
			paint.setColor(color);
	
			paint.setTextSize(dp2px(context, size));
	
			Rect bounds = new Rect();
	
			paint.getTextBounds(text, 0, text.length(), bounds);
	
			return drawTextToBitmap(context, bitmap, text, paint, bounds,
	
					dp2px(context, paddingLeft),
	
					bitmap.getHeight() - dp2px(context, paddingBottom));
	
		}
	
	
	
		/**
	
		 * 绘制文字到中间
	
		 * @param context
	
		 * @param bitmap
	
		 * @param text
	
		 * @param size
	
		 * @param color
	
		 * @return
	
		 */
	
		public static Bitmap drawTextToCenter(Context context, Bitmap bitmap, String text,
	
											  int size, int color) {
	
			Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	
			paint.setColor(color);
	
			paint.setTextSize(dp2px(context, size));
	
			Rect bounds = new Rect();
	
			paint.getTextBounds(text, 0, text.length(), bounds);
	
			return drawTextToBitmap(context, bitmap, text, paint, bounds,
	
					(bitmap.getWidth() - bounds.width()) / 2,
	
					(bitmap.getHeight() + bounds.height()) / 2);
	
		}
	
	
	
		//图片上绘制文字
	
		private static Bitmap drawTextToBitmap(Context context, Bitmap bitmap, String text,
	
											   Paint paint, Rect bounds, int paddingLeft, int paddingTop) {
	
			android.graphics.Bitmap.Config bitmapConfig = bitmap.getConfig();
	
	
	
			paint.setDither(true); // 获取跟清晰的图像采样
	
			paint.setFilterBitmap(true);// 过滤一些
	
			if (bitmapConfig == null) {
	
				bitmapConfig = android.graphics.Bitmap.Config.ARGB_8888;
	
			}
	
			bitmap = bitmap.copy(bitmapConfig, true);
	
			Canvas canvas = new Canvas(bitmap);
	
	
	
			canvas.drawText(text, paddingLeft, paddingTop, paint);
	
			return bitmap;
	
		}
	
	
	
		/**
	
		 * 缩放图片
	
		 * @param src
	
		 * @param w
	
		 * @param h
	
		 * @return
	
		 */
	
		public static Bitmap scaleWithWH(Bitmap src, double w, double h) {
	
			if (w == 0 || h == 0 || src == null) {
	
				return src;
	
			} else {
	
				// 记录src的宽高
	
				int width = src.getWidth();
	
				int height = src.getHeight();
	
				// 创建一个matrix容器
	
				Matrix matrix = new Matrix();
	
				// 计算缩放比例
	
				float scaleWidth = (float) (w / width);
	
				float scaleHeight = (float) (h / height);
	
				// 开始缩放
	
				matrix.postScale(scaleWidth, scaleHeight);
	
				// 创建缩放后的图片
	
				return Bitmap.createBitmap(src, 0, 0, width, height, matrix, true);
	
			}
	
		}
	
	
	
		/**
	
		 * dip转pix
	
		 * @param context
	
		 * @param dp
	
		 * @return
	
		 */
	
		public static int dp2px(Context context, float dp) {
	
			final float scale = context.getResources().getDisplayMetrics().density;
	
			return (int) (dp * scale + 0.5f);
	
		}
	
	
		/* @param path 图片路径
		 * @param quality 质量 0-100,100表示原图
		 * @return
		 */
		public static Bitmap losslessScale(Bitmap bitmap,int quality){
			//Bitmap bitmap = BitmapFactory.decodeFile(path);
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			bitmap.compress(Bitmap.CompressFormat.JPEG, quality, baos);
			Log.e("哈哈","原始大小：" + baos.toByteArray().length);
			// 因为质量压缩不是可以无限缩小的，所以一张高质量的图片，再怎么压缩，
			// 最终size可能还是大于你指定的size，造成异常
			// 所以不建议循环压缩，而是指定quality，进行一次压缩就可以了
	//		  while (baos.toByteArray().length / 1024 > maxSize) {
	//			  quality -= 10;
	//			  baos.reset();
	//			  bitmap.compress(CompressFormat.JPEG, quality, baos);
	//			  Log.e("哈哈","过程中大小为："
	//					  + baos.toByteArray().length);
	//		  }
			bitmap.compress(Bitmap.CompressFormat.JPEG, quality, baos);
			Log.e("哈哈","最终大小" + baos.toByteArray().length);
			Bitmap compressedBitmap = BitmapFactory.decodeByteArray(
					baos.toByteArray(), 0, baos.toByteArray().length);
			return compressedBitmap;
		}
	
	
		public	ArrayList<Bitmap> getBitmapArrayByGif(int inResId) {
			try {
				ArrayList<Bitmap> BitmapList = new ArrayList<>();
				int i=0;
				//GifDrawable gifDrawable = new GifDrawable(getResources(), R.drawable.cardidle);//加载一个动态图gif
				GifDrawable gifDrawable = new GifDrawable(getResources(), inResId);//加载一个动态图gif
				int totalCount = gifDrawable.getNumberOfFrames();
				for(i=0;i<totalCount;i++)
				{
					BitmapList.add(gifDrawable.seekToFrameAndGet(i));
				}
				return BitmapList;
			} catch (Exception e) {
				return null;
			}
		}
		public static OutputStream createGif(ArrayList<Bitmap> BitmapList)
		{
		// True for dither. Will need more memory and CPU
		AnimatedGIFWriter writer = new AnimatedGIFWriter(true);
		OutputStream os = null;
		{
			try {
				os = new FileOutputStream("animated.gif");
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			}
		}
		  try {
			  writer.prepareForWrite(os, -1, -1);
		  }catch (Exception e) {
			  e.printStackTrace();
		  }
	
			Bitmap bitmap;
	
			if (BitmapList.size() > 0) {
				for (int i = 0; i < BitmapList.size(); i++) {
					bitmap = BitmapList.get(i);
					try {
						writer.writeFrame(os, bitmap);
					} catch (Exception e) {
						e.printStackTrace();
					}
	
				}
			}
			
			try {
				writer.finishWrite(os);
			} catch (Exception e) {
				e.printStackTrace();
			}
	
			return os;
	// And you are done!!!
		}
		public static ByteArrayOutputStream createGif(ArrayList<Bitmap> BitmapList, int fps) throws IOException {
	
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			AnimatedGifEncoder localAnimatedGifEncoder = new AnimatedGifEncoder();
			localAnimatedGifEncoder.start(baos);//start
			localAnimatedGifEncoder.setRepeat(0);//设置生成gif的开始播放时间。0为立即开始播放
			localAnimatedGifEncoder.setDelay(fps);
	
			if (BitmapList.size() > 0) {
				for (int i = 0; i < BitmapList.size(); i++) {
					Bitmap bitmap = BitmapList.get(i);
					localAnimatedGifEncoder.addFrame(bitmap);
				}
			}
			localAnimatedGifEncoder.finish();//finish
	
			return baos;
		}
	
		public int	getResource(String imageName) {
			Context ctx = getBaseContext();
			int resId = getResources().getIdentifier(imageName, "drawable", ctx.getPackageName());
			return resId;
		}
	
			public String DisplayUI(String text) throws InterruptedException {
	
			String user_str="";
			in_string = text;
	
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					setContentView(R.layout.display_cardidle);
	
					//Intent intent=getIntent();
					//String dispmsg=intent.getStringExtra("pass_in_string");
					//Log.i("dispmsg", dispmsg);
	
					String dispmsg=in_string;
					Log.i(TAG, dispmsg);
	
					// Show status bar
					getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
	
					
					String[] dispmsginfo = dispmsg.split("\\|");
					int msgcnt = dispmsginfo.length;
	
	
					GifImageView gifImageView1 = (GifImageView) findViewById(R.id.gif1);
					try {// 如果加载的是gif动图，第一步需要先将gif动图资源转化为GifDrawable
						// 将gif图资源转化为GifDrawable
						ArrayList<Bitmap> BitmapList = new ArrayList<>();
						ArrayList<Bitmap> BitmapListNew = new ArrayList<>();
						ArrayList<Integer> Fontcolor = new ArrayList<>();
						BitmapDrawable bd = null;
	
						Fontcolor.add(Color.RED);
						Fontcolor.add(Color.BLUE);
						Fontcolor.add(Color.GREEN);
						Fontcolor.add(Color.YELLOW);
						Fontcolor.add(Color.WHITE);
						Fontcolor.add(Color.BLACK);
						
						Bitmap newbp = null;
						Bitmap tempbp;
						byte[] gifbyte = null;
	
						AnimationDrawable animation = new AnimationDrawable();
	
	
	
	
						int i = 0;
	
						int ResId =getResource(dispmsginfo[0]);
						BitmapList = getBitmapArrayByGif(ResId);
	
						Log.i("BitmapList.size()", BitmapList.size()+"");
	
						//String input = "TEST第一次";
	
						for(i=0;i<BitmapList.size();i++) {
							Log.i("BitmapList.size()", i+"");
							//Thread.sleep(5000);
							if(msgcnt == 2)
							{
								newbp = BitmapList.get(i);
							}
							else if(msgcnt == 3)
							{
								newbp = drawTextToRightTop(getApplicationContext(), BitmapList.get(i), dispmsginfo[2], 16, Color.BLACK, 6, 6);
							}
							else if(msgcnt == 4)
							{
								tempbp = drawTextToRightTop(getApplicationContext(), BitmapList.get(i), dispmsginfo[2], 16, Color.BLACK, 6, 6);
								newbp = drawTextToRightTop(getApplicationContext(), tempbp, dispmsginfo[3], 28, Color.BLACK, 10, 100);
							}
							else if(msgcnt == 5)
							{
								tempbp = drawTextToRightTop(getApplicationContext(), BitmapList.get(i), dispmsginfo[2], 16, Color.BLACK, 6, 6);
								tempbp = drawTextToRightTop(getApplicationContext(), tempbp, dispmsginfo[3], 28, Color.BLACK, 10, 100);
								newbp = drawTextToLeftTop(getApplicationContext(), tempbp, dispmsginfo[4], 28, Color.BLACK, 10, 100);
							}
	
		
							//newbp  = drawTextToCenter(getApplicationContext(), tempbp, dispmsginfo[3], 20, Color.WHITE);
							bd = new BitmapDrawable(getResources(), newbp);
							animation.addFrame(bd, 200);
	
						}
						animation.setOneShot(false);
	
							if( dispmsginfo[1].equals("front")) {
							gifImageView1.setImageDrawable(animation);
						}else {
							gifImageView1.setBackground(animation);
						}
						// start the animation!
						animation.start();
	
	
					} catch(Exception e){
						e.printStackTrace();
					}
	
				}
			});
	
			return user_str;
		}

    Bitmap myBitmap = null;
    ImageView imageView_receipt_bmp;
    File receipt_bmp_file = null;

    public String LCDDisplay(String text) throws InterruptedException {

        String user_str="";
        in_string = text;
        cardentrytimer.cancel();

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

                setContentView(R.layout.lcddisplay_message);

                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView textViewmsg;
                TextView textView_dtxn;
                TextView textView_cur;
                TextView textView_opt1;
                TextView textView_opt2;

                // For BMP Display
                ConstraintLayout linearLayout;
                LinearLayout linearLayout_bmp;

                // For Status Display
                LinearLayout linearLayout_status;
                TextView textView_status1;
                TextView textView_status2;
                ImageView imageView_status;

                LinearLayout lcd_header;
                LinearLayout lcd_txntitle;
                LinearLayout lcd_txn_cur;

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);


                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;

                textViewmsg = (TextView) findViewById(R.id.textView_msg);
                textView_dtxn = (TextView) findViewById(R.id.textView_dtxn);
                textView_cur = (TextView) findViewById(R.id.textView_cur);
                textView_opt1 = (TextView) findViewById(R.id.textView6);
                textView_opt2 = (TextView) findViewById(R.id.textView7);

                linearLayout = (ConstraintLayout) findViewById(R.id.linearLayout);
                linearLayout_bmp= (LinearLayout) findViewById(R.id.linearLayout_bmp);

                linearLayout_status = (LinearLayout) findViewById(R.id.linearLayout_status);

                lcd_header = (LinearLayout) findViewById(R.id.lcd_header);
                lcd_txntitle = (LinearLayout) findViewById(R.id.lcd_txntitle);
                lcd_txn_cur = (LinearLayout) findViewById(R.id.lcd_txn_cur);

                try
                {
                    //to do

                    for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                        System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                        switch (inIdx) {
                            case 0:
                                textView_dtxn.setText(dispmsginfo[inIdx]);
                                break;
                            case 1:
                                textViewmsg.setText(dispmsginfo[inIdx]);

                                // ---------------------------------------------------------------------------------------------------------------------------
                                /*
                                if (dispmsginfo[1].toString().equals("PROCESSING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Processing");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);

                                }
                                else if (dispmsginfo[1].toString().equals("LOADING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Loading application");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                else if (dispmsginfo[1].toString().equals("INITIALIZING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Initializing");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                else if (dispmsginfo[1].toString().equals("SENDING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Sending");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                else if (dispmsginfo[1].toString().equals("RECEIVING..."))
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_status.setVisibility(View.VISIBLE);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    imageView_status.setImageDrawable(getResources().getDrawable(R.drawable.icon_process));

                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status1.setText("Receiving");

                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);
                                    textView_status2.setText("Please wait...");

                                    // declare animation
                                    final Animation stb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.stb);
                                    final Animation ttb = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.ttb);

                                    imageView_status = (ImageView) findViewById(R.id.imageViewStatus);
                                    textView_status1 = (TextView) findViewById(R.id.box_msg_status1);
                                    textView_status2 = (TextView) findViewById(R.id.box_msg_status2);

                                    // set animation
                                    imageView_status.startAnimation(stb);

                                    textView_status1.startAnimation(ttb);
                                    textView_status2.startAnimation(ttb);
                                }
                                 */
                                if(dispmsginfo[1].toString().equals("PRINTING...")) // Printing animation
                                {
                                    linearLayout.setVisibility(View.GONE);
                                    linearLayout_bmp.setVisibility(View.VISIBLE);

                                    receipt_bmp_file = new File("/data/data/pub/Print_BMP.bmp");

                                    myBitmap = BitmapFactory.decodeFile(receipt_bmp_file.getAbsolutePath());
                                    imageView_receipt_bmp = (ImageView) findViewById(R.id.receipt_bmp);
                                    imageView_receipt_bmp.setImageBitmap(myBitmap);

                                    Animation animation = AnimationUtils.loadAnimation(getApplicationContext(),R.anim.move_up);
                                    animation.setDuration(10000);
                                    imageView_receipt_bmp.startAnimation(animation);

                                }
                                //----------------------------------------------------------------------------------------------------------------------------
                                break;
                            case 2:
                                textView_cur.setText(dispmsginfo[inIdx]);
                                break;
                            case 3:
                                textView_opt1.setText(dispmsginfo[inIdx]);
                                break;
                            case 4:
                                textView_opt2.setText(dispmsginfo[inIdx]);
                                break;
                        }
                    }

					// sidumili: Set visibility to GONE when passing no value. Fix on press cancel during printing preview
                    if (dispmsginfo[0].trim().length() <= 0 || dispmsginfo[1].trim().length() <= 0)
                        isHide = true;
                    else
                        isHide = false;

                    Log.i("sidumili", "run: isHide="+isHide+",msgcnt="+msgcnt);
                    if (isHide)
                    {
                        Log.i("sidumili", "run: Hide me now");
                        lcd_header.setVisibility(View.GONE);
                        lcd_txntitle.setVisibility(View.GONE);
                        lcd_txn_cur.setVisibility(View.GONE);
                    }
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }

            }

        });
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }
        user_str = "LCDDisplay_DONE";
        return user_str;
    }

    final CountDownTimer cardentrytimer;
    int inTimeOut=30;

    {
        cardentrytimer = new CountDownTimer(inTimeOut * 1000, 1000) {

            @Override
            public void onTick(long millisUntilFinished) {
                Log.i("Timer", "cardentrytimer onTick");
            }

            @Override
            public void onFinish() {
                Log.i("Timer", "cardentrytimer onFinish");

                inCTOSS_CARDENTRYTIMEOUT();
                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }
        };
    }

	public String GetPinUI(String text) throws InterruptedException {

		in_string = text;
//		final CtLed ctLed = new CtLed();
//		ctLed.init(MainActivity.this);


		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					for (; ; ) {
						byte[] bytes = inCTOS_GetBuffer();
//						ctLed.draw(bytes);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();

		return in_string;
	}


	public String BackToProgress(String text) throws InterruptedException {
	
			in_string = text;
	
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
	
					jni_api.REG_CB_CTOS(jni_cb_ctos);
					setContentView(R.layout.activity_disp);
					lsv_LCDTPrint = (ListView) findViewById(R.id.lsv_LCDTPrint);
					edtLog = (EditText) findViewById(R.id.edtLog);
					adapter = new ArrayAdapter(MainActivity.this, R.layout.listitem, list) {
	
						@NonNull
						@Override
						public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
							View view = super.getView(position, convertView, parent);
	
							TextView textView = (TextView) view;
	
							if (addPadding[position] != 0)
								textView.setPadding(36 * addPadding[position], 0, 0, 0);
	
	
							if (displayMode[position].equals("0")) {
								textView.setGravity(Gravity.LEFT);
							} else if (displayMode[position].equals("1")) {
								textView.setGravity(Gravity.CENTER);
							} else if (displayMode[position].equals("2")) {
								textView.setGravity(Gravity.RIGHT);
							}
	
	
							return view;
	
						}
					};
				}
			});
	
			return in_string;
		}

    public String UserInputString(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, UserInputString.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = UserInputString.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String InputStringUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;


        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, S1InputString.class);
                    //intent.setClass(MainActivity.this, UserInputString.class);


                    //bundle.putString("PARAM_DISPLAY", "PASSWORD");
                    //bundle.putString("PARAM_MINLENGTH", "1");
                    //bundle.putString("PARAM_MAXLENGTH", "6");
                    //bundle.putString("PARAM_TYPE", "1");
                    //bundle.putString("PARAM_PASSWORD", "123456");

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = S1InputString.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH S1InputString");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String UserConfirmMenu(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, ConfirmMenu.class);
//                    intent.setClass(MainActivity.this, com.Source.S1_AMEX.AMEX.model.ConfirmMenu.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = ConfirmMenu.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    private boolean isCTMSParameterFileExist() {
        String packageName = getPackageName();
        String packagePath =  "/data/data/" + getPackageName() + "/files/" + packageName.toUpperCase() + ".prm";
        Log.i(TAG, "packagePath: " + packagePath);
        File file = new File(packagePath);
        if(file.exists()) {
            Log.i(TAG, "reset FIRSTRUN: ");
            return true;
        }
        return false;
    }

			public String BatchReviewUI(String text) throws InterruptedException {
		
				String user_str="";
				pass_in_string = text;
		
				new Thread()
				{
					public void run()
					{
						try
						{
							Intent intent = new Intent();
							Bundle bundle = new Bundle();
		
							intent.setClass(MainActivity.this, BatchReview.class);
		
							bundle.putString("pass_in_string", pass_in_string);
							Log.i("pass_in_string", pass_in_string);
		
							intent.putExtras(bundle);
							startActivity(intent);
							//startActivityForResult(intent, 0);
						}
						catch(Exception e)
						{
							e.printStackTrace();
						}
					}
				}.start();
		
				MainActivity.LOCK.setCondition(false);
		
				synchronized (LOCK) {
					while (!LOCK.conditionMet()) {
						try {
							LOCK.wait();
						} catch (InterruptedException e) {
							Log.e(TAG, "Exception when waiting for condition", e);
							return "Exception";
						}
					}
				}
		
				user_str = BatchReview.final_string;
		
				Log.i("PATRICK", user_str);
				Log.i("PATRICK", "FINISH 789");
				//pin_num = pin_num + "XXX";
		//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
				return user_str;
			}	

			
				public String BatchReviewUIPHQR(String text) throws InterruptedException {
			
					String user_str="";
					pass_in_string = text;
			
					new Thread()
					{
						public void run()
						{
							try
							{
								Intent intent = new Intent();
								Bundle bundle = new Bundle();
			
								intent.setClass(MainActivity.this, BatchReview2.class);
			
								bundle.putString("pass_in_string", pass_in_string);
								Log.i("pass_in_string", pass_in_string);
			
								intent.putExtras(bundle);
								startActivity(intent);
								//startActivityForResult(intent, 0);
							}
							catch(Exception e)
							{
								e.printStackTrace();
							}
						}
					}.start();
			
					MainActivity.LOCK.setCondition(false);
			
					synchronized (LOCK) {
						while (!LOCK.conditionMet()) {
							try {
								LOCK.wait();
							} catch (InterruptedException e) {
								Log.e(TAG, "Exception when waiting for condition", e);
								return "Exception";
							}
						}
					}
			
					user_str = BatchReview2.final_string2;
			
					Log.i("PATRICK", user_str);
					Log.i("PATRICK", "FINISH 789");
					//pin_num = pin_num + "XXX";
			//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
					return user_str;
				}	

			public String UserConfirmOKMenu(String text) throws InterruptedException {
		
				String user_str="";
				pass_in_string = text;
		
				new Thread()
				{
					public void run()
					{
						try
						{
							Intent intent = new Intent();
							Bundle bundle = new Bundle();
		
							intent.setClass(MainActivity.this, ConfirmOKMenu.class);
		
							bundle.putString("pass_in_string", pass_in_string);
							Log.i("pass_in_string", pass_in_string);
		
							intent.putExtras(bundle);
							startActivity(intent);
							//startActivityForResult(intent, 0);
						}
						catch(Exception e)
						{
							e.printStackTrace();
						}
					}
				}.start();
		
				MainActivity.LOCK.setCondition(false);
		
				synchronized (LOCK) {
					while (!LOCK.conditionMet()) {
						try {
							LOCK.wait();
						} catch (InterruptedException e) {
							Log.e(TAG, "Exception when waiting for condition", e);
							return "Exception";
						}
					}
				}
		
				user_str = ConfirmOKMenu.final_string;
		
				Log.i("PATRICK", user_str);
				Log.i("PATRICK", "FINISH 789");
				//pin_num = pin_num + "XXX";
		//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
				return user_str;
			}

		public String UserSelectUpDown(String text) throws InterruptedException {
	
			String user_str="";
			pass_in_string = text;
	
			new Thread()
			{
				public void run()
				{
					try
					{
						Intent intent = new Intent();
						Bundle bundle = new Bundle();
	
						intent.setClass(MainActivity.this, UpDownMenu.class);
	
						bundle.putString("pass_in_string", pass_in_string);
						
						intent.putExtras(bundle);
						startActivity(intent);
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			}.start();
	
			MainActivity.LOCK.setCondition(false);
	
			synchronized (LOCK) {
				while (!LOCK.conditionMet()) {
					try {
						LOCK.wait();
					} catch (InterruptedException e) {
						Log.e(TAG, "Exception when waiting for condition", e);
						return "Exception";
					}
				}
			}
	
			user_str = UpDownMenu.final_string;
	
			Log.i("PATRICK", "FINISH 789");
			//pin_num = pin_num + "XXX";
	//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
			return user_str;

		}


    public String getAnyNumStr(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, NormalKeyBoard.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "IPT_ERR";
                }
            }
        }

        user_str = NormalKeyBoard.user_num_str;
        //Log.i("PATRICK123456", pin_num);
        Log.i("PATRICK", "FINISH 777");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String DOptionMenuDisplay(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, DOptionMenu.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "IPT_ERR";
                }
            }
        }

        user_str = DOptionMenu.select_item;
        //Log.i("PATRICK123456", pin_num);
        Log.i("PATRICK", "FINISH 777");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String DPopupMenuDisplay(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, DPopupMenuActivity.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();


        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "IPT_ERR";
                }
            }
        }

        user_str = DPopupMenuActivity.select_item;
        //Log.i("PATRICK123456", pin_num);
        Log.i("PATRICK", "FINISH 777");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

    public String UserConfirmMenuInvandAmt(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, UserConfirmVoid.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = UserConfirmVoid.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH 789");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;

    }

@SuppressLint("LongLogTag")
public String GetPIN_With_3DESDUKPTEx(int KeySet, int KeyIndex, String pInData, int pinBypassAllow) throws InterruptedException {


	inKeySet = KeySet;
	inKeyIndex = KeyIndex;
	StrInData = pInData;	
	inPinBypassAllow = pinBypassAllow;
	Enter_Press = 0;
	inKeyType = 3;
	


	Log.i("saturn GetPIN_With_3DESDUKPTEx", inKeySet+"");
	Log.i("saturn GetPIN_With_3DESDUKPTEx", inKeyIndex+"");
	Log.i("saturn GetPIN_With_3DESDUKPTEx", StrInData+"");

	kmsui.init(MainActivity.this, inKeySet, inKeyIndex, StrInData, inKeyType, inPinBypassAllow);

	while(MainActivity.Enter_Press==0)
	{
		//Log.d(TAG_A, MainActivity.Enter_Press+"");
	}

	Log.d("saturn GetPIN_With_3DESDUKPTEx", MainActivity.Enter_Press+"");


	OutputStr = String.valueOf(MainActivity.inRet)+'*'+sPinBlock+'|'+sKSN;

	Log.i("saturn GetPIN_With_3DESDUKPTEx", OutputStr);
  
	return OutputStr;

}

public String GetRootFS(String text) throws InterruptedException {
 CtSystem system = new CtSystem(); 

 
 Log.i("saturn GetRootFS", OutputStr);
 // GetRootFS	 
 int Rootfs = 30; 
 //byte buf[] = new byte[17]; 

 
 try {		
 Log.d(TAG, "saturn ROOTFS Version = " + system.getModuleVersion(Rootfs)); 
  
 String buf = system.getModuleVersion(Rootfs); 			
 //Log.d(TAG, String.format("saturn Rootfs = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", buf[0], buf[1], buf[2], 
//							buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] , 
//							buf[10], buf[11], buf[12], buf[13], buf[14], buf[15])); 	 

//OutputStr = String.format("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", buf[0], buf[1], buf[2], 
	//							   buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] , 
	//							   buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);		
	OutputStr = buf;
 
 } catch (CtSystemException e) 
 {e.showStatus();		   
 }	 

 return OutputStr;

}

    public String ReceiptOnScreen(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        //int inTimeOut=0;

		inPrintConfirmTimeOut=inCTOSS_GetEnvInt("PRINT_CONF_TO");
		if(inPrintConfirmTimeOut <= 0)
			inPrintConfirmTimeOut=10;


        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, PrintReceiptonScreen.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = PrintReceiptonScreen.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH PrintReceiptonScreen");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

    public String EliteReceiptOnScreen(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, EliteReceiptonScreen.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = EliteReceiptonScreen.final_string;

        Log.i("TINE", user_str);
        Log.i("TINE", "FINISH EliteReceiptonScreen");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }
	
    public String UserConfirmCard(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        cardentrytimer.cancel();

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, UserConfirmCard.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = UserConfirmCard.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH UserConfirmCard");
        return user_str;
    }

	public String AppPackageInfo (String text) {

        int x=0;
        String szCRC = "";
		String szAppCRC = "";
        szAppCRC = text;
        Log.i("TINE", "getAppInfor - START");
		Log.i("szAppCRC", ""+szAppCRC);
        GetAppInfor getAppInfor = new GetAppInfor(mContext);
        //String currentSignature = getAppInfor.getInstalledAppHashCode();
        String currentSignature = getAppInfor.getAppHashCode(szAppCRC);
        Log.i("currentSignature", "" + currentSignature);
        //Toast.makeText(getApplicationContext(), "currentSignature " + currentSignature, Toast.LENGTH_SHORT).show();
        x=0;
        for(int i=0; i<9; i++)
        {
            szCRC = szCRC + currentSignature.substring(x, x+1);
            x = x+4;
        }

        Log.i("szCRC", ""+szCRC);
        return szCRC;
	}

	/*sidumili: modification to display text and image*/
	public String DisplayBox(String text) throws InterruptedException {

        String user_str="";
        in_string = text;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                if(MainActivity.inTermType == 5)
                    setContentView(R.layout.displaybox_message_s1em);
				else
					setContentView(R.layout.displaybox_message);


                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView box_msg;
                TextView box_msg2;
                TextView box_msg3;
                ImageView imageView;

                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;
                String image_str = "";

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

                for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                    System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                    switch (inIdx) {
                        case 0: // Message 1
                            box_msg = (TextView) findViewById(R.id.box_msg);
                            box_msg.setText(dispmsginfo[inIdx]);
                            break;
                        case 1: // Message 2
                            box_msg2 = (TextView) findViewById(R.id.box_msg2);
                            box_msg2.setText(dispmsginfo[inIdx]);
                            break;
                        case 2: // Message 3
                            box_msg3 = (TextView) findViewById(R.id.box_msg3);
                            box_msg3.setText(dispmsginfo[inIdx]);
                            break;
                        case 3: // Image Icon
                            image_str = dispmsginfo[inIdx];
                            System.out.println("image_str [" + image_str + "]");
                            imageView = (ImageView) findViewById(R.id.imageView);
                            wub_lib.ViewImageResourcesByType(image_str, imageView); // sidumili: added to call function in any java, code optimization
                            break;
                    }
                }

            }

        });
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }
        return user_str;
    }

    public String DisplayStatusBox(String text) throws InterruptedException {

        String user_str="";
        in_string = text;
        cardentrytimer.cancel();

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                if(MainActivity.inTermType == 5)
                    setContentView(R.layout.displaystatusbox_message_s1em);
                else
                    setContentView(R.layout.displaystatusbox_message);


                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView box_msg;
                TextView box_msg2;
                ImageView imageView;

                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;
                String image_str = "";

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

                for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                    System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                    switch (inIdx) {
                        case 0: // Message 1
                            box_msg = (TextView) findViewById(R.id.box_msg);
                            box_msg.setText(dispmsginfo[inIdx]);
                            break;
                        case 1: // Message 2
                            box_msg2 = (TextView) findViewById(R.id.box_msg2);
                            box_msg2.setText(dispmsginfo[inIdx]);
                            break;
                        case 2: // Image Icon
                            image_str = dispmsginfo[inIdx];
                            System.out.println("image_str [" + image_str + "]");
                            imageView = (ImageView) findViewById(R.id.imageView);
                            wub_lib.ViewImageResourcesByType(image_str, imageView); // sidumili: added to call function in any java, code optimization
                            break;
                    }
                }

                if(image_str.equals("process") || image_str.equals("wait") || image_str.equals("init"))
                    AnimateProcessing();
                else
                    AnimationMessageStatusBox();
            }
        });
        // synchronized (MainActivity.LOCK) {
        //    MainActivity.LOCK.setCondition(true);
        //    MainActivity.LOCK.notifyAll();

        // }
        return user_str;
    }

    public void AnimationMessageBox()
    {
        TextView box_msg;
        TextView box_msg2;
        TextView box_msg3;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.stb);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);
        box_msg3 = (TextView) findViewById(R.id.box_msg3);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);
        box_msg3.startAnimation(ttb);

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

    }

    public void AnimationMessageStatusBox()
    {
        TextView box_msg;
        TextView box_msg2;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.stb);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);

        // synchronized (MainActivity.LOCK) {
        //    MainActivity.LOCK.setCondition(true);
        //    MainActivity.LOCK.notifyAll();

        // }

    }

    public void AnimateProcessing()
    {
        TextView box_msg;
        TextView box_msg2;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.clockwise);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);

        // synchronized (MainActivity.LOCK) {
        //    MainActivity.LOCK.setCondition(true);
        //    MainActivity.LOCK.notifyAll();

        // }

    }

    public void AnimateTimeout()
    {
        TextView box_msg;
        TextView box_msg2;
        TextView box_msg3;
        ImageView imageView;

        // declare animation
        final Animation stb = AnimationUtils.loadAnimation(this, R.anim.halfturn);
        final Animation ttb = AnimationUtils.loadAnimation(this, R.anim.ttb);

        imageView = (ImageView) findViewById(R.id.imageView);
        box_msg = (TextView) findViewById(R.id.box_msg);
        box_msg2 = (TextView) findViewById(R.id.box_msg2);
        box_msg3 = (TextView) findViewById(R.id.box_msg3);

        // set animaiton
        imageView.startAnimation(stb);

        box_msg.startAnimation(ttb);
        box_msg2.startAnimation(ttb);
        box_msg3.startAnimation(ttb);

        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

    }

	public String DisplayErrorMsg2(String text) throws InterruptedException {

        String user_str="";
        in_string = text;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                overridePendingTransition(0, 0); // disable the animation, faster

                setContentView(R.layout.display_errorbox2);

                //Intent intent=getIntent();
                //String dispmsg=intent.getStringExtra("pass_in_string");
                //Log.i("dispmsg", dispmsg);

                String dispmsg = in_string;
                Log.i(TAG, dispmsg);

                TextView error_msg1;
                TextView error_msg2;
                TextView error_msg3;

                // Show status bar
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

                String[] dispmsginfo = dispmsg.split("\\|");
                int msgcnt = dispmsginfo.length;

                error_msg1 = (TextView) findViewById(R.id.error_msg1);
                error_msg2 = (TextView) findViewById(R.id.error_msg2);
                error_msg3 = (TextView) findViewById(R.id.error_msg3);

                error_msg1.setText("");
                error_msg2.setText("");
                error_msg3.setText("");

                try
                {
                    //to do

                    for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
                        System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
                        switch (inIdx) {
                            case 0:
                                error_msg1.setText(dispmsginfo[inIdx]);
                                break;
                            case 1:
                                error_msg2.setText(dispmsginfo[inIdx]);
                                break;
                            case 2:
                                error_msg3.setText(dispmsginfo[inIdx]);
                                break;
                        }
                    }
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }

            }

        });
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }
        return user_str;
    }


    public String fGetConnectStatus(String text) {

        String user_str="";
        //in_string = text;
        ConnectivityManager cm = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        if (activeNetwork != null) {
            Log.i("saturn", "AAA activeNetwork is active");
            // connected to the internet
            if (text.equals("4")) {
                if (activeNetwork.getType() == ConnectivityManager.TYPE_WIFI) {
                    // connected to wifi
                    Log.i("saturn", "AAA active getType is WIFI");
                    user_str = "YES";
                } else if (activeNetwork.getType() == ConnectivityManager.TYPE_MOBILE) {
                    // connected to mobile data
                    Log.i("saturn", "AAA active getType is mobile");
                    user_str = "FALLBACK";
                }
            }
            if (text.equals("2"))
            {
                //if (activeNetwork.getType() == ConnectivityManager.TYPE_MOBILE)
                    user_str = "YES";
                //else
                    //user_str = "NO";
            }
        } else {
            // not connected to the internet
            Log.i("saturn", "AAA active getType is no connectivity");
            user_str = "NO";
        }

 /*
			Log.i("saturn", "saturn GetConnectionStatus text " + text);
		GetNetworkInfor getNetworkInfor = new GetNetworkInfor(mContext);


        if (text.equals("4")){

			Log.i("saturn", "saturn wifi mode");
			String wifiSSID = getNetworkInfor.getWifiSSID();

			if (wifiSSID != null)
				user_str = "YES";
			else
				user_str = "NO";


        }else{
        Log.i("saturn", "saturn gprs  mode");
			Boolean isonline = getNetworkInfor.isOnline();
			if (isonline == true)
				user_str = "YES";
			else
				user_str = "NO";
		}

		Log.i("saturn", "saturn test GetConnectionStatus: " + user_str);
	*/
        return user_str;

    }

	public String UserConfirmMenu2(String text) throws InterruptedException {

		String user_str="";
		pass_in_string = text;

		new Thread()
		{
			public void run()
			{
				try
				{
					Intent intent = new Intent();
					Bundle bundle = new Bundle();

					intent.setClass(MainActivity.this, ConfirmMenu2.class);

					bundle.putString("pass_in_string", pass_in_string);
					Log.i("pass_in_string", pass_in_string);

					intent.putExtras(bundle);
					startActivity(intent);
					//startActivityForResult(intent, 0);
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
			}
		}.start();

		MainActivity.LOCK.setCondition(false);

		synchronized (LOCK) {
			while (!LOCK.conditionMet()) {
				try {
					LOCK.wait();
				} catch (InterruptedException e) {
					Log.e(TAG, "Exception when waiting for condition", e);
					return "Exception";
				}
			}
		}

		user_str = ConfirmMenu2.final_string;

		Log.i("PATRICK", user_str);
		Log.i("PATRICK", "FINISH 789");
		//pin_num = pin_num + "XXX";
//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
		return user_str;
	}

    public String GetSerialNumber(String text) throws InterruptedException {
        CtSystem system = new CtSystem();


        Log.i("saturn GetSerialNumber", OutputStr);
        // getFactorySN
        byte buf[] = new byte[17];

        try {
            buf = system.getFactorySN();
            Log.d(TAG, String.format("saturn FactorySN = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", buf[0], buf[1], buf[2],
                    buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] ,
                    buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]));

            OutputStr = String.format("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", buf[0], buf[1], buf[2],
                    buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] ,
                    buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);

        } catch (CtSystemException e)
        {e.showStatus();
        }

        return OutputStr;

    }


	public String GetRSA(String input) throws Exception {

			Log.i("SATURN rsa input =",input);

			//String fileName = "/data/data/pub/private-key.pem";
			//String filePath = "/data/data/pub/private-key.pem";

			String strPk = "";

        //strPk = new String ( Files.readAllBytes( Paths.get(filePath) ) );

			
			//String actual = Files.readString(fileName);
			//Log.i("SATURN rsa key =",strPk);


			//String input2 = "{\"mid\":\"800000000000014\",\"reqMsgId\":\"523401F0286A45C59F753446C10C80A3\",\"reqTime\":\"2021-11-02T15:11:08.963+08:00\",\"serialNo\":\"0821772041\",\"tid\":\"80000014\"}";
			 
			// Not a real private key! Replace with your private key!
			//String strPk = "-----BEGIN PRIVATE KEY-----"
			//+"MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQCY6HT0Joo6f+jD"
			//+"eeG/CrTWbKvp1QnAu4EKAfRMo3I8YJ+l29XTNidBzOyADMd/2GCYEWwLKFVjWWzP"
			//+"555OrrFKS6NozLvP5EpJ2SOtuCdduM5Zo22i5uzXj1N3TPhQwhZBl47BnANl2OUt"
			//+"UPNxql463shb2IguNM/AkckJEvHUNTj4wd3fLGp87sEAyPZB7Jxc+QySkyNNrlAX"
			//+"K5iBB2RlVHHiUuej3UYmrsOwcD8twZIdG6+Npajmx6ot75flBVX4sQuIsx1gAxAT"
			//+"2u6vNL+Dp9ulk8BT3x0PPY3CAM2zt/Nz/ql3gRWDTJUhokkeHbjzx2GKjsFqefzu"
			//+"9ZeF7UU5AgMBAAECggEANjAC6lcSG2Mi4Ms7phEsPuTqE4bM2wFX3G0O32342M7B"
			//+"JO0AHMaPKPFvSn284WCT8rAR4u5bPEb/mboSTv33lEWz1jrfv+++J+eMjk65nKwW"
			//+"bIvPGbDOdCG1yYx8X1o4uuvzykb4qUoxBVsvurNOi3xR8sKMmBtdGHhRjyaojrP1"
			//+"irvLfaV1C1xHeQcIqwaz/WPPANNDwU7f3PHM1BMqhvZynJqALUew7jB9ACZbQZET"
			//+"DdLBtmySQG56TofJS5lTikMtgntlrXQDHgRcQJNL//R3n+ZkPRlCeeO7s6cv/HCc"
			//+"/XiGRr3IAZIU3GYGOxBY/v5QqjX/4o86OQSkd2XPVQKBgQDI2E/noFJpWs5r4tSe"
			//+"z9ah0cW5qtVH0nVRC8NSycL2CeMrvCEHTBJr9JawfXi0abRkK21KwPacBLGs5shv"
			//+"ngqJ5PveuhhyA/0i+fzLC4zLz2GLeWIk5dAcOqaCEJ6NMryLBfyq8JrY0PBdFs2c"
			//+"UHAqtmffVBD3akkVFj1PBXwDVwKBgQDC5hwdiUbbJsNkR94q3pw+rzl9/yPBQ2Ww"
			//+"6EVdwe/vqXXzyaioibRGlQ+3v59bU0wwWR6TbydjPscomMCVOG8bLgLBL6MfOD9I"
			//+"Ef4JIXCiWsboUxp4G6uP3n89yVCp8z8uYj2yCcUmZ7bUu/+UPnzphnMT2UbiaN8Z"
			//+"KLOMloGx7wKBgQCZd1gPGB0Arcc+/rMSqqNJQL4B8fxIhhLom8FjGBIl6EYyW+0s"
			//+"4z7MSLeAIk/CI2cYh2pajXmim5+cCzgquas51MN0W+Rj8pJRUI2OJMQv08WoSlbo"
			//+"9MWBXHtwtPxBzfvtiuSXRYQ8meKgbtUrVPdwW7KhHl5cf1VajITKRdmNvwKBgQC4"
			//+"wUY5wZ6HtRFYiMBUxz46NgU8KZcNaASIVUccf/IhsPT9q1bUneVlW3zQ5Tf9bNM2"
			//+"qeTQQniGHkbRdyxoyFya2V0HIU9zEvD6MwQsR6TsYz1Xkx/uZ9yUv4jDW4m58bsd"
			//+"D0gI9DYOGBc7RxQaxbnH7go88KW/j0aeJPU/+pTrxQKBgQCOwh4tLe5aLc9LqNBX"
			//+"78Kui1cBrv9LQpz2dtqReIV/sCwKURUoiv0GUB1OrPaBifyJBWNVPCJooimrnLff"
			//+"QI/qRAIfU+aWExbxq2mc3pasG1GgGBcU95ukz4tgl0TqBWE+jNe7E2Uikbk/vUsO"
			//+"F/q0q7Ev7pbsHnRrZWMYfFFVCQ=="
			//+"-----END PRIVATE KEY-----";

			if(inCTOSS_GetEnvInt("UAT_PRIVATE_KEY") == 1)
			{
    			strPk = "-----BEGIN PRIVATE KEY-----"
    			+"MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQCY6HT0Joo6f+jD"
    			+"eeG/CrTWbKvp1QnAu4EKAfRMo3I8YJ+l29XTNidBzOyADMd/2GCYEWwLKFVjWWzP"
    			+"555OrrFKS6NozLvP5EpJ2SOtuCdduM5Zo22i5uzXj1N3TPhQwhZBl47BnANl2OUt"
    			+"UPNxql463shb2IguNM/AkckJEvHUNTj4wd3fLGp87sEAyPZB7Jxc+QySkyNNrlAX"
    			+"K5iBB2RlVHHiUuej3UYmrsOwcD8twZIdG6+Npajmx6ot75flBVX4sQuIsx1gAxAT"
    			+"2u6vNL+Dp9ulk8BT3x0PPY3CAM2zt/Nz/ql3gRWDTJUhokkeHbjzx2GKjsFqefzu"
    			+"9ZeF7UU5AgMBAAECggEANjAC6lcSG2Mi4Ms7phEsPuTqE4bM2wFX3G0O32342M7B"
    			+"JO0AHMaPKPFvSn284WCT8rAR4u5bPEb/mboSTv33lEWz1jrfv+++J+eMjk65nKwW"
    			+"bIvPGbDOdCG1yYx8X1o4uuvzykb4qUoxBVsvurNOi3xR8sKMmBtdGHhRjyaojrP1"
    			+"irvLfaV1C1xHeQcIqwaz/WPPANNDwU7f3PHM1BMqhvZynJqALUew7jB9ACZbQZET"
    			+"DdLBtmySQG56TofJS5lTikMtgntlrXQDHgRcQJNL//R3n+ZkPRlCeeO7s6cv/HCc"
    			+"/XiGRr3IAZIU3GYGOxBY/v5QqjX/4o86OQSkd2XPVQKBgQDI2E/noFJpWs5r4tSe"
    			+"z9ah0cW5qtVH0nVRC8NSycL2CeMrvCEHTBJr9JawfXi0abRkK21KwPacBLGs5shv"
    			+"ngqJ5PveuhhyA/0i+fzLC4zLz2GLeWIk5dAcOqaCEJ6NMryLBfyq8JrY0PBdFs2c"
    			+"UHAqtmffVBD3akkVFj1PBXwDVwKBgQDC5hwdiUbbJsNkR94q3pw+rzl9/yPBQ2Ww"
    			+"6EVdwe/vqXXzyaioibRGlQ+3v59bU0wwWR6TbydjPscomMCVOG8bLgLBL6MfOD9I"
    			+"Ef4JIXCiWsboUxp4G6uP3n89yVCp8z8uYj2yCcUmZ7bUu/+UPnzphnMT2UbiaN8Z"
    			+"KLOMloGx7wKBgQCZd1gPGB0Arcc+/rMSqqNJQL4B8fxIhhLom8FjGBIl6EYyW+0s"
    			+"4z7MSLeAIk/CI2cYh2pajXmim5+cCzgquas51MN0W+Rj8pJRUI2OJMQv08WoSlbo"
    			+"9MWBXHtwtPxBzfvtiuSXRYQ8meKgbtUrVPdwW7KhHl5cf1VajITKRdmNvwKBgQC4"
    			+"wUY5wZ6HtRFYiMBUxz46NgU8KZcNaASIVUccf/IhsPT9q1bUneVlW3zQ5Tf9bNM2"
    			+"qeTQQniGHkbRdyxoyFya2V0HIU9zEvD6MwQsR6TsYz1Xkx/uZ9yUv4jDW4m58bsd"
    			+"D0gI9DYOGBc7RxQaxbnH7go88KW/j0aeJPU/+pTrxQKBgQCOwh4tLe5aLc9LqNBX"
    			+"78Kui1cBrv9LQpz2dtqReIV/sCwKURUoiv0GUB1OrPaBifyJBWNVPCJooimrnLff"
    			+"QI/qRAIfU+aWExbxq2mc3pasG1GgGBcU95ukz4tgl0TqBWE+jNe7E2Uikbk/vUsO"
    			+"F/q0q7Ev7pbsHnRrZWMYfFFVCQ=="
    			+"-----END PRIVATE KEY-----";
			}
			else
			{
    			strPk = "-----BEGIN PRIVATE KEY-----"
    			+"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDLzZI9dW0cHdQt"
    			+"EgtQGV++pflvIE6p1S1+Z+n4QeCifBW0cpvtPUcD2B5s7mGjNmUIMehHc5iKwm7g"
    			+"N/z8HWRfb8r4tkFCtiznAfNpF5jcA93Ix0t84axQ3GyUS6j4wnWBQ1+R3EdcV/DL"
    			+"RKuqx6D2pX5gbEFMuLQbgP5H0a0R+dXCBvZrUxjYbsFgfad0IAackBflPo3kI2zH"
    			+"lS1dqUYMJaY5+3jMnLKWzm7CgHI2U2upz8CLeNKW5bgXQkdKc+jYD1KHPZOi+t4s"
    			+"gAd2pa3G/9YHWepjv0fhjcvzece2U3okIMUQ6Il1YbP9858eEqsfMW1i2HHqrq+B"
    			+"gCaPhqE3AgMBAAECggEBAJq5oCvuqX5uGvZ4u4P9L5ZS9bLrSS1gveA4zgV/JibV"
    			+"o7jSRhTvr6l4e2jh9dhuZHe7RAhy9z1pbbnmZ7eULO7TtbnqzFL0cdSTw5NmqdNn"
    			+"UAqtW18dCbB+GCTh6A+run//gWAAZ2DxFsuLf/TxCAAZFn/n0A+KLpqu7N3jjsyU"
    			+"kF50C+KpVS+wiZq9Qmp4EFfu7QfOjqp7l5I0dY2R/WAN+XI7qy7E111hpxhqUvu4"
    			+"22N4JzpFx7671N318HAdyhB285TNThruBKb7aAl9IvvfdUMDbLCrE7oi79CzOgeR"
    			+"ct8t48TcdOuddMfTuT21L1Pcoq1jhfIwGWPRrRjXbhkCgYEA5vZeoJyux2hYQwbE"
    			+"NqW1+AkfLY9dMl81gPmD3BxMh5Mp1n5l2uytL+xwPWswYnW3QdQLShQDN1NGik1X"
    			+"PGWnzOeowx1JJ3o9K9b+Bs3SihglFFamtnmOOyaOZduQpikyPR++HAVyn6CTKqsP"
    			+"LYLR5j/JQWWPCTn3rKQ3tg6uHTUCgYEA4eV6r2RYh6nBvsmmG/qFjx5oHgPt81a7"
    			+"/eBAK8MTToSIaCyMBh1Ou250/m3I7Pcid+Lel3njTWTUx1FZ9LEeHNKevELE+U0n"
    			+"LPRm5cLGvKuyvyqp6X0lKtrFZFXqV3F7boAjpa5bWYyDvlvzUsjPyP+4Ev5sPYSv"
    			+"5HOTAms5DjsCgYB9IOkvdvOyETQQTDAtsyWdFcQQeOVzDuX71DK+I47KvM4j6SLh"
    			+"suw5VI2SmOoDzebVAT6EicccSD+lWjVzAVghyThDTTB+9NkDUV0a7pOsQVyFYlS7"
    			+"86s9HKx1PrllbFhNiU8D+2eYV/SjXz2/2GpDpjcw+Qa/ZupqXCXauqVWKQKBgA2N"
    			+"kxrKznT1PmaP+qNbGIBJtr12RuR+o7DLr43wmkvvr69wXMZRI9QjAGnzdEajXTIM"
    			+"LoawDW8rSYjb3KXNqpzYFiX9a4v5kQwlbbcDRKE1O/g0OxWDKKO1lHo8nV65tEog"
    			+"7gKWv2xT/eOB6MtiuPS6QvUAxsW6HEN8m6ldfYLZAoGBAN1gkM1X99y9Q0JExD1F"
    			+"XHQe3svj1UCBQ+sG6eHf3sFyfMdmsQ8NURIWAFF29FwwJocitogD2yZyRmmVKrsW"
    			+"6X6lAvmeIcz2SDrRKgHTmtHklteTvI/V8O/ocM2SC2JkIQn6nhyUueEfKXWW6kl4"
    			+"9rIeoiMFUiA60gyhiy0y4H0v"
    			+"-----END PRIVATE KEY-----";
			}
			 
			String base64Signature = signSHA256RSA(input,strPk);
			//System.out.println("Signature="+base64Signature);
			
			Log.i("SATURN rsa Signature =",base64Signature);
			return base64Signature;
		}
		
		// Create base64 encoded signature using SHA256/RSA.
		public static String signSHA256RSA(String input, String strPk) throws Exception {

		    Log.i("SATURN rsa signSHA256RSA =",strPk);
			// Remove markers and new line characters in private key
			String realPK = strPk.replaceAll("-----END PRIVATE KEY-----", "")
								 .replaceAll("-----BEGIN PRIVATE KEY-----", "")
								 .replaceAll("\n", "");
		
			byte[] b1 = Base64.getDecoder().decode(realPK);
			PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(b1);
			KeyFactory kf = KeyFactory.getInstance("RSA");
		
			Signature privateSignature = Signature.getInstance("SHA256withRSA");
			privateSignature.initSign(kf.generatePrivate(spec));
			privateSignature.update(input.getBytes("UTF-8"));
			byte[] s = privateSignature.sign();
			return Base64.getEncoder().encodeToString(s);
		
		}

		public String GetHMAC256(String input) throws Exception {
			
			String[] szBufferinfo = input.split("\\|");
			int msgcnt = szBufferinfo.length;
			String Buff="";
			String key="";
			String szHMAC="";


			Log.i("GetHMAC256 input ",input);
			Log.i("szBufferinfo 0 ",szBufferinfo[0]);
			Log.i("szBufferinfo 1 ",szBufferinfo[1]);
			
			//Log.i("szBufferinfo COUNT "+msgcnt);

			
			for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
				System.out.println("split msg [" + inIdx + "][" + szBufferinfo[inIdx] + "]");
				switch (inIdx) {
					case 0:
						Buff = szBufferinfo[inIdx];
						break;
					case 1:
						key = szBufferinfo[inIdx];
						break;
				}
			}

			Log.i("Test BUFF ",Buff);
			Log.i("test KEY ",key);

			//key = "8F341559D9604517BCC2A90881B0D2DA";
			//Buff = "80000000000004480000044";


			Mac sha256_HMAC = Mac.getInstance(ALGORITHM);

            SecretKeySpec secret_key = new SecretKeySpec(key.getBytes("UTF-8"), ALGORITHM);
            sha256_HMAC.init(secret_key);

            szHMAC = byteArrayToHex(sha256_HMAC.doFinal(Buff.getBytes("UTF-8")));

			Log.i("szHMAC ",szHMAC);

			return szHMAC;

		}
		
		public static String byteArrayToHex(byte[] a) {
				StringBuilder sb = new StringBuilder(a.length * 2);
				for(byte b: a)
					sb.append(String.format("%02x", b));

				//Log.i("byteArrayToHex ",sb.toString);
				return sb.toString();
			}
		


	    public String MenuTransactions(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;


		Log.i("SATURN MenuTransactions", "test");


        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, Transactions.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("SATURN pass_in_string", pass_in_string);

                    sLocalAppHeaderDetails = GetAppHeaderDetails();
                    bundle.putString("AppHeaderBuff", sLocalAppHeaderDetails);
                    Log.i("SATURN AppHeaderBuff", "test header");

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = Transactions.select_item;

        Log.i("TINE", "FINISH MenuTransactionType "+user_str);

        //Toast.makeText(this, user_str, Toast.LENGTH_LONG).show();

        return user_str;
    }

    public String TransDetailListViewUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        Log.i("TransDetailListViewUI:", "1");
        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    Log.i("TransDetailListViewUI:", "2");
                    intent.setClass(MainActivity.this, TransDetailListView.class);
                    Log.i("TransDetailListViewUI:", "3");
                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str =  TransDetailListView.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH TransDetailListViewUI");
        //pin_num = pin_num + "XXX";
//        Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }

    public String UserConfirmQRCode(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;

        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();

                    intent.setClass(MainActivity.this, ConfirmQRCode.class);

                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str = ConfirmQRCode.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH UserConfirmQRCode");
        //pin_num = pin_num + "XXX";
//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
        return user_str;
    }


		public String UserConfirmQRCodePHQR(String text) throws InterruptedException {
	
			String user_str = "TIME OUT"; // patrick fix crash by initial user_str value.

			pass_in_string = text;
	
			new Thread()
			{
				public void run()
				{
					try
					{
						Intent intent = new Intent();
						Bundle bundle = new Bundle();
	
						intent.setClass(MainActivity.this, ConfirmQRCodePHQR.class);
	
						bundle.putString("pass_in_string", pass_in_string);
						Log.i("pass_in_string", pass_in_string);
	
						intent.putExtras(bundle);
						startActivity(intent);
						//startActivityForResult(intent, 0);
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			}.start();
	
			MainActivity.LOCK.setCondition(false);
	
			synchronized (LOCK) {
				while (!LOCK.conditionMet()) {
					try {
						LOCK.wait();
					} catch (InterruptedException e) {
						Log.e(TAG, "Exception when waiting for condition", e);
						return "Exception";
					}
				}
			}
	
			user_str = ConfirmQRCodePHQR.final_string2;
	
			Log.i("PATRICK", user_str);
			Log.i("PATRICK", "FINISH UserConfirmQRCode");
			//pin_num = pin_num + "XXX";
	//		  Toast.makeText(this, pin_num, Toast.LENGTH_SHORT).show();
			return user_str;
		}
	



	public String usSetDateAndTime(String text) {
		
		//int iYear=0,iMonth=0, iDay=0, iHour=0, iMinutes=0, iSeconds=0;
		String sYear="",sMonth="", sDay="", sHour="", sMinutes="", sSeconds="";

		String user_str="";
		in_string = text;
		String dispmsg = in_string;
        String szCurrentYear ="";
		
        String szCompleteDateTime ="";
		
		String[] dispmsginfo = dispmsg.split("\\|");
		int msgcnt = dispmsginfo.length;

		for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
			System.out.println("saturn split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
			switch (inIdx) {
				case 0:
					//iMonth = Integer.parseInt(dispmsginfo[inIdx]);
					sMonth = dispmsginfo[inIdx];
					break;
				case 1:
					//iDay= Integer.parseInt(dispmsginfo[inIdx]);
					sDay = dispmsginfo[inIdx];
					break;
				case 2:
					//iYear = Integer.parseInt(dispmsginfo[inIdx]);	
					sYear = dispmsginfo[inIdx];
					break;
				case 3:
					//iHour = Integer.parseInt(dispmsginfo[inIdx]);
					sHour = dispmsginfo[inIdx];
					break;	
				case 4:
					//iMinutes = Integer.parseInt(dispmsginfo[inIdx]);
					sMinutes = dispmsginfo[inIdx];
					break;		
				case 5:
					//iSeconds = Integer.parseInt(dispmsginfo[inIdx]);
					sSeconds = dispmsginfo[inIdx];
					break;
			}
		}


	   if(sYear.equals("x")){
		   SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
		   szCurrentYear = sdf.format(new Date());
		   Log.i("saturn", "saturn szCurrentYear "+ szCurrentYear);
		   sYear = szCurrentYear;

	   }

	   	   
	   CtSystem system = new CtSystem(); 

       try{
	   SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");

	   szCompleteDateTime = sYear+sMonth+sDay+sHour+sMinutes+sSeconds;
	   
	   Date date = simpleDateFormat.parse(szCompleteDateTime);
	   
	   Log.d(TAG, "saturn setSystemTime return = " + Integer.toString(system.setSystemTime(date)));
       } catch (ParseException e) {

	   }

       user_str = "time set";
	
	 	return user_str;
	
	}

    public String ScanQRCodeUI(String text) throws InterruptedException {

        String user_str="";
        pass_in_string = text;
        Log.i("ScanQRCodeUI:", "run");
        new Thread()
        {
            public void run()
            {
                try
                {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    intent.setClass(MainActivity.this, ScanQRCode.class);
                    bundle.putString("pass_in_string", pass_in_string);
                    Log.i("pass_in_string", pass_in_string);

                    intent.putExtras(bundle);
                    startActivity(intent);
                    //startActivityForResult(intent, 0);
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
        }.start();

        MainActivity.LOCK.setCondition(false);

        synchronized (LOCK) {
            while (!LOCK.conditionMet()) {
                try {
                    LOCK.wait();
                } catch (InterruptedException e) {
                    Log.e(TAG, "Exception when waiting for condition", e);
                    return "Exception";
                }
            }
        }

        user_str =  ScanQRCode.final_string;

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH ScanQRCodeUI");

        return user_str;
    }

    // print qrcode -- patrick/sidumili
    public String PrintQRCodeUI(String text)
    {
        CtPrint print = new CtPrint();

        String print_font;
        int print_x = 0;
        int print_y = 40;
        int Currently_high = 20;
        int ret = 0;
        Bitmap bitmap = null;
        String user_str="";

		//Bitmap myLogo = BitmapFactory.decodeResource(getResources(), R.drawable.phqrlogotext);


        Log.i(TAG, "PrintQRCodeUI: run");
        Log.i(TAG, "text="+text);

        try
        {
            //print.initPage(240);
            //bitmap = print.encodeToBitmap(text, print.QR_CODE, 240, 240);
            //print.drawImage(bitmap, 80, 0); // center
            
            print.initPage(300);
            bitmap = print.encodeToBitmap(text, print.QR_CODE, 300, 300);
            print.drawImage(bitmap, 40,0); // center
           // print.drawImage(myLogo, 40, 0);
            print.printPage();

            /*
            print.printPage();

            ret = print.roll(10);
            Log.d(TAG, String.format("Roll ret = %d", ret));

            ret = print.status();
            Log.d(TAG, String.format("status ret = %d", ret));

            print.setHeatLevel(2);

            print.initPage(100);

            print_font = "PRINT TESTING";
            print.drawText(0, print_y + Currently_high, print_font, print_y, 1, true, (float) 0, false, false);
            Currently_high += print_y;
            print.printPage();
             */
        }
        catch (Exception e)
        {
            Log.e(TAG, "Exception when waiting for condition", e);
        }

        user_str =  "CONFIRM";

        Log.i("PATRICK", user_str);
        Log.i("PATRICK", "FINISH PrintQRCode");

        return user_str;
    }

    ConnectivityManager.NetworkCallback mGPRSCallback = null;

    public void switchGPRS() {
        Log.d(TAG, "switchGPRS");
        try {
	        final ConnectivityManager connection_manager = (ConnectivityManager) this.getSystemService(CONNECTIVITY_SERVICE);

	        NetworkRequest.Builder request = new NetworkRequest.Builder();
	        request.addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET);
	        request.addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR);

	        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) 
			{
			    if(mGPRSCallback == null)
			    {
			            //Init only once
	        			Log.d(TAG, "mGPRSCallback");
			            mGPRSCallback = new ConnectivityManager.NetworkCallback() {
			                @Override
			                public void onAvailable(final Network network) {
			                    try {
			                        Log.d(TAG, "mGPRSCallback onAvailable");

			                        try {
			                            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
			                                Log.d(TAG, "setProcessDefaultNetwork");
			                                ConnectivityManager.setProcessDefaultNetwork(network);
			                            } else {
			                                Log.d(TAG, "bindProcessToNetwork");
			                                connection_manager.bindProcessToNetwork(network);
			                            }
			                        } catch (IllegalStateException e) {
			                            Log.e(TAG, "ConnectivityManager fail: ", e);
			                        }
			                    } catch (NullPointerException npe) {
			                        npe.printStackTrace();
			                    }
			                }

	                        @Override
	                        public void onUnavailable() {
	                            super.onUnavailable();
	                            // do remove callback
	                            connection_manager.unregisterNetworkCallback(this);
	                        }
			            };
					}
					else
					{
						Log.d(TAG, "mGPRSCallback EXITS");
					}
			
			        connection_manager.requestNetwork(request.build(), mGPRSCallback);
			        Log.d(TAG, "mGPRSCallback requestNetwork");

	        }
        }catch(Exception e){
            e.printStackTrace();
        }
		
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "MCCPAY onDestroy()");
        this.mWakeLock.release();
        super.onDestroy();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
            finishAffinity(); // Kill the app completely
        }

        Log.d(TAG, "finishAffinity()");
        //releaseAll();

        if(receipt_bmp_file != null) {
            receipt_bmp_file = null;        //tine:  clear content
            Log.i("MCCPAY Main", "receipt_bmp_file cleared...");
        }

        if(myBitmap != null) {
            myBitmap.recycle();
            myBitmap = null;                //tine:  clear content
            Log.i("MCCPAY Main", "myBitmap cleared...");
        }
/*
        final_string = null;;
        PinBlock = null;
        KSN = null;

        sPinBlock = null;
        sKSN = null;
        OutputStr = null;
        lsv_LCDTPrint = null;
        adapter = null;
        mPager = null;
        //mDatas.clear();
        mDatas = null;
        //mPagerList.clear();
        mPagerList = null;
        mLlDot = null;
        inflater = null;

 */
    }
    private void releaseAll() {
        Log.i(TAG, "MCCPAY releaseAll: ");

        AppHeaderBuff = null;
        final_string = null;
//        Enter_Press = 0;
//        inRet = 0;
        PinBlock = null;
        KSN = null;

        sPinBlock = null;
        sKSN = null;
        OutputStr = null;

        mContext = null;
        mHandler = null;

        //LOCK = null;
        jni_api = null;

        jni_cb_ctos = null;


        lsv_LCDTPrint = null;
        adapter = null;
        mPager = null;
        //mDatas.clear();
        mDatas = null;
        //mPagerList.clear();GetAmountString
        mPagerList = null;
        mLlDot = null;
        inflater = null;
    }

    public String TLE_MAC(String text) {
        String user_str="hello";
        String szData = "";
        String szKey = "";

        System.out.println("TLE_MAC:"+text);

        String[] parsemsg = text.split("\\|");
        szData=parsemsg[0];
        szKey=parsemsg[1];

        System.out.println("szData:"+szData);
        System.out.println("szKey:"+szKey);

        Log.i("TLE_MAC", "1.encrypted_data:"+szData);
        Encryption mEncryption = new Encryption();
        user_str = mEncryption.TLE_MAC(szData, szKey);
        text="";
        System.out.println("TLE_MAC: user_str:"+user_str);

        return user_str;

    }

    public String TLE_Encrypt(String text) {
        String user_str="hello";
        String szData = "";
        String szKey = "";

        System.out.println("TLE_Encypt:"+text);

        String[] parsemsg = text.split("\\|");
        szData=parsemsg[0];
        szKey=parsemsg[1];

        System.out.println("szData:"+szData);
        System.out.println("szKey:"+szKey);

        //byte[] baEncryptedData = new byte[text.length()];
        Log.i("TLE_Encypt", "1.encrypted_data:"+szData);
        Encryption mEncryption = new Encryption();
        user_str = mEncryption.TLE_Encrypt(szData, szKey);
        text="";
        System.out.println("TLE_Encypt user_str:"+user_str);

        return user_str;
    }

    public String TLE_Decrypt(String text) {
        String user_str="hello";
        String szData = "";
        String szKey = "";

        System.out.println("TLE_Decrypt:"+text);

        String[] parsemsg = text.split("\\|");
        szData=parsemsg[0];
        szKey=parsemsg[1];

        System.out.println("szData:"+szData);
        System.out.println("szKey:"+szKey);

        //byte[] baEncryptedData = new byte[text.length()];
        Log.i("TLE_Encypt", "1.encrypted_data:"+szData);
        Encryption mEncryption = new Encryption();
        user_str = mEncryption.TLE_Decrypt(szData, szKey);
        text="";
        System.out.println("TLE_Decrypt user_str:"+user_str);

        return user_str;
    }




/**
* A native method that is implemented by the 'native-lib' native library,
* which is packaged with this application.
*/
public native int inCTOSS_InitWaveData();
public native int inCTOSS_Sale();
public native int inCTOSS_WAVE_SALE();
public native int inCTOSS_VOID();
public native int inCTOSS_SALE_OFFLINE();
public native int inCTOSS_REFUND();
public native int inCTOSS_TIPADJUST();
public native int inCTOSS_PREAUTH();
public native int inCTOSS_SALECOMPLETE();
public native int inCTOSS_EPP_SALE();
public native int inCTOSS_EPP_VOID();
public native int inCTOSS_SETTLEMENT();
public native int vdCTOSS_DeleteReversal();
public native int inCTOSS_REPRINT_LAST();
public native int inCTOSS_REPRINTF_LAST_SETTLEMENT();
public native int inCTOSS_REPRINT_ANY();
public native int inCTOSS_PRINTF_DETAIL();
public native int inCTOSS_PRINTF_SUMMARY();
public native int inCTOSS_SETTLE_ALL();
public native int inCTOSS_VOIDPREAUTH();
public native int inCTOSS_GetPHQRInit();
public native int inCTOSS_BATCH_TOTAL();
public native int inCTOSS_BATCH_REVIEW();
public native int inCTOSS_PAYCASH();
public native int inCTOSS_SubAPMain();
public native int inCTOSS_MultiAP_CheckMainAPStatus();
public native byte[] inCTOS_GetBuffer();
public native int inCTOSS_CARDENTRYTIMEOUT();
public native String GetAppHeaderDetails();
public native int inCTOSS_Get1stInitFlag();
public native int inCTOSS_GetDLNotFinishedFlag();
public native int inCTOSS_ProcessCfgExpress();
public native int inCTOSS_GetEnvInt(String sztag);
public native int inCTOSS_CreatefsdataFolder();

}

