package com.Source.S1_MCCPAY.MCCPAY.model;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.PowerManager;
import android.os.SystemClock;
import android.text.Editable;
import android.text.InputType;
import android.util.Log;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.PopupWindow;
import android.widget.ScrollView;

import com.Source.S1_MCCPAY.MCCPAY.CloseActivityClass;
import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;
import com.Source.S1_MCCPAY.MCCPAY.SysApplication;

import java.util.Random;

public class NormalKeyBoard extends DemoAppActivity {

	public static String user_num_str;

	private EditText editText;
	private Editable editable;
	private Button button;
	private String money = "";

	private PopupWindow popupWindow;
	private Button btn1,btn2,btn3,btn4,btn5,btn6,btn7,btn8,btn9,btn0,btn_del,btn_clear,btn_conf;
	private Button btn_f0, btn_f1, btn_f2, btn_f3, btn_f4;
    private Button btn_star, btn_hash;

	private String any_num = "";

	private ScrollView root;

	private int inMinLen = 1;
	private int inMaxLen = 12;
	private int inTimeOut = 20;
	private int inBypass = 0;

	protected PowerManager.WakeLock mWakeLock;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		CloseActivityClass.activityList.add(this);
		SysApplication.getInstance().addActivity(this);
		super.onCreate(savedInstanceState);
		Bundle bundle = getIntent().getExtras();


		overridePendingTransition(0, 0); // disable the animation, faster

		// TODO Auto-generated method stub

		setContentView(R.layout.normal_getnum);

		/* This code together with the one in onDestroy()
		 * will make the screen be always on until this Activity gets destroyed. */
		final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_MCCPAY:AAA-NormalKeyBoard>>WAKE_LOCK");
		this.mWakeLock.acquire();

		Intent intent=getIntent();
		String value=intent.getStringExtra("pass_in_string");
		Log.i("main pass in value", value);

		editText = (EditText) findViewById(R.id.n_num_edit);
		//button = (Button) findViewById(R.id.pay_button);
		root = (ScrollView) findViewById(R.id.root);

		//button.requestFocus();
		editable = editText.getEditableText();

		View keyboardView = LayoutInflater.from(this).inflate(R.layout.normal_keyboard, null);


        Display display = getWindowManager().getDefaultDisplay();
        //int height = (int) getResources().getDimension(R.dimen.rnd_btn_height);
        int height = 60;
        popupWindow = new PopupWindow(keyboardView,display.getWidth(),height*16,false);
        //int height = (int) getResources().getDimension(R.dimen.rnd_btn_height);
        //popupWindow = new PopupWindow(keyboardView,display.getWidth(), height,false);

		btn0 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn0);
		btn1 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn1);
		btn2 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn2);
		btn3 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn3);
		btn4 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn4);
		btn5 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn5);
		btn6 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn6);
		btn7 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn7);
		btn8 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn8);
		btn9 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn9);
		btn_del = (Button) keyboardView.findViewById(R.id.n_keyboard_btn_del);
		btn_clear = (Button) keyboardView.findViewById(R.id.n_keyboard_btn_clear);
		btn_conf = (Button) keyboardView.findViewById(R.id.n_keyboard_btn_conf);
		btn_f1 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn_f1);
		btn_f2 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn_f2);
		btn_f3 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn_f3);
		btn_f4 = (Button) keyboardView.findViewById(R.id.n_keyboard_btn_f4);

		/*Start timer*/
		getTimerRestart();

		btn0.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub

				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn0.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();

			}
		});
		btn1.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn1.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn2.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn2.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn3.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn3.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn4.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn4.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn5.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn5.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn6.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn6.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn7.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn7.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn8.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn8.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});
		btn9.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (any_num.length() >= inMaxLen)
					return;

				int index = editText.getSelectionStart();
				String str = btn9.getText().toString();
				editable.insert(index, str);
				//editable.insert(index, "*");
				any_num = any_num+str;
				getTimerCancel(); getTimerRestart();
			}
		});

		btn_f1.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getTimerCancel(); getTimerRestart();
				//cbNetsOnGetPINOtherKeys("F1");
			}
		});
		btn_f2.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getTimerCancel(); getTimerRestart();
				//cbNetsOnGetPINOtherKeys("F2");
			}
		});
		btn_f3.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getTimerCancel(); getTimerRestart();
				//cbNetsOnGetPINOtherKeys("F3");
			}
		});
		btn_f4.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getTimerCancel(); getTimerRestart();
				//cbNetsOnGetPINOtherKeys("F4");
			}
		});

		btn_del.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				try {
					int index = editText.getSelectionStart();
					editable.delete(index-1, index);
					any_num = any_num.substring(0, any_num.length()-1); // clear num str
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				getTimerCancel(); getTimerRestart();
				//cbNetsOnGetPINBackspace("DEL");
			}
		});
		btn_clear.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				editable.clear();
				any_num = "CLS";
				getTimerCancel();
				//cbNetsOnGetPINCancel("CLS");

				popupWindow.dismiss();
				//button.requestFocus();

				startActivity(new Intent(NormalKeyBoard.this,MainActivity.class));
                user_num_str = any_num.toString();

				Log.i("PATRICK", "FINISH NormalKeyBoard");
				NormalKeyBoard.this.finish();

				// do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
				synchronized (MainActivity.LOCK) {
					MainActivity.LOCK.setCondition(true);
					MainActivity.LOCK.notifyAll();

					//cbNetsOnGetPINEnterKeys(pin_num);

				}
			}
		});
		btn_conf.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getTimerCancel();

                /*check PIN len*/
				if (0 == inBypass && (any_num.length() < inMinLen || any_num.length() > inMaxLen))
				{
					getTimerRestart(); // PIN bypass not allowed, so reset the timer.
					return;
				}

				popupWindow.dismiss();
				//button.requestFocus();

                user_num_str = any_num.toString();
				if (any_num.length() == 0)
                    user_num_str = "BYPASS";

				//Toast.makeText(getApplicationContext(), pin_num2, Toast.LENGTH_SHORT).show();
				Log.i("PATRICK", user_num_str);
				Log.i("PATRICK", "Toast");
				startActivity(new Intent(NormalKeyBoard.this,MainActivity.class));
				//String filename = "PATRICK.TXT";

				//FileHelper fHelper = new FileHelper(getApplicationContext());

				//try {
				//	fHelper.save(filename, pin_num2);
				//} catch (Exception e) {
				//	e.printStackTrace();
				//}

				Log.i("PATRICK", "FINISH NormalKeyBoard");
				NormalKeyBoard.this.finish();

				// do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
				synchronized (MainActivity.LOCK) {
					MainActivity.LOCK.setCondition(true);
					MainActivity.LOCK.notifyAll();

					//cbNetsOnGetPINEnterKeys(pin_num);

				}
			}
		});
		//禁止EditText获得焦点后弹出系统键盘
		editText.setInputType(InputType.TYPE_NULL);
		editText.setOnFocusChangeListener(new OnFocusChangeListener() {
			@Override
			public void onFocusChange(View arg0, boolean arg1) {
				// TODO Auto-generated method stub
				if(arg1){
					//updateKeyBoard();
					popupWindow.showAsDropDown(editText,0,0);
					popupWindow.update();
				}
			}
		});

		new Handler().postDelayed(new Runnable() {

			public void run() {
				editText.dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), MotionEvent.ACTION_DOWN , 0, 0, 0));
				editText.dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), MotionEvent.ACTION_UP , 0, 0, 0));
			}
		}, 100);
	}
	private void updateKeyBoard(){
		int[] randomNum = getRandomNum();
		btn0.setText(randomNum[0]+"");
		btn1.setText(randomNum[1]+"");
		btn2.setText(randomNum[2]+"");
		btn3.setText(randomNum[3]+"");
		btn4.setText(randomNum[4]+"");
		btn5.setText(randomNum[5]+"");
		btn6.setText(randomNum[6]+"");
		btn7.setText(randomNum[7]+"");
		btn8.setText(randomNum[8]+"");
		btn9.setText(randomNum[9]+"");
	}

	private int[] getRandomNum(){
		Random random = new Random();
		int[] data = new int[10];
		boolean b;
		boolean b2 = false;
		boolean b3 = true;
		int x;
		for(int i=0;i<10;i++){
			b = true;
			while(b){
				x = random.nextInt(10);
				if(x==0 && b3){
					b3 = false;
					b = false;
				}
				for(int y : data){
					if(x==y){
						b2 = false;
						break;
					}else{
						b2 = true;
					}
				}
				if(b2){
					data[i] = x;
					b = false;
					break;
				}
			}

		}
		return data;
	}

	/**
	 * 取消倒计时
	 */
	public void getTimerCancel() {
		timer.cancel();
	}

	/**
	 * 开始倒计时
	 */
	public void getTimerRestart()
	{
		timer.start();
	}

	private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

		@Override
		public void onTick(long millisUntilFinished) {
			Log.i("Timer", "Timer onTick");
		}

		@Override
		public void onFinish() {
			Log.i("Timer", "Timer onFinish");

			editable.clear();
			any_num = "TO";

			popupWindow.dismiss();
			//button.requestFocus();

			//startActivity(new Intent(RandomKeyBoard.this,MainActivity.class));
            user_num_str = any_num.toString();

			Log.i("PATRICK", "FINISH RandomKeyBoard");
			NormalKeyBoard.this.finish();

			// do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
			synchronized (MainActivity.LOCK) {
				MainActivity.LOCK.setCondition(true);
				MainActivity.LOCK.notifyAll();
				//cbNetsOnGetPINEnterKeys(pin_num);
			}
		}
	};

	@Override
	protected void onDestroy() {
		this.mWakeLock.release();
		super.onDestroy();
        user_num_str = null;
		//NormalKeyBoard.getRefWatcher().watch(this);
		SysApplication.getInstance().removeActivity(this);
	}


}



















