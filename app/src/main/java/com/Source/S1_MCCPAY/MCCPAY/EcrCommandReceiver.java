package com.Source.S1_MCCPAY.MCCPAY;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.util.Log;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.utility.GetMemoryInfor;

public class EcrCommandReceiver extends BroadcastReceiver {
    private static final String ACTION = "com.persistent.app.RECEIVER";

    static {
        System.loadLibrary("crypto");
    }

    static {
        System.loadLibrary("ssl");
    }

    static {
        System.loadLibrary("native-lib");
    }

    private static final String TAG = "EcrCommandReceiver";
    static final String ECR_APPLICATION_ID = "com.persistent.app";
    private Context mContext = null;
    private boolean isEcrProcessing = false;
    private static boolean isSetInstance = false;
    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i(TAG, "1.onReceive: " + isSetInstance);
        String str;
        String strPaymentType;
        mContext = context;
//        GetMemoryInfor.displayBriefMemory(mContext);
        if(!isSetInstance) {
            vdEcrSetupInstance();
            isSetInstance = true;
        }

        Log.i(TAG, "2.onReceive: " + com.Source.S1_MCC.MCC.EcrCommandDefintion.ECR_REQ_CMD_NAME);
		Log.i(TAG, "3.onReceive: " + com.Source.S1_MCC.MCC.EcrCommandDefintion.ECR_REQ_PAYMENT_TYPE);
		
        //GetMemoryInfor.displayBriefMemory(mContext);
        if (intent.hasExtra(com.Source.S1_MCC.MCC.EcrCommandDefintion.ECR_REQ_CMD_NAME)) {
            str = intent.getStringExtra(com.Source.S1_MCC.MCC.EcrCommandDefintion.ECR_REQ_CMD_NAME);
            strPaymentType = intent.getStringExtra(com.Source.S1_MCC.MCC.EcrCommandDefintion.ECR_REQ_PAYMENT_TYPE);
			Log.i(TAG, "4.onReceive: " + " ECR CMD: " + " PaymentType: ");
            Log.i(TAG, "5.onReceive: " + " ECR CMD: " + str + " PaymentType: " + strPaymentType);
            if(str != null) {
//                if(str.equals(EcrCommandDefintion.ECR_REQ_CMD_SALE))
                {

                    if(isEcrProcessing){
                        Log.i(TAG, "onReceive: busy now ignore??");
                        return;
                    }

                    Log.i(TAG, "start to process Ecr sale");
                    //Step 1: Call main app start to do ECR txn
//                    if(MainActivity.getInstace()!=null)
                    callActivityDoTxn(context, str);
                    isEcrProcessing = true;
                    //step 2: when main app finish txn, call share ecr app and send back ecr response
                    //ecr app received msg via BroadcastReceiver ???
//                    sendBackEcrResponse(" ");

                }
            }
        }
		else
			Log.i(TAG, "intent.hasExtra(EcrCommandDefintion.ECR_REQ_CMD_NAME)");
    }

    private void callActivityDoTxn(Context context, String txnType) {
        Log.i(TAG, "callActivityDoTxn: " + txnType + "  " + context.getPackageName());
//        GetMemoryInfor.displayBriefMemory(mContext);
        Intent intent = new Intent();
        intent.setClass(context, MainActivity.class);
        intent.putExtra(com.Source.S1_MCC.MCC.EcrCommandDefintion.ECR_REQ_CMD_NAME, txnType);
//        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
//        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//        GetMemoryInfor.displayBriefMemory(mContext);
        context.startActivity(intent);
//        GetMemoryInfor.displayBriefMemory(mContext);
        Log.i(TAG, "callActivityDoTxn:  done");
    }

    public void sendBackEcrResponse(String str) {
        Log.i(TAG, "sendBackEcrResponse: " + str);
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
//            intent.setComponent(new ComponentName(ECR_APPLICATION_ID, ECR_APPLICATION_ID+".RECEIVER"));
        }
        intent.putExtra("ECR_RESP", "SUCC");
        mContext.sendBroadcast(intent);
        isEcrProcessing = false;
        Log.i(TAG, "sendBackEcrResponse: done");
    }

    public native int vdEcrSetupInstance();
}
