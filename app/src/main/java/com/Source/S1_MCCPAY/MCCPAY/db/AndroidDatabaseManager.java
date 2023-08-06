package com.Source.S1_MCCPAY.MCCPAY.db;//add your package name here example: package com.example.dbm;

//all required import files

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.HorizontalScrollView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TableRow.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_MCCPAY.MCCPAY.Main.MainActivity;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.LinkedList;


public class AndroidDatabaseManager extends Activity implements OnItemClickListener {



	//a static class to save cursor,table values etc which is used by functions to share data in the program.
	static class indexInfo
	{
		public static int index = 10;
		public static int numberofpages = 0;
		public static int currentpage = 0;
		public static String table_name="";
		public static Cursor maincursor;
		public static int cursorpostion=0;
		public static ArrayList<String> value_string;
		public static ArrayList<String> tableheadernames;
		public static ArrayList<String> emptytablecolumnnames;
		public static boolean isEmpty;
		public static boolean isCustomQuery;
	}

// all global variables
    public int chmod(File path, int mode) throws Exception {
        Class fileUtils = Class.forName("android.os.FileUtils");
        Method setPermissions =
                fileUtils.getMethod("setPermissions", String.class, int.class, int.class, int.class);
        return (Integer) setPermissions.invoke(null, path.getAbsolutePath(), mode, -1, -1);
    }

	//in the below line Change the text 'yourCustomSqlHelper' with your custom sqlitehelper class name.
	//Do not change the variable name dbm
	//yourCustomSqlLiteHelperclass dbm;
	SQLDBHelper dbm;
	TableLayout tableLayout;
	TableRow.LayoutParams tableRowParams;
	HorizontalScrollView hsv;
	ScrollView mainscrollview;
	LinearLayout mainLayout;
	TextView tvmessage;
	Button previous;
	Button next;
	Button refresh;
	Button cancel;
	Spinner select_table;
	TextView tv;
	String dbname;

	indexInfo info = new indexInfo();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);


		//in the below line Change the text 'yourCustomSqlHelper' with your custom sqlitehelper class name
		//dbm = new yourCustomSqlLiteHelperclass(AndroidDatabaseManager.this);
		Bundle bundle = getIntent().getExtras();
		dbname = bundle.getString("dbname");

		//Toast.makeText(this, dbname, Toast.LENGTH_LONG).show();

		dbm=new SQLDBHelper(this, dbname, null, 1);
		mainscrollview = new ScrollView(AndroidDatabaseManager.this);

		//the main linear layout to which all tables spinners etc will be added.In this activity every element is created dynamically  to avoid using xml file
		mainLayout = new LinearLayout(AndroidDatabaseManager.this);
		mainLayout.setOrientation(LinearLayout.VERTICAL);
		mainLayout.setBackgroundColor(Color.WHITE);
		mainLayout.setScrollContainer(true);
		mainscrollview.addView(mainLayout);

		//all required layouts are created dynamically and added to the main scrollview
		setContentView(mainscrollview);

	/*	String path = getFilesDir().getAbsolutePath();
		Toast.makeText(getApplicationContext(), path, Toast.LENGTH_LONG).show();*/

		//the first row of layout which has a text view and spinner
		final LinearLayout firstrow = new LinearLayout(AndroidDatabaseManager.this);
		firstrow.setPadding(0,10,0,20);
		LinearLayout.LayoutParams firstrowlp = new LinearLayout.LayoutParams(0, 150);
		firstrowlp.weight = 1;

		TextView maintext = new TextView(AndroidDatabaseManager.this);
		maintext.setText("Select Table");
		maintext.setTextSize(16);
		maintext.setLayoutParams(firstrowlp);
		select_table=new Spinner(AndroidDatabaseManager.this);
		select_table.setLayoutParams(firstrowlp);

		firstrow.addView(maintext);
		firstrow.addView(select_table);
		mainLayout.addView(firstrow);

		ArrayList<Cursor> alc ;

		//the horizontal scroll view for table if the table content doesnot fit into screen
		hsv = new HorizontalScrollView(AndroidDatabaseManager.this);

		//the main table layout where the content of the sql tables will be displayed when user selects a table
		tableLayout = new TableLayout(AndroidDatabaseManager.this);
		tableLayout.setHorizontalScrollBarEnabled(true);
		hsv.addView(tableLayout);

		//the second row of the layout which shows number of records in the table selected by user
		final LinearLayout secondrow = new LinearLayout(AndroidDatabaseManager.this);
		secondrow.setPadding(0,20,0,10);
		LinearLayout.LayoutParams secondrowlp = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
		secondrowlp.weight = 1;
		TextView secondrowtext = new TextView(AndroidDatabaseManager.this);
		secondrowtext.setText("No. Of Records : ");
		secondrowtext.setTextSize(16);
		secondrowtext.setLayoutParams(secondrowlp);
		tv =new TextView(AndroidDatabaseManager.this);
		tv.setTextSize(16);
		tv.setLayoutParams(secondrowlp);
		secondrow.addView(secondrowtext);
		secondrow.addView(tv);
		mainLayout.addView(secondrow);
		//A button which generates a text view from which user can write custome queries
		final EditText customquerytext = new EditText(this);
		customquerytext.setVisibility(View.GONE);
		customquerytext.setHint("Enter Your Query here and Click on Submit Query Button .Results will be displayed below");
		mainLayout.addView(customquerytext);

		final Button submitQuery = new Button(AndroidDatabaseManager.this);
		submitQuery.setVisibility(View.GONE);
		submitQuery.setText("Submit Query");

		submitQuery.setBackgroundColor(Color.parseColor("#BAE7F6"));
		mainLayout.addView(submitQuery);

		final TextView help = new TextView(AndroidDatabaseManager.this);
		help.setText("Click on the row below to update values or delete the record.");
		help.setPadding(0,5,0,5);

		// the spinner which gives user a option to add new row , drop or delete table
		final Spinner spinnertable =new Spinner(AndroidDatabaseManager.this);
		mainLayout.addView(spinnertable);
		mainLayout.addView(help);
		hsv.setPadding(0,10,0,10);
		hsv.setScrollbarFadingEnabled(false);
		hsv.setScrollBarStyle(View.SCROLLBARS_OUTSIDE_INSET);
		mainLayout.addView(hsv);
		//the third layout which has buttons for the pagination of content from database
		final LinearLayout thirdrow = new LinearLayout(AndroidDatabaseManager.this);
		previous = new Button(AndroidDatabaseManager.this);
		previous.setText("Previous");
		previous.setBackgroundColor(Color.parseColor("#BAE7F6"));
		previous.setLayoutParams(secondrowlp);

		cancel = new Button(AndroidDatabaseManager.this);
		cancel.setText("Cancel");
		cancel.setBackgroundColor(Color.parseColor("#BAE7F6"));
		cancel.setLayoutParams(secondrowlp);

		next = new Button(AndroidDatabaseManager.this);
		next.setText("Next");
		next.setBackgroundColor(Color.parseColor("#BAE7F6"));
		next.setLayoutParams(secondrowlp);

		TextView tvblank = new TextView(this);
		tvblank.setLayoutParams(secondrowlp);
		thirdrow.setPadding(0,10,0,10);

		secondrowlp.setMargins(5,5,5,5);

		thirdrow.addView(previous);
		thirdrow.addView(cancel);
		//thirdrow.addView(tvblank);
		thirdrow.addView(next);

		//mainLayout.getLayoutParams().width=480;
		mainLayout.addView(thirdrow);

		//the text view at the bottom of the screen which displays error or success messages after a query is executed
		tvmessage =new TextView(AndroidDatabaseManager.this);

		//tvmessage.setText("Error Messages will be displayed here");
		String Query = "SELECT name _id FROM sqlite_master WHERE type ='table'";
		tvmessage.setTextSize(18);
		mainLayout.addView(tvmessage);

		final Button customQuery = new Button(AndroidDatabaseManager.this);
		//customQuery.setText("Custom Query");
		customQuery.setText("Exit");
		customQuery.setBackgroundColor(Color.parseColor("#BAE7F6"));
		mainLayout.addView(customQuery);

		customQuery.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				//set drop down to custom Query
//				indexInfo.isCustomQuery=true;
//				secondrow.setVisibility(View.GONE);
//				spinnertable.setVisibility(View.GONE);
//				help.setVisibility(View.GONE);
//				customquerytext.setVisibility(View.VISIBLE);
//				submitQuery.setVisibility(View.VISIBLE);
//				select_table.setSelection(0);
//				customQuery.setVisibility(View.GONE);
//				hsv.setVisibility(View.GONE);
				try {
					chmod(new File(dbname), 0777);
					Log.d("chmod", dbname);
				} catch (Exception e) {
					e.printStackTrace();
				}

				Intent intent = new Intent();
				intent.setClass(AndroidDatabaseManager.this, MainActivity.class);
				startActivity(intent);
			}
		});


		//when user enter a custom query in text view and clicks on submit query button
		//display results in tablelayout
		submitQuery.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				tableLayout.removeAllViews();
				customQuery.setVisibility(View.GONE);

				ArrayList<Cursor> alc2;
				String Query10=customquerytext.getText().toString();
				Log.d("query",Query10);
				//pass the query to getdata method and get results
				alc2 = dbm.getData(Query10);
				final Cursor c4=alc2.get(0);
				Cursor Message2 =alc2.get(1);
				Message2.moveToLast();

				//if the query returns results display the results in table layout
				if(Message2.getString(0).equalsIgnoreCase("Success"))
				{

					//tvmessage.setBackgroundColor(Color.parseColor("#2ecc71"));
					if(c4!=null){
						//tvmessage.setText("Query Executed successfully.Number of rows returned :"+c4.getCount());
						Toast.makeText(getApplicationContext(), "Query Executed successfully. Number of records returned: "+c4.getCount(), Toast.LENGTH_LONG).show();
						if(c4.getCount()>0)
						{
							indexInfo.maincursor=c4;
							refreshTable(1);
							submitQuery.setVisibility(View.GONE);
							customquerytext.setVisibility(View.GONE);
							customquerytext.setText("");
							customQuery.setVisibility(View.VISIBLE);

							secondrow.setVisibility(View.VISIBLE);
							spinnertable.setVisibility(View.VISIBLE);

							hsv.setVisibility(View.VISIBLE);
						}
					}
					else
					{
						//tvmessage.setText("Query Executed successfully");
						Toast.makeText(getApplicationContext(), "Query Executed successfully", Toast.LENGTH_LONG).show();
						refreshTable(1);
					}
				}
				else
				{
					//if there is any error we displayed the error message at the bottom of the screen
					//tvmessage.setBackgroundColor(Color.parseColor("#e74c3c"));
					//tvmessage.setText("Error:"+Message2.getString(0));
					Toast.makeText(getApplicationContext(), "Error:"+Message2.getString(0), Toast.LENGTH_LONG).show();

				}
			}
		});
		//layout parameters for each row in the table
		tableRowParams = new TableRow.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		tableRowParams.setMargins(0, 0, 2, 0);

		// a query which returns a cursor with the list of tables in the database.We use this cursor to populate spinner in the first row
		alc = dbm.getData(Query);

		//the first cursor has reults of the query
		final Cursor c=alc.get(0);

		//the second cursor has error messages
		Cursor Message =alc.get(1);

		Message.moveToLast();
		String msg = Message.getString(0);
		Log.d("Message from sql = ",msg);

		ArrayList<String> tablenames = new ArrayList<String>();

		if(c!=null)
		{

			c.moveToFirst();
			//tablenames.add("click here");
			do{
				//add names of the table to tablenames array list
				tablenames.add(c.getString(0));
			}while(c.moveToNext());
		}
		//an array adapter with above created arraylist
		ArrayAdapter<String> tablenamesadapter = new ArrayAdapter<String>(AndroidDatabaseManager.this,
				android.R.layout.simple_spinner_item, tablenames) {

			public View getView(int position, View convertView, ViewGroup parent) {
				View v = super.getView(position, convertView, parent);

				v.setBackgroundColor(Color.WHITE);
				TextView adap =(TextView)v;
				adap.setTextSize(20);

				return adap;
			}


			public View getDropDownView(int position, View convertView, ViewGroup parent) {
				View v =super.getDropDownView(position, convertView, parent);

				v.setBackgroundColor(Color.WHITE);

				return v;
			}
		};

		tablenamesadapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		if(tablenamesadapter!=null)
		{
			//set the adpater to select_table spinner
			select_table.setAdapter(tablenamesadapter);
		}

		// when a table names is selecte display the table contents
		select_table.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> parent,
                                       View view, int pos, long id) {
	            	/*
	            	if(pos==0&&!indexInfo.isCustomQuery)
	            	{
	            		secondrow.setVisibility(View.GONE);
	            		hsv.setVisibility(View.GONE);
	            		thirdrow.setVisibility(View.GONE);
						spinnertable.setVisibility(View.GONE);
						help.setVisibility(View.GONE);
						tvmessage.setVisibility(View.GONE);
						customquerytext.setVisibility(View.GONE);
						submitQuery.setVisibility(View.GONE);
						customQuery.setVisibility(View.GONE);
	            	}

	            	if(pos!=0)*/{
					secondrow.setVisibility(View.VISIBLE);
					spinnertable.setVisibility(View.VISIBLE);
					help.setVisibility(View.VISIBLE);
					customquerytext.setVisibility(View.GONE);
					submitQuery.setVisibility(View.GONE);
					customQuery.setVisibility(View.VISIBLE);
					hsv.setVisibility(View.VISIBLE);

					tvmessage.setVisibility(View.VISIBLE);

					thirdrow.setVisibility(View.VISIBLE);
					c.moveToPosition(pos);
					indexInfo.cursorpostion=pos;
					//displaying the content of the table which is selected in the select_table spinner
					Log.d("selected table name is",""+c.getString(0));
					indexInfo.table_name=c.getString(0);
					//tvmessage.setText("Error Messages will be displayed here");
					//tvmessage.setBackgroundColor(Color.WHITE);

					//removes any data if present in the table layout
					tableLayout.removeAllViews();
					ArrayList<String> spinnertablevalues = new ArrayList<String>();
					spinnertablevalues.add("Click here to change this table");
					spinnertablevalues.add("Add row to this table");
					spinnertablevalues.add("Delete this table");
					spinnertablevalues.add("Drop this table");
					ArrayAdapter<String> spinnerArrayAdapter = new ArrayAdapter<String>(getApplicationContext(), android.R.layout.simple_spinner_dropdown_item, spinnertablevalues);
					spinnerArrayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_item);

					// a array adapter which add values to the spinner which helps in user making changes to the table
					ArrayAdapter<String> adapter = new ArrayAdapter<String>(AndroidDatabaseManager.this,
							android.R.layout.simple_spinner_item, spinnertablevalues) {

						public View getView(int position, View convertView, ViewGroup parent) {
							View v = super.getView(position, convertView, parent);

							v.setBackgroundColor(Color.WHITE);
							TextView adap =(TextView)v;
							adap.setTextSize(20);

							return adap;
						}

						public View getDropDownView(int position, View convertView, ViewGroup parent) {
							View v =super.getDropDownView(position, convertView, parent);

							v.setBackgroundColor(Color.WHITE);

							return v;
						}
					};

					adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
					spinnertable.setAdapter(adapter);
					String Query2 ="select * from "+c.getString(0);
					Log.d("",""+Query2);

					//getting contents of the table which user selected from the select_table spinner
					ArrayList<Cursor> alc2=dbm.getData(Query2);
					final Cursor c2=alc2.get(0);
					//saving cursor to the static indexinfo class which can be resued by the other functions
					indexInfo.maincursor=c2;

					// if the cursor returned form the database is not null we display the data in table layout
					if(c2!=null)
					{
						int counts = c2.getCount();
						indexInfo.isEmpty=false;
						Log.d("counts",""+counts);
						tv.setText(""+counts);


						//the spinnertable has the 3 items to drop , delete , add row to the table selected by the user
						//here we handle the 3 operations.
						spinnertable.setOnItemSelectedListener((new AdapterView.OnItemSelectedListener() {
							@Override
							public void onItemSelected(AdapterView<?> parentView, View selectedItemView, int position, long id) {


								((TextView)parentView.getChildAt(0)).setTextColor(Color.rgb(0,0,0));
								//when user selects to drop the table the below code in if block will be executed
								if(spinnertable.getSelectedItem().toString().equals("Drop this table"))
								{
									// an alert dialog to confirm user selection
									runOnUiThread(new Runnable() {
										@Override
										public void run() {
											if(!isFinishing()){

												new AlertDialog.Builder(AndroidDatabaseManager.this)
														.setTitle("Are you sure ?")
														.setMessage("Pressing yes will remove "+ indexInfo.table_name+" table from database")
														.setPositiveButton("yes",
																new DialogInterface.OnClickListener() {
																	// when user confirms by clicking on yes we drop the table by executing drop table query
																	public void onClick(DialogInterface dialog, int which) {

																		String Query6 = "Drop table "+ indexInfo.table_name;
																		ArrayList<Cursor> aldropt=dbm.getData(Query6);
																		Cursor tempc=aldropt.get(1);
																		tempc.moveToLast();
																		Log.d("Drop table Mesage",tempc.getString(0));
																		if(tempc.getString(0).equalsIgnoreCase("Success"))
																		{
																			//tvmessage.setBackgroundColor(Color.parseColor("#2ecc71"));
																			//tvmessage.setText(indexInfo.table_name+"Dropped successfully");
																			Toast.makeText(getApplicationContext(), indexInfo.table_name+"Dropped successfully", Toast.LENGTH_LONG).show();
																			refreshactivity();
																		}
																		else
																		{
																			//if there is any error we displayd the error message at the bottom of the screen
																			//tvmessage.setBackgroundColor(Color.parseColor("#e74c3c"));
																			//tvmessage.setText("Error:"+tempc.getString(0));
																			Toast.makeText(getApplicationContext(), "Error:"+tempc.getString(0), Toast.LENGTH_LONG).show();
																			spinnertable.setSelection(0);
																		}
																	}})
														.setNegativeButton("No",
																new DialogInterface.OnClickListener() {
																	public void onClick(DialogInterface dialog, int which) {
																		spinnertable.setSelection(0);
																	}
																})
														.create().show();
											}
										}
									});

								}
								//when user selects to drop the table the below code in if block will be executed
								if(spinnertable.getSelectedItem().toString().equals("Delete this table"))
								{	// an alert dialog to confirm user selection
									runOnUiThread(new Runnable() {
										@Override
										public void run() {
											if(!isFinishing()){

												new AlertDialog.Builder(AndroidDatabaseManager.this)
														.setTitle("Are you sure?")
														.setMessage("Clicking on yes will delete all the contents of "+ indexInfo.table_name+" table from database")
														.setPositiveButton("yes",
																new DialogInterface.OnClickListener() {

																	// when user confirms by clicking on yes we drop the table by executing delete table query
																	public void onClick(DialogInterface dialog, int which) {
																		String Query7 = "Delete  from "+ indexInfo.table_name;
																		Log.d("delete table query",Query7);
																		ArrayList<Cursor> aldeletet=dbm.getData(Query7);
																		Cursor tempc=aldeletet.get(1);
																		tempc.moveToLast();
																		Log.d("Delete table Mesage",tempc.getString(0));
																		if(tempc.getString(0).equalsIgnoreCase("Success"))
																		{
																			//tvmessage.setBackgroundColor(Color.parseColor("#2ecc71"));
																			//tvmessage.setText(indexInfo.table_name+" table content deleted successfully");
																			Toast.makeText(getApplicationContext(), indexInfo.table_name+" table content deleted successfully.", Toast.LENGTH_LONG).show();
																			indexInfo.isEmpty=true;
																			refreshTable(0);
																		}
																		else
																		{
																			//tvmessage.setBackgroundColor(Color.parseColor("#e74c3c"));
																			//tvmessage.setText("Error:"+tempc.getString(0));
																			Toast.makeText(getApplicationContext(),"Error:"+tempc.getString(0), Toast.LENGTH_LONG).show();
																			spinnertable.setSelection(0);
																		}
																	}})
														.setNegativeButton("No",
																new DialogInterface.OnClickListener() {
																	public void onClick(DialogInterface dialog, int which) {
																		spinnertable.setSelection(0);
																	}
																})
														.create().show();
											}
										}
									});

								}

								//when user selects to add row to the table the below code in if block will be executed
								if(spinnertable.getSelectedItem().toString().equals("Add row to this table"))
								{
									//we create a layout which has textviews with column names of the table and edittexts where
									//user can enter value which will be inserted into the datbase.
									final LinkedList<TextView> addnewrownames = new LinkedList<TextView>();
									final LinkedList<EditText> addnewrowvalues = new LinkedList<EditText>();
									final ScrollView addrowsv =new ScrollView(AndroidDatabaseManager.this);
									Cursor c4 = indexInfo.maincursor;
									if(indexInfo.isEmpty)
									{
										getcolumnnames();
										for(int i = 0; i< indexInfo.emptytablecolumnnames.size(); i++)
										{
											String cname = indexInfo.emptytablecolumnnames.get(i);
											TextView tv = new TextView(getApplicationContext());
											tv.setText(cname);
											addnewrownames.add(tv);

										}
										for(int i=0;i<addnewrownames.size();i++)
										{
											EditText et = new EditText(getApplicationContext());

											addnewrowvalues.add(et);
										}

									}
									else{
										for(int i=0;i<c4.getColumnCount();i++)
										{
											String cname = c4.getColumnName(i);
											TextView tv = new TextView(getApplicationContext());
											tv.setText(cname);
											addnewrownames.add(tv);

										}
										for(int i=0;i<addnewrownames.size();i++)
										{
											EditText et = new EditText(getApplicationContext());

											addnewrowvalues.add(et);
										}
									}
									final RelativeLayout addnewlayout = new RelativeLayout(AndroidDatabaseManager.this);
									RelativeLayout.LayoutParams addnewparams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
									addnewparams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
									for(int i=0;i<addnewrownames.size();i++)
									{
										TextView tv =addnewrownames.get(i);
										EditText et=addnewrowvalues.get(i);
										int t = i+400;
										int k = i+500;
										int lid = i+600;

										tv.setId(t);
										tv.setTextColor(Color.parseColor("#000000"));
										et.setBackgroundColor(Color.parseColor("#F2F2F2"));
										et.setTextColor(Color.parseColor("#000000"));
										et.setId(k);
										final LinearLayout ll = new LinearLayout(AndroidDatabaseManager.this);
										LinearLayout.LayoutParams tvl = new LinearLayout.LayoutParams(0, 100);
										tvl.weight = 1;
										ll.addView(tv,tvl);
										ll.addView(et,tvl);
										ll.setId(lid);

										Log.d("Edit Text Value",""+et.getText().toString());

										RelativeLayout.LayoutParams rll = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
										rll.addRule(RelativeLayout.BELOW,ll.getId()-1 );
										rll.setMargins(0, 20, 0, 0);
										addnewlayout.addView(ll, rll);

									}
									addnewlayout.setBackgroundColor(Color.WHITE);
									addrowsv.addView(addnewlayout);
									Log.d("Button Clicked", "");
									//the above form layout which we have created above will be displayed in an alert dialog
									runOnUiThread(new Runnable() {
										@Override
										public void run() {
											if(!isFinishing()){
												new AlertDialog.Builder(AndroidDatabaseManager.this)
														.setTitle("values")
														.setCancelable(false)
														.setView(addrowsv)
														.setPositiveButton("Add",
																new DialogInterface.OnClickListener() {
																	// after entering values if user clicks on add we take the values and run a insert query
																	public void onClick(DialogInterface dialog, int which) {

																		indexInfo.index = 10;
																		//tableLayout.removeAllViews();
																		//trigger select table listener to be triggerd
																		String Query4 ="Insert into "+ indexInfo.table_name+" (";
																		for(int i=0 ; i<addnewrownames.size();i++)
																		{

																			TextView tv = addnewrownames.get(i);
																			tv.getText().toString();
																			if(i==addnewrownames.size()-1)
																			{

																				Query4=Query4+tv.getText().toString();

																			}
																			else
																			{
																				Query4=Query4+tv.getText().toString()+", ";
																			}
																		}
																		Query4=Query4+" ) VALUES ( ";
																		for(int i=0 ; i<addnewrownames.size();i++)
																		{
																			EditText et = addnewrowvalues.get(i);
																			et.getText().toString();

																			if(i==addnewrownames.size()-1)
																			{

																				Query4=Query4+"'"+et.getText().toString()+"' ) ";
																			}
																			else
																			{
																				Query4=Query4+"'"+et.getText().toString()+"' , ";
																			}


																		}
																		//this is the insert query which has been generated
																		Log.d("Insert Query",Query4);
																		ArrayList<Cursor> altc=dbm.getData(Query4);
																		Cursor tempc=altc.get(1);
																		tempc.moveToLast();
																		Log.d("Add New Row",tempc.getString(0));
																		if(tempc.getString(0).equalsIgnoreCase("Success"))
																		{
																			//tvmessage.setBackgroundColor(Color.parseColor("#2ecc71"));
																			//tvmessage.setText("New Row added succesfully to "+indexInfo.table_name);
																			Toast.makeText(getApplicationContext(), "New Row added succesfully to "+ indexInfo.table_name, Toast.LENGTH_LONG).show();
																			refreshTable(0);
																		}
																		else
																		{
																			//tvmessage.setBackgroundColor(Color.parseColor("#e74c3c"));
																			//tvmessage.setText("Error:"+tempc.getString(0));
																			Toast.makeText(getApplicationContext(), "Error:"+tempc.getString(0), Toast.LENGTH_LONG).show();
																			spinnertable.setSelection(0);
																		}

																	}
																})
														.setNegativeButton("close",
																new DialogInterface.OnClickListener() {
																	public void onClick(DialogInterface dialog, int which) {
																		spinnertable.setSelection(0);
																	}
																})
														.create().show();
											}
										}
									});
								}
							}
							public void onNothingSelected(AdapterView<?> arg0) { }
						}));

						//display the first row of the table with column names of the table selected by the user
						TableRow tableheader = new TableRow(getApplicationContext());

						tableheader.setBackgroundColor(Color.BLACK);
						tableheader.setPadding(0, 2, 0, 2);
						for(int k=0;k<c2.getColumnCount();k++)
						{
							LinearLayout cell = new LinearLayout(AndroidDatabaseManager.this);
							//cell.setBackgroundColor(Color.WHITE);
							cell.setBackgroundColor(Color.parseColor("#BAE7F6"));
							cell.setLayoutParams(tableRowParams);
							final TextView tableheadercolums = new TextView(getApplicationContext());
							// tableheadercolums.setBackgroundDrawable(gd);
							tableheadercolums.setPadding(0, 0, 4, 3);
							tableheadercolums.setText(""+c2.getColumnName(k));
							tableheadercolums.setTextColor(Color.parseColor("#000000"));

							//columsView.setLayoutParams(tableRowParams);
							cell.addView(tableheadercolums);
							tableheader.addView(cell);

						}
						tableLayout.addView(tableheader);
						c2.moveToFirst();

						//after displaying columnnames in the first row  we display data in the remaining columns
						//the below paginatetbale function will display the first 10 tuples of the tables
						//the remaining tuples can be viewed by clicking on the next button
						paginatetable(c2.getCount());

					}
					else{
						//if the cursor returned from the database is empty we show that table is empty
						help.setVisibility(View.GONE);
						tableLayout.removeAllViews();
						getcolumnnames();
						TableRow tableheader2 = new TableRow(getApplicationContext());
						tableheader2.setBackgroundColor(Color.BLACK);
						tableheader2.setPadding(0, 2, 0, 2);

						LinearLayout cell = new LinearLayout(AndroidDatabaseManager.this);
						//cell.setBackgroundColor(Color.WHITE);
						cell.setBackgroundColor(Color.parseColor("#BAE7F6"));
						cell.setLayoutParams(tableRowParams);
						final TextView tableheadercolums = new TextView(getApplicationContext());

						tableheadercolums.setPadding(0, 0, 4, 3);
						tableheadercolums.setText("   Table   Is   Empty   ");
						tableheadercolums.setTextSize(30);
						tableheadercolums.setTextColor(Color.RED);

						cell.addView(tableheadercolums);
						tableheader2.addView(cell);


						tableLayout.addView(tableheader2);

						tv.setText(""+0);
					}
				}}
			@Override
			public void onNothingSelected(AdapterView<?> arg0) {
			}
		});
	}

	public static byte[] strToByteArray(String str) {
		if (str == null) {
			return null;
		}
		byte[] byteArray = new byte[0];
		try {
			byteArray = str.getBytes("UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		return byteArray;
	}
	/**
	 * @Title:string2HexString
	 * @Description:�ַ���ת16�����ַ���
	 * @param strPart
	 *            �ַ���
	 * @return 16�����ַ���
	 * @throws
	 */
	public static String string2HexString(String strPart) {
		StringBuffer hexString = new StringBuffer();
		for (int i = 0; i < strPart.length(); i++) {
			int ch = (int) strPart.charAt(i);
			String strHex = Integer.toHexString(ch);
			hexString.append(strHex);
		}
		return hexString.toString();
	}

	/**
	 * @Title:hexString2String
	 * @Description:16�����ַ���ת�ַ���
	 * @param src
	 *            16�����ַ���
	 * @return �ֽ�����
	 * @throws
	 */
	public static String hexString2String(String src) {
		String temp = "";
		for (int i = 0; i < src.length() / 2; i++) {
			temp = temp
					+ (char) Integer.valueOf(src.substring(i * 2, i * 2 + 2),
					16).byteValue();
		}
		return temp;
	}


	public static String byteArrayToStr(byte[] byteArray) {


		if (byteArray == null) {
			return null;
		}
		String str = null;
		try {
			str = new String(byteArray, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		return str;
	}


	private static void printHexString(byte[] b, int iLen)
	{
		for (int i = 0; i < iLen; i++)
		{
			String hex = Integer.toHexString(b[i] & 0xFF);
			if (hex.length() == 1)
			{
				hex = '0' + hex;
			}
			System.out.print(hex.toUpperCase() + " ");
		}
		System.out.println("");
	}


	/**
	 * @Title:bytes2HexString
	 * @Description:�ֽ�����ת16�����ַ���
	 * @param b
	 *            �ֽ�����
	 * @return 16�����ַ���
	 * @throws
	 */
	public static String bytes2HexString(byte[] b) {
		StringBuffer result = new StringBuffer();
		String hex;
		for (int i = 0; i < b.length; i++) {
			hex = Integer.toHexString(b[i] & 0xFF);
			if (hex.length() == 1) {
				hex = '0' + hex;
			}
			result.append(hex.toUpperCase());
		}
		return result.toString();
	}

	/**
	 * @Title:hexString2Bytes
	 * @Description:16�����ַ���ת�ֽ�����
	 * @param src
	 *            16�����ַ���
	 * @return �ֽ�����
	 * @throws
	 */
	public static byte[] hexString2Bytes(String src) {
		int l = src.length() / 2;
		byte[] ret = new byte[l];
		for (int i = 0; i < l; i++) {
			ret[i] = (byte) Integer
					.valueOf(src.substring(i * 2, i * 2 + 2), 16).byteValue();
		}
		return ret;
	}


	/**
	 * hexתbyte����
	 * @param hex
	 * @return
	 */
	public static byte[] hexToByte(String hex){
		int m = 0, n = 0;
		int byteLen = hex.length() / 2; // ÿ�����ַ�����һ���ֽ�
		byte[] ret = new byte[byteLen];
		for (int i = 0; i < byteLen; i++) {
			m = i * 2 + 1;
			n = m + 1;
			int intVal = Integer.decode("0x" + hex.substring(i * 2, m) + hex.substring(m, n));
			ret[i] = Byte.valueOf((byte)intVal);
		}
		return ret;
	}


	/**
	 * byte����תhex
	 * @param bytes
	 * @return
	 */
	public static String byteToHex(byte[] bytes){
		String strHex = "";
		StringBuilder sb = new StringBuilder("");
		for (int n = 0; n < bytes.length; n++) {
			strHex = Integer.toHexString(bytes[n] & 0xFF);
			sb.append((strHex.length() == 1) ? "0" + strHex : strHex); // ÿ���ֽ��������ַ���ʾ��λ����������λ��0
		}
		return sb.toString().trim();
	}

	public static String toHexString(byte[] ba) {
		StringBuilder str = new StringBuilder();
		for(int i = 0; i < ba.length; i++)
			str.append(String.format("%x", ba[i]));
		return str.toString();
	}

	public static String str2HexStr(String origin) {
		byte[] bytes = origin.getBytes();
		String hex = bytesToHexString(bytes);
		return hex;
	}

	public static String hexStr2Str(String hex) {
		byte[] bb = hexStringToBytes(hex);
		String rr = new String(bb);
		return rr;
	}

	private static byte[] hexStringToBytes(String hexString) {
		if (hexString == null || hexString.equals("")) {
			return null;
		}
		hexString = hexString.toUpperCase();
		int length = hexString.length() / 2;
		char[] hexChars = hexString.toCharArray();
		byte[] d = new byte[length];
		for (int i = 0; i < length; i++) {
			int pos = i * 2;
			d[i] = (byte) (charToByte(hexChars[pos]) << 4 | charToByte(hexChars[pos + 1]));
		}
		return d;
	}
	/**
	 * Convert char to byte
	 *
	 * @param c char
	 * @return byte
	 */
	private static byte charToByte(char c) {
		return (byte) "0123456789ABCDEF".indexOf(c);
	}

	private static String bytesToHexString(byte[] src) {
		StringBuilder stringBuilder = new StringBuilder("");
		if (src == null || src.length <= 0) {
			return null;
		}
		for (int i = 0; i < src.length; i++) {
			int v = src[i] & 0xFF;
			String hv = Integer.toHexString(v);
			if (hv.length() < 2) {
				stringBuilder.append(0);
			}
			stringBuilder.append(hv);
		}
		return stringBuilder.toString();
	}



	public static byte[] getBytes(char[] chars) {
		Charset cs = Charset.forName("ASCII");
		CharBuffer cb = CharBuffer.allocate(chars.length);
		cb.put(chars);
		cb.flip();
		ByteBuffer bb = cs.encode(cb);
		return bb.array();
	}

	private static String hexToAscii(String hexStr) {
		StringBuilder output = new StringBuilder("");

		for (int i = 0; i < hexStr.length(); i += 2) {
			String str = hexStr.substring(i, i + 2);
			output.append((char) Integer.parseInt(str, 16));
		}

		return output.toString();
	}
	public static String toHex(byte [] buf) {
		StringBuffer strbuf = new StringBuffer(buf.length * 2);
		int i;
		for (i = 0; i < buf.length; i++) {
			if (((int) buf[i] & 0xff) < 0x10) {
				strbuf.append("0");
			}
			strbuf.append(Long.toString((int) buf[i] & 0xff, 16));
		}
		return strbuf.toString();
	}

	public static char[] getChars(byte[] bytes) {
		Charset cs = Charset.forName("ASCII");
		ByteBuffer bb = ByteBuffer.allocate(bytes.length);
		bb.put(bytes);
		bb.flip();
		CharBuffer cb = cs.decode(bb);
		return cb.array();
	}

	public static String byteArrayToHex(byte[] a) {
		StringBuilder sb = new StringBuilder(a.length * 2);
		for(byte b: a)
			sb.append(String.format("%02x", b));
		return sb.toString();
	}

	public static byte[] hexStringToByteArray(String s) {
		int len = s.length();
		byte[] data = new byte[len / 2];
		for (int i = 0; i < len; i += 2) {
			data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
					+ Character.digit(s.charAt(i+1), 16));
		}
		return data;
	}

	//get columnnames of the empty tables and save them in a array list
	public void getcolumnnames()
	{
		ArrayList<Cursor> alc3=dbm.getData("PRAGMA table_info("+ indexInfo.table_name+")");
		Cursor c5=alc3.get(0);
		indexInfo.isEmpty=true;
		if(c5!=null)
		{
			indexInfo.isEmpty=true;

			ArrayList<String> emptytablecolumnnames= new ArrayList<String>();
			c5.moveToFirst();
			do
			{
				emptytablecolumnnames.add(c5.getString(1));
			}while(c5.moveToNext());
			indexInfo.emptytablecolumnnames=emptytablecolumnnames;
		}



	}
	//displays alert dialog from which use can update or delete a row
	public void updateDeletePopup(int row)
	{
		Cursor c2= indexInfo.maincursor;
		// a spinner which gives options to update or delete the row which user has selected
		ArrayList<String> spinnerArray = new ArrayList<String>();
		//spinnerArray.add("Click Here to modify this record");
		c2.moveToFirst();
		spinnerArray.add("Update record");
		spinnerArray.add("Delete record");

		//create a layout with text values which has the column names and
		//edit texts which has the values of the row which user has selected
		final ArrayList<String> value_string = indexInfo.value_string;
		final LinkedList<TextView> columnames = new LinkedList<TextView>();
        final ArrayList<Integer> columtypes = new ArrayList<Integer>();
		final LinkedList<EditText> columvalues = new LinkedList<EditText>();
		int columntype = 0;

		int inColumnCount = c2.getColumnCount();
		System.out.println("c2 inColumnCount:" + inColumnCount);

		for(int i=0;i<c2.getColumnCount();i++)
		{
			String cname = c2.getColumnName(i);	
			System.out.println("ColumnName:" + cname);
			TextView tv = new TextView(getApplicationContext());
			tv.setText(cname);
			columnames.add(tv);

		}

		for(int i=0;i<c2.getColumnCount();i++)
		{
			columntype = c2.getType(i);
			System.out.println("ColumnType:" + columntype);
		   // TextView tv = new TextView(getApplicationContext());
			//tv.setText(String.valueOf(columntype));
            columtypes.add(columntype);
		}
		
		for(int i=0;i<columnames.size();i++)
		{
			String cv =value_string.get(i);
			System.out.println("value_string:" + cv);

			EditText et = new EditText(getApplicationContext());
			//value_string.add(cv);
			et.setText(cv);
			columvalues.add(et);
		}

		int lastrid = 0;
		// all text views , edit texts are added to this relative layout lp
		final RelativeLayout lp = new RelativeLayout(AndroidDatabaseManager.this);
		lp.setBackgroundColor(Color.WHITE);
		RelativeLayout.LayoutParams lay = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		lay.addRule(RelativeLayout.ALIGN_PARENT_TOP);

		final ScrollView updaterowsv =new ScrollView(AndroidDatabaseManager.this);
		LinearLayout lcrud = new LinearLayout(AndroidDatabaseManager.this);

		LinearLayout.LayoutParams paramcrudtext = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);

		paramcrudtext.setMargins(0, 20, 0, 0);

		//spinner which displays update , delete options
		final Spinner crud_dropdown = new Spinner(getApplicationContext());

		ArrayAdapter<String> crudadapter = new ArrayAdapter<String>(AndroidDatabaseManager.this,
				android.R.layout.simple_spinner_item, spinnerArray) {

			public View getView(int position, View convertView, ViewGroup parent) {
				View v = super.getView(position, convertView, parent);

				v.setBackgroundColor(Color.WHITE);
				TextView adap =(TextView)v;
				adap.setTextSize(20);

				return adap;
			}


			public View getDropDownView(int position, View convertView, ViewGroup parent) {
				View v =super.getDropDownView(position, convertView, parent);

				v.setBackgroundColor(Color.WHITE);

				if (v == null) {
					LayoutInflater inflater = LayoutInflater.from(AndroidDatabaseManager.this);
					v = inflater.inflate(
							android.R.layout.simple_spinner_item, parent, false);
				}

				TextView tv = (TextView) v
						.findViewById(android.R.id.text1);
				tv.setTextColor(Color.BLACK);
				tv.setTextSize(20);

				return v;
			}
		};


		crudadapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		crud_dropdown.setAdapter(crudadapter);
		lcrud.setId(299);
		lcrud.addView(crud_dropdown,paramcrudtext);

		RelativeLayout.LayoutParams rlcrudparam = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		rlcrudparam.addRule(RelativeLayout.BELOW,lastrid);

		lp.addView(lcrud, rlcrudparam);
		for(int i=0;i<columnames.size();i++)
		{
			TextView tv =columnames.get(i);
			EditText et=columvalues.get(i);
			int t = i+100;
			int k = i+200;
			int lid = i+300;

			tv.setId(t);
			tv.setTextColor(Color.parseColor("#000000"));
			et.setBackgroundColor(Color.parseColor("#F2F2F2"));

			et.setTextColor(Color.parseColor("#000000"));
			et.setId(k);
			Log.d("text View Value",""+tv.getText().toString());
			final LinearLayout ll = new LinearLayout(AndroidDatabaseManager.this);
			ll.setBackgroundColor(Color.parseColor("#FFFFFF"));
			ll.setId(lid);
			LinearLayout.LayoutParams lpp = new LinearLayout.LayoutParams(0, 100);
			lpp.weight = 1;
			tv.setLayoutParams(lpp);
			et.setLayoutParams(lpp);
			ll.addView(tv);
			ll.addView(et);

			Log.d("Edit Text Value",""+et.getText().toString());

			RelativeLayout.LayoutParams rll = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
			rll.addRule(RelativeLayout.BELOW,ll.getId()-1 );
			rll.setMargins(0, 20, 0, 0);
			lastrid=ll.getId();
			lp.addView(ll, rll);

		}

		updaterowsv.addView(lp);
		//after the layout has been created display it in a alert dialog
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if(!isFinishing()){
					new AlertDialog.Builder(AndroidDatabaseManager.this)
							.setTitle("values")
							.setView(updaterowsv)
							.setCancelable(false)
							.setPositiveButton("Ok",
									new DialogInterface.OnClickListener() {

										//this code will be executed when user changes values of edit text or spinner and clicks on ok button
										public void onClick(DialogInterface dialog, int which) {

											//get spinner value
											String spinner_value = crud_dropdown.getSelectedItem().toString();

											//it he spinner value is update this row get the values from
											//edit text fields generate a update query and execute it
											if(spinner_value.equalsIgnoreCase("Update record"))
											{
												indexInfo.index = 10;
												String Query3="UPDATE "+ indexInfo.table_name+" SET ";
												String temStr;

												ContentValues values= new ContentValues();

												for(int i=0;i<columnames.size();i++)
												{
													TextView tvc = columnames.get(i);
													EditText etc = columvalues.get(i);
													int columntype = columtypes.get(i);

                                                    System.out.println("Colum Type:" + columntype);
													System.out.println("Field Name:" + tvc.getText().toString());
													System.out.println("Field Value(new):"+etc.getText().toString());

													if(!etc.getText().toString().equals("null"))
													{

														//Query3=Query3+tvc.getText().toString()+" = ";
														if(columntype == 4)
														{
															values.put(tvc.getText().toString(), hexString2Bytes(etc.getText().toString()));

														}else {
															values.put(tvc.getText().toString(), etc.getText().toString());
														}
														if(i==columnames.size()-1)
														{

															//Query3=Query3+"'"+etc.getText().toString()+"'";


														}
														else{
															if(columntype == 4) {
																//Query3=Query3+hexString2Bytes(etc.getText().toString())+" , ";
																// Query3 = Query3 + "'" + hexToByte(etc.getText().toString()) + "' , ";
																//Query3 = Query3 + "'" + etc.getText().toString() + "' , ";
															}
															else {
																//Query3 = Query3 + "'" + etc.getText().toString() + "' , ";
															}

														}
													}

												}
												//Query3=Query3+" where ";
												for(int i=0;i<columnames.size();i++)
												{
													TextView tvc = columnames.get(i);
													int columntype = columtypes.get(i);

													System.out.println("Field Name:" + tvc.getText().toString());
													System.out.println("Field Value(org):"+value_string.get(i));


													if(!value_string.get(i).equals("null"))
													{

														Query3=Query3+tvc.getText().toString()+" = ";

														if(i==columnames.size()-1)
														{

															Query3=Query3+"'"+value_string.get(i)+"' ";

														}
														else
														{
															if(columntype == 4) {
																// Query3 = Query3 +  strToByteArray(value_string.get(i)) + "and ";
																//Query3 = Query3 + "'" + hexToByte(value_string.get(i)) + "' and ";
																//Query3 = Query3 + "'" + value_string.get(i) + "' and ";
															}
															else {
																Query3 = Query3 + "'" + value_string.get(i) + "' and ";
															}
														}

													}
												}
												Log.d("Update Query",Query3);

												ArrayList<Cursor> aluc = dbm.updateTable(indexInfo.table_name, values, columnames.get(0).getText().toString()+ "=" + value_string.get(0), null);

//												Toast.makeText(getApplicationContext(), indexInfo.table_name+" table Updated Successfully", Toast.LENGTH_LONG).show();
//												refreshTable(0);
												//dbm.getData(Query3);
//												ArrayList<Cursor> aluc=dbm.getData(Query3);

												Cursor tempc=aluc.get(1);
												tempc.moveToLast();
												Log.d("Update Mesage",tempc.getString(0));

												if(tempc.getString(0).equalsIgnoreCase("Success"))
												{
													//tvmessage.setBackgroundColor(Color.parseColor("#2ecc71"));
													//tvmessage.setText(indexInfo.table_name+" table Updated Successfully");
													Toast.makeText(getApplicationContext(), indexInfo.table_name+" table Updated Successfully", Toast.LENGTH_LONG).show();
													refreshTable(0);
												}
												else
												{
													//tvmessage.setBackgroundColor(Color.parseColor("#e74c3c"));
													//tvmessage.setText("Error:"+tempc.getString(0));
													Toast.makeText(getApplicationContext(), "Error:"+tempc.getString(0), Toast.LENGTH_LONG).show();
												}
											}
											//it he spinner value is delete this row get the values from
											//edit text fields generate a delete query and execute it

											if(spinner_value.equalsIgnoreCase("Delete record"))
											{

												indexInfo.index = 10;
												String Query5="DELETE FROM "+ indexInfo.table_name+" WHERE ";

												for(int i=0;i<columnames.size();i++)
												{
													TextView tvc = columnames.get(i);
													if(!value_string.get(i).equals("null"))
													{

														Query5=Query5+tvc.getText().toString()+" = ";

														if(i==columnames.size()-1)
														{

															Query5=Query5+"'"+value_string.get(i)+"' ";

														}
														else
														{
															Query5=Query5+"'"+value_string.get(i)+"' and ";
														}

													}
												}
												Log.d("Delete Query",Query5);

												dbm.getData(Query5);

												ArrayList<Cursor> aldc=dbm.getData(Query5);
												Cursor tempc=aldc.get(1);
												tempc.moveToLast();
												Log.d("Update Mesage",tempc.getString(0));

												if(tempc.getString(0).equalsIgnoreCase("Success"))
												{
													//tvmessage.setBackgroundColor(Color.parseColor("#2ecc71"));
													//tvmessage.setText("Row deleted from "+indexInfo.table_name+" table");
													Toast.makeText(getApplicationContext(), "Row deleted from "+ indexInfo.table_name+" table", Toast.LENGTH_LONG).show();
													refreshTable(0);
												}
												else
												{
													//tvmessage.setBackgroundColor(Color.parseColor("#e74c3c"));
													//tvmessage.setText("Error:"+tempc.getString(0));
													Toast.makeText(getApplicationContext(), "Error:"+tempc.getString(0), Toast.LENGTH_LONG).show();
												}
											}
										}

									})
							.setNegativeButton("close",
									new DialogInterface.OnClickListener() {
										public void onClick(DialogInterface dialog, int which) {

										}
									})
							.create().show();
				}
			}
		});
	}

	public void refreshactivity()
	{

		finish();
		startActivity(getIntent());
	}

	public void refreshTable(int d )
	{
		Cursor c3=null;
		tableLayout.removeAllViews();
		if(d==0)
		{
			String Query8 = "select * from "+ indexInfo.table_name;
			ArrayList<Cursor> alc3=dbm.getData(Query8);
			c3=alc3.get(0);
			//saving cursor to the static indexinfo class which can be resued by the other functions
			indexInfo.maincursor=c3;
		}
		if(d==1)
		{
			c3= indexInfo.maincursor;
		}
		// if the cursor returened form tha database is not null we display the data in table layout
		if(c3!=null)
		{
			int counts = c3.getCount();

			Log.d("counts",""+counts);
			tv.setText(""+counts);
			TableRow tableheader = new TableRow(getApplicationContext());

			tableheader.setBackgroundColor(Color.BLACK);
			tableheader.setPadding(0, 2, 0, 2);
			for(int k=0;k<c3.getColumnCount();k++)
			{
				LinearLayout cell = new LinearLayout(AndroidDatabaseManager.this);
				//cell.setBackgroundColor(Color.WHITE);
				cell.setBackgroundColor(Color.parseColor("#BAE7F6"));
				cell.setLayoutParams(tableRowParams);
				final TextView tableheadercolums = new TextView(getApplicationContext());
				tableheadercolums.setPadding(0, 0, 4, 3);
				tableheadercolums.setText(""+c3.getColumnName(k));
				tableheadercolums.setTextColor(Color.parseColor("#000000"));
				cell.addView(tableheadercolums);
				tableheader.addView(cell);

			}
			tableLayout.addView(tableheader);
			c3.moveToFirst();

			//after displaying column names in the first row  we display data in the remaining columns
			//the below paginate table function will display the first 10 tuples of the tables
			//the remaining tuples can be viewed by clicking on the next button
			paginatetable(c3.getCount());
		}
		else{

			TableRow tableheader2 = new TableRow(getApplicationContext());
			tableheader2.setBackgroundColor(Color.BLACK);
			tableheader2.setPadding(0, 2, 0, 2);

			LinearLayout cell = new LinearLayout(AndroidDatabaseManager.this);
			cell.setBackgroundColor(Color.WHITE);
			cell.setLayoutParams(tableRowParams);

			final TextView tableheadercolums = new TextView(getApplicationContext());
			tableheadercolums.setPadding(0, 0, 4, 3);
			tableheadercolums.setText("   Table   Is   Empty   ");
			tableheadercolums.setTextSize(30);
			tableheadercolums.setTextColor(Color.RED);

			cell.addView(tableheadercolums);
			tableheader2.addView(cell);


			tableLayout.addView(tableheader2);

			tv.setText(""+0);
		}

	}

	//the function which displays tuples from database in a table layout
	public void paginatetable(final int number)
	{


		final Cursor c3 = indexInfo.maincursor;
		indexInfo.numberofpages=(c3.getCount()/10)+1;
		indexInfo.currentpage=1;
		c3.moveToFirst();
		int currentrow=0;
		int columnType = 0;

		//display the first 10 tuples of the table selected by user
		do
		{

			final TableRow tableRow = new TableRow(getApplicationContext());
			tableRow.setBackgroundColor(Color.BLACK);
			tableRow.setPadding(0, 2, 0, 2);

			int inColumnCount = c3.getColumnCount();
			System.out.println("c3 inColumnCount:" + inColumnCount);

			for(int j=0 ;j<c3.getColumnCount();j++)
			{
				LinearLayout cell = new LinearLayout(this);
				cell.setBackgroundColor(Color.WHITE);
				cell.setLayoutParams(tableRowParams);
				final TextView columsView = new TextView(getApplicationContext());
				String column_data = "";
				columnType = c3.getType(j);

                System.out.println("columnType:" + columnType);

				if(Cursor.FIELD_TYPE_STRING == columnType)
				{
					column_data = c3.getString(j);
				}else if(Cursor.FIELD_TYPE_INTEGER == columnType)
				{
					column_data = String.valueOf(c3.getInt(j));
				}else if(Cursor.FIELD_TYPE_FLOAT == columnType)
				{
					column_data = String.valueOf(c3.getFloat(j));
				}else if(Cursor.FIELD_TYPE_BLOB == columnType)
				{
					column_data = bytes2HexString(c3.getBlob(j));
				}else if(Cursor.FIELD_TYPE_NULL == columnType)
				{
					column_data = "null";
				}

				System.out.println("columname:" + c3.getColumnName(j));
                System.out.println("column_data:" + column_data);

//				try{
//					column_data = c3.getString(j);
//					System.out.println("column_data:" + column_data);
//				}catch(Exception e){
//					// Column data is not a string , do not display it
//					System.out.println("Column data is not a string");
//				}
				columsView.setText(column_data);
				columsView.setTextColor(Color.parseColor("#000000"));
				columsView.setPadding(0, 0, 4, 3);
				cell.addView(columsView);
				tableRow.addView(cell);

			}

			tableRow.setVisibility(View.VISIBLE);
			currentrow=currentrow+1;
			//we create listener for each table row when clicked a alert dialog will be displayed
			//from where user can update or delete the row
			tableRow.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {

					final ArrayList<String> value_string = new ArrayList<String>();
					for(int i=0;i<c3.getColumnCount();i++)
					{
						LinearLayout llcolumn = (LinearLayout) tableRow.getChildAt(i);
						TextView tc =(TextView)llcolumn.getChildAt(0);

						String cv =tc.getText().toString();
						value_string.add(cv);

					}
					indexInfo.value_string=value_string;
					//the below function will display the alert dialog
					updateDeletePopup(0);
				}
			});
			tableLayout.addView(tableRow);


		}while(c3.moveToNext()&&currentrow<10);

		indexInfo.index=currentrow;


		// when user clicks on the previous button update the table with the previous 10 tuples from the database
		previous.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				int tobestartindex=(indexInfo.currentpage-2)*10;

				//if the tbale layout has the first 10 tuples then toast that this is the first page
				if(indexInfo.currentpage==1)
				{
					Toast.makeText(getApplicationContext(), "This is the first page", Toast.LENGTH_LONG).show();
				}
				else
				{
					indexInfo.currentpage= indexInfo.currentpage-1;
					c3.moveToPosition(tobestartindex);

					boolean decider=true;
					for(int i=1;i<tableLayout.getChildCount();i++)
					{
						TableRow tableRow = (TableRow) tableLayout.getChildAt(i);


						if(decider)
						{
							tableRow.setVisibility(View.VISIBLE);
							for(int j=0;j<tableRow.getChildCount();j++)
							{
								LinearLayout llcolumn = (LinearLayout) tableRow.getChildAt(j);
								TextView columsView = (TextView) llcolumn.getChildAt(0);

								columsView.setText(""+c3.getString(j));

							}
							decider=!c3.isLast();
							if(!c3.isLast()){c3.moveToNext();}
						}
						else
						{
							tableRow.setVisibility(View.GONE);
						}

					}

					indexInfo.index=tobestartindex;

					Log.d("index =",""+ indexInfo.index);
				}
			}
		});

		cancel.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v) {
				//Toast.makeText(getApplicationContext(), "This is cancel button", Toast.LENGTH_LONG).show();
				refreshactivity();
			}
		});

		// when user clicks on the next button update the table with the next 10 tuples from the database
		next.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{

				//if there are no tuples to be shown toast that this the last page
				if(indexInfo.currentpage>= indexInfo.numberofpages)
				{
					Toast.makeText(getApplicationContext(), "This is the last page", Toast.LENGTH_LONG).show();
				}
				else
				{
					indexInfo.currentpage= indexInfo.currentpage+1;
					boolean decider=true;


					for(int i=1;i<tableLayout.getChildCount();i++)
					{
						TableRow tableRow = (TableRow) tableLayout.getChildAt(i);


						if(decider)
						{
							tableRow.setVisibility(View.VISIBLE);
							for(int j=0;j<tableRow.getChildCount();j++)
							{
								LinearLayout llcolumn = (LinearLayout) tableRow.getChildAt(j);
								TextView columsView =(TextView)llcolumn.getChildAt(0);

								columsView.setText(""+c3.getString(j));

							}
							decider=!c3.isLast();
							if(!c3.isLast()){c3.moveToNext();}
						}
						else
						{
							tableRow.setVisibility(View.GONE);
						}
					}
				}
			}
		});

	}
	@Override
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		// TODO Auto-generated method stub

	}

}
