package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.content.Intent;
import android.os.Bundle;
import android.content.Context;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.PayCheck;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;
import com.Source.S1_MCCPAY.MCCPAY.model.DemoAppActivity;

public class NETSprompt_refno extends DemoAppActivity {

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
    int total_price;

    boolean androidThinking;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();
        total_price = bundle.getInt("total_price");

        Toast.makeText(getApplicationContext(), String.valueOf(total_price), Toast.LENGTH_SHORT).show();

        //EditText etRefNo;
        //final TextView tv1;
        //Double dRefNo;

        /*LayoutInflater mInflater = (LayoutInflater) getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
        View view= (View) mInflater.inflate(R.layout.prompt_refno, null);
        ConstraintLayout.LayoutParams p = new ConstraintLayout.LayoutParams(
                ConstraintLayout.LayoutParams.WRAP_CONTENT,
                ConstraintLayout.LayoutParams.WRAP_CONTENT);
        p.setMargins(5, 5, 5, 5);*/ //ok till here
        //layout.setGravity(Gravity.CENTER_HORIZONTAL); compile error cannot find setGravity
        //layout.addView(view, p);

        //super.onCreate(Bundle savedInstanceState);
        setContentView(R.layout.prompt_refno_main);

        tvMessage = (TextView) findViewById(R.id.MessageLabel);

        etAmount = (EditText) findViewById(R.id.RefNoInput);
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(etAmount, InputMethodManager.SHOW_IMPLICIT);
        etAmount.setTransformationMethod(null);
        //etAmount.setOnEditorActionListener(new DoneOnEditorActionListener());
        //etAmount.requestFocus();
        //getDialog().getWindow().setSoftInputMode(LayoutParams.SOFT_INPUT_STATE_VISIBLE);

        /*etAmount.setOnFocusChangeListener(new OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
            etAmount.post(new Runnable() {
                @Override
                public void run() {
                    //InputMethodManager imm = (InputMethodManager) getActivity().getSystemService(Context.INPUT_METHOD_SERVICE);
                    InputMethodManager imm = (InputMethodManager) this.getSystemService(Context.INPUT_METHOD_SERVICE);
                    imm.showSoftInput(etAmount, InputMethodManager.SHOW_IMPLICIT);
                }
            });
            }
        });
        etAmount.requestFocus();*/

        buOK = (Button) findViewById(R.id.OKButton);
        buCancel = (Button) findViewById(R.id.CancelButton);



        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_VISIBLE);

        buOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                Intent intent = new Intent();
                Bundle bundle = new Bundle();
                intent.setClass(NETSprompt_refno.this, PayCheck.class);

                Toast.makeText(getApplicationContext(), String.valueOf(total_price), Toast.LENGTH_SHORT).show();

                bundle.putInt("total_price", total_price);

                intent.putExtras(bundle);
                startActivity(intent);
                finish();

                inResult = 0;
                return;
            }
        });

        buCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //vdNETS_DisplayTxnCancelledExit();

                startActivity(new Intent(NETSprompt_refno.this,MainActivity.class));
                NETSprompt_refno.this.finish();
            }
        });
    }

    @Override
    public void onBackPressed() {

    }

}




