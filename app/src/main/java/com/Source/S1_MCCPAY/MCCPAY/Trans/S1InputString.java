package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;
import com.Source.S1_MCCPAY.MCCPAY.wub_lib;

public class S1InputString extends DemoAppActivity {


    Button btnKey1, btnKey2, btnKey3,btnKey4,btnKey5,btnKey6,btnKey7,btnKey8,btnKey9,
            btnKey0,btnKeyEnter,btnKeyCancel,btnKeyClear;
    TextView textView_txn;
    TextView textView_disp;
    TextView textView_entry;
    String userentry="", maskedentry="", password="", passwordchar="*";


    public static String final_amt_string;
    public static String final_string;
    int invmaxlength=0, invminlength=0, typedef=0;


    private int inTimeOut = 0;
    String display, maxlength, minlength, type, password1="", password2="", password3="";

    CountDownTimer ctimer = null;

    final Context c = this;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        //| View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        //| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        //| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        //| View.SYSTEM_UI_FLAG_FULLSCREEN);

        if(MainActivity.inTermType == 5)
            setContentView(R.layout.activity_user_entry_s1em);
        else
            setContentView(R.layout.activity_user_entry);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:S1InputString>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String value=intent.getStringExtra("pass_in_string");
        Log.i("value", value);

        btnKey1=(Button) findViewById(R.id.ibtnOne);
        btnKey2=(Button) findViewById(R.id.ibtnTwo);
        btnKey3=(Button) findViewById(R.id.ibtnThree);
        btnKey4=(Button) findViewById(R.id.ibtnFour);
        btnKey5=(Button) findViewById(R.id.ibtnFive);
        btnKey6=(Button) findViewById(R.id.ibtnSix);
        btnKey7=(Button) findViewById(R.id.ibtnSeven);
        btnKey8=(Button) findViewById(R.id.ibtnEight);
        btnKey9=(Button) findViewById(R.id.ibtnNine);
        btnKey0=(Button) findViewById(R.id.ibtnZero);
        btnKeyCancel=(Button) findViewById(R.id.ibtnCancel);
        btnKeyClear=(Button) findViewById(R.id.ibtnClear);
        btnKeyEnter=(Button) findViewById(R.id.ibtnEnter);


        textView_txn = (TextView) findViewById(R.id.textView_txn);
        textView_entry= (TextView) findViewById(R.id.textView_entry);
        textView_disp = (TextView) findViewById(R.id.textView_disp);

        String[] dispmsginfo = value.split("\\|");
        int msgcnt = dispmsginfo.length;

        System.out.println("msgcnt [" + msgcnt + "]");
        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    type=dispmsginfo[inIdx];
                    break;
                case 1:
                    minlength=dispmsginfo[inIdx];
                    break;
                case 2:
                    maxlength=dispmsginfo[inIdx];
                    break;
                case 3:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textView_disp.setText(dispmsginfo[inIdx]);
                    if(dispmsginfo[inIdx].equals("CARD NUMBER: "))
                    {
                        inTimeOut = 90;
                        getTimerRestart(inTimeOut);
                    }
                    break;
                case 5:
                    password1=dispmsginfo[inIdx];
                    break;
                case 6:
                    password2=dispmsginfo[inIdx];
                    break;
                case 7:
                    password3=dispmsginfo[inIdx];
                    break;
            }
        }

        typedef = Integer.valueOf(type);
        invminlength = Integer.valueOf(minlength);
        invmaxlength = Integer.valueOf(maxlength);

        //Toast.makeText(getApplicationContext(), "display: "+display, Toast.LENGTH_SHORT).show();
        //Toast.makeText(getApplicationContext(), "minlength: "+minlength, Toast.LENGTH_SHORT).show();
        //Toast.makeText(getApplicationContext(), "maxlength: "+maxlength, Toast.LENGTH_SHORT).show();
        //Toast.makeText(getApplicationContext(), "type: "+type, Toast.LENGTH_SHORT).show();

        Log.i("pass in value", value);



        FuncKey1();
        FuncKey2();
        FuncKey3();
        FuncKey4();
        FuncKey5();
        FuncKey6();
        FuncKey7();
        FuncKey8();
        FuncKey9();
        FuncKey0();
        FuncKeyCancel();
        FuncKeyClear();
        FuncKeyEnter();

    }

    public void FuncKey1() {
        btnKey1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey1); // sidumili: Press animation

                userentry = userentry + "1";

                //Type 1 - numeric
                if(typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }
                //Type 2 = password
                if(typedef == 2){
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }
            }
        });
    }

    public void FuncKey2() {
        btnKey2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey2); // sidumili: Press animation

                userentry = userentry + "2";

                //Type 1 - numeric
                if(typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if(typedef == 2){
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKey3() {
        btnKey3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey3); // sidumili: Press animation

                userentry = userentry + "3";

                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKey4() {
        btnKey4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey4); // sidumili: Press animation

                userentry = userentry + "4";
                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKey5() {
        btnKey5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey5); // sidumili: Press animation

                userentry = userentry + "5";
                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }


            }
        });
    }

    public void FuncKey6() {
        btnKey6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey6); // sidumili: Press animation

                userentry = userentry + "6";
                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKey7() {
        btnKey7.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey7); // sidumili: Press animation

                userentry = userentry + "7";
                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKey8() {
        btnKey8.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey8); // sidumili: Press animation

                userentry = userentry + "8";
                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        // Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKey9() {
        btnKey9.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey9); // sidumili: Press animation

                userentry = userentry + "9";
                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_SHORT).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }


    public void FuncKey0() {
        btnKey0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKey0); // sidumili: Press animation

                userentry = userentry + "0";
                //Type 1 - numeric
                if (typedef == 1) {
                    if (userentry.length() > invmaxlength) {
                        invalidLength("maxlength");
                        //Toast.makeText(getApplicationContext(), "Maximum of " + String.valueOf(invmaxlength) + "digits only", Toast.LENGTH_LONG).show();
                        userentry = userentry.substring(0, userentry.length() - 1);
                        return;
                    }
                    textView_entry.setText(userentry);
                }

                //Type 2 = password
                if (typedef == 2) {
                    maskedentry = maskedentry + passwordchar;
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKeyCancel(){
        btnKeyCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKeyCancel); // sidumili: Press animation

                if(userentry.length() > 0) {
                    userentry = "";
                    textView_entry.setText(userentry);

                    if(typedef == 2){
                        maskedentry = "";
                        textView_entry.setText(maskedentry);
                    }
                    return;
                }

                /*cancel timer first*/
                getTimerCancel();


                final_string = "CANCEL";
                //startActivity(new Intent(S1InputString.this, MainActivity.class));
                S1InputString.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buCancel");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

    }

    public void FuncKeyClear() {
        btnKeyClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKeyClear); // sidumili: Press animation

                if(userentry.length() > 0) {
                    userentry = userentry.substring(0, userentry.length()-1);
                    if(typedef == 2)
                        maskedentry = maskedentry.substring(0, maskedentry.length()-1);
                }

                //txtViewAmount.setText(tools.getFormatAmount(Amount).toString());
                if(typedef == 1)
                    textView_entry.setText(userentry);
                else if(typedef == 2){
                    textView_entry.setText(maskedentry);
                }

            }
        });
    }

    public void FuncKeyEnter() {
        btnKeyEnter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                wub_lib.ButtonPressed(c, btnKeyEnter); // sidumili: Press animation

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                //final_amt_string = etAmount.getText().toString();
                //final_amt_string="12345678";
                if(typedef == 1) {

                    if (userentry.length() < invminlength) {
                        invalidLength("minlength");
                        //Toast.makeText(getApplicationContext(), "Minimum length must be " + String.valueOf(invminlength), Toast.LENGTH_LONG).show();
                        userentry="";
                        return;
                    }

                    /*cancel timer first*/
                    getTimerCancel();

                    final_string = userentry;
                    S1InputString.this.finish();

                    Log.i("Tine", "Get Invoice KeyBoard buOK");
                }
                else if(typedef == 2){
                    if (userentry.length() < invminlength) {
                        invalidLength("minlength");
                        //Toast.makeText(getApplicationContext(), "Minimum length must be " + String.valueOf(invminlength), Toast.LENGTH_SHORT).show();
                        return;
                    }

                    if(password1.length() > 0 && password1.equals(userentry) || password2.length() > 0 && password2.equals(userentry) ||
					password3.length() > 0 && password3.equals(userentry))
                    {
                        //Toast.makeText(getApplicationContext(), "PASSWORD CORRECT", Toast.LENGTH_SHORT).show();
                        //final_string = userentry;
                        //S1InputString.this.finish();
                    }
                    else
                    {
                        Toast.makeText(getApplicationContext(), "WRONG PASSWORD", Toast.LENGTH_SHORT).show();
                        userentry = "";
                        maskedentry = "";
                        textView_entry.setText(maskedentry);
                        return;
                    }

                    getTimerCancel();
					final_string = userentry;
					S1InputString.this.finish();
                }


                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();


                }
            }

        });
    }

    private void invalidLength(String lengthparam) {
        AlertDialog.Builder dialog=new AlertDialog.Builder(this);
        if(lengthparam.equals("maxlength"))
            dialog.setMessage("Maximum of " + String.valueOf(invmaxlength) + " digits only");
        else if(lengthparam.equals("minlength"))
            dialog.setMessage("Invalid input length");
        //dialog.setTitle("Dialog Box");
        dialog.setPositiveButton("OK",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog,
                                        int which) {
                        //Toast.makeText(getApplicationContext(),"Yes is clicked",Toast.LENGTH_LONG).show();

                    }
                });
        /*
        dialog.setNegativeButton("cancel",new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Toast.makeText(getApplicationContext(),"cancel is clicked",Toast.LENGTH_LONG).show();
            }
        });
        */
        AlertDialog alertDialog=dialog.create();

		alertDialog.getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
             WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE);

        alertDialog.show();

    }

    /**
     * 取消倒计时
     */
    public void getTimerCancel() {
        if(ctimer != null)
            ctimer.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart(int cTimeOut)
    {
        Log.i("ctimer", "Timer set at: " + cTimeOut);
        ctimer = new CountDownTimer(cTimeOut*1000, 1000) {

            @Override
            public void onTick(long millisUntilFinished) {
                Log.i("Timer", "Timer onTick");
            }

            @Override
            public void onFinish() {
                Log.i("Timer", "Timer onFinish");

                //etInputStr.clear();
                final_string = "TO";

                //startActivity(new Intent(S1InputString.this,MainActivity.class));

                Log.i("PATRICK", "Timeout UserInputString");
                S1InputString.this.finish();

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();
                }
            }
        }.start();

    }

    @Override
    public void onBackPressed() {
        //startActivity(new Intent(S1InputString.this,MainActivity.class));
        getTimerCancel();
        final_string = "CANCEL";
        S1InputString.this.finish();

        Log.i("PATRICK", "Back");

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();
        }

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        final_string = null;
        ctimer = null;
        SysApplication.getInstance().removeActivity(this);
    }

}
