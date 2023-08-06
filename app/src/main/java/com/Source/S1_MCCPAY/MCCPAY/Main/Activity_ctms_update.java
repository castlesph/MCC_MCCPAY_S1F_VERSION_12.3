package com.Source.S1_MCCPAY.MCCPAY.Main;

import android.app.ActivityManager;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.os.BatteryManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.MccpayApplication;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;

import org.json.JSONObject;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.List;

import CTOS.CtCtms;
import CTOS.CtSystem;
import castles.ctms.module.commonbusiness.DownloadInfo;
import castles.ctms.module.commonbusiness.IStatusCallback;
import castles.ctms.module.commonbusiness.PackageInfo;

public class Activity_ctms_update extends DemoAppActivity {
    private static final String TAG = "Activity_ctms_update";
    private CtCtms ctCtms;
    private int installNum = 0;
    private int totalNum = 0;
    private TextView tvStatus;
    private ProgressBar progressBar;
    private boolean downloaded = false;
    private String currentDownloadFileName;
    public String strPreDownloadfileName = "";
    CtSystem ctSystem = new CtSystem();
    private String strSn = null;
    final Handler handler = new Handler();
    private final boolean CreatedCtmsObj = true;
    byte bootupConnectFlag = 0;
    private boolean bootflag = false;//This flag is used for resume download, eg: if got 2 app, one app downloaded, terminal restart, after restart terminal should auto do ctms call
    private boolean isRegisteredCallback = false;
    private boolean backUpDbFlag = false;

    private String HostIP = null;
    private int HostPort = 0;
    private String strTlsHostIP = null;
    private int TlsHostPort = 0;
    private int strMode = 0;
    private boolean fGotFileDownloadFailed = false;//20210127 for CTMS
    private int ConnectFailedTimes = 0;
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

    //for bind service?
/*
    private ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.i(TAG, "onServiceConnected: " + name);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.i(TAG, "onServiceDisconnected: " + name);
        }
    };
*/

    private void vdServiceList(final String className) {
        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningServiceInfo> info = activityManager.getRunningServices(Integer.MAX_VALUE);
        if (info == null || info.size() == 0) {
            Log.i(TAG, "no service: ");
            return ;
        }
        for (ActivityManager.RunningServiceInfo aInfo : info) {
            Log.i(TAG, "aInfo.service" + aInfo.service);
            if (className.equals(aInfo.service.getClassName())) {
                Log.i(TAG, "found: found");
//                return true;
            }
        }
//        return false;
    }
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        Log.i(TAG, "onCreate: ");
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();
        setContentView(R.layout.activity_ctms_update);
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_VISIBLE);

        tvStatus = (TextView) findViewById(R.id.tv_ctms_msg);
        progressBar = (ProgressBar) findViewById(R.id.downloadStatusPb);
        progressBar.setVisibility(View.GONE);

        Intent intent = getIntent();
        if(intent != null) {
            //20210127 for CTMS, can remove this flag in the 2nd phase, because we don't need to do Resume download by app level, can run it background
//            bootflag = intent.getBooleanExtra("REBOOT", false);
            Log.i(TAG, "bootflag: " + bootflag);
        }

        String dispmsg = intent.getStringExtra("pass_in_string");
        String[] dispmsginfo = dispmsg.split("\\|");
        int msgcnt = dispmsginfo.length;

        for (int inIdx = 0; inIdx < msgcnt; inIdx++) {
            //System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx) {
                case 0:
                    strSn = dispmsginfo[inIdx];
                    break;
                case 1:
                    //TCP
                    HostIP = dispmsginfo[inIdx];
                    break;
                case 2:
                    //TCP
                    HostPort = Integer.parseInt(dispmsginfo[inIdx]);
                    break;
                case 3:
                    //TLS
                    strTlsHostIP = dispmsginfo[inIdx];
                    break;
                case 4:
                    //TLS
                    TlsHostPort = Integer.parseInt(dispmsginfo[inIdx]);
                    break;
                case 5:
                    strMode = Integer.parseInt(dispmsginfo[inIdx]);
                    break;

            }
        }

        if(CreatedCtmsObj) {
            ctCtms = MccpayApplication.getCtCtmsObj();
            Log.i(TAG, "CreatedCtmsObj: " + (ctCtms == null));
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    Log.i(TAG, "run: registerCallback");
                    ctCtms.registerCallback(statusCalback);
                    isRegisteredCallback = true;
                }
            }, 1000);
        }
        else {

            ctCtms = new CtCtms(this);
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    Log.i(TAG, "run: registerCallback");
                    ctCtms.registerCallback(statusCalback);
                    isRegisteredCallback = true;
                }
            }, 3000);
        }

/*
        try {
            strSn = ctSystem.getFactorySNEx();
            Log.i(TAG, "getFactorySNEx: " + strSn);
        } catch (CtSystemException e) {
            e.printStackTrace();
            Log.e(TAG, "CtSystemException: " + e );
        }

 */
        //TCP
//        String HostIP = "staging-pos.castechcentre.com";
        //String HostIP = "pos.uat.castechcentre.com";
//        int HostPort = 80;
//        String strTlsHostIP = "staging.castechcentre.com";
//        String strTlsHostIP = "ctms.uat.castechcentre.com";
//        int TlsHostPort = 443;
        byte ctmsFlag = 1;
        Log.i(TAG, "getCTMSStatus: " + ctCtms.getCTMSStatus());
        ctCtms.setCTMSEnable(ctmsFlag);
        //20210127 for CTMS
        //install mode can set it here or can set it from CTMS server with CTMS agent CSV file(rebootInstallMode), must use flag install mode
        int RebootInstallMode = ctCtms.getRebootInstallMode();//0 - reboot install if got file(MUST not use this setting), 1- only getRebootInstallFlag is true, then install file,after install done, agent set setRebootInstallFlag to false
        boolean RebootInstallAllow = ctCtms.getRebootInstallFlag();
        Log.i(TAG, "getRebootInstallMode: " + RebootInstallMode);
        Log.i(TAG, "getRebootInstallFlag: " + RebootInstallAllow);
        ctCtms.setRebootInstallMode(1);//0 force reboot install, means very reboot just install if any, 1 flag to control reboot install, only getRebootInstallFlag to true, then install during boot up
        //set all config with Json format
        {
            String strOriJsonConfig;
            JSONObject jsonObject;
            try {
//                                String strTCPConfig = ctCtms.getConfig(CtCtms.CONFIG_COMMUNICATE_TCP);
//                                Log.i(TAG, "strTCPConfig: " + strTCPConfig);

                String str = ctCtms.getAllConfig();
                Log.i(TAG, "getAllConfig: " + str);
                jsonObject = new JSONObject(str);
                Log.i(TAG, "get all config: " + jsonObject.toString());
                jsonObject.put("Serial_Number", strSn);
                //20210127 for CTMS, use setBootConnectEnable set boot connect, with new Fw, now we can turn on boot up connect if u need, because app can control the install part
                Log.i(TAG, "ctCtms.getBootConnectStatus(): " + ctCtms.getBootConnectStatus());
//                ctCtms.setBootConnectEnable(bootupConnectFlag);
//                Log.i(TAG, "ctCtms.getBootConnectStatus(): " + ctCtms.getBootConnectStatus());//setBootConnectEnable
                //TCP config
                JSONObject jsonObjTCP = jsonObject.getJSONObject("TCP");
                Log.i(TAG, "get jsonObjTCP config: " + jsonObjTCP.toString());
                jsonObjTCP.put("Host_IP", HostIP);
                jsonObjTCP.put("Host_Port", HostPort);

                //TLS config
                JSONObject jsonObjTLS = jsonObject.getJSONObject("TLS");
                Log.i(TAG, "get jsonObjTLS config: " + jsonObjTLS.toString());
                jsonObjTLS.put("Host_IP", strTlsHostIP);
                jsonObjTLS.put("Host_Port", TlsHostPort);

                Log.i(TAG, "set all config: " + jsonObject.toString());
                ctCtms.setAllConfig(jsonObject.toString());

            } catch (Exception e) {
                Log.e(TAG, "Exception: " + e);
                e.printStackTrace();
            }

            new Thread(new Runnable() {
                @Override
                public void run() {
                    Log.i(TAG, "run TMS update: ");
                    UpdateNow(ctCtms);
                }
            }).start();

        }
    }

    @Override
    public void onBackPressed() {
        Log.i(TAG, "onBackPressed: ");
        super.onBackPressed();
        finish();
    }

    @Override
    protected void onDestroy() {
        Log.i(TAG, "onDestroy: " + downloaded);
        if(isRegisteredCallback)
            ctCtms.unregisterCallback(statusCalback);//if not added, show ServiceConnectionLeaked
//        ctCtms.unbindService(Activitsy_ctms_update.this);
        if (!downloaded) {
            Log.i(TAG, "NO UPDATES: ");
            vdUpdateDownloadStatus("NO UPDATES");//no use, other activity already in front
        }
        super.onDestroy();
        Intent intent = new Intent();
        Bundle bundle = new Bundle();
        intent.setClass(Activity_ctms_update.this, MainActivity.class);
        startActivity(intent);
        this.finish();

    }

    private String UpdateNow(CtCtms ctCtms) {
        Log.i(TAG, "UpdateNow: ");
        String strConnectTime = null;
        int status = 0;
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd-HH:mm");
        Calendar connectTime = ctCtms.getConnectTime();
        Calendar triggerTime = ctCtms.getTrigger();
        Log.i(TAG, " when the terminal connects to the server, getConnectTime: " + connectTime.getTime());
        Log.i(TAG, "Getting the trigger time setting, triggerTime: " + triggerTime.getTime());
        Calendar activeTime = ctCtms.getActiveTime();
        Log.i(TAG, "Getting the active time setting, activeTime: " + activeTime.getTime());
/*        ctCtms.setActiveMode(CtCtms.OPTION_MANUAL_CONTROL);
        if(status == 0){
            Log.d(TAG, "setActiveMode ok: ");
        }
        Log.i(TAG, "getDebugStatus: " + ctCtms.getDebugStatus() + " getBootConnectStatus:" + ctCtms.getBootConnectStatus());

        status = ctCtms.getTriggerStatus();
        if(status == 0){
            Log.d(TAG, "disabled ");
        } else if(status == 1) {
            Log.d(TAG, "Enable ");
        } else {
            Log.w(TAG, "failure: ");
        }

        ctCtms.setTriggerEnable(CtCtms.OPTION_DISABLE);//CTMS will follow trigger time to connect to CTMS serve*/

        Log.i(TAG, "call UpdateImmediately: ");
        status = ctCtms.UpdateImmediately();
        Log.i(TAG, "after UpdateImmediately status: " + status);
        if(status == 0) {
            strConnectTime = dateFormat.format(connectTime.getTime());
        } else {
            strConnectTime = "Connect faileds";
        }
        Log.i(TAG, "strConnectTime: " + strConnectTime);
        return strConnectTime;
    }

    private void vdUpdateDownloadStatus (final String msg) {
        Log.i(TAG, "vdUpdateDownloadStatus: " + msg);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tvStatus.setText(msg);
            }
        });
    }
    private void vdUpdateDownloadProcessBar (final int progress) {
        Log.i(TAG, "vdUpdateDownloadProcessBar: " + progress);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                progressBar.setVisibility(View.VISIBLE);
                progressBar.setProgress(progress);
            }
        });
    }

    private void vdRebootNow() {
        Log.i(TAG, "vdRebootNow: ");
        CtSystem system = new CtSystem();
        byte bmode = 0;// shutdown
        vdUpdateDownloadStatus("Rebooting...");
        try {
            Log.d("PATRICK", String.format("return = %X", system.shutdown(bmode)));
        } catch (Exception e) {
            Log.d("Castles", "SHUTDOWN encountered ERROR");
            e.printStackTrace();
        }
    }
    IStatusCallback statusCalback = new IStatusCallback() {
//    IAgentCallback statusCalback = new IAgentCallback.Stub() {

        @Override
        public void readyCallback() {
            Log.i("CTMS-cb", "readyCallback: Callback when CTMS process is finish, got download:" + downloaded);
            Log.i(TAG, "totalNum downloaded file: " + totalNum);
//            inCTOSS_SetDownloadedNotFinishedFlag(0);//20210127 for CTMS
            if(downloaded || bootflag)
            {
                Log.i(TAG, "readyCallback: if Dl multi app, then this will be called multi times");
                Log.i(TAG, ".myPid(): " + android.os.Process.myPid());
                //20210127 for CTMS start
                Log.i(TAG, "fGotFileDownloadFailed: " + fGotFileDownloadFailed);
                BatteryManager batteryManager=(BatteryManager)getSystemService(BATTERY_SERVICE);
                int batteryRemain = 0;
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    batteryRemain = batteryManager.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY);
                    Log.i(TAG, "BATTERY_PROPERTY_CAPACITY: " + batteryRemain) ;
                    if(batteryRemain > 35) {
                        //Do not allow to do installation
                        //Fw + software
                        Log.i(TAG, "getRebootInstallFlag: " + ctCtms.getRebootInstallFlag());
                        if(!fGotFileDownloadFailed)
                            ctCtms.setRebootInstallFlag(true);
                    } else {
                        //TODO: we will not set allow to install flag here, and pls show some msg based on your project
                        //then only next CTMS call will try to install the new software and when battery is higher than 35%
                        Log.w(TAG, "Please charge your terminal, will not set install flag to true");
                        vdUpdateDownloadStatus("Please charge your terminal above 30%, and install new software");
                        Activity_ctms_update.this.finish();
                        handler.postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                Log.i(TAG, "run: vdRebootNow();");
                                vdRebootNow();
                            }
                        }, 4000);
                        Log.i(TAG, "reboot after delay 1: ");
                        return;
                    }
                }
                //20210127 for CTMS end
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        Log.i(TAG, "run: vdRebootNow();");
                        vdRebootNow();
                    }
                }, 4000);
                Log.i(TAG, "reboot after delay: ");
            }

            Activity_ctms_update.this.finish();
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    Log.i(TAG, "run: vdRebootNow();");
                    vdRebootNow();
                }
            }, 4000);
            Log.i(TAG, "reboot after delay 2: ");
        }

        @Override
        public void connectCallback(int i) {
            Log.i("CTMS-cb " + i,  "connectCallback: callback when connect to server:");
            vdUpdateDownloadStatus("Connecting...");
            //20210127 for CTMS
            if(i != 0) {
                ConnectFailedTimes ++ ;
                Log.e(TAG, "connectCallback failed: " + ConnectFailedTimes);
                vdUpdateDownloadStatus("Connect failed..." + ConnectFailedTimes);
                if(ConnectFailedTimes > 3) {
                    Activity_ctms_update.this.finish();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            Log.i(TAG, "run: vdRebootNow();");
                            vdRebootNow();
                        }
                    }, 4000);
                    Log.i(TAG, "reboot after delay 3: ");
                    return;
                }
            }
        }

        @Override
        public void getTerminalInfoCallback(int i)  {
            Log.i("CTMS-cb " + i, "getTerminalInfoCallback: callback when upload app and FW infor:");
            vdUpdateDownloadStatus("Upload app and FW infor...");
        }

        @Override
        public void downloadCallback(int i, DownloadInfo downloadInfo) {
            Log.i("CTMS-cb " + i, "Download fileName: " + downloadInfo.fileName + "  fullSize:" + downloadInfo.fullSize + "   downloadSize=" + downloadInfo.downloadSize + "  totalDownloadNumber:" + downloadInfo.totalDownloadNumber+ " downloadIndex=" + downloadInfo.downloadIndex);
            //20210127 for CTMS
            if((i != 0) && (i != 4096)) {
                //Download not succ, cannot set install flag to true when goes readyCallback
                    fGotFileDownloadFailed = true;
                Log.e("CTMS-cb-DL ", "File DL failed: " + downloadInfo.fileName);
            }
            if (downloadInfo.downloadSize > 0) {
                Log.i(TAG, "downloadSize >0:" + " Pre-DownloadfileName:" + strPreDownloadfileName);
                if(!downloadInfo.fileName.equals("UpdateList")) {
                    if(downloadInfo.downloadSize >= downloadInfo.fullSize)
                    {
                        if(!strPreDownloadfileName.equals(downloadInfo.fileName))
                        {
                            strPreDownloadfileName = "";
                            strPreDownloadfileName = downloadInfo.fileName;
                            totalNum++;
                        }

                        Log.i(TAG, "finish download one file, total file num increase to: " + totalNum);
                    }

                    downloaded = true;
                    //back up data based on the .tbd file
                    if(!backUpDbFlag) {
                        Log.i(TAG, "back db file: ");
                        inCTOSS_BackupDataScript();
                        backUpDbFlag = true;
                    }

                    //TODO: need to set back later
                    //set downloaded flag to true, so when boot up will do auto update again if download not finished yet
//                    inCTOSS_SetDownloadedNotFinishedFlag(1);//20210127 for CTMS

                    int progress = (int) ((((float) downloadInfo.downloadSize) / ((float) downloadInfo.fullSize)) * 100);

                    Log.i(TAG, "progress: " + progress);
                    String s= String.valueOf(progress) + "%";

                    String filesize;
                    double size;
                    StringBuffer bytes = new StringBuffer();
                    DecimalFormat format = new DecimalFormat("###.0");
                    if(downloadInfo.fullSize > (1024*1024)) {
                        size = (downloadInfo.fullSize / (1024.0 * 1024.0));
                        bytes.append(format.format(size)).append("MB");
                    } else if(downloadInfo.fullSize > (1024))
                    {
                        size = (downloadInfo.fullSize / (1024.0));
                        bytes.append(format.format(size)).append("KB");

                    } else {
                        bytes.append((int) downloadInfo.fullSize).append("B");
                    }
                    filesize = bytes.toString();
                    vdUpdateDownloadStatus("Downloading...\n" + downloadInfo.fileName + "\n"+ filesize + "\n" + s);
                    vdUpdateDownloadProcessBar(progress);
                    Log.d(TAG, "got update: ");
                }

            } else {
                Log.i(TAG, "No update from CTMS server: ");
                //CtSystem system = new CtSystem();
                //byte bmode = 0;// reboots
                //Log.d("PATRICK", String.format("return = %X", system.shutdown(bmode)));
            }
        }

        @Override
        public void diagnosticCallback(int i) {
            Log.i("CTMS-cb " + i, "diagnosticCallback: callback when run diag");
            vdUpdateDownloadStatus("Diagnostic...");
        }

        @Override
        public void installCallback(int i, PackageInfo packageInfo) {
            Log.i("CTMS-cb " + i, "installCallback: " + packageInfo.fileName);
            if(i == 4096)//4096 start, 0 end
                installNum++;
            String strInstallNum = "[install " + installNum + "/" + totalNum + "]\n";
            vdUpdateDownloadStatus("Installing...\n" + strInstallNum +  packageInfo.fileName);
        }

        @Override
        public void freeTypeDownloadCallback(int i, DownloadInfo downloadInfo) {

        }

        @Override
        public void freeTypeInstallCallback(int i, PackageInfo packageInfo) {

        }
    };

    public native int inCTOSS_BackupDataScript();
    //20210127 for CTMS, can remove this function in the 2nd release, CTMS can auto do resume download, and app only control when need to install process
//    public native int inCTOSS_SetDownloadedNotFinishedFlag(int value);
}
