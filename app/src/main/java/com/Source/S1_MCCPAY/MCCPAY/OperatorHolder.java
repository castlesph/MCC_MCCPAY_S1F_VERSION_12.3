package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Context;
import android.telephony.TelephonyManager;



public class OperatorHolder {
    private TelephonyManager manager;

    public OperatorHolder(Context context){
        manager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
    }
    public String getOperatorName() {
        return manager.getNetworkOperatorName();
    }
}
