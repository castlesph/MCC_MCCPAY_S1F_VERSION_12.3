package com.Source.S1_MCCPAY.MCCPAY;

import android.app.Activity;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by xuit96lo on 2017/8/11.
 */

public class CloseActivityClass{
    public static List<Activity> activityList = new ArrayList<Activity>();

    public static void exitClient()
    {
        // 關閉所有Activity
        for (int i = 0; i < activityList.size(); i++)
        {
            if (null != activityList.get(i))
            {
                activityList.get(i).finish();
            }
        }
    }
}