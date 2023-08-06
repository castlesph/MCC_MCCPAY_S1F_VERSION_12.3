package com.Source.S1_MCCPAY.MCCPAY.Kms;

import android.app.Activity;
import android.binder.aidl.IKMS2Callback;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;
import java.io.File;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;

import CTOS.CtKMS2CustomPINPadbyImage;
import CTOS.CtKMS2Dukpt;
import CTOS.CtKMS2System;

public class
PINPADDUKPT extends Thread
{
    private static final String TAG = "PINPADDUKPT";

    public Activity activity;
    public int inKeySet;
    public int inKeyIndex;
    public String StrInData;
    public int inPinBypassAllow;
    private TextView PinDigitTV;
    private EditText PinDigitET;
    private EditText FunctionKeyET;


    private Handler background_ON_Handler;

    private Handler background_OFF_Handler;

    private Runnable background_on = new Runnable () {
        public void run() {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    PinDigitTV.setEnabled(false);
                    PinDigitTV.setAlpha(0.0f);
                    PinDigitET.setEnabled(false);
                    PinDigitET.setAlpha(0.0f);
                    FunctionKeyET.setAlpha(0.0f);
                    FunctionKeyET.setEnabled(false);
                    FunctionKeyET.setAlpha(0.0f);
                }
            });
        }
    };

    private Runnable background_off = new Runnable () {
        public void run() {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    PinDigitTV.setEnabled(true);
                    PinDigitTV.setAlpha(1.0f);
                    PinDigitET.setEnabled(true);
                    PinDigitET.setText("");
                    PinDigitET.setAlpha(1.0f);
                    FunctionKeyET.setEnabled(true);
                    FunctionKeyET.setText("");
                    FunctionKeyET.setAlpha(1.0f);
                }
            });
        }
    };

    public PINPADDUKPT(Activity activity, int inKeySet, int inKeyIndex, String StrInData, int inPinBypassAllow)
    {
        this.activity = activity;
        this.inKeySet = inKeySet;
        this.inKeyIndex = inKeyIndex;
        this.StrInData = StrInData;
        this.inPinBypassAllow = inPinBypassAllow;
        PinDigitTV = (TextView) activity.findViewById(R.id.PinDigitTextView);
        PinDigitET = (EditText) activity.findViewById(R.id.PinDigitEditText);
        FunctionKeyET = (EditText) activity.findViewById(R.id.FunctionkeyText);
        HandlerThread background_ON_Thread = new HandlerThread("background_on");
        background_ON_Thread.start();
        background_ON_Handler = new Handler (background_ON_Thread.getLooper());
        HandlerThread background_OFF_Thread = new HandlerThread("background_off");
        background_OFF_Thread.start();
        background_OFF_Handler = new Handler (background_OFF_Thread.getLooper());
    }
    @Override
    public void run()
    {
        IKMS2Callback.Stub callback = new IKMS2Callback.Stub(){
            StringBuffer sb = new StringBuffer();
            byte recv;
            @Override
            public int testCancel(){
                return 0;
            }
            @Override
            public void onGetDigit(byte Digit) {
                Log.d(TAG, String.format("NoDigits = %d, OnGetPINDigit.", Digit));
                recv = Digit;
                sb.setLength(0);
                for(int i = 0;i < recv;i ++)
                    sb.append("*");
                activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        PinDigitET.setText(sb);
                    }
                });
            }

            @Override
            public void onGetFunctionKey(byte FunctionKey) {
                Log.d(TAG, "OnGetPINOtherKeys");
                if(FunctionKey=='A')
                {
                    Log.d(TAG, "Enter Key");
                    if(sb.length()!=0)
                    {
                        MainActivity.Enter_Press = 1;
                    }
                    Log.d(TAG, MainActivity.Enter_Press+"");
                }
                Log.d(TAG, String.format("FunctionKey = %d, OnGetPINOtherKeys.", FunctionKey));
                recv = FunctionKey;
                activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        // FunctionKeyET.setText(String.format("FunctionKey = %d",recv));
                    }
                });
            }};

        String rtn ="";
        CtKMS2CustomPINPadbyImage setvirtualpinpad_para = new CtKMS2CustomPINPadbyImage();
        setvirtualpinpad_para.VKBD_0.x = 50;
        setvirtualpinpad_para.VKBD_0.y = 545;
        setvirtualpinpad_para.VKBD_0.width = 140;
        setvirtualpinpad_para.VKBD_0.height = 140;
        setvirtualpinpad_para.VKBD_0.filepath = new File("/data/data/pub/0.bmp");
        setvirtualpinpad_para.VKBD_1.x = 210;
        setvirtualpinpad_para.VKBD_1.y = 545;
        setvirtualpinpad_para.VKBD_1.width = 140;
        setvirtualpinpad_para.VKBD_1.height = 140;
        setvirtualpinpad_para.VKBD_1.filepath = new File("/data/data/pub/1.bmp");
        setvirtualpinpad_para.VKBD_2.x = 370;
        setvirtualpinpad_para.VKBD_2.y = 545;
        setvirtualpinpad_para.VKBD_2.width = 140;
        setvirtualpinpad_para.VKBD_2.height = 140;
        setvirtualpinpad_para.VKBD_2.filepath = new File("/data/data/pub/2.bmp");
        setvirtualpinpad_para.VKBD_3.x = 50;
        setvirtualpinpad_para.VKBD_3.y = 695;
        setvirtualpinpad_para.VKBD_3.width = 140;
        setvirtualpinpad_para.VKBD_3.height = 140;
        setvirtualpinpad_para.VKBD_3.filepath = new File("/data/data/pub/3.bmp");
        setvirtualpinpad_para.VKBD_4.x = 210;
        setvirtualpinpad_para.VKBD_4.y = 695;
        setvirtualpinpad_para.VKBD_4.width = 140;
        setvirtualpinpad_para.VKBD_4.height = 140;
        setvirtualpinpad_para.VKBD_4.filepath = new File("/data/data/pub/4.bmp");
        setvirtualpinpad_para.VKBD_5.x = 370;
        setvirtualpinpad_para.VKBD_5.y = 695;
        setvirtualpinpad_para.VKBD_5.width = 140;
        setvirtualpinpad_para.VKBD_5.height = 140;
        setvirtualpinpad_para.VKBD_5.filepath = new File("/data/data/pub/5.bmp");
        setvirtualpinpad_para.VKBD_6.x = 50;
        setvirtualpinpad_para.VKBD_6.y = 845;
        setvirtualpinpad_para.VKBD_6.width = 140;
        setvirtualpinpad_para.VKBD_6.height = 140;
        setvirtualpinpad_para.VKBD_6.filepath = new File("/data/data/pub/6.bmp");
        setvirtualpinpad_para.VKBD_7.x = 210;
        setvirtualpinpad_para.VKBD_7.y = 845;
        setvirtualpinpad_para.VKBD_7.width = 140;
        setvirtualpinpad_para.VKBD_7.height = 140;
        setvirtualpinpad_para.VKBD_7.filepath = new File("/data/data/pub/7.bmp");
        setvirtualpinpad_para.VKBD_8.x = 370;
        setvirtualpinpad_para.VKBD_8.y = 845;
        setvirtualpinpad_para.VKBD_8.width = 140;
        setvirtualpinpad_para.VKBD_8.height = 140;
        setvirtualpinpad_para.VKBD_8.filepath = new File("/data/data/pub/8.bmp");
        setvirtualpinpad_para.VKBD_9.x = 210;
        setvirtualpinpad_para.VKBD_9.y = 995;
        setvirtualpinpad_para.VKBD_9.width = 140;
        setvirtualpinpad_para.VKBD_9.height = 140;
        setvirtualpinpad_para.VKBD_9.filepath = new File("/data/data/pub/9.bmp");
        setvirtualpinpad_para.VKBD_CLEAR.x = 530;
        setvirtualpinpad_para.VKBD_CLEAR.y = 695;
        setvirtualpinpad_para.VKBD_CLEAR.width = 140;
        setvirtualpinpad_para.VKBD_CLEAR.height = 140;
        setvirtualpinpad_para.VKBD_CLEAR.filepath = new File("/data/data/pub/backspace.bmp");
        setvirtualpinpad_para.VKBD_CANCEL.x = 530;
        setvirtualpinpad_para.VKBD_CANCEL.y = 845;
        setvirtualpinpad_para.VKBD_CANCEL.width = 140;
        setvirtualpinpad_para.VKBD_CANCEL.height = 140;
        setvirtualpinpad_para.VKBD_CANCEL.filepath = new File("/data/data/pub/cancel.bmp");
        setvirtualpinpad_para.VKBD_ENTER.x = 530;
        setvirtualpinpad_para.VKBD_ENTER.y = 545;
        setvirtualpinpad_para.VKBD_ENTER.width = 140;
        setvirtualpinpad_para.VKBD_ENTER.height = 140;
        setvirtualpinpad_para.VKBD_ENTER.filepath = new File("/data/data/pub/enter.bmp");
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY1.x = 50;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY1.y = 445;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY1.width = 140;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY1.height = 90;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY1.value = 'S';
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY1.filepath = new File("/data/data/pub/f1.bmp");
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY2.x = 210;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY2.y = 445;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY2.width = 140;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY2.height = 90;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY2.value = 'S';
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY2.filepath = new File("/data/data/pub/f2.bmp");
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY3.x = 370;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY3.y = 445;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY3.width = 140;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY3.height = 90;
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY3.value = 'S';
        setvirtualpinpad_para.VKBD_EX_FUNCTIONKEY3.filepath = new File("/data/data/pub/f3.bmp");
        setvirtualpinpad_para.setMovingRange(0,0,720,1100,20,20);
        byte outblocklen = 8;
        int keySet =inKeySet;
        int keyIndex = inKeyIndex;
        int PinBypassAllow = inPinBypassAllow;
        byte[] PAN_number = StrInData.getBytes();
        int PAN_offset = 0;
        byte Null_PIN = 0;
        int Timeout = 64;
        int First_timeout = 0;
        int MaxDigit = 6,MinDigit = 4;

        if(PinBypassAllow==1)
            Null_PIN = 1;

        try{
            CtKMS2System system = new CtKMS2System();
            system.setPINPADMoving(false);
            system.setPINScrambling(true);
            system.setPINSound(true);
            CtKMS2Dukpt dukpt = new CtKMS2Dukpt();
            Log.d(TAG, String.format("keySet = %d", keySet));
            Log.d(TAG, String.format("keySet = %d", keyIndex));
            Log.d(TAG, String.format("PAN_number.length = %d", PAN_number.length));

            dukpt.selectKey(keySet,keyIndex);
            dukpt.setCipherMethod(dukpt.PIN_CIPHER_METHOD_ECB);
            dukpt.setPinInfo(dukpt.PIN_BLOCKTYPE_ANSI_X9_8_ISO_0,(byte)MaxDigit,(byte)MinDigit,outblocklen);
            dukpt.setCallback(callback);
            dukpt.setInputData(PAN_number,0,PAN_number.length);
            dukpt.setPinControl(Null_PIN, Timeout, First_timeout);
            dukpt.isUseCurrentKey(false);
			background_OFF_Handler.post(background_off);
            dukpt.startVirtualPin(activity, setvirtualpinpad_para);
            String out = (Convert.ByteArrayTohexString(dukpt.getOutpuData()));
            Log.d(TAG, "OutpuData:" + out); 

            background_ON_Handler.post(background_on);
            String ksn = (Convert.ByteArrayTohexString(dukpt.getKSN()));
            Log.d(TAG, "KSN:" + ksn);
            MainActivity.sPinBlock=out;
            MainActivity.sKSN = ksn;
            MainActivity.inRet = 0;
            MainActivity.Enter_Press = 1;
        } catch (CTOS.CtKMS2Exception e) {
            MainActivity.Enter_Press = 1;
            MainActivity.inRet = 1;
            rtn = Integer.toHexString(e.getError());
            MainActivity.sKSN = rtn;
            Log.d(TAG, "rtn =" + rtn);
            background_ON_Handler.post(background_on);
        }
    }
}
