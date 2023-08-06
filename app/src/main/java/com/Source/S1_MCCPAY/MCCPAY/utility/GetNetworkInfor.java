package com.Source.S1_MCCPAY.MCCPAY.utility;

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.util.Log;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;  
  


import static android.content.Context.WIFI_SERVICE;
import static android.content.Context.CONNECTIVITY_SERVICE;


public class GetNetworkInfor {
    private static final String TAG = "GetNetworkInfor";
    private  Context mContext;

    public GetNetworkInfor(Context context) {
        mContext = context;
    }


 
    public  String getWifiSSID() {
        String s = "";

		Log.i(TAG, "saturn getWifiSSID");
        WifiManager wm = (WifiManager) mContext.getSystemService(WIFI_SERVICE);
        if (wm != null) {
            WifiInfo winfo = wm.getConnectionInfo();
            if (winfo != null) {
                s = winfo.getSSID();
                Log.i(TAG, "saturn getWifiSSID: " + s);
                if (s.length() > 2 && s.charAt(0) == '"' && s.charAt(s.length() - 1) == '"'){
                    return s.substring(1, s.length() - 1);
                }
            }
        }
        return s;
    }

    public String getWifiBSSID() {
        WifiManager wm = (WifiManager) mContext.getSystemService(WIFI_SERVICE);
        if (wm != null) {
            WifiInfo winfo = wm.getConnectionInfo();
            return  winfo.getBSSID();
        }
        return null;
    }


	public boolean isOnline() {
		ConnectivityManager cm = (ConnectivityManager) mContext.getSystemService(CONNECTIVITY_SERVICE);
    	NetworkInfo netInfo = cm.getActiveNetworkInfo();
    	if (netInfo != null && netInfo.isConnected()) {
			//Log.i(TAG, "saturn isOnline : true ");
        	return true;
    	}else {  
			//Log.i(TAG, "saturn isOnline : false ");
        	return false;
    	}
    }

	
}


