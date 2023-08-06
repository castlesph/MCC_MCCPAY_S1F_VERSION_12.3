package com.Source.S1_MCCPAY.MCCPAY.Main;


import android.app.ActivityManager;
import android.support.constraint.ConstraintLayout;
import android.util.Log;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.model.Convert;
import com.Source.S1_MCCPAY.MCCPAY.model.VKeyPad;


import java.util.Arrays;
import java.util.List;


public class JNICB_Lib {
    public JNICB_Lib() {
    }

    public interface  cb_CTOS {
        long CTOS_LCDTPrintXY(short usX, short usY, String pbBuf);
        long CTOS_LCDTPrintAligned(short usY, String pbBuf, byte bMode);
        long CTOS_LCDTClearDisplay();
	 long CTOS_bring2Front();
    }

    public interface  cb_IPT {
        long ShowVirtualKeyPad(byte[] pVirtualKeyPadPara);
        long ShowKeyPadMsg(byte digitsNum, final byte pinType, final byte remainingCounter);
        long GetKeyPadDone();
    }
}

class JNICB_implements_CTOS implements JNICB_Lib.cb_CTOS
{

    private MainActivity MainContext;


    public JNICB_implements_CTOS(MainActivity InContext)
    {
        MainContext = InContext;
    }

	//test android

    public long CTOS_LCDTPrintXY(short usX, short usY, String pbBuf)
    {
        Log.d("CTOS_LCDTPrintXY","CTOS_LCDTPrintXY: " + pbBuf);

        final String msg = pbBuf;
        final short y = usY;
        MainContext.displayMode[y-1] = "4";
        MainContext.addPadding[y-1] = usX-1;

        MainContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if(y > 0 && y <= 16)
                {
                    MainContext.list[y-1] = msg;
                    MainContext.adapter.notifyDataSetChanged();
                    MainContext.lsv_LCDTPrint.setAdapter(MainContext.adapter);
                }
                else
                {
                    MainContext.edtLog.append(msg + '\n');
                }
            }
        });
        return 0;
    }

//    @Override
    public long CTOS_LCDTPrintAligned(short usY, String pbBuf, byte bMode) {
        Log.d("CTOS_LCDTPrintAligned","CTOS_LCDTPrintAligned: " + pbBuf);

        final String msg = pbBuf;
        final short y = usY;
        MainContext.displayMode[y-1] = Byte.toString(bMode);
        MainContext.addPadding[y-1] = 0;

        Log.d("DISPMODE", MainContext.displayMode[y-1]);



        MainContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {

                String align = null;
                if(y > 0 && y <= 16)
                {
                    MainContext.list[y-1] = msg;
                    MainContext.adapter.notifyDataSetChanged();
                    MainContext.lsv_LCDTPrint.setAdapter(MainContext.adapter);
                }
                else
                {
                    MainContext.edtLog.append(msg + '\n');
                }
            }
        });
        return 0;
    }

    public long CTOS_LCDTClearDisplay()
    {
        MainContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                for(int i = 0; i < 16; i++)
                {
                    MainContext.list[i] = "";
                }
                MainContext.adapter.notifyDataSetChanged();
                MainContext.lsv_LCDTPrint.setAdapter(MainContext.adapter);
            }
        });
        return 0;
    }

    /**
     * 将当前应用运行到前台
     */
    public long CTOS_bring2Front() {
        ActivityManager activtyManager = (ActivityManager) MainContext.getSystemService(MainContext.ACTIVITY_SERVICE);
        List<ActivityManager.RunningTaskInfo> runningTaskInfos = activtyManager.getRunningTasks(3);
        for (ActivityManager.RunningTaskInfo runningTaskInfo : runningTaskInfos) {
            if (MainContext.getPackageName().equals(runningTaskInfo.topActivity.getPackageName())) {
                activtyManager.moveTaskToFront(runningTaskInfo.id, ActivityManager.MOVE_TASK_WITH_HOME);
		   return 0;
            }
        }
        return 0;
    }
    
}


class JNICB_implements_IPT implements JNICB_Lib.cb_IPT
{
    MainActivity MainContext;
    VKeyPad vkeypad;

    public JNICB_implements_IPT(MainActivity InContext)
    {
        MainContext = InContext;
    }


    boolean isViewCreated = false;
    public long ShowVirtualKeyPad(byte[] pVirtualKeyPadPara) {
        Log.d("test", Convert.ByteArrayTohexString(pVirtualKeyPadPara));
        isViewCreated = false;
        MainContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                RelativeLayout layout = (RelativeLayout) MainContext.findViewById(R.id.activity_keypad);
                layout.setVisibility(View.VISIBLE);

                //ConstraintLayout clayout = (ConstraintLayout) MainContext.findViewById(R.id.viewpager);
                //clayout.setVisibility(View.INVISIBLE);
                isViewCreated = true;
            }
        });

        do
        {
            try
            {
                Thread.sleep(1500);
            }
            catch(Exception e)
            {
                e.printStackTrace();
            }

        }while(isViewCreated == false);

        RelativeLayout layout = (RelativeLayout) MainContext.findViewById(R.id.activity_keypad);
        vkeypad = new VKeyPad(MainContext, layout);

        int[][] KBDAttribute = vkeypad.getKeyPadLocation();

        int k = 3;

        byte[] baVirtualKeyPadPara = new byte[1024];
        for(int i = 0; i < 16; i++)
        {
            Log.d("baVirtualPinPadPara" + i,"");
            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][0]/256);
            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][0]%256);
            Log.d(String.format("baVirtualPinPadPara_X : %02X", KBDAttribute[i][0]),"");

            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][1]/256);
            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][1]%256);
            Log.d(String.format("baVirtualPinPadPara_Y : %02X", KBDAttribute[i][1]),"");

            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][2]/256);
            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][2]%256);
            Log.d(String.format("baVirtualPinPadPara_X+W : %02X", KBDAttribute[i][2]),"");

            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][3]/256);
            baVirtualKeyPadPara[k++] = (byte)(KBDAttribute[i][3]%256);
            Log.d(String.format("baVirtualPinPadPara_Y+H : %02X", KBDAttribute[i][3]),"");

            baVirtualKeyPadPara[k++] = (byte)KBDAttribute[i][4];
            Log.d(String.format("baVirtualPinPadPara_V : %02X", KBDAttribute[i][4]),"");
        }

        baVirtualKeyPadPara[0] = 16;
        baVirtualKeyPadPara[1] = (byte)(0 / 256);
        baVirtualKeyPadPara[2] = (byte)(144 % 256);

        Log.d("k : ",  String.valueOf(k));

        Log.d("baVirtualPinPadPara : ", Convert.ByteArrayTohexString(baVirtualKeyPadPara));

        System.arraycopy(baVirtualKeyPadPara,0, baVirtualKeyPadPara,0, k);
        Log.d("pVirtualPinPadPara : ", Convert.ByteArrayTohexString(baVirtualKeyPadPara));



        return 0;
    }


    public long ShowKeyPadMsg(byte digitsNum, final byte pinType, final byte remainingCounter)
    {
        byte[] pinMaskStr = new byte[32];
        Arrays.fill(pinMaskStr, 0, digitsNum, (byte)'*');
        final String str = new String(pinMaskStr);

        MainContext.runOnUiThread(new Runnable()
                                  {
                                      @Override
                                      public void run()
                                      {
                                          TextView textView;
                                          textView = (TextView) MainContext.findViewById(R.id.txvOfflinePin);
                                          TextView txv = (TextView) MainContext.findViewById(R.id.txvOfflinePin);



                                          if(pinType == 0)
                                          {
                                              txv.setText("Enter Online Pin(" + String.valueOf(remainingCounter) + ") :\n");
                                              textView.setText(str);
                                          }
                                          else
                                          {
                                              txv.setText("Enter Offline Pin(" + String.valueOf(remainingCounter) + ") :\n");
                                              textView.setText(str);
                                          }

                                      }
                                  }
        );


        return 0;
    }

    public long GetKeyPadDone()
    {
        isViewCreated = false;
        MainContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ConstraintLayout clayout = (ConstraintLayout) MainContext.findViewById(R.id.viewpager);
                clayout.setVisibility(View.VISIBLE);
                RelativeLayout playout = (RelativeLayout) MainContext.findViewById(R.id.activity_keypad);
                vkeypad.removeAllView(MainContext);
                playout.setVisibility(View.INVISIBLE);
                isViewCreated = true;
            }
        });

        do
        {
            try
            {
                Thread.sleep(1500);
            }
            catch(Exception e)
            {
                e.printStackTrace();
            }

        }while(isViewCreated == false);

        return 0;
    }

    public String IPT_InputAmountEx(String in_data)
    {

        Log.d("IPT_InputAmountEx","IPT_InputAmountEx: " + in_data);

        final String msg = in_data;
        //final short y = usY;

        MainContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {


            }
        });
        return "OK";

    }

}
