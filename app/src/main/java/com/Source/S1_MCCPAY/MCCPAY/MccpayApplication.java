package com.Source.S1_MCCPAY.MCCPAY;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;

//import com.squareup.leakcanary.LeakCanary;


import java.util.ArrayList;
import java.util.List;

import CTOS.CtCtms;

public class MccpayApplication extends Application {
    private static final String TAG = "MccpayApplication";
    private Context mContext;
    private static int activityNum = 0;
    private static List<String> activityList = new ArrayList<String>();
    private static CtCtms ctCtms;
    public static List<String> getActivityList() {
        return activityList;
    }

    public static int getActivityNum() {
        Log.i(TAG, "getActivityNum: " + activityNum);
        return activityNum;
    }

    public static CtCtms getCtCtmsObj() {
        Log.i(TAG, "getCtCtmsObj getCTMSStatus: " + ctCtms.getCTMSStatus());
        return ctCtms;
    }



    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "onCreate: ");
/*
        //Check Memory Leak
        if (LeakCanary.isInAnalyzerProcess(this)) {
            //此过程专用于LeakCanary进行堆分析。在此过程中不应初始化应用程序。
            Log.i(TAG, "isInAnalyzerProcess: ");
            return;
        }
        LeakCanary.install(this);
*/
        registerActivityLifecycleCallbacks(new ActivityLifecycleCallbacks() {
            @Override
            public void onActivityCreated(Activity activity, Bundle bundle) {
                Log.i(TAG, "onActivityCreated: " + activity.toString());
                activityNum++;
                activityList.add(activity.getLocalClassName());
            }

            @Override
            public void onActivityStarted(Activity activity) {
                Log.i(TAG, "onActivityStarted: " + activity.toString());
            }

            @Override
            public void onActivityResumed(Activity activity) {
                Log.i(TAG, "onActivityResumed: " + activity.toString());
                //MyActivityManager.getInstance().setCurrentActivity(activity);
            }

            @Override
            public void onActivityPaused(Activity activity) {
                Log.i(TAG, "onActivityPaused: " + activity.toString());
            }

            @Override
            public void onActivityStopped(Activity activity) {
                Log.i(TAG, "onActivityStopped: " + activity.toString());
            }

            @Override
            public void onActivitySaveInstanceState(Activity activity, Bundle bundle) {
                Log.i(TAG, "onActivitySaveInstanceState: ");
            }

            @Override
            public void onActivityDestroyed(Activity activity) {
                Log.i(TAG, "onActivityDestroyed: " + activity.toString() + " class name: " + activity.getLocalClassName());
                activityNum--;
                activityList.remove(activity.getLocalClassName());
                CloseActivityClass.activityList.remove(activity);
            }
        });

    }
}

