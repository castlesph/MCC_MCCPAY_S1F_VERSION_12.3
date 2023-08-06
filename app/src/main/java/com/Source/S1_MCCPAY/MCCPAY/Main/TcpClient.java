package com.Source.S1_MCCPAY.MCCPAY.Main;
import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Arrays;

/*for testing only*/
public class TcpClient {

    public static final String TAG = TcpClient.class.getSimpleName();
    public static final String SERVER_IP = "192.168.68.110"; //server IP address
    public static final int SERVER_PORT = 5041;
    // message to send to the server
    private String mServerMessage;
    // sends message received notifications
    //private OnMessageReceived mMessageListener = null;
    // while this is true, the server will continue running
    private boolean mRun = false;
    // used to send messages
    private PrintWriter mBufferOut;
    // used to read messages from the server
    private BufferedReader mBufferIn;

    public boolean bConnected = false;

    Socket socket;
    BufferedReader receive_PackedResponseData = null;
    BufferedOutputStream outStream = null;

    //private Socket socket = null;
    private OutputStream out = null;
    private InputStream in = null;
    /**
     * Constructor of the class. OnMessagedReceived listens for the messages received from server
     */
    /*
    public TcpClient(OnMessageReceived listener) {
        mMessageListener = listener;
    }
     */

    /**
     * Sends the message entered by client to the server
     *
     * @param message text entered by client
     */
    /*
    public void sendMessage(final String message) {
        String NewMessage="";
        StringBuffer sb = new StringBuffer();

        Log.i(TAG, "sendMessage: "+message);

        //Converting string to character array
        //String2Bcd(message);
        try {
            mBufferOut = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
        } catch (IOException e) {
            e.printStackTrace();
        }

        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (mBufferOut != null) {
                    Log.i(TAG, "Sending: " + message);
                    mBufferOut.println(message);
                    mBufferOut.flush();
                }
            }
        };
        Thread thread = new Thread(runnable);
        thread.start();
    }
    */

    public int Send(String message) {
        int inResult=1;



        String NewMessage="";

        byte[] bytesMessage = message.getBytes();

        Log.i(TAG, "Send: " + message);

        //Converting string to character array
        //String2Bcd(message);
        byte[] b = bin_to_bcd(bytesMessage);
        //byte[] c = packData(b);

        try {
            out.write(b);
            String req = Arrays.toString(b);
            //printing request to console
            System.out.println(TAG+"Sent to server : " + req);

            // Receiving reply from server
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            byte buffer[] = new byte[1024];
            baos.write(buffer, 0 , in.read(buffer));

            byte result[] = baos.toByteArray();

            String res = Arrays.toString(result);

            // printing reply to console
            System.out.println(TAG+"Recieved from server : " + res);
            inResult=0;
        } catch (IOException e) {
            System.out.println(TAG+"--error---");
            inResult=1;
            e.printStackTrace();
        }

        return inResult;
    }

    public int Receive()
    {
        //
        String mServerMessage;

        try {
            //System.out.println(TAG+"-- RESPONSE recieved---");
            //System.out.println(TAG+receive_PackedResponseData);
            //in this while the client listens for the messages sent by the server
            while (mRun) {
                System.out.println(TAG+": Receiving...");
                mServerMessage = receive_PackedResponseData.readLine();

                //if (mServerMessage != null && mMessageListener != null) {
                if (mServerMessage != null) {
                    //call the method messageReceived from MyActivity class
                    //mMessageListener.messageReceived(mServerMessage);
                    System.out.println(TAG+"-- RESPONSE recieved---");
                }

            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            receive_PackedResponseData.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return 0;
    }
    /**
     * Close the connection and release the members
     */
    public void stopClient() {
        Log.i(TAG, "stopClient");
        try {
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        mRun = false;

        if (mBufferOut != null) {
            mBufferOut.flush();
            mBufferOut.close();
        }

        //mMessageListener = null;
        mBufferIn = null;
        mBufferOut = null;
        mServerMessage = null;
    }
/*
    public void runx() {

        mRun = true;

        try {
            //here you must put your computer's IP address.
            InetAddress serverAddr = InetAddress.getByName(SERVER_IP);

            Log.i("TCP Client", "C: Connecting...");

            //create a socket to make the connection with the server
            Socket socket = new Socket(serverAddr, SERVER_PORT);

            try {


                //sends the message to the server
                mBufferOut = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);

                //receives the message which the server sends back
                //mBufferIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                //in this while the client listens for the messages sent by the server
                while (mRun) {
                    Log.i("TCP Client", "C: Connecting...");
                    mServerMessage = mBufferIn.readLine();

                    if (mServerMessage != null && mMessageListener != null) {
                        //call the method messageReceived from MyActivity class
                        mMessageListener.messageReceived(mServerMessage);
                    }

                }

                Log.i("TCP Client", "S: Received Message: '" + mServerMessage + "'");

            } catch (Exception e) {
                Log.e("TCP", "S: Error", e);
            } finally {
                //the socket must be closed. It is not possible to reconnect to this socket
                // after it is closed, which means a new socket instance has to be created.
                socket.close();
            }

        } catch (Exception e) {
            Log.e("TCP", "C: Error", e);
        }

    }

    //Declare the interface. The method messageReceived(String message) will must be implemented in the Activity
    //class at on AsyncTask doInBackground
    public interface OnMessageReceived {
        public void messageReceived(String message);
    }
*/

    public void Connect(String szIP, int inPort) {
        Log.i(TAG, "Connect");
        Log.i(TAG, "IP:"+szIP);
        Log.i(TAG, "Port:"+inPort);

        try {

            InetAddress serverAddr = InetAddress.getByName(szIP);
            InetSocketAddress sockAdr = new InetSocketAddress(serverAddr, inPort);
            socket = new Socket();
            socket.connect(sockAdr, 10000);

            /*
            Log.i(TAG, "C: Connecting...");
            //create a socket to make the connection with the server
            socket = new Socket(serverAddr, inPort);
             */
            socket.setSoTimeout(30000);
            //socket.set
            bConnected=socket.isConnected();
            if(bConnected) {
                // sends output to the socket
                out = new DataOutputStream(socket.getOutputStream());
                //takes input from socket
                in = new DataInputStream(socket.getInputStream());
            }
            mRun=true;
            //Log.i(TAG, "C: Connecting...");
        } catch (Exception e) {
            Log.e("TCP", "C: Error", e);
        }
    }

	public static String byteToHex(byte[] bytes){
		String strHex = "";
		StringBuilder sb = new StringBuilder("");
		for (int n = 0; n < bytes.length; n++) {
			strHex = Integer.toHexString(bytes[n] & 0xFF);
			sb.append((strHex.length() == 1) ? "0" + strHex : strHex); // ÿ���ֽ��������ַ���ʾ��λ����������λ��0
		}
		return sb.toString().trim();
	}

    static byte[] packData(byte[] data) {
        int len = data.length;
        byte buf[] = new byte[len + 2];
        buf[0] = (byte) (len >> 8 & 255);
        buf[1] = (byte) (len & 255);
        System.arraycopy(data, 0, buf, 2, len);
        return buf;
    }

    static byte[] bin_to_bcd(byte[] data) {
        int out_index=0, in_index=0;
        int len = data.length/2;

        byte buf[] = new byte[len];
        byte bFirstNibble, bSecondNibble, bTempData;

        //System.out.println("len:"+len+"\n");

        for(out_index=0; out_index<len; out_index++)
        {
            bTempData=data[in_index];
            if ((data[in_index] >= 'A') && (data[in_index] <= 'F'))
                bTempData=HexAlpha(bTempData);

            bFirstNibble=(byte) (bTempData << 4);
            //System.out.println(String.format("a:%02X", a));

            in_index++;
            bTempData=data[in_index];
            if ((data[in_index] >= 'A') && (data[in_index] <= 'F'))
                bTempData=HexAlpha(bTempData);
            bSecondNibble=(byte) (bTempData & 0x0f);
            //System.out.println(String.format("b:%02X", b));
            in_index++;

            buf[out_index] = (byte) (bFirstNibble+bSecondNibble);

            //System.out.println(String.format("cc:%02X", buf[out_index]));
        }
        //System.out.println("out_index:"+ out_index);

        return buf;
    }

    static String bcd_to_bin(byte[] data) {
        int out_index=0, in_index=0;
        int len = data.length;

        //len=16;

        byte buf[] = new byte[len*2];
        byte bFirstNibble, bSecondNibble, bTempData;

        //System.out.println("user_str len:"+len+"\n");

        for(out_index=0; out_index<len; out_index++)
        {
            bTempData=data[out_index];
            //System.out.println(String.format("bTempData:%02X", (bTempData & 0xf0)));
            if((bTempData & 0xf0) == 0xA0)
                bFirstNibble=0x41;
            else if((bTempData & 0xf0) == 0xB0)
                bFirstNibble=0x42;
            else if((bTempData & 0xf0) == 0xC0)
                bFirstNibble=0x43;
            else if((bTempData & 0xf0) == 0xD0)
                bFirstNibble=0x44;
            else if((bTempData & 0xf0) == 0xE0)
                bFirstNibble=0x45;
            else if((bTempData & 0xf0) == 0xF0)
                bFirstNibble=0x46;
            else
                bFirstNibble=(byte) (((bTempData >> 4) & 0x0f) + 0x30);

            //System.out.println("in_index:"+in_index);
            buf[in_index]=bFirstNibble;
            in_index++;

            bTempData=data[out_index];
            if((bTempData & 0x0f) == 0x0A)
                bSecondNibble=0x41;
            else if((bTempData & 0x0f) == 0x0B)
                bSecondNibble=0x42;
            else if((bTempData & 0x0f) == 0x0C)
                bSecondNibble=0x43;
            else if((bTempData & 0x0f) == 0x0D)
                bSecondNibble=0x44;
            else if((bTempData & 0x0f) == 0x0E)
                bSecondNibble=0x45;
            else if((bTempData & 0x0f) == 0x0F)
                bSecondNibble=0x46;
            else
                bSecondNibble=(byte) ((bTempData & 0x0f)+0x30);

            buf[in_index]=bSecondNibble;
            in_index++;
            //System.out.println("in_index:"+in_index+" new:"+new String(buf));
        }
        //System.out.println("out_index:"+ out_index);
        return new String(buf);
    }

    static byte HexAlpha(byte data) {
        byte bByte=0x00;

        if(data == 'A')
            bByte = 0x0A;
        else if(data == 'B')
            bByte = 0x0B;
        else if(data == 'C')
            bByte = 0x0C;
        else if(data == 'D')
            bByte = 0x0D;
        else if(data == 'E')
            bByte = 0x0E;
        else if(data == 'F')
            bByte = 0x0F;

        return bByte;
    }
}
