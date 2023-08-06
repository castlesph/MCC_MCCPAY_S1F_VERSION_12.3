package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.content.Intent;
import android.os.Bundle;
import android.content.Context;
import android.icu.math.BigDecimal;
import android.icu.text.NumberFormat;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

public class NETSprompt_amount extends DemoAppActivity {

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

    boolean androidThinking;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        setContentView(R.layout.prompt_amount_main);

        tvMessage = (TextView) findViewById(R.id.MessageLabel);

        etAmount = (EditText) findViewById(R.id.AmountInput);
        //etAmount.setSelection(etAmount.getText().length()); //move bottom can work

        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(etAmount, InputMethodManager.SHOW_IMPLICIT);

        etAmount.setTransformationMethod(null); //set android:inputType="numberPassword" in xml and set this to display numeric only
        etAmount.setOnEditorActionListener(new DoneOnEditorActionListener());
        etAmount.setSelection(etAmount.getText().length());

        buOK = (Button) findViewById(R.id.OKButton);
        buCancel = (Button) findViewById(R.id.CancelButton);

        stResult = this.getJniString();
        //this.getJniString1();
		//this.getJniString();
			
		etAmount.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void afterTextChanged(Editable editable) {
                if (etAmount == null) return;
                String inputString = editable.toString();
                etAmount.removeTextChangedListener(this);
                String cleanString = inputString.toString().replaceAll("[$,.]", "");
                //String cleanString = inputString.toString();
                BigDecimal bigDecimal = new BigDecimal(cleanString).setScale(2, BigDecimal.ROUND_FLOOR).divide(new BigDecimal(100), BigDecimal.ROUND_FLOOR);
                //String  converted = NumberFormat.getCurrencyInstance().format(bigDecimal);
                String  converted = NumberFormat.getNumberInstance().format(bigDecimal);
                etAmount.setText(converted);
                etAmount.setSelection(converted.length());
                etAmount.addTextChangedListener(this);
            }
        });

        buOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dAmount = Double.parseDouble(etAmount.getText().toString());

                /* this is working
                stResult = etAmount.getText().toString();
                tvMessage.setText(stResult);
                tvMessage.setVisibility(View.VISIBLE);
                */

                //this is working
                //setContentView(R.layout.prompt_refno);
                Intent intent = new Intent();
                Bundle bundle = new Bundle();
                intent.setClass(NETSprompt_amount.this, NETSprompt_refno.class);

                dAmount = Double.parseDouble(etAmount.getText().toString());

                //first convert the Double to String
                String s = String.valueOf(dAmount);
                // remove all . (dots) from the String
                String str = s.replace(".", "");
                //Convert the string back to int
                int total_price = Integer.parseInt(str);
                // here i becomes 4556

                Toast.makeText(getApplicationContext(), str, Toast.LENGTH_SHORT).show();

                bundle.putInt("total_price", total_price);
                intent.putExtras(bundle);
                startActivity(intent);
                finish();
/*
                inNETS_InputStoreInvNum();
                if (inResult != 0)
                {
                    vdNETS_DisplayTxnCancelledExit();
                }
                else
                {

                    stResult = etAmount.getText().toString();
                    tvMessage.setText(stResult);
                    tvMessage.setVisibility(View.VISIBLE);
                }
*/
            }
        });

        buCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ///tvMessage.setText("Transaction Cancelled");
                //((TextView) findViewById(R.id.MessageLabel)).setText( "Transaction Cancelled");
                //setContentView(R.layout.prompt_amount);
                /*final Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        tvMessage.setText("Transaction Cancelled");
                    }
                }, 2000);*/
                /*final Handler handler = new Handler();
                Timer t = new Timer();
                t.schedule(new TimerTask() {
                    public void run() {
                        handler.post(new Runnable() {
                            public void run() {
                                tvMessage.setText("Transaction Cancelled");
                            }
                        });
                    }
                }, 2000);*/
                /*Timer timer = new Timer();
                timer.schedule(new TimerTask() {
                    @Override
                    public void run() {
                        tvMessage.setText("Transaction Cancelled");
                    }
                }, 2000);*/
                /*final Handler handler = new Handler();
                Runnable runnable = new Runnable() {
                    @Override
                    public void run() {
                        tvMessage.setText("Transaction Cancelled");
                        //handler.postDelayed(this, 2000);
                    }
                };
                handler.postDelayed(runnable, 2000);*/
                vdNETS_DisplayTxnCancelledExit();

                startActivity(new Intent(NETSprompt_amount.this,MainActivity.class));
                NETSprompt_amount.this.finish();

                //tvMessage.setText("Transaction Cancelled");
                ///finish();
                ///System.exit(0); // if add this cannot see text
            }
        });

        /*mButton = (Button) findViewById(R.id.OKButton);
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                LayoutInflater layoutInflaterAndroid = LayoutInflater.from(c);
                View mView = layoutInflaterAndroid.inflate(R.layout.prompt_refno, null);
                //AlertDialog.Builder alertDialogBuilderUserInput = new AlertDialog.Builder(c);
                //alertDialogBuilderUserInput.setView(mView);

                final EditText userInputDialogEditText = (EditText) mView.findViewById(R.id.AmountInput);
                alertDialogBuilderUserInput
                        .setCancelable(false)
                        .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialogBox, int id) {
                                // ToDo get user input here
                            }
                        })

                        .setNegativeButton("Cancel",
                                new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialogBox, int id) {
                                        main.cancel();
                                    }
                                });

                AlertDialog alertDialogAndroid = alertDialogBuilderUserInput.create();
                alertDialogAndroid.show();
            }
        });*/
    }

    public void vdNETS_DisplayTxnCancelledExit() {
    /*
        tvMessage.setText("Transaction Cancelled");
        tvMessage.setVisibility(View.VISIBLE);
        androidThinking = true;    //explained below
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                tvMessage.setVisibility(View.GONE);
                finish();
                //CODE  for android's turn
                androidThinking = false;  //explained below
            }
        }, 2000);
     */
    }

    @Override
    public void onBackPressed() {

    }

    public String messageNETSprompt_refno(String text) {
        System.out.println(text);
        Log.i("Castles", text);

        dAmount = Double.parseDouble(etAmount.getText().toString());

        Intent intent = new Intent();
        Bundle bundle = new Bundle();
        //intent.setClass(NETSprompt_amount.this, NETSprompt_refno.class);
		intent.setClass(getApplicationContext(), NETSprompt_refno.class);

        dAmount = Double.parseDouble(etAmount.getText().toString());

        //first convert the Double to String
        String s = String.valueOf(dAmount);
        // remove all . (dots) from the String
        String str = s.replace(".", "");
        //Convert the string back to int
        int total_price = Integer.parseInt(str);
        // here i becomes 4556

        Toast.makeText(getApplicationContext(), str, Toast.LENGTH_SHORT).show();

        bundle.putInt("total_price", total_price);
        intent.putExtras(bundle);
        startActivity(intent);
        finish();

        return text;
    }

    public native String getJniString();

}

