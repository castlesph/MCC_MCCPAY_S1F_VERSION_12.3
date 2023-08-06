package com.Source.S1_MCCPAY.MCCPAY.db;

public class SQLDB {
    private static final String DATABASE_NAME = "collectors.s3db";

    public static final String TABLE_NAME = "collectors";

    String str_columnname;
    String str_columntype;
    String str_value;
    int int_columntype;

    public SQLDB(String str_columnname, String str_value, int int_columntype) {
        this.str_columnname = str_columnname;
        this.str_value = str_value;
        this.int_columntype = int_columntype;
    }

    public int getInt_columntype() {

        return int_columntype;
    }

    public void setInt_columntype(int int_columntype) {
        this.int_columntype = int_columntype;
    }

    public SQLDB(String str_columnname, String str_columntype, String str_value) {
        this.str_columnname = str_columnname;
        this.str_columntype = str_columntype;
        this.str_value = str_value;
    }

    public SQLDB(String str_columnname, String str_columntype) {
        this.str_columnname = str_columnname;
        this.str_columntype = str_columntype;
    }

    public SQLDB() {
    }

    public SQLDB(String str_value) {
        this.str_value = str_value;
    }

    public String getStr_columnname() {
        return str_columnname;
    }

    public void setStr_columnname(String str_columnname) {
        this.str_columnname = str_columnname;
    }

    public String getStr_columntype() {
        return str_columntype;
    }

    public void setStr_columntype(String str_columntype) {
        this.str_columntype = str_columntype;
    }

    public String getStr_value() {
        return str_value;
    }

    public void setStr_value(String str_value) {
        this.str_value = str_value;
    }
}

