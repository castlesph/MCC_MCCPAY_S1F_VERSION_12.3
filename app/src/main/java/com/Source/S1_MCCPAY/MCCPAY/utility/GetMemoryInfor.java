package com.Source.S1_MCCPAY.MCCPAY.utility;

import android.app.ActivityManager;
import android.content.Context;
import android.util.Log;

import static android.content.Context.ACTIVITY_SERVICE;

public class GetMemoryInfor{
    private static final String TAG = "GetMemoryInfor";
    public static void displayBriefMemory(Context context) {

        final ActivityManager activityManager = (ActivityManager) context.getSystemService(ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo info = new ActivityManager.MemoryInfo();
        activityManager.getMemoryInfo(info);
        Log.i("MEM","Sys  Total:"+(info.totalMem >> 10)+"k");//922416k
        Log.i("MEM","Sys avail:"+(info.availMem >> 10)+"k");
        Log.i("MEM","is low memory: "+info.lowMemory);
        //Log.i("MEM","threshold:"+info.threshold);
        Log.i("MEM","threshold:"+(info.threshold >> 10)+"k");
        int maxMemory = (int) (Runtime.getRuntime().maxMemory() / 1024);
        Log.d(TAG, "the virtual machine attempt Max memory is " + maxMemory + "KB");
        int totalMemory = (int) (Runtime.getRuntime().totalMemory() / 1024);
        Log.d(TAG, "the virtual machine attempt total memory is " + totalMemory + "KB");
        int freeMemory = (int) (Runtime.getRuntime().freeMemory() / 1024);
        Log.d(TAG, "the virtual machine free Memory is " + freeMemory + "KB");
    }
}
