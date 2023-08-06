package com.Source.S1_MCCPAY.MCCPAY;


import java.io.File;
import java.io.FileOutputStream;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.view.View;

public class BitmapView extends View
{
    private Matrix matrix = null;
    public BitmapView(Context context)
    {
        super(context);
    }
    public void onDraw(Canvas canvas)
    {
        // 獲取資源檔案的引用res
        Resources res = getResources();
        // 獲取圖形資源檔案
        Bitmap bmp = BitmapFactory.decodeResource(res, R.drawable.sign);
        // 設定canvas畫布背景為白色
        canvas.drawColor(Color.BLACK);
        canvas.drawBitmap(bmp, 0, 0, null);
        // 定義矩陣物件
        matrix = new Matrix();
        Bitmap bitmap = Bitmap.createBitmap(bmp, 0, 50, bmp.getWidth(), bmp.getHeight()/2,matrix, true);
        canvas.drawBitmap(bitmap, 0, 250, null);
        SaveBitmap(bitmap);
    }
    //儲存到本地
    public void SaveBitmap(Bitmap bmp)
    {
        Bitmap bitmap = Bitmap.createBitmap(800, 600, Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        //載入背景圖片
        Bitmap bmps = BitmapFactory.decodeResource(getResources(), R.drawable.sign);
        canvas.drawBitmap(bmps, 0, 0, null);
        //載入要儲存的畫面
        canvas.drawBitmap(bmp, 10, 100, null);
        //儲存全部圖層
        //canvas.save(Canvas.ALL_SAVE_FLAG);
        canvas.save();
        canvas.restore();
        //存儲路徑
        File file = new File("/mnt/sdcard/DCIM/");
        if(!file.exists())
            file.mkdirs();
        try {
            FileOutputStream fileOutputStream = new FileOutputStream(file.getPath() + "/gaozhen_08189.jpg");
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fileOutputStream);
            fileOutputStream.close();
            System.out.println("saveBmp is here");
        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}