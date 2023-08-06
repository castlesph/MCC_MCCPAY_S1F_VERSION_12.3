package com.Source.S1_MCCPAY.MCCPAY.utility;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import java.lang.Object;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class GetAppInfor {
    private static final String TAG = "saturn GetAppInfor";
    private Context mContext = null;

    public GetAppInfor(Context context) {
        mContext = context;
    }

    public String getInstalledAppHashCode() {
        File file = new File(mContext.getApplicationContext().getPackageCodePath());
        Log.i(TAG, "code path: " + mContext.getApplicationContext().getPackageCodePath());
        FileInputStream input = null;
        ByteArrayOutputStream output = null;
        String hash = "";
        int bufferSize = 0;
        if (!file.exists())
            return null;

        Log.i(TAG, "getInstalledAppHashCode: ");
        try {
            input = new FileInputStream(file);
            bufferSize = input.available();
            Log.i(TAG, "getInstalledAppHashCode: " + bufferSize);
            output = new ByteArrayOutputStream();
//            Log.i(TAG, "allocate buffer size: ");
            byte[] buffer = new byte[bufferSize];
//            Log.i(TAG, "allocate buffer size done: ");
            int l = 0;

            while (true) {
                try {
                    if ((l = input.read(buffer)) > 0) break;
//                if (!((l = input.read (buffer)) > 0)) break;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            Log.i(TAG, "write len: " + l);
            output.write(buffer, 0, l);
            byte[] data = output.toByteArray();

            if (input != null) input.close();
            if (output != null) output.close();

            MessageDigest digest = null;
            try {
                digest = MessageDigest.getInstance("SHA-1");
            } catch (NoSuchAlgorithmException e) {
                e.printStackTrace();
            }

            byte[] bytes = data;
            Log.i(TAG, "bytes.length: " + bytes.length);
            digest.update(bytes, 0, bytes.length);
            bytes = digest.digest();
            StringBuilder sb = new StringBuilder();
            for (byte b : bytes)
                sb.append(String.format("%02X", b));
            hash = sb.toString();

        } catch (IOException e) {
            e.printStackTrace();
        } finally {

        }

        Log.i(TAG, "saturn getInstalledAppHashCode: " + hash);
        return hash;
    }

    public String getAppHashCode(String appFileName) {

        FileInputStream input = null;
        ByteArrayOutputStream output = null;
        String hash = "";
        int bufferSize = 0;


	Log.i(TAG, "saturn getapphahcode");
		//test
		PackageManager pm = mContext.getPackageManager();

		//for (ApplicationInfo app : pm.getInstalledApplications(0)) {

		    //ApplicationInfo app  = mContext.getPackageManager().getApplicationInfo("tes",0);
  		//	Log.d("saturn PackageList", "saturn package: " + app.packageName + ", sourceDir: " + app.sourceDir);
		//}
        String szSourceDir="";

		try {
			ApplicationInfo app = pm.getApplicationInfo(appFileName, 0);
			Log.d("saturn PackageList", "saturn package: " + app.packageName + ", sourceDir: " + app.sourceDir);
            szSourceDir = app.sourceDir;
		} catch (PackageManager.NameNotFoundException e) {
			Log.w("yourtag", "Error Package name not found ", e);
		}
		
        for (int i = 1; i < 6; i++) {
            //String appFilePath = "/data/app/" + appFileName + "-" + i + "/base.apk";

           String appFilePath  = szSourceDir;

           

            //File file=new File(mContext.getApplicationContext().getPackageCodePath());
            //Log.i(TAG, "code path: " + mContext.getApplicationContext().getPackageCodePath());

            File file = new File(appFilePath);
            Log.i(TAG, "saturn code path: " + appFilePath);
            if (!file.exists()) {
                Log.i("saturn getAppHashCode", "saturn UNKNOWN FILE");
                //return "0000000000000000000000000000000000000000";
            } else {
                Log.i(TAG, "saturn getInstalledAppHashCode: ");
                try {
                    input = new FileInputStream(file);
                    bufferSize = input.available();
                    Log.i(TAG, "saturn getInstalledAppHashCode: " + bufferSize);
                    output = new ByteArrayOutputStream();
//                  Log.i(TAG, "allocate buffer size: ");
                    byte[] buffer = new byte[bufferSize];
//                  Log.i(TAG, "allocate buffer size done: ");
                    int l = 0;

                    while (true) {
                        try {
                            if ((l = input.read(buffer)) > 0) break;
//                          if (!((l = input.read (buffer)) > 0)) break;
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                    Log.i(TAG, "write len: " + l);
                    output.write(buffer, 0, l);
                    byte[] data = output.toByteArray();

                    if (input != null) input.close();
                    if (output != null) output.close();

                    MessageDigest digest = null;
                    try {
                        digest = MessageDigest.getInstance("SHA-1");
                    } catch (NoSuchAlgorithmException e) {
                        e.printStackTrace();
                    }

                    byte[] bytes = data;
                    Log.i(TAG, "bytes.length: " + bytes.length);
                    digest.update(bytes, 0, bytes.length);
                    bytes = digest.digest();
                    StringBuilder sb = new StringBuilder();
                    for (byte b : bytes)
                        sb.append(String.format("%02X", b));
                    hash = sb.toString();

                } catch (IOException e) {
                    e.printStackTrace();
                } finally {

                }
            break;
            }
        }

        Log.i(TAG, "getInstalledAppHashCode: " + hash);
        if(hash.equals(null))
            return "0000000000000000000000000000000000000000";
        else
            return hash;
    }

}
