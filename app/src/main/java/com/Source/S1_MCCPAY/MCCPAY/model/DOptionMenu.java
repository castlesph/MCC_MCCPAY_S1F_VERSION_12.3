package com.Source.S1_MCCPAY.MCCPAY.model;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;

public class DOptionMenu extends Activity {

    private boolean isFirstTrigger=true;
    private int flag=1;
    private Button button1=null;
    //private Button button2=null;
    private String TAG="Menu";

    private static Handler mHandler;

    public static String select_item;

    private String MenuTitle;
    private String FullMenuTimes;
    private String[] MenuItmes = new String[100];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate");

        overridePendingTransition(0, 0); // disable the animation, faster

        setContentView(R.layout.activity_option_menu);

        Intent intent=getIntent();
        String pass_in_value=intent.getStringExtra("pass_in_string");
        Log.i("main pass in value", pass_in_value);

        MenuTitle = pass_in_value.substring(0, pass_in_value.indexOf("|"));
        FullMenuTimes = pass_in_value.substring(pass_in_value.indexOf("|")+1);
        Log.i("MenuTitle", MenuTitle);
        Log.i("FullMenuTimes", FullMenuTimes);

        /*split out the menu items*/
        MenuItmes = FullMenuTimes.split(" \n");

        button1=(Button)findViewById(R.id.option_menu_button11);

        button1.setText(MenuTitle);

        button1.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {
                flag=1;
                /*if(Build.VERSION.SDK_INT>=11&&isFirstTrigger){
                    menuRefresh();
                    isFirstTrigger=false;
                }*/
                openOptionsMenu();
            }
        });

        /*
        button2=(Button)findViewById(R.id.menu_button12);
        button2.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {
                flag=2;
                if (Build.VERSION.SDK_INT >= 11&&isFirstTrigger) {
                    menuRefresh();
                    isFirstTrigger=false;
                }
                openOptionsMenu();
            }
        });
        */

        /*don't want change this sample code, so set auto click.*/
        new Handler().postDelayed(new Runnable() {

            public void run() {
                //button1.dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), MotionEvent.ACTION_BUTTON_PRESS , 0, 0, 0));
                //button1.dispatchTouchEvent(MotionEvent.obtain(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), MotionEvent.ACTION_BUTTON_RELEASE , 0, 0, 0));
                button1.callOnClick();
            }
        }, 100);
    }
    /*调用invalidateOptionsMenu会重新触发onCreateOptionsMenu和onPrepareOptionsMenu方法*/
    @SuppressLint("NewApi")
    public void menuRefresh() {
        if (Build.VERSION.SDK_INT >= 11) {//手机或者Emulator的sdk版本
            invalidateOptionsMenu();
        }
    }
    @Override
    /* 1. 当手机(Emulator)sdk版本>=11（如我的手机Android Version是4.1.1, Build.VERSION.SDK_INT是16）
     * 在创建Activity时触发。
     * 2. 当手机(Emulator)sdk版本<11(如我的手机Android Version是2.3.4，Build.VERSION.SDK_INT是10)
     * 在第一次单击Menu时触发。
     */
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "onCreateOptionsMenu");

        //getMenuInflater().inflate(R.menu.option_menu, menu);
        /*
        menu.add(0, 0, 0, "菜单1");
        menu.add(0, 1, 1, "菜单2");
        menu.add(0, 2, 2, "菜单3");
        menu.add(0, 3, 3, "菜单4");
        */
        for (int i = 0; i < MenuItmes.length; i++) {
            System.out.println(MenuItmes[i]);
            menu.add(0, i, i, MenuItmes[i]);
        }

        return true;
    }
    /* 1. 当手机(Emulator)sdk版本>=11（如我的手机Android Version是4.1.1, Build.VERSION.SDK_INT是16）
     * 在创建Activity触发onCreateOptionsMenu后触发，
     * 第一次单击Menu时不触发，之后每次单机Menu时触发。
     * 2. 当手机(Emulator)sdk版本<11(如我的手机Android Version是2.3.4，Build.VERSION.SDK_INT是10)
     * 在每次单击Menu时触发。
     * */
    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        Log.i(TAG, "onPrepareOptionsMenu");

        /*
        menu.findItem(0).setVisible(false);
        menu.findItem(1).setVisible(false);
        menu.findItem(2).setVisible(false);
        menu.findItem(3).setVisible(false);
        if(flag==1){
            menu.findItem(0).setVisible(true);
            menu.findItem(1).setVisible(true);
        }else {
            menu.findItem(2).setVisible(true);
            menu.findItem(3).setVisible(true);
        }
        */

        return super.onPrepareOptionsMenu(menu);
    }
    /*选择MENU里面的内容后触发*/
    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        Log.i(TAG, "onOptionsItemSelected");

        /*
        switch (item.getItemId()) {
            case 0:
                Toast.makeText(this, "菜单1", Toast.LENGTH_LONG)
                        .show();
                break;
            case 1:
                Toast.makeText(this, "菜单2", Toast.LENGTH_LONG)
                        .show();
                break;
            case 2:
                Toast.makeText(this, "菜单3", Toast.LENGTH_LONG)
                        .show();
                break;
            case 3:
                Toast.makeText(this, "菜单4", Toast.LENGTH_LONG)
                        .show();
                break;
            default:
                break;
        }
        */

        select_item = String.valueOf(item.getItemId());

        startActivity(new Intent(DOptionMenu.this,MainActivity.class));

        Log.i("PATRICK", "FINISH DMenuList");
        DOptionMenu.this.finish();

        // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
        synchronized (MainActivity.LOCK) {
            MainActivity.LOCK.setCondition(true);
            MainActivity.LOCK.notifyAll();

        }

        return true;
    }

    /*This part of code is simple popup menu*/
    /*
    public void onPopupButtonClick(View button)
    {
        // 创建PopupMenu对象
        popup = new PopupMenu(this, button2);
        // 将R.menu.popup_menu菜单资源加载到popup菜单中
        getMenuInflater().inflate(R.menu.popup_menu, popup.getMenu());

        // 为popup菜单的菜单项单击事件绑定事件监听器
        popup.setOnMenuItemClickListener(
                new PopupMenu.OnMenuItemClickListener()
                {
                    @Override
                    public boolean onMenuItemClick(MenuItem item)
                    {
                        switch (item.getItemId())
                        {
                            case R.id.exit:
                                // 隐藏该对话框
                                popup.dismiss();
                                break;
                            default:
                                // 使用Toast显示用户单击的菜单项
                                Toast.makeText(DOptionMenu.this,
                                        "您单击了【" + item.getTitle() + "】菜单项"
                                        , Toast.LENGTH_SHORT).show();
                        }
                        return true;
                    }
                });
        popup.show();
    }
    */

    }