#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <vwdleapi.h>
#include <sqlite3.h>
#include <CTOSInput.h>

#include "..\Includes\POSTypedef.h"
#include "..\Debug\Debug.h"

#include "..\Includes\POSMain.h"
#include "..\Includes\POSTrans.h"
#include "..\Includes\POSHost.h"
#include "..\Includes\POSSale.h"
#include "..\Includes\POSbatch.h"
#include "..\ui\Display.h"
#include "..\Includes\V5IsoFunc.h"
#include "..\Accum\Accum.h"
#include "..\print\Print.h"
#include "..\Comm\V5Comm.h"
#include "..\Includes\MultiApLib.h"
#include "..\Aptrans\MultiAptrans.h"
#include "..\Includes\Wub_lib.h"
#include "..\Database\DatabaseFunc.h"
#include "..\ApTrans\MultiShareEMV.h"
#include "..\Includes\CardUtil.h"
#include "..\Includes\POSSetting.h"
#include "..\POWRFAIL\POSPOWRFAIL.h"
#include <ctos_qrcode.h>
#include "..\DigitalWallet\POSPhqr.h"
#include "..\DigitalWallet\POSXMLFunction.h"
#include "../md5/CRCmd5.h"

#include <openssl\rsa.h>
#include <openssl\sha.h>
#include <openssl\pem.h>
#include <openssl\Err.h>
#include <openssl/evp.h>
#include <ctos_qrcode.h>
#include <curl\curl.h>
#include <curl/easy.h>

#include "..\Includes\CTOSInput.h"


//extern BOOL fTimeOutFlag;
//extern BOOL fECRTxnFlg;
extern BOOL fECRBuildSendOK;
extern BOOL ErmTrans_Approved;
extern BOOL fECRApproved;


//for S1F
//#define PRIVATE_PEM "/data/data/pub/private-key.pem"
//#define PUBLIC_PEM "/data/data/pub/public-key.pem"
#define PRIVATE_PEM "/data/data/pub/PRIVATE.PEM"
#define PUBLIC_PEM "/data/data/pub/PUBLIC.PEM"
#define PUBLIC_KEY	1
#define PRIVATE_KEY	0

BYTE szGlobalSignature[2000];
//#define bool    unsigned char 
BYTE szQRString[2048+1];
BYTE szGlobalRefNumber[20+1];


RSA* createRSAWithFilename(char * filename,int public)
{
    FILE * fp = fopen(filename,"r");

	vdDebug_LogPrintf("*** createRSAWithFilename ***");
	
    if(fp == NULL)
    {
        vdDebug_LogPrintf("Unable to open file %s",filename);
        return NULL;    
    }
    RSA *rsa= RSA_new() ;
 
    if(public)
    {
    	vdDebug_LogPrintf("PUBLIC KEY");
        rsa = PEM_read_RSA_PUBKEY(fp, &rsa,NULL, NULL);
    }
    else
    {
    	vdDebug_LogPrintf("PRIVATE KEY");
        rsa = PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);
    }
 
    return rsa;
}

bool RSAVerifySignature( RSA* rsa,
                         unsigned char* MsgHash,
                         size_t MsgHashLen,
                         const char* Msg,
                         size_t MsgLen,
                         bool* Authentic) {
  *Authentic = 0;
  EVP_PKEY* pubKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(pubKey, rsa);
  //EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();
  EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_new();

  if (EVP_DigestVerifyInit(m_RSAVerifyCtx,NULL, EVP_sha256(),NULL,pubKey)<=0) {
    return 0;
  }
  
  if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
    return 0;
  }
  
  int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
  
  if (AuthStatus==1) {
    *Authentic = 1;
    // EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return 1;
  } else if(AuthStatus==0){
    *Authentic = 0;
    // EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return 1;
  } else{
    *Authentic = 0;
    // EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return 0;
  }
}

size_t calcDecodeLength(const char* b64input) {
  size_t len = strlen(b64input), padding = 0;

  if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
    padding = 2;
  else if (b64input[len-1] == '=') //last char is =
    padding = 1;
  return (len*3)/4 - padding;
}

void Base64Encode( const unsigned char* buffer,
                   size_t length,
                   char** base64Text) {
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_write(bio, buffer, length);
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &bufferPtr);
  BIO_set_close(bio, BIO_NOCLOSE);
  BIO_free_all(bio);

  *base64Text=(*bufferPtr).data;
  (*bufferPtr).data[(*bufferPtr).length] = 0x00;

}


void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length) {
  BIO *bio, *b64;

  int decodeLen = calcDecodeLength(b64message);
  *buffer = (unsigned char*)malloc(decodeLen + 1);
  (*buffer)[decodeLen] = '\0';

  bio = BIO_new_mem_buf((void *)b64message, -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);

  *length = BIO_read(bio, *buffer, strlen(b64message));
  BIO_free_all(bio);
}

bool RSASign( RSA* rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc) {
  // EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
    EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_new();
  EVP_PKEY* priKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(priKey, rsa);
  if (EVP_DigestSignInit(m_RSASignCtx,NULL, EVP_sha256(), NULL,priKey)<=0) {
      return 0;
  }
  
  if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
      return 0;
  }
  
  if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <=0) {
      return 0;
  }
  
  *EncMsg = (unsigned char*)malloc(*MsgLenEnc);
  if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
      return 0;
  }
  
  // EVP_MD_CTX_cleanup(m_RSASignCtx);
  EVP_MD_CTX_free(m_RSASignCtx);
  return 1;
}

char* signMessageEx(char *plainText) {
  RSA* privateRSA;
  unsigned char* encMessage;
  char* base64Text;
  size_t encMessageLength;

  privateRSA = createRSAWithFilename(PRIVATE_PEM,PRIVATE_KEY);

  if(privateRSA == NULL)
  {
  	vdDebug_LogPrintf("CREATE RSA FAILED");
	return "FAIL";
  }
  RSASign(privateRSA, (unsigned char*) plainText, strlen(plainText), &encMessage, &encMessageLength);
  DebugAddHEX("PRIVATE KEY ENC DATA",encMessage,encMessageLength);
  Base64Encode(encMessage, encMessageLength, &base64Text);
  vdDebug_LogPrintf("PRIVATE KEY SIGN %s",base64Text);
  free(encMessage);
  return base64Text;
}

int inPHQR_Initialize_Logon(int inTransType){

    int inRet;

#if 1
	BYTE szStringMsg[2000];
	BYTE szOutBuf[2000];
    BYTE byLen = 0;

	memset(srTransRec.szXMLcode_url,0x00, sizeof(srTransRec.szXMLcode_url));

	strcpy(srTransRec.szXMLcode_url,"00020101021228560011ph.ppmi.p2m0111MBTCPHMMXXX03158000000000000440503310520460125303608540518.005802PH5913P2M UPLOAD MB6006Makati62610012ph.ppmi.qrph03158000000000000440510000000069007088000004488710012ph.ppmi.qrph0151800000000000044~800000000000044~80000044~000000069063041B55");

/*
	memset(szStringMsg,0x00, sizeof(szStringMsg));
	strcpy(szStringMsg, "SALE");
	strcat(szStringMsg, "|");
	strcat(szStringMsg, "PHP 1.00");
	strcat(szStringMsg, "|");
	strcat(szStringMsg, "PHP 1.00");
	strcat(szStringMsg, "|");
	strcat(szStringMsg, srTransRec.szXMLcode_url);
	strcat(szStringMsg, "|");
	strcat(szStringMsg, "wechat");
	strcat(szStringMsg, "|");

vdDebug_LogPrintf("szStringMsg=[%s]", szStringMsg);

inRet = inCallJAVA_UserConfirmQRCode(szStringMsg, szOutBuf, &byLen);
vdDebug_LogPrintf("=====End inUserConfirmQRCode  szOutBuf[%s]=====", szOutBuf);
*/
 //vdPrintQRCode();

 usCTOSS_PrintQRCodeUI(srTransRec.szXMLcode_url);
 return d_OK;
#endif

	
   
    if (inTransType == d_IPC_CMD_PHQR_SALE)
		vdCTOS_SetTransType(PHQR_INITIALIZE);
	else	
		vdCTOS_SetTransType(PHQR_LOGON);
		
	inIITRead(PHQR_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");  

	
	srTransRec.IITid = PHQR_ISSUER_INDEX;
	srTransRec.HDTid = PHQR_HOST_INDEX;
	strCDT.HDTid = PHQR_HOST_INDEX;
	inIITRead(srTransRec.IITid);
	inCPTRead(srTransRec.HDTid);
	strcpy(srTransRec.szCardLable, strIIT.szIssuerLabel);

	
	inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;

    inRet = inCTOS_GetTxnPassword();
    if(d_OK != inRet)
        return inRet;

	inRet = inCTOS_CheckIssuerEnable();
    if(d_OK != inRet)
        return inRet;	
		
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
        return inRet;

	
	inDatabase_TerminalCloseDatabase();

    inRet = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inRet)
        return inRet;

	


	
	inRet = inWalletTableRead(1, srTransRec.HDTid); 

	vdDebug_LogPrintf("inWalletTableRead ret %d, devkey = %s", inRet, strPHQR.szDeviceKey);
	
    if (inTransType == d_IPC_CMD_PHQR_SALE)
		inPHQR_Initialize_SendAndReceive();
	else
		inPHQR_Login_SendAndReceive();

	
	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	//CTOS_LCDTClearDisplay(); 
	
    return d_OK;
}


int inPHQR_GenerateQR(void){

    int inRet, inRet2;
	int ininit = get_env_int("PHQRINIT");
	
	int inExpDate[6+1]; 
	
	int inExpTime[6+1]; 
	BYTE szCurrentDate[20+1];
	BYTE szCurrentTime[20+1];

	BOOL fExit=VS_TRUE;
	
	CTOS_RTC SetRTC;


	vdDebug_LogPrintf("testlang12 2");


    vdCTOS_TxnsBeginInit();

	if(get_env_int("ECRPROCESS")==1)
	{
		vdSetECRTransactionFlg(1);
		vdSetECRResponse(ECR_UNKNOWN_ERR);
	}
	else
        vdSetECRTransactionFlg(0);
	

   if(fGetECRTransactionFlg() == TRUE)
   {
        inRet = inCTOS_MultiAPGetData();
        if(d_OK != inRet)
        return inRet;

   }	
    
    //Read the date and the time //
    CTOS_RTCGet(&SetRTC); 

	inCTOSS_GetEnvDB ("PHQREXPDATE", inExpDate);
	inCTOSS_GetEnvDB ("PHQREXPTIME", inExpTime);

	vdDebug_LogPrintf("testlang8 expdate %s - %ld", inExpDate, atol(inExpDate));
	
	vdDebug_LogPrintf("testlang8 inExpTime %s- %ld", inExpTime, atol (inExpTime));

   memset(szCurrentDate, 0x00, sizeof(szCurrentDate));
   sprintf(szCurrentDate,"%02d%02d%02d",SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay );
   sprintf(szCurrentTime,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);

   
   vdDebug_LogPrintf("testlang8 currentdate get_env_int(%d) %s- %ld", get_env_int("ECRPROCESS"),szCurrentDate, atol (szCurrentDate));

   /*if(get_env_int("ECRPROCESS")==1)
		vdSetECRTransactionFlg(1);
	else
				
        vdSetECRTransactionFlg(0);*/
	
   // inSetColorMenuMode();
	vdCTOS_SetTransType(SALE);
		
	//inIITRead(PHQR_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");  

	
	srTransRec.IITid = PHQR_ISSUER_INDEX;
	srTransRec.HDTid = PHQR_HOST_INDEX;
	strCDT.HDTid = PHQR_HOST_INDEX;
	
	inDatabase_TerminalOpenDatabase();
	inIITReadEx(srTransRec.IITid);
	inCPTReadEx(srTransRec.HDTid);
	inDatabase_TerminalCloseDatabase();

	strcpy(srTransRec.szCardLable, strIIT.szIssuerLabel);

#if 0	
	inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;

    inRet = inCTOS_GetTxnPassword();
    if(d_OK != inRet)
        return inRet;
#endif

	inRet = inCTOS_CheckIssuerEnable();
    if(d_OK != inRet)
        return inRet;	
		
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
    {
         if(fGetECRTransactionFlg())
         {
              vdSetECRResponse(ECR_UNKNOWN_ERR);
              //inMultiAP_ECRSendSuccessResponse();
         }
         return inRet;
    }
	
	inDatabase_TerminalCloseDatabase();

    inRet = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inRet)
    {
         if(fGetECRTransactionFlg())
         {
              vdSetECRResponse(ECR_UNKNOWN_ERR);
              //inMultiAP_ECRSendSuccessResponse();
         }
         return inRet;
    }

	inRet = inCTOS_CheckMustSettle();
    if(d_OK != inRet)
    {
         if(fGetECRTransactionFlg())
         {
              vdSetECRResponse(ECR_UNKNOWN_ERR);
              //inMultiAP_ECRSendSuccessResponse();
         }
         return inRet;
    }
	inRet = inCTOS_GetTxnBaseAmount();
	if(d_OK != inRet)		
		return inRet;

	inRet = inCTOS_UpdateTxnTotalAmount();
	if(d_OK != inRet)		
		return inRet;


	inRet = inCTOS_GetInvoice();
	if(d_OK != inRet)
			return inRet;
	

	inWalletTableRead(1, srTransRec.HDTid); 


    if (ininit != 1){
		inRet = inPHQR_Initialize_SendAndReceive();
		
		if(d_OK != inRet)
				return inRet;
    }
	

	if (atol(szCurrentDate) >= atol(inExpDate) && atol(szCurrentTime) > atol(inExpTime)){

		inRet = inPHQR_Login_SendAndReceive();
		if(d_OK != inRet)
			return inRet;
	
	}


	inRet = inPHQR_GenerateQR_SendAndReceive();
	if(d_OK != inRet)
    {
         return inRet;
    }
	vdSetGlobalVariable();
		
	//strDigitalWallet.szout_trade_no

    strGBLVar.fGBLvRetrieveList = TRUE;
	iRetrieveSaveData();
		
    while(1)
    {
		fExit=VS_FALSE;
        inRet2 = inUserConfirmQRCodePHQR();
        if (inRet2 != d_OK){
			fExit=VS_TRUE;
            //return inRet;
        }
    
        vdCTOS_SetTransType(SALE);
        
        inRet = inPHQR_Check_Status_SendAndReceive();

        if(inRet == d_OK)
           break; 

		if(fExit == VS_TRUE) //if cancel/timeout during QR confirmation
		{
			if(inRet2 == d_USER_CANCEL)
			{
				if(fGetECRTransactionFlg())
				{
                    vdSetECRResponse(ECR_OPER_CANCEL_RESP);
					inMultiAP_ECRSendSuccessResponse();
				}
				//vdDisplayErrorMsg(1, 8, "USER CANCEL");
			}
			else
			{
				if(fGetECRTransactionFlg())
				{
                    vdSetECRResponse(ECR_TIMEOUT_ERR);
					inMultiAP_ECRSendSuccessResponse();
				}
				//vdDisplayErrorMsg(1, 8, "TIMEOUT");
			}
			return d_NO;
		}
    }
	
	//srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
	//srTransRec.iXMLRetrieveType = WECHAT_AUTO_RETRIEVE;
	//srRetrieveRec.byVoided = FALSE;
	//srTransRec.byVoided = FALSE;
		

	vdDebug_LogPrintf("testlang3 before batch save");

	inRet = inCTOS_SaveBatchTxn();

	vdDebug_LogPrintf("testlang12 before batch save %d, inRet");
    if(d_OK != inRet)
    {
         if(fGetECRTransactionFlg())
         {
              vdSetECRResponse(ECR_UNKNOWN_ERR);
              //inMultiAP_ECRSendSuccessResponse();
         }
         return inRet;
    }
	vdDebug_LogPrintf("testlang3 before update accum");
	
    inRet = inCTOS_UpdateAccumTotal();
    if(d_OK != inRet)
    {
         if(fGetECRTransactionFlg())
         {
              vdSetECRResponse(ECR_UNKNOWN_ERR);
              //inMultiAP_ECRSendSuccessResponse();
         }
         return inRet;
    }
	//Added to send correct time to ECR
	//memcpy(szTime,&srTransRec.szXMLtime_end[8],6);
	//wub_str_2_hex(szTime,srTransRec.szTime,6);

	//fECRBuildSendOK = TRUE;	
	//if (fECRBuildSendOK){
	//	inRet = inMultiAP_ECRSendSuccessResponse();
	//}

	if (isCheckTerminalMP200() == d_OK)
	{
		vdCTOSS_DisplayStatus(d_OK);

		inRet = inCTOSS_ERM_ReceiptRecvVia();
		if(d_OK != inRet)
	    	return inRet;
	}

	vdDebug_LogPrintf("inPHQR_GenerateQR inMultiAP_ECRSendSuccessResponse");
	inMultiAP_ECRSendSuccessResponse();
	
	inRet = ushCTOS_printReceipt();
	if (inRet != d_OK)
		return inRet;
	else
	{
		if (fFSRMode() == TRUE)
         {  
              inCTOSS_UploadReceipt(FALSE, FALSE);	 
         }
         vdSetErrorMessage("");
	}

	if(strTCT.byERMMode != 0)
		ErmTrans_Approved  = TRUE;
	
	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	//CTOS_LCDTClearDisplay(); 
	


	
	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	//CTOS_LCDTClearDisplay(); 

	if (fGetECRTransactionFlg())
    {
         fECRApproved=TRUE;
         inMultiAP_ECRSendSuccessResponse();
    }
	
	vdCTOS_TransEndReset();
    return d_OK;
}


int inPHQR_Initialize_SendAndReceive(void)
	{
		int inRet, i, iWidth = 16;
		MemoryStruct write_data, read_data;
		char szTempBuff[1000];
		char szEMVString[500];
		char szErrorString[64+1];
		char szBuffer[100+1];
		char szResponseBuff[1000] = {0};
		BYTE key;
		BYTE szInvNum[6+1];
	
		BYTE szMsg[20+1];
		
		BYTE szGetInv[INVOICE_ASC_SIZE + 1] = {0};
	
		
		BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
		int inRescode;

	    vdDebug_LogPrintf("--inPHQR_Initialize_SendAndReceive--");
	
		for (i = 1; i<= strPHQR.inCurlRetries;i++)
		{
			write_data.memory = malloc(1); 
			write_data.size = 0;	 
	
			inRet = inPHQRSend_request(&read_data,&write_data, PHQR_INITIALIZE);
	
			//connect failed
			if (inRet == 7){
				vdDisplayErrorMsgResp2("","Connect Failed","");
				free(write_data.memory);
				return ST_ERROR;
			}	
			//end

            
			
			vdDebug_LogPrintf("inBDOPayGenerateQRCode %d",inRet);
			
			memset(szTempBuff, 0x00, sizeof(szTempBuff));
			memcpy(szTempBuff, write_data.memory, write_data.size);
			
			free(write_data.memory);
			
			vdDebug_LogPrintf("receive data %s",szTempBuff );

			//inPrintXMLPacket("RX-PHQR BODY", szTempBuff, strlen(szTempBuff), FALSE);
			
			if (inRet == d_OK)
				break;
			
			
	}

    //TESTLANG-REMOVE LATER
    //memset(szTempBuff, 0x00, sizeof(szTempBuff));
	//strcpy(szTempBuff,"{\"reqMsgId\":\"523401F0286A45C59F753446C10C80A3\",\"respTime\":\"2021-11-10T13:28:08+08:00\",\"deviceKey\":\"EE73282073AE97F6DC27497EBB5DF0A3AC65259B63DE00BED4E342C144B033D0\",\"merchantName\":\"P2M UPLOAD MB\",\"merchantAddress\":\"Makati\",\"resultCode\":\"00\",\"resultStatus\":\"Success\",\"message\":\"Device has been initialized.\"}");
	//TESTLANG

    memset(szBuffer, 0x00, sizeof(szBuffer));
	inExtractField(szTempBuff, szBuffer, "resultStatus");

    //get status and error message
	if ((strcmp(szBuffer, "Success") != 0) && (strcmp(szBuffer, "success") != 0)){

		//get result code
		memset(szBuffer, 0x00, sizeof(szBuffer));
		inExtractField(szTempBuff, szBuffer, "resultCode");
		inRescode = atoi(szBuffer);

		if (inRescode == 2){
			
			put_env_int("PHQRINIT",1);
			return d_OK; //already initialized
		}
			
		if (inDisplaResultcode(inRescode) != d_OK)
			vdDisplayErrorMsgResp2(" ", "INITIALIZE FAILED", " "); 
				

		return d_NO;
		
	}

	//get and save device key
	
	//memset(szBuffer, 0x00, sizeof(szBuffer));
	inExtractField(szTempBuff, strPHQR.szDeviceKey, "deviceKey");
	//inExtractField(szTempBuff, szBuffer, "deviceKey");


	//vdDebug_LogPrintf("testlang devkey %s", szBuffer);

	if (strlen(strPHQR.szDeviceKey)> 0){
		inPHQRSave(1);
		put_env_int("PHQRINIT",1);
	}else{
		vdDisplayErrorMsgResp2(" ", "DEVICE KEY", "NOT PROVIDED"); 
		return d_NO;
	}

	

	return d_OK;

}
int inDisplaResultcode(int inCode){
	int inRet = d_OK;
	

    switch(inCode){
	  case 1: vdDisplayErrorMsgResp2(" ", "REQUEST IS NOT", "AUTHORIZED"); break;
	  case 2: vdDisplayMessage(" ", "DEVICE ALREADY INITALIZED", " "); 
	          CTOS_Beep();
   			  CTOS_Delay(1500);
			  break;
	  case 3: vdDisplayErrorMsgResp2(" ", "MERCHANT IS INACTIVE", " "); break;
	  case 4: vdDisplayErrorMsgResp2(" ", "DEVICE IS INACTIVE.", " "); break;
	  case 5: vdDisplayErrorMsgResp2(" ", "NO EXISTING USER", "FOR THE MERCHANT"); break;  
	  case 6: vdDisplayErrorMsgResp2(" ", "TRANSACTION", "DOES NOT EXIST"); break;
	  case 7: vdDisplayErrorMsgResp2(" ", "CANNOT INSERT", "DUPLICATE TRANSACTION"); break;
	  case 20: vdDisplayErrorMsgResp2(" ", "MID IS NOT PROVIDED", " "); break;
	  case 21: vdDisplayErrorMsgResp2(" ", "TID IS NOT PROVIDED", " "); break;
	  case 22: vdDisplayErrorMsgResp2(" ", "SERIAL NUMBER", "IS NOT PROVIDED"); break;
	  case 23: vdDisplayErrorMsgResp2(" ", "USERNAME", "IS NOT PROVIDED"); break;
	  case 24: vdDisplayErrorMsgResp2(" ", "USERNAME", "IS NOT VALID"); break;
	  case 25: vdDisplayErrorMsgResp2(" ", "PASSWORD", "IS NOT PROVIDED"); break;
	  case 26: vdDisplayErrorMsgResp2(" ", "PASSWORD", "IS NOT VALID"); break;
	  case 27: vdDisplayErrorMsgResp2(" ", "TRANSACTION AMOUNT", "IS NOT PROVIDED"); break;
	  case 28: vdDisplayErrorMsgResp2(" ", "TRANSACTION AMOUNT", "IS NOT VALID"); break;
	  case 29: vdDisplayErrorMsgResp2(" ", "CURRENCY", "IS NOT PROVIDED"); break;
	  case 30: vdDisplayErrorMsgResp2(" ", "CURRENCY", "IS NOT VALID"); break;
	  case 31: vdDisplayErrorMsgResp2(" ", "INVOICE", "IS NOT PROVIDED"); break;
	  case 33: vdDisplayErrorMsgResp2(" ", "TRANS REF NUMBER", "IS NOT PROVIDED"); break;
	  case 99: vdDisplayErrorMsgResp2(" ", "SYSTEM ERROR", " "); break;

	  default:
	  	inRet = d_NO; 
	  	   
    }
		
	
	return inRet;

}

int inPHQRSend_request(struct MemoryStruct *contents,struct MemoryStruct *response, int TransType) 
{

	CURL *curl;
 	CURLcode res;
	int inResult;
	int i, inDay,inRet = 0;
	char szBuffer[1000];
	char szClock[40+1];
	char szConent_len[30+1];
	char szAuthorization[100+1]={0};
	BYTE pbtBuff[4096+1]; 
	//BYTE szEncrypted[4098+1] = {0};
	USHORT pusLen;
    char postthis[3000];
	char postBody[1000];
	char szURL[150+1];
	char szTempURL[150];
	char szTemp[512+1] = {0};
	char szSignature[512+1];
	char szTempBuffer[200+1];

	BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
	int inCPTIndex = 0;

	char szApiKey[100+1];

	char szApiKeyHeader[100+1];
	
	char szDeviceKeyHeader[100+1];

	char szCallerIDHeader[50];
	
	char szSessionKeyHeader[100+1];
	//char szRSAEncrypted[128+1];
    extern int g_inCURLConnect;

	vdDebug_LogPrintf("vdSend_request %d", TransType);
	
    //vdDisplayMessageStatusBox(1, 8, "PROCESSING", MSG_PLS_WAIT, MSG_TYPE_PROCESS);
	
    vdDebug_LogPrintf("strTCT.byRS232ECRPort:%d", strTCT.byRS232ECRPort);
    if(strTCT.byRS232ECRPort == 11)
    {
        inCPTRead(25);
		vdDebug_LogPrintf("strCPT.inCommunicationMode:%d", strCPT.inCommunicationMode);
        if(strCPT.inCommunicationMode == GPRS_MODE)
            inCallJAVA_switchGPRS();
    }

    if(get_env_int("#CRADLE") == 1 && get_env_int("#CRADLEWIFI") == 1)
    {
        if(strCPT.inCommunicationMode == WIFI_MODE)
        {
            if (inCTOS_InitComm(strCPT.inCommunicationMode) != d_OK) 
            {
                return(d_NO);
            }
            
            inCTOS_CheckInitComm(strCPT.inCommunicationMode); 
            if (srCommFuncPoint.inCheckComm(&srTransRec) != d_OK)
            {
                inCTOS_inDisconnect();
                return(d_NO);
            }
            
            if (srCommFuncPoint.inConnect(&srTransRec) != ST_SUCCESS)
            {
                inCTOS_inDisconnect();
                return d_NO;
            }
        }
    }
	
	memset(postthis, 0x00, sizeof(postthis));
	vdPHQRCreatePOSTData(&postthis, TransType);

	for(inCPTIndex=1; inCPTIndex<=2; inCPTIndex++){
	
		res = curl_global_init(CURL_GLOBAL_DEFAULT);
	
		/* Check for errors */
		if (res != CURLE_OK) {		
			vdDebug_LogPrintf("curl_global_init failed");	
			return d_OK;
		}
		vdDebug_LogPrintf("curl_global_init successful");
	
		curl = curl_easy_init();
	
		if(curl) {
		
			vdDebug_LogPrintf("curl_easy_init successful");

			//vdDebug_LogPrintf("fIP_URL[%d]", strGCash.fIP_URL);
		
			if (TransType == PHQR_INITIALIZE)
			{
				//#if 1
				//memset(szURL, 0x00, sizeof(szURL));
				//if (strGCash.fIP_URL == TRUE)
				//	vdSetNewURL(szURL, strGCash.GenerateQRURL, inCPTIndex);
				//else
				//strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/initialize");
				sprintf(szURL,"%s",strPHQR.szInitializeURL);
			
				vdDebug_LogPrintf("Initialize URL is %s",szURL);
			}else if (TransType == PHQR_LOGON){
				//strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/login");
				sprintf(szURL,"%s",strPHQR.szLogonURL);		
				vdDebug_LogPrintf("Logon URL is %s",szURL);

			}else if (TransType== PHQR_GENERATE_QR)	{
				//strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/generate-qr");
				sprintf(szURL,"%s",strPHQR.szGenerateQrURL);
				vdDebug_LogPrintf("GENERATE QR URL is %s",szURL);
				
			}else if (TransType == PHQR_CHECK_STATUS){
				//strcpy(szURL, "https://210.23.217.105:443/api/merchant/generate-qr");
				sprintf(szURL,"%s",strPHQR.szCheckTransctionURL);
				vdDebug_LogPrintf("CHECK STATUS URL is %s",szURL);
			}


			vdDebug_LogPrintf("testlang URL %s", szURL);
			
		
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

			
			curl_easy_setopt(curl, CURLOPT_URL,szURL);
			
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		

			/* we want to use our own read function */
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_phqr_callback);

			/* pointer to pass to our read function */
			curl_easy_setopt(curl, CURLOPT_READDATA, (void *)contents);

			/* send all data to this function  */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_phqr_callback);

			/* we pass our 'chunk' struct to the callback function */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);

			curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

			/* if we don't provide POSTFIELDSIZE, libcurl will strlen() byitself */ 

			struct curl_slist *headerlist = NULL;

			//Set content length;		
			memset(szConent_len,0x00,sizeof(szConent_len));
			sprintf(szConent_len,"content-length: %d",strlen(postthis));
			vdDebug_LogPrintf("content-length: %d",strlen(postthis));		

			vdDebug_LogPrintf("content-length: %s",postthis);	

			sprintf(szSignature,"Signature: %s",szGlobalSignature);


            memset(szApiKey, 0x00, sizeof(szApiKey));	
            memset(szApiKeyHeader, 0x00, sizeof(szApiKeyHeader));

			//if (strPHQR.inAPIKeyCreated == 1){
			//	strcpy(strPHQR.szAPIKey,szApiKey);
			//}else{
				memset(szTempBuffer, 0x00, sizeof(szTempBuffer));
				sprintf(szTempBuffer,"%s%s",srTransRec.szMID, srTransRec.szTID);

				vdDebug_LogPrintf("testlang test buff %s", szTempBuffer);
				inCompute_HMAC_SHA256(szTempBuffer, szApiKey);
				strupr(szApiKey);
				vdDebug_LogPrintf("sha256 api-key %s", szApiKey);
				//strcpy(szApiKey, "33CBE02FA94707CFFCFD327D502765B949D5CCAC918437981F72FB5D2D417BFB");
				
				//strPHQR.inAPIKeyCreated = 1;
				//inPHQRSave(1);
			//}

			                  
			sprintf(szApiKeyHeader,"Api-Key: %s", szApiKey);

            memset(szDeviceKeyHeader, 0x00, sizeof(szDeviceKeyHeader));


			//inCTOSS_GetEnvDB("DEVKEY", szDeviceKeyHeader);
			sprintf(szDeviceKeyHeader,"Device-Key: %s", strPHQR.szDeviceKey);
			vdDebug_LogPrintf("testlang devkey %s", szDeviceKeyHeader);


			if ((TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))	{
				memset(szSessionKeyHeader, 0x00, sizeof(szSessionKeyHeader));
				sprintf(szSessionKeyHeader,"Session-Key: %s", strPHQR.szSessionKey);
				
				vdDebug_LogPrintf("testlang szSessionKey %s", szSessionKeyHeader);
			}


			memset(szCallerIDHeader, 0x00, sizeof(szCallerIDHeader));
			
			sprintf(szCallerIDHeader,"Caller-Id: %s", strPHQR.szCallerID);
			
			vdDebug_LogPrintf("testlang Caller-Id: %s", szCallerIDHeader);

			//testlang-remove later
			//memset(szDeviceKeyHeader, 0x00, sizeof(szDeviceKeyHeader));
			//memset(szApiKeyHeader, 0x00, sizeof(szApiKeyHeader));
			//memset(szSessionKeyHeader, 0x00, sizeof(szSessionKeyHeader));

			//strcpy(szDeviceKeyHeader,"Device-Key: 22909A4BBD6239DD153E939E9A847D0FA8CDA60262D22AB06959BA7F55408068");
			//strcpy(szApiKeyHeader,"Api-Key: B4ABAA4412BB8D122143DC236936B2A0C0DB0E7AE3281863E287C3F7A2FED1E8");
			//strcpy(szSessionKeyHeader,"Session-Key: F9E79286C0647C9A6D3DE473335EE7D892059BB38471859C0AB98C5E73D2B696");

			//testlang-remove later
	                	
			//if (TransType == PHQR_INITIALIZE){
				vdDebug_LogPrintf("REQUEST HEADER AND BODY");
				headerlist = curl_slist_append(headerlist, "cache-control: no-cache");
				headerlist = curl_slist_append(headerlist, "Connection: keep-alive");
				headerlist = curl_slist_append(headerlist, szConent_len);
				headerlist = curl_slist_append(headerlist, "Content-Type: application/json");
				headerlist = curl_slist_append(headerlist, szApiKeyHeader);
				
				if ((TransType == PHQR_LOGON) || (TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))
					headerlist = curl_slist_append(headerlist, szDeviceKeyHeader);

				if ((TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))		
					headerlist = curl_slist_append(headerlist, szSessionKeyHeader);

				headerlist = curl_slist_append(headerlist, szCallerIDHeader);

				headerlist = curl_slist_append(headerlist, szSignature);
				vdDebug_LogPrintf("curl_slist_append data1=[%s]",headerlist->data);
				vdDebug_LogPrintf("curl_slist_append data2=[%s]",headerlist->next->data);
				vdDebug_LogPrintf("curl_slist_append data3=[%s]",headerlist->next->next->data);
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->data);
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->data);
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->data);			
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->next->data);
				
				if ((TransType == PHQR_LOGON) || (TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))	
					vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->next->next->data);

			if ((TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))	
					vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->next->next->next->data);

        	//}	
		
		
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		
        	//if (TransType == PHQR_INITIALIZE)
			//{
				vdDebug_LogPrintf("DATA TO SEND %s",postthis);
				vdDebug_LogPrintf("POSTFIELDSIZE %li",(long)strlen(postthis));
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));			
        	//}

        	//for (i = 1; i<= strBDOPAY.inCurlRetries;i++){

			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT , strPHQR.inCurlConnectTimeout); 
 			curl_easy_setopt(curl, CURLOPT_TIMEOUT, strPHQR.inCurlRequestTimeout);

			/* Perform the request, res will get the return code */ 
			res = curl_easy_perform(curl);

			vdDebug_LogPrintf("curl_easy_perform %d retries=%d", res, i);


			if( fXMLPrint && 
			((res == CURLE_OPERATION_TIMEDOUT) || (res == CURLE_OK)))
			{

				char szReqBuff[2000] = {0};

				strcpy(szReqBuff, postthis);
			
				inPrintXMLPacket("TX-PHQR BODY", szReqBuff, strlen(szReqBuff), TRUE);
			}


			/* Check for errors */ 
			if(res != CURLE_OK) 
			{
				vdDebug_LogPrintf("curl_easy_perform %s ", curl_easy_strerror(res));
				//GetDPerformResutlMessage(res);	

				vdDebug_LogPrintf("curl_easy_cleanup !!!");			
				curl_easy_cleanup(curl);// always cleanup		

				vdDebug_LogPrintf("curl_slist_free_all !!!");			
				curl_slist_free_all(headerlist);// cleanup headerlist

				vdDebug_LogPrintf("curl_global_cleanup !!!");			
				curl_global_cleanup();// we're done with libcurl, so clean it up

				vdDebug_LogPrintf("curl_easy_reset !!!");
				curl_easy_reset(curl);	// reset all handles

				vdDebug_LogPrintf("Encountered error!!!");
            
				continue;
				//return(res);
			}
			else
			{
				vdCTOS_DispStatusMessage("RECEIVING...");
				/* always cleanup */ 
				curl_easy_cleanup(curl);			
			
				vdDebug_LogPrintf("curl_slist_free_all !!!");			
				curl_slist_free_all(headerlist); // cleanup headerlist

				vdDebug_LogPrintf("curl_global_cleanup !!!");			
				curl_global_cleanup();// we're done with libcurl, so clean it up

				vdDebug_LogPrintf("curl_easy_reset !!!");			
				curl_easy_reset(curl);// reset all handle
				break;
			}
		
			/* always cleanup */ 
			//vdDebug_LogPrintf("curl_easy_cleanup !!!");
			//curl_easy_cleanup(curl);
		}
	}

    if(get_env_int("#CRADLE") == 1 && get_env_int("#CRADLEWIFI") == 1)
    {
        if(strCPT.inCommunicationMode == WIFI_MODE)
            inCTOS_inDisconnect();
    }
	
    return res;
}

int inPHQRSend_requestEx(char *response, int TransType) 
{

	CURL *curl;
 	CURLcode res;
	int inResult;
	int i, inDay,inRet = 0;
	char szBuffer[1000];
	char szClock[40+1];
	char szConent_len[30+1];
	char szAuthorization[100+1]={0};
	BYTE pbtBuff[4096+1]; 
	//BYTE szEncrypted[4098+1] = {0};
	USHORT pusLen;
    char szBody[3000];
	char postBody[1000];
	char szURL[150+1];
	char szTempURL[150];
	char szTemp[512+1] = {0};
	char szSignature[512+1];
	char szTempBuffer[200+1];
	
	//int inResult;
	TRANS_DATA_TABLE *srTransPara;
	BYTE szXMLData[XML_RECV_SIZE+1] = {0};
	int iCounter = 0;
	int iLimit = 1;
	BOOL fReceiveOK = FALSE;
	BOOL fWait = TRUE;
	BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
	BOOL fXMLDebug = (BOOL)get_env_int("XMLDBG");
	BOOL fTerminated = FALSE;
	BOOL fConnectFailed = FALSE;
	BOOL fXMLEnd = FALSE;
	BOOL iReceiveCounter = 0;
	BYTE szXMLReceiveTemp[XML_RECV_SIZE+1], szHeader[XML_RECV_SIZE+1];
	int iXMLReceiveLen = 0;
    int inXMLSendLen=0, inBodyLen=0, inHeaderLen=0;
	
	//BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
	int inCPTIndex = 0;

	char szApiKey[100+1];
	char szApiKeyHeader[100+1];
	char szDeviceKeyHeader[100+1];
	char szCallerIDHeader[50];
	char szSessionKeyHeader[100+1];
	char szLength[5+1];
	
	//char szRSAEncrypted[128+1];
    extern int g_inCURLConnect;

	vdDebug_LogPrintf("vdSend_request %d", TransType);
	
	vdCTOS_DispStatusMessage("PROCESSING ex...");

    vdDebug_LogPrintf("strTCT.byRS232ECRPort:%d", strTCT.byRS232ECRPort);
    if(strTCT.byRS232ECRPort == 11)
    {
        inCPTRead(25);
		vdDebug_LogPrintf("strCPT.inCommunicationMode:%d", strCPT.inCommunicationMode);
        if(strCPT.inCommunicationMode == GPRS_MODE)
            inCallJAVA_switchGPRS();
    }
	
	memset(szBody, 0x00, sizeof(szBody));
	vdPHQRCreatePOSTData(&szBody, TransType);

    inBodyLen=strlen(szBody);

    if (TransType == PHQR_INITIALIZE)
    {
       //strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/initialize");
       sprintf(szURL,"%s",strPHQR.szInitializeURL); 
        vdDebug_LogPrintf("Initialize URL is %s",szURL);
    }
	else if (TransType == PHQR_LOGON)
    {
        //strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/login");
        sprintf(szURL,"%s",strPHQR.szLogonURL);		
        vdDebug_LogPrintf("Logon URL is %s",szURL);
    }
	else if (TransType== PHQR_GENERATE_QR)	
    {
        //strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/generate-qr");
        sprintf(szURL,"%s",strPHQR.szGenerateQrURL);
        vdDebug_LogPrintf("GENERATE QR URL is %s",szURL);
    }
	else if (TransType == PHQR_CHECK_STATUS)
	{
        //strcpy(szURL, "https://210.23.217.105:443/api/merchant/generate-qr");
        sprintf(szURL,"%s",strPHQR.szCheckTransctionURL);
        vdDebug_LogPrintf("CHECK STATUS URL is %s",szURL);
    }

    memset(szTempBuffer, 0x00, sizeof(szTempBuffer));
    sprintf(szTempBuffer,"%s%s",srTransRec.szMID, srTransRec.szTID);
    vdDebug_LogPrintf("testlang test buff %s", szTempBuffer);
    inCompute_HMAC_SHA256(szTempBuffer, szApiKey);
    strupr(szApiKey);
		
	memset(szLength, 0x00, sizeof(szLength));
	sprintf(szLength, "%d", inBodyLen);
	
	memset(szHeader, 0, sizeof(szHeader));
	strcpy(szHeader, "POST ");
	strcat(szHeader, szURL);
	strcat(szHeader, szPostLine2);
	strcat(szHeader, "\x0D\x0A");
	
	strcat(szHeader, "Host: ");
	strcat(szHeader, "metrobankqrpay.metrobankcard.com");
	strcat(szHeader, "\x0D\x0A");
	
	strcat(szHeader,"Content-Type: ");
	strcat(szHeader, "application/json;charset=UTF-8");
	strcat(szHeader, "\x0D\x0A");
	strcat(szHeader,"Cache-Control: ");
	strcat(szHeader, szCacheControl);
	strcat(szHeader, "\x0D\x0A");
	strcat(szHeader,"Connection: ");
	strcat(szHeader, szConnectionMsg);
	strcat(szHeader, "\x0D\x0A");	
    strcat(szHeader,"Api-Key: ");
    strcat(szHeader, szApiKey);
    strcat(szHeader, "\x0D\x0A");	
    strcat(szHeader,"Device-Key: ");
    strcat(szHeader, strPHQR.szDeviceKey);
    strcat(szHeader, "\x0D\x0A");	
    strcat(szHeader,"Caller-Id: ");
    strcat(szHeader, strPHQR.szCallerID);
    strcat(szHeader, "\x0D\x0A");	
	strcat(szHeader,"Signature: ");
    strcat(szHeader, szGlobalSignature);
    strcat(szHeader, "\x0D\x0A");
	strcat(szHeader, "Content-Length: ");
	strcat(szHeader, szLength);
	strcat(szHeader, "\x0D\x0A\x0D\x0A");

    //vdDebug_LogPrintf("szHeader:%s", szHeader);
	//vdDebug_LogPrintf("szBody:%s", szBody);
	
    inHeaderLen=strlen(szHeader);
	
	memset(srTransRec.szXMLSendData, 0, sizeof(srTransRec.szXMLSendData));
    strcpy(srTransRec.szXMLSendData, szHeader);
	strcat(srTransRec.szXMLSendData, szBody);
	
	srTransRec.iXMLSendLen=inHeaderLen+inBodyLen;

    vdDebug_LogPrintf("srTransRec.szXMLSendData:%s", srTransRec.szXMLSendData);
	vdDebug_LogPrintf("srTransRec.iXMLSendLen:%d", srTransRec.iXMLSendLen);
	
	//return d_OK;
	/*
	sha256 api-key C465D42EFB2111DCC1CBCA1D9F8EC3733D9924521C757EAB90616EA9332559DA
	testlang devkey Device-Key: E3087DB6B9B3C58281A124444A02CD6DDF856471B20A352B01B7C2BEF28FD262
	testlang Caller-Id: Caller-Id: 231E0FAEA31B42DB92D9FFE05979E331
	REQUEST HEADER AND BODY
	curl_slist_append data1=[cache-control: no-cache]
	curl_slist_append data2=[Connection: keep-alive]
	curl_slist_append data3=[content-length: 165]
	curl_slist_append data4=[Content-Type: application/json]
	curl_slist_append data4=[Api-Key: C465D42EFB2111DCC1CBCA1D9F8EC3733D9924521C757EAB90616EA9332559DA]
	curl_slist_append data4=[Device-Key: E3087DB6B9B3C58281A124444A02CD6DDF856471B20A352B01B7C2BEF28FD262]
	curl_slist_append data4=[Caller-Id: 231E0FAEA31B42DB92D9FFE05979E331]
	curl_slist_append data4=[Signature: YwT1KZah4N6FrXfpfi9CFWfQ6J7/AkkaChpopXgVbVT+ZwOxQ1peGoy1fVZVXxMbZ5gboA34fiNBrBY1ioUZTrIRe2KOVR5+b9JOS0sesod+/uvE53PGyq6ROXZH4vkRCdnHbeS8RS5arYtP0BB7pWl8Y6CoP/eN58DhmT8unAGv0imebjE0VpciYhfJug3/qTYLDue2lAqJpQimuUEa5Zc5cmVSgycHzopAvWlkh4epVajAIh/Aj2F4Od3pnTNgiNYCXx7IIZkVX88PJBD1S2dxKQjo9n9U5/80vYRE6INtpO2FAgH3s5k4az295EoW2vaSoqGP0Z4I9r9nE3sJQQ==]
	*/

	
	vdDebug_LogPrintf("SSL[%d]",strCPT.fSSLEnable);
	vdDebug_LogPrintf("fShareComEnable[%d]", strTCT.fShareComEnable);
	vdDebug_LogPrintf("inIPHeader[%d]", strCPT.inIPHeader);
	vdDebug_LogPrintf("Primary[%s]port[%ld]",strCPT.szPriTxnHostIP,strCPT.inPriTxnHostPortNum);
	vdDebug_LogPrintf("Secondary[%s]port[%ld]",strCPT.szSecTxnHostIP,strCPT.inSecTxnHostPortNum);
	vdDebug_LogPrintf("fDHCPEnable[%d]", strTCP.fDHCPEnable);
	vdDebug_LogPrintf("szAPN[%s].szUserName[%s].szPassword[%s]", strTCP.szAPN, strTCP.szUserName, strTCP.szPassword);
	//vdDebug_LogPrintf("iLimit[%d].fXMLPrint[%d].fXMLDebug[%d].fAutoRetrieve[%d].fRetrieveOnce[%d]", iLimit, fXMLPrint, fXMLDebug, fAutoRetrieve, fRetrieveOnce);

	if (inCTOS_InitComm(strCPT.inCommunicationMode) != d_OK) 
	{
		return(d_NO);
	}
	
	inCTOS_CheckInitComm(strCPT.inCommunicationMode); 
	if (srCommFuncPoint.inCheckComm(&srTransRec) != d_OK)
	{
		inCTOS_inDisconnect();
		return(d_NO);
	}

	// Check connection
	inResult = srCommFuncPoint.inConnect(&srTransPara);
	if (inResult != d_OK)
	{
		CTOS_Delay(500);
		if(inResult == GPRS_FAIL)
			vdDisplayErrorMsgResp2(" ", "GPRS Problem", "Please Try Again");
		else if(inResult == ETHERNET_FAIL)
			vdDisplayErrorMsgResp2(" ", "Check LAN","Connectivity");
		else if(inResult == WIFI_FAIL)
			vdDisplayErrorMsgResp2(" ", "WiFi Problem", "Please Try Again");
		else    
			vdDisplayErrorMsgResp2(" ", "Connect Failed", "Please Try Again");
		
		vdSetErrorMessage("");
		if (fGetECRTransactionFlg())
        {
             vdSetECRResponse(ECR_COMMS_ERR);
             //inMultiAP_ECRSendSuccessResponse();
        }
			
		return d_NO;
	}

    vdDebug_LogPrintf("1.inPHQRSend_requestEx connected");

    //return d_OK;
	
		// Send XML 
	iCounter = 0;
	fReceiveOK = FALSE;
	do
	{
		if (fXMLDebug == TRUE)
			vdDebug_LogPrintf("iCounter[%d].SendXML.Len[%d].Data[\n%s\n]", iCounter, strlen(srTransRec.szXMLSendData), srTransRec.szXMLSendData);
	
		put_env_int("ERMMULTISEND",1);

		inResult = srCommFuncPoint.inSendData(&srTransPara, srTransRec.szXMLSendData, srTransRec.iXMLSendLen);

		vdDebug_LogPrintf("iCounter[%d].XML inSendData[%d]", iCounter, inResult);

		if (inResult != d_OK)
		{
			vdDisplayErrorMsgResp2(" Connection ","Terminated","Please Try Again");
			vdSetErrorMessage("");
			fTerminated = TRUE;
			break;
		}

		//if (fXMLPrint == TRUE && inResult == d_OK)					
			//inPrintXMLPacket("TX-XML", srTransRec.szXMLSendData, srTransRec.iXMLSendLen, TRUE); 

		CTOS_Delay(500);

		if (inResult == d_OK) 
			break;
	
		iCounter++;

		if (iCounter > iLimit)
			break;
		
		iCounter++;
		
	}while (fWait);

	do
	{
		srTransRec.iXMLReceiveLen = 0;
		inResult = srCommFuncPoint.inRecData(&srTransPara, srTransRec.szXMLReceiveData);
	
		vdDebug_LogPrintf("iCounter[%d]	%d]", iCounter, inResult);

		#if 0
		if (inResult == -7)
		{
			vdDisplayErrorMsgResp2(" ", "Connect Failed", "Please Try Again");
			vdSetErrorMessage("");
			fTerminated = TRUE;
			break;
		}

		fXMLEnd = fValidXMLEnd(srTransRec.szXMLReceiveData);				
		vdDebug_LogPrintf("fXMLEnd=[%d]", fXMLEnd);
		if (!fXMLEnd)
		{
			//vdDisplayErrorMsg(1, 8, "MISSING </XML>. IGNORE");
			vdDebug_LogPrintf("MISSING </XML>");
		}
		#endif
		
		srTransRec.iXMLReceiveLen = inResult; // Length
		
		if (srTransRec.iXMLReceiveLen > 0)
		{
			memset(szXMLData, 0x00, sizeof(szXMLData));
			memcpy((char*)szXMLData, (char*)srTransRec.szXMLReceiveData, srTransRec.iXMLReceiveLen); 		
			put_env_int("ERMMULTISEND",0);
			srTransRec.fStateTimeOut = FALSE;

			if (fXMLDebug == TRUE)
				vdDebug_LogPrintf("iCounter[%d].ReceiveXML.Len[%d.Data[\n%s\n]", iCounter, strlen(srTransRec.szXMLReceiveData), srTransRec.szXMLReceiveData);

			//if (fXMLPrint == TRUE && srTransRec.iXMLReceiveLen > 0)
				//inPrintXMLPacket("RX-XML", srTransRec.szXMLReceiveData, strlen(srTransRec.szXMLReceiveData), TRUE);
#if 0
			if ((fXMLBadRequestMessage((char*)szXMLData) != d_OK) ||
			   (fXMLForbiddenMessage((char*)szXMLData) != d_OK) ||
			   (fXMLNotOKMessage((char*)szXMLData) != d_OK) ||
			   (iXMLCheckResponseStatus((char*)szXMLData) != d_OK)) 		   
			{
				fReceiveOK = FALSE;
				break;
			}
			else
			{
				// Success Receive
				iXMLAnalyzeResponse((char*)szXMLData);
				fReceiveOK = TRUE;
				break;
			}	
#endif
		}

		iCounter++;

		if (iCounter > iLimit)
			break;
		
	}while(fWait);

	if (fReceiveOK)
	{
		inResult = d_OK;
	}
	else
	{
		inResult = d_NO;
	}

	if (srTransRec.iXMLReceiveLen <= 0 && !fTerminated)
	{
		vdDisplayMessage("", "", "NO RESPONSE FROM HOST");		
		CTOS_Beep();
		CTOS_Delay(3000);
		srTransRec.fStateTimeOut = TRUE;
	}
	
	inCTOS_inDisconnect();
	
#if 0
	for(inCPTIndex=1; inCPTIndex<=2; inCPTIndex++){
	
		res = curl_global_init(CURL_GLOBAL_DEFAULT);
	
		/* Check for errors */
		if (res != CURLE_OK) {		
			vdDebug_LogPrintf("curl_global_init failed");	
			return d_OK;
		}
		vdDebug_LogPrintf("curl_global_init successful");
	
		curl = curl_easy_init();
	
		if(curl) {
		
			vdDebug_LogPrintf("curl_easy_init successful");

			//vdDebug_LogPrintf("fIP_URL[%d]", strGCash.fIP_URL);
		
			if (TransType == PHQR_INITIALIZE)
			{
				//#if 1
				//memset(szURL, 0x00, sizeof(szURL));
				//if (strGCash.fIP_URL == TRUE)
				//	vdSetNewURL(szURL, strGCash.GenerateQRURL, inCPTIndex);
				//else
				//strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/initialize");
				sprintf(szURL,"%s",strPHQR.szInitializeURL);
			
				vdDebug_LogPrintf("Initialize URL is %s",szURL);
			}else if (TransType == PHQR_LOGON){
				//strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/login");
				sprintf(szURL,"%s",strPHQR.szLogonURL);		
				vdDebug_LogPrintf("Logon URL is %s",szURL);

			}else if (TransType== PHQR_GENERATE_QR)	{
				//strcpy(szURL, "https://metrobankqrpay.metrobankcard.com/api/merchant/generate-qr");
				sprintf(szURL,"%s",strPHQR.szGenerateQrURL);
				vdDebug_LogPrintf("GENERATE QR URL is %s",szURL);
				
			}else if (TransType == PHQR_CHECK_STATUS){
				//strcpy(szURL, "https://210.23.217.105:443/api/merchant/generate-qr");
				sprintf(szURL,"%s",strPHQR.szCheckTransctionURL);
				vdDebug_LogPrintf("CHECK STATUS URL is %s",szURL);
			}


			vdDebug_LogPrintf("testlang URL %s", szURL);
			
		
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

			
			curl_easy_setopt(curl, CURLOPT_URL,szURL);
			
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		

			/* we want to use our own read function */
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_phqr_callback);

			/* pointer to pass to our read function */
			curl_easy_setopt(curl, CURLOPT_READDATA, (void *)contents);

			/* send all data to this function  */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_phqr_callback);

			/* we pass our 'chunk' struct to the callback function */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);

			curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

			/* if we don't provide POSTFIELDSIZE, libcurl will strlen() byitself */ 

			struct curl_slist *headerlist = NULL;

			//Set content length;		
			memset(szConent_len,0x00,sizeof(szConent_len));
			sprintf(szConent_len,"content-length: %d",strlen(postthis));
			vdDebug_LogPrintf("content-length: %d",strlen(postthis));		

			vdDebug_LogPrintf("content-length: %s",postthis);	

			sprintf(szSignature,"Signature: %s",szGlobalSignature);


            memset(szApiKey, 0x00, sizeof(szApiKey));	
            memset(szApiKeyHeader, 0x00, sizeof(szApiKeyHeader));

			//if (strPHQR.inAPIKeyCreated == 1){
			//	strcpy(strPHQR.szAPIKey,szApiKey);
			//}else{
				memset(szTempBuffer, 0x00, sizeof(szTempBuffer));
				sprintf(szTempBuffer,"%s%s",srTransRec.szMID, srTransRec.szTID);

				vdDebug_LogPrintf("testlang test buff %s", szTempBuffer);
				inCompute_HMAC_SHA256(szTempBuffer, szApiKey);
				strupr(szApiKey);
				vdDebug_LogPrintf("sha256 api-key %s", szApiKey);
				//strcpy(szApiKey, "33CBE02FA94707CFFCFD327D502765B949D5CCAC918437981F72FB5D2D417BFB");
				
				//strPHQR.inAPIKeyCreated = 1;
				//inPHQRSave(1);
			//}

			                  
			sprintf(szApiKeyHeader,"Api-Key: %s", szApiKey);

            memset(szDeviceKeyHeader, 0x00, sizeof(szDeviceKeyHeader));


			//inCTOSS_GetEnvDB("DEVKEY", szDeviceKeyHeader);
			sprintf(szDeviceKeyHeader,"Device-Key: %s", strPHQR.szDeviceKey);
			vdDebug_LogPrintf("testlang devkey %s", szDeviceKeyHeader);


			if ((TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))	{
				memset(szSessionKeyHeader, 0x00, sizeof(szSessionKeyHeader));
				sprintf(szSessionKeyHeader,"Session-Key: %s", strPHQR.szSessionKey);
				
				vdDebug_LogPrintf("testlang szSessionKey %s", szSessionKeyHeader);
			}


			memset(szCallerIDHeader, 0x00, sizeof(szCallerIDHeader));
			
			sprintf(szCallerIDHeader,"Caller-Id: %s", strPHQR.szCallerID);
			
			vdDebug_LogPrintf("testlang Caller-Id: %s", szCallerIDHeader);

			//testlang-remove later
			//memset(szDeviceKeyHeader, 0x00, sizeof(szDeviceKeyHeader));
			//memset(szApiKeyHeader, 0x00, sizeof(szApiKeyHeader));
			//memset(szSessionKeyHeader, 0x00, sizeof(szSessionKeyHeader));

			//strcpy(szDeviceKeyHeader,"Device-Key: 22909A4BBD6239DD153E939E9A847D0FA8CDA60262D22AB06959BA7F55408068");
			//strcpy(szApiKeyHeader,"Api-Key: B4ABAA4412BB8D122143DC236936B2A0C0DB0E7AE3281863E287C3F7A2FED1E8");
			//strcpy(szSessionKeyHeader,"Session-Key: F9E79286C0647C9A6D3DE473335EE7D892059BB38471859C0AB98C5E73D2B696");

			//testlang-remove later
	                	
			//if (TransType == PHQR_INITIALIZE){
				vdDebug_LogPrintf("REQUEST HEADER AND BODY");
				headerlist = curl_slist_append(headerlist, "cache-control: no-cache");
				headerlist = curl_slist_append(headerlist, "Connection: keep-alive");
				headerlist = curl_slist_append(headerlist, szConent_len);
				headerlist = curl_slist_append(headerlist, "Content-Type: application/json");
				headerlist = curl_slist_append(headerlist, szApiKeyHeader);
				
				if ((TransType == PHQR_LOGON) || (TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))
					headerlist = curl_slist_append(headerlist, szDeviceKeyHeader);

				if ((TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))		
					headerlist = curl_slist_append(headerlist, szSessionKeyHeader);

				headerlist = curl_slist_append(headerlist, szCallerIDHeader);

				headerlist = curl_slist_append(headerlist, szSignature);
				vdDebug_LogPrintf("curl_slist_append data1=[%s]",headerlist->data);
				vdDebug_LogPrintf("curl_slist_append data2=[%s]",headerlist->next->data);
				vdDebug_LogPrintf("curl_slist_append data3=[%s]",headerlist->next->next->data);
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->data);
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->data);
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->data);			
				vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->next->data);
				
				if ((TransType == PHQR_LOGON) || (TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))	
					vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->next->next->data);

			if ((TransType == PHQR_GENERATE_QR) || (TransType == PHQR_CHECK_STATUS))	
					vdDebug_LogPrintf("curl_slist_append data4=[%s]",headerlist->next->next->next->next->next->next->next->next->data);

        	//}	
		
		
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		
        	//if (TransType == PHQR_INITIALIZE)
			//{
				vdDebug_LogPrintf("DATA TO SEND %s",postthis);
				vdDebug_LogPrintf("POSTFIELDSIZE %li",(long)strlen(postthis));
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));			
        	//}

        	//for (i = 1; i<= strBDOPAY.inCurlRetries;i++){

			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT , strPHQR.inCurlConnectTimeout); 
 			curl_easy_setopt(curl, CURLOPT_TIMEOUT, strPHQR.inCurlRequestTimeout);

			/* Perform the request, res will get the return code */ 
			res = curl_easy_perform(curl);

			vdDebug_LogPrintf("curl_easy_perform %d retries=%d", res, i);


			if( fXMLPrint && 
			((res == CURLE_OPERATION_TIMEDOUT) || (res == CURLE_OK)))
			{

				char szReqBuff[2000] = {0};

				strcpy(szReqBuff, postthis);
			
				inPrintXMLPacket("TX-PHQR BODY", szReqBuff, strlen(szReqBuff), TRUE);
			}


			/* Check for errors */ 
			if(res != CURLE_OK) 
			{
				vdDebug_LogPrintf("curl_easy_perform %s ", curl_easy_strerror(res));
				//GetDPerformResutlMessage(res);	

				vdDebug_LogPrintf("curl_easy_cleanup !!!");			
				curl_easy_cleanup(curl);// always cleanup		

				vdDebug_LogPrintf("curl_slist_free_all !!!");			
				curl_slist_free_all(headerlist);// cleanup headerlist

				vdDebug_LogPrintf("curl_global_cleanup !!!");			
				curl_global_cleanup();// we're done with libcurl, so clean it up

				vdDebug_LogPrintf("curl_easy_reset !!!");
				curl_easy_reset(curl);	// reset all handles

				vdDebug_LogPrintf("Encountered error!!!");
            
				continue;
				//return(res);
			}
			else
			{
				vdCTOS_DispStatusMessage("RECEIVING...");
				/* always cleanup */ 
				curl_easy_cleanup(curl);			
			
				vdDebug_LogPrintf("curl_slist_free_all !!!");			
				curl_slist_free_all(headerlist); // cleanup headerlist

				vdDebug_LogPrintf("curl_global_cleanup !!!");			
				curl_global_cleanup();// we're done with libcurl, so clean it up

				vdDebug_LogPrintf("curl_easy_reset !!!");			
				curl_easy_reset(curl);// reset all handle
				break;
			}
		
			/* always cleanup */ 
			//vdDebug_LogPrintf("curl_easy_cleanup !!!");
			//curl_easy_cleanup(curl);
		}
	}
	#endif
	
    return inResult;
}

void vdPHQRCreatePOSTData(char *szBuffer, int inType){

	char	szISO8601Time[50+1];
	
	BYTE uszMD5Data[200+1] = {0};
	int Result;
	BYTE szSignature[2000];
	BYTE szRequestMID[100];
	BYTE szSerialNum[16+1];
	BYTE szConcatSerialNumber[20];

	
	char szTemp[d_LINE_SIZE + 1] = {0};
	char szTemp4[d_LINE_SIZE + 1] = {0};
	
	char szTemp1[d_LINE_SIZE + 1] = {0};
	
	

    //create request MID
    memset(szRequestMID, 0x00, sizeof(szRequestMID));
	memset(szConcatSerialNumber, 0x00, sizeof(szConcatSerialNumber));
	usGetSerialNumber(szSerialNum);

	strncpy(szConcatSerialNumber, szSerialNum, 15);
		
    //if (strPHQR.inRegMIDCreated == 1){
	//	strcpy(szRequestMID, strPHQR.szRequestMessageID);
    //}else{
    	memset(szRequestMID, 0x00, sizeof(szRequestMID));
		//usGetSerialNumber(szSerialNum);
		//sprintf(szRequestMID,"%s%s%s",szSerialNum, srTransRec.szTID, srTransRec.szMID);
		strncpy(szRequestMID, szSerialNum, 15);
		strcat(szRequestMID, srTransRec.szMID);
		strcat(szRequestMID,"00");
		//strPHQR.inRegMIDCreated = 1;
		//inPHQRSave(1);	
    //}
	


	memset(szISO8601Time,0x00,sizeof(szISO8601Time));
	vdGet_ISO8601_Clock(szISO8601Time);
	
    memset(szBuffer, 0x00, sizeof(szBuffer));

    if (inType == PHQR_INITIALIZE){

		vdDebug_LogPrintf("testlang11 phqr INITIALIZE");

		sprintf(szBuffer, "{\"mid\":\"%s\",\"reqMsgId\":\"%s\",\"reqTime\":\"%s\",\"serialNo\":\"%s\",\"tid\":\"%s\"}",
		srTransRec.szMID,//mid,
		szRequestMID,//reqmsgid
		szISO8601Time,//reqtime
		szConcatSerialNumber,//serialno,
		srTransRec.szTID//tid);
    	);
    }else if (inType == PHQR_LOGON){
		vdDebug_LogPrintf("testlang11 phqr LOGON");

		//testlang
		//memset(strPHQR.szUsername, 0x00, sizeof(strPHQR.szUsername));
		//strcpy(strPHQR.szUsername, "admin");
		
		
		//memset(strPHQR.szPassword, 0x00, sizeof(strPHQR.szPassword));
		//strcpy(strPHQR.szPassword, "123456");

		//testlang
 	    sprintf(szBuffer, "{\"reqMsgId\":\"%s\",\"reqTime\":\"%s\",\"tid\":\"%s\",\"mid\":\"%s\",\"username\":\"%s\",\"password\":\"%s\"}",
         szRequestMID,
         szISO8601Time,
         srTransRec.szTID,
         srTransRec.szMID,
         strPHQR.szUsername,
         strPHQR.szPassword
        );

	}else if (inType == PHQR_GENERATE_QR){

	vdDebug_LogPrintf("testlang11 GENERATE PHQR");
	
		memset(szTemp, 0x00, sizeof(szTemp));
		memset(szTemp4, 0x00, sizeof(szTemp4));
		
		memset(szTemp1, 0x00, sizeof(szTemp1));
		wub_hex_2_str(srTransRec.szBaseAmount, szTemp, AMT_BCD_SIZE);
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTemp4);

		
		wub_hex_2_str(srTransRec.szInvoiceNo, szTemp1, INVOICE_BCD_SIZE);
		
	    sprintf(szBuffer, "{\"reqMsgId\":\"%s\",\"reqTime\":\"%s\",\"tid\":\"%s\",\"mid\":\"%s\",\"txnAmount\":\"%s\",\"currency\":608,\"terminalInvoice\":\"%s\"}",
		szRequestMID,
		szISO8601Time,
		srTransRec.szTID,
		srTransRec.szMID,
		szTemp4,
		szTemp1
		);

	}else if (inType == PHQR_CHECK_STATUS){

	 	vdDebug_LogPrintf("testlang11 phqr check status");
    	sprintf(szBuffer,"{\"mid\":\"%s\",\"reqMsgId\":\"%s\",\"reqTime\":\"%s\",\"tid\":\"%s\",\"txnRefNo\":\"%s\"}",
		srTransRec.szMID,
		szRequestMID,
		szISO8601Time,
		srTransRec.szTID,
		szGlobalRefNumber
		);

		
	}

    vdDebug_LogPrintf("POSTData:%s", szBuffer);
	
    memset(szSignature, 0x00, sizeof(szSignature));
	//testRSA(szBuffer, szSignature );
	inRSASignEx(szBuffer,szSignature);
		
    memset(szGlobalSignature, 0x00, sizeof(szGlobalSignature));
	strcpy(szGlobalSignature, szSignature);

	vdDebug_LogPrintf("szSignature=[%s]", szGlobalSignature);



}

void vdGet_ISO8601_Clock(char *szClock)
{
	int inRet = 0;
	char szDay[3+1];
	char szMonthNames[36+1] = "JanFebMarAprMayJunJulAugSepOctNovDec";
	char szMonth[4] = {0};
	BYTE szCurrentTime[20];
	BYTE szCurrentDate[2+1];
	CTOS_RTC SetRTC;
	
	CTOS_RTCGet(&SetRTC);
			
	vdDebug_LogPrintf("%02d:%02d:%02d :: %02d %02d %02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond,SetRTC.bMonth,SetRTC.bDay,2000+SetRTC.bYear);
		
	sprintf(szClock,"%d-%02d-%02dT%02d:%02d:%02d+08:00",2000+SetRTC.bYear,SetRTC.bMonth,SetRTC.bDay,SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
	vdDebug_LogPrintf("szClock[%s]",szClock);

}

size_t read_phqr_callback(void *dest, size_t size, size_t nmemb, void *userp)
{
	struct MemoryStruct *wt = (struct MemoryStruct *)userp;
	size_t buffer_size = size*nmemb;

	vdDebug_LogPrintf("read_callback %s", dest);

	if (wt->size) {
		/* copy as much as possible from the source to the destination */
		size_t copy_this_much = wt->size;
		if (copy_this_much > buffer_size)
			copy_this_much = buffer_size;
		memcpy(dest, wt->memory, copy_this_much);

		wt->memory += copy_this_much;
		wt->size -= copy_this_much;
		
		vdDebug_LogPrintf("read_callback=[%d][%s]", wt->size, dest);
		return copy_this_much; /* we copied this many bytes */
	}

	return 0; /* no more data left to deliver */
}


size_t write_phqr_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);

	vdDebug_LogPrintf("write_callback=[%d][%s]", realsize, contents);
	DebugAddHEX("CONTENTS",contents,realsize);
	
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}


#if 0
int inCTOS_WechatQRRequest(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_WechatQRRequest--");
	
   // inSetColorMenuMode();
		
	CTOS_LCDTClearDisplay();
	vdCTOS_SetTransType(SALE);
	inIITRead(WECHAT_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	vdDispTransTitle(srTransRec.byTransType);
	vdCTOS_DispStatusMessage("PROCESSING");  

    vdCTOS_TxnsBeginInit();

/*
	if (fECRTxnFlg == 1){
		
		inRet = inCTOS_MultiAPGetData();
		if(d_OK != inRet)
			return inRet;
		
	}
*/

	// Static	
	srTransRec.IITid = WECHAT_ISSUER_INDEX;
	srTransRec.HDTid = WECHAT_HOST_INDEX;
	strCDT.HDTid = WECHAT_HOST_INDEX;
	inIITRead(srTransRec.IITid);
	inCPTRead(srTransRec.HDTid);
	strcpy(srTransRec.szCardLable, strIIT.szIssuerLabel);

	vdDebug_LogPrintf("ISSUER %d %d %d", strIIT.inIssuerNumber, strIIT.inCheckHost, srTransRec.IITid);
	inRet = inCTOS_WechatQRRequestFlowProcess();

	if (isCheckTerminalMP200() == d_OK && inRet == d_OK)
	{
		if (inRet == d_OK || ErmTrans_Approved)
	    {
	    	// Preconnect transferred to inCTOSS_UploadReceipt
	    	/*
	    	inRet = inCTOS_PreConnect();
			if (inRet != d_OK)
			{
				vdCTOS_TransEndReset();
				return inRet;
			}
			*/

			inCTOSS_UploadReceipt(FALSE, FALSE);
	    }
	}
	
	inCTOS_inDisconnect();
	vdCTOS_TransEndReset();
	
	return inRet;
}

int inCTOS_WechatQRRequestFlowProcess(void)
{
    int inRet = d_NO;
	int inEVENT_ID = 0;
	char szTime[6+1];	
    CTOS_RTC SetRTC;
	
	//fTimeOutFlag = FALSE; /*BDO: Flag for timeout --sidumili*/
	srTransRec.iXMLType = WECHAT_ALIPAY_REQUEST_QRCODE;	
	vdCTOS_SetTransType(SALE);
	    
    inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;

    inRet = inCTOS_GetTxnPassword();
    if(d_OK != inRet)
        return inRet;

	//inRet = inCTOS_TEMPCheckAndSelectMutipleMID();
	//if(d_OK != inRet)
	//	return inRet;

	inRet = inCTOS_CheckIssuerEnable();
    if(d_OK != inRet)
        return inRet;	
		
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
        return inRet;


#if 0
	vdDebug_LogPrintf("srTransRec.IITid[%d]", srTransRec.IITid);
	vdDebug_LogPrintf("srTransRec.HDTid[%d]", srTransRec.HDTid);
	vdDebug_LogPrintf("srTransRec.MITid[%d]", srTransRec.MITid);
    if (inMMTReadRecordEx(srTransRec.HDTid,srTransRec.MITid) != d_OK) // Also check if Merchant using host is enabled
    {
        
		vdDebug_LogPrintf("TRANSACTION not allowed");
        vdDisplayErrorMsgResp2(" ", "TRANSACTION", "NOT ALLOWED");
		inDatabase_TerminalCloseDatabase();
        return(d_NO);
    }	
#endif
	
	inDatabase_TerminalCloseDatabase();

    inRet = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inRet)
        return inRet;


	inRet = inCTOS_CheckMustSettle();
    if(d_OK != inRet)
        return inRet;

	inRet = inCTOS_GetTxnBaseAmount();
	if(d_OK != inRet)		
		return inRet;
	
	inRet = inCTOS_SelectQRPresented();
		if(d_OK != inRet)
			return inRet;

	if (iGetQRPresentedType() == CPQR_TYPE)
	{
		inRet = inCTOS_ScanQRCode();
			if(d_OK != inRet)
			return inRet;
	}
	
	CTOS_RTCGet(&SetRTC);  
	sprintf(szTime,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
    wub_str_2_hex(szTime,srTransRec.szTime,6);
	
	srTransRec.iXMLType = WECHAT_ALIPAY_REQUEST_QRCODE;
	vdCTOS_SetTransType(SALE);

	//inRet = inXML_CommsInit();	
	//if (inRet != d_OK){	
		//strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
	//	return inRet;	
	//}

	inRet = inCTOS_GetInvoice();
	if(d_OK != inRet)
			return inRet;
	
	//inRet = inXMLCreateRequestData();
	//if (inRet != d_OK)
	//	return inRet;	
	
	//inRet = inXMSendAndReceiveRequestMessage();
	//if (inRet != d_OK){
	//	inRet = vdXMLProcessReversal();		
	//	return inRet;
	//}	

	if (iGetQRPresentedType() == MPQR_TYPE) 
	{
	
	//testlang
	memset(srTransRec.szXMLcode_url, 0x00, sizeof(srTransRec.szXMLcode_url));
	strcpy(srTransRec.szXMLcode_url,"00020101021228730015ph.ppmi.p2micro0111BNORPHMMXXX031230551000000204123055100000020503311520407435303608540410.05802PH5908test BDO6008Shenzhen6222050600000507081111111188400012ph.ppmi.qrph01201020211008107669184963047958");

	//testlang
		inPrintWechatQRCode(); // Print QRCode

		// Save to Retrieve
		iRetrieveSaveData();

		// sidumili: added for display QRCode in S1
		inRet = inUserConfirmQRCode();
		if (inRet != d_OK){
			//strcpy(srTransRec.szECRRespCode, ECR_OPER_CANCEL_RESP);//for ecr
			return inRet;
		}

		srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
		srTransRec.iXMLRetrieveType = WECHAT_AUTO_RETRIEVE;
		srRetrieveRec.byVoided = FALSE;
		srTransRec.byVoided = FALSE;
		
		vdCTOS_SetTransType(SALE);

		//inRet = inXML_CommsInit();	
		//if (inRet != d_OK){
			//strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
			//return inRet;
		//}

		inRet = inXMLCreateRequestData();
		if (inRet != d_OK)
			return inRet;	

		inRet = inXMSendAndReceiveRetrieveMessage(FALSE);
		vdDebug_LogPrintf("sidumili:::inRet=[%d], fXMLTradeStateOK=[%d], szTradeState=[%s]", inRet, srTransRec.fXMLTradeStateOK, srTransRec.szTradeState);
		if (inRet != d_OK)
		{
			// Send Close Interface
			if (!srTransRec.fXMLTradeStateOK && strlen(srTransRec.szTradeState) > 0 && (strcmp(srTransRec.szTradeState, TRADE_STATE_NOTPAY) == 0))
			{
				vdDisplayMessageStatusBox(1, 8, "SENDING CLOSE INTERFACE", MSG_PLS_WAIT, MSG_TYPE_SEND);
				CTOS_Delay(2000);
				inRet = inXML_CommsInit();
				if (inRet != d_OK)
				{
					vdDisplayMessageBox(1, 8, "SENDING", "CLOSE INTERFACE", "FAILED", MSG_TYPE_ERROR);
				}
				else
				{
					srTransRec.iXMLType = WECHAT_ALIPAY_CLOSE_QRCODE;
					inXMLCreateRequestData();
					inRet = inXMSendAndReceiveCloseQRCodeMessage();
					if (inRet != d_OK)
						vdDisplayMessageBox(1, 8, "SENDING", "CLOSE INTERFACE", "FAILED", MSG_TYPE_ERROR);
					else
						vdDisplayMessageBox(1, 8, "SENDING", "CLOSE INTERFACE", "SUCCESS", MSG_TYPE_SUCCESS);

					inRet = d_NO;
				}
				
				CTOS_Beep();
				CTOS_Delay(1000);			
			}
			
			return inRet;
		}

		iSetDummyFieldForERM();
	}
	else
	{
		iRetrieveSaveData();
	}
	
	iRetrieveUpdateDataByOutTradeNo(); 	// Update Retrieve Data by OutTradeNo -- sidumili
	
	inRet = inCTOS_SaveBatchTxn();
    if(d_OK != inRet)
        return inRet;
	
    inRet = inCTOS_UpdateAccumTotal();
    if(d_OK != inRet)
        return inRet;

	//Added to send correct time to ECR
	memcpy(szTime,&srTransRec.szXMLtime_end[8],6);
	wub_str_2_hex(szTime,srTransRec.szTime,6);

	//fECRBuildSendOK = TRUE;	
	//if (fECRBuildSendOK){
	//	inRet = inMultiAP_ECRSendSuccessResponse();
	//}

	if (isCheckTerminalMP200() == d_OK)
	{
		vdCTOSS_DisplayStatus(d_OK);

		inRet = inCTOSS_ERM_ReceiptRecvVia();
		if(d_OK != inRet)
	    	return inRet;
	}
	
	inRet = ushCTOS_printReceipt();
	if (inRet != d_OK)
		return inRet;
	else
	{
		if (fFSRMode() == TRUE)
         {  
              inCTOSS_UploadReceipt(FALSE, FALSE);	 
         }
         vdSetErrorMessage("");
	}

	if(strTCT.byERMMode != 0)
		ErmTrans_Approved  = TRUE;
	
	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	CTOS_LCDTClearDisplay(); 
	
    return d_OK;
}

int inCTOS_WechatQRRetrieve(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_WechatQRRetrieve--");
	
    //inSetColorMenuMode();
		
	CTOS_LCDTClearDisplay();
	srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
	vdCTOS_SetTransType(PRE_AUTH);
	inIITRead(WECHAT_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	vdDispTransTitle(srTransRec.byTransType);
	vdCTOS_DispStatusMessage("PROCESSING");  

    vdCTOS_TxnsBeginInit();

	// Static	
	srTransRec.IITid = WECHAT_ISSUER_INDEX;
	srTransRec.HDTid = WECHAT_HOST_INDEX;
	strCDT.HDTid = WECHAT_HOST_INDEX;
	inIITRead(srTransRec.IITid);
	inCPTRead(srTransRec.HDTid);
	
	inRet = inCTOS_WechatQRRetrieveFlowProcess();

	if (isCheckTerminalMP200() == d_OK && inRet == d_OK)
	{
		if (inRet == d_OK || ErmTrans_Approved)
	    {
	    	// Preconnect transferred to inCTOSS_UploadReceipt
	    	/*
	    	inRet = inCTOS_PreConnect();
			if (inRet != d_OK)
			{
				vdCTOS_TransEndReset();
				return inRet;
			}
			*/

			inCTOSS_UploadReceipt(FALSE, FALSE);
	    }
	}
	
	inCTOS_inDisconnect();
	vdCTOS_TransEndReset();
	
	return inRet;
}

int inCTOS_WechatQRRetrieveFlowProcess(void)
{
    int inRet = d_NO;
	int inEVENT_ID = 0;
	BOOL fExist = FALSE;
	
	//fTimeOutFlag = FALSE; /*BDO: Flag for timeout --sidumili*/
	srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;	
	vdCTOS_SetTransType(PRE_AUTH);
	   
    inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;

    inRet = inCTOS_GetTxnPassword();
    if(d_OK != inRet)
        return inRet;

	//inRet = inCTOS_TEMPCheckAndSelectMutipleMID();
	//if(d_OK != inRet)
	//	return inRet;

	inRet = inCTOS_CheckIssuerEnable();
    if(d_OK != inRet)
        return inRet;		
	
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
        return inRet;

	vdDebug_LogPrintf("srTransRec.IITid[%d]", srTransRec.IITid);
	vdDebug_LogPrintf("srTransRec.HDTid[%d]", srTransRec.HDTid);
	vdDebug_LogPrintf("srTransRec.MITid[%d]", srTransRec.MITid);	
#if 0
    if (inMMTReadRecordEx(srTransRec.HDTid,srTransRec.MITid) != d_OK) // Also check if Merchant using host is enabled
    {
        
		vdDebug_LogPrintf("TRANSACTION not allowed");
        vdDisplayErrorMsgResp2(" ", "TRANSACTION", "NOT ALLOWED");
		inDatabase_TerminalCloseDatabase();
        return(d_NO);
    }	
#endif
	
	inDatabase_TerminalCloseDatabase();


    inRet = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inRet)
        return inRet;

	inRet = inCTOS_CheckMustSettle();
    if(d_OK != inRet)
        return inRet;

	vdSetGlobalVariable();
	
	if (strGBLVar.fGBLvRetrieveList)
	{
		inRet = iXMLRetrieveListView();
		if(d_OK != inRet)
		return inRet;

		vdXMLSelectedRetrieve();		
	}
	else
	{		
		inRet = iXMLTransactionIDEntry();
		if(d_OK != inRet)
			return inRet;
	}

	vdDebug_LogPrintf("From ListView->[%02X%02X%02X]", srTransRec.szInvoiceNo[0], srTransRec.szInvoiceNo[1], srTransRec.szInvoiceNo[2]);
	
	srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
	srTransRec.iXMLRetrieveType = WECHAT_ALIPAY_MANUAL_RETRIEVE;
	vdCTOS_SetTransType(PRE_AUTH);
	
	inRet = inXML_CommsInit();	
	if (inRet != d_OK)
		return inRet;	
	
	inRet = inXMLCreateRequestData();
	if (inRet != d_OK)
		return inRet;	
	
	inRet = inXMSendAndReceiveRetrieveMessage(TRUE);
	if (inRet != d_OK)
		return inRet;

	iRetrieveUpdateDataByOutTradeNo(); 	// Update Retrieve Data by OutTradeNo -- sidumili
	
	iSetDummyFieldForERM();
	
	vdDebug_LogPrintf("-->>Wechat Search for TransactionID");
	vdDebug_LogPrintf("szXMLtransaction_id[%s].HDTid[%d].MITid[%d]", srTransRec.szXMLtransaction_id, srTransRec.HDTid, srTransRec.MITid);
	vdDebug_LogPrintf("srRetrieveRec.fRetrieveInList=[%d]", srRetrieveRec.fRetrieveInList);

	if (srRetrieveRec.fRetrieveInList && strlen(srTransRec.szXMLtransaction_id) <= 0)
	{		
		fExist = TRUE;
		wub_str_2_hex(strGBLVar.szGBLvInvoiceNo,srTransRec.szInvoiceNo,INVOICE_ASC_SIZE);
	}
	else
	{
		fExist = iDigitalWalletSearchTransactionID((char*)srTransRec.szXMLtransaction_id, srTransRec.HDTid, srTransRec.MITid);
	}
		
	vdDebug_LogPrintf("fExist[%d].InvoiceNo[%02X%02X%02X]", fExist, srTransRec.szInvoiceNo[0], srTransRec.szInvoiceNo[1], srTransRec.szInvoiceNo[2]);
	
	if (fExist == TRUE)
	{
		vdDebug_LogPrintf("Record found.!!!");		

		inCTOS_BatchSearch();
	}		
	else
	{
		vdDebug_LogPrintf("Not Record found.!!!");

		inRet = inCTOS_GetInvoice();
		if(d_OK != inRet)
			return inRet;
	
		inRet = inCTOS_SaveBatchTxn();
	    if(d_OK != inRet)
	        return inRet;
		
	    inRet = inCTOS_UpdateAccumTotal();
	    if(d_OK != inRet)
	        return inRet;
	}

	if (isCheckTerminalMP200() == d_OK)
	{
		vdCTOSS_DisplayStatus(d_OK);

		inRet = inCTOSS_ERM_ReceiptRecvVia();
		if(d_OK != inRet)
	    	return inRet;
	}
	
	inRet = ushCTOS_printReceipt();
	if (inRet != d_OK)
		return inRet;

	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	CTOS_LCDTClearDisplay(); 
	
    return d_OK;
}

int inCTOS_WechatQRDisplayTest(void)
{	
	CTOS_LCDTClearDisplay();
	vdCTOS_SetTransType(SALE);
	vdDispTransTitle(srTransRec.byTransType);
	vdCTOS_DispStatusMessage("PROCESSING");  

    vdCTOS_TxnsBeginInit();

	// Static	
	srTransRec.IITid = WECHAT_ISSUER_INDEX;
	srTransRec.HDTid = WECHAT_HOST_INDEX;
	strCDT.HDTid = WECHAT_HOST_INDEX;
	inIITRead(srTransRec.IITid);
	inCPTRead(srTransRec.HDTid);
	
	iWechatDisplayQRCode(TRUE);
	
	inCTOS_inDisconnect();
	vdCTOS_TransEndReset();
}

int iXMLSelectSearchType(void)
{
	BYTE key;
	int inRet;
	char szDebug[40 + 1]={0};
	BYTE bySelect1;
	BYTE bySelect2;				

	setLCDPrint(6, DISPLAY_POSITION_LEFT, "SELECT SEARCH TYPE:");


	if (strTCT.byTerminalType % 2 == 0)
	{
		CTOS_LCDTPrintAligned(7, "[1]TRANSACTION ID", d_LCD_ALIGNLEFT);
		CTOS_LCDTPrintAligned(8, "[2]INVOICE NUMBER", d_LCD_ALIGNLEFT);
	}
	else
	{
		CTOS_LCDTPrintAligned(7, "TRANSACTION ID", d_LCD_ALIGNLEFT);
		CTOS_LCDTPrintAligned(7, "INVOICE NO", d_LCD_ALIGNRIGHT);
		CTOS_LCDTPrintAligned(8, "[F1]", d_LCD_ALIGNLEFT);
		CTOS_LCDTPrintAligned(8, "[F4]", d_LCD_ALIGNRIGHT);
	}
	
	do{
	
		key=WaitKey(inGetIdleTimeOut(TRUE));

		if (strTCT.byTerminalType % 2 == 0)
		{
			bySelect1 = d_KBD_1;
			bySelect2 = d_KBD_2;
		}
		else
		{
			bySelect1 = d_KBD_F1;
			bySelect2 = d_KBD_F4;
		}
		
		if (key == bySelect1){
			inRet = d_OK;
			srTransRec.iXMLSearchType = WECHAT_ALIPAY_SEARCH_TXNID;		
			break;
		}else if (key == bySelect2){
			inRet = d_OK;
			srTransRec.iXMLSearchType = WECHAT_ALIPAY_SEARCH_INVNO;
			break;
		}else if (key == d_KBD_CANCEL){

			//sidumili: disconnect communication when USER PRESS CANCEL KEY
			if (strCPT.inCommunicationMode == DIAL_UP_MODE){
							inCTOS_inDisconnect();
			}
			//sidumili: disconnect communication when USER PRESS CANCEL KEY
			
			inRet = d_NO;
			break;
		}
		else
			CTOS_Beep();
	
	}while(1);
	
	return inRet;
	
	
}

int inCTOS_Wechat_Alipay_QRVoid(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_Wechat_Alipay_QRVoid--");
	vdDebug_LogPrintf("srTransRec.HDTid=[%d]", srTransRec.HDTid);
	
    //inSetColorMenuMode();
		
	CTOS_LCDTClearDisplay();	
	vdCTOS_DispStatusMessage("PROCESSING");  

	if(srTransRec.HDTid == WECHAT_HOST_INDEX)
	{
		srTransRec.IITid = WECHAT_ISSUER_INDEX;
		strCDT.HDTid = WECHAT_HOST_INDEX;
	}
	else if(srTransRec.HDTid == ALIPAY_HOST_INDEX)
	{
		srTransRec.IITid = ALIPAY_ISSUER_INDEX;
		strCDT.HDTid = ALIPAY_HOST_INDEX;
	}
	else if(srTransRec.HDTid == GCASH_HOST_INDEX)
	{
		srTransRec.IITid = GCASH_ISSUER_INDEX;
		strCDT.HDTid = GCASH_HOST_INDEX;
	}
	else if(srTransRec.HDTid == GRAB_HOST_INDEX)
	{
		srTransRec.IITid = GRAB_ISSUER_INDEX;
		strCDT.HDTid = GRAB_HOST_INDEX;
	}
	else if(srTransRec.HDTid == UPI_HOST_INDEX)
	{
		srTransRec.IITid = UPI_ISSUER_INDEX;
		strCDT.HDTid = UPI_HOST_INDEX;
	}

	vdDebug_LogPrintf("srTransRec.IITid=[%d]", srTransRec.IITid);
	vdDebug_LogPrintf("strCDT.HDTid=[%d]", strCDT.HDTid);

	
	inIITRead(srTransRec.IITid);
	inCPTRead(srTransRec.HDTid);

	
	inRet = inCTOS_Wechat_ALipay_QRVoidFlowProcess();

	if (isCheckTerminalMP200() == d_OK && inRet == d_OK)
	{
		if (inRet == d_OK || ErmTrans_Approved)
	    {
	    	// Preconnect transferred to inCTOSS_UploadReceipt
	    	/*
	    	inRet = inCTOS_PreConnect();
			if (inRet != d_OK)
			{
				vdCTOS_TransEndReset();
				return inRet;
			}
			*/

			inCTOSS_UploadReceipt(FALSE, FALSE);
	    }
	}
	
	inCTOS_inDisconnect();
	vdCTOS_TransEndReset();
	
	return inRet;
}

int inCTOS_Wechat_ALipay_QRVoidFlowProcess(void)
{
    int inRet = d_NO;
	int inEVENT_ID = 0;
	BOOL fExist = FALSE;

	CTOS_RTC SetRTC;
	char szTime[14+1] = {0};	

	srTransRec.iXMLType = WECHAT_ALIPAY_VOID;	
	//vdCTOS_SetTransType(VOID);
	    
    
	inRet = inCTOS_CheckVOID();
    if(d_OK != inRet)
        return inRet;
	
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
        return inRet;

	#if 0
	vdDebug_LogPrintf("srTransRec.IITid[%d]", srTransRec.IITid);
	vdDebug_LogPrintf("srTransRec.HDTid[%d]", srTransRec.HDTid);
	vdDebug_LogPrintf("srTransRec.MITid[%d]", srTransRec.MITid);	
    if (inMMTReadRecordEx(srTransRec.HDTid,srTransRec.MITid) != d_OK) // Also check if Merchant using host is enabled
    {        
		vdDebug_LogPrintf("TRANSACTION not allowed");
        vdDisplayErrorMsgResp2(" ", "TRANSACTION", "NOT ALLOWED");
		inDatabase_TerminalCloseDatabase();
        return(d_NO);
    }	
	#endif
	
	inDatabase_TerminalCloseDatabase();

	inRet = inCTOS_CheckMustSettle();
    if(d_OK != inRet)
        return inRet;

	inRet = inCTOS_ConfirmInvAmt();
	vdDebug_LogPrintf("inCTOS_ConfirmInvAmt, inRet=[%d]", inRet);
	if(d_OK != inRet)
        return inRet;

	inRet = inXML_CommsInit();	
	if (inRet != d_OK)
		return inRet;	

	inRet = inXMLCreateRequestData();
	if (inRet != d_OK)
		return inRet;	

	inRet = inXMSendAndReceiveVoidMessage();
	if (inRet != d_OK)
		return inRet;

	//Issue# 14 - Void transaction time same on wechat/alipay void receipt
	CTOS_RTCGet(&SetRTC);  
	sprintf(szTime,"20%02d%02d%02d%02d%02d%02d",SetRTC.bYear,SetRTC.bMonth, SetRTC.bDay,SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
	memset(srTransRec.szXMLtime_end, 0x00, sizeof(srTransRec.szXMLtime_end));
	strcpy(srTransRec.szXMLtime_end, szTime);
	vdDebug_LogPrintf("VOID srTransRec.szXMLtime_end [%s]", srTransRec.szXMLtime_end);

	inRet = iRetrieveUpdateData();
	if (inRet != d_OK)
		return inRet;

	iRetrieveUpdateDataByOutTradeNo(); // Update Retrieve Data by OutTradeNo -- sidumili
	
	vdDebug_LogPrintf("0. byTransType[%d].byOrgTransType[%d].szOriginTipTrType[%d]IITid[%d]", srTransRec.byTransType, srTransRec.byOrgTransType, szOriginTipTrType, srTransRec.IITid);
	inRet = inCTOS_SaveBatchTxn();
		if(d_OK != inRet)
		return inRet;
	
	vdDebug_LogPrintf("1. byTransType[%d].byOrgTransType[%d].szOriginTipTrType[%d]IITid[%d]", srTransRec.byTransType, srTransRec.byOrgTransType, szOriginTipTrType, srTransRec.IITid);
	inRet = inCTOS_UpdateAccumTotal();
		if(d_OK != inRet)
		return inRet;

	if (isCheckTerminalMP200() == d_OK)
	{
		vdCTOSS_DisplayStatus(d_OK);

		inRet = inCTOSS_ERM_ReceiptRecvVia();
		if(d_OK != inRet)
	    	return inRet;

	}
	 
	//Added to send correct time to ECR
	memcpy(szTime,&srTransRec.szXMLtime_end[8],6);
	memset(srTransRec.szTime,0x00,sizeof(srTransRec.szTime));
	wub_str_2_hex(szTime,srTransRec.szTime,6);
		
	inRet = ushCTOS_printReceipt();
	if (inRet != d_OK)
		return inRet;

	if(strTCT.byERMMode != 0)
		ErmTrans_Approved  = TRUE;
	
	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	CTOS_LCDTClearDisplay(); 
	
    return d_OK;
}

#endif
int inCompute_HMAC_SHA256(char *szInput, char *szDigest)
{
	BYTE bDigest[128+1] = {0};

	char szPartnerSecret[100];
	//fix for s1
	//char szBuffer[50];
	char szBuffer[100];
	
	vdDebug_LogPrintf("string for HMAC %s",szInput);
	memset(bDigest,0x00,sizeof(bDigest));
	memset(szPartnerSecret, 0x00, sizeof(szPartnerSecret));

	//strcpy(szPartnerSecret,"8F341559D9604517BCC2A90881B0D2DA");
	
	strcpy(szPartnerSecret,strPHQR.szMerchantKey);

	memset(szBuffer, 0x00, sizeof(szBuffer));
	sprintf(szBuffer,"%s|%s", szInput, szPartnerSecret);

	
	vdDebug_LogPrintf("string for HMAC 2 %s",szBuffer);
	
	//vdCTOSS_HMAC_SHA256((BYTE *) szInput, strlen(szInput),	szPartnerSecret,	strlen(szPartnerSecret), bDigest);
	vdComputeHMAC_SHA256(szBuffer, szDigest);
	vdDebug_LogPrintf("HMAC SIGNATURE %s len %d",bDigest,strlen(bDigest));
	DebugAddHEX("HMAC SIGNATURE",bDigest,strlen(bDigest));

	

}


int inRSASignEx(char *szMsgBody, BYTE *szEncrypted) {

	int inRetVal = -1, inLen = -1;
	char* signature = signMessageEx(szMsgBody);
	
	if(strcmp(signature,"FAIL") == 0)
		return FAIL;
	
	//bool authentic = verifySignatureEx(szMsgBody, signature);
  	
	//if ( authentic )
	//	vdDebug_LogPrintf("KEYS ARE AUTHENTIC");
	//else
	//	vdDebug_LogPrintf("KEYS ARE NOT AUTHENTIC");

	inLen = strlen(szMsgBody);
	vdDebug_LogPrintf("szMsgBody[%s] :: len[%d]",szMsgBody,inLen);
	vdDebug_LogPrintf("signature[%s] :: len[%d]",signature,strlen(signature));
	
	vdFormatSignature(signature,szEncrypted);
	vdDebug_LogPrintf("szEncrypted %s",szEncrypted);
	
	return 0;
}

bool verifySignatureEx(char *plainText, char* signatureBase64) {
  RSA* publicRSA;
  unsigned char* encMessage;
  size_t encMessageLength;
  bool authentic;

  publicRSA = createRSAWithFilename(PUBLIC_PEM,PUBLIC_KEY);
  Base64Decode(signatureBase64, &encMessage, &encMessageLength);
  bool result = RSAVerifySignature(publicRSA, encMessage, encMessageLength, plainText, strlen(plainText), &authentic);
  return result & authentic;
}


void vdFormatSignature(char *plainText, unsigned char *szConvertText)
{
	int inLoop;
	int inOffSet = 0;

	vdDebug_LogPrintf("*** vdFormatSignature ***");
	for (inLoop = 0; inLoop <= strlen(plainText); inLoop++)
	{
		if ((plainText[inLoop]	== 0x20)&&(plainText[inLoop] == ':'))
		{
			continue;
		}
		else if (plainText[inLoop]	== 0x0D)
		{
			continue;
		}
		else if (plainText[inLoop]	== 0x0A)
		{
			continue;
		}
		else if (plainText[inLoop] == 0x22)
		{
			memcpy((char *)&szConvertText[inOffSet], "\\\"", 2);
			inOffSet = inOffSet+2;
		}		 
		else
		{
			szConvertText[inOffSet] = (unsigned char)plainText[inLoop];
			inOffSet ++;
		}
	}

	vdDebug_LogPrintf("szConvertText %s len %d",szConvertText, strlen(szConvertText));
	
}


int inExtractField(unsigned char *uszRecData, char *szField, char *szSearchString){
	char *ptr;
 	char szWorkBuff1[4096+1];
 	char szWorkBuff2[4096+1];
 	char szSearchToken[2];
 	int i;


	vdDebug_LogPrintf("inExtractField");

	//vdDebug_LogPrintf("extract field %s", uszRecData);

 	memset(szSearchToken, 0x00, sizeof(szSearchToken));
 	szSearchToken[0] = '"';
 	memset(szWorkBuff1, 0x00, sizeof(szWorkBuff1));
 	ptr = NULL;
 	strcpy(szWorkBuff1,uszRecData);
 	ptr =strstr(szWorkBuff1, szSearchString);

    if (ptr == NULL)
		{
			vdDebug_LogPrintf("inExtractField  NULL");
			
			return FAIL;
   	 	}
	
 	memset(szWorkBuff2, 0x00, sizeof(szWorkBuff2));
 	strcpy(szWorkBuff2, ptr);
	

    for (i = 1; i<4; i++){


		vdDebug_LogPrintf("inExtractField  --- i [%d]", i);
		
  		memset(szWorkBuff1, 0x00, sizeof(szWorkBuff1));
  		ptr = NULL;
  		strcpy(szWorkBuff1, szWorkBuff2);
		
  
  		ptr =strstr(szWorkBuff1, szSearchToken);
  
 		memset(szWorkBuff2, 0x00, sizeof(szWorkBuff2));
  		strcpy(szWorkBuff2, ptr+1);
		
  
    }

 	memcpy(szField, szWorkBuff1, ptr - szWorkBuff1);

 
 	vdDebug_LogPrintf("szField %s-[%s] ", szSearchString, szField);

	return d_OK;
}


int inPHQR_Login_SendAndReceive(void)
	{
		int inRet, i, iWidth = 16;
		MemoryStruct write_data, read_data;
		char szTempBuff[1000];
		char szEMVString[500];
		char szErrorString[64+1];
		char szBuffer[100+1];
		char szResponseBuff[1000] = {0};
		BYTE key;
		BYTE szInvNum[6+1];
	
		BYTE szMsg[20+1];
		
		BYTE szGetInv[INVOICE_ASC_SIZE + 1] = {0};
	
		
		BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
		int inRescode;

	    vdDebug_LogPrintf("--inPHQR_Login_SendAndReceive--");

		#if 0
		for (i = 1; i<= strPHQR.inCurlRetries;i++)
		{
			inRet = inPHQRSend_requestEx(&szTempBuff, PHQR_LOGON);

            if (inRet == 7)
            {
                vdDisplayErrorMsgResp2("","Connect Failed","");
                if(fGetECRTransactionFlg())
                {
                    vdSetECRResponse(ECR_COMMS_ERR);
                    inMultiAP_ECRSendSuccessResponse();
                }
                return ST_ERROR;
            }	

            vdDebug_LogPrintf("receive data %s",szTempBuff );
            
            if (inRet == d_OK)
                break;
		}
		#else
		for (i = 1; i<= strPHQR.inCurlRetries;i++)
		{
			write_data.memory = malloc(1); 
			write_data.size = 0;	 
	
			inRet = inPHQRSend_request(&read_data,&write_data, PHQR_LOGON);
	
			//connect failed
			if (inRet == 7){
				vdDisplayErrorMsgResp2("","Connect Failed","");
				free(write_data.memory);
				 if(fGetECRTransactionFlg())
                 {
                      vdSetECRResponse(ECR_COMMS_ERR);
                      inMultiAP_ECRSendSuccessResponse();
                 }
				return ST_ERROR;
			}	
			//end

            
			
			vdDebug_LogPrintf("inPHQR_Login_SendAndReceive %d",inRet);
			
			memset(szTempBuff, 0x00, sizeof(szTempBuff));
			memcpy(szTempBuff, write_data.memory, write_data.size);
			
			free(write_data.memory);
			
			vdDebug_LogPrintf("receive data %s",szTempBuff );

			if (inRet == d_OK)
				break;
			
			
	}
    #endif
 
    memset(szBuffer, 0x00, sizeof(szBuffer));
	inExtractField(szTempBuff, szBuffer, "resultStatus");

    //get status and error message
	if ((strcmp(szBuffer, "Success") != 0) && (strcmp(szBuffer, "success") != 0)){

		//get result code
		memset(szBuffer, 0x00, sizeof(szBuffer));
		inExtractField(szTempBuff, szBuffer, "resultCode");
		inRescode = atoi(szBuffer);

		//if (inRescode == 2)
		//	return d_OK; //already initialized
			
		if (inDisplaResultcode(inRescode) != d_OK)
			vdDisplayErrorMsgResp2(" ", "LOGIN FAILED", " "); 
				
		return d_NO;
		
	}

	//get and save device key
	
	//memset(szBuffer, 0x00, sizeof(szBuffer));
	memset(strPHQR.szSessionKey, 0x00, sizeof(strPHQR.szSessionKey));
	inExtractField(szTempBuff, strPHQR.szSessionKey, "sessionKey");

	vdDebug_LogPrintf("testlang sessionKey %s", strPHQR.szSessionKey);

	
	memset(szBuffer, 0x00, sizeof(szBuffer));
	inExtractField(szTempBuff, szBuffer, "sessionExpiry");

	//format date
    //2021-11-17T11:12:10+08:00
	memset(strPHQR.szSessionExpDate, 0x00, sizeof(strPHQR.szSessionExpDate));
	strPHQR.szSessionExpDate[0]=szBuffer[2];
	strPHQR.szSessionExpDate[1]=szBuffer[3];
	strPHQR.szSessionExpDate[2]=szBuffer[5];
	strPHQR.szSessionExpDate[3]=szBuffer[6];
	strPHQR.szSessionExpDate[4]=szBuffer[8];
	strPHQR.szSessionExpDate[5]=szBuffer[9];

	//time
	memset(strPHQR.szSessionExpTime, 0x00, sizeof(strPHQR.szSessionExpTime));
	strPHQR.szSessionExpTime[0] =szBuffer[11];
	strPHQR.szSessionExpTime[1] =szBuffer[12];
	strPHQR.szSessionExpTime[2] =szBuffer[14];
	strPHQR.szSessionExpTime[3] =szBuffer[15];
	strPHQR.szSessionExpTime[4] =szBuffer[17];
	strPHQR.szSessionExpTime[5] =szBuffer[18];

	put_env_char("PHQREXPDATE",strPHQR.szSessionExpDate);
	put_env_char("PHQREXPTIME",strPHQR.szSessionExpTime);


	
	vdDebug_LogPrintf("testlang2 ret %d, date = %s", inRet, strPHQR.szSessionExpDate);
	
	vdDebug_LogPrintf("testlang2 ret %d, time = %s", inRet, strPHQR.szSessionExpTime);

	if (strlen(strPHQR.szSessionKey)> 0)
		inPHQRSave(1);
	else{
		vdDisplayErrorMsgResp2(" ", "SESSION KEY", "NOT PROVIDED"); 
		return d_NO;
	}


    memset(strPHQR.szSessionKey, 0x00, sizeof(strPHQR.szSessionKey));
	inRet = inWalletTableRead(1, srTransRec.HDTid); 

	vdDebug_LogPrintf("testlang2 ret %d, SessionKey = %s", inRet, strPHQR.szSessionKey);
	//vdDebug_LogPrintf("testlang2 ret %d, SessionKey = %s", inRet, strPHQR.szSessionExpDate);
	
///vdDebug_LogPrintf("testlang2 ret %d, SessionKey = %s", inRet, strPHQR.szSessionExpTime);

	
	//vdDebug_LogPrintf("inWalletTableRead ret %d, ussrname = %s", inRet, strPHQR.szUsername);

	//vdDebug_LogPrintf("inWalletTableRead ret %d, szPassword = %s", inRet, strPHQR.szPassword);


	

	return d_OK;

}


int inPHQR_GenerateQR_SendAndReceive(void)
	{
		int inRet, i, iWidth = 16;
		MemoryStruct write_data, read_data;
		char szTempBuff[1000];
		char szEMVString[500];
		char szErrorString[64+1];
		char szBuffer[100+1];
		char szResponseBuff[1000] = {0};
		BYTE key;
		BYTE szInvNum[6+1];
	
		BYTE szMsg[20+1];
		
		BYTE szGetInv[INVOICE_ASC_SIZE + 1] = {0};
	
		
		BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
		int inRescode;
	
	    vdDebug_LogPrintf("--inPHQR_GenerateQR_SendAndReceive--");
	
		for (i = 1; i<= strPHQR.inCurlRetries;i++)
		{
			write_data.memory = malloc(1); 
			write_data.size = 0;	 
	
			inRet = inPHQRSend_request(&read_data,&write_data, PHQR_GENERATE_QR);
	
			//connect failed
			if (inRet == 7){
				vdDisplayErrorMsgResp2("","Connect Failed","");
				free(write_data.memory);
                if(fGetECRTransactionFlg())
                {
                     vdSetECRResponse(ECR_COMMS_ERR);
                     inMultiAP_ECRSendSuccessResponse();
                }
				return ST_ERROR;
			}	
			//end

            
			
			vdDebug_LogPrintf("inBDOPayGenerateQRCode %d",inRet);
			
			memset(szTempBuff, 0x00, sizeof(szTempBuff));
			memcpy(szTempBuff, write_data.memory, write_data.size);
			
			free(write_data.memory);
			
			vdDebug_LogPrintf("receive data %s",szTempBuff );
            //inPrintXMLPacket("RX-GENQR BODY", szTempBuff, strlen(szTempBuff), FALSE);
			
			if (inRet == d_OK)
				break;
			
			
	}

    memset(szBuffer, 0x00, sizeof(szBuffer));
	inExtractField(szTempBuff, szBuffer, "resultStatus");

    //get status and error message
	if ((strcmp(szBuffer, "Success") != 0) && (strcmp(szBuffer, "success") != 0)){

		//get result code
		memset(szBuffer, 0x00, sizeof(szBuffer));
		inExtractField(szTempBuff, szBuffer, "resultCode");
		inRescode = atoi(szBuffer);

		if (inRescode == 2)
			return d_OK; //already initialized
			
		if (inDisplaResultcode(inRescode) != d_OK)
			vdDisplayErrorMsgResp2(" ", "TRANSACTION FAILED", " "); 
				

		return d_NO;
		
	}

	//get and save device key
	
	//memset(szBuffer, 0x00, sizeof(szBuffer));
	inExtractField(szTempBuff, szQRString, "qrString");
	//inExtractField(szTempBuff, szBuffer, "deviceKey");

    memset(strDigitalWallet.szout_trade_no, 0x00, sizeof(strDigitalWallet.szout_trade_no));
	inExtractField(szTempBuff, strDigitalWallet.szout_trade_no, "txnRefNo");
	strcpy(szGlobalRefNumber, strDigitalWallet.szout_trade_no);

	//vdDebug_LogPrintf("testlang devkey %s", szBuffer);

	if (strlen(szQRString)<= 0){
		vdDisplayErrorMsgResp2(" ", "QR STRING", "NOT PROVIDED"); 
		return d_NO;
	}

	

	return d_OK;

}

int inPHQR_Check_Status_SendAndReceive(void)
	{
		int inRet, i, iWidth = 16;
		MemoryStruct write_data, read_data;
		char szTempBuff[1000];
		char szEMVString[500];
		char szErrorString[64+1];
		char szBuffer[100+1];
		char szResponseBuff[1000] = {0};
		BYTE key;
		BYTE szInvNum[6+1];
	
		BYTE szMsg[20+1];
		
		BYTE szGetInv[INVOICE_ASC_SIZE + 1] = {0};
		BYTE szDateTime[30+1];
	
		
		BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
		int inRescode;
	
	
		for (i = 1; i<= strPHQR.inCurlRetries;i++)
		{
			write_data.memory = malloc(1); 
			write_data.size = 0;	 
	
			inRet = inPHQRSend_request(&read_data,&write_data, PHQR_CHECK_STATUS);

	
			//connect failed
			if (inRet == 7){
				vdDisplayErrorMsgResp2("","Connect Failed","");
				free(write_data.memory);
				 if(fGetECRTransactionFlg())
                 {
                      vdSetECRResponse(ECR_COMMS_ERR);
                      inMultiAP_ECRSendSuccessResponse();
                 }
				return ST_ERROR;
			}	
			//end

            
			
			vdDebug_LogPrintf("inBDOPayGenerateQRCode %d",inRet);
			
			memset(szTempBuff, 0x00, sizeof(szTempBuff));
			memcpy(szTempBuff, write_data.memory, write_data.size);
			
			free(write_data.memory);
			
			vdDebug_LogPrintf("receive data %s",szTempBuff );

			if (inRet == d_OK)
				break;
			
			
	}


    //TESTLANG-REMOVE LATER
    
	//memset(srTransRec.szXMLtransaction_id, 0x00, sizeof(srTransRec.szXMLtransaction_id));
	//inExtractField(szTempBuff, srTransRec.szXMLtransaction_id, "txnRefNo");
    //memset(szTempBuff, 0x00, sizeof(szTempBuff));
	//sprintf(szTempBuff,"{\"reqMsgId\":\"5F860485FB2C442485BEBD03908E2E09\",\"respTime\":\"2021-11-02T15:11:44+08:00\",\"txnRefNo\":\"0000000627\",\"invoice\":\"999999\",\"txnStatus\":\"Success\",\"resultCode\":\"00\",\"resultStatus\":\"Success\",\"message\":\"Check transaction status processed successfully.\"}");
	//TESTLANG

    memset(szBuffer, 0x00, sizeof(szBuffer));
	inExtractField(szTempBuff, szBuffer, "txnStatus");

    //get status and error message
	if ((strcmp(szBuffer, "Success") != 0) && (strcmp(szBuffer, "success") != 0)){

		//get result code
		memset(szBuffer, 0x00, sizeof(szBuffer));
		inExtractField(szTempBuff, szBuffer, "resultCode");
		inRescode = atoi(szBuffer);

		if (inRescode == 2)
			return d_OK; //already initialized
			
		if (inDisplaResultcode(inRescode) != d_OK)
			{
			     vdDisplayErrorMsgResp2(" ", "TRANSACTION FAILED", " "); 
				 if(fGetECRTransactionFlg())
                 {
                      strcpy(srTransRec.szECRRespCode, ECR_DECLINED_ERR);
                      strcpy(srTransRec.szResponseText, ECR_INVALID_TRANS_RESP);
                      inMultiAP_ECRSendSuccessResponse();
                 }
			}	 

		return d_NO;
		
	}


	//bringback-later
	memset(srTransRec.szXMLtransaction_id, 0x00, sizeof(srTransRec.szXMLtransaction_id));
	inExtractField(szTempBuff, srTransRec.szXMLtransaction_id, "txnRefNo");
    //
	
	memset(srTransRec.szXMLout_transaction_id, 0x00, sizeof(srTransRec.szXMLtransaction_id));
	inExtractField(szTempBuff, srTransRec.szXMLout_transaction_id, "invoice");

	memset(szDateTime, 0x00, sizeof(szDateTime));
	memset(srTransRec.szXMLtime_end, 0x00, sizeof(srTransRec.szXMLtime_end));
	inExtractField(szTempBuff, szDateTime, "respTime");

    //date
	srTransRec.szXMLtime_end[0] = szDateTime[0];
	srTransRec.szXMLtime_end[1] = szDateTime[1];
	srTransRec.szXMLtime_end[2] = szDateTime[2];
	srTransRec.szXMLtime_end[3] = szDateTime[3];
	srTransRec.szXMLtime_end[4] = szDateTime[5];
	srTransRec.szXMLtime_end[5] = szDateTime[6];
	srTransRec.szXMLtime_end[6] = szDateTime[8];
	srTransRec.szXMLtime_end[7] = szDateTime[9];

    //time	
	srTransRec.szXMLtime_end[8] = szDateTime[11];
	srTransRec.szXMLtime_end[9] = szDateTime[12];
	srTransRec.szXMLtime_end[10] = szDateTime[14];
	srTransRec.szXMLtime_end[11] = szDateTime[15];
	srTransRec.szXMLtime_end[12] = szDateTime[17];
	srTransRec.szXMLtime_end[13] = szDateTime[18];


	//vdDebug_LogPrintf("testlang devkey %s", szBuffer);

	//if (strlen(szQRString)<= 0){
	//	vdDisplayErrorMsgResp2(" ", "QR STRING", "NOT PROVIDED"); 
	//	return d_NO;
	//}

	

	return d_OK;

}



int inCTOS_PHQRRetrieve(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_WechatQRRetrieve--");
	
    //inSetColorMenuMode();
		
	//CTOS_LCDTClearDisplay();
	//srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
	vdCTOS_SetTransType(PRE_AUTH);
	//inIITRead(PHQR_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");  

    vdCTOS_TxnsBeginInit();

	// Static	
	srTransRec.IITid = PHQR_ISSUER_INDEX;
	srTransRec.HDTid = PHQR_HOST_INDEX;
	strCDT.HDTid = PHQR_HOST_INDEX;
	
	inDatabase_TerminalOpenDatabase();
	inIITReadEx(srTransRec.IITid);
	inCPTReadEx(srTransRec.HDTid);
	inDatabase_TerminalCloseDatabase();

	
	inRet = inCTOS_PHQRRetrieveFlowProcess();

	if (isCheckTerminalMP200() == d_OK && inRet == d_OK)
	{
		if (inRet == d_OK || ErmTrans_Approved){

			inCTOSS_UploadReceipt(FALSE, FALSE);
	    }
	}
	
	inCTOS_inDisconnect();
	vdCTOS_TransEndReset();
	
	return inRet;
}


int inCTOS_PHQRRetrieveFlowProcess(void)
{
    int inRet = d_NO;
	int inEVENT_ID = 0;
	BOOL fExist = FALSE;
	BYTE szTemp[20];
	 char szBcd[INVOICE_BCD_SIZE+1];
	BYTE szReferenceNumber[30];
	
	RETRIEVE_DATA_TABLE sLocalData;
	
	//fTimeOutFlag = FALSE; /*BDO: Flag for timeout --sidumili*/
	//srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;	
	vdCTOS_SetTransType(PRE_AUTH);

#if 0	   
    inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;

    inRet = inCTOS_GetTxnPassword();
    if(d_OK != inRet)
        return inRet;
#endif

	//inRet = inCTOS_TEMPCheckAndSelectMutipleMID();
	//if(d_OK != inRet)
	//	return inRet;

	inRet = inCTOS_CheckIssuerEnable();
    if(d_OK != inRet)
        return inRet;		
	
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
        return inRet;

	vdDebug_LogPrintf("srTransRec.IITid[%d]", srTransRec.IITid);
	vdDebug_LogPrintf("srTransRec.HDTid[%d]", srTransRec.HDTid);
	vdDebug_LogPrintf("srTransRec.MITid[%d]", srTransRec.MITid);	
#if 0
    if (inMMTReadRecordEx(srTransRec.HDTid,srTransRec.MITid) != d_OK) // Also check if Merchant using host is enabled
    {
        
		vdDebug_LogPrintf("TRANSACTION not allowed");
        vdDisplayErrorMsgResp2(" ", "TRANSACTION", "NOT ALLOWED");
		inDatabase_TerminalCloseDatabase();
        return(d_NO);
    }	
#endif
	
	inDatabase_TerminalCloseDatabase();


    inRet = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inRet)
        return inRet;

	inRet = inCTOS_CheckMustSettle();
    if(d_OK != inRet)
        return inRet;

	vdSetGlobalVariable();

	//testlang-remove later
	//strGBLVar.fGBLvRetrieveList = 1;
	
	//if (strGBLVar.fGBLvRetrieveList)
	//{
	inRet = iXMLRetrieveListView();
	if(d_OK != inRet)
	return inRet;

	vdXMLSelectedRetrieve();		
	//}
	//else
	//{		
	//	inRet = iXMLTransactionIDEntry();
	//	if(d_OK != inRet)
	//		return inRet;
	//}


	//vdDebug_LogPrintf("testlang4 refnum %s", srTransRec.szXMLtransaction_id);

   // inRet = iPHQRRetrieveReadData(atol(srTransRec.szXMLtransaction_id));
	//if(d_OK != inRet)
	//		return inRet;
	
    //save invoice from batch
    wub_str_2_hex(strGBLVar.szGBLvInvoiceNo, srTransRec.szInvoiceNo, INVOICE_ASC_SIZE);


    //save ref number
    vdDebug_LogPrintf ("testlang18 ref number %s",strGBLVar.szGBLvOutTradeNo);
	//strcpy(szGlobalRefNumber, srTransRec.szXMLout_trade_no);
	strcpy(szGlobalRefNumber, strGBLVar.szGBLvOutTradeNo);

	vdDebug_LogPrintf("testlang6 match %s", srRetrieveRec.szXMLlocal_total_fee);

	memset(srTransRec.szBaseAmount, 0x00, sizeof(srTransRec.szBaseAmount));

	wub_str_2_hex(srRetrieveRec.szXMLlocal_total_fee,srTransRec.szBaseAmount,12);
	
    memset(szTemp, 0x00, sizeof(szTemp));

	
	sprintf(szTemp, "%012ld", atol(srRetrieveRec.szXMLlocal_total_fee));
	//strcpy(szTemp, srRetrieveRec.szXMLlocal_total_fee);

	vdDebug_LogPrintf("testlang7 %s %d", szTemp, strlen(szTemp));
	
	//memset(srTransRec.szBaseAmount, 0x00, sizeof(srTransRec.szBaseAmount));
	wub_str_2_hex(szTemp,srTransRec.szBaseAmount,12);
	
	wub_str_2_hex(szTemp,srTransRec.szTotalAmount,12);


	memset(szReferenceNumber, 0x00, sizeof(szReferenceNumber));		
    strcpy(szReferenceNumber, szGlobalRefNumber);
    
	//inRet = inCTOS_GetInvoice();
	//if(d_OK != inRet)
	//		return inRet;

	
	inWalletTableRead(1, srTransRec.HDTid); 


	vdCTOS_SetTransType(SALE);

	inRet = inPHQR_Check_Status_SendAndReceive();
	if(d_OK != inRet)
		return inRet;

	//srTransRec.szXMLtransaction_id


  	vdDebug_LogPrintf("testlang17 invoice before search %02x%02x%02x", srTransRec.szInvoiceNo[0],srTransRec.szInvoiceNo[1],srTransRec.szInvoiceNo[2]);

	vdDebug_LogPrintf("testlang17 &s", szReferenceNumber);
	
	fExist = iDigitalWalletSearchTransactionID((char*)szReferenceNumber, srTransRec.HDTid, srTransRec.MITid);
	
  	vdDebug_LogPrintf("testlang17 invoice %02x%02x%02x", srTransRec.szInvoiceNo[0],srTransRec.szInvoiceNo[1],srTransRec.szInvoiceNo[2]);

	if (fExist == TRUE)
	{
		vdDebug_LogPrintf("Record found.!!!");		

		inCTOS_BatchSearch();

		
		vdDebug_LogPrintf("testlang17 batch invoice %02x%02x%02x", srTransRec.szInvoiceNo[0],srTransRec.szInvoiceNo[1],srTransRec.szInvoiceNo[2]);
	}else{

		vdDebug_LogPrintf("testlang3 before batch save");

		inRet = inCTOS_SaveBatchTxn();
    	if(d_OK != inRet)
        	return inRet;

		vdDebug_LogPrintf("testlang3 before update accum");
	
   		inRet = inCTOS_UpdateAccumTotal();
    	if(d_OK != inRet)
        	return inRet;

		//inBcdAddOne(szBcd, strTCT.szInvoiceNo, INVOICE_BCD_SIZE);
	}



	if (isCheckTerminalMP200() == d_OK)
	{
		vdCTOSS_DisplayStatus(d_OK);

		inRet = inCTOSS_ERM_ReceiptRecvVia();
		if(d_OK != inRet)
	    	return inRet;
	}

	vdDebug_LogPrintf("testlang3 before print");
	
	inRet = ushCTOS_printReceipt();
	if (inRet != d_OK)
		return inRet;
	else
	{
		if (fFSRMode() == TRUE)
         {  
              inCTOSS_UploadReceipt(FALSE, FALSE);	 
         }
         vdSetErrorMessage("");
	}

	if(strTCT.byERMMode != 0)
		ErmTrans_Approved  = TRUE;
	
	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	//CTOS_LCDTClearDisplay(); 
	


	
	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);	
		
    vdSetErrorMessage("");

	//CTOS_LCDTClearDisplay(); 

}



int iPHQRRetrieveReadData(long dSerarchRefnum)
{
	int inBatchNumOfRecord = 0;
	int *pinTransDataid = 0 ;
	int inCount = 0;
	int inResult = d_NO;
	int i = 0;
	BYTE szID[d_LINE_SIZE + 1] = {0};
	BYTE szDummy[30 + 1] = {0};
	BYTE szAmount[30 + 1] = {0};
	BYTE szTempAmount[30 + 1] = {0};
	BYTE szTransDataid[6 + 1];
	BYTE szFormattedDateTime[20+1];
	BYTE szInvoiceNo[6 + 1] = {0};
	BYTE szQRPresented[30 + 1] = {0};
	BOOL fVoided = FALSE;
	BOOL fMatch = FALSE;

    long dRecRefnum;
	

	
	vdDebug_LogPrintf("testlang4 long refnum %ld", dSerarchRefnum);
	vdDebug_LogPrintf("--iRetrieveReadData--");

	inBatchNumOfRecord = inRetrieveBatchNumRecord();
	vdDebug_LogPrintf("inRetrieveBatchNumRecord, inBatchNumOfRecord=[%d]", inBatchNumOfRecord);

	if (inBatchNumOfRecord > 0)
	{
		vdDeleteFile(TRANS_DETAIL_FILE); // Delete file
		//memset(strWriteFile.szWriteData, 0x00, sizeof(strWriteFile.szWriteData));
		//memset(strWriteFile.szWriteTemp, 0x00, sizeof(strWriteFile.szWriteTemp));
		//strWriteFile.inWriteSize = 0;
	
		pinTransDataid = (int*)malloc(inBatchNumOfRecord  * sizeof(int));
		inCount = 0;
		inRetrieveBatchByMerchandHost(inBatchNumOfRecord, srTransRec.HDTid, srTransRec.MITid, pinTransDataid);
		vdDebug_LogPrintf("inBatchNumOfRecord=[%d]", inBatchNumOfRecord);
		for(i=0; i<inBatchNumOfRecord; i++)
		{
			inResult = inDatabase_RetrieveBatchReadByTransId(&srRetrieveRec, pinTransDataid[inCount]);
		    inCount ++;

			vdDebug_LogPrintf("--------------------------------------------------------------------------------");
			vdDebug_LogPrintf("Record [%d of %d]", i, inBatchNumOfRecord);
			vdDebug_LogPrintf("TransDataid=[%d]", srRetrieveRec.TransDataid);
			vdDebug_LogPrintf("MITid=[%d], HDTid=[%d], IITid[%d]", srRetrieveRec.MITid, srRetrieveRec.HDTid, srRetrieveRec.IITid);
			vdDebug_LogPrintf("szHostLabel=[%s], szIssuerLabel=[%s]", srRetrieveRec.szHostLabel, srRetrieveRec.szIssuerLabel);
			vdDebug_LogPrintf("szTotalAmount=[%s], szBaseAmount=[%s]", srRetrieveRec.szTotalAmount, srRetrieveRec.szBaseAmount);
			vdDebug_LogPrintf("szDate=[%s], szTime=[%s]", srRetrieveRec.szDate, srRetrieveRec.szTime);
			DebugAddHEX("AAA - srRetrieveRec.szDate",srRetrieveRec.szDate,DATE_BCD_SIZE);
	        DebugAddHEX("AAA - srRetrieveRec.szTime",srRetrieveRec.szTime,TIME_BCD_SIZE);
			vdDebug_LogPrintf("szXMLlocal_total_fee=[%s]", srRetrieveRec.szXMLlocal_total_fee);

			vdDebug_LogPrintf("ref number=[%s]", srRetrieveRec.szXMLout_trade_no);			
			
			vdDebug_LogPrintf("szXMLout_refund_no=[%s]", srRetrieveRec.szXMLout_refund_no);
			vdDebug_LogPrintf("szXMLout_transaction_id=[%s]", srRetrieveRec.szXMLout_transaction_id);
			vdDebug_LogPrintf("szXMLtransaction_id=[%s]", srRetrieveRec.szXMLtransaction_id);
			vdDebug_LogPrintf("szXMLcode_url=[%s]", srRetrieveRec.szXMLcode_url);
			vdDebug_LogPrintf("szXMLtime_end=[%s]", srRetrieveRec.szXMLtime_end);			
			vdDebug_LogPrintf("byVoided=[%d]", srRetrieveRec.byVoided);

			//memcpy(srTransRec.szDate, srRetrieveRec.szDate, DATE_BCD_SIZE); //For retrieve list date
			
			
			// Amount			
			memset(szAmount, 0x00, sizeof(szAmount));
			memset(szTempAmount, 0x00, sizeof(szTempAmount));
			vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", srRetrieveRec.szXMLlocal_total_fee, szTempAmount);
			vdDebug_LogPrintf("testlang5 srRetrieveRec.szXMLlocal_total_fee=[%s]", srRetrieveRec.szXMLlocal_total_fee);
			vdDebug_LogPrintf("testlang5 szTempAmount=[%s]", szTempAmount);

			if (fVoided)
				sprintf(szAmount, "%s-%s", strCST.szCurSymbol, szTempAmount);
			else
				sprintf(szAmount, "%s%s", strCST.szCurSymbol, szTempAmount);
			
			memset(szInvoiceNo, 0x00, sizeof(szInvoiceNo));
			wub_hex_2_str(srRetrieveRec.szInvoiceNo, szInvoiceNo, INVOICE_BCD_SIZE);
			
			vdDebug_LogPrintf("Inv Num:[%02X%02X%02X]",srRetrieveRec.szInvoiceNo[0], srRetrieveRec.szInvoiceNo[1], srRetrieveRec.szInvoiceNo[2]);
			vdDebug_LogPrintf("szInvoiceNo=[%s]", szInvoiceNo);
			vdDebug_LogPrintf("szXMLout_refund_no=[%s]", srRetrieveRec.szXMLout_refund_no);
			vdDebug_LogPrintf("srRetrieveRec.inQRPresented=[%d]", srRetrieveRec.inQRPresented);
			vdDebug_LogPrintf("szAmount=[%s]", szAmount);
			vdDebug_LogPrintf("--------------------------------------------------------------------------------");


            
			dRecRefnum = atol(srRetrieveRec.szXMLout_trade_no);
			vdDebug_LogPrintf("testlang4 record refnum = %ld", dRecRefnum);

				
			vdDebug_LogPrintf("testlang4 search refnum = %ld", dSerarchRefnum);

            if (dSerarchRefnum == dRecRefnum){
				
				vdDebug_LogPrintf("testlang6 match %s", srRetrieveRec.szXMLlocal_total_fee);
				fMatch = TRUE;
				free(pinTransDataid);
				return d_OK;
               break;
            }

			
			// Write to file
			if (srRetrieveRec.HDTid > 0)
			{
			   //do nothing
			}
			
			
		}

		 free(pinTransDataid);

         if (fMatch == TRUE)
		 	return d_OK;
		 
		 vdDebug_LogPrintf("No retrieve record found...");
		 vdDisplayMessageBox(1, 8, " ", "NO RECORD FOUND", "", MSG_TYPE_INFO);
		 CTOS_Beep();
		 CTOS_Delay(1000);
		 
		 return d_NO;

		 // Save to file
		//inSaveFile((char*)strWriteFile.szWriteData, strWriteFile.inWriteSize, TRANS_DETAIL_FILE);
	}
	else
	{
		vdDebug_LogPrintf("No retrieve record found...");
		vdDisplayMessageBox(1, 8, " ", "NO RECORD FOUND", "", MSG_TYPE_INFO);
		CTOS_Beep();
		CTOS_Delay(1000);
	
		return d_NO;
	}
	
	return d_OK;
}


int inPrintPHQRCode(void)
{
	vdDebug_LogPrintf("--inPrintWechatQRCode--");
	BOOL fJavaCall = TRUE;

	BYTE key;
	USHORT res;
	USHORT usRet = d_OK;
	//CTOS_QRCODE_INFO qrcodeInfo;	
	CTOS_QRCODE_INFO_S1 qrcode_info;
	USHORT usXstart = 0;

	
	if( printCheckPaper()==-1)
        return -1;


	vdCTOSS_PrinterStart(100);
	CTOS_PrinterSetHeatLevel(4);
	

	
	memset(srTransRec.szXMLcode_url,0x00, sizeof(srTransRec.szXMLcode_url));

	strcpy(srTransRec.szXMLcode_url,"00020101021228560011ph.ppmi.p2m0111MBTCPHMMXXX03158000000000000440503310520460125303608540518.005802PH5913P2M UPLOAD MB6006Makati62610012ph.ppmi.qrph03158000000000000440510000000069007088000004488710012ph.ppmi.qrph0151800000000000044~800000000000044~80000044~000000069063041B55");

	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);


	vdDebug_LogPrintf("--testlang10--");
	vdDebug_LogPrintf("srTransRec.szXMLcode_url=[%s]", srTransRec.szXMLcode_url);

    memset(&qrcode_info, 0x0, sizeof(qrcode_info));
    //qrcode_info.InfoVersion = QR_INFO_VERSION;
	//qrcode_info.Size = 5;
    qrcode_info.Version = QR_VERSION21X21;
    qrcode_info.Level = QR_ECLEVEL_L;

	vdCTOS_PrinterFline(1);
	

	if (srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX)
	{
		qrcode_info.Level = QR_ECLEVEL_Q;
		usXstart = 0;
	}		
	else
	{
		qrcode_info.Level = QR_ECLEVEL_L;
		usXstart = 40;
	}		
	
	usRet = CTOS_QRCodePrinter(&qrcode_info, srTransRec.szXMLcode_url, usXstart);
	
	vdDebug_LogPrintf("CTOS_QRCodePrinter, usRet=[%d]", usRet);
		


	
	

	//iXMLPrintWechatQRCode();

	
	vdCTOS_PrinterFline(1);

	vdCTOSS_PrinterEnd();

	//CTOS_LCDTClearDisplay();
	
	return d_OK;
}

int inCurl_CommsInit(void)
{

	int inRetVal; 

    vdDebug_LogPrintf("inCurl_CommsInit");
	
	//inCPTRead(108);
	//inCPTRead(25);
	
    //strCPT.inCommunicationMode=GPRS;
	
	srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;

    if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
    {
        //vdSetErrorMessage("COMM INIT ERR");
        //vdDisplayErrorMsgResp2(" ", " ", "COMM INIT ERR");
		vdDisplayErrorMsgResp2("","Initialization","Error");
		vdSetErrorMessage("");
		vdDebug_LogPrintf("Initialization error - inCTOS_InitComm");
        return(d_NO);
    }

    inRetVal = inCTOS_CheckInitComm(srTransRec.usTerminalCommunicationMode); 

	if (inRetVal != d_OK)
	{
		if (srTransRec.usTerminalCommunicationMode == GPRS_MODE)
		{
			vdDisplayErrorMsgResp2(" ", "GPRS Problem","Please Try Again");
			vdSetErrorMessage("");
		}
		//wifi-mod2
		else if (srTransRec.usTerminalCommunicationMode == WIFI_MODE)		
		{
			vdDisplayErrorMsgResp2(" ", "WIFI Problem","Please Try Again");
			vdSetErrorMessage("");
		}
		//wifi-mod2
		else
		{
			//vdDisplayErrorMsgResp2(" ", " ", "COMM INIT ERR");
			vdDisplayErrorMsgResp2("","Initialization","Error");
			
			vdDebug_LogPrintf("Initialization error - inCTOS_CheckInitComm");
			vdSetErrorMessage("");
		}
	
		return(d_NO);
	}

	srCommFuncPoint.inConnect(&srTransRec);

	return d_OK;
}

int inPHQR_Login(void)
{
    int inRet;
	int ininit = get_env_int("PHQRINIT");
	
	int inExpDate[6+1]; 
	
	int inExpTime[6+1]; 
	BYTE szCurrentDate[20+1];
	BYTE szCurrentTime[20+1];

	CTOS_RTC SetRTC;

    //put_env_int("SSLV23", 1);
	//put_env_int("SSL_CLI_AUTH", 0);

	vdDebug_LogPrintf("testlang12 2");


    vdCTOS_TxnsBeginInit();

	if(get_env_int("ECRPROCESS")==1)
	{
		vdSetECRTransactionFlg(1);
		vdSetECRResponse(ECR_UNKNOWN_ERR);
	}
	else
        vdSetECRTransactionFlg(0);
	

   if(fGetECRTransactionFlg() == TRUE)
   {
        inRet = inCTOS_MultiAPGetData();
        if(d_OK != inRet)
        return inRet;

   }	
    
    //Read the date and the time //
    CTOS_RTCGet(&SetRTC); 

	inCTOSS_GetEnvDB ("PHQREXPDATE", inExpDate);
	inCTOSS_GetEnvDB ("PHQREXPTIME", inExpTime);

	vdDebug_LogPrintf("testlang8 expdate %s - %ld", inExpDate, atol(inExpDate));
	
	vdDebug_LogPrintf("testlang8 inExpTime %s- %ld", inExpTime, atol (inExpTime));

   memset(szCurrentDate, 0x00, sizeof(szCurrentDate));
   sprintf(szCurrentDate,"%02d%02d%02d",SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay );
   sprintf(szCurrentTime,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);

   
   vdDebug_LogPrintf("testlang8 currentdate get_env_int(%d) %s- %ld", get_env_int("ECRPROCESS"),szCurrentDate, atol (szCurrentDate));

   /*if(get_env_int("ECRPROCESS")==1)
		vdSetECRTransactionFlg(1);
	else
				
        vdSetECRTransactionFlg(0);*/
	
   // inSetColorMenuMode();
	vdCTOS_SetTransType(SALE);
		
	inIITRead(PHQR_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");  

	
	srTransRec.IITid = PHQR_ISSUER_INDEX;
	srTransRec.HDTid = PHQR_HOST_INDEX;
	strCDT.HDTid = PHQR_HOST_INDEX;
	inIITRead(srTransRec.IITid);
	inCPTRead(srTransRec.HDTid);
	strcpy(srTransRec.szCardLable, strIIT.szIssuerLabel);

	
	inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;

    inRet = inCTOS_GetTxnPassword();
    if(d_OK != inRet)
        return inRet;


	inRet = inCTOS_CheckIssuerEnable();
    if(d_OK != inRet)
        return inRet;	
		
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
        return inRet;
	
	inDatabase_TerminalCloseDatabase();

    inRet = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inRet)
        return inRet;

	inRet = inCTOS_CheckMustSettle();
    if(d_OK != inRet)
        return inRet;

	inRet = inCTOS_GetInvoice();
	if(d_OK != inRet)
			return inRet;
	
	inWalletTableRead(1, srTransRec.HDTid); 

    if (ininit != 1){
		inRet = inPHQR_Initialize_SendAndReceive();
		
		if(d_OK != inRet)
				return inRet;
    }
	inRet = inPHQR_Login_SendAndReceive();
	
	vdDebug_LogPrintf("inPHQR_Login_SendAndReceive:%d", inRet);
	
	if(d_OK != inRet)
		return inRet;

    vdDisplayMessageBox(1, 8, "LOGIN", "SUCCESSFUL", "", MSG_TYPE_SUCCESS);
    CTOS_Beep();
    CTOS_Delay(2000);
	
}

