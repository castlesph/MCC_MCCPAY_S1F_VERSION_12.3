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
#include "..\DigitalWallet\POSGrab.h"
#include "..\DigitalWallet\POSXMLFunction.h"
#include "../md5/CRCmd5.h"


//extern BOOL fTimeOutFlag;
extern BOOL fECRBuildSendOK;
//extern BOOL fECRTxnFlg;
extern BOOL ErmTrans_Approved;

int inCTOS_GrabQRRequest(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_GrabQRRequest--");

    //inSetColorMenuMode();

	if(get_env_int("ECRPROCESS")==1)
		vdSetECRTransactionFlg(1);
	else	
        vdSetECRTransactionFlg(0);

	//CTOS_LCDTClearDisplay();
	vdCTOS_SetTransType(SALE);
	//inIITRead(GRAB_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
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
	srTransRec.IITid = GRAB_ISSUER_INDEX;
	srTransRec.HDTid = GRAB_HOST_INDEX;
	strCDT.HDTid = GRAB_HOST_INDEX;
	
	inDatabase_TerminalOpenDatabase();
	inIITReadEx(srTransRec.IITid);
	inCPTReadEx(srTransRec.HDTid);
	int inDatabase_TerminalCloseDatabase();

	strcpy(srTransRec.szCardLable, strIIT.szIssuerLabel);

	inRet = inCTOS_GrabQRRequestFlowProcess();

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

int inCTOS_GrabQRRequestFlowProcess(void)
{
    int inRet = d_NO, inRet2=0;
	int inEVENT_ID = 0;
    BOOL fExit=VS_FALSE;
	
	char szTime[6+1];
    CTOS_RTC SetRTC;

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

	vdDebug_LogPrintf("inCTOS_GrabQRRequestFlowProcess");

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
		strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
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
	//    inRet = inMultiAP_ECRSendSuccessResponse();
	//}

	if (isCheckTerminalMP200() == d_OK)
	{
		vdCTOSS_DisplayStatus(d_OK);

		inRet = inCTOSS_ERM_ReceiptRecvVia();
		if(d_OK != inRet)
	    	return inRet;
	}

	vdDebug_LogPrintf("inCTOS_GrabQRRequestFlowProcess inMultiAP_ECRSendSuccessResponse");
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
         //fECRApproved=TRUE;
         inMultiAP_ECRSendSuccessResponse();
    }*/

	vdDebug_LogPrintf("inCTOS_GrabQRRequestFlowProcess END");

    return d_OK;
}

int inCTOS_GrabQRRetrieve(void)
{
	int inRet = d_NO;
	unsigned char bTC2ndbyte = 0,
		bTC3rdbyte = 0;

	vdDebug_LogPrintf("--inCTOS_GrabQRRetrieve--");

    //inSetColorMenuMode();

	//CTOS_LCDTClearDisplay();
	srTransRec.iXMLType = WECHAT_ALIPAY_RETRIEVE_QRCODE;
	vdCTOS_SetTransType(PRE_AUTH);
	//inIITRead(GRAB_ISSUER_INDEX); // Read IIT for trans title header -- sidumili
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");

    vdCTOS_TxnsBeginInit();

	// Static
	srTransRec.IITid = GRAB_ISSUER_INDEX;
	srTransRec.HDTid = GRAB_HOST_INDEX;
	strCDT.HDTid = GRAB_HOST_INDEX;
	
	inDatabase_TerminalOpenDatabase();
	inIITReadEx(srTransRec.IITid);
	inCPTReadEx(srTransRec.HDTid);
	int inDatabase_TerminalCloseDatabase();

	inRet = inCTOS_GrabQRRetrieveFlowProcess();

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

int inCTOS_GrabQRRetrieveFlowProcess(void)
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

	vdDebug_LogPrintf("-->>Grab Search for TransactionID");
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

    return d_OK;
}


