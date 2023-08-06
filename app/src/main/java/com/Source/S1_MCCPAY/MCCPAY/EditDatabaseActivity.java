package com.Source.S1_MCCPAY.MCCPAY;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.db.AndroidDatabaseManager;

public class EditDatabaseActivity extends AppCompatActivity implements View.OnClickListener{
    Button btnterminal;
    Button btnwave;
    Button btnenv;
    Button btnexit;

    private static final String TAG = EditDatabaseActivity.class.getSimpleName();
    private String TerminalDb = "/data/data/pub/TERMINAL.S3DB";
    private String WaveDb = "/data/data/pub/WAVE.S3DB";
    private String EmvDb = "/data/data/pub/EMV.S3DB";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_database);

        btnterminal = (Button) findViewById(R.id.btn_edit_terminal);
        btnwave = (Button) findViewById(R.id.btn_edit_wave);
        btnenv = (Button) findViewById(R.id.btn_edit_env);
        btnexit = (Button) findViewById(R.id.btn_exit);

        btnterminal.setOnClickListener(this);
        btnwave.setOnClickListener(this);
        btnenv.setOnClickListener(this);
        btnexit.setOnClickListener(this);


    }

    @Override
    public void onClick(View view) {
        Intent intent = new Intent();
        Bundle bundle = new Bundle();

        switch (view.getId())
        {
            case R.id.btn_edit_terminal:
                Log.d(TAG, "btn_edit_terminal");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", TerminalDb);
                intent.putExtras(bundle);
                startActivity(intent);
                break;
            case R.id.btn_edit_wave:
                Log.d(TAG, "btn_edit_wave");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", WaveDb);
                intent.putExtras(bundle);
                startActivity(intent);
                break;
            case R.id.btn_edit_env:
                Log.d(TAG, "btn_edit_env");
                intent.setClass(EditDatabaseActivity.this, AndroidDatabaseManager.class);
                bundle.putString("dbname", EmvDb);
                intent.putExtras(bundle);
                startActivity(intent);
                break;
            case R.id.btn_exit:
                Log.d(TAG, "btn_edit_env");
                intent.setClass(EditDatabaseActivity.this, MainActivity.class);
                startActivity(intent);
                break;

        }



    }
}
