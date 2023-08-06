package com.Source.S1_MCCPAY.MCCPAY.model;

import java.util.ArrayList;
import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;
import com.Source.S1_MCCPAY.MCCPAY.R;


/**
 * 自定义弹出菜单
 * @author guopeng
 * @created 2015年11月27日
 */
public class DPopupMenu {

    private Context context;

    private ArrayList<Item> itemList;

    private PopupWindow popupWindow;

    private ListView listView;


    public static class Item {  
        public String text;  
        public int id;  
  
        public Item(String text, int id) {  
            this.text = text;  
            this.id = id;  
        }  
    }  
    
    @SuppressWarnings("deprecation")
    public DPopupMenu(Context context) {
        
        this.context = context;
        itemList = new ArrayList<Item>(20);  //Set MAX Menu items
        View view;

		if(MainActivity.inTermType == 5)
            view = LayoutInflater.from(context).inflate(R.layout.popup_menu_s1em, null);
        else
			view = LayoutInflater.from(context).inflate(R.layout.popup_menu, null);
        
        //设置 listview
        listView = (ListView)view.findViewById(R.id.popup_view_listView);
        listView.setAdapter(new PopAdapter());
        listView.setFocusableInTouchMode(true);
        listView.setFocusable(true);
        
        //popupWindow = new PopupWindow(view, 300, LayoutParams.WRAP_CONTENT);
        popupWindow = new PopupWindow(view, 700,LayoutParams.WRAP_CONTENT);
        
        // 这个是为了点击“返回Back”也能使其消失，并且并不会影响你的背景（很神奇的）
        popupWindow.setBackgroundDrawable(new BitmapDrawable());
    }

    //设置菜单项点击监听器
    public void setOnItemClickListener(OnItemClickListener listener) {
        listView.setOnItemClickListener(listener);
    }

    //批量添加菜单项
    public void addItems(Item[] items) {
        for (Item s : items)
            itemList.add(s);
    }

    //单个添加菜单项
    public void addItem(Item item) {
        itemList.add(item);
    }

    //下拉式 弹出 pop菜单 parent 右下角
    public void showAsDropDown(View parent) {
        popupWindow.showAsDropDown(parent, 10, 
                //保证尺寸是根据屏幕像素密度来的
                //context.getResources().getDimensionPixelSize(R.dimen.popmenu_yoff));
                50);
        
        // 使其聚集
        popupWindow.setFocusable(false);
        // 设置允许在外点击消失 true or false
        //popupWindow.setOutsideTouchable(true);
        popupWindow.setOutsideTouchable(false);
        //刷新状态
        popupWindow.update();
    }
    
    //隐藏菜单
    public void dismiss() {
        popupWindow.dismiss();
    }

    // 适配器
    private final class PopAdapter extends BaseAdapter {

        @Override
        public int getCount() {
            return itemList.size();
        }

        @Override
        public Object getItem(int position) {
            return itemList.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ViewHolder holder;
            if (convertView == null) {
                convertView = LayoutInflater.from(context).inflate(R.layout.popmenu_item, null);
                holder = new ViewHolder();

                convertView.setTag(holder);

                holder.groupItem = (TextView) convertView.findViewById(R.id.pop_item_header);
                holder.groupImg = (ImageView) convertView.findViewById(R.id.pop_item_img);

            } else {
                holder = (ViewHolder) convertView.getTag();
            }

            holder.groupItem.setText(itemList.get(position).text);
            holder.groupImg.setBackgroundResource(itemList.get(position).id);
            return convertView;
        }

        private final class ViewHolder {
            ImageView groupImg;
            TextView groupItem;
        }
    }
}