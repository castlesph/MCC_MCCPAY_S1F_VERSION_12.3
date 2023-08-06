package com.Source.S1_MCCPAY.MCCPAY.utility;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

public class TemporaryData {
    public static final String FileCopyTime = "FileCopyTime";
    public static final String DispLine01 = "DispLine01";
    public static final String DispLine02 = "DispLine02";
    public static final String DispLine03 = "DispLine03";
    public static final String DispLine04 = "DispLine04";
    public static final String DispLine05 = "DispLine05";
    public static final String DispLine06 = "DispLine06";
    public static final String DispLine07 = "DispLine07";
    public static final String DispLine08 = "DispLine08";
    public static final String DispLine09 = "DispLine09";
    public static final String DispLine10 = "DispLine10";
    public static final String DispLine11 = "DispLine11";
    public static final String DispLine12 = "DispLine12";
    public static final String DispLine13 = "DispLine13";
    public static final String DispLine14 = "DispLine14";
    public static final String DispLine15 = "DispLine15";
    public static final String DispLine16 = "DispLine16";

    public static final String Gravity01 = "Gravity01";
    public static final String Gravity02 = "Gravity02";
    public static final String Gravity03 = "Gravity03";
    public static final String Gravity04 = "Gravity04";
    public static final String Gravity05 = "Gravity05";
    public static final String Gravity06 = "Gravity06";
    public static final String Gravity07 = "Gravity07";
    public static final String Gravity08 = "Gravity08";
    public static final String Gravity09 = "Gravity09";
    public static final String Gravity10 = "Gravity10";
    public static final String Gravity11 = "Gravity11";
    public static final String Gravity12 = "Gravity12";
    public static final String Gravity13 = "Gravity13";
    public static final String Gravity14 = "Gravity14";
    public static final String Gravity15 = "Gravity15";
    public static final String Gravity16 = "Gravity16";

    public static final String PaddingLeft01 = "PaddingLeft01";
    public static final String PaddingLeft02 = "PaddingLeft02";
    public static final String PaddingLeft03 = "PaddingLeft03";
    public static final String PaddingLeft04 = "PaddingLeft04";
    public static final String PaddingLeft05 = "PaddingLeft05";
    public static final String PaddingLeft06 = "PaddingLeft06";
    public static final String PaddingLeft07 = "PaddingLeft07";
    public static final String PaddingLeft08 = "PaddingLeft08";
    public static final String PaddingLeft09 = "PaddingLeft09";
    public static final String PaddingLeft10 = "PaddingLeft10";
    public static final String PaddingLeft11 = "PaddingLeft11";
    public static final String PaddingLeft12 = "PaddingLeft12";
    public static final String PaddingLeft13 = "PaddingLeft13";
    public static final String PaddingLeft14 = "PaddingLeft14";
    public static final String PaddingLeft15 = "PaddingLeft15";
    public static final String PaddingLeft16 = "PaddingLeft16";

	
    public static final String CTLSDEMO = "CTLSDEMO";

    private static SharedPreferences mSharedPreferences;

    public static void init(Context context) {
        mSharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);

        SharedPreferences.Editor editor = mSharedPreferences.edit();
        editor.putInt(FileCopyTime, 0);
        editor.apply();
    }


    public static SharedPreferences getSharedPreferences() {
        return mSharedPreferences;
    }


    public static int FileCopyTime() {
        int inTimes = mSharedPreferences.getInt(FileCopyTime, 1);
        inTimes = inTimes + 1;

        SharedPreferences.Editor editor = mSharedPreferences.edit();
        editor.putInt(FileCopyTime, inTimes);
        editor.apply();

        return inTimes;
    }


}
