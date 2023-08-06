package com.Source.S1_MCCPAY.MCCPAY.model;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.TextView;

import com.Source.S1_MCCPAY.MCCPAY.R;

import java.io.File;
import java.util.ArrayList;

public class TxnPopupMenu {

    private Context context;

    private ArrayList<TxnPopupMenu.Item> itemList;

    private PopupWindow popupWindow;

    private ListView listView;


    public static class Item {
        public String text;
        public int id;

        public Item(String text, int id) {
            this.text = ("/data/data/pub/"+text);
            this.id = id;
        }
    }

    @SuppressWarnings("deprecation")
    public TxnPopupMenu(Context context) {

        this.context = context;
        itemList = new ArrayList<TxnPopupMenu.Item>(20);  //Set MAX Menu items
        View view = LayoutInflater.from(context).inflate(R.layout.mcc_txn_popup_menu, null);

        //设置 listview
        listView = (ListView)view.findViewById(R.id.mcc_popup_view_listView);
        listView.setAdapter(new TxnPopupMenu.PopAdapter());
        listView.setFocusableInTouchMode(true);
        listView.setFocusable(true);

        //popupWindow = new PopupWindow(view, 300, LayoutParams.WRAP_CONTENT);
        popupWindow = new PopupWindow(view, 700, ViewGroup.LayoutParams.WRAP_CONTENT);

        // 这个是为了点击“返回Back”也能使其消失，并且并不会影响你的背景（很神奇的）
        popupWindow.setBackgroundDrawable(new BitmapDrawable());
    }

    //设置菜单项点击监听器
    public void setOnItemClickListener(AdapterView.OnItemClickListener listener) {
        listView.setOnItemClickListener(listener);
    }

    //批量添加菜单项 -- add menu items in bulk
    public void addItems(TxnPopupMenu.Item[] items) {
        for (TxnPopupMenu.Item s : items)
            itemList.add(s);
    }

    //单个添加菜单项 -- single add menu item
    public void addItem(TxnPopupMenu.Item item) {
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
            TxnPopupMenu.PopAdapter.ViewHolder holder;
            if (convertView == null) {
                convertView = LayoutInflater.from(context).inflate(R.layout.mcc_popmenu_item, null);
                holder = new TxnPopupMenu.PopAdapter.ViewHolder();

                convertView.setTag(holder);

                //holder.groupItem = (TextView) convertView.findViewById(R.id.mcc_pop_item_header);
                holder.groupImg = (ImageView) convertView.findViewById(R.id.mcc_pop_item_img);

            } else {
                holder = (TxnPopupMenu.PopAdapter.ViewHolder) convertView.getTag();
            }

            File txnlogo_bmp_file = new File(itemList.get(position).text);
            Bitmap txnlogo = BitmapFactory.decodeFile(txnlogo_bmp_file.getAbsolutePath());

            //holder.groupItem.setText(itemList.get(position).text);
            //holder.groupImg.setBackgroundResource(itemList.get(position).id);
            holder.groupImg.setImageBitmap(txnlogo);
            return convertView;
        }

        private final class ViewHolder {
            ImageView groupImg;
            //TextView groupItem;
        }
    }
}
