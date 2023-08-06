package com.Source.S1_MCCPAY.MCCPAY;

import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class SocketActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
   static {
        System.loadLibrary("native-lib");
    }

    static {
        System.loadLibrary("ssl");
    }

    static {
        System.loadLibrary("crypto");
    }
	
    Button send;
    Button close;
    Button init;
    EditText ip;
    EditText port;
    EditText sendEdit;
    EditText recvEdit;
    Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what){
                case 11:
                    int len = msg.getData().getString("data").length();
					
                    printHexString(msg.getData().getString("data").getBytes(), len);

                    String recvData = msg.getData().getString("data");

                    System.out.println("recvData :" + recvData);


                    recvEdit.setText(recvData);
                    break;
            }

        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_socket);
        send = (Button) findViewById(R.id.send_button);
        close = (Button) findViewById(R.id.close_button);
        init = (Button) findViewById(R.id.init_button);
        ip = (EditText)findViewById(R.id.ip);
        port = (EditText)findViewById(R.id.port);
        sendEdit = (EditText) findViewById(R.id.send_edit);
        recvEdit = (EditText) findViewById(R.id.recv_edit);
        ip.setText("118.201.48.213");
        port.setText("5010");
        //sendEdit.setText("49204c6f7665");
        sendEdit.setText("0095600145000002002020010000C01003010000000001055530303030303030313030303030303030303030303030316DA6DFCCAECDB0CA0031333A505030302E30314D53414E4445533A505030302E30314D53414E4445531234567890ABCDEF");
        //sendEdit.setText("333333");
        init.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                inEthernet_ConnectHost(ip.getText().toString(), Integer.valueOf(port.getText().toString()));
                init.setText("connected");
                init.setClickable(false);
                init.setBackgroundColor(Color.DKGRAY);
            }
        });
        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String temp = sendEdit.getText().toString();

                System.out.println("sendData temp Len :" + temp.length());
                System.out.println("sendData temp:" + temp);

                inEthernet_SendData(hexStrToByteArray(temp));
                inEthernet_ReceiveData();
            }
        });

        close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                inEthernet_Disconnection();
                init.setText("INIT CLIENT");
                init.setClickable(true);
                init.setBackgroundColor(Color.LTGRAY);
            }
        });

    }
    public void setRecevieData(byte[] bb){

        int len = bb.length;

        printHexString(bb, len);

        //char[] cc = new char[bb.length];
        //for(int i=0;i<bb.length;i++){
            //cc[i]=(char)bb[i];
        //}
        //Log.d("native_socket",String.valueOf(cc));

        String recvData = byteArrayToHexStr(bb);

        Message message = new Message();
        message.what = 11;
        Bundle bundle = new Bundle();
        bundle.putString("data", recvData);
        message.setData(bundle);
        handler.sendMessage(message);
    }


	public static String byteArrayToHexStr(byte[] byteArray) {
    if (byteArray == null){
        return null;
    }
    char[] hexArray = "0123456789ABCDEF".toCharArray();
    char[] hexChars = new char[byteArray.length * 2];
    for (int j = 0; j < byteArray.length; j++) {
		int v = byteArray[j] & 0xFF;
		
        hexChars[j * 2] = hexArray[v >>> 4];
        hexChars[j * 2 + 1] = hexArray[v & 0x0F];
    }
    return new String(hexChars);
}

	public static byte[] hexStrToByteArray(String str)
{
    if (str == null) {
        return null;
    }
    if (str.length() == 0) {
        return new byte[0];
    }
    byte[] byteArray = new byte[str.length() / 2];
    for (int i = 0; i < byteArray.length; i++){
        String subStr = str.substring(2 * i, 2 * i + 2);
        byteArray[i] = ((byte) Integer.parseInt(subStr, 16));
    }
    return byteArray;
}

    /**
     * @Title:bytes2HexString
     * @Description:字节数组转16进制字符串
     * @param b
     *            字节数组
     * @return 16进制字符串
     * @throws
     */
    public static String bytes2HexString(byte[] b) {
        StringBuffer result = new StringBuffer();
        String hex;
        for (int i = 0; i < b.length; i++) {
            hex = Integer.toHexString(b[i] & 0xFF);
            if (hex.length() == 1) {
                hex = '0' + hex;
            }
            result.append(hex.toUpperCase());
        }
        return result.toString();
    }

    /**
     * @Title:hexString2Bytes
     * @Description:16进制字符串转字节数组
     * @param src
     *            16进制字符串
     * @return 字节数组
     * @throws
     */
    public static byte[] hexString2Bytes(String src) {
        int l = src.length() / 2;
        byte[] ret = new byte[l];
        for (int i = 0; i < l; i++) {
            ret[i] = (byte) Integer
                    .valueOf(src.substring(i * 2, i * 2 + 2), 16).byteValue();
        }
        return ret;
    }

    /**
     * @Title:string2HexString
     * @Description:字符串转16进制字符串
     * @param strPart
     *            字符串
     * @return 16进制字符串
     * @throws
     */
    public static String string2HexString(String strPart) {
        StringBuffer hexString = new StringBuffer();
        for (int i = 0; i < strPart.length(); i++) {
            int ch = (int) strPart.charAt(i);
            String strHex = Integer.toHexString(ch);
            hexString.append(strHex);
        }
        return hexString.toString();
    }

    /**
     * @Title:hexString2String
     * @Description:16进制字符串转字符串
     * @param src
     *            16进制字符串
     * @return 字节数组
     * @throws
     */
    public static String hexString2String(String src) {
        String temp = "";
        for (int i = 0; i < src.length() / 2; i++) {
            temp = temp
                    + (char) Integer.valueOf(src.substring(i * 2, i * 2 + 2),
                    16).byteValue();
        }
        return temp;
    }

    /**
     * @Title:char2Byte
     * @Description:字符转成字节数据char-->integer-->byte
     * @param src
     * @return
     * @throws
     */
    public static Byte char2Byte(Character src) {
        return Integer.valueOf((int)src).byteValue();
    }

    /**
     * @Title:intToHexString
     * @Description:10进制数字转成16进制
     * @param a 转化数据
     * @param len 占用字节数
     * @return
     * @throws
     */
    private static String intToHexString(int a, int len){
        len<<=1;
        String hexString = Integer.toHexString(a);
        int b = len -hexString.length();
        if(b>0){
            for(int i=0;i<b;i++)  {
                hexString = "0" + hexString;
            }
        }
        return hexString;
    }
    private static void printHexString(byte[] b, int iLen)
    {
        for (int i = 0; i < iLen; i++)
        {
            String hex = Integer.toHexString(b[i] & 0xFF);
            if (hex.length() == 1)
            {
                hex = '0' + hex;
            }
            System.out.print(hex.toUpperCase() + " ");
        }
        System.out.println("");
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int inEthernet_ConnectHost(String ip, int port);
    public native void inEthernet_Disconnection();
    public native int inEthernet_SendData(byte[] buffer);
	public native int inEthernet_ReceiveData();
}
