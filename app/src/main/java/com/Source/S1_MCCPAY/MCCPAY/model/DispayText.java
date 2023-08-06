package com.Source.S1_MCCPAY.MCCPAY.model;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.util.Log;
import android.view.Gravity;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.utility.TemporaryData;

/**
 * Created by RexTsai on 2017/9/1.
 */

//public abstract class ShowbmpActivity extends Activitypublic
public class DispayText extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    EditText etAmount;
    Button buOK;
    Button buCancel;
    TextView tvMessage;

    Double dAmount;
    int inResult;
    String stResult;
    int inGravity;

    TextView tvMsgLine01;
    TextView tvMsgLine02;
    TextView tvMsgLine03;
    TextView tvMsgLine04;
    TextView tvMsgLine05;
    TextView tvMsgLine06;
    TextView tvMsgLine07;
    TextView tvMsgLine08;
    TextView tvMsgLine09;
    TextView tvMsgLine10;
    TextView tvMsgLine11;
    TextView tvMsgLine12;
    TextView tvMsgLine13;
    TextView tvMsgLine14;
    TextView tvMsgLine15;
    TextView tvMsgLine16;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        System.out.println("start DispayText...........");
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        //Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

        setContentView(R.layout.displaytextmessage);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-DispayText>>WAKE_LOCK");
        this.mWakeLock.acquire();

        System.out.println("end DispayText...........");

        //tvLine10 = (TextView)findViewById(R.id.text_line_11);
        //tvLine10.setText("1111111111");

        tvMsgLine01 = (TextView) findViewById(R.id.msg_text_01);
        tvMsgLine02 = (TextView) findViewById(R.id.msg_text_02);
        tvMsgLine03 = (TextView) findViewById(R.id.msg_text_03);
        tvMsgLine04 = (TextView) findViewById(R.id.msg_text_04);
        tvMsgLine05 = (TextView) findViewById(R.id.msg_text_05);
        tvMsgLine06 = (TextView) findViewById(R.id.msg_text_06);
        tvMsgLine07 = (TextView) findViewById(R.id.msg_text_07);
        tvMsgLine08 = (TextView) findViewById(R.id.msg_text_08);
        tvMsgLine09 = (TextView) findViewById(R.id.msg_text_09);
        tvMsgLine10 = (TextView) findViewById(R.id.msg_text_10);
        tvMsgLine11 = (TextView) findViewById(R.id.msg_text_11);
        tvMsgLine12 = (TextView) findViewById(R.id.msg_text_12);
        tvMsgLine13 = (TextView) findViewById(R.id.msg_text_13);
        tvMsgLine14 = (TextView) findViewById(R.id.msg_text_14);
        tvMsgLine15 = (TextView) findViewById(R.id.msg_text_15);
        tvMsgLine16 = (TextView) findViewById(R.id.msg_text_16);
        //displaymsg();

        {
            getMsg();
            Intent intent=getIntent();
            String displaymsg=intent.getStringExtra("multiple_msg");

            Log.i("multiple_msg", displaymsg);
            String[] parsemsg = displaymsg.split("\\|");

            System.out.println("line:"+parsemsg[1]+"msg:"+parsemsg[2]+"mode:"+parsemsg[3]);

            int inmsgcnt = parsemsg.length;
            int line = Integer.parseInt(parsemsg[1]);
            String msg = parsemsg[2];
            int dispmode = Integer.parseInt(parsemsg[3]);

            System.out.println("Original Msg");
            System.out.println("line:"+parsemsg[1]+parsemsg[2]+parsemsg[3]);
            System.out.println("Convert Msg");
            System.out.println("line:"+line+msg+dispmode);

            System.out.println("line:"+line+msg+dispmode);

            System.out.printf("dispmode[%d]\n", dispmode);

            if(dispmode == 0)
                inGravity = Gravity.LEFT;
            else if(dispmode == 1)
                inGravity = Gravity.CENTER;
            else if(dispmode == 2)
                inGravity = Gravity.RIGHT;

            System.out.printf("inGravity[%d]\n", inGravity);

            switch (line) {
                case 1:
                    tvMsgLine01.setGravity(inGravity);
                    tvMsgLine01.setText(parsemsg[2]);

                    break;
                case 2:
                    tvMsgLine02.setGravity(inGravity);
                    tvMsgLine02.setText(parsemsg[2]);

                    break;
                case 3:
                    tvMsgLine03.setGravity(inGravity);
                    tvMsgLine03.setText(parsemsg[2]);
                    break;
                case 4:
                    tvMsgLine04.setGravity(inGravity);
                    tvMsgLine04.setText(parsemsg[2]);
                    break;
                case 5:
                    tvMsgLine05.setGravity(inGravity);
                    tvMsgLine05.setText(parsemsg[2]);
                    break;
                case 6:
                    tvMsgLine06.setGravity(inGravity);
                    tvMsgLine06.setText(parsemsg[2]);
                    break;
                case 7:
                    tvMsgLine07.setGravity(inGravity);
                    tvMsgLine07.setText(parsemsg[2]);
                    break;
                case 8:
                    tvMsgLine08.setGravity(inGravity);
                    tvMsgLine08.setText(parsemsg[2]);
                    break;
                case 9:
                    tvMsgLine09.setText(parsemsg[2]);
                    tvMsgLine09.setText(parsemsg[2]);
                    break;
                case 10:
                    tvMsgLine10.setGravity(inGravity);
                    tvMsgLine10.setText(parsemsg[2]);
                    break;
                case 11:
                    tvMsgLine11.setGravity(inGravity);
                    tvMsgLine11.setText(parsemsg[2]);
                    break;
                case 12:
                    tvMsgLine12.setGravity(inGravity);
                    tvMsgLine12.setText(parsemsg[2]);
                    break;
                case 13:
                    tvMsgLine13.setGravity(inGravity);
                    tvMsgLine13.setText(parsemsg[2]);
                    break;
                case 14:
                    tvMsgLine14.setGravity(inGravity);
                    tvMsgLine14.setText(parsemsg[2]);
                    break;
                case 15:
                    tvMsgLine15.setGravity(inGravity);
                    tvMsgLine15.setText(parsemsg[2]);
                    break;
                case 16:
                    tvMsgLine16.setGravity(inGravity);
                    tvMsgLine16.setText(parsemsg[2]);
                    break;
            }
        }
        //finish();
   /*     Intent intent=getIntent();
        String m_msg_=intent.getStringExtra("multiple_msg");

        Log.i("multiple_msg", m_msg_);
        String[] pass_msg = m_msg_.split("\\|");

        int inMsgCnt = pass_msg.length;
        System.out.println("inMsgCnt [" + inMsgCnt + "]");

        for (int inIdx = 0 ; inIdx < inMsgCnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + pass_msg[inIdx] + "]");
            switch (inIdx)
            {
                case 1:
                    tvMsgLine01.setText(pass_msg[inIdx]);
                    break;
                case 2:
                    tvMsgLine02.setText(pass_msg[inIdx]);
                    break;
                case 3:
                    tvMsgLine03.setText(pass_msg[inIdx]);
                    break;
                case 4:
                    tvMsgLine04.setText(pass_msg[inIdx]);
                    break;
                case 5:
                    tvMsgLine05.setText(pass_msg[inIdx]);
                    break;
                case 6:
                    tvMsgLine06.setText(pass_msg[inIdx]);
                    break;
                case 7:
                    tvMsgLine07.setText(pass_msg[inIdx]);
                    break;
                case 8:
                    tvMsgLine08.setText(pass_msg[inIdx]);
                    break;
                case 9:
                    tvMsgLine09.setText(pass_msg[inIdx]);
                    break;
                case 10:
                    tvMsgLine10.setText(pass_msg[inIdx]);
                    break;
                case 11:
                    tvMsgLine11.setText(pass_msg[inIdx]);
                    break;
                case 12:
                    tvMsgLine12.setText(pass_msg[inIdx]);
                    break;
                case 13:
                    tvMsgLine13.setText(pass_msg[inIdx]);
                    break;
                case 14:
                    tvMsgLine14.setText(pass_msg[inIdx]);
                    break;
                case 15:
                    tvMsgLine15.setText(pass_msg[inIdx]);
                    break;
                case 16:
                    tvMsgLine16.setText(pass_msg[inIdx]);
                    break;
            }
        }*/

        /*
        tvMsgLine01.setText("Test msg 01");
        tvMsgLine02.setText("Test msg 02");
        tvMsgLine03.setText("Test msg 03");
        tvMsgLine04.setText("Test msg 04");
        tvMsgLine05.setText("Test msg 05");
        tvMsgLine06.setText("Test msg 06");
        tvMsgLine07.setText("Test msg 07");
        */


        /*
        TextView v = new TextView(this.getApplicationContext());
        v.setCompoundDrawablePadding(-20);
        v.setGravity(Gravity.CENTER_HORIZONTAL);
        Drawable image = getResources().getDrawable(R.drawable.processing);

        image.setBounds(0, 0, image.getMinimumWidth(), image.getMinimumHeight());//非常重要，必须设置，否则图片不会显示
        v.setCompoundDrawables(null,image, null, null);
        */
        //v.setText("测试文字");
        //v.setTextSize(TypedValue.COMPLEX_UNIT_PX,19);//设置字体大小为19px
        //this.wait(500);

        new Handler().postDelayed(new Runnable() {
            public void run() {
                System.out.println("now we finish the activity");
                //execute the task
                DispayText.this.finish();
            }
        }, 5000);  // Please kindly set the delay timeout
    }

    private void displaymsg(){
         runOnUiThread(new Runnable() {
             @Override
             public void run() {
                 getMsg();
                 Intent intent=getIntent();
                 String displaymsg=intent.getStringExtra("multiple_msg");

                 Log.i("multiple_msg", displaymsg);
                 String[] parsemsg = displaymsg.split("\\|");

                 System.out.println("line:"+parsemsg[1]+"msg:"+parsemsg[2]+"mode:"+parsemsg[3]);

                 int inmsgcnt = parsemsg.length;
                 int line = Integer.parseInt(parsemsg[1]);
                 String msg = parsemsg[2];
                 int dispmode = Integer.parseInt(parsemsg[3]);

                 System.out.println("Original Msg");
                 System.out.println("line:"+parsemsg[1]+parsemsg[2]+parsemsg[3]);
                 System.out.println("Convert Msg");
                 System.out.println("line:"+line+msg+dispmode);

                 System.out.println("line:"+line+msg+dispmode);

                 System.out.printf("dispmode[%d]\n", dispmode);

                 if(dispmode == 0)
                     inGravity = Gravity.LEFT;
                 else if(dispmode == 1)
                     inGravity = Gravity.CENTER;
                 else if(dispmode == 2)
                     inGravity = Gravity.RIGHT;

                 System.out.printf("inGravity[%d]\n", inGravity);

                 switch (line) {
                     case 1:
                         tvMsgLine01.setGravity(inGravity);
                         tvMsgLine01.setText(parsemsg[2]);

                         break;
                     case 2:
                         tvMsgLine02.setGravity(inGravity);
                         tvMsgLine02.setText(parsemsg[2]);

                         break;
                     case 3:
                         tvMsgLine03.setGravity(inGravity);
                         tvMsgLine03.setText(parsemsg[2]);
                         break;
                     case 4:
                         tvMsgLine04.setGravity(inGravity);
                         tvMsgLine04.setText(parsemsg[2]);
                         break;
                     case 5:
                         tvMsgLine05.setGravity(inGravity);
                         tvMsgLine05.setText(parsemsg[2]);
                         break;
                     case 6:
                         tvMsgLine06.setGravity(inGravity);
                         tvMsgLine06.setText(parsemsg[2]);
                         break;
                     case 7:
                         tvMsgLine07.setGravity(inGravity);
                         tvMsgLine07.setText(parsemsg[2]);
                         break;
                     case 8:
                         tvMsgLine08.setGravity(inGravity);
                         tvMsgLine08.setText(parsemsg[2]);
                         break;
                     case 9:
                         tvMsgLine09.setText(parsemsg[2]);
                         tvMsgLine09.setText(parsemsg[2]);
                         break;
                     case 10:
                         tvMsgLine10.setGravity(inGravity);
                         tvMsgLine10.setText(parsemsg[2]);
                         break;
                     case 11:
                         tvMsgLine11.setGravity(inGravity);
                         tvMsgLine11.setText(parsemsg[2]);
                         break;
                     case 12:
                         tvMsgLine12.setGravity(inGravity);
                         tvMsgLine12.setText(parsemsg[2]);
                         break;
                     case 13:
                         tvMsgLine13.setGravity(inGravity);
                         tvMsgLine13.setText(parsemsg[2]);
                         break;
                     case 14:
                         tvMsgLine14.setGravity(inGravity);
                         tvMsgLine14.setText(parsemsg[2]);
                         break;
                     case 15:
                         tvMsgLine15.setGravity(inGravity);
                         tvMsgLine15.setText(parsemsg[2]);
                         break;
                     case 16:
                         tvMsgLine16.setGravity(inGravity);
                         tvMsgLine16.setText(parsemsg[2]);
                         break;
                 }
             }
         });


    }

    public void getMsg() {
        tvMsgLine01.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine01, null));
        tvMsgLine02.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine02, null));
        tvMsgLine03.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine03, null));
        tvMsgLine04.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine04, null));
        tvMsgLine05.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine05, null));
        tvMsgLine06.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine06, null));
        tvMsgLine07.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine07, null));
        tvMsgLine08.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine08, null));
        tvMsgLine09.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine09, null));
        tvMsgLine10.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine10, null));
        tvMsgLine11.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine11, null));
        tvMsgLine12.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine12, null));
        tvMsgLine13.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine13, null));
        tvMsgLine14.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine14, null));
        tvMsgLine15.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine15, null));
        tvMsgLine16.setText(TemporaryData.getSharedPreferences().getString(TemporaryData.DispLine16, null));

    }

    public void setMsg(){

        SharedPreferences.Editor editor = TemporaryData.getSharedPreferences().edit();
        editor.putString(TemporaryData.DispLine01, tvMsgLine01.getText().toString());
        editor.putString(TemporaryData.DispLine02, tvMsgLine02.getText().toString());
        editor.putString(TemporaryData.DispLine03, tvMsgLine03.getText().toString());
        editor.putString(TemporaryData.DispLine04, tvMsgLine04.getText().toString());
        editor.putString(TemporaryData.DispLine05, tvMsgLine05.getText().toString());
        editor.putString(TemporaryData.DispLine06, tvMsgLine06.getText().toString());
        editor.putString(TemporaryData.DispLine07, tvMsgLine07.getText().toString());
        editor.putString(TemporaryData.DispLine08, tvMsgLine08.getText().toString());
        editor.putString(TemporaryData.DispLine09, tvMsgLine09.getText().toString());
        editor.putString(TemporaryData.DispLine10, tvMsgLine10.getText().toString());
        editor.putString(TemporaryData.DispLine11, tvMsgLine11.getText().toString());
        editor.putString(TemporaryData.DispLine12, tvMsgLine12.getText().toString());
        editor.putString(TemporaryData.DispLine13, tvMsgLine13.getText().toString());
        editor.putString(TemporaryData.DispLine14, tvMsgLine14.getText().toString());
        editor.putString(TemporaryData.DispLine15, tvMsgLine15.getText().toString());
        editor.putString(TemporaryData.DispLine16, tvMsgLine16.getText().toString());
        editor.apply();

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        setMsg();
        mContext = null;
        SysApplication.getInstance().removeActivity(this);
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        System.out.println("onSaveInstanceState");
        //save the value
        outState.putString("Line01", tvMsgLine01.getText().toString());
        outState.putString("Line02", tvMsgLine02.getText().toString());
        outState.putString("Line03", tvMsgLine03.getText().toString());
        outState.putString("Line04", tvMsgLine04.getText().toString());
        outState.putString("Line05", tvMsgLine05.getText().toString());
        outState.putString("Line06", tvMsgLine06.getText().toString());
        outState.putString("Line07", tvMsgLine07.getText().toString());
        outState.putString("Line08", tvMsgLine08.getText().toString());
        outState.putString("Line09", tvMsgLine09.getText().toString());
        outState.putString("Line10", tvMsgLine10.getText().toString());
        outState.putString("Line11", tvMsgLine11.getText().toString());
        outState.putString("Line12", tvMsgLine12.getText().toString());
        outState.putString("Line13", tvMsgLine13.getText().toString());
        outState.putString("Line14", tvMsgLine14.getText().toString());
        outState.putString("Line15", tvMsgLine15.getText().toString());
        outState.putString("Line16", tvMsgLine16.getText().toString());


        outState.putInt("Gravity01", tvMsgLine01.getGravity());
        outState.putInt("Gravity02", tvMsgLine02.getGravity());
        outState.putInt("Gravity03", tvMsgLine03.getGravity());
        outState.putInt("Gravity04", tvMsgLine04.getGravity());
        outState.putInt("Gravity05", tvMsgLine05.getGravity());
        outState.putInt("Gravity06", tvMsgLine06.getGravity());
        outState.putInt("Gravity07", tvMsgLine07.getGravity());
        outState.putInt("Gravity08", tvMsgLine08.getGravity());
        outState.putInt("Gravity09", tvMsgLine09.getGravity());
        outState.putInt("Gravity10", tvMsgLine10.getGravity());
        outState.putInt("Gravity11", tvMsgLine11.getGravity());
        outState.putInt("Gravity12", tvMsgLine12.getGravity());
        outState.putInt("Gravity13", tvMsgLine13.getGravity());
        outState.putInt("Gravity14", tvMsgLine14.getGravity());
        outState.putInt("Gravity15", tvMsgLine15.getGravity());
        outState.putInt("Gravity16", tvMsgLine16.getGravity());

    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);

        System.out.println("onRestoreInstanceState");
        System.out.println(savedInstanceState.getString("Line01"));
        System.out.println(savedInstanceState.getString("Line02"));
        System.out.println(savedInstanceState.getString("Line03"));
        System.out.println(savedInstanceState.getString("Line04"));
        System.out.println(savedInstanceState.getString("Line05"));
        System.out.println(savedInstanceState.getString("Line06"));
        System.out.println(savedInstanceState.getString("Line07"));
        //get the value
        tvMsgLine01.setText(savedInstanceState.getString("Line01"));
        tvMsgLine02.setText(savedInstanceState.getString("Line02"));
        tvMsgLine03.setText(savedInstanceState.getString("Line03"));
        tvMsgLine04.setText(savedInstanceState.getString("Line04"));
        tvMsgLine05.setText(savedInstanceState.getString("Line05"));
        tvMsgLine06.setText(savedInstanceState.getString("Line06"));
        tvMsgLine07.setText(savedInstanceState.getString("Line07"));
        tvMsgLine08.setText(savedInstanceState.getString("Line08"));
        tvMsgLine09.setText(savedInstanceState.getString("Line09"));
        tvMsgLine10.setText(savedInstanceState.getString("Line10"));
        tvMsgLine11.setText(savedInstanceState.getString("Line11"));
        tvMsgLine12.setText(savedInstanceState.getString("Line12"));
        tvMsgLine13.setText(savedInstanceState.getString("Line13"));
        tvMsgLine14.setText(savedInstanceState.getString("Line14"));
        tvMsgLine15.setText(savedInstanceState.getString("Line15"));
        tvMsgLine16.setText(savedInstanceState.getString("Line16"));


        tvMsgLine01.setGravity(savedInstanceState.getInt("Gravity01"));
        tvMsgLine02.setGravity(savedInstanceState.getInt("Gravity02"));
        tvMsgLine03.setGravity(savedInstanceState.getInt("Gravity03"));
        tvMsgLine04.setGravity(savedInstanceState.getInt("Gravity04"));
        tvMsgLine05.setGravity(savedInstanceState.getInt("Gravity05"));
        tvMsgLine06.setGravity(savedInstanceState.getInt("Gravity06"));
        tvMsgLine07.setGravity(savedInstanceState.getInt("Gravity07"));
        tvMsgLine08.setGravity(savedInstanceState.getInt("Gravity08"));
        tvMsgLine09.setGravity(savedInstanceState.getInt("Gravity09"));
        tvMsgLine10.setGravity(savedInstanceState.getInt("Gravity10"));
        tvMsgLine11.setGravity(savedInstanceState.getInt("Gravity11"));
        tvMsgLine12.setGravity(savedInstanceState.getInt("Gravity12"));
        tvMsgLine13.setGravity(savedInstanceState.getInt("Gravity13"));
        tvMsgLine14.setGravity(savedInstanceState.getInt("Gravity14"));
        tvMsgLine15.setGravity(savedInstanceState.getInt("Gravity15"));
        tvMsgLine16.setGravity(savedInstanceState.getInt("Gravity16"));



    }

}
