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
#include "..\DigitalWallet\POSWechat.h"
#include "..\DigitalWallet\POSXMLFunction.h"
#include "../md5/CRCmd5.h"


//extern BOOL fTimeOutFlag;
//extern BOOL fECRTxnFlg;
extern BOOL fECRBuildSendOK;
extern BOOL ErmTrans_Approved;
BOOL fECRApproved;

int inCTOS_WechatQRRequest(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_WechatQRRequest--");
	
   // inSetColorMenuMode();

   if(get_env_int("ECRPROCESS")==1)
		vdSetECRTransactionFlg(1);
	else
        vdSetECRTransactionFlg(0);
		
	//CTOS_LCDTClearDisplay();
	vdCTOS_SetTransType(SALE);
	//inIITRead(WECHAT_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");  

    vdCTOS_TxnsBeginInit();

    if(fGetECRTransactionFlg() == TRUE)
    {
       inRet = inCTOS_MultiAPGetData();
       if(d_OK != inRet)
       return inRet;
    
    }    

	// Static	
	srTransRec.IITid = WECHAT_ISSUER_INDEX;
	srTransRec.HDTid = WECHAT_HOST_INDEX;
	strCDT.HDTid = WECHAT_HOST_INDEX;
	
	inDatabase_TerminalOpenDatabase();
	inIITReadEx(srTransRec.IITid);
	inCPTReadEx(srTransRec.HDTid);
	inDatabase_TerminalCloseDatabase();
		
	strcpy(srTransRec.szCardLable, strIIT.szIssuerLabel);

	vdDebug_LogPrintf("ISSUER %d %d %d", strIIT.inIssuerNumber, strIIT.inCheckHost, srTransRec.IITid);
	
	inRet = inCTOS_WechatQRRequestFlowProcess();

	//inMultiAP_ECRSendSuccessResponse();

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

	if (fGetECRTransactionFlg())
    {
         //fECRApproved=TRUE;
         inMultiAP_ECRSendSuccessResponse();
    }
    
	vdCTOS_TransEndReset();
	
	return inRet;
}

int inCTOS_WechatQRRequestFlowProcess(void)
{
    int inRet = d_NO, inRet2=0;
	int inEVENT_ID = 0;
	char szTime[6+1];	
    CTOS_RTC SetRTC;
	BOOL fExit=VS_FALSE;
	
	fECRApproved=FALSE;
	
	//fTimeOutFlag = FALSE; /*BDO: Flag for timeout --sidumili*/
	srTransRec.iXMLType = WECHAT_ALIPAY_REQUEST_QRCODE;	
	vdCTOS_SetTransType(SALE);

#if 0	    
    inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;

    inRet = inCTOS_GetTxnPassword();
    if(d_OK != inRet)
        return inRet;

	//inRet = inCTOS_TEMPCheckAndSelectMutipleMID();
	//if(d_OK != inRet)
	//	return inRet;
#endif

	inRet = inCTOS_CheckIssuerEnable();
    if(d_OK != inRet)
        return inRet;	
		
    inRet = inCTOS_XMLSelectHost();
    if(d_OK != inRet)
        return inRet;

	
#if 0

	if (inMultiAP_CheckSubAPStatus() == d_OK)
	{
		inRet = inCTOS_MultiAPGetData();
		if(d_OK != inRet)
			return inRet;
		
		inRet = inCTOS_MultiAPReloadTable();
		if(d_OK != inRet)
			return inRet;
	}
		inRet = inCTOS_MultiAPCheckAllowd();
		if(d_OK != inRet)
			return inRet;

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

	inRet = inXML_CommsInit();	
	if (inRet != d_OK){	
		//strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
		return inRet;	
	}

	inRet = inCTOS_GetInvoice();
	if(d_OK != inRet)
			return inRet;
	
	inRet = inXMLCreateRequestData();
	if (inRet != d_OK)
		return inRet;	
	
	inRet = inXMSendAndReceiveRequestMessage();
	if (inRet != d_OK){
		inRet = vdXMLProcessReversal();		
		return inRet;
	}	

	if (iGetQRPresentedType() == MPQR_TYPE) 
	{
		inPrintWechatQRCode(); // Print QRCode

		// Save to Retrieve
		iRetrieveSaveData();

		while(1)
		{
			fExit=VS_FALSE;
    		// sidumili: added for display QRCode in S1
    		inRet2 = inUserConfirmQRCode();
    		if (inRet2 != d_OK){
    			//strcpy(srTransRec.szECRRespCode, ECR_OPER_CANCEL_RESP);//for ecr
    			//return inRet;
    			fExit=VS_TRUE;
    		}
    		
    		srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
    		srTransRec.iXMLRetrieveType = WECHAT_AUTO_RETRIEVE;
    		srRetrieveRec.byVoided = FALSE;
    		srTransRec.byVoided = FALSE;
    		
    		vdCTOS_SetTransType(SALE);
    
    		inRet = inXML_CommsInit();	
    		if (inRet != d_OK){
    			//strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
    			return inRet;
    		}
    
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
					if(fExit == VS_FALSE)
						continue;
					
    				vdDisplayMessageStatusBox(1, 8, "SENDING CLOSE INTERFACE", MSG_PLS_WAIT, MSG_TYPE_SEND);
    				//CTOS_Delay(2000);
    				CTOS_Delay(100);
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
						//return d_NO;
					}	
					
    				CTOS_Beep();
    				//CTOS_Delay(1000);	
    				CTOS_Delay(100);
    			}
    			
    			return inRet;
    		}
    
    		iSetDummyFieldForERM();
			break;
		}
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

	//fECRApproved=TRUE;
	vdDebug_LogPrintf("inCTOS_WechatQRRequestFlowProcess inMultiAP_ECRSendSuccessResponse");
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

   /*if (fGetECRTransactionFlg())
    {
         fECRApproved=TRUE;
         inMultiAP_ECRSendSuccessResponse();
    }*/
	
    return d_OK;
}

int inCTOS_WechatQRRetrieve(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_WechatQRRetrieve--");
	
    //inSetColorMenuMode();
		
	//CTOS_LCDTClearDisplay();
	srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
	vdCTOS_SetTransType(PRE_AUTH);
	//inIITRead(WECHAT_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");  

    vdCTOS_TxnsBeginInit();

	// Static	
	srTransRec.IITid = WECHAT_ISSUER_INDEX;
	srTransRec.HDTid = WECHAT_HOST_INDEX;
	strCDT.HDTid = WECHAT_HOST_INDEX;
	
	inDatabase_TerminalOpenDatabase();
	inIITReadEx(srTransRec.IITid);
	inCPTReadEx(srTransRec.HDTid);
	inDatabase_TerminalCloseDatabase();

	
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

	//CTOS_LCDTClearDisplay(); 
	
    return d_OK;
}

int inCTOS_WechatQRDisplayTest(void)
{	
	//CTOS_LCDTClearDisplay();
	vdCTOS_SetTransType(SALE);
	vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");  

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
		
	//CTOS_LCDTClearDisplay();	
	//vdCTOS_DispStatusMessage("PROCESSING");  

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

    inMultiAP_ECRSendSuccessResponse();
	
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

	//if(get_env_int("ECRPROCESS")==1)
    //{
    //inMultiAP_ECRSendSuccessResponse();
    //}
	
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

	//Added to send correct time to ECR
	memcpy(szTime,&srTransRec.szXMLtime_end[8],6);
	wub_str_2_hex(szTime,srTransRec.szTime,6);

	if (isCheckTerminalMP200() == d_OK)
	{
		vdCTOSS_DisplayStatus(d_OK);

		inRet = inCTOSS_ERM_ReceiptRecvVia();
		if(d_OK != inRet)
	    	return inRet;

	}

	inMultiAP_ECRSendSuccessResponse();

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

	//CTOS_LCDTClearDisplay(); 

	fECRApproved=TRUE;
    //inMultiAP_ECRSendSuccessResponse();
	
    return d_OK;
}

