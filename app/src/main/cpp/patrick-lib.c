//
// Created by patri on 6/3/2018.
//

#include "patrick-lib.h"
#include <sqlite3.h>
#include "Database/DatabaseFunc.h"
#include "Debug/debug.h"
#include "Aptrans/MultiAptrans.h"
#include "Ctls/POSWave.h"
#include "Includes/POSSale.h"
#include "Ctls/POSCtls.h"
#include "Includes/POSVoid.h"
#include <ctosapi.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <dirent.h>
#include <vegaapi.h>
#include <android_jni_log.h>
#include <KMS2Client.h>

sqlite3 * db;
sqlite3_stmt *stmt;

extern JavaVM *jvm;
extern jclass activityClass;
extern jobject activityObj;
extern JNIEnv *g_env;


//<<<<<<< .mine
//JavaVM *g_JavaVM;

//extern JavaVM *g_JavaVM;
//=======
JavaVM *g_JavaVM;
//>>>>>>> .r11230
extern jobject g_callback_obj_ctos;
extern jmethodID  g_callback_mid_bring2Front;

jmethodID g_callback_mid_lcd;
jmethodID  g_callback_mid_clear;
jmethodID  g_callback_mid_aligned;

jobject g_callback_obj_input;
jmethodID  g_callback_mid_showkeypad;
jmethodID  g_callback_mid_keypadmsg;
jmethodID  g_callback_mid_keypaddone;
jmethodID  g_callback_mid_InputAmountEx;

JavaVM *g_vm;

//ecr
jclass ecrClass;
jobject ecrObj;
JavaVM *ecrjvm;


int ing_KeyPressed = 0;
int printcopies_cntr = 0;


JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1InitWaveData(JNIEnv *env, jobject instance) {
	// TODO

	int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

	BYTE bInBuf[40];
	BYTE bOutBuf[40];
	USHORT usInLen = 0;
	USHORT usOutLen = 0;

	char cwd[1024];

	//inTCTRead(1);
	//strTCT.byRS232DebugPort = 8;
	//inTCTSave(1);
     if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
	
	jint rs = (*env)->GetJavaVM(env, &jvm);

	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);
	
	vdCTOS_InitWaveData();
	
	// here put call all subAP 0x99 events
	inMultiAP_RunIPCCmdTypesEx("com.Source.S1_MCCPAY.MCCPAY", 0x99, bInBuf, usInLen, bOutBuf, &usOutLen);
	
 //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);
	
    return 0;
}


JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1WAVE_1SALE(JNIEnv *env, jobject instance) {

    	inTCTRead(1);

        if(activityClass != NULL)
             (*env)->DeleteGlobalRef(env, activityClass);
        if(activityObj != NULL)
             (*env)->DeleteGlobalRef(env, activityObj);

	jint rs = (*env)->GetJavaVM(env, &jvm);

	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);

   	vdDebug_LogPrintf("inCTOS_WAVE_SALE.............");

   	inCTOS_WAVE_SALE();

        //release memory
        if(cls != NULL)
             (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1Main(JNIEnv *env, jobject instance) {
	// TODO

	int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

	//inTCTRead(1);
	//strTCT.byRS232DebugPort = 8;
	//inTCTSave(1);

        if(activityClass != NULL)
             (*env)->DeleteGlobalRef(env, activityClass);
        if(activityObj != NULL)
             (*env)->DeleteGlobalRef(env, activityObj);

	jint rs = (*env)->GetJavaVM(env, &jvm);

	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);
	
	vdCTOS_InitWaveData();

      //release memory
      if(cls != NULL)
           (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1Sale(JNIEnv *env, jobject instance) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];


	vdDebug_LogPrintf("SATURN SALE");

    //inRetVal = inTCTRead(1);
    //vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 0;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.byERMMode  = 0;
	
    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("SATURN inCTOSS_Sale.............");
     inCTOS_WAVE_SALE();

      //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1Void(JNIEnv *env, jobject instance) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 0;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);


    vdDebug_LogPrintf("inCTOS_VOID.............");
    inCTOS_VOID();

   //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1Sale1(JNIEnv *env, jobject instance) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //strTCT.fDemo= 1;
    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;

    //inRetVal = inTCTSave(1);
    vdDebug_LogPrintf("saturn inTCTSave inRetVal[%d]", inRetVal);

    inCTOS_SALE();
    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1Settle(JNIEnv *env, jobject instance) {
    // TODO

    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;
    EMVCL_RC_DATA_EX stRCDataEx;

    char cwd[1024];

    //inRetVal = inTCTRead(1);
    //vdDebug_LogPrintf("inTCTRead inRetVal[%d] strTCT.fDemo [%d]", inRetVal, strTCT.fDemo);
    //strTCT.byRS232DebugPort = 8;
    //inRetVal = inTCTSave(1);
    //vdDebug_LogPrintf("inTCTSave inRetVal[%d]", inRetVal);

    /*Here do test init, load TCT PRM*/
    //strTCT.fShareComEnable = 1;
    //strTCT.fDemo = 0;
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
	
    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);
	
    inCTOS_SETTLEMENT();

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}



JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inDisplayMsg1(JNIEnv *env, jobject instance) {


    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

    char cwd[1024];

    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inDisplayMsg");
    inDisplayMsg1();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inDisplayImage(JNIEnv *env, jobject instance) {


    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

    char cwd[1024];

    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inDisplayImage");
    inDisplayImage();

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}


JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inDisplayMsg(JNIEnv *env, jobject instance) {


    inDisplayMsg1();
    return 0;
}


int inCallJAVA_DisplayMultipleMessage(char *pszMsg)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_DisplayMultipleMessage=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("jstring[%s]", "This comes from inCallJAVA_DisplayMultipleMessage.");

    jstring jstr = (*env)->NewStringUTF(env, pszMsg);
    vdDebug_LogPrintf("jstring[%s][%s]", "This this Pass in string data to Java", pszMsg);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "displayMultipleMsg", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "displayMultipleMsg");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        vdDebug_LogPrintf("strcpy");

        memset(uszBuffer, 0x00, sizeof(uszBuffer));
        strcpy(uszBuffer, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
        vdDebug_LogPrintf("get nothing...");
    }

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayMultipleMessage");

    if(0 == strcmp(uszBuffer, "OK"))
        return d_OK;
    else
        return d_NO;
}


int inCallJAVA_DisplayUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_DisplayUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


  
    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "DisplayUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayUI");
    return d_OK;
}

int inCallJAVA_GetPinUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    //vdDebug_LogPrintf("saturn =====inCallJAVA_GetPinUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    //vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    //dDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

   //vdDebug_LogPrintf("jstring[%s]", "com/Source/com.Source.S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetPinUI", "(Ljava/lang/String;)Ljava/lang/String;");

    //vdDebug_LogPrintf("jstring[%s]", "GetPinUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_GetPinUI");
    return d_OK;
}


int inCallJAVA_BackToProgress(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====BackToProgress=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "BackToProgress", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "BackToProgress");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end BackToProgress");
    return d_OK;
}



int inCallJAVA_DisplayImage(int x, int y, char *pszMsg_Img)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_DisplayImage=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("jstring[%s]", "This comes from inCallJAVA_DisplayImage.");

    jstring jstr = (*env)->NewStringUTF(env, pszMsg_Img);
    vdDebug_LogPrintf("jstring[%s][%s]", "This this Pass in string data to Java", pszMsg_Img);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "displayImage", "(IILjava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "inCallJAVA_DisplayImage");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, x, y, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        vdDebug_LogPrintf("strcpy");

        memset(uszBuffer, 0x00, sizeof(uszBuffer));
        strcpy(uszBuffer, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
        vdDebug_LogPrintf("get nothing...");
    }

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayImage");

    if(0 == strcmp(uszBuffer, "OK"))
        return d_OK;
    else
        return d_NO;
}

int inCallJAVA_GetAmountString(BYTE *pbDispMsg, BYTE *pbAmtStr, BYTE *pbAmtLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_GetAmountString=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    //else
    //pbDispMsg = "Tine: UserConfirm activity";

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

    //jclass clazz = (*env)->FindClass(env, "com/Source/com.Source.S1_MCCPAY/MCCPAY/Main/MainActivity");
    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetAmountString", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetAmountString");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbAmtLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbAmtStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbAmtLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_GetAmountString");
    return d_OK;
}



int inCallJAVA_UserConfirmMenu(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[d_MAX_IPC_BUFFER];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_UserConfirmMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


  
    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenu", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmMenu");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmMenu");
    return d_OK;
}

int inCallJAVA_UserSelectUpDown(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserSelectUpDown=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


  
    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_NPX/NPX/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_NPX/NPX/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserSelectUpDown", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "inCallJAVA_UserSelectUpDown");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 

	if(result != NULL)
	    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserSelectUpDown");
    return d_OK;
}


int inCallJAVA_UserInputString(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserInputString=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserInputString", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserInputString");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserInputString");
    return d_OK;
}

int inCallJAVA_S1InputString(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[1024+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_S1InputString=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("SATURN jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("saturn pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("saturn jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "InputStringUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("saturn test jstring[%s]", "InputStringUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);


	vdDebug_LogPrintf("saturn inCallJAVA_S1InputString test");

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_S1InputString");
    return d_OK;
}

int inCallJAVA_EnterAnyNum(char *pbNumLen, char *pbaNum)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;

    char baPINBlk[8];

    vdDebug_LogPrintf("=====inCallJAVA_EnterAnyNum=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("jstring[%s]", "This comes from CallJavaForNumString.");

    jstring jstr = (*env)->NewStringUTF(env, "This this Pass in string data to Java");
    vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "getAnyNumStr", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "getAnyNumStr");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbNumLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbaNum, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);


        vdDebug_LogPrintf("Copy data back");
        if (0 == strcmp(str, "BYPASS"))
            *pbNumLen = 0;
    }
    else
        *pbNumLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_EnterAnyNum");
    return d_OK;
}

int inCallJAVA_DOptionMenuDisplay(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DOptionMenuDisplay=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DOptionMenuDisplay", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "DOptionMenuDisplay");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DOptionMenuDisplay");
    return d_OK;
}

int inCallJAVA_DPopupMenuDisplay(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[100+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_DPopupMenuDisplay=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DPopupMenuDisplay", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "DPopupMenuDisplay");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DPopupMenuDisplay");
    return d_OK;
}

/*below is the new way to display UI*/

//android test
#if 0
USHORT CTOS_LCDTClearDisplay(void)
{
    JNIEnv *tenv;

	
    LOGD("---JNI AP CB--- CTOS_LCDTClearDisplay TEST \n");

    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        
		LOGD("---JNI AP CB--- CTOS_LCDTClearDisplay TEST2 \n");
        // attachCurrentThread() failed.
    }
    else {
		
		LOGD("---JNI AP CB--- CTOS_LCDTClearDisplay TEST3 \n");
        (*tenv)->CallLongMethod(tenv, g_callback_obj_ctos, g_callback_mid_clear);
    }
	
    LOGD("---JNI AP CB--- CTOS_LCDTClearDisplay RETURN \n");
    return 0;
}


USHORT CTOS_LCDTPrintXY(USHORT usX, USHORT usY, UCHAR* pbBuf)
{
    JNIEnv *tenv;
    jshort jsusX;
    jshort jsusY;

    LOGD("---JNI AP CB--- CTOS_LCDTPrintXY\n");
	
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        jsusX = (jshort) usX;
        jsusY = (jshort) usY;
        jstring str = (*tenv)->NewStringUTF(tenv, (char*)pbBuf);
        (*tenv)->CallLongMethod(tenv, g_callback_obj_ctos, g_callback_mid_lcd, jsusX, jsusY, str);
	 (*tenv)->DeleteLocalRef(tenv, str);
    }



    return 0;
}


USHORT CTOS_LCDTPrintAligned(USHORT usY, UCHAR* pbBuf, BYTE bMode)
{
    JNIEnv *tenv;
    jshort jsusY;
    jbyte jbbMode;

    LOGD("---JNI AP CB--- CTOS_LCDTPrintAligned\n");
	
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        jsusY = (jshort) usY;
        jbbMode = (jbyte) bMode;
        jstring str = (*tenv)->NewStringUTF(tenv, (char*)pbBuf);
        (*tenv)->CallLongMethod(tenv, g_callback_obj_ctos, g_callback_mid_aligned, jsusY, str, jbbMode);
    }
    return 0;
}
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env;
    g_vm = vm;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }

    return JNI_VERSION_1_6;
}

JNIEXPORT jlong JNICALL Java_com_Source_S1_1MCCPAY_MCCPAY_Main_JNI_1Lib_REG_1CB_1CTOS(
        JNIEnv *env, jobject obj, jobject instance)
{
    int Rtn = 0;

    LOGD("SATURN ---JNI AP CB--- SET CALLBACK \n");

    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, g_callback_obj_ctos);


    (*env)->GetJavaVM(env, &g_JavaVM);
    g_callback_obj_ctos = (*env)->NewGlobalRef(env,instance);
    jclass clz = (*env)->GetObjectClass(env,g_callback_obj_ctos);


    if(clz == NULL)
    {
        //failed to find class
    }

    //test
    //g_callback_mid_lcd = (*env)->GetMethodID(env, clz, "CTOS_LCDTPrintXY", "(SSLjava/lang/String;)J");
    //g_callback_mid_aligned = (*env)->GetMethodID(env, clz, "CTOS_LCDTPrintAligned", "(SLjava/lang/String;B)J");
    //g_callback_mid_clear = (*env)->GetMethodID(env, clz, "CTOS_LCDTClearDisplay", "()J");
    //g_callback_mid_bring2Front = (*env)->GetMethodID(env, clz, "CTOS_bring2Front", "()J");
    //test

    //g_callback_mid_pin_result = env->GetMethodID(clz, "CTOS_PINGetResult", "([B)J");

    JavaVM *javaVM = g_vm;
    jint res = (*javaVM)->GetEnv(javaVM, (void **) &env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);

        }
    }

    if(clz != NULL)
        (*env)->DeleteLocalRef(env, clz);

    return Rtn;
}


int ShowVirtualKeypPad(OUT USHORT *pusKeyPadButtonNum, OUT BYTE *pbKeyPadButtonInfo, OUT USHORT *pusKeyPadButtonInfoLen)
{
    JNIEnv* env = NULL;

    LOGD("---JNI AP CB--- ShowVirtualKeypPad\n");

    //inCallJAVA_GetAmountString(pusKeyPadButtonNum, pbKeyPadButtonInfo, pusKeyPadButtonInfoLen);
    //return d_OK;

    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {

    }

    int buffsize = 1024;
    int i = 0;

    BYTE baInputbuffer[1024];
    BYTE baOutputbuffer[1024];

    //jbyte *by = (jbyte*)baInputbuffer;

    jbyteArray jarray = (*env)->NewByteArray(env, buffsize);

    (*env)->SetByteArrayRegion(env, jarray, 0, buffsize, (jbyte*)baInputbuffer);

    LOGE("buffsize= %d", buffsize);

    (*env)->CallLongMethod(env, g_callback_obj_input, g_callback_mid_showkeypad, jarray);

    //jsize len = env->GetArrayLength (jarray);
    //unsigned char* buf = new unsigned char[len];

    (*env)->GetByteArrayRegion (env, jarray, 0, buffsize, (jbyte*)(baOutputbuffer));

    *pusKeyPadButtonNum = baOutputbuffer[0];

    LOGE("baOutputbuffer[0]= %02X", baOutputbuffer[0]);
    LOGE("baOutputbuffer[1]= %02X", baOutputbuffer[1]);
    LOGE("baOutputbuffer[2]= %02X", baOutputbuffer[2]);

    *pusKeyPadButtonInfoLen = 0;
    *pusKeyPadButtonInfoLen += (baOutputbuffer[1]*256);
    *pusKeyPadButtonInfoLen += (baOutputbuffer[2]);

    LOGE("pusPINPadButtonNum= %d", *pusKeyPadButtonNum);
    LOGE("pusPINPadButtonInfoLen= %d", *pusKeyPadButtonInfoLen);


    //memcpy(pbKeyPadButtonInfo, &baOutputbuffer[3], *pusKeyPadButtonInfoLen);

    return 0;
}


int GetKeyPadDone(void)
{
    JNIEnv *tenv;
    jshort jsusX;
    jshort jsusY;

    LOGD("---JNI AP CB--- GetKeyPadDone\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        (*tenv)->CallLongMethod(tenv, g_callback_obj_input, g_callback_mid_keypaddone);
    }
    return 0;

}


int ShowKeyPadMsg(BYTE digitsNum, BYTE bPINType, BYTE bRemainingCounter)
{
    JNIEnv *tenv;
    jbyte digits;


    LOGD("---JNI AP CB--- ShowKeyPadMsg\n");
    if((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &tenv, NULL) != JNI_OK)
    {
        // attachCurrentThread() failed.
    }
    else {
        digits = (jbyte) digitsNum;
        (*tenv)->CallLongMethod(g_callback_obj_input, g_callback_mid_keypadmsg, digits, bPINType, bRemainingCounter);
    }
    return 0;

}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Trans_AmountEntryActivity_vdCTOSS_1FormatAmountUI(JNIEnv *env,
                                                                              jobject instance,
                                                                              jstring szFmt_,
                                                                              jstring szInAmt_) {
    const char *szFmt = (*env)->GetStringUTFChars(env, szFmt_, 0);
    const char *szInAmt = (*env)->GetStringUTFChars(env, szInAmt_, 0);
    char temp[24+1];

    // TODO
    memset(temp, 0, sizeof(temp));
    vdCTOSS_FormatAmount(szFmt, szInAmt, temp);

    (*env)->ReleaseStringUTFChars(env, szFmt_, szFmt);
    (*env)->ReleaseStringUTFChars(env, szInAmt_, szInAmt);

    return (*env)->NewStringUTF(env, temp);
}

int inCallJAVA_UserConfirmMenuInvandAmt(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
	unsigned char uszBuffer[512+1];
	int inRet = 0;


	vdDebug_LogPrintf("saturn =====inCallJAVA_UserConfirmMenu=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
	//else
		//pbDispMsg = "Tine: UserConfirm activity";

	vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	vdDebug_LogPrintf("saturn jstring[%s]", uszBuffer);

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
	vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenuInvandAmt", "(Ljava/lang/String;)Ljava/lang/String;");

	vdDebug_LogPrintf("saturn jstring[%s]", "UserConfirmMenuInvandAmt");

	jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

	jbyte* str = NULL;
	str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
	if (str!=NULL)
	{
		vdDebug_LogPrintf("%s", str);
		*pbOutLen = strlen(str);
		vdDebug_LogPrintf("strcpy");
		strcpy(pbOutStr, str);

		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, result, str);

	}
	else
		*pbOutLen = 0;

	(*env)->DeleteLocalRef(env, jstr);
	(*env)->DeleteLocalRef(env, clazz);
	(*env)->DeleteLocalRef(env, result);

	vdDebug_LogPrintf("end inCallJAVA_UserConfirmMenu");
	return d_OK;
}



JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1REPORTS(JNIEnv *env, jobject instance) {

    // TODO
    int inRetVal = 0;
    BYTE outbuf[d_MAX_IPC_BUFFER];
    USHORT out_len = 0;

    char cwd[1024];

    //inTCTRead(1);
    //strTCT.byRS232DebugPort = 8;
    //inTCTSave(1);
     if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("inReportSelecion");
    inReportSelecion();

    //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}


JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MultiAP_1SubAP_inCTOSS_1SubAPMain(JNIEnv *env, jobject instance){

    int inRetVal = 0;

    char szDisplayBuf[30];
    BYTE baAmount[20];
    ULONG ulAmount = 0L;


    vdDebug_LogPrintf("saturn inCTOSS_1SubAPMain 1");
   // inTCTRead(1);
//    strTCT.byRS232DebugPort = 8;
     if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);
    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);

    vdDebug_LogPrintf("saturn inCTOSS_1SubAPMain 2");
//	vdCTOS_DeleteReversal();
    inRetVal = main(0, NULL);
/*
//    ProcessingUI();
	InputAmountEx(1, 8, szDisplayBuf, 2, 0x00, baAmount, &ulAmount, 3000, 0);
//    ProcessingUI();
	usCTOSS_Confirm("123");
//    ProcessingUI();
//	sleep(1500);

	CTOS_LCDTClearDisplay();
	CTOS_LCDTPrintXY(1, 7, "sending...	   ");

//	sleep(1500);
*/

     //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);
	
    return 0;
    

}

/*
JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1SubAPMain(JNIEnv *env,
                                                                         jobject instance) {


	int inRetVal = 0;
	
	char szDisplayBuf[30];
	BYTE baAmount[20];
	ULONG ulAmount = 0L;

	vdDebug_LogPrintf("saturn mainactivity inCTOSS_1SubAPMain test 1");
	
	inTCTRead(1);
	//	  strTCT.byRS232DebugPort = 8;
	
	jint rs = (*env)->GetJavaVM(env, &jvm);
	
	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);
	
		vdDebug_LogPrintf("saturn main inCTOSS_1SubAPMain");
	//	vdCTOS_DeleteReversal();
		inRetVal = main(0, NULL);

}*/

JNIEXPORT jbyteArray JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOS_1GetBuffer(JNIEnv *env,
                                                                        jobject instance) {

	int size = 720*1280*4 + 54;
	BYTE* buffer = NULL;

	//vdDebug_LogPrintf("SATURN CTOS_GETGBBuffer");
	while(1)
	{
		buffer = CTOS_GETGBBuffer();

	
		//vdDebug_LogPrintf("SATURN %s", buffer );
		if(buffer == NULL)
			continue;
		break;
	}
	jbyteArray array;
	array = (*env)->NewByteArray(env, size);
	(*env)->SetByteArrayRegion(env, array, 0, size, (jbyte*)buffer);
	buffer = NULL;
	return array;

}

int inCallJAVA_LCDDisplay(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_LCDDisplay=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "LCDDisplay", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
    {
		*pbOutLen = 0;
		strcpy(pbOutStr, "OK");
    }
        

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_LCDDisplay");
    return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1CARDENTRYTIMEOUT(JNIEnv *env,
                                                                                jobject instance) {


    // TODO
    vdDebug_LogPrintf("inCTOSS_CARDENTRYTIMEOUT.............");
    CTOS_KBDBufPut('T'); // patrick hit cancel 20190406
    ing_KeyPressed = 'T';
    vdDebug_LogPrintf("ing_KeyPressed = [%d]", ing_KeyPressed);
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1MultiAP_1CheckMainAPStatus(
        JNIEnv *env, jobject instance) {
    inTCTRead(1);

    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        vdDebug_LogPrintf("inMultiAP_CheckMainAPStatus d_OK[%d]",1);
        return 1;
    }
    else
    {
        vdDebug_LogPrintf("inMultiAP_CheckMainAPStatus d_NO[%d]",0);
        return 0;
    }
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1SubAPMain(JNIEnv *env,
                                                                         jobject instance) {

	int inRetVal = 0;
	
	char szDisplayBuf[30];
	BYTE baAmount[20];
	ULONG ulAmount = 0L;

	vdDebug_LogPrintf("saturn mainactivity inCTOSS_1SubAPMain test 1");
	vdCTOSS_ReleaseAllDrvHandle();
	//inTCTRead(1);
	//	  strTCT.byRS232DebugPort = 8;
        if(activityClass != NULL)
            (*env)->DeleteGlobalRef(env, activityClass);
        if(activityObj != NULL)
            (*env)->DeleteGlobalRef(env, activityObj);
	
	jint rs = (*env)->GetJavaVM(env, &jvm);
	
	jclass cls = (*env)->GetObjectClass(env, instance);
	activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
	activityObj = (*env)->NewGlobalRef(env, instance);
	
		vdDebug_LogPrintf("saturn main inCTOSS_1SubAPMain");
	//	vdCTOS_DeleteReversal();
    inRetVal = main(0, NULL);

	 //release memory
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);
	vdCTOSS_ReleaseAllDrvHandle();
}

int inCallJAVA_BatchReviewUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_BatchReviewUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
  
  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "BatchReviewUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "BatchReviewUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_BatchReviewUI");
    return d_OK;
}

int inCallJAVA_BatchReviewUI_PHQR(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_BatchReviewUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
  
  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "BatchReviewUIPHQR", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "BatchReviewUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_BatchReviewUI");
    return d_OK;
}


int inCallJAVA_UserConfirmOKMenu(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmOKMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    //else
    //pbDispMsg = "Tine: UserConfirm activity";

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmOKMenu", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmOKMenu");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmOKMenu");
    return d_OK;
}

/* Analyze a string for str1,str2 */
int SplitString(char *str, char *str1, char *str2, char c)
{
    int i,j;

    for (j=0,i=0;str[i]!='\0'&&str[i]!=c;j++,i++)
        str1[j]=str[i];
    str1[j]='\0';

    if (str[i]=='\0') str2[0]='\0';
    else
    {
        for (j=0,i++;str[i]!='\0';j++,i++)
            str2[j]=str[i];
        str2[j]='\0';
    }

    return strlen(str2);
}

int inCallJAVA_inGetPIN_With_3DESDUKPTEx(USHORT KeySet,  USHORT KeyIndex,  BYTE* pInData, BYTE* szPINBlock, BYTE* szKSN, BYTE* szIndex, USHORT pinBypassAllow)
{
	unsigned char uszpInData[70+1];
	unsigned char PinBlockKsn[20+1];
	int inRet = 0;

	vdDebug_LogPrintf("saturn inCallJAVA_inGetPIN_With_3DESDUKPTEx Start KeySet[%d] KeyIndex[%d]", KeySet, KeyIndex);

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("saturn jint[%d] *env[%x]", rs, *env);


	memset(uszpInData, 0x00, sizeof(uszpInData));
	vdDebug_LogPrintf("saturn strlen(pInData)[%d]", strlen(pInData));


	if (strlen(pInData)>0)
		strcpy(uszpInData, pInData);

	vdDebug_LogPrintf("saturn uszpInData[%s]", uszpInData);

	jstring Inputstr = (*env)->NewStringUTF(env, uszpInData);
	vdDebug_LogPrintf("saturn jstring[%s]", uszpInData);

	vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetPIN_With_3DESDUKPTEx", "(IILjava/lang/String;I)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "GetIPPPinEx");

	jobject Result = (*env)->CallObjectMethod(env, activityObj, methodID, KeySet, KeyIndex, Inputstr, pinBypassAllow);
	
	jbyte* Outstr = NULL;

	
	if(NULL != Result)
	{
	   Outstr = (*env)->GetStringUTFChars(env,(jstring) Result, NULL);
	}
	else
	{
		vdDebug_LogPrintf("Result is NULL");
	}

	
	if (Outstr!=NULL)
	{

		memset(uszpInData, 0x00, sizeof(uszpInData));

		vdDebug_LogPrintf("%s", Outstr);
		SplitString(Outstr, szIndex, PinBlockKsn, '*');
		vdDebug_LogPrintf("szIndex[%s]", szIndex);
		vdDebug_LogPrintf("PinBlockKsn[%s]", PinBlockKsn);
		SplitString(PinBlockKsn, szPINBlock, szKSN, '|');
		vdDebug_LogPrintf("szPINBlock[%s]", szPINBlock);
		vdDebug_LogPrintf("szKSN[%s]", szKSN);
		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, Result, Outstr);
        if(strlen(szPINBlock) == 0)
            inRet = d_NO;
        else
            inRet = d_OK;
	}
	else {
		*szPINBlock = 0;
		*szKSN = 0;
        vdDebug_LogPrintf("PINBYPASS");
        inRet = d_NO;
	}

	(*env)->DeleteLocalRef(env, Inputstr);
	(*env)->DeleteLocalRef(env, Result);

	vdDebug_LogPrintf("end inCallJAVA_inGetPIN_With_3DESDUKPTEx[%d]", inRet);
	return inRet;
}

int inCallJAVA_usGetRootFS(BYTE *pbOutStr, BYTE *pbOutLen)

{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_usGetSerialNumber=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    //if (strlen(pbDispMsg)>0)
    //    strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);
	strcpy(uszBuffer, "test");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetRootFS", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetSerialNumber");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_usGetSerialNumber");
    return d_OK;
}

int inCallJAVA_PrintReceiptUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_PrintReceiptUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("saturn jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "ReceiptOnScreen", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "ReceiptOnScreen");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_PrintReceiptUI");
    return d_OK;
}

int inCallJAVA_EliteReceiptUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_EliteReceiptUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "EliteReceiptOnScreen", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "EliteReceiptOnScreen");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_EliteReceiptUI");
    return d_OK;
}


int inCallJAVA_UserConfirmCard(BYTE *pbCardDisplay, BYTE *pbOutBuf, BYTE *pbLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmCard=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbCardDisplay)>0)
        strcpy(uszBuffer, pbCardDisplay);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmCard", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmCard");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    vdDebug_LogPrintf("jstring[%s]", "CallObjectMethod");
    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutBuf, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmCard");
    return d_OK;
}

int is_utf8(const char * string)
{

    vdDebug_LogPrintf("AAA -is_utf8 start ");
    if(!string)
        return 0;
    vdDebug_LogPrintf("AAA -is_utf8 1 ");
    const unsigned char * bytes = (const unsigned char *)string;
    while(*bytes)
    {
        if( (// ASCII
             // use bytes[0] <= 0x7F to allow ASCII control characters
                bytes[0] == 0x09 ||
                bytes[0] == 0x0A ||
                bytes[0] == 0x0D ||
                (0x20 <= bytes[0] && bytes[0] <= 0x7E)
            )
        ) {
            bytes += 1;
            continue;
        }

        if( (// non-overlong 2-byte
                (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF)
            )
        ) {
            bytes += 2;
            continue;
        }

        if( (// excluding overlongs
                bytes[0] == 0xE0 &&
                (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// straight 3-byte
                ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                    bytes[0] == 0xEE ||
                    bytes[0] == 0xEF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// excluding surrogates
                bytes[0] == 0xED &&
                (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            )
        ) {
            bytes += 3;
            continue;
        }

        if( (// planes 1-3
                bytes[0] == 0xF0 &&
                (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// planes 4-15
                (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// plane 16
                bytes[0] == 0xF4 &&
                (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            )
        ) {
            bytes += 4;
            continue;
        }
         vdDebug_LogPrintf("AAA -is_utf8 start return 0");
        return 0;
    }
     vdDebug_LogPrintf("AAA -is_utf8 return 1 ");
    return 1;
}

int inCallJAVA_getAppPackageInfo(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
	unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_getAppPackageInfo=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");


    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "AppPackageInfo", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "AppPackageInfo");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_getAppPackageInfo");
    return d_OK;
}

int inCallJAVA_DisplayBox(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DisplayBox=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayBox", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayBox");
    return d_OK;
}

int inCallJAVA_DisplayErrorMsg2(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DisplayErrorMsg2=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayErrorMsg2", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayErrorMsg2");
    return d_OK;
}

int inCallJAVA_usGetConnectionStatus(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)

{
    unsigned char uszBuffer[128+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_usGetSerialNumber=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    memset(uszBuffer, 0x00, sizeof(uszBuffer));
    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "fGetConnectStatus", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("saturn jstring[%s]", "fGetConnectStatus");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end fGetConnectStatus");
    return d_OK;
}


int inCallJAVA_UserConfirmMenu2(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[d_MAX_IPC_BUFFER];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_UserConfirmMenu=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


  
    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

	
	vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmMenu2", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmMenu2");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("saturn ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("saturn end inCallJAVA_UserConfirmMenu2");
    return d_OK;
}

int inCallJAVA_usGetSerialNumber(BYTE *pbOutStr, BYTE *pbOutLen) {
    unsigned char uszBuffer[128 + 1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_usGetSerialNumber=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    //if (strlen(pbDispMsg)>0)
    //    strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);
    strcpy(uszBuffer, "test");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetSerialNumber",
                                             "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetSerialNumber");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte *str = NULL;
    str = (*env)->GetStringUTFChars(env, (jstring) result, NULL);
    if (str != NULL) {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    } else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_usGetSerialNumber");
    return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1ProcessCfgExpress(JNIEnv *env,
                                                                                   jobject instance) {

    // TODO
    vdDebug_LogPrintf("saturn RUN AAA - inCTOSS_ProcessCfgExpress");
    inCTOSS_ProcessCfgExpress();
    return d_OK;
}

int inCallJAVA_MenuTransactions(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_MenuTransactions=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("pbDispMsg[%s]", pbDispMsg);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);
    else
        strcpy(uszBuffer, "PASS IN MSG");

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("SATURN jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("SATURN jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "MenuTransactions", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "MenuTransactions");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
    //rs = (*jvm)->DetachCurrentThread(jvm);

    jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_MenuTransactions");
    return d_OK;
}

JNIEXPORT jstring JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_GetAppHeaderDetails(JNIEnv *env, jobject instance) {

	// TODO
	char szTrxn[20][20];
	char szHeader[256+1];
	int inRet, inSeek, inLoop;
	char reg_status[2];
	char idleMode[2];

	inTCTRead(1);
	vdDebug_LogPrintf("strTCT.fRegister=[%d]", strTCT.fRegister);
	sprintf(reg_status, "%d", strTCT.fRegister);
	vdDebug_LogPrintf("reg_status=[%s]", reg_status);
	sprintf(idleMode, "%d", strTCT.inIdleMode);
	strTCT.fShareComEnable = 1;
	inRet = inTCTSave(1);
	vdDebug_LogPrintf("inRet = [%d]", inRet);

	memset(szTrxn, 0x00, sizeof(szTrxn));

	strTCT.inMMTid = 19;
	inSeek = strTCT.inMMTid;
	vdDebug_LogPrintf("strTCT.inMMTid::inSeek = [%d]", inSeek);
	//inRet = inMMTRead(inSeek, szTrxn);

	// sidumili: AppHeaderDetails to load MMT record per apps
	switch (inSeek)
	{
		case 1: // credit
			inMMTReadRecord(19,1);
		break;
		case 2: // bancnet
			inMMTReadRecord(12,1);
		break;
		case 3: // mccpay
			inMMTReadRecord(20,1);
		break;
		default:
			inMMTReadRecord(19,1);
			break;
	}
	
	memset(szHeader, 0x00, sizeof(szHeader));
	strcpy(szHeader, strTCT.szAppVersion);
	strcat(szHeader, " \n");
	strcat(szHeader, &strMMT[0].szMerchantName);
	strcat(szHeader, " \n");
	strcat(szHeader, &strMMT[0].szMID);
	strcat(szHeader, " \n");
	strcat(szHeader, &strMMT[0].szTID);
	strcat(szHeader, " \n");
	strcat(szHeader, reg_status);
	strcat(szHeader, " \n");
	strcat(szHeader, idleMode);

	vdDebug_LogPrintf("szHeader %s %d", szHeader, strlen(szHeader));
	vdRemoveCard();

	return (*env)->NewStringUTF(env, szHeader);

}


int inCallJAVA_UserConfirmQRCode(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmQRCode=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmQRCode", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmQRCode");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmQRCode");
    return d_OK;
}

int inCallJAVA_DisplayStatusBox(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[528];
    int inRet = 0;


    vdDebug_LogPrintf("=====inCallJAVA_DisplayStatusBox=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);


    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
	
    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "DisplayStatusBox", "(Ljava/lang/String;)Ljava/lang/String;");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    //(*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_DisplayStatusBox");
    return d_OK;
}

int inCallJAVA_TransDetailListViewUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[250+1];
    int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_TransDetailListViewUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
  
  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "TransDetailListViewUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "TransDetailListViewUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_TransDetailListViewUI");
    return d_OK;
}

int inCallJAVA_usSetDateTime(BYTE *pbDispMsg, BYTE *pbOutStr_datetime, BYTE *pbOutLen)
{
	unsigned char uszBuffer[512+1];
	int inRet = 0;


	vdDebug_LogPrintf("=====inCallJAVA_usCARDENTRY=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



	if (strlen(pbDispMsg)>0)
		strcpy(uszBuffer, pbDispMsg);


	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	vdDebug_LogPrintf("jstring[%s]", uszBuffer);

	//jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
	//vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "usSetDateAndTime", "(Ljava/lang/String;)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "usSetDateTime");

	jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

	jbyte* str = NULL;
	str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
	if (str!=NULL)
	{
		vdDebug_LogPrintf("%s", str);
		*pbOutLen = strlen(str);
		vdDebug_LogPrintf("strcpy");
		strcpy(pbOutStr_datetime, str);

		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, result, str);

	}
	else
	{
		*pbOutLen = 0;
		//strcpy(pbOutStr, "OK");
	}


	(*env)->DeleteLocalRef(env, jstr);
	//(*env)->DeleteLocalRef(env, clazz);
	(*env)->DeleteLocalRef(env, result);

	vdDebug_LogPrintf("end inCallJAVA_usCARDENTRY");
	return d_OK;
}

int inCallJAVA_ScanQRCodeUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_ScanQRCodeUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
  
  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "ScanQRCodeUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "ScanQRCodeUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_ScanQRCodeUI");
    return d_OK;
}

int inCallJAVA_PrintQRCodeUI(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_PrintQRCodeUI=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);
  
  if (strlen(pbDispMsg)>0)
	  strcpy(uszBuffer, pbDispMsg);
	
	vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "PrintQRCodeUI", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "PrintQRCodeUI");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL; 
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); 
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_PrintQRCodeUI");
    return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1Get1stInitFlag(JNIEnv *env, jobject thiz) {
		int ret = 0;
		inTCTRead(1);
		ret = strTCT.fFirstInit;
		vdDebug_LogPrintf("inCTOSS_1Get1stInitFlag[%d].............", ret);

		return ret;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1GetDLNotFinishedFlag(JNIEnv *env,
                                                                            jobject thiz) {
    int ret = 0;
    inTCTRead(1);
    ret = strTCT.byDLNotFinished;
    vdDebug_LogPrintf("inCTOSS_1Get1stInitFlag[%d].............", ret);

    return ret;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1BackupDataScript(JNIEnv *env,
                                                                                  jobject thiz) {
    // TODO: implement inCTOSS_BackupDataScript()

		vdDebug_LogPrintf("inCTOSS_BackupDataScript.............");
		char szOutPrmFile[100+1];

		memset(szOutPrmFile, 0, sizeof(szOutPrmFile));
	//	  inCTOSS_BackupDataScript("/data/data/pub/com.Source.S1_UOB.UOB.tbd", szOutPrmFile);
	//	  inCTOSS_BackupDataScript("/data/data/com.Source.S1_UOB.UOB/com.Source.S1_UOB.UOB.tbd", szOutPrmFile);
		inCTOSS_BackupDataScript("/data/data/com.Source.S1_MCC.MCC/com.Source.S1_MCC.MCC.tbd", szOutPrmFile);
		return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1SetDownloadedNotFinishedFlag(
        JNIEnv *env, jobject thiz, jint value) {
    // TODO: implement inCTOSS_SetDownloadedNotFinishedFlag()
    vdDebug_LogPrintf("inCTOSS_SetDownloadedNotFinishedFlag,byDLNotFinished[%d],value[%d].............",strTCT.byDLNotFinished, value);
    strTCT.byDLNotFinished = value;

    //if(value == 0)
    //    strTCT.fFirstInit = 1;

    inTCTSave(1);
    return 0;
}

int inCallJAVA_UserConfirmQRCodePHQR(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen)
{
    unsigned char uszBuffer[512+1];
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_UserConfirmQRCode=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);

    vdDebug_LogPrintf("uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "UserConfirmQRCodePHQR", "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "UserConfirmQRCode");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte* str = NULL;
    str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
    if (str!=NULL)
    {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    }
    else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_UserConfirmQRCode");
    return d_OK;
}

int inCallJAVA_HMAC256(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen) {
    unsigned char uszBuffer[500 + 1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_HMAC256=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetHMAC256",
                                             "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetRSA");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte *str = NULL;
    str = (*env)->GetStringUTFChars(env, (jstring) result, NULL);
    if (str != NULL) {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    } else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_HMAC256");
    return d_OK;
}

int inCallJAVA_RSA(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen) {
    unsigned char uszBuffer[500 + 1];
    int inRet = 0;


    vdDebug_LogPrintf("saturn =====inCallJAVA_RSA=====");

    JNIEnv *env;
    jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    // Use the env pointer...
    vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);



    if (strlen(pbDispMsg)>0)
        strcpy(uszBuffer, pbDispMsg);


    vdDebug_LogPrintf("saturn uszBuffer[%s]", uszBuffer);

    jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
    vdDebug_LogPrintf("jstring[%s]", uszBuffer);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("saturn jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "GetRSA",
                                             "(Ljava/lang/String;)Ljava/lang/String;");

    vdDebug_LogPrintf("jstring[%s]", "GetRSA");

    jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);

    jbyte *str = NULL;
    str = (*env)->GetStringUTFChars(env, (jstring) result, NULL);
    if (str != NULL) {
        vdDebug_LogPrintf("%s", str);
        *pbOutLen = strlen(str);
        vdDebug_LogPrintf("strcpy");
        strcpy(pbOutStr, str);

        vdDebug_LogPrintf("ReleaseStringUTFChars");
        (*env)->ReleaseStringUTFChars(env, result, str);

    } else
        *pbOutLen = 0;

    (*env)->DeleteLocalRef(env, jstr);
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, result);

    vdDebug_LogPrintf("end inCallJAVA_usGetSerialNumber");
    return d_OK;
}



int inCallJAVA_SendEcrResponse()
{
    vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 1");
	JNIEnv *env;
    if(FALSE)
    {
        vdDebug_LogPrintf("=====Test code, don't send ecr resp=====");
        return d_OK;
    }

	vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 2");

	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

	vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 3");
    jstring jstr = (*env)->NewStringUTF(env, "dummy");

	vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 4");

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
	vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

    jmethodID methodID = (*env)->GetMethodID(env, activityClass, "vdMCCPaysendBackEcrResponse", "(Ljava/lang/String;)V");

	vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 5");

    (*env)->CallVoidMethod(env, activityObj, methodID, jstr);

	vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 6");
    vdDebug_LogPrintf("Clear buff");
    (*env)->DeleteLocalRef(env, jstr);
	vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 7");

   vdDebug_LogPrintf("AAA3 =====inCallJAVA_SendEcrResponse===== 8");
    return d_OK;
}

JNIEXPORT jint
JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_EcrCommandReceiver_vdEcrSetupInstance(JNIEnv *env, jobject instance) {
    vdDebug_LogPrintf("=====vdEcrSetupInstance=====");
    if(ecrClass != NULL)
        (*env)->DeleteGlobalRef(env, ecrClass);
    if(ecrObj != NULL)
        (*env)->DeleteGlobalRef(env, ecrObj);

    jint rs = (*env)->GetJavaVM(env, &ecrjvm);
    jclass cls = (*env)->GetObjectClass(env, instance);
    ecrClass = (jclass) (*env)->NewGlobalRef(env, cls);
    ecrObj = (*env)->NewGlobalRef(env, instance);
    vdDebug_LogPrintf("=====vdEcrSetupInstance exit=====");
    if(cls != NULL)
        (*env)->DeleteLocalRef(env, cls);

    return 0;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1GetPHQRInit(JNIEnv *env,
                                                                         jobject thiz) {
    // //TODO: implement inCTOSS_GetPHQRInit()
    
	int ininit = get_env_int("PHQRINIT");

	return ininit;
}

int inCallJAVA_switchGPRS()
{
    int inRet = 0;

    vdDebug_LogPrintf("=====inCallJAVA_switchGPRS=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");
    vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCCPAY/MCCPAY/Main/MainActivity");

	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "switchGPRS", "()V");

	vdDebug_LogPrintf("jstring[%s]", "switchGPRS");
    if (methodID)
    {
        vdDebug_LogPrintf("GetMethodID[%s]", "switchGPRS");
        (*env)->CallVoidMethod(env, activityObj, methodID);
    }

    if (clazz)
        (*env)->DeleteLocalRef(env, clazz);

    return inRet;
}

#if 0
JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1GetEnvInt(JNIEnv *env, jobject instance, jstring tag) {
    // TODO: implement inCTOSS_Get1stInitFlag()
    int ret = 0;


#if 0
    if(activityClass != NULL)
        (*env)->DeleteGlobalRef(env, activityClass);
    if(activityObj != NULL)
        (*env)->DeleteGlobalRef(env, activityObj);

    jint rs = (*env)->GetJavaVM(env, &jvm);

    jclass cls = (*env)->GetObjectClass(env, instance);
    activityClass = (jclass) (*env)->NewGlobalRef(env, cls);
    activityObj = (*env)->NewGlobalRef(env, instance);
#endif

    const char *szFmt = (*env)->GetStringUTFChars(env, tag, 0);

	ret=get_env_int(szFmt);
    //inTCTRead(1);
    //ret = strTCT.fFirstInit;
    vdDebug_LogPrintf("inCTOSS_1GetEnvInt[%d].............", ret);

    return ret;
}
#endif

int inCallJAVA_TLE_Encrypt(BYTE *pbInStr, BYTE *pbOutStr, int inLen)
{
	unsigned char uszBuffer[2048+1];
	int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_TLE_Encrypt=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStr[%s]", pbInStr);
	//vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStrKey[%s]", pbInStrKey);
	
	if (strlen(pbInStr)>0)
	{
		strcpy(uszBuffer, pbInStr);
	}
	//else
	//pbDispMsg = "Tine: UserConfirm activity";

	vdDebug_LogPrintf("1.uszBuffer[%s]", uszBuffer);

	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
	vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "TLE_Encrypt", "(Ljava/lang/String;)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "GetAmountString");

	jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
	//rs = (*jvm)->DetachCurrentThread(jvm);

	jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); 
// should be released but what a heck, it's a tutorial :)
	str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	if (str!=NULL)
	{
		vdDebug_LogPrintf("%s", str);
		//*pbAmtLen = strlen(str);
		vdDebug_LogPrintf("strcpy");
		strcpy(pbOutStr, str);

		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, result, str);

	}
	//else
		//*pbAmtLen = 0;

	(*env)->DeleteLocalRef(env, jstr);
	(*env)->DeleteLocalRef(env, clazz);
	(*env)->DeleteLocalRef(env, result);

	vdDebug_LogPrintf("end inCallJAVA_TLE_Encrypt");
	return d_OK;
}

int inCallJAVA_TLE_Decrypt(BYTE *pbInStr, BYTE *pbOutStr, int inLen)
{
	unsigned char uszBuffer[2048+1];
	int inRet = 0;

	vdDebug_LogPrintf("=====inCallJAVA_TLE_Decrypt=====");

	JNIEnv *env;
	jint rs = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
	// Use the env pointer...
	vdDebug_LogPrintf("jint[%d] *env[%x]", rs, *env);

    vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStr[%s]", pbInStr);
	//vdDebug_LogPrintf("inCallJAVA_GetMAC.pbInStrKey[%s]", pbInStrKey);
	
	if (strlen(pbInStr)>0)
	{
		strcpy(uszBuffer, pbInStr);
	}
	//else
	//pbDispMsg = "Tine: UserConfirm activity";

	vdDebug_LogPrintf("1.uszBuffer[%s]", uszBuffer);

	jstring jstr = (*env)->NewStringUTF(env, uszBuffer);
	vdDebug_LogPrintf("jstring[%s]", "This this Pass in string data to Java");

	jclass clazz = (*env)->FindClass(env, "com/Source/S1_MCC/MCC/Main/MainActivity");
	vdDebug_LogPrintf("jstring[%s]", "com/Source/S1_MCC/MCC/Main/MainActivity");

//	jmethodID methodID = (*env)->GetMethodID(env, clazz, "rndGetNumStr", "(Ljava/lang/String;)Ljava/lang/String;");
	jmethodID methodID = (*env)->GetMethodID(env, activityClass, "TLE_Decrypt", "(Ljava/lang/String;)Ljava/lang/String;");

	vdDebug_LogPrintf("jstring[%s]", "GetAmountString");

	jobject result = (*env)->CallObjectMethod(env, activityObj, methodID, jstr);
	//rs = (*jvm)->DetachCurrentThread(jvm);

	jbyte* str = NULL; //(*env)->GetStringUTFChars(env,(jstring) result, NULL); 
// should be released but what a heck, it's a tutorial :)
	str = (*env)->GetStringUTFChars(env,(jstring) result, NULL); // should be released but what a heck, it's a tutorial :)
	if (str!=NULL)
	{
		vdDebug_LogPrintf("%s", str);
		//*pbAmtLen = strlen(str);
		vdDebug_LogPrintf("strcpy");
		strcpy(pbOutStr, str);

		vdDebug_LogPrintf("ReleaseStringUTFChars");
		(*env)->ReleaseStringUTFChars(env, result, str);

	}
	//else
		//*pbAmtLen = 0;

	(*env)->DeleteLocalRef(env, jstr);
	(*env)->DeleteLocalRef(env, clazz);
	(*env)->DeleteLocalRef(env, result);

	vdDebug_LogPrintf("end inCallJAVA_TLE_Decrypt");
	return d_OK;
}

JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1GetEnvInt(JNIEnv *env, jobject instance, jstring tag) {
    int ret = 0;

    const char *szFmt = (*env)->GetStringUTFChars(env, tag, 0);

	ret=get_env_int(szFmt);
    //inTCTRead(1);
    //ret = strTCT.fFirstInit;
    vdDebug_LogPrintf("inCTOSS_1GetEnvInt[%d].............", ret);

    return ret;
}


JNIEXPORT jint JNICALL
Java_com_Source_S1_1MCCPAY_MCCPAY_Main_MainActivity_inCTOSS_1CreatefsdataFolder(JNIEnv *env,
																		 jobject thiz) {
   // TODO: implement inCTOSS_CreatefsdataFolder()
   #define d_FILE_EXIST 0x01

   BYTE pbaFileName[20] = {"PatTesting.txt"};
   BYTE bStorageType = 0;
   ULONG ulFileSize = 0, ulHandle;
   int ret = 0;

   vdDebug_LogPrintf("CreatefsdataFolder");
   
   /* Get the file size with specific file name.
	* If Get file size > 0, the file is already existed 	 */
   ret = CTOS_FileGetSize(pbaFileName,&ulFileSize);

   if (ulFileSize > 0)
	   return d_FILE_EXIST;

   if ((ret != d_OK) && (ret != d_FS_FILE_NOT_FOUND))
	   return ret;

   /* Open a file and return a number called a file handle.
	* If the specified file name does not exist , it will be created first. */
   ret =CTOS_FileOpen(pbaFileName , bStorageType , &ulHandle);

   if (ret == d_OK)
	   CTOS_FileClose(ulHandle);
}


