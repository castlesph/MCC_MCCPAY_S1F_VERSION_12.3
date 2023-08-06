package com.Source.S1_MCCPAY.MCCPAY.model;

import android.app.Activity;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.RelativeLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

public class VKeyPad
	{
		Object syncToken = new Object();

		private static final byte VKBD_0 = '0';    // 0x30;
		private static final byte VKBD_1 = '1';    // 0x31;
		private static final byte VKBD_2 = '2';    // 0x32;
		private static final byte VKBD_3 = '3';    // 0x33;
		private static final byte VKBD_4 = '4';    // 0x34;
		private static final byte VKBD_5 = '5';    // 0x35;
		private static final byte VKBD_6 = '6';    // 0x36;
		private static final byte VKBD_7 = '7';    // 0x37;
		private static final byte VKBD_8 = '8';    // 0x38;
		private static final byte VKBD_9 = '9';    // 0x39;
		private static final byte VKBD_ENTER = 'A';    // 0x41;
		private static final byte VKBD_CLEAR = 'R';    // 0x52;
		private static final byte VKBD_CANCEL = 'C';    // 0x43;
		private static final byte VKBD_SPACE = 'S';    // 0x53;

		private byte[] KeyPadBody;
		private int [][] KeyPadLocation;
		private TextView[] textView;
		boolean isViewCreated = false;

		public VKeyPad(final Activity activity, final RelativeLayout layout)
		{
			KeyPadLocation = new int[16][5];

			KeyPadBody = new byte[]{
					VKBD_1, VKBD_2, VKBD_3,
            		VKBD_4, VKBD_5, VKBD_6,
            		VKBD_7, VKBD_8, VKBD_9,
            		VKBD_0,
            		VKBD_CLEAR, VKBD_ENTER, VKBD_CANCEL,
            		VKBD_SPACE, VKBD_SPACE, VKBD_SPACE
			};

			textView = new TextView[16];
			for(int i = 0;i < textView.length;i ++)
			{
				textView[i] = new TextView(activity);
				textView[i].setGravity(Gravity.CENTER);
				textView[i].setTextColor(Color.DKGRAY);
				textView[i].setHeight(130);
				textView[i].setWidth(130);
			}

			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {

					RelativeLayout.LayoutParams parentlayoutParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);//wigth, height
					parentlayoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);

					TableLayout.LayoutParams tableParams = new TableLayout.LayoutParams(TableLayout.LayoutParams.WRAP_CONTENT, TableLayout.LayoutParams.WRAP_CONTENT);



					TableLayout tableLayout = new TableLayout(activity);
					tableLayout.setLayoutParams(parentlayoutParams);
					tableLayout.setGravity(Gravity.CENTER);

					int k = 0;
					int index = 0;
					int count = 0;
					int retrieveType = 0;
					for(int i = 0;i < 5;i ++)
					{
						TableRow tableRow = new TableRow(activity);
						tableRow.setGravity(Gravity.CENTER);
						tableRow.setLayoutParams(tableParams);

						for(int j = 0;j < 3;j ++)
						{
							TableRow.LayoutParams rowParams = new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT);

							rowParams.setMargins(0, 0, 0, 0);

							GradientDrawable gd = new GradientDrawable();
							gd.setColor(0xFFFFFFFF); // Changes this drawbale to use a single color instead of a gradient
							gd.setStroke(3, Color.DKGRAY);
							gd.setShape(GradientDrawable.OVAL);
							//gd.setShape(GradientDrawable.RECTANGLE);
							gd.setCornerRadius(20);

							index = i * 3 + j;

							Animation am = new AlphaAnimation(0.0f, 1.0f);
							am.setDuration((index)*300);

							if(index > 15)
								continue;
							Log.d("index",index + "");
							textView[index].setLayoutParams(rowParams);

							retrieveType = 4;
							if(index == 12)
							{
								retrieveType = 1;
							}
							else if(index == 9)
							{
								retrieveType = 2;
							}
							else if(index == 11)
							{
								retrieveType = 3;
							}
							else if( (0 <= index && index <= 8) || (index == 10) )
							{
								retrieveType = 0;
							}

							switch(retrieveType)
							{
								case 0:
									textView[index].setText(new String(new byte [] {KeyPadBody[count++]}));
									textView[index].setTextSize(35);
									textView[index].setBackground(gd);
									break;
								case 1:
									textView[index].setText("Cancel");
									textView[index].setTextSize(20);
									break;
								case 2:
									textView[index].setText("Clear");
									textView[index].setTextSize(20);
									break;
								case 3:
									textView[index].setText("Enter");
									textView[index].setTextSize(20);
									break;

								default :
									textView[index].setText(" ");
									break;
							}
							textView[index].setAnimation(am);

							if(textView[index].getParent()!=null)
								((ViewGroup)textView[index].getParent()).removeView(textView[index]);
							tableRow.addView(textView[index]);
						}
						tableLayout.addView(tableRow);
					}

                RelativeLayout.LayoutParams cancelTextViewParentParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);//wigth, height
                cancelTextViewParentParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);
                cancelTextViewParentParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, RelativeLayout.TRUE);
                cancelTextViewParentParams.setMargins(0, 0, 0, 0);

                textView[12].setLayoutParams(cancelTextViewParentParams);
                textView[12].setText(new String("Cancel"));
                textView[12].setTextSize(20);
                textView[12].setGravity(Gravity.CENTER);
                textView[12].setTextColor(Color.GRAY);
                textView[12].setHeight(70);
                textView[12].setWidth(140);


                layout.addView(tableLayout);
                if(textView[12].getParent()!= null)
                	((ViewGroup)textView[12].getParent()).removeView(textView[12]);
                layout.addView(textView[12]);
                isViewCreated = true;
				}
			});

			ViewTreeObserver observer = layout.getViewTreeObserver();
			observer.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
				@Override
				public void onGlobalLayout()
				{
					// TODO Auto-generated method stub
					synchronized(KeyPadLocation)
					{
						for (int i = 0; i < textView.length; i++)
						{
							int[] location = new int[2];
							textView[i].getLocationOnScreen(location);
							int x = location[0];
							int y = location[1];
							Log.d("location x = ", String.valueOf(location[0]));
							Log.d("location y = ", String.valueOf(location[1]));
							Log.d("tv.getWidth() = ", String.valueOf(textView[i].getWidth()));
							Log.d("tv.getHeight() = ", String.valueOf(textView[i].getHeight()));

							KeyPadLocation[i][0] = x;
							KeyPadLocation[i][1] = y;
							KeyPadLocation[i][2] = x + textView[i].getWidth();
							KeyPadLocation[i][3] = y + textView[i].getHeight();

							String s = textView[i].getText().toString();
							byte key;
							if(s.equals("Enter") == true)
							{
								key = VKBD_ENTER;
							}
							else if(s.equals("Clear") == true)
							{
								key = VKBD_CLEAR;
							}
							else if(s.equals("Cancel") == true)
							{
								key = VKBD_CANCEL;
							}
							else if(s.equals(" ") == true)
							{
								key = VKBD_SPACE;
							}
							else
							{
								if(s == null)
								{
									Log.d("s", "null");
									key = 'S';
								}
								else if(s.length() > 0)
								{
									Log.d("s", s.toString());
									key = (s.getBytes())[0];
								}
								else
								{
									key = 'S';
								}
							}

							KeyPadLocation[i][4] = key;

						}
					}

					if (isKeyPadLocationChanged())
					{
						synchronized (syncToken)
						{
							syncToken.notify();
						}
					}
				}
			});
		}

		private boolean isKeyPadLocationChanged()
		{
			synchronized(KeyPadLocation)
			{
				for (int[] row : KeyPadLocation)
				{
					for (int location : row)
						if (location != 0)
							return true;
				}
				return false;
			}
		}


		/*
		public byte[] getKeyPadBody()
		{
			return KeyPadBody;
		}
		*/

		public void removeAllView(final Activity activity)
		{
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					((TableLayout)((TableRow)textView[0].getParent()).getParent()).removeAllViews();
					((RelativeLayout)textView[12].getParent()).removeView(textView[12]);
				}
			});
		}

		public int[][] getKeyPadLocation()
		{
			if(!isKeyPadLocationChanged())
			{
				synchronized(syncToken)
				{
					try {
						syncToken.wait();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
			return KeyPadLocation;
		}
	}