package com.Source.S1_MCCPAY.MCCPAY.Kms;

/**
 * Created by aaron on 2017/4/21.
 */

public class Convert {

    private static final String TAG_A = "aaron";
    private static final String TAG_C = "Convert";

    public static int byteArrayToInt(byte[] b) {
        return b[3] & 0xFF |
                (b[2] & 0xFF) << 8 |
                (b[1] & 0xFF) << 16 |
                (b[0] & 0xFF) << 24;
    }

    public static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        if (len % 2 != 0) {
            return data;
        }
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }

    public static String ByteArrayTohexString(byte[] b) {
        String str = "";
        for (byte aB : b) {
            str += Integer.toString((aB & 0xff) + 0x100, 16).substring(1);
        }
        return str;
    }
}
