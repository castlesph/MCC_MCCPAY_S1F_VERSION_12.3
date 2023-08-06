package com.Source.S1_MCCPAY.MCCPAY.Trans;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.support.annotation.NonNull;
import android.support.constraint.ConstraintLayout;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import com.Source.S1_MCCPAY.MCCPAY.R;

// sidumili: Adapter for detail transaction
public class TransDetailAdapter extends RecyclerView.Adapter<TransDetailAdapter.MyViewHolder> {

    String data1[], data2[], data3[], data4[], data5[], data6[], data7[], data8[], data9[], data10[], data11[];
    Context context;
    private int lastPosition = -1;
    int row_index = -1;
    public static String final_rb_selected_string = "";

    public TransDetailAdapter(Context ct, String s1[], String s2[], String s3[], String s4[], String s5[], String s6[], String s7[], String s8[], String s9[], String s10[], String s11[]){
        context = ct;
        data1 = s1;
        data2 = s2;
        data3 = s3;
        data4 = s4;
        data5 = s5;
        data6 = s6;
        data7 = s7;
        data8 = s8;
        data9 = s9;
        data10 = s10;
        data11 = s11;
    }

    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int i) {

        LayoutInflater inflater = LayoutInflater.from(context);
        View view = inflater.inflate(R.layout.row_retrieve_layout, parent, false);

        return new MyViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull final MyViewHolder holder, @SuppressLint("RecyclerView") final int position) {
       holder.myText1.setText(data1[position]);
       holder.myText2.setText(data2[position]);
       holder.myText5.setText(data5[position]);
       holder.myText6.setText(data6[position]);

        holder.myText3.setText(data3[position]); // RefundNo
        holder.myText7.setText(data9[position]); // InvoiceNo
        holder.myText8.setText(data11[position]); // Voided

       // Set image
        Log.i("data8[position]", data8[position]);
       if (data8[position].length() > 0)
       {
           int iIndex = Integer.parseInt(data8[position]) - 1; // start with index 0
           holder.myImage.setImageResource(TransDetailListView.images[iIndex]); // access public variable
       }

        // Select item
        holder.mainLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                row_index = position;
                notifyDataSetChanged();

                String sOutTradeNo = data2[position];
                String sOutRefundNo = data3[position];
                String sInvoiceNo = data9[position];
                String sQPresented = data10[position];
                String sVoided =  data11[position];
                String sTotalFee = data6[position].replaceAll("[PHP,$,.]", "");

                final_rb_selected_string = "";
                final_rb_selected_string = sOutTradeNo + "^" + sTotalFee + "^" + sOutRefundNo + "^" + sInvoiceNo + "^" + sQPresented + "^" + sVoided; // out_trade_no^amount
            }
        });

        Log.i("sidumili", "onBindViewHolder: row_index="+row_index+",position="+position);
       // Set backcolor
       if (row_index == position)
       {
           Log.i("sidumili", "onBindViewHolder: Make backgroound selected");
           //holder.mainLayout.setBackgroundColor(Color.parseColor("#90CAF9"));
           holder.mainLayout.setBackgroundResource(R.drawable.shape_selected);
       }
       else
       {
           Log.i("sidumili", "onBindViewHolder: Make backgroound default");
           //holder.mainLayout.setBackgroundColor(Color.parseColor("#E3F2FD"));
           holder.mainLayout.setBackgroundResource(R.drawable.shape_gray);
       }
    }

    @Override
    public int getItemCount() {
        return data1.length;
    }

    public class MyViewHolder extends RecyclerView.ViewHolder {

        TextView myText1, myText2, myText5, myText6;
        TextView myText3, myText7, myText8;
        ImageView myImage;
        ConstraintLayout mainLayout;

        public MyViewHolder(@NonNull View itemView) {
            super(itemView);

            myText1 = (TextView) itemView.findViewById(R.id.myTextView1);
            myText2 = (TextView) itemView.findViewById(R.id.myTextView2);
            myText5 = (TextView) itemView.findViewById(R.id.myTextView5);
            myText6 = (TextView) itemView.findViewById(R.id.myTextView6);

            myText3 = (TextView) itemView.findViewById(R.id.myTextView3);
            myText7 = (TextView) itemView.findViewById(R.id.myTextView7);
            myText8 = (TextView) itemView.findViewById(R.id.myTextView8);

            myImage = (ImageView) itemView.findViewById(R.id.myImageView);
            mainLayout = itemView.findViewById(R.id.rowConstraintLayout);

        }
    }

}
