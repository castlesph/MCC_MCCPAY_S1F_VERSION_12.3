package com.Source.S1_MCCPAY.MCCPAY.db;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

//import info.de,.sqlite.database.model.Note;
//import demo.com.pos.Notes;

public class SQLDBHelper extends SQLiteOpenHelper {
    SQLDB sqldb;

    // Database Version
    private static final int DATABASE_VERSION = 1;

    // Database Name
    private static final String DATABASE_NAME = "collectors.s3db";

    public static final String TABLE_NAME = "collectors";
/*
    public SQLDBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }
*/
    /* Constructor with all input parameter*/
    public SQLDBHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);
        //ctx = context;
    }

    // Creating Tables
    @Override
    public void onCreate(SQLiteDatabase db) {

        // create notes table
        //db.execSQL(sqldb.CREATE_TABLE);
    }

    // Upgrading database
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        // Drop older table if existed
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);

        // Create tables again
        onCreate(db);
    }

    public List<SQLDB> getColumnNameAndType(String table_name) {
        List<SQLDB> sqldb_array = new ArrayList<>();

        String Query = "PRAGMA table_info(" + table_name + ")";
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.rawQuery(Query, null);

        if (cursor.moveToFirst()) {
            do {
                SQLDB sqldb_new = new SQLDB(
                        cursor.getString(cursor.getColumnIndex("name")),
                        cursor.getString(cursor.getColumnIndex("type")));
                sqldb_array.add(sqldb_new);
            } while (cursor.moveToNext());
        }
        cursor.close();
        db.close();

        return sqldb_array;
    }

    public List<SQLDB> getRecord(String columnName, String columnType, String table_name) {
        List<SQLDB> sqldb_array = new ArrayList<>();
        String string_value;
        String Query = "SELECT " + columnName + " FROM " + table_name;
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.rawQuery(Query, null);

        if(cursor.getCount() <= 0) {
            cursor.close();
            db.close();
            return null;
        }

        if (cursor != null)
            cursor.moveToFirst();

        //SQLDB sqldb_new = new SQLDB();

        if (cursor.moveToFirst()) {
            do {

                /*
                if(columnType.equals("INTEGER"))
                    string_value=String.valueOf(cursor.getInt(cursor.getColumnIndex(columnName)));
                else
                    string_value=cursor.getString(cursor.getColumnIndex(columnName));
                */

                if(columnType.equals("STRING"))
                {
                    string_value = cursor.getString(cursor.getColumnIndex(columnName));
                }
                else if(columnType.equals("INTEGER"))
                {
                    string_value = String.valueOf(cursor.getInt(cursor.getColumnIndex(columnName)));
                }
                else if(columnType.equals("FLOAT"))
                {
                    string_value = String.valueOf(cursor.getFloat(cursor.getColumnIndex(columnName)));
                }
                else if(columnType.equals("BLOB"))
                {
                    string_value = String.valueOf(cursor.getBlob(cursor.getColumnIndex(columnName)));
                }
                else //if(Cursor.FIELD_TYPE_NULL == columnType)
                {
                    //string_value = "null";
                    string_value = cursor.getString(cursor.getColumnIndex(columnName));
                }

                SQLDB sqldb_new = new SQLDB(string_value);
                sqldb_array.add(sqldb_new);

            } while (cursor.moveToNext());
        }

        cursor.close();
        db.close();
        return sqldb_array;
    }

    public List<SQLDB> getTable() {
        List<SQLDB> sqldb_array = new ArrayList<>();

        String Query = "SELECT name FROM sqlite_master WHERE type = 'table' AND name NOT LIKE 'sqlite_%' ORDER BY 1";
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.rawQuery(Query, null);

        if (cursor.moveToFirst()) {
            do {
                SQLDB sqldb_new = new SQLDB(
                        cursor.getString(cursor.getColumnIndex("name")));
                sqldb_array.add(sqldb_new);
            } while (cursor.moveToNext());
        }
        cursor.close();
        db.close();

        return sqldb_array;
    }

    public int getCount(String table_name) {
        String countQuery = "SELECT  * FROM " + table_name;
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.rawQuery(countQuery, null);

        int count = cursor.getCount();
        cursor.close();

        db.close();
        // return count
        return count;
    }

    public List<SQLDB> getRecordAll(String table_name) {
        List<SQLDB> sqldb_array = new ArrayList<>();
        String string_value;
        //String Query = "SELECT * FROM " + table_name;
        SQLiteDatabase db = this.getReadableDatabase();
        //Cursor cursor = db.rawQuery(Query, null);
        Cursor cursor = db.query(table_name, null, null, null, null, null, null);

        if(cursor.getCount() <= 0) {
            cursor.close();
            db.close();
            return null;
        }

        if (cursor != null)
            cursor.moveToFirst();

        String columnNamesArr[] = cursor.getColumnNames();
        //SQLDB sqldb_new = new SQLDB();

        if (cursor.moveToFirst()) {
            do {
                int columnCount = columnNamesArr.length;
                for(int i=0;i<columnCount;i++)
                {
                    String columnName = columnNamesArr[i];
                    // This is the column value.
                    String columnValue = "";
                    int columnIndex = cursor.getColumnIndex(columnName);

                    // Get current column data type.
                    int columnType = cursor.getType(columnIndex);

                    if(Cursor.FIELD_TYPE_STRING == columnType)
                    {
                        columnValue = cursor.getString(columnIndex);
                        SQLDB sqldb_new = new SQLDB(columnValue);
                        sqldb_array.add(sqldb_new);
                    }
                    else if(Cursor.FIELD_TYPE_INTEGER == columnType)
                    {
                        columnValue = String.valueOf(cursor.getInt(columnIndex));
                        SQLDB sqldb_new = new SQLDB(columnValue);
                        sqldb_array.add(sqldb_new);
                    }
                    else if(Cursor.FIELD_TYPE_FLOAT == columnType)
                    {
                        columnValue = String.valueOf(cursor.getFloat(columnIndex));
                        SQLDB sqldb_new = new SQLDB(columnValue);
                        sqldb_array.add(sqldb_new);
                    }
                    else if(Cursor.FIELD_TYPE_BLOB == columnType)
                    {
                        columnValue = String.valueOf(cursor.getBlob(columnIndex));
                        SQLDB sqldb_new = new SQLDB(columnValue);
                        sqldb_array.add(sqldb_new);
                    }
                    else if(Cursor.FIELD_TYPE_NULL == columnType)
                    {
                        columnValue = "null";
                        SQLDB sqldb_new = new SQLDB(columnValue);
                        sqldb_array.add(sqldb_new);
                    }
                }
            } while (cursor.moveToNext());
        }

        cursor.close();
        db.close();
        return sqldb_array;
    }

    public List<Map<String, String>> queryAllReturnListMap(String tableName)
    {
        List<Map<String, String>> ret = new ArrayList<Map<String, String>>();

        // Query all rows in table.
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.query(tableName, null, null, null, null, null, null);

        if(cursor!=null)
        {
            // Get all column name in an array.
            String columnNamesArr[] = cursor.getColumnNames();

            // Move to first cursor.
            cursor.moveToFirst();
            do {
                // Create a map object represent a table row data.
                Map<String, String> rowMap = new HashMap<String, String>();

                // Get column count.
                int columnCount = columnNamesArr.length;
                for(int i=0;i<columnCount;i++)
                {
                    // Get each column name.
                    String columnName = columnNamesArr[i];

                    // This is the column value.
                    String columnValue = "";

                    // Get column index value.
                    int columnIndex = cursor.getColumnIndex(columnName);

                    // Get current column data type.
                    int columnType = cursor.getType(columnIndex);

                    if(Cursor.FIELD_TYPE_STRING == columnType)
                    {
                        columnValue = cursor.getString(columnIndex);
                    }else if(Cursor.FIELD_TYPE_INTEGER == columnType)
                    {
                        columnValue = String.valueOf(cursor.getInt(columnIndex));
                    }else if(Cursor.FIELD_TYPE_FLOAT == columnType)
                    {
                        columnValue = String.valueOf(cursor.getFloat(columnIndex));
                    }else if(Cursor.FIELD_TYPE_BLOB == columnType)
                    {
                        columnValue = String.valueOf(cursor.getBlob(columnIndex));
                    }else if(Cursor.FIELD_TYPE_NULL == columnType)
                    {
                        columnValue = "null";
                    }

                    rowMap.put(columnName, columnValue);

                    ret.add(rowMap);
                }
            }while(cursor.moveToNext());

            cursor.close();
            db.close();
        }
        return ret;
    }

    public void deleteRecord(String tableName, String columnName, String Value) {
        SQLiteDatabase db = this.getWritableDatabase();
        db.delete(tableName, columnName + " = ?",
                new String[]{Value});
        db.close();
    }

    public int updateRecord(String tableName, String columnName, String Value, String id, String id_value) {
        SQLiteDatabase db = this.getWritableDatabase();

        ContentValues values = new ContentValues();
        values.put(columnName, Value);

        // updating row
        return db.update(tableName, values, id + " = ?",
                new String[]{id_value});
    }

    public boolean ExecuteQuery(String Query) {
        boolean fResult=false;

        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor;
        try {
            cursor = db.rawQuery(Query, null);
            cursor.moveToFirst();

            cursor.close();
            db.close();
            fResult=true;
        }
        catch(Exception e)
        {
            fResult=false;
        }

        return fResult;
    }



    public ArrayList<Cursor> getData(String Query){
        //get writable database
        SQLiteDatabase sqlDB = this.getWritableDatabase();
        String[] columns = new String[] { "message" };
        //an array list of cursor to save two cursors one has results from the query
        //other cursor stores error message if any errors are triggered
        ArrayList<Cursor> alc = new ArrayList<Cursor>(2);
        MatrixCursor Cursor2= new MatrixCursor(columns);
        alc.add(null);
        alc.add(null);

        try{
            String maxQuery = Query ;

		    System.out.println("getData"+maxQuery);

            //execute the query results will be save in Cursor c
            Cursor c = sqlDB.rawQuery(maxQuery, null);

            //add value to cursor2
            Cursor2.addRow(new Object[] { "Success" });

            alc.set(1,Cursor2);
            if (null != c && c.getCount() > 0) {

                alc.set(0,c);
                c.moveToFirst();

                return alc ;
            }
            return alc;
        } catch(SQLException sqlEx){
            Log.d("printing exception", sqlEx.getMessage());
            //if any exceptions are triggered save the error message to cursor an return the arraylist
            Cursor2.addRow(new Object[] { ""+sqlEx.getMessage() });
            alc.set(1,Cursor2);
            return alc;
        } catch(Exception ex){
            Log.d("printing exception", ex.getMessage());

            //if any exceptions are triggered save the error message to cursor an return the arraylist
            Cursor2.addRow(new Object[] { ""+ex.getMessage() });
            alc.set(1,Cursor2);
            return alc;
        }
    }

    public ArrayList<Cursor> updateTable(String table, ContentValues values, String whereClause, String[] whereArgs){
        //get writable database
        SQLiteDatabase sqlDB = this.getWritableDatabase();
        String[] columns = new String[] { "message" };
        //an array list of cursor to save two cursors one has results from the query
        //other cursor stores error message if any errors are triggered
        ArrayList<Cursor> alc = new ArrayList<Cursor>(2);
        MatrixCursor Cursor2= new MatrixCursor(columns);
        alc.add(null);
        alc.add(null);

        try{

            //execute the query results will be save in Cursor c
             sqlDB.update(table, values, whereClause, whereArgs);

            //add value to cursor2
             Cursor2.addRow(new Object[] { "Success" });
             alc.set(1, Cursor2);

            return alc;
        } catch(SQLException sqlEx){
            Log.d("printing exception", sqlEx.getMessage());
            //if any exceptions are triggered save the error message to cursor an return the arraylist
            Cursor2.addRow(new Object[] { ""+sqlEx.getMessage() });
            alc.set(1,Cursor2);
            return alc;
        } catch(Exception ex){
            Log.d("printing exception", ex.getMessage());

            //if any exceptions are triggered save the error message to cursor an return the arraylist
            Cursor2.addRow(new Object[] { ""+ex.getMessage() });
            alc.set(1,Cursor2);
            return alc;
        }
    }
}
