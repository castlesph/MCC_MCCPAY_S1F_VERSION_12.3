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
#include "..\DigitalWallet\POSAlipay.h"
#include "..\DigitalWallet\POSXMLFunction.h"
#include "../md5/CRCmd5.h"
#include "..\Aptrans\MultiShareCOM.h"

//extern BOOL fTimeOutFlag;
extern BOOL fGPRSConnectOK;

int inXMLCreateHTTPpostHeader(char *pszHeader, int inContentLength)
{
	char szLength[5+1];

	vdDebug_LogPrintf("--inXMLCreateHTTPpostHeader--");

	memset(szLength, 0x00, sizeof(szLength));
	sprintf(szLength, "%d", inContentLength);
	memset(pszHeader,0,sizeof(pszHeader));

	strcpy(pszHeader, "POST ");
	strcat(pszHeader, strDigitalWallet.szurl);
	strcat(pszHeader, szPostLine2);
	strcat(pszHeader, "\r\n");
	strcat(pszHeader, "Host: ");
	strcat(pszHeader, strDigitalWallet.szhost);
	strcat(pszHeader, "\r\n");
	strcat(pszHeader,"Content-Type: ");
	strcat(pszHeader, szContentLine);
	strcat(pszHeader, "\r\n");
	strcat(pszHeader,"Cache-Control: ");
	strcat(pszHeader, szCacheControl);
	strcat(pszHeader, "\r\n");
	strcat(pszHeader,"Connection: ");
	strcat(pszHeader, szConnectionMsg);
	strcat(pszHeader, "\r\n");
	strcat(pszHeader, "Content-Length: ");
	strcat(pszHeader, szLength);
	strcat(pszHeader, "\r\n\r\n");

	//URLencode(pszHeader);

	//vdDebug_LogPrintf("Len[%d].pszHeader[%s]", strlen(pszHeader), pszHeader);

}

int inXMLCreateRequestData(void)
{
	int inSendSize;
	char szBody[XML_DATA_SIZE + 1];
	char szKVPair[MAX_KV_KEY_SIZE+MAX_KV_VALUE_SIZE+1];
	int i = 0;
	int inContentLength = 0;
	char szContent[XML_DATA_SIZE + 1];
	BOOL fCData = FALSE;
	char szDate[8+1] = {0};
	char szTime[6+1] = {0};
	char szDateTime[14+1] = {0};
	BYTE szNonce_str[32 + 1] = {0};
	BYTE szOut_trade_no[32 + 1] = {0};
	BYTE szService[32 + 1];

	vdDebug_LogPrintf("--inXMLCreateRequestData--");
	vdDebug_LogPrintf("byTransType[%d]", srTransRec.byTransType);
	vdDebug_LogPrintf("iXMLType[%d]", srTransRec.iXMLType);
	vdDebug_LogPrintf("iXMLRetrieveType[%d]", srTransRec.iXMLRetrieveType);
	vdDebug_LogPrintf("HDTid[%d]", srTransRec.HDTid);
	vdDebug_LogPrintf("fGBLvRetrieveList[%d]", strGBLVar.fGBLvRetrieveList);
	vdDebug_LogPrintf("szGBLvSelectedValue[%s]", strGBLVar.szGBLvSelectedValue);

	vdDebug_LogPrintf("saturn srTransRec.byTransType = %d %d", srTransRec.byTransType, srTransRec.byVoided);

	vdDebug_LogPrintf("saturn srRetrieveRec.byvoided = %c %d", srRetrieveRec.byVoided, srRetrieveRec.byVoided);
	vdDebug_LogPrintf("srTransRec.inQRPresented=[%d]", srTransRec.inQRPresented);
	vdDebug_LogPrintf("srRetrieveRec.byVoided=[%d]", srRetrieveRec.byVoided);
	vdDebug_LogPrintf("srTransRec.byPackType=[%d]", srTransRec.byPackType);

	//inTCTReadEx(1);

	// move reading to one function -- sidumili
	/*
	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		inMCCWechatRead(1);

	if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		inMCCAlipayRead(1);
	*/

	inWalletTableRead(1, srTransRec.HDTid); // function to call MCCPAY database per table -- sidumili
	
	iXMLGetDateTime(szDate, szTime);
	vXMLGenerateNonce_Str(szNonce_str);
	vXMLGetOut_trade_no(szOut_trade_no);
	//vXMLGetService(szService);

	memset(szService, 0x00, sizeof(szService));


	if (iGetQRPresentedType() == CPQR_TYPE)
	{
		if(srTransRec.iXMLType == WECHAT_ALIPAY_VOID)
			strcpy(szService, "unified.trade.refund");
		else if (srTransRec.iXMLType == WECHAT_ALIPAY_RETRIEVE_QRCODE)
		{
			if (srTransRec.byVoided)			
				strcpy(szService, "unified.trade.refundquery");			
		    else		    
				strcpy(szService, "unified.trade.query");		    				
	    }
		else
		{
			strcpy(szService, "unified.trade.micropay");
		}			
	}
	else
	{
		if (srTransRec.iXMLType == WECHAT_ALIPAY_RETRIEVE_QRCODE)
		{
			if (srTransRec.byVoided)
				strcpy(szService, "unified.trade.refundquery");
			else	
				strcpy(szService, "unified.trade.query");
		}			
		else if(srTransRec.iXMLType == WECHAT_ALIPAY_REQUEST_QRCODE)
			strcpy(szService, strDigitalWallet.szservice);
		else if(srTransRec.iXMLType == WECHAT_ALIPAY_CLOSE_QRCODE)
			strcpy(szService, "unified.trade.close");		
		else // WECHAT_ALIPAY_VOID
			strcpy(szService, "unified.trade.refund");
	}

	// Reversal
	if (iGetQRPresentedType() == CPQR_TYPE && srTransRec.iXMLType == WECHAT_ALIPAY_REVERSAL && srTransRec.byPackType == REVERSAL)
	{
		strcpy(szService, "unified.micropay.reverse");
	}
	
	strcpy(strDigitalWallet.sztotal_fee, srTransRec.szXMLtotalfee); // Amount
	sprintf(szDateTime, "%s%s", szDate, szTime);
	strcpy(strDigitalWallet.sztime_expire, szDateTime);            // Expire
	strcpy(strDigitalWallet.sztime_start, szDateTime);		     // Start
	strcpy(strDigitalWallet.sznonce_str, (char*)szNonce_str);
	strcpy(strDigitalWallet.szservice, (char*)szService);

	if(srTransRec.iXMLType == WECHAT_ALIPAY_VOID)
	{
		strcpy(strDigitalWallet.szout_refund_no, (char*)szOut_trade_no);
		strcpy(strDigitalWallet.sztransaction_id,srTransRec.szXMLtransaction_id);
		wub_hex_2_str(srTransRec.szBaseAmount, srTransRec.szXMLtotalfee, AMT_BCD_SIZE);
	}
	else if (srTransRec.iXMLType == WECHAT_ALIPAY_CLOSE_QRCODE)
	{
		strcpy(strDigitalWallet.szout_trade_no, (char*)szOut_trade_no);
		wub_hex_2_str(srTransRec.szBaseAmount, srTransRec.szXMLtotalfee, AMT_BCD_SIZE);
	}
	else
	{
		strcpy(strDigitalWallet.szout_trade_no, (char*)szOut_trade_no);
	}

	// Temp
	//strcpy(strDigitalWallet.szmerchantsignkey, "985b9957036bc5c26f270db082d12e15"); // for mid 150590000001
	//strcpy(strDigitalWallet.szmerchantsignkey, "5d79f1aa4b75e5270bd2df8b9306d8c1"); // for mid 150580000007
	
	if (iGetQRPresentedType() == CPQR_TYPE)
	{
		strcpy(strDigitalWallet.szauth_code, strGBLVar.szGBLvAuthCode);
		strcpy(strDigitalWallet.szop_user_id, srTransRec.szMID);
		strcpy(strDigitalWallet.szbody, strDigitalWallet.szqrpresentedbody);		
	}

	if (iGetQRPresentedType() == MPQR_TYPE)
	{
		strcpy(strDigitalWallet.szop_user_id, srTransRec.szMID);
		strcpy(strDigitalWallet.szbody, strDigitalWallet.szqrpresentedbody);
	}
			
	vdDebug_LogPrintf("***DIGITAL WALLET CONFIG---------------------------------------------------------------------");
	vdDebug_LogPrintf("strDigitalWallet.BWTid[%d]", strDigitalWallet.BWTid);
	vdDebug_LogPrintf("strDigitalWallet.szattach[%s]", strDigitalWallet.szattach);
	vdDebug_LogPrintf("strDigitalWallet.szbody[%s]", strDigitalWallet.szbody);
	vdDebug_LogPrintf("strDigitalWallet.szmch_create_ip[%s]", strDigitalWallet.szmch_create_ip);
	vdDebug_LogPrintf("strDigitalWallet.sznonce_str[%s]", strDigitalWallet.sznonce_str);
	vdDebug_LogPrintf("strDigitalWallet.sznotify_url[%s]", strDigitalWallet.sznotify_url);
	vdDebug_LogPrintf("strDigitalWallet.szout_trade_no[%s]", strDigitalWallet.szout_trade_no);
	vdDebug_LogPrintf("strDigitalWallet.szservice[%s]", strDigitalWallet.szservice);
	vdDebug_LogPrintf("strDigitalWallet.szsign[%s]", strDigitalWallet.szsign);
	vdDebug_LogPrintf("strDigitalWallet.sztime_expire[%s]", strDigitalWallet.sztime_expire);
	vdDebug_LogPrintf("strDigitalWallet.sztime_start[%s]", strDigitalWallet.sztime_start);
	vdDebug_LogPrintf("strDigitalWallet.sztotal_fee[%s]", strDigitalWallet.sztotal_fee);
	vdDebug_LogPrintf("strDigitalWallet.szmerchantsignkey[%s]", strDigitalWallet.szmerchantsignkey);
	vdDebug_LogPrintf("strDigitalWallet.fSignatureLine[%d]", strDigitalWallet.fSignatureLine);
	vdDebug_LogPrintf("strDigitalWallet.szversion[%s]", strDigitalWallet.szversion);
	vdDebug_LogPrintf("strDigitalWallet.szcharset[%s]", strDigitalWallet.szcharset);
	vdDebug_LogPrintf("strDigitalWallet.szsign_type[%s]", strDigitalWallet.szsign_type);
	vdDebug_LogPrintf("strDigitalWallet.szurl[%s]", strDigitalWallet.szurl);
	vdDebug_LogPrintf("strDigitalWallet.szhost[%s]", strDigitalWallet.szhost);
	vdDebug_LogPrintf("strDigitalWallet.szop_user_id[%s]", strDigitalWallet.szop_user_id);
	vdDebug_LogPrintf("strDigitalWallet.szlimit_credit_pay[%s]", strDigitalWallet.szlimit_credit_pay);
	vdDebug_LogPrintf("strDigitalWallet.sztransaction_id[%s]", strDigitalWallet.sztransaction_id);
	vdDebug_LogPrintf("strTCT.szInvoiceNo[%s]", strTCT.szInvoiceNo);
	vdDebug_LogPrintf("srTransRec.szHostLabel[%s]", srTransRec.szHostLabel);
	vdDebug_LogPrintf("srTransRec.szMID[%s]", srTransRec.szMID);
	vdDebug_LogPrintf("srTransRec.szTID[%s]", srTransRec.szTID);
	vdDebug_LogPrintf("strDigitalWallet.szdevice_info[%s]", strDigitalWallet.szdevice_info);
	vdDebug_LogPrintf("strDigitalWallet.szwitholding_tax[%s]", strDigitalWallet.szwitholding_tax);
	vdDebug_LogPrintf("strDigitalWallet.szqrpresentedbody[%s]", strDigitalWallet.szqrpresentedbody);
	vdDebug_LogPrintf("strDigitalWallet.szout_refund_no[%s]", strDigitalWallet.szout_refund_no);
	vdDebug_LogPrintf("***DIGITAL WALLET CONFIG---------------------------------------------------------------------");

	// Get tag <Sign> using Mechant Signature Key
	iGetSignatureKey(srTransRec.iXMLType);

	memset(&srRequest,0x00,sizeof(HTTP_REQUEST));
	memset(szBody,0x00, sizeof (szBody));

	inInitKVPairCol(&srRequest.srKVPairCol);

	if (srTransRec.iXMLType == WECHAT_ALIPAY_RETRIEVE_QRCODE)
	{
		// --------------------------------------------------------------
		// MPQR
		// --------------------------------------------------------------
		if (iGetQRPresentedType() == MPQR_TYPE)
		{
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_CHARSET, strDigitalWallet.szcharset);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_ID, srTransRec.szMID);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_NONCE_STR, strDigitalWallet.sznonce_str);

			if (srTransRec.iXMLRetrieveType == WECHAT_AUTO_RETRIEVE)
			{
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
			}

			if (srTransRec.iXMLRetrieveType == WECHAT_ALIPAY_MANUAL_RETRIEVE)
			{
				if (srTransRec.byVoided)			
					inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OP_USER_ID, strDigitalWallet.szop_user_id);
				
				if (strGBLVar.fGBLvRetrieveList && strlen(strGBLVar.szGBLvSelectedValue) > 0)
				{
					if (srTransRec.byVoided)
					{
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_REFUND_NO, strDigitalWallet.szout_refund_no);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_REFUND_FEE, strDigitalWallet.sztotal_fee);	
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);				
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TOTAL_FEE, strDigitalWallet.sztotal_fee);
					}
					else
					{
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
					}
					
				}
				else
				{
					inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TRANSACTION_ID, strDigitalWallet.sztransaction_id);
				}
				
			}

			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_VERSION, strDigitalWallet.szversion);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN, strDigitalWallet.szsign);
		
		}
		
		// --------------------------------------------------------------
		// CPQR
		// --------------------------------------------------------------
		if (iGetQRPresentedType() == CPQR_TYPE)
		{	
			if (srTransRec.iXMLRetrieveType == WECHAT_AUTO_RETRIEVE)
			{
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
			}

			if (srTransRec.iXMLRetrieveType == WECHAT_ALIPAY_MANUAL_RETRIEVE)
			{
				
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_CHARSET, strDigitalWallet.szcharset);
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_ID, srTransRec.szMID);
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_NONCE_STR, strDigitalWallet.sznonce_str);

				if (srTransRec.byVoided)			
					inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OP_USER_ID, strDigitalWallet.szop_user_id);
			
				if (strGBLVar.fGBLvRetrieveList && strlen(strGBLVar.szGBLvSelectedValue) > 0)
				{
					if (srTransRec.byVoided)
					{
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_REFUND_NO, strDigitalWallet.szout_refund_no);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_REFUND_FEE, strDigitalWallet.sztotal_fee);	
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);				
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TOTAL_FEE, strDigitalWallet.sztotal_fee);
						
					}
					else
					{
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);				
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
						inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
					}					
				}
				else
				{
					inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TRANSACTION_ID, strDigitalWallet.sztransaction_id);
				}
				
			}
			
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_VERSION, strDigitalWallet.szversion);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN, strDigitalWallet.szsign);
		}
		
	}
	else if(srTransRec.iXMLType == WECHAT_ALIPAY_REQUEST_QRCODE)
	{
		if (iGetQRPresentedType() == CPQR_TYPE)
		{
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_AUTH_CODE, strDigitalWallet.szauth_code);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_BODY, strDigitalWallet.szbody);

			if (srTransRec.HDTid != GCASH_HOST_INDEX)
			{
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_DEVICE_INFO, strDigitalWallet.szdevice_info);
			}
			
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_CREATE_IP, strDigitalWallet.szmch_create_ip);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_ID, srTransRec.szMID);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_NONCE_STR, strDigitalWallet.sznonce_str);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OP_USER_ID, strDigitalWallet.szop_user_id);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN, strDigitalWallet.szsign);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TOTAL_FEE, strDigitalWallet.sztotal_fee);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_VERSION, strDigitalWallet.szversion);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_WTIHHOLDING_TAX, strDigitalWallet.szwitholding_tax);
			
		}
		else
		{
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_ATTACH, strDigitalWallet.szattach);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_BODY, strDigitalWallet.szbody);

			if (srTransRec.HDTid != GCASH_HOST_INDEX)
			{
				inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_DEVICE_INFO, strDigitalWallet.szdevice_info);
			}
						
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_CREATE_IP, strDigitalWallet.szmch_create_ip);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_ID, srTransRec.szMID);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_NONCE_STR, strDigitalWallet.sznonce_str);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_NOTIFY_URL, strDigitalWallet.sznotify_url);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN, strDigitalWallet.szsign);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TIME_START, strDigitalWallet.sztime_start);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TOTAL_FEE, strDigitalWallet.sztotal_fee);
		}		
	}
	else if(srTransRec.iXMLType == WECHAT_ALIPAY_CLOSE_QRCODE || 
		    srTransRec.iXMLType == WECHAT_ALIPAY_REVERSAL) // For Reversal
	{
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_CHARSET, strDigitalWallet.szcharset);
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_ID, srTransRec.szMID);
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_NONCE_STR, strDigitalWallet.sznonce_str);
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_VERSION, strDigitalWallet.szversion);
		inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN, strDigitalWallet.szsign);
		
	}
	else //WECHAT_ALIPAY_VOID
	{

			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_CHARSET, strDigitalWallet.szcharset);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_MCH_ID, srTransRec.szMID);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_NONCE_STR, strDigitalWallet.sznonce_str);

		//inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OP_USER_ID, srTransRec.szMID);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_OUT_REFUND_NO, strDigitalWallet.szout_refund_no);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_REFUND_FEE, srTransRec.szXMLtotalfee);//set Total fee as refund amount.
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SERVICE, strDigitalWallet.szservice);
			 
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TOTAL_FEE, srTransRec.szXMLtotalfee);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_TRANSACTION_ID, strDigitalWallet.sztransaction_id);
			
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_VERSION, strDigitalWallet.szversion);
			inAddKeyValuePairToCol(&(srRequest.srKVPairCol), TAG_SIGN, strDigitalWallet.szsign);
		//}
	}

	//vdDebug_LogPrintf("START LOOP PAIR -->>");

	memset(szKVPair,0, sizeof(szKVPair));

	for (i = 0; i < srRequest.srKVPairCol.iCount; i++)
	{
		fCData = fCDataSection(srRequest.srKVPairCol.kvArray[i].szKey);

		//vdDebug_LogPrintf("srRequest.srKVPairCol.kvArray[%d].szKey[%s].szValue[%s]", i, srRequest.srKVPairCol.kvArray[i].szKey, srRequest.srKVPairCol.kvArray[i].szValue);

		memset(szKVPair, 0x00, sizeof(szKVPair));

		if (fCData)
		{
			sprintf(szKVPair,"<%s><![CDATA[%s]]></%s>",srRequest.srKVPairCol.kvArray[i].szKey, srRequest.srKVPairCol.kvArray[i].szValue, srRequest.srKVPairCol.kvArray[i].szKey);

		}
		else
		{
			sprintf(szKVPair,"<%s>%s</%s>",srRequest.srKVPairCol.kvArray[i].szKey, srRequest.srKVPairCol.kvArray[i].szValue, srRequest.srKVPairCol.kvArray[i].szKey);
		}

		//vdDebug_LogPrintf("[%d of %d]..szKVPair->>Len[%d].[%s].fCData[%d]", i, srRequest.srKVPairCol.iCount, strlen(szKVPair), szKVPair, fCData);

		strcat(szBody, szKVPair);
		strcat(szBody, "\r\n");

		//vdDebug_LogPrintf("Body Len[%d]", strlen(szBody));

	}
	//vdDebug_LogPrintf("END LOOP PAIR -->>");

	// Body
	szBody[strlen(szBody)] = '\0';

	memset(szContent, 0x00, sizeof(szContent));
	strcpy(szContent, "<xml>\r\n");
	strcat(szContent, szBody);
	strcat(szContent, "</xml>\r\n");

	inContentLength = strlen(szContent);

	inXMLCreateHTTPpostHeader(srRequest.szHeader, inContentLength);

	strcpy(srTransRec.szXMLSendData, srRequest.szHeader);
	strcat(srTransRec.szXMLSendData, szContent);
	srTransRec.iXMLSendLen = inContentLength + strlen(srRequest.szHeader);

	return d_OK;
}

int inInitKVPairCol(KEY_VALUE_PAIR_COL *pKeyValPair)
{
	//vdDebug_LogPrintf("--inInitKVPairCol--");

	pKeyValPair->iCount = 0;
	memset(pKeyValPair->kvArray,0, sizeof(KEY_VALUE_PAIR)*MAX_KV_PAIRS);

	return d_OK;
}

int inAddKeyValuePairToCol(KEY_VALUE_PAIR_COL *pKeyValPair, char *pszKey, char *pszValue)
{
	KEY_VALUE_PAIR srKVPair;

	//vdDebug_LogPrintf("--inAddKeyValuePairToCol--");

	strcpy(srKVPair.szKey, pszKey);
	strcpy(srKVPair.szValue, pszValue);

	//vdDebug_LogPrintf("inAddKeyValuePairToCol::Len[%d].srKVPair.szKey[%s], Len[%d].srKVPair.szValue[%s]", strlen(srKVPair.szKey), srKVPair.szKey, strlen(srKVPair.szValue), srKVPair.szValue);

	pKeyValPair->kvArray[pKeyValPair->iCount] = srKVPair;
	pKeyValPair->iCount++;

	return d_OK;
}

int URLencode(char *szBuffer)
{
	char szTempBuff[XML_DATA_SIZE + 1];
	char szTempBuff1[XML_DATA_SIZE + 1];
	char temp[10];
	int i;

	vdDebug_LogPrintf("--URLencode--");

	memset(szTempBuff,'\0', sizeof(szTempBuff));
	memset(szTempBuff1,'\0', sizeof(szTempBuff1));

	memset(temp,'\0', sizeof(temp));

	strcpy(szTempBuff, szBuffer);


	for (i=0; i< strlen(szTempBuff);i++)
	{
		if  ((szTempBuff[i] == '<') || (szTempBuff[i] == '/') || (szTempBuff[i] == '>')  || (szTempBuff[i] == ':') || (szTempBuff[i] == '!')  ||
			 (szTempBuff[i] == '[')  || (szTempBuff[i] == ']') || (szTempBuff[i] == '.') || (szTempBuff[i] == '\r') || (szTempBuff[i] == '\n') ||
			 (szTempBuff[i] == '&') || (szTempBuff[i] == '?') || (szTempBuff[i] == '='))
		{
			sprintf(temp,"%%%02x", szTempBuff[i]);
			strupr(temp);
			strcat(szTempBuff1, temp);
		}else
		{
			szTempBuff1[strlen(szTempBuff1)] = szTempBuff[i];
		}
	}

	memset(szBuffer,'\0', sizeof(szBuffer));

	strcpy(szBuffer, szTempBuff1);

	//vdDebug_LogPrintf("URLCodeData[%s]", szBuffer);

	return d_OK;

}

char* strlwr(char* s)
{
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}

char* strupr(char* s)
{
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
}

int iXMLAnalyzeResponse(char* szXMLData)
{
	char szStatus[256 + 1] = {0};
	char szErroMsg[256 + 1] = {0};
	int iWidth = 16;

	vdDebug_LogPrintf("--iXMLAnalyzeResponse--");

	inInitKVPairCol(&srResponse.srKVPairCol);

	//vdDebug_LogPrintf("Len[%d], uszReceiveData[%s]", strlen(srTransRec.szXMLReceiveData), srTransRec.szXMLReceiveData);

	/*
	// Check Response Status
	if (iXMLCheckResponseStatus(szXMLData) != d_OK)
	return d_NO;

	// Check Result Code
	if (iXMLCheckResponseResultCode(szXMLData) != d_OK)
	return d_NO;

	// Check Trade State
	if (iXMLCheckTradeState(szXMLData) != d_OK)
	return d_NO;
	*/

	// Save To Collection
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_BANK_TYPE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_CASH_FEE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_CASH_FEE_TYPE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_FEE_TYPE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_LOCAL_FEE_TYPE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_ORDER_FEE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_OUT_TRADE_NO);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_OUT_TRANSACTION_ID);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_PLATFORM_RATE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_TIME_START);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_TIME_END);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_TOTAL_FEE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_LOCAL_TOTAL_FEE);	
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_TRANSACTION_ID);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_CODE_URL);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_REFUND_ID);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_TRADE_STATE);

	// For testing if can get data
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_CHARSET);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_CODE_IMG_URL);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_CODE_URL);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_MCH_ID);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_NONCE_STR);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_RESULT_CODE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_SIGN);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_SIGN_TYPE);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_STATUS);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_UUID);

	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_OUT_REFUND_NO);
	vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, szXMLData, TAG_REFUND_STATUS);
	
	
	// Debug
	vdDebug_LogPrintf("szMID[%s]", srTransRec.szMID);
	vdDebug_LogPrintf("szTID[%s]", srTransRec.szTID);
	vdDebug_LogPrintf("HDTid[%d]", srTransRec.HDTid);
	vdDebug_LogPrintf("MITid[%d]", srTransRec.MITid);
	vdDebug_LogPrintf("CDTid[%d]", srTransRec.CDTid);
	vdDebug_LogPrintf("IITid[%d]", srTransRec.IITid);
	vdDebug_LogPrintf("szHostLabel[%s]", srTransRec.szHostLabel);
	vdDebug_LogPrintf("szBatchNo[%02X%02X%02X]", srTransRec.szBatchNo[0], srTransRec.szBatchNo[1], srTransRec.szBatchNo[2]);
	vdDebug_LogPrintf("byTransType[%d]", srTransRec.byTransType);
	vdDebug_LogPrintf("szTotalAmount[%02X%02X%02X%02X%02X%02X]", srTransRec.szTotalAmount[0], srTransRec.szTotalAmount[1], srTransRec.szTotalAmount[2], srTransRec.szTotalAmount[3], srTransRec.szTotalAmount[4], srTransRec.szTotalAmount[5]);
	vdDebug_LogPrintf("szBaseAmount[%02X%02X%02X%02X%02X%02X]", srTransRec.szBaseAmount[0], srTransRec.szBaseAmount[1], srTransRec.szBaseAmount[2], srTransRec.szBaseAmount[3], srTransRec.szBaseAmount[4], srTransRec.szBaseAmount[5]);
	vdDebug_LogPrintf("szInvoiceNo[%02X%02X%02X]", srTransRec.szInvoiceNo[0], srTransRec.szInvoiceNo[1], srTransRec.szInvoiceNo[2]);

	vdDebug_LogPrintf("Len[%d].szXMLbank_type[%s]", strlen(srTransRec.szXMLbank_type), srTransRec.szXMLbank_type);
	vdDebug_LogPrintf("Len[%d].szXMLcash_fee[%s]", strlen(srTransRec.szXMLcash_fee), srTransRec.szXMLcash_fee);
	vdDebug_LogPrintf("Len[%d].szXMLcash_fee_type[%s]", strlen(srTransRec.szXMLcash_fee_type), srTransRec.szXMLcash_fee_type);
	vdDebug_LogPrintf("Len[%d].szXMLfee_type[%s]", strlen(srTransRec.szXMLfee_type), srTransRec.szXMLfee_type);
	vdDebug_LogPrintf("Len[%d].szXMLlocal_fee_type[%s]", strlen(srTransRec.szXMLlocal_fee_type), srTransRec.szXMLlocal_fee_type);
	vdDebug_LogPrintf("Len[%d].szXMLorder_fee[%s]", strlen(srTransRec.szXMLorder_fee), srTransRec.szXMLorder_fee);
	vdDebug_LogPrintf("Len[%d].szXMLout_trade_no[%s]", strlen(srTransRec.szXMLout_trade_no), srTransRec.szXMLout_trade_no);
	vdDebug_LogPrintf("Len[%d].szXMLout_transaction_id[%s]", strlen(srTransRec.szXMLout_transaction_id), srTransRec.szXMLout_transaction_id);
	vdDebug_LogPrintf("Len[%d].szXMLplatform_rate[%s]", strlen(srTransRec.szXMLplatform_rate), srTransRec.szXMLplatform_rate);
	vdDebug_LogPrintf("Len[%d].szXMLtime_start[%s]", strlen(srTransRec.szXMLtime_start), srTransRec.szXMLtime_start);
	vdDebug_LogPrintf("Len[%d].szXMLtime_end[%s]", strlen(srTransRec.szXMLtime_end), srTransRec.szXMLtime_end);
	vdDebug_LogPrintf("Len[%d].szXMLtotal_fee[%s]", strlen(srTransRec.szXMLtotal_fee), srTransRec.szXMLtotal_fee);
	vdDebug_LogPrintf("Len[%d].szXMLtotalfee[%s]", strlen(srTransRec.szXMLtotalfee), srTransRec.szXMLtotalfee);
	vdDebug_LogPrintf("Len[%d].szXMLlocal_total_fee[%s]", strlen(srTransRec.szXMLlocal_total_fee), srTransRec.szXMLlocal_total_fee);
	vdDebug_LogPrintf("Len[%d].szXMLtransaction_id[%s]", strlen(srTransRec.szXMLtransaction_id), srTransRec.szXMLtransaction_id);
	vdDebug_LogPrintf("Len[%d].szXMLcode_url[%s]", strlen(srTransRec.szXMLcode_url), srTransRec.szXMLcode_url);
	vdDebug_LogPrintf("Len[%d].szRefundID[%s]", strlen(srTransRec.szRefundID), srTransRec.szRefundID);
	vdDebug_LogPrintf("Len[%d].szXMLtrade_state[%s]", strlen(srTransRec.szXMLtrade_state), srTransRec.szXMLtrade_state);
	vdDebug_LogPrintf("Len[%d].szXMLout_refund_no[%s]", strlen(srTransRec.szXMLout_refund_no), srTransRec.szXMLout_refund_no);
	vdDebug_LogPrintf("Len[%d].szXMLrefund_status[%s]", strlen(srTransRec.szXMLrefund_status), srTransRec.szXMLrefund_status);

	return d_OK;
}

void vXMLParseAndStoreToCollection(HTTP_REQUEST *srResponse, int iParseType, char* szResponseData, char* szXMLTag)
{
	char szXMLValue[XML_DATA_SIZE + 1] = {0};
	BOOL fTagExist = FALSE;

	//vdDebug_LogPrintf("--vXMLParseAndStoreToCollection--");

	fTagExist = fXMLCheckTagExist((char*)szResponseData, (char*)szXMLTag);

	if (fTagExist)
	{
		memset(szXMLValue, 0x00, sizeof(szXMLValue));
		switch (iParseType)
		{
			case XML_PARSE_STRING:
				strcpy(szXMLValue, getXMLCdataVal((char*)szResponseData, szXMLTag));
				break;
			case XML_PARSE_NUMERIC:
				strcpy(szXMLValue, getXMLVal((char*)szResponseData, szXMLTag));
				break;
			default:
				break;
		}

		//vdDebug_LogPrintf("szXMLTag[%s].szXMLValue[%s]", szXMLTag, szXMLValue);

		if (strlen(szXMLValue) > 0)
		{
			//inAddKeyValuePairToCol(&(srResponse->srKVPairCol), szXMLTag, szXMLValue);

			// Set Value By Tag
			iXMLSetValueByTag(szXMLTag, szXMLValue);
		}
	}
	else
	{
		// Do nothing...
		//vdDebug_LogPrintf("XML Tag[%s] does not exist.!!!", szXMLTag);
	}

}

void iGetSignatureKey(int iMessageType)
{
	BYTE uszMD5Data[64 + 1] = {0};
	BYTE szData[XML_SEND_SIZE + 1] = {0};
	BYTE szDummy[XML_SEND_SIZE + 1] = {0};
	BYTE szTemp[64 + 1] = {0};
	int i = 0;

	vdDebug_LogPrintf("--iGetSignatureKey--");
	vdDebug_LogPrintf("iMessageType[%d]", iMessageType);
	vdDebug_LogPrintf("iXMLRetrieveType[%d]", srTransRec.iXMLRetrieveType);
	vdDebug_LogPrintf("Merchant Signature Key[%s]", strDigitalWallet.szmerchantsignkey);
	vdDebug_LogPrintf("fGBLvRetrieveList[%d]", strGBLVar.fGBLvRetrieveList);
	vdDebug_LogPrintf("szGBLvSelectedValue[%s]", strGBLVar.szGBLvSelectedValue);
	vdDebug_LogPrintf("Sign Type[%s]", strDigitalWallet.szsign_type);
	vdDebug_LogPrintf("srRetrieveRec.byVoided=[%d]", srRetrieveRec.byVoided);
	vdDebug_LogPrintf("srTransRec.inQRPresented=[%d]", srTransRec.inQRPresented);
	vdDebug_LogPrintf("szout_trade_no[%s]", strDigitalWallet.szout_trade_no);	
	vdDebug_LogPrintf("srTransRec.HDTid[%d]", srTransRec.HDTid);
	
	memset(uszMD5Data, 0x00, sizeof(uszMD5Data));
	memset(szData, 0x00, sizeof(szData));

	switch (iMessageType)
	{
	 	case WECHAT_ALIPAY_REQUEST_QRCODE:

			if (iGetQRPresentedType() == CPQR_TYPE)
			{
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_AUTH_CODE, strDigitalWallet.szauth_code);
				strcpy(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_BODY, strDigitalWallet.szbody);
				strcat(szData, szDummy);

				if (srTransRec.HDTid != GCASH_HOST_INDEX)
				{
					memset(szDummy, 0x00, sizeof(szDummy));
					sprintf(szDummy, "%s=%s&", TAG_DEVICE_INFO, strDigitalWallet.szdevice_info);
					strcat(szData, szDummy);
				}
				
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_MCH_CREATE_IP, strDigitalWallet.szmch_create_ip);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_MCH_ID, srTransRec.szMID);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_NONCE_STR, strDigitalWallet.sznonce_str);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_OP_USER_ID, strDigitalWallet.szop_user_id);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SERVICE, strDigitalWallet.szservice);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_TOTAL_FEE, strDigitalWallet.sztotal_fee);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_VERSION, strDigitalWallet.szversion);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_WTIHHOLDING_TAX, strDigitalWallet.szwitholding_tax);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s", TAG_KEY, strDigitalWallet.szmerchantsignkey);
				strcat(szData, szDummy);
			}
			else
			{
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_ATTACH, strDigitalWallet.szattach);
				strcpy(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_BODY, strDigitalWallet.szbody);
				strcat(szData, szDummy);

				if (srTransRec.HDTid != GCASH_HOST_INDEX)
				{
					memset(szDummy, 0x00, sizeof(szDummy));
					sprintf(szDummy, "%s=%s&", TAG_DEVICE_INFO, strDigitalWallet.szdevice_info);
					strcat(szData, szDummy);
				}
				
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_MCH_CREATE_IP, strDigitalWallet.szmch_create_ip);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_MCH_ID, srTransRec.szMID);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_NONCE_STR, strDigitalWallet.sznonce_str);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_NOTIFY_URL, strDigitalWallet.sznotify_url);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SERVICE, strDigitalWallet.szservice);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_TIME_START, strDigitalWallet.sztime_start);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_TOTAL_FEE, strDigitalWallet.sztotal_fee);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s", TAG_KEY, strDigitalWallet.szmerchantsignkey);
				strcat(szData, szDummy);
			}

			break;
		case WECHAT_ALIPAY_RETRIEVE_QRCODE:

			// -------------------------------------------------------
			// CPQR
			// -------------------------------------------------------
			if (iGetQRPresentedType() == CPQR_TYPE || (iGetQRPresentedType() == MPQR_TYPE))
			{
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_CHARSET, strDigitalWallet.szcharset);
				strcpy(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_MCH_ID, srTransRec.szMID);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_NONCE_STR, strDigitalWallet.sznonce_str);
				strcat(szData, szDummy);

				if (srTransRec.byVoided)
				{
					memset(szDummy, 0x00, sizeof(szDummy));
					sprintf(szDummy, "%s=%s&", TAG_OP_USER_ID, strDigitalWallet.szop_user_id);
					strcat(szData, szDummy);
				}		

				if (strGBLVar.fGBLvRetrieveList && strlen(strGBLVar.szGBLvSelectedValue) > 0)
				{
					if (srTransRec.iXMLRetrieveType == WECHAT_ALIPAY_MANUAL_RETRIEVE)
					{											
						if (srTransRec.byVoided)
						{
												
							memset(szDummy, 0x00, sizeof(szDummy));
							sprintf(szDummy, "%s=%s&", TAG_OUT_REFUND_NO, strDigitalWallet.szout_refund_no);
							strcat(szData, szDummy);

							memset(szDummy, 0x00, sizeof(szDummy));
							sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
							strcat(szData, szDummy);
													
							memset(szDummy, 0x00, sizeof(szDummy));
							sprintf(szDummy, "%s=%s&", TAG_REFUND_FEE, strDigitalWallet.sztotal_fee);
							strcat(szData, szDummy);
						}	
						else
						{
							memset(szDummy, 0x00, sizeof(szDummy));
							sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
							strcat(szData, szDummy);
						}
					}
					else
					{
						memset(szDummy, 0x00, sizeof(szDummy));
						sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
						strcat(szData, szDummy);
					}
				}
				else
				{
					if (srTransRec.iXMLRetrieveType == WECHAT_AUTO_RETRIEVE)
					{
						memset(szDummy, 0x00, sizeof(szDummy));
						sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
						strcat(szData, szDummy);										
					}
				}

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SERVICE, strDigitalWallet.szservice);
				strcat(szData, szDummy);			

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
				strcat(szData, szDummy);			

				if (srTransRec.byVoided)
				{
					memset(szDummy, 0x00, sizeof(szDummy));
					sprintf(szDummy, "%s=%s&", TAG_TOTAL_FEE, strDigitalWallet.sztotal_fee);
					strcat(szData, szDummy);
				}
				
				if (strGBLVar.fGBLvRetrieveList && strlen(strGBLVar.szGBLvSelectedValue) > 0)
				{
					// do nothing
				}
				else
				{
					if (srTransRec.iXMLRetrieveType == WECHAT_ALIPAY_MANUAL_RETRIEVE)
					{
						memset(szDummy, 0x00, sizeof(szDummy));
						sprintf(szDummy, "%s=%s&", TAG_TRANSACTION_ID, strDigitalWallet.sztransaction_id);
						strcat(szData, szDummy);					
					}
				}

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_VERSION, strDigitalWallet.szversion);
				strcat(szData, szDummy);	

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s", TAG_KEY, strDigitalWallet.szmerchantsignkey);
				strcat(szData, szDummy);	
			}
			// -------------------------------------------------------
			// CPQR
			// -------------------------------------------------------

			// -------------------------------------------------------
			// MPQR
			// -------------------------------------------------------
			#if 0
			if (iGetQRPresentedType() == MPQR_TYPE)
			{
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_CHARSET, strDigitalWallet.szcharset);
				strcpy(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_MCH_ID, srTransRec.szMID);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_NONCE_STR, strDigitalWallet.sznonce_str);
				strcat(szData, szDummy);

				if (strGBLVar.fGBLvRetrieveList && strlen(strGBLVar.szGBLvSelectedValue) > 0)
				{
					if (srTransRec.iXMLRetrieveType == WECHAT_ALIPAY_MANUAL_RETRIEVE)
					{
						memset(szDummy, 0x00, sizeof(szDummy));
						sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
						strcat(szData, szDummy);
					}
				}
				else
				{
					if (srTransRec.iXMLRetrieveType == WECHAT_AUTO_RETRIEVE)
					{
						memset(szDummy, 0x00, sizeof(szDummy));
						sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
						strcat(szData, szDummy);
					}
				}

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SERVICE, strDigitalWallet.szservice);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
				strcat(szData, szDummy);

				if (strGBLVar.fGBLvRetrieveList && strlen(strGBLVar.szGBLvSelectedValue) > 0)
				{
					// do nothing
				}
				else
				{
					if (srTransRec.iXMLRetrieveType == WECHAT_ALIPAY_MANUAL_RETRIEVE)
					{
						memset(szDummy, 0x00, sizeof(szDummy));
						sprintf(szDummy, "%s=%s&", TAG_TRANSACTION_ID, strDigitalWallet.sztransaction_id);
						strcat(szData, szDummy);
					}
				}

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_VERSION, strDigitalWallet.szversion);
				strcat(szData, szDummy);

				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s", TAG_KEY, strDigitalWallet.szmerchantsignkey);
				strcat(szData, szDummy);
			}
			// -------------------------------------------------------
			// MPQR
			// -------------------------------------------------------
			#endif
			
			break;
		 case WECHAT_ALIPAY_CLOSE_QRCODE:
			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_CHARSET, strDigitalWallet.szcharset);
			strcpy(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_MCH_ID, srTransRec.szMID);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_NONCE_STR, strDigitalWallet.sznonce_str);
			strcat(szData, szDummy);

			if (srTransRec.iXMLRetrieveType == WECHAT_AUTO_RETRIEVE)
			{
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
				strcat(szData, szDummy);
			}

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_SERVICE, strDigitalWallet.szservice);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
			strcat(szData, szDummy);			

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_VERSION, strDigitalWallet.szversion);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s", TAG_KEY, strDigitalWallet.szmerchantsignkey);
			strcat(szData, szDummy);
			break;
		case WECHAT_ALIPAY_VOID:
			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_CHARSET, strDigitalWallet.szcharset);
			strcpy(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_MCH_ID, srTransRec.szMID);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_NONCE_STR, strDigitalWallet.sznonce_str);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_OP_USER_ID, srTransRec.szMID);
			strcat(szData, szDummy);
			
			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_OUT_REFUND_NO, strDigitalWallet.szout_refund_no);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_REFUND_FEE, srTransRec.szXMLtotalfee);
			strcat(szData, szDummy);
			
			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_SERVICE, strDigitalWallet.szservice);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_TOTAL_FEE, srTransRec.szXMLtotalfee);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_TRANSACTION_ID, strDigitalWallet.sztransaction_id);
			strcat(szData, szDummy);
				
			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_VERSION, strDigitalWallet.szversion);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s", TAG_KEY, strDigitalWallet.szmerchantsignkey);
			strcat(szData, szDummy);
			
			break;
		case WECHAT_ALIPAY_REVERSAL:
			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_CHARSET, strDigitalWallet.szcharset);
			strcpy(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_MCH_ID, srTransRec.szMID);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_NONCE_STR, strDigitalWallet.sznonce_str);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_OUT_TRADE_NO, strDigitalWallet.szout_trade_no);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_SERVICE, strDigitalWallet.szservice);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_SIGN_TYPE, strDigitalWallet.szsign_type);
			strcat(szData, szDummy);			

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s&", TAG_VERSION, strDigitalWallet.szversion);
			strcat(szData, szDummy);

			memset(szDummy, 0x00, sizeof(szDummy));
			sprintf(szDummy, "%s=%s", TAG_KEY, strDigitalWallet.szmerchantsignkey);
			strcat(szData, szDummy);
			break;
		default:
			break;
	}

	vdDebug_LogPrintf("DATA FOR MD5[%s]", szData);

	if (strcmp(strDigitalWallet.szsign_type, "MD5") == 0)
		sprintf(uszMD5Data, "%s", MOLMD5String(szData));

	if (strcmp(strDigitalWallet.szsign_type, "SHA256") == 0)
		sprintf(uszMD5Data, "%s", SHA256String(szData));

	vdDebug_LogPrintf("uszMD5Data=[%s]", uszMD5Data);

	memset(szTemp, 0x00, sizeof(szTemp));
	for(i = 0; i < strlen(uszMD5Data); i++)
	{
       	szTemp[i] = toupper(uszMD5Data[i]);
	}

	memset(uszMD5Data, 0x00, sizeof(uszMD5Data));
	strcpy(uszMD5Data, szTemp);	

	// Sign
	strcpy(strDigitalWallet.szsign, uszMD5Data);

	vdDebug_LogPrintf("Sign Key[%s]", uszMD5Data);

}

char *getXMLVal(char* szXMLData, char* szTag)
{
   char sStartTag[XML_DATA_SIZE + 1] = {0};
   char sEndTag[XML_DATA_SIZE + 1] = {0};
   char *Startptr;
   char *Endptr;
   char szValue[XML_DATA_SIZE + 1];

   int fFound = FALSE;
   int inStartPos=0, inEndPos=0;
   int inLen=0,inTotalLen;

	memset(sStartTag, 0x00, sizeof(sStartTag));
	sprintf(sStartTag, "<%s>", szTag);
	memset(sEndTag, 0x00, sizeof(sEndTag));
	sprintf(sEndTag, "</%s>", szTag);

    inTotalLen=strlen(szXMLData);
    //printf("szXMLData len:%d\n", strlen(szXMLData));
    Startptr = strstr(szXMLData, sStartTag);

    inStartPos=strlen(Startptr)-strlen(sStartTag);
    //printf("sStartTag is: %s\nlen:%d\n", Startptr, inStartPos);

	Startptr = strstr(szXMLData, sEndTag);

    inEndPos=strlen(Startptr);
    //printf("sEndTag is: %s\nlen:%d\n", Startptr, inEndPos);

    inLen=inStartPos-inEndPos;

    //printf("inLen is: %d\n", inLen);

    memset(szValue, 0, sizeof(szValue));
    memcpy(szValue, szXMLData+(inTotalLen-inStartPos), inLen);
    printf("%s=%s\n",szTag, szValue);

	return szValue;
}

char *getXMLCdataVal(char* szXMLCData, char* szTag)
{
   char sStartTag[XML_DATA_SIZE + 1] = {0};
   char sEndTag[XML_DATA_SIZE + 1] = {0};
   char *Startptr;
   char *Endptr;
   char szValue[XML_DATA_SIZE + 1];
   char szCDataValue[XML_DATA_SIZE + 1];

   int fFound = FALSE;
   int inStartPos=0, inEndPos=0;
   int inLen=0,inTotalLen;

	memset(sStartTag, 0x00, sizeof(sStartTag));
	sprintf(sStartTag, "<%s>", szTag);
	memset(sEndTag, 0x00, sizeof(sEndTag));
	sprintf(sEndTag, "</%s>", szTag);

    inTotalLen=strlen(szXMLCData);
    //printf("szXMLData len:%d\n", strlen(szXMLData));
    Startptr = strstr(szXMLCData, sStartTag);

    inStartPos=strlen(Startptr)-strlen(sStartTag);
    //printf("sStartTag is: %s\nlen:%d\n", Startptr, inStartPos);

	Startptr = strstr(szXMLCData, sEndTag);

    inEndPos=strlen(Startptr);
    //printf("sEndTag is: %s\nlen:%d\n", Startptr, inEndPos);

    inLen=inStartPos-inEndPos;

    //printf("inLen is: %d\n", inLen);

    memset(szValue, 0, sizeof(szValue));
    memcpy(szValue, szXMLCData+(inTotalLen-inStartPos), inLen);
    //printf("%s=%s\n",szTag, szValue);

    memset(szCDataValue, 0, sizeof(szCDataValue));
    memcpy(szCDataValue, szValue+9, strlen(szValue)-12);

	return szCDataValue;
}

int iWechatDisplayQRCode(BOOL fConfirm)
{
	BYTE key;
	USHORT res;
	CTOS_QRCODE_INFO qrcodeInfo;
	char szCodeURL[255 + 1] = {0};
	int iQRSize = 0;
	int iQRX = 0;
	int iQRY = 0;
	int inRet;

	vdDebug_LogPrintf("--iWechatDisplayQRCode--");
	vdDebug_LogPrintf("byTransType=[%d]", srTransRec.byTransType);

	//CTOS_LCDTClearDisplay();
	//vdDispTransTitle(srTransRec.byTransType);

	if ((strTCT.byTerminalType % 2) == 0)
	{
		iQRSize = 7;
		iQRX = 50;
		iQRY = 100;
	}
	else
	{
		iQRSize = 4;
		iQRX = 100;
		iQRY = 50;
	}

	qrcodeInfo.InfoVersion = QR_INFO_VERSION;
	qrcodeInfo.Size = iQRSize;
	qrcodeInfo.Version = QR_VERSION21X21;
	qrcodeInfo.Level = QR_LEVEL_L;

	memset(szCodeURL, 0x00, sizeof(szCodeURL));
	strcpy(szCodeURL, srTransRec.szXMLcode_url);

	vdDebug_LogPrintf("szCodeURL[%s]", szCodeURL);

	//iGetKeyValuePairToCol(TAG_CODE_URL, szCodeURL);

	res = CTOS_QRCodeDisplay(&qrcodeInfo, szCodeURL, iQRX, iQRY);

	if (fConfirm)
		inRet = iXMLConfirmQRCode();

	return inRet;
}

BOOL fXMLBadRequestMessage(char* szXMLData)
{
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--fXMLBadRequestMessage--");

	fFound = fBadRequest(szXMLData);

	if (fFound)
	{
		vdDisplayErrorMsg(1, 8, "XML BAD REQUEST");
		vdSetErrorMessage("");
		return d_NO;
	}
	else
	{
		return d_OK;
	}
}

BOOL fXMLNotOKMessage(char* szXMLData)
{
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--fXMLNotOKMessage--");

	fFound = fOKMsg(szXMLData);

	if (!fFound)
	{
		vdDisplayErrorMsg(1, 8, "XML INVALID RESPONSE");
		vdSetErrorMessage("");
		return d_NO;
	}
	else
	{
		return d_OK;
	}
}

BOOL fBadRequest(char* szXMLData)
{
	char WorkBuffer[XML_SEND_SIZE];
	char *Startptr;
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--fBadRequest--");

	do
	{
		memset(WorkBuffer, 0x00, sizeof(WorkBuffer));
    	Startptr = strstr(szXMLData, szBadRequestMsg);

		if (Startptr != NULL)
		{
			fFound = TRUE;
			break;
		}

	}
	while (Startptr != NULL);

	return fFound;
}

BOOL fOKMsg(char* szXMLData)
{
	char WorkBuffer[XML_SEND_SIZE];
	char *Startptr;
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--fOKMsg--");

	do
	{
		memset(WorkBuffer, 0x00, sizeof(WorkBuffer));
    	Startptr = strstr(szXMLData, szOKMsg);

		if (Startptr != NULL)
		{
			fFound = TRUE;
			break;
		}

	}
	while (Startptr != NULL);

	return fFound;
}

BOOL fXMLForbiddenMessage(char* szXMLData)
{
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--fXMLForbiddenMessage--");

	fFound = fForbiddenRequest(szXMLData);

	if (fFound)
	{
		vdDisplayErrorMsg(1, 8, "403 FORBIDDEN");
		vdSetErrorMessage("");
		return d_NO;
	}
	else
	{
		return d_OK;
	}
}

BOOL fForbiddenRequest(char* szXMLData)
{
	char WorkBuffer[XML_SEND_SIZE];
	char *Startptr;
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--fForbiddenRequest--");

	do
	{
		memset(WorkBuffer, 0x00, sizeof(WorkBuffer));
    	Startptr = strstr(szXMLData, szForbiddenMsg);

		if (Startptr != NULL)
		{
			fFound = TRUE;
			break;
		}

	}
	while (Startptr != NULL);

	return fFound;
}

BOOL fCDataSection(char* szTag)
{
	BOOL fCData = TRUE;

	if (strcmp(szTag, TAG_NULL) == 0)
	{
		fCData = FALSE;
	}

	return fCData;

}

void iXMLGetDateTime(char* szCurrentDate, char* szCurrentTime)
{
	CTOS_RTC SetRTC;
	char szYear[4+1] = {0};
	char szMonth[2+1] = {0};
	char szDay[2+1] = {0};
	char szTime[6+1] = {0};

	vdDebug_LogPrintf("--iXMLGetDateTime--");

	CTOS_RTCGet(&SetRTC);

	sprintf(szYear ,"20%02d",SetRTC.bYear);
	sprintf(szMonth ,"%02d",SetRTC.bMonth);
	sprintf(szDay ,"%02d",SetRTC.bDay);
	sprintf(szTime ,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);

	vdDebug_LogPrintf("szYear[%s]", szYear);
	vdDebug_LogPrintf("szMonth[%s]", szMonth);
	vdDebug_LogPrintf("szDay[%s]", szDay);
	vdDebug_LogPrintf("szTime[%s]", szTime);

	sprintf(szCurrentDate, "%s%s%s", szYear, szMonth, szDay);
	sprintf(szCurrentTime, "%s", szTime);

	vdDebug_LogPrintf("szCurrentDate[%s]", szCurrentDate);
	vdDebug_LogPrintf("szCurrentTime[%s]", szCurrentTime);
}

int iGetKeyValuePairToCol(char* szTag, char* szValue)
{
	int i;
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--iGetKeyValuePairToCol--");

	memset(szValue, 0x00, sizeof(szValue));

	for (i = 0; i < srResponse.srKVPairCol.iCount; i++)
	{
		//vdDebug_LogPrintf("srResponse.srKVPairCol.kvArray[%d].szKey[%s].szValue[%s]", i, srResponse.srKVPairCol.kvArray[i].szKey, srResponse.srKVPairCol.kvArray[i].szValue);
		if (strcmp(szTag, srResponse.srKVPairCol.kvArray[i].szKey) == 0)
		{
			fFound = TRUE;
			strcpy(szValue, srResponse.srKVPairCol.kvArray[i].szValue);
			break;
		}
	}

	//vdDebug_LogPrintf("fFound[%d]", fFound);
	//vdDebug_LogPrintf("szTag[%s].szValue[%s]", szTag, szValue);

	return d_OK;
}

void iXMLDisplayResponse(char* szMessage, int iLen)
{
	char szBuffer[256 + 1] = {0};
	char szMsg1[256 + 1] = {0};
	char szMsg2[256 + 1] = {0};
	char szMsg3[256 + 1] = {0};
	int iLenWidth = 16;
	char szTemp[256 + 1] = {0};
        int i = 0;

	//vdDebug_LogPrintf("--iXMLDisplayResponse--");

	// To Upper Case
	memset(szTemp, 0x00, sizeof(szTemp));
	for(i = 0; i < strlen(szMessage); i++)
	{
	   	szTemp[i] = toupper(szMessage[i]);
	}

	iWord_Wrap(szBuffer, szTemp, iLen);

	GetDelimitedString(szBuffer, 1, '\n', szMsg1);
	GetDelimitedString(szBuffer, 2, '\n', szMsg2);
	GetDelimitedString(szBuffer, 3, '\n', szMsg3);

	vdDebug_LogPrintf("szMsg1[%s]", szMsg1);
	DebugAddHEX("szMsg1", szMsg1, strlen(szMsg1));
	vdDebug_LogPrintf("szMsg2[%s]", szMsg2);
	DebugAddHEX("szMsg2", szMsg2, strlen(szMsg2));
	vdDebug_LogPrintf("szMsg3[%s]", szMsg3);
	DebugAddHEX("szMsg3", szMsg3, strlen(szMsg3));

#if 0
    memset(szMsg1,0,sizeof(szMsg1));
	memset(szMsg2,0,sizeof(szMsg2));
	memset(szMsg3,0,sizeof(szMsg3));
	
	strcpy(szMsg1,"当前商户需补齐相关资");
	strcpy(szMsg2,"后，才可进");
	strcpy(szMsg3,"行相应的支");

	DebugAddHEX("1.szMsg1", szMsg1, strlen(szMsg1));
	DebugAddHEX("1.szMsg2", szMsg2, strlen(szMsg2));
	DebugAddHEX("1.szMsg3", szMsg3, strlen(szMsg3));
#endif

	if(fGetECRTransactionFlg()==TRUE)
    {
         strcpy(srTransRec.szECRRespCode, ECR_DECLINED_ERR);
         strcpy(srTransRec.szResponseText, ECR_INVALID_TRANS_RESP);
    }
	vdDisplayMessage(szMsg1, szMsg2, szMsg3);
	CTOS_Beep();
    CTOS_Delay(3000);

}

BOOL fXMLCheckTagExist(char* szXMLData, char* szTag)
{
	char WorkBuffer[XML_SEND_SIZE];
	char *Startptr;
	BOOL fFound = FALSE;

	//vdDebug_LogPrintf("--fXMLCheckTagExist--");

	do
	{
		memset(WorkBuffer, 0x00, sizeof(WorkBuffer));
    	Startptr = strstr(szXMLData, szTag);

		if (Startptr != NULL)
		{
			fFound = TRUE;
			break;
		}

	}
	while (Startptr != NULL);

	//vdDebug_LogPrintf("fFound[%d]", fFound);

	return fFound;
}

int iXMLCheckResponseStatus(char* szXMLReceiveData)
{
	BOOL fValid = FALSE;
	BYTE szTemp[256 + 1] = {0};
	BYTE szXMLValue[256 + 1] = {0};
	BOOL fTagExist = FALSE;
	BYTE szErroMsg[256 + 1] = {0};
	int iWidth = 16;
	int i;

	vdDebug_LogPrintf("--iXMLCheckResponseStatus--");

	srTransRec.inXMLstatus = d_NO;

	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_STATUS, szXMLValue);

	// Check Status
	if (strcmp(szXMLValue, "0") == 0)
	{
		fValid = TRUE;
		srTransRec.inXMLstatus = 0;
	}

	if (!fValid)
	{
		// Display Error Message (TAG_MESSAGE)
		vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_MESSAGE, szXMLValue);

		if (strlen(szXMLValue) > 0)
		{
			iXMLDisplayResponse(szXMLValue, iWidth);
		}

		return d_NO;

	}

    //check result code
    vdDebug_LogPrintf("--iXMLCheckResponseStatus-- CHECK RESULT CODE");
	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_RESULT_CODE, szXMLValue);
	if (strcmp(szXMLValue, "0") == 0)
	{
		srTransRec.inXMLstatus = 0;
	}else{
		memset(szXMLValue, 0x00, sizeof(szXMLValue));
		vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_ERR_MSG, szXMLValue);
		
		if (strlen(szXMLValue) > 0)
			iXMLDisplayResponse(szXMLValue, iWidth);
		else	
			iXMLDisplayResponse("ERROR RESPONSE", iWidth);
		
		return d_NO;

	}

	//end-check result code
	

	return d_OK;
}

int iXMLCheckResponseResultCode(char* szXMLReceiveData)
{
	BOOL fValid = FALSE;
	BYTE szTemp[256 + 1] = {0};
	BYTE szXMLValue[256 + 1] = {0};
	BOOL fTagExist = FALSE;
	BYTE szErroMsg[256 + 1] = {0};
	int iWidth = 16;
	int i;

	vdDebug_LogPrintf("--iXMLCheckResponseResultCode--");

	srTransRec.inXMLresult_code = d_NO;

	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_RESULT_CODE, szXMLValue);

	// Check Result Code
	if (strcmp(szXMLValue, "0") == 0)
	{
		fValid = TRUE;
		srTransRec.inXMLresult_code = 0;
		//strcpy(srTransRec.szRespCode,"00");
	}

	if (!fValid)
	{
		// Display Error Message (TAG_ERR_CODE)
		vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_ERR_CODE, szXMLValue);

		if (strlen(szXMLValue) > 0)
		{
			iXMLDisplayResponse(szXMLValue, iWidth);
		}

		return d_NO;
	}

	return d_OK;
}

int iXMLCheckTradeState(char* szXMLReceiveData)
{
	BOOL fValid = FALSE;
	BYTE szTemp[256 + 1] = {0};
	BYTE szXMLValue[256 + 1] = {0};
	BOOL fTagExist = FALSE;
	BYTE szErroMsg[256 + 1] = {0};
	int iWidth = 16;
	int i;
	BOOL fAutoRetrieve = (BOOL)get_env_int("XMLAUTORETRIEVE");

	vdDebug_LogPrintf("--iXMLCheckTradeState--");

	memset(srTransRec.szXMLtrade_state, 0x00, sizeof(srTransRec.szXMLtrade_state));
	srTransRec.fXMLTradeStateOK = FALSE;

	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_TRADE_STATE, szXMLValue);
	vdDebug_LogPrintf("TAG_TRADE_STATE[%s]", szXMLValue);

	if (strlen(szXMLValue) > 0)
	{
		if (strcmp(szXMLValue, TRADE_STATE_SUCCESS) == 0)
		{
			vdDebug_LogPrintf("TRADE STATE = SUCCESS!!!");
			fValid = TRUE;
			srTransRec.fXMLTradeStateOK = TRUE;
		}
		else if (strcmp(szXMLValue, TRADE_STATE_NOTPAY) == 0)
		{
			vdDebug_LogPrintf("TRADE STATE = NOTPAY!!!");

			if (fAutoRetrieve == TRUE)
			{
				fValid = TRUE;
				srTransRec.fXMLTradeStateOK = FALSE;
			}
		}

		sprintf(srTransRec.szXMLtrade_state, "TRADE STATE %s", szXMLValue);
		strcpy(srTransRec.szTradeState, szXMLValue);
	}

	if (!fValid)
	{
		sprintf(szErroMsg, "TRADE STATE %s", szXMLValue);

		if (strlen(szErroMsg) > 0)
		{
			iXMLDisplayResponse(szXMLValue, iWidth);
		}

		return d_NO;
	}

	return d_OK;
}


int iXMLCheckOKStatus(char* szXMLReceiveData)
{
	BOOL fValid = FALSE;
	BOOL szTemp[256 + 1] = {0};
	BOOL szXMLValue[256 + 1] = {0};
	BOOL fTagExist = FALSE;
	char szErroMsg[256 + 1] = {0};
	int iWidth = 16;
	int i;

	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_TRADE_STATE, szXMLValue);

	if (strlen(szXMLValue) > 0)
	{
		if (strcmp(szXMLValue, "OK") == 0)
		{
			fValid = TRUE;
		}
	}
	else
	{
		fValid = TRUE;
	}

	if (!fValid)
	{
		vdDisplayMessage("XML", "INVALID RESPONSE", "");

		return d_NO;
	}

	return d_OK;
}

void vXMLGetResponseDataByTag(int iParseType, char* szXMLReceiveData, char* szXMLTag, char* szXMLValue)
{
	BOOL fTagExist = FALSE;

	//vdDebug_LogPrintf("--vXMLGetResponseDataByTag--");

	memset(szXMLValue, 0x00, sizeof(szXMLValue));

	fTagExist = fXMLCheckTagExist((char*)szXMLReceiveData, (char*)szXMLTag);

	if (fTagExist)
	{

		switch (iParseType)
		{
			case XML_PARSE_STRING:
				strcpy(szXMLValue, getXMLCdataVal((char*)szXMLReceiveData, szXMLTag));
				break;
			case XML_PARSE_NUMERIC:
				strcpy(szXMLValue, getXMLVal((char*)szXMLReceiveData, szXMLTag));
				break;
			default:
				break;
		}
	}
	else
	{
		vdDebug_LogPrintf("XML Tag[%s] does not exist.!!!", szXMLTag);
	}

	//vdDebug_LogPrintf("fTagExist[%d].szXMLTag[%s].szXMLValue[%s]", fTagExist, szXMLTag, szXMLValue);

}

int vXMLGetNonce_str(char* szNonce_str)
{
char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
size_t stringLen = 26*2+10+7;
int length = 32;
unsigned int key = 0;
int n = 0;
char *randomstring;

	vdDebug_LogPrintf("--vXMLGetNonce_str--");

	randomstring = malloc(sizeof(char) * (length +1));

	if (!randomstring) {
		//possible fix for crash
		free(randomstring);
	    return (char*)0;
	}

	for (n = 0;n < length;n++)
	{
	    key = rand() % stringLen;
	    randomstring[n] = string[key];
	}

	randomstring[length] = '\0';
	strcpy(szNonce_str, randomstring);	

	//possible fix for crash
	free(randomstring);
	
}

void vXMLGenerateNonce_Str(char* szNonce_str)
{
	BYTE szTemp[32 + 1] = {0};
	BOOL fValid = TRUE;
	int iCounter = 0;

	vdDebug_LogPrintf("--vXMLGenerateNonce_Str--");
	
	do
	{
		vXMLGetNonce_str((char*)szTemp);

		iCounter++;

		vdDebug_LogPrintf("iCounter[%d].Len[%d].szNonce_str[%s]", iCounter, strlen(szTemp), szTemp);
		
		if (strlen(szTemp) > 5)
		break;
		
	} while (fValid);

	memset(szNonce_str, 0x00, sizeof(szNonce_str));
	strcpy(szNonce_str, szTemp);

	vdDebug_LogPrintf("Exit: Len[%d].szTemp[%s]", strlen(szTemp), szTemp);
	vdDebug_LogPrintf("Exit: Len[%d].szNonce_str[%s]", strlen(szNonce_str), szNonce_str);
	
}

void vXMLGetOut_trade_no(char* szOut_trade_no)
{
	CTOS_RTC SetRTC;
	char szYear[4+1] = {0};
	char szMonth[2+1] = {0};
	char szDay[2+1] = {0};
	char szTime[6+1] = {0};
	char szCurrentDate[8 + 1] = {0};
	char szCurrentTime[6 + 1] = {0};
	char szInvNo[6 + 1];

	vdDebug_LogPrintf("--vXMLGetOut_trade_no--");
	vdDebug_LogPrintf("iXMLType[%d]", srTransRec.iXMLType);

	memset(szInvNo, 0x00, sizeof(szInvNo));
	memset(szYear, 0x00, sizeof(szYear));
	memset(szMonth, 0x00, sizeof(szMonth));
	memset(szDay, 0x00, sizeof(szDay));
	memset(szCurrentDate, 0x00, sizeof(szCurrentDate));
	memset(szCurrentTime, 0x00, sizeof(szCurrentTime));
	memset(szOut_trade_no, 0x00, sizeof(szOut_trade_no));

	CTOS_RTCGet(&SetRTC);

	sprintf(szYear ,"%02d",SetRTC.bYear);
	sprintf(szMonth ,"%02d",SetRTC.bMonth);
	sprintf(szDay ,"%02d",SetRTC.bDay);
	sprintf(szTime ,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
	sprintf(szCurrentDate, "%s%s%s", szYear, szMonth, szDay);
	sprintf(szCurrentTime, "%s", szTime);

	wub_hex_2_str(srTransRec.szInvoiceNo, szInvNo, INVOICE_BCD_SIZE);

	if (srTransRec.iXMLType == WECHAT_ALIPAY_REQUEST_QRCODE || srTransRec.iXMLType == WECHAT_ALIPAY_VOID)
	{
		sprintf(szOut_trade_no, "%s%s%s", srTransRec.szMID, szCurrentTime, szInvNo);
	}
	else
	{
		memset(szOut_trade_no, 0x00, sizeof(szOut_trade_no));
		strcpy(szOut_trade_no, strDigitalWallet.szout_trade_no);
	}

	vdDebug_LogPrintf("Len[%d].szOut_trade_no[%s]", strlen(szOut_trade_no), szOut_trade_no);
}

int iXMLQRDisplayComplete(void)
{
	vdDebug_LogPrintf("--iXMLQRDisplayComplete--");
	vdDebug_LogPrintf("iXMLType[%d]", srTransRec.iXMLType);

	if (srTransRec.iXMLType == WECHAT_ALIPAY_REQUEST_QRCODE)
		vdDisplayMessage("WECHAT", "QRCODE REQUEST", "COMPLETE");

	else if (srTransRec.iXMLType == WECHAT_ALIPAY_RETRIEVE_QRCODE)
		vdDisplayMessage("WECHAT", "QRCODE RETRIEVE", "COMPLETE");

	return d_OK;
}

int iXMLConfirmQRCode(void)
{
	unsigned char key;
	int inRet = d_NO;
	BOOL fConfirm = (BOOL)get_env_int("XMLCONFIRMQRCODE");
	int iDelay = get_env_int("XMLCONFIRMDELAY");
	
	if (iDelay <= 0) iDelay = 10;

	if ((strTCT.byTerminalType % 2) == 0)
	{
		clearLine(13);
		clearLine(14);
		CTOS_LCDTPrintAligned(13, "PLEASE SCAN QR", d_LCD_ALIGNCENTER);

		if (fConfirm)
			CTOS_LCDTPrintAligned(14, "PRESS ENTER KEY", d_LCD_ALIGNCENTER);
		else
			CTOS_Delay(iDelay * 1000); // Delay
	}
	else
	{
		clearLine(7);
		clearLine(8);
		CTOS_LCDTPrintAligned(7, "PLEASE SCAN QR", d_LCD_ALIGNCENTER);

		if (fConfirm)
			CTOS_LCDTPrintAligned(8, "PRESS ENTER KEY", d_LCD_ALIGNCENTER);
		else
			CTOS_Delay(iDelay * 1000); // Delay
	}

	if (fConfirm)
	{
		while(1)
		{
			CTOS_KBDGet(&key);

			if (key == d_KBD_ENTER)
			{
				inRet = d_OK;
				break;
			}

			if (key == d_KBD_CANCEL)
			{
				inRet = d_NO;
				break;
			}
		}

		if (inRet != d_OK)
			vdSetErrorMessage("USER CANCEL");
	}
	else
	{
		inRet = d_OK;
	}

	return inRet;
}


BYTE iXMLGetchWithTimeOut(short Sec)
{
    unsigned char c;
    BOOL isKey;
    long mlsec;

    mlsec=Sec*100;

    CTOS_TimeOutSet(TIMER_ID_3,mlsec);

    while(1)//loop for time out
    {
        CTOS_KBDInKey(&isKey);
        if (isKey){ //If isKey is TRUE, represent key be pressed //
            vduiLightOn();
            //Get a key from keyboard //
            CTOS_KBDGet(&c);
            return c;
        }
        else if (CTOS_TimeOutCheck(TIMER_ID_3) == d_YES)
        {
            return d_KBD_CANCEL;
        }
    }
}

void vXMLGetService(char* szService)
{
	switch (srTransRec.iXMLType)
	{
		case WECHAT_ALIPAY_REQUEST_QRCODE:
				if (srTransRec.HDTid == WECHAT_HOST_INDEX)
					strcpy(szService, "pay.weixin.native.intl");
				if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
					strcpy(szService, "pay.alipay.native.intl");
			break;
		case WECHAT_ALIPAY_RETRIEVE_QRCODE:
					strcpy(szService, "unified.trade.query");
			break;
	}
}

int iXMLSetValueByTag(char* szTag, char* szValue)
{
	char szTemp[20 + 1] = {0};

	vdDebug_LogPrintf("iXMLSetValueByTag::szTag[%s].szValue[%s]", szTag, szValue);
		
	if (strcmp(szTag, TAG_BANK_TYPE) == 0)
		strcpy(srTransRec.szXMLbank_type, szValue);

	else if (strcmp(szTag, TAG_CASH_FEE) == 0)
	{
		memset(szTemp, 0x00, sizeof(szTemp));
		sprintf(szTemp, "%012ld", atol(szValue));
		strcpy(srTransRec.szXMLcash_fee, szTemp);
	}		

	else if (strcmp(szTag, TAG_CASH_FEE_TYPE) == 0)
		strcpy(srTransRec.szXMLcash_fee_type, szValue);

	else if (strcmp(szTag, TAG_FEE_TYPE) == 0)
		strcpy(srTransRec.szXMLfee_type, szValue);

	else if (strcmp(szTag, TAG_LOCAL_FEE_TYPE) == 0)
		strcpy(srTransRec.szXMLlocal_fee_type, szValue);

	else if (strcmp(szTag, TAG_LOCAL_TOTAL_FEE) == 0)
	{
		memset(szTemp, 0x00, sizeof(szTemp));
		sprintf(szTemp, "%012ld", atol(szValue));
		strcpy(srTransRec.szXMLlocal_total_fee, szTemp);
	}
		
	else if (strcmp(szTag, TAG_ORDER_FEE) == 0)
	{
		strcpy(srTransRec.szXMLorder_fee, szValue);

		if (strlen(szValue) > 0)
		{
			memset(szTemp, 0x00, sizeof(szTemp));
			sprintf(szTemp, "%012ld", atol(srTransRec.szXMLorder_fee));
    	    wub_str_2_hex(szTemp,srTransRec.szBaseAmount, 12);
			wub_str_2_hex(szTemp,srTransRec.szTotalAmount, 12);
		}
	}

	else if (strcmp(szTag, TAG_OUT_TRADE_NO) == 0)
		strcpy(srTransRec.szXMLout_trade_no, szValue);

	else if (strcmp(szTag, TAG_OUT_TRANSACTION_ID) == 0)
		strcpy(srTransRec.szXMLout_transaction_id, szValue);

	else if (strcmp(szTag, TAG_PLATFORM_RATE) == 0)
		strcpy(srTransRec.szXMLplatform_rate, szValue);

	else if (strcmp(szTag, TAG_TIME_START) == 0)
		strcpy(srTransRec.szXMLtime_start, szValue);

	else if (strcmp(szTag, TAG_TIME_END) == 0)
		strcpy(srTransRec.szXMLtime_end, szValue);

	else if (strcmp(szTag, TAG_TOTAL_FEE) == 0)
	{
		memset(szTemp, 0x00, sizeof(szTemp));
		sprintf(szTemp, "%012ld", atol(szValue));
		strcpy(srTransRec.szXMLtotal_fee, szTemp);
	}

	else if (strcmp(szTag, TAG_TRANSACTION_ID) == 0)
		strcpy(srTransRec.szXMLtransaction_id, szValue);

	else if (strcmp(szTag, TAG_CODE_URL) == 0)
	{
		strcpy(srTransRec.szXMLcode_url, szValue);
		vdDebug_LogPrintf("TAG_CODE_URL = [%s]", szValue);
	}
		
	else if (strcmp(szTag, TAG_REFUND_ID) == 0)
		strcpy(srTransRec.szRefundID, szValue);

	// For testing purpose
	else if (strcmp(szTag, TAG_SIGN) == 0)
		vdDebug_LogPrintf("TAG_SIGN = [%s]", szValue);

	else if (strcmp(szTag, TAG_SIGN_TYPE) == 0)
		vdDebug_LogPrintf("TAG_SIGN_TYPE = [%s]", szValue);

	else if (strcmp(szTag, TAG_CODE_IMG_URL) == 0)
		vdDebug_LogPrintf("TAG_CODE_IMG_URL = [%s]", szValue);

	else if (strcmp(szTag, TAG_UUID) == 0)
		vdDebug_LogPrintf("TAG_UUID = [%s]", szValue);

	else if (strcmp(szTag, TAG_OUT_REFUND_NO) == 0)
		strcpy(srTransRec.szXMLout_refund_no, szValue);
	
	else if (strcmp(szTag, TAG_REFUND_STATUS) == 0)
		strcpy(srTransRec.szXMLrefund_status, szValue);

	else if (strcmp(szTag, TAG_REFUND_FEE) == 0)
		strcpy(srTransRec.szXMLrefund_fee, szValue);
}

/* Word Wrap -- sidumili */
char* iWord_Wrap (char* szOutBuffer, char* szInString, int line_width)
{
	int i = 0, inLastSpace = 0, inDifference = 0, j = 1;
	int k, counter;
	int inRow = 0;

	while(i < strlen( szInString ) )
	{
		// copy string until the end of the line is reached
		for ( counter = 1; counter <= line_width; counter++ )
		{
			// check if end of string reached
			if ( i == strlen( szInString ) )
			{
				szOutBuffer[ i + inDifference ] = 0;
				return szOutBuffer;
			}
			szOutBuffer[ i + inDifference] = szInString[ i ];
			// check for newlines embedded in the original input
			// and reset the index
			if ( szOutBuffer[ i + inDifference ] == '\n' )
			{
				counter = 1;
			}
			i++;
		}
		// check for whitespace
		if ( isspace( szInString[ i ] ) )
		{
			szOutBuffer[i + inDifference] = '\n';
			inLastSpace = i;
			i++;
		}
		else
		{
			// check for nearest whitespace back in string
			for ( k = i; k > 0; k--)
			{
				if(j >= line_width+1)
				{

					j = 1;
					szOutBuffer[ i + inDifference ] = '\n';
					inDifference++;
					break;
				}

				if ( isspace( szInString[ k ] ) )
				{
					szOutBuffer[ k + inDifference ] = '\n';
					// set string index back to character after this one
					i = k + 1;
					inLastSpace = k;
					j = 1;//reset vaule of j.
					break;					
				}

				j++;
			}
		}
		
	}
	szOutBuffer[ i + inDifference] = 0;

	return(szOutBuffer);

}


/* Delimited string -- sidumili */
int GetDelimitedString(char *buf, int fldnum, unsigned char sep, char *dest)
{
	unsigned char *ptr;
	int i;

	ptr = (unsigned char *) buf;
	for (i=1; i < fldnum; i++)
	{
		if ((ptr = (unsigned char *) strchr((char *) ptr, sep))) ptr++;
		else return(0);
	}

	for (i=0;*ptr != sep && *ptr && i<256;i++) *dest++ = *ptr++;
	*dest = '\0';

	if (*ptr != sep) return(0);

	return(i);
}

int iXMLTransactionIDEntry(void)
{
	BYTE key;
	USHORT usX =1, usY = 6;
	BYTE baString[100+1];
	USHORT iStrLen = 6;
	BYTE bShowAttr = 0x02; 
	USHORT usInvoiceLen = 6;
	BYTE baPIN[6 + 1];
	BYTE   szInvNoAsc[30+1];
	BYTE   szInvNoBcd[3];
	BYTE bRet;
	int iLens = 6;
	int  inResult;
	char szBcd[INVOICE_BCD_SIZE+1];

	BYTE szTitle[16];
	BYTE szDisplay[100];
	BYTE szDisMsg[100];
	BYTE   szTransactionID[30 + 1] = {0};

	vdDebug_LogPrintf("--iXMLTransactionIDEntry--");
	
	memset(szTitle, 0x00, sizeof(szTitle));
	szGetTransTitle(srTransRec.byTransType, szTitle);

	/*
	if (inMultiAP_CheckSubAPStatus() == d_OK)
	{
		return d_OK;
	}
	*/

	memset(szInvNoAsc, 0x00, sizeof(szInvNoAsc));
	memset(szInvNoBcd, 0x00, sizeof(szInvNoBcd));
	
	strcpy(szDisplay, "1");
	strcat(szDisplay, "|");
    strcat(szDisplay, "30");
	strcat(szDisplay, "|");
    strcat(szDisplay, szTitle);
    strcat(szDisplay, "|");
    if (srTransRec.HDTid = PHQR_HOST_INDEX)
    	strcat(szDisplay, "ENTER REFERENCE NO.:");
	else
    	strcat(szDisplay, "ENTER ORDER ID:");
		
	while(TRUE)
	{
		usInvoiceLen = 30;
		vdDebug_LogPrintf("11bRet[%d]atoi(szInvNoAsc)=[%d]usInvoiceLen[%d]",bRet,atoi(szInvNoAsc),usInvoiceLen);
		bRet = InputStringUI(0x01, 0x02, szInvNoAsc, &usInvoiceLen, 1, 6000, szDisplay);		
		vdDebug_LogPrintf("bRet[%d]atoi(szInvNoAsc)=[%d]usInvoiceLen[%d]",bRet,atoi(szInvNoAsc),usInvoiceLen);
		if (bRet == d_KBD_CANCEL )
		{
			return (d_EDM_USER_CANCEL);
		}
		else if (bRet == d_KBD_ENTER ) 
		{
			if(strlen(szInvNoAsc) != 0)
			{		
				memset(strDigitalWallet.sztransaction_id, 0x00, sizeof(strDigitalWallet.sztransaction_id));
				memset(srTransRec.szXMLtransaction_id, 0x00, sizeof(srTransRec.szXMLtransaction_id));
				strcpy(strDigitalWallet.sztransaction_id, szInvNoAsc);
				strcpy(srTransRec.szXMLtransaction_id, szInvNoAsc);

				vdDebug_LogPrintf("Len[%d].strDigitalWallet.sztransaction_id[%s]", strlen(strDigitalWallet.sztransaction_id), strDigitalWallet.sztransaction_id);
				
				break;
			}
			else
			{
				vdDisplayMessageBox(1, 8, "", "INVALID INPUT", "", MSG_TYPE_ERROR);
                CTOS_Beep();
                CTOS_Delay(1500);
			}
		}
		else
		{
			vdDisplayMessageBox(1, 8, "", "INVALID INPUT", "", MSG_TYPE_ERROR);
            CTOS_Beep();
            CTOS_Delay(1500);
		}								
	}
	 
	return ST_SUCCESS;
}



int inCTOS_XMLSelectHost(void)
{
    short shGroupId ;
    int inHostIndex;
    short shCommLink;
    int inCurrencyIdx=0;
	char szAscBuf[4 + 1], szBcdBuf[2 + 1];
	char szStr[16+1]={0};

	inDatabase_TerminalOpenDatabase();

	inHostIndex = srTransRec.HDTid;

	strCDT.HDTid = inHostIndex;
	/* BDO: Make sure we use the BDO Credit host details first - end -- jzg */

	vdDebug_LogPrintf("inCTOS_XMLSelectHost::Txn Type = [%d]", srTransRec.byTransType);
	vdDebug_LogPrintf("1inCTOS_XMLSelectHost = [%d]", inHostIndex);

	/*
	if(inMultiAP_CheckSubAPStatus() == d_OK)	
		return d_OK;	
	*/
	
	//if(inHDTRead(inHostIndex) != d_OK)
	if(inHDTReadEx(inHostIndex) != d_OK)
	{
		vdDebug_LogPrintf("inHDTReadEx error");
		inHDTReadDataEx(inHostIndex);
		strcpy(szStr,strHDT_Temp.szHostLabel);
		memset(strHDT_Temp.szHostLabel,0x00,16+1);
		sprintf(strHDT_Temp.szHostLabel," %s ",szStr);
		vdDisplayErrorMsgResp2(strHDT_Temp.szHostLabel, "TRANSACTION", "NOT ALLOWED");
		inDatabase_TerminalCloseDatabase();
		return(d_NO);
	} 
	else 
	{
	    
		vdDebug_LogPrintf(" strHDT.inCurrencyIdx = [%d]", strHDT.inCurrencyIdx);
		srTransRec.HDTid = inHostIndex;

		inCurrencyIdx = strHDT.inCurrencyIdx;

		if(inCSTReadEx(inCurrencyIdx) != d_OK) 
		{
		    
			vdDebug_LogPrintf(" cst read err");
			vdSetErrorMessage("LOAD CST ERR");
			inDatabase_TerminalCloseDatabase();
			return(d_NO);
		}

        //if(strTCT.fSingleComms == TRUE)
		//	inHostIndex=1;

		if(inCPTReadEx(inHostIndex) != d_OK)
		{
			vdSetErrorMessage("LOAD CPT ERR");
			inDatabase_TerminalCloseDatabase();
			return(d_NO);
		}

		if (srTransRec.byEntryMode == CARD_ENTRY_WAVE)
		if (strCST.inCurrencyIndex > 1)
		{
			memset(szAscBuf, 0x00, sizeof(szAscBuf));	
			memset(szBcdBuf, 0x00, sizeof(szBcdBuf));
		
			sprintf(szAscBuf,"%4s",strCST.szCurCode);	
			
			wub_str_2_hex(szAscBuf, szBcdBuf, 4);
			memcpy((char *)srTransRec.stEMVinfo.T5F2A, &szBcdBuf[0], 2);
			DebugAddHEX("T5F2A..",srTransRec.stEMVinfo.T5F2A,2);
		}
		
		return (d_OK);
	}	
}

int inXML_CommsInit(void)
{
	int inRetVal; 	

	vdDebug_LogPrintf("--inXML_CommsInit--");	
	vdDebug_LogPrintf("HDTid[%d]", srTransRec.HDTid);
	//vdDebug_LogPrintf("fSingleComms[%d]", strTCT.fSingleComms);
	vdDebug_LogPrintf("inCommunicationMode[%d]", strCPT.inCommunicationMode);

	//CTOS_LCDTClearDisplay();
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");
	
	inCPTRead(srTransRec.HDTid);			
	strCPT.fSSLEnable = TRUE;
	if(srTransRec.HDTid == 25)
		strCPT.fSSLEnable = FALSE;
	strCPT.inIPHeader = NO_HEADER_LEN;
	
	srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
	
	vdDebug_LogPrintf("usTerminalCommunicationMode[%d]", srTransRec.usTerminalCommunicationMode);
	vdDebug_LogPrintf("SSL[%d]",strCPT.fSSLEnable);
	vdDebug_LogPrintf("fDHCPEnable[%d]",strTCP.fDHCPEnable);
	vdDebug_LogPrintf("fShareComEnable[%d]",strTCT.fShareComEnable);
	vdDebug_LogPrintf("Primary[%s]port[%ld]",strCPT.szPriTxnHostIP,strCPT.inPriTxnHostPortNum);
	vdDebug_LogPrintf("Secondary[%s]port[%ld]",strCPT.szSecTxnHostIP,strCPT.inSecTxnHostPortNum);

	vdSetInit_Connect(1);
	
    if ((strCPT.inCommunicationMode == GPRS_MODE) && strCPT.fSSLEnable == TRUE)
	    vdSetInit_Connect(1); // sidumili: set to 1 to support gprs connection

	if ((strCPT.inCommunicationMode == ETHERNET_MODE) && strCPT.fSSLEnable == TRUE)
		vdSetInit_Connect(0);
		
    if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK)
    {
		vdDisplayErrorMsgResp2("","Initialization","Error");
		vdSetErrorMessage("");
		vdDebug_LogPrintf("Initialization error");
        return(d_NO);
    }


	vdDebug_LogPrintf("--inXML_CommsInit--BEFORE inCTOS_CheckInitComm");	

    	inRetVal = inCTOS_CheckInitComm(srTransRec.usTerminalCommunicationMode);

	vdDebug_LogPrintf("saturn inCTOS_CheckInitComm | inReturn[%d]|fGPRSConnectOK[%d]", inRetVal, fGPRSConnectOK);

	if (inRetVal != d_OK || !fGPRSConnectOK)
	{
		switch (srTransRec.usTerminalCommunicationMode )
		{
			case GPRS_MODE:
				//vdDisplayMultiLineMsgAlign("", "GPRS PROBLEM", "PLESE TRY AGAIN", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "GPRS PROBLEM", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
			case WIFI_MODE:
				//vdDisplayMultiLineMsgAlign("", "WIFI PROBLEM", "PLESE TRY AGAIN", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "WIFI PROBLEM", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
			case ETHERNET_MODE:
				//vdDisplayMultiLineMsgAlign("", "ETHERNET PROBLEM", "PLESE TRY AGAIN", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "ETHERNET PROBLEM", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
			default:
				//vdDisplayMultiLineMsgAlign("", "COMM INIT", "ERROR", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "COMM INIT", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
		}

		vdSetErrorMessage("");
		if(srTransRec.byTransType == SETTLE)
			vdDisplayErrorMsg2(1, 8, "SETTLE FAILED", " ", MSG_TYPE_ERROR);
		else
        {
             vdDisplayErrorMsg2(1, 8, "CONNECT FAILED", " ", MSG_TYPE_ERROR);
             if(fGetECRTransactionFlg())
             {
                  vdSetECRResponse(ECR_COMMS_ERR);
                   //inMultiAP_ECRSendSuccessResponse();
             }
        }
		return(d_NO);
	}

	return d_OK;
}

int inXMSendAndReceiveRequestMessage(void)
{
	int inResult;
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
	BYTE szXMLReceiveTemp[XML_RECV_SIZE+1];
	int iXMLReceiveLen = 0;
	
	vdDebug_LogPrintf("--inXMSendAndReceiveRequestMessage--");
	vdDebug_LogPrintf("fXMLPrint[%d]", fXMLPrint);

	vdCTOS_SetTransType(SALE);
	
	//CTOS_LCDTClearDisplay();
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");
	
	vdDebug_LogPrintf("SSL[%d]",strCPT.fSSLEnable);
	vdDebug_LogPrintf("fShareComEnable[%d]", strTCT.fShareComEnable);
	vdDebug_LogPrintf("inIPHeader[%d]", strCPT.inIPHeader);
	vdDebug_LogPrintf("Primary[%s]port[%ld]",strCPT.szPriTxnHostIP,strCPT.inPriTxnHostPortNum);
	vdDebug_LogPrintf("Secondary[%s]port[%ld]",strCPT.szSecTxnHostIP,strCPT.inSecTxnHostPortNum);
	vdDebug_LogPrintf("fDHCPEnable[%d]", strTCP.fDHCPEnable);
	vdDebug_LogPrintf("szAPN[%s].szUserName[%s].szPassword[%s]", strTCP.szAPN, strTCP.szUserName, strTCP.szPassword);
	
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

		if (fXMLPrint == TRUE && inResult == d_OK)					
			inPrintXMLPacket("TX-XML", srTransRec.szXMLSendData, srTransRec.iXMLSendLen, TRUE); 

		//CTOS_Delay(500);

		if (inResult == d_OK) 
			break;
	
		iCounter++;

		if (iCounter > iLimit)
			break;
		
		iCounter++;
		
	}while (fWait);

	// Return d_NO when connection is terminated -- sidumili
	if (fTerminated) return d_NO;
	
	// Receive XML
	iCounter = 0;
	fReceiveOK = FALSE;
	srTransRec.iXMLReceiveLen = 0;
	srTransRec.fStateTimeOut = FALSE;
	memset(srTransRec.szXMLReceiveData, 0x00, sizeof(srTransRec.szXMLReceiveData));
	
	do
	{
		srTransRec.iXMLReceiveLen = 0;
		inResult = srCommFuncPoint.inRecData(&srTransPara, srTransRec.szXMLReceiveData);
	
		vdDebug_LogPrintf("iCounter[%d].XML inRecData.inResult[%d]", iCounter, inResult);

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
		
		srTransRec.iXMLReceiveLen = inResult; // Length
		
		if (srTransRec.iXMLReceiveLen > 0)
		{
			memset(szXMLData, 0x00, sizeof(szXMLData));
			memcpy((char*)szXMLData, (char*)srTransRec.szXMLReceiveData, srTransRec.iXMLReceiveLen); 		
			put_env_int("ERMMULTISEND",0);
			srTransRec.fStateTimeOut = FALSE;

			if (fXMLDebug == TRUE)
				vdDebug_LogPrintf("iCounter[%d].ReceiveXML.Len[%d.Data[\n%s\n]", iCounter, strlen(srTransRec.szXMLReceiveData), srTransRec.szXMLReceiveData);

			if (fXMLPrint == TRUE && srTransRec.iXMLReceiveLen > 0)
				inPrintXMLPacket("RX-XML", srTransRec.szXMLReceiveData, strlen(srTransRec.szXMLReceiveData), TRUE);

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
		}

		iCounter++;

		if (iCounter > iLimit)
			break;
		
	}while(fWait);

	vdDebug_LogPrintf("fReceiveOK[%d]", fReceiveOK);
	
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

	vdDebug_LogPrintf("inXMSendAndReceiveRequestMessage.inResult[%d]", inResult);
	
	return inResult;
}

int inXMSendAndReceiveRetrieveMessage(BOOL fRetrieveOnce)
{
	int inResult;
	TRANS_DATA_TABLE *srTransPara;
	BYTE szXMLData[XML_RECV_SIZE+1] = {0};
	BYTE szXMLHoldSendData[XML_SEND_SIZE+1] = {0};
	int iHoldSendLen = 0;
	BOOL fWait = TRUE;
	BYTE key;
	int iCounter = 0;
	int iLimit = get_env_int("XMLRETRYLIMIT");
	BOOL fReceiveOK = FALSE;
	BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
	BOOL fXMLDebug = (BOOL)get_env_int("XMLDBG");
	BOOL fTerminated = FALSE;
	BOOL fAutoRetrieve = (BOOL)get_env_int("XMLAUTORETRIEVE");
	BYTE szTemp[50 + 1] = {0};
	BOOL fStateStatus = FALSE;

	vdCTOS_SetTransType(SALE);
	
	vdDebug_LogPrintf("--inXMSendAndReceiveRetrieveMessage--");
	vdDebug_LogPrintf("fXMLPrint[%d]", fXMLPrint);

	if (iLimit <= 0 || !fAutoRetrieve || fRetrieveOnce) iLimit = 1;

	if (!fRetrieveOnce)
	{
		//CTOS_LCDTClearDisplay();
		//vdDispTransTitle(srTransRec.byTransType);
	}
	
	//vdCTOS_DispStatusMessage("PROCESSING");
	
	vdDebug_LogPrintf("SSL[%d]",strCPT.fSSLEnable);
	vdDebug_LogPrintf("fShareComEnable[%d]", strTCT.fShareComEnable);
	vdDebug_LogPrintf("inIPHeader[%d]", strCPT.inIPHeader);
	vdDebug_LogPrintf("Primary[%s]port[%ld]",strCPT.szPriTxnHostIP,strCPT.inPriTxnHostPortNum);
	vdDebug_LogPrintf("Secondary[%s]port[%ld]",strCPT.szSecTxnHostIP,strCPT.inSecTxnHostPortNum);
	vdDebug_LogPrintf("fDHCPEnable[%d]", strTCP.fDHCPEnable);
	vdDebug_LogPrintf("szAPN[%s].szUserName[%s].szPassword[%s]", strTCP.szAPN, strTCP.szUserName, strTCP.szPassword);
	vdDebug_LogPrintf("iLimit[%d].fXMLPrint[%d].fXMLDebug[%d].fAutoRetrieve[%d].fRetrieveOnce[%d]", iLimit, fXMLPrint, fXMLDebug, fAutoRetrieve, fRetrieveOnce);
	
	// Check connection
	inResult = srCommFuncPoint.inConnect(&srTransPara);
	if (inResult != d_OK)
	{
	   // strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
		vdDisplayErrorMsgResp2(" ", "Connect Failed", "Please Try Again");
		vdSetErrorMessage("");
		return d_NO;
	}
	
	put_env_int("ERMMULTISEND",1);

	iCounter = 0;
	fReceiveOK = FALSE;
	srTransRec.iXMLReceiveLen = 0;
	fStateStatus = FALSE;
	srTransRec.fStateTimeOut = FALSE;
	memset(srTransRec.szXMLReceiveData, 0x00, sizeof(srTransRec.szXMLReceiveData));
	do
	{				
		if (fXMLDebug == TRUE)
			vdDebug_LogPrintf("iCounter[%d]\nSendXML.Len[%d].Data[\n%s\n]", iCounter, strlen(srTransRec.szXMLSendData), srTransRec.szXMLSendData);

		// -->> Hold Send Data
		memset(szXMLHoldSendData, 0x00, sizeof(szXMLHoldSendData));
		memcpy(szXMLHoldSendData, srTransRec.szXMLSendData, srTransRec.iXMLSendLen);
		iHoldSendLen = srTransRec.iXMLSendLen;
		// -->> Hold Send Data
		
		inResult = srCommFuncPoint.inSendData(&srTransPara, srTransRec.szXMLSendData, srTransRec.iXMLSendLen);

		vdDebug_LogPrintf("iCounter[%d].XML inSendData[%d]", iCounter, inResult);

		if (inResult != d_OK)
		{
		    //strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
			vdDisplayErrorMsgResp2(" ", "Connect Failed", "Please Try Again");
			vdSetErrorMessage("");
			fTerminated = TRUE;
			break;
		}
		
		if (fXMLPrint == TRUE && inResult == d_OK)
			inPrintXMLPacket("TX-XML", srTransRec.szXMLSendData, srTransRec.iXMLSendLen, FALSE);

		srTransRec.iXMLReceiveLen = 0;					
		inResult = srCommFuncPoint.inRecData(&srTransPara, srTransRec.szXMLReceiveData);

		vdDebug_LogPrintf("Final Receive:::iCounter[%d].XML inRecData.inResult[%d]", iCounter, inResult);

		if (inResult == -7)
		{
		   // strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
			vdDisplayErrorMsgResp2(" Connection ","Terminated","Please Try Again");
			vdSetErrorMessage("");
			fTerminated = TRUE;
			break;
		}

		srTransRec.iXMLReceiveLen = inResult; // Length
		
		if (srTransRec.iXMLReceiveLen > 0)
		{
			memset(szXMLData, 0x00, sizeof(szXMLData));
			memcpy((char*)szXMLData, (char*)srTransRec.szXMLReceiveData, srTransRec.iXMLReceiveLen);
			put_env_int("ERMMULTISEND",0);

			if (fXMLDebug == TRUE)
				vdDebug_LogPrintf("iCounter[%d].ReceiveXML.Len[%d.Data[\n%s\n]", iCounter, strlen(srTransRec.szXMLReceiveData), srTransRec.szXMLReceiveData);

			if (fXMLPrint == TRUE && srTransRec.iXMLReceiveLen > 0)
				inPrintXMLPacket("RX-XML", srTransRec.szXMLReceiveData, strlen(srTransRec.szXMLReceiveData), FALSE);		

			if ((fXMLBadRequestMessage((char*)szXMLData) != d_OK) ||
			   (fXMLForbiddenMessage((char*)szXMLData) != d_OK) ||
			   (fXMLNotOKMessage((char*)szXMLData) != d_OK) ||
			   (iXMLCheckResponseStatus((char*)szXMLData) != d_OK) ||
			   (iXMLCheckResponseResultCode((char*)szXMLData) != d_OK)) 		   
			{
				fReceiveOK = FALSE;
				break;
			}

			vdDebug_LogPrintf("srTransRec.byVoided=[%d]", srTransRec.byVoided);
			if (srTransRec.byVoided) // to be check
			{
				if (iXMLCheckResponseRefundStatus((char*)szXMLData) != d_OK)
					fStateStatus = FALSE;
				else
					fStateStatus = TRUE;
			}
			else
			{
				if (iXMLCheckTradeState((char*)szXMLData) != d_OK)				
					fStateStatus = FALSE;
				else
					fStateStatus = TRUE;
				
			}
			
			// Check Trade State
			//if (iXMLCheckTradeState((char*)szXMLData) != d_OK)
			if (!fStateStatus)
			{
				fReceiveOK = FALSE;
				break;
			}
			else
			{
				// Success Receive
				vdDebug_LogPrintf("srTransRec.fXMLTradeStateOK[%d]", srTransRec.fXMLTradeStateOK);
				if (srTransRec.fXMLTradeStateOK == TRUE)
				{
					if (srTransRec.byVoided) // to be check
					{
						vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, (char*)szXMLData, TAG_OUT_TRADE_NO);
						vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, (char*)szXMLData, TAG_OUT_TRANSACTION_ID);
						//vXMLParseAndStoreToCollection(&srResponse, XML_PARSE_STRING, (char*)szXMLData, TAG_REFUND_FEE);
	
					}
					else
					{
						iXMLAnalyzeResponse((char*)szXMLData);
					}
					strcpy(srTransRec.szRespCode,"00");
					fReceiveOK = TRUE;
					break;
				}
				else
				{
					memset(srTransRec.szXMLSendData, 0x00, sizeof(srTransRec.szXMLSendData));
					memcpy(srTransRec.szXMLSendData, szXMLHoldSendData, iHoldSendLen);
					srTransRec.iXMLSendLen = iHoldSendLen;
					//vdCTOS_DispStatusMessage("PROCESSING");
				}
			}						
		}					
	
		iCounter++; 	

		if (fXMLDebug == TRUE)
		{
			memset(szTemp, 0x00, sizeof(szTemp));
			sprintf(szTemp, "Retry Counter[%d/%d]", iCounter, iLimit);
			CTOS_LCDTPrintAligned(7, szTemp, d_LCD_ALIGNLEFT);
			CTOS_Delay(1000);
		}

		if (iCounter % 2 == 0 && iLimit > 2)//Fix for issue where BROKEN PIPE is returned by SHARLS_COM on 3rd transaction.
		{
			vdDebug_LogPrintf("INSIDE FIX");
			inCTOS_inDisconnect();

			inXML_CommsReInit();
			inResult = srCommFuncPoint.inConnect(&srTransPara);
			if (inResult != d_OK)
			{
			   // strcpy(srTransRec.szECRRespCode, ECR_COMMS_ERR);//for ecr
				vdDisplayErrorMsgResp2(" ", "Connect Failed", "Please Try Again");
				vdSetErrorMessage("");
				return d_NO;
			}

			put_env_int("ERMMULTISEND",1);
			fReceiveOK = FALSE;
			srTransRec.iXMLReceiveLen = 0;
			memset(srTransRec.szXMLReceiveData, 0x00, sizeof(srTransRec.szXMLReceiveData));
			fStateStatus = FALSE;
			srTransRec.fStateTimeOut = FALSE;
		}
		
		if (iCounter >= iLimit)
			break;

		CTOS_Delay(5000); // Delay in seconds for next request
		
	}while (fWait);

	vdDebug_LogPrintf("fReceiveOK[%d]", fReceiveOK);
	
	if (fReceiveOK)
	{
		inResult = d_OK;
	}
	else
	{
	    //strcpy(srTransRec.szECRRespCode, ECR_QR_ERROR);//for ecr
		inResult = d_NO;
	}

	if (fAutoRetrieve && iCounter >= iLimit)
	{
		iXMLDisplayResponse(srTransRec.szXMLtrade_state, 16);
	}
	else
	{
		if (srTransRec.iXMLReceiveLen <= 0 && !fTerminated)
		{
		    //strcpy(srTransRec.szECRRespCode, ECR_TIMEOUT_RESP);//for ecr
		    srTransRec.fStateTimeOut = TRUE;
			vdDisplayMessage("", "", "NO RESPONSE FROM HOST");		
			CTOS_Beep();
			CTOS_Delay(3000);
		}
	}
			
	inCTOS_inDisconnect();

	vdDebug_LogPrintf("inXMSendAndReceiveRetrieveMessage.inResult[%d]", inResult);
	
	return inResult;
}


int inXML_CommsReInit(void)//Same as inXML_CommsReInit but removed clearing of display.
{
	int inRetVal; 	

	vdDebug_LogPrintf("--inXML_CommsInit--");	
	vdDebug_LogPrintf("HDTid[%d]", srTransRec.HDTid);
	vdDebug_LogPrintf("inCommunicationMode[%d]", strCPT.inCommunicationMode);

	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING...");
	
	inCPTRead(srTransRec.HDTid);			
	strCPT.fSSLEnable = TRUE;
	strCPT.inIPHeader = NO_HEADER_LEN;
	
	srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
	
	vdDebug_LogPrintf("usTerminalCommunicationMode[%d]", srTransRec.usTerminalCommunicationMode);
	vdDebug_LogPrintf("SSL[%d]",strCPT.fSSLEnable);
	vdDebug_LogPrintf("fDHCPEnable[%d]",strTCP.fDHCPEnable);
	vdDebug_LogPrintf("fShareComEnable[%d]",strTCT.fShareComEnable);
	vdDebug_LogPrintf("Primary[%s]port[%ld]",strCPT.szPriTxnHostIP,strCPT.inPriTxnHostPortNum);
	vdDebug_LogPrintf("Secondary[%s]port[%ld]",strCPT.szSecTxnHostIP,strCPT.inSecTxnHostPortNum);

	vdSetInit_Connect(1);
	
    if ((strCPT.inCommunicationMode == GPRS_MODE) && strCPT.fSSLEnable == TRUE)
	    vdSetInit_Connect(1); // sidumili: set to 1 to support gprs connection

	if ((strCPT.inCommunicationMode == ETHERNET_MODE) && strCPT.fSSLEnable == TRUE)
		vdSetInit_Connect(0);
		
    if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK)
    {
		vdDisplayErrorMsgResp2("","Initialization","Error");
		vdSetErrorMessage("");
		vdDebug_LogPrintf("Initialization error");
        return(d_NO);
    }

    inRetVal = inCTOS_CheckInitComm(srTransRec.usTerminalCommunicationMode);

	vdDebug_LogPrintf("saturn inCTOS_CheckInitComm | inReturn[%d]|fGPRSConnectOK[%d]", inRetVal, fGPRSConnectOK);

	if (inRetVal != d_OK)
	{
		switch (srTransRec.usTerminalCommunicationMode )
		{
			case GPRS_MODE:
				//vdDisplayMultiLineMsgAlign("", "GPRS PROBLEM", "PLESE TRY AGAIN", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "GPRS PROBLEM", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
			case WIFI_MODE:
				//vdDisplayMultiLineMsgAlign("", "WIFI PROBLEM", "PLESE TRY AGAIN", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "WIFI PROBLEM", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
			case ETHERNET_MODE:
				//vdDisplayMultiLineMsgAlign("", "ETHERNET PROBLEM", "PLESE TRY AGAIN", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "ETHERNET PROBLEM", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
			default:
				//vdDisplayMultiLineMsgAlign("", "COMM INIT", "ERROR", DISPLAY_POSITION_CENTER);
				vdDisplayErrorMsg2(1, 8, "COMM INIT", "PLESE TRY AGAIN", MSG_TYPE_ERROR);
				break;
		}

		vdSetErrorMessage("");
		if(srTransRec.byTransType == SETTLE)
			vdDisplayErrorMsg2(1, 8, "SETTLE FAILED", " ", MSG_TYPE_ERROR);
		else
			vdDisplayErrorMsg2(1, 8, "CONNECT FAILED", " ", MSG_TYPE_ERROR);
	
		return(d_NO);
	}

	return d_OK;
}

void iSetDummyFieldForERM(void)
{	
	// Get PAN
	strcpy(srTransRec.szPAN, strDigitalWallet.szPAN);
	
	// Get Invoice No (TCT)
	memcpy(srTransRec.szInvoiceNo, strTCT.szInvoiceNo, INVOICE_BCD_SIZE);
		
	// Get Batch No	
	memcpy(srTransRec.szBatchNo, strMMT[0].szBatchNo, BATCH_NO_BCD_SIZE);

	// RRN
	strcpy(srTransRec.szRRN, "000000000000");

	// AuthCode
	strcpy(srTransRec.szAuthCode, "000000");

	// Host Label
	strcpy(srTransRec.szHostLabel, strHDT.szHostLabel);

	DebugAddHEX("szBatchNo", srTransRec.szBatchNo, 3);
	DebugAddHEX("szInvoiceNo", srTransRec.szInvoiceNo, 3);
	vdDebug_LogPrintf("srTransRec.HDTid=[%d], srTransRec.MITid=[%d]", srTransRec.HDTid, srTransRec.MITid);
	vdDebug_LogPrintf("srTransRec.szRRN=[%s]", srTransRec.szRRN);
	vdDebug_LogPrintf("srTransRec.szAuthCode=[%s]", srTransRec.szAuthCode);
	vdDebug_LogPrintf("srTransRec.szHostLabel=[%s]", srTransRec.szHostLabel);
	vdDebug_LogPrintf("srTransRec.szPAN=[%s]", srTransRec.szPAN);
	vdDebug_LogPrintf("strDigitalWallet.szPAN=[%s]", strDigitalWallet.szPAN);
}

int inXMSendAndReceiveVoidMessage(void)
{
	int inResult;
	TRANS_DATA_TABLE *srTransPara;
	BYTE szXMLData[XML_RECV_SIZE+1] = {0};
	BYTE szXMLHoldSendData[XML_SEND_SIZE+1] = {0};
	int iHoldSendLen = 0;
	BOOL fWait = TRUE;
	BYTE key;
	int iCounter = 0;
	int iLimit = get_env_int("XMLRETRYLIMIT");
	BOOL fReceiveOK = FALSE;
	BOOL fXMLPrint = (BOOL)get_env_int("XMLPRINT");
	BOOL fXMLDebug = (BOOL)get_env_int("XMLDBG");
	BOOL fTerminated = FALSE;
	BOOL fAutoRetrieve = (BOOL)get_env_int("XMLAUTORETRIEVE");
	BYTE szTemp[50 + 1] = {0};
	BOOL fXMLEnd = FALSE;
	BOOL iReceiveCounter = 0;
	BYTE szXMLReceiveTemp[XML_RECV_SIZE+1];
	int iXMLReceiveLen = 0;
	BOOL fTagStatusEnd = FALSE;
	
	//vdCTOS_SetTransType(SALE);
	vdCTOS_SetTransType(VOID);
	
	vdDebug_LogPrintf("--inXMSendAndReceiveVoidMessage--");
	vdDebug_LogPrintf("fXMLPrint[%d]", fXMLPrint);

	if (iLimit <= 0)
		iLimit = 1;

	//CTOS_LCDTClearDisplay();
	//vdDispTransTitle(srTransRec.byTransType);
		
	//vdCTOS_DispStatusMessage("PROCESSING");
	
	vdDebug_LogPrintf("SSL[%d]",strCPT.fSSLEnable);
	vdDebug_LogPrintf("fShareComEnable[%d]", strTCT.fShareComEnable);
	vdDebug_LogPrintf("inIPHeader[%d]", strCPT.inIPHeader);
	vdDebug_LogPrintf("Primary[%s]port[%ld]",strCPT.szPriTxnHostIP,strCPT.inPriTxnHostPortNum);
	vdDebug_LogPrintf("Secondary[%s]port[%ld]",strCPT.szSecTxnHostIP,strCPT.inSecTxnHostPortNum);
	vdDebug_LogPrintf("fDHCPEnable[%d]", strTCP.fDHCPEnable);
	vdDebug_LogPrintf("szAPN[%s].szUserName[%s].szPassword[%s]", strTCP.szAPN, strTCP.szUserName, strTCP.szPassword);
	vdDebug_LogPrintf("iLimit[%d].fXMLPrint[%d].fXMLDebug[%d].fAutoRetrieve[%d]", iLimit, fXMLPrint, fXMLDebug, fAutoRetrieve);
	
	// Check connection
	inResult = srCommFuncPoint.inConnect(&srTransPara);
	if (inResult != d_OK)
	{
		vdDisplayErrorMsgResp2(" ", "Connect Failed", "Please Call");
		vdSetErrorMessage("");
		return d_NO;
	}
	
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

		if (fXMLPrint == TRUE && inResult == d_OK)					
			inPrintXMLPacket("TX-XML", srTransRec.szXMLSendData, srTransRec.iXMLSendLen, TRUE); 

		CTOS_Delay(500);

		if (inResult == d_OK) 
			break;
	
		iCounter++;

		if (iCounter > iLimit)
			break;
		
		iCounter++;
		
	}while (fWait);

	// Return d_NO when connection is terminated -- sidumili
	if (fTerminated) return d_NO;
	
	// Receive XML
	iCounter = 0;
	fReceiveOK = FALSE;
	srTransRec.iXMLReceiveLen = 0;
	memset(srTransRec.szXMLReceiveData, 0x00, sizeof(srTransRec.szXMLReceiveData));
	do
	{
		srTransRec.iXMLReceiveLen = 0;
		inResult = srCommFuncPoint.inRecData(&srTransPara, srTransRec.szXMLReceiveData);		
		vdDebug_LogPrintf("iCounter[%d].XML inRecData.inResult[%d]", iCounter, inResult);

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
			iXMLReceiveLen = inResult;
			//vdDisplayErrorMsg(1, 8, "MISSING </XML>. IGNORE");
			vdDebug_LogPrintf("MISSING </XML>");
			
			memset(szXMLReceiveTemp, 0x00, sizeof(szXMLReceiveTemp));
			inResult = srCommFuncPoint.inRecData(&srTransPara, szXMLReceiveTemp);
			vdDebug_LogPrintf("Receive Again::iCounter[%d].ReceiveXML.Len[%d.Data[\n%s\n]", iCounter, strlen(szXMLReceiveTemp), szXMLReceiveTemp);

			fTagStatusEnd = fValidTagStatusEnd(srTransRec.szXMLReceiveData);
			vdDebug_LogPrintf("fTagStatusEnd=[%d]", fTagStatusEnd);
			if (!fTagStatusEnd)
			{
				strcat(srTransRec.szXMLReceiveData, ">");
			}			
			strcat(srTransRec.szXMLReceiveData, szXMLReceiveTemp);
			iXMLReceiveLen += inResult;
			inResult = iXMLReceiveLen;			
		}
		
		srTransRec.iXMLReceiveLen = inResult; // Length
		
		if (srTransRec.iXMLReceiveLen > 0)
		{
			memset(szXMLData, 0x00, sizeof(szXMLData));
			memcpy((char*)szXMLData, (char*)srTransRec.szXMLReceiveData, srTransRec.iXMLReceiveLen); 		
			put_env_int("ERMMULTISEND",0);

			if (fXMLDebug == TRUE)
				vdDebug_LogPrintf("iCounter[%d].ReceiveXML.Len[%d.Data[\n%s\n]", iCounter, strlen(srTransRec.szXMLReceiveData), srTransRec.szXMLReceiveData);

			if (fXMLPrint == TRUE && srTransRec.iXMLReceiveLen > 0)
				inPrintXMLPacket("RX-XML", srTransRec.szXMLReceiveData, strlen(srTransRec.szXMLReceiveData), TRUE);

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
				iXMLCheckResponseOutRefundNo((char*)szXMLData); // Get Refund No
				iXMLCheckResponseOutRefundID((char*)szXMLData); // Get Refund ID
				break;
			}							
		}

		iCounter++;

		if (iCounter > iLimit)
			break;
		
	}while(fWait);

	vdDebug_LogPrintf("fReceiveOK[%d]", fReceiveOK);
	
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
		vdDisplayMessage("No Response", "from Bank", "Please Try Again");		
		CTOS_Beep();
		CTOS_Delay(3000);
	}
	
	inCTOS_inDisconnect();

	vdDebug_LogPrintf("inXMSendAndReceiveRetrieveMessage.inResult[%d]", inResult);
	
	return inResult;
}

void szGetDigitalWalletTransTitle(BYTE byTransType, BYTE *szTitle)
{    
    int i;
    szTitle[0] = 0x00;
	BYTE szTemp[10 + 1] = {0};

	strcpy(szTemp, (srTransRec.inQRPresented == MPQR_TYPE ? "MP" : "CP"));
    
    switch(byTransType)
    {
		case SALE:
			if (srTransRec.HDTid == WECHAT_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "WECHAT QRPAY", szTemp);
			else if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "ALIPAY QRPAY", szTemp);
			else if (srTransRec.HDTid == GCASH_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "GCASH QRPAY", szTemp);
			else if (srTransRec.HDTid == GRAB_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "GRAB QRPAY", szTemp);
			else if (srTransRec.HDTid == UPI_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "UPI QRPAY", szTemp);
			else
				strcpy(szTitle, "SALE");
            break;       
        case VOID:
            if (srTransRec.HDTid == WECHAT_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "WECHAT QRPAY VOID", szTemp);
			else if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "ALIPAY QRPAY VOID", szTemp);
			else if (srTransRec.HDTid == GCASH_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "GCASH QRPAY VOID", szTemp);
			else if (srTransRec.HDTid == GRAB_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "GRAB QRPAY VOID", szTemp);
			else if (srTransRec.HDTid == UPI_HOST_INDEX)
				sprintf(szTitle, "%s-%s", "UPI QRPAY VOID", szTemp);
			else
				strcpy(szTitle, "VOID");
            break;        					
        default:
            strcpy(szTitle, "");
            break;
    }
    i = strlen(szTitle);
    szTitle[i]=0x00;
	
    return ;
}

BOOL fValidXMLEnd(char* szXMLData)
{
	char WorkBuffer[XML_SEND_SIZE];
	char *Startptr;
	BOOL fFound = FALSE;

	do
	{
		memset(WorkBuffer, 0x00, sizeof(WorkBuffer));
    	Startptr = strstr(szXMLData, szTagXMLEnd);

		if (Startptr != NULL)
		{
			fFound = TRUE;
			break;
		}

	}
	while (Startptr != NULL);

	return fFound;
}

BOOL fValidTagStatusEnd(char* szXMLData)
{
	char WorkBuffer[XML_SEND_SIZE];
	char *Startptr;
	BOOL fFound = FALSE;

	do
	{
		memset(WorkBuffer, 0x00, sizeof(WorkBuffer));
		Startptr = strstr(szXMLData, szTagStatusEnd);

		if (Startptr != NULL)
		{
			fFound = TRUE;
			break;
		}

	}
	while (Startptr != NULL);

	return fFound;
}

int inXMSendAndReceiveCloseQRCodeMessage(void)
{
	int inResult;
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
	BYTE szXMLReceiveTemp[XML_RECV_SIZE+1];
	int iXMLReceiveLen = 0;
	
	vdDebug_LogPrintf("--inXMSendAndReceiveCloseQRCodeMessage--");
	vdDebug_LogPrintf("fXMLPrint[%d]", fXMLPrint);

	vdCTOS_SetTransType(SALE);
	
	//CTOS_LCDTClearDisplay();
	//vdDispTransTitle(srTransRec.byTransType);
	//vdCTOS_DispStatusMessage("PROCESSING");
	
	vdDebug_LogPrintf("SSL[%d]",strCPT.fSSLEnable);
	vdDebug_LogPrintf("fShareComEnable[%d]", strTCT.fShareComEnable);
	vdDebug_LogPrintf("inIPHeader[%d]", strCPT.inIPHeader);
	vdDebug_LogPrintf("Primary[%s]port[%ld]",strCPT.szPriTxnHostIP,strCPT.inPriTxnHostPortNum);
	vdDebug_LogPrintf("Secondary[%s]port[%ld]",strCPT.szSecTxnHostIP,strCPT.inSecTxnHostPortNum);
	vdDebug_LogPrintf("fDHCPEnable[%d]", strTCP.fDHCPEnable);
	vdDebug_LogPrintf("szAPN[%s].szUserName[%s].szPassword[%s]", strTCP.szAPN, strTCP.szUserName, strTCP.szPassword);
	
	// Check connection
	inResult = srCommFuncPoint.inConnect(&srTransPara);
	if (inResult != d_OK)
	{
		//CTOS_Delay(500);
		if(inResult == GPRS_FAIL)
			vdDisplayErrorMsgResp2(" ", "GPRS Problem", "Please Try Again");
		else if(inResult == ETHERNET_FAIL)
			vdDisplayErrorMsgResp2(" ", "Check LAN","Connectivity");
		else if(inResult == WIFI_FAIL)
			vdDisplayErrorMsgResp2(" ", "WiFi Problem", "Please Try Again");
		else    
			vdDisplayErrorMsgResp2(" ", "Connect Failed", "Please Try Again");
		
		vdSetErrorMessage("");
		return d_NO;
	}
		
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

		if (fXMLPrint == TRUE && inResult == d_OK)					
			inPrintXMLPacket("TX-XML", srTransRec.szXMLSendData, srTransRec.iXMLSendLen, TRUE); 

		//CTOS_Delay(500);

		if (inResult == d_OK) 
			break;
	
		iCounter++;

		if (iCounter > iLimit)
			break;
		
		iCounter++;
		
	}while (fWait);

	// Return d_NO when connection is terminated -- sidumili
	if (fTerminated) return d_NO;
	
	// Receive XML
	iCounter = 0;
	fReceiveOK = FALSE;
	srTransRec.iXMLReceiveLen = 0;
	memset(srTransRec.szXMLReceiveData, 0x00, sizeof(srTransRec.szXMLReceiveData));
	do
	{
		srTransRec.iXMLReceiveLen = 0;
		inResult = srCommFuncPoint.inRecData(&srTransPara, srTransRec.szXMLReceiveData);
	
		vdDebug_LogPrintf("iCounter[%d].XML inRecData.inResult[%d]", iCounter, inResult);

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
		
		srTransRec.iXMLReceiveLen = inResult; // Length
		
		if (srTransRec.iXMLReceiveLen > 0)
		{
			memset(szXMLData, 0x00, sizeof(szXMLData));
			memcpy((char*)szXMLData, (char*)srTransRec.szXMLReceiveData, srTransRec.iXMLReceiveLen); 		
			put_env_int("ERMMULTISEND",0);

			if (fXMLDebug == TRUE)
				vdDebug_LogPrintf("iCounter[%d].ReceiveXML.Len[%d.Data[\n%s\n]", iCounter, strlen(srTransRec.szXMLReceiveData), srTransRec.szXMLReceiveData);

			if (fXMLPrint == TRUE && srTransRec.iXMLReceiveLen > 0)
				inPrintXMLPacket("RX-XML", srTransRec.szXMLReceiveData, strlen(srTransRec.szXMLReceiveData), TRUE);

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
		}

		iCounter++;

		if (iCounter > iLimit)
			break;
		
	}while(fWait);

	vdDebug_LogPrintf("fReceiveOK[%d]", fReceiveOK);
	
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
	}
	
	inCTOS_inDisconnect();

	vdDebug_LogPrintf("inXMSendAndReceiveRequestMessage.inResult[%d]", inResult);
	
	return inResult;
}

int iXMLCaptureWechatQRCode(void)
{	
	BYTE key;
	USHORT res;
	CTOS_QRCODE_INFO qrcodeInfo;	

	vdDebug_LogPrintf("--iXMLCaptureWechatQRCode--");
	vdDebug_LogPrintf("srTransRec.szXMLcode_url");

	vdCTOSS_PrinterStart(100);
	
	qrcodeInfo.InfoVersion = QR_INFO_VERSION;
	qrcodeInfo.Size = 6;
	qrcodeInfo.Version = QR_VERSION21X21;
	qrcodeInfo.Level = QR_LEVEL_L;

	vdCTOS_PrinterFline(1);
	
	//vdCTOSS_PrinterBufferQRCode(&qrcodeInfo, "weixin://wxpay/bizpayurl?pr=4RWYcPh", 100, 60);
	vdCTOSS_PrinterBufferQRCode(&qrcodeInfo, srTransRec.szXMLcode_url, 100, 60);

	vdCTOSS_PrinterEnd_CConvert2BMP("/data/data/pub/QRCode_BMP.bmp");
	
	vdCTOSS_PrinterEnd();
		
	return d_OK;	
}

int iGetXMLEndIndex(char* szXMLData)
{
	char WorkBuffer[XML_SEND_SIZE];
	char *Startptr;
	BOOL fFound = FALSE;
	int iPosition = 0;

	Startptr = strstr(szXMLData, szTagXMLEnd);
	iPosition = Startptr - szXMLData;
	
	return iPosition;
}

void iGeTotalFormattedAmount(char* szFormattedAmt)
{
	char szTemp[d_LINE_SIZE + 1] = {0};
	char szTemp4[d_LINE_SIZE + 1] = {0};

	vdDebug_LogPrintf("--iGeTotalFormattedAmount--");
	memset(szFormattedAmt, 0x00, sizeof(szFormattedAmt));
	
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	memset(szFormattedAmt, 0x00, sizeof(szFormattedAmt));
	wub_hex_2_str(srTransRec.szBaseAmount, szTemp, AMT_BCD_SIZE);
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTemp4);

	if (strlen(srTransRec.szXMLlocal_fee_type) > 0)
		sprintf(szFormattedAmt, "TOTAL AMOUNT: %s%s", srTransRec.szXMLlocal_fee_type, szTemp4);
	else
		sprintf(szFormattedAmt, "TOTAL AMOUNT: %s%s", strCST.szCurSymbol, szTemp4);

	vdDebug_LogPrintf("szFormattedAmt=[%s]", szFormattedAmt);
}

void iGetConvertedAmount(char* szFormattedAmt)
{
	char szTemp[d_LINE_SIZE + 1] = {0};
	char szTemp4[d_LINE_SIZE + 1] = {0};

	vdDebug_LogPrintf("--iGetConvertedAmount--");
	memset(szFormattedAmt, 0x00, sizeof(szFormattedAmt));
	
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	memset(szFormattedAmt, 0x00, sizeof(szFormattedAmt));
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", srTransRec.szXMLcash_fee, szTemp4);
	sprintf(szFormattedAmt, "%s%s", srTransRec.szXMLlocal_fee_type, szTemp4);

	vdDebug_LogPrintf("szFormattedAmt=[%s]", szFormattedAmt);
}

int iRetrieveReadData(void)
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
	
	vdDebug_LogPrintf("--iRetrieveReadData--");

	inBatchNumOfRecord = inRetrieveBatchNumRecord();
	vdDebug_LogPrintf("inRetrieveBatchNumRecord, inBatchNumOfRecord=[%d]", inBatchNumOfRecord);

	if (inBatchNumOfRecord > 0)
	{
		vdDeleteFile(TRANS_DETAIL_FILE); // Delete file
		memset(strWriteFile.szWriteData, 0x00, sizeof(strWriteFile.szWriteData));
		memset(strWriteFile.szWriteTemp, 0x00, sizeof(strWriteFile.szWriteTemp));
		strWriteFile.inWriteSize = 0;
	
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
			vdDebug_LogPrintf("szXMLout_trade_no=[%s]", srRetrieveRec.szXMLout_trade_no);
			vdDebug_LogPrintf("szXMLout_refund_no=[%s]", srRetrieveRec.szXMLout_refund_no);
			vdDebug_LogPrintf("szXMLout_transaction_id=[%s]", srRetrieveRec.szXMLout_transaction_id);
			vdDebug_LogPrintf("szXMLtransaction_id=[%s]", srRetrieveRec.szXMLtransaction_id);
			vdDebug_LogPrintf("szXMLcode_url=[%s]", srRetrieveRec.szXMLcode_url);
			vdDebug_LogPrintf("szXMLtime_end=[%s]", srRetrieveRec.szXMLtime_end);			
			vdDebug_LogPrintf("byVoided=[%d]", srRetrieveRec.byVoided);
			vdDebug_LogPrintf("srTransRec.byVoided=[%d]", srTransRec.byVoided);

			//memcpy(srTransRec.szDate, srRetrieveRec.szDate, DATE_BCD_SIZE); //For retrieve list date

			fVoided = isAlreadyVoided(srRetrieveRec.szInvoiceNo);
			
			/*if (srRetrieveRec.byVoided == 49) // Voided
				fVoided = TRUE;
			else
				fVoided = FALSE;
			*/
			// Amount			
			memset(szAmount, 0x00, sizeof(szAmount));
			memset(szTempAmount, 0x00, sizeof(szTempAmount));
			vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", srRetrieveRec.szXMLlocal_total_fee, szTempAmount);

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

			// Write to file
			if (srRetrieveRec.HDTid > 0)
			{
				memset(strWriteFile.szWriteTemp, 0x00, sizeof(strWriteFile.szWriteTemp));
				memset(szID, 0x00, sizeof(szID));
				memset(szDummy, 0x00, sizeof(szDummy));
				sprintf(szID, "%d|%d", srRetrieveRec.HDTid, srRetrieveRec.IITid);
				strcpy(szDummy, " ");

				memset(szTransDataid, 0x00, sizeof(szTransDataid));
				sprintf(szTransDataid, "%d", srRetrieveRec.TransDataid);

				// Format End DateTime
				memset(srTransRec.szXMLtime_end, 0x00, sizeof(srTransRec.szXMLtime_end));
				strcpy(srTransRec.szXMLtime_end, srRetrieveRec.szXMLtime_end);

				memset(szFormattedDateTime, 0x00, sizeof(szFormattedDateTime));
				vdXMLFormatDateEndTime(szFormattedDateTime);
				vdDebug_LogPrintf("i[%d]::szTransDataid=[%s], szFormattedDateTime=[%s]", i, szTransDataid, szFormattedDateTime);
				
				strcpy(strWriteFile.szWriteTemp, szID);
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);

				if (srRetrieveRec.HDTid == WECHAT_HOST_INDEX)
					strcat(strWriteFile.szWriteTemp, "WECHAT");
				else if (srRetrieveRec.HDTid == ALIPAY_HOST_INDEX)
					strcat(strWriteFile.szWriteTemp, "ALIPAY");
				else if (srRetrieveRec.HDTid == GCASH_HOST_INDEX)
					strcat(strWriteFile.szWriteTemp, "GCASH");
				else if (srRetrieveRec.HDTid == GRAB_HOST_INDEX)
					strcat(strWriteFile.szWriteTemp, "GRAB");
				else if (srRetrieveRec.HDTid == UPI_HOST_INDEX)
					strcat(strWriteFile.szWriteTemp, "UPI");
				else if (srRetrieveRec.HDTid == PHQR_HOST_INDEX)
					strcat(strWriteFile.szWriteTemp, "QRPH");
									
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, srRetrieveRec.szXMLout_trade_no);

				vdDebug_LogPrintf("testlang14 %s", srRetrieveRec.szXMLtransaction_id);
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, szTransDataid);
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, szInvoiceNo);
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);

				// sidumili: for qr presented indentification
				memset(szQRPresented, 0x00, sizeof(szQRPresented));

				
				if (srTransRec.HDTid == PHQR_HOST_INDEX)
					sprintf(szQRPresented, "QRPH-RETRIEVE");
			    else
					sprintf(szQRPresented, "%s-%s",(srRetrieveRec.inQRPresented == MPQR_TYPE ? "MPQR" : "CPQR"), (srTransRec.byTransType == PRE_AUTH ? "RETRIEVE" : "SALE"));
				vdDebug_LogPrintf("szQRPresented=[%s]",szQRPresented);
				
				//strcat(strWriteFile.szWriteTemp, (srTransRec.byTransType == PRE_AUTH ? "RETRIEVE" : "SALE"));
				strcat(strWriteFile.szWriteTemp, szQRPresented);
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);

                
				if (srTransRec.HDTid == PHQR_HOST_INDEX){
					strcat(strWriteFile.szWriteTemp,szInvoiceNo);
				}else{
					if (fVoided)
						strcat(strWriteFile.szWriteTemp, (strlen(srRetrieveRec.szXMLout_refund_no) > 0 ? srRetrieveRec.szXMLout_refund_no : "-"));
					else
						strcat(strWriteFile.szWriteTemp, (strlen(srRetrieveRec.szXMLout_transaction_id) > 0 ? srRetrieveRec.szXMLout_transaction_id : "-"));
				}
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, (srRetrieveRec.inQRPresented == MPQR_TYPE ? "MPQR" : "CPQR"));
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, szFormattedDateTime);
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, szAmount);					
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, (fVoided ? "VOIDED" : "-"));
				strcat(strWriteFile.szWriteTemp, DELIM_PIPE);
				strcat(strWriteFile.szWriteTemp, "\n");

				strWriteFile.inWriteSize += strlen(strWriteFile.szWriteTemp);

				strcat(strWriteFile.szWriteData, strWriteFile.szWriteTemp);
				
				vdDebug_LogPrintf("inWriteSize=[%d] szWriteTemp=[%s]", strWriteFile.inWriteSize, strWriteFile.szWriteTemp);
				
			}
			
			
		}

		 free(pinTransDataid);

		 // Save to file
		inSaveFile((char*)strWriteFile.szWriteData, strWriteFile.inWriteSize, TRANS_DETAIL_FILE);
	}
	else
	{
		vdDebug_LogPrintf("No retrieve record found...");
		vdDisplayMessageBox(1, 8, "NO RECORD FOUND", "FOR RETRIEVE LIST", "", MSG_TYPE_INFO);
		CTOS_Beep();
		CTOS_Delay(1000);
	
		return d_NO;
	}
	
	return d_OK;
}

int iRetrieveSaveData(void)
{
	int inResult = d_NO;
	RETRIEVE_DATA_TABLE sLocalData;
	CTOS_RTC SetRTC;
    BYTE szCurrentDate[20];
	BYTE szCurrentTime[20];
	BYTE szDate[DATE_BCD_SIZE+1];     /* BCD MMDD        */
    BYTE szTime[TIME_BCD_SIZE+1];     /* BCD hhmmss        */
	BYTE szCurrentDateTime[40];

	vdDebug_LogPrintf("--iRetrieveSaveData-- 1");
	
	if (!strGBLVar.fGBLvRetrieveList)
		return d_OK;
	
	vdDebug_LogPrintf("--iRetrieveSaveData--");

	CTOS_RTCGet(&SetRTC);
    memset(szCurrentDate, 0x00, sizeof(szCurrentDate));
	memset(szDate, 0x00, sizeof(szDate));
	memset(szTime, 0x00, sizeof(szTime));

	//20200421125207 -->> sample time_end
	//2020- year, 04-month, 21-day, 125207 - hhmmss
	
    sprintf(szCurrentDate,"20%02d%02d%02d",SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay);
    //wub_str_2_hex(szCurrentDate,szDate,DATE_ASC_SIZE);
    
    memset(szCurrentTime, 0x00, sizeof(szCurrentTime));
    sprintf(szCurrentTime,"%02d%02d%02d", SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
    //wub_str_2_hex(szCurrentTime,szTime,TIME_ASC_SIZE);

	memset(szCurrentDateTime, 0x00, sizeof(szCurrentDateTime));
	sprintf(szCurrentDateTime, "%s%s", szCurrentDate, szCurrentTime);

	vdCTOS_SetDateTime();
	
	memset(&sLocalData, 0x00, sizeof(RETRIEVE_DATA_TABLE));

	sLocalData.HDTid = srTransRec.HDTid;
	sLocalData.MITid = srTransRec.MITid;
	sLocalData.IITid = srTransRec.IITid;
	strcpy(sLocalData.szHostLabel, srTransRec.szHostLabel);
	strcpy(sLocalData.szIssuerLabel, srTransRec.szIssuerLabel);
	memcpy(sLocalData.szTotalAmount, srTransRec.szTotalAmount, AMT_BCD_SIZE);
	memcpy(sLocalData.szBaseAmount, srTransRec.szBaseAmount, AMT_BCD_SIZE);

	DebugAddHEX("AAA - srTransRec.szDate",srTransRec.szDate,DATE_BCD_SIZE);
	DebugAddHEX("AAA - srTransRec.szTime",srTransRec.szTime,TIME_BCD_SIZE);
	
	memcpy(sLocalData.szDate, srTransRec.szDate, DATE_BCD_SIZE);
	memcpy(sLocalData.szTime, srTransRec.szTime, TIME_BCD_SIZE);

	DebugAddHEX("AAA - sLocalData.szDate",sLocalData.szDate,DATE_BCD_SIZE);
	DebugAddHEX("AAA - sLocalData.szTime",sLocalData.szTime,TIME_BCD_SIZE);

	strcpy(sLocalData.szXMLlocal_total_fee, srTransRec.szXMLtotalfee);
	strcpy(sLocalData.szXMLout_trade_no, strDigitalWallet.szout_trade_no);
	strcpy(sLocalData.szXMLcode_url, srTransRec.szXMLcode_url);
	strcpy(sLocalData.szXMLtime_end, szCurrentDateTime);
	sLocalData.byVoided = 0x30;
	memcpy(sLocalData.szInvoiceNo, srTransRec.szInvoiceNo, INVOICE_BCD_SIZE);
	strcpy(sLocalData.szXMLout_refund_no, strDigitalWallet.szout_refund_no);
	sLocalData.inQRPresented = srTransRec.inQRPresented;

	vdDebug_LogPrintf("MITid=[%d], HDTid=[%d], IITid[%d]", sLocalData.MITid, sLocalData.HDTid, sLocalData.IITid);
	vdDebug_LogPrintf("szTotalAmount=[%s], szBaseAmount=[%s]", sLocalData.szTotalAmount, sLocalData.szBaseAmount);
	vdDebug_LogPrintf("szDate=[%s], szTime=[%s]", sLocalData.szDate, sLocalData.szTime);
	vdDebug_LogPrintf("szXMLlocal_total_fee=[%s]", sLocalData.szXMLlocal_total_fee);
	vdDebug_LogPrintf("szXMLout_trade_no=[%s]", sLocalData.szXMLout_trade_no);
	vdDebug_LogPrintf("szXMLcode_url=[%s]", sLocalData.szXMLcode_url);
	vdDebug_LogPrintf("szXMLtime_end=[%s]", sLocalData.szXMLtime_end);
	vdDebug_LogPrintf("byVoided=[%d]", sLocalData.byVoided);
	vdDebug_LogPrintf("Inv Num:[%02X%02X%02X]",sLocalData.szInvoiceNo[0], sLocalData.szInvoiceNo[1], sLocalData.szInvoiceNo[2]);
	vdDebug_LogPrintf("szXMLout_refund_no=[%s]", sLocalData.szXMLout_refund_no);
	vdDebug_LogPrintf("inQRPresented=[%d]", sLocalData.inQRPresented);

	inResult = inDatabase_RetrieveBatchInsert(&sLocalData);
	vdDebug_LogPrintf("inDatabase_RetrieveBatchInsert, inResult=[%d]", inResult);

	if (inResult != d_OK)
		vdDebug_LogPrintf("inDatabase_RetrieveBatchInsert failed");
	else
		vdDebug_LogPrintf("inDatabase_RetrieveBatchInsert success");
	
    strcpy(srTransRec.szRespCode,"00");
	
	return d_OK;

	
}

int iXMLRetrieveListView(void)
{
	int inKey = d_NO;
	int inRet = d_NO;
	BYTE szTitle[40 + 1] = {0};

	vdDebug_LogPrintf("--iXMLRetrieveListView--");

	inIITRead(srTransRec.IITid);
	
	// Read Data
	inRet = iRetrieveReadData();
	if (inRet != d_OK)	
	return d_NO;	
	
	memset(strGBLVar.szGBLvProcessType, 0x00, sizeof(strGBLVar.szGBLvProcessType));
	memset(strGBLVar.szGBLvSelectedValue, 0x00, sizeof(strGBLVar.szGBLvSelectedValue));
	memset(strGBLVar.szGBLvProcessDesc, 0x00, sizeof(strGBLVar.szGBLvProcessDesc));

	memset(szTitle, 0x00, sizeof(szTitle));
	//if (srTransRec.HDTid == PHQR_HOST_INDEX)
		sprintf(szTitle, "%s %s|x", "RETRIEVE LIST", strIIT.szIssuerLabel);
	//else
	//	sprintf(szTitle, "%s<%s>|x", "RETRIEVE LIST", strIIT.szIssuerAbbrev);
	inKey = usCTOSS_TransDetailListViewUI(szTitle);
	
	vdDebug_LogPrintf("usCTOSS_TransDetailListViewUI::inKey=[%d]", inKey);
	vdDebug_LogPrintf("strGBLVar.szGBLvProcessType=[%s]", strGBLVar.szGBLvProcessType);
	vdDebug_LogPrintf("strGBLVar.szGBLvSelectedValue=[%s]", strGBLVar.szGBLvSelectedValue);
	vdDebug_LogPrintf("strGBLVar.szGBLvProcessDesc=[%s]", strGBLVar.szGBLvProcessDesc);

	switch (inKey)
	{
		case d_KBD_CANCEL:
			vdDisplayMessageBox(1, 8, "", "USER CANCEL", "", MSG_TYPE_INFO);
			inRet = d_NO;
			break;
		case d_OK: // Confirm

			if (0 == strcmp(strGBLVar.szGBLvProcessType, "1")) // Retrive
			{
				memset(strGBLVar.szGBLvOutTradeNo, 0x00, sizeof(strGBLVar.szGBLvOutTradeNo));
				memset(strGBLVar.szGBLvTotalFee, 0x00, sizeof(strGBLVar.szGBLvTotalFee));
				memset(strGBLVar.szGBLvOutRefundNo, 0x00, sizeof(strGBLVar.szGBLvOutRefundNo));
				memset(strGBLVar.szGBLvInvoiceNo, 0x00, sizeof(strGBLVar.szGBLvInvoiceNo));
				memset(strGBLVar.szGBLvTemp, 0x00, sizeof(strGBLVar.szGBLvTemp));
				
				GetDelimitedString(strGBLVar.szGBLvSelectedValue, 1, '^', strGBLVar.szGBLvOutTradeNo);
				GetDelimitedString(strGBLVar.szGBLvSelectedValue, 2, '^', strGBLVar.szGBLvTotalFee);

				GetDelimitedString(strGBLVar.szGBLvSelectedValue, 3, '^', strGBLVar.szGBLvOutRefundNo);
				GetDelimitedString(strGBLVar.szGBLvSelectedValue, 4, '^', strGBLVar.szGBLvInvoiceNo);
				GetDelimitedString(strGBLVar.szGBLvSelectedValue, 5, '^', strGBLVar.szGBLvTemp);
				GetDelimitedString(strGBLVar.szGBLvSelectedValue, 6, '^', strGBLVar.szGBLvVoided);
				
				vdDebug_LogPrintf("strGBLVar.szGBLvOutTradeNo=[%s]", strGBLVar.szGBLvOutTradeNo);
				vdDebug_LogPrintf("strGBLVar.szGBLvTotalFee=[%s]", strGBLVar.szGBLvTotalFee);
				vdDebug_LogPrintf("strGBLVar.szGBLvOutRefundNo=[%s]", strGBLVar.szGBLvOutRefundNo);
				vdDebug_LogPrintf("strGBLVar.szGBLvInvoiceNo=[%s]", strGBLVar.szGBLvInvoiceNo);
				vdDebug_LogPrintf("strGBLVar.szGBLvTemp=[%s]", strGBLVar.szGBLvTemp);
				vdDebug_LogPrintf("strGBLVar.szGBLvVoided=[%s]", strGBLVar.szGBLvVoided);

				srTransRec.byVoided = FALSE;
				if (strcmp(strGBLVar.szGBLvVoided, "VOIDED") == 0)
					srTransRec.byVoided = TRUE;

				if (strcmp(strGBLVar.szGBLvTemp, "MPQR") == 0)
					strGBLVar.inGBLvQRPresented = MPQR_TYPE;
				else if (strcmp(strGBLVar.szGBLvTemp, "CPQR") == 0)
					strGBLVar.inGBLvQRPresented = CPQR_TYPE;
					
				vdDebug_LogPrintf("strGBLVar.inGBLvQRPresented=[%d]", strGBLVar.inGBLvQRPresented);
				vdDebug_LogPrintf("srTransRec.byVoided=[%d]", srTransRec.byVoided);
				
				
				inRet = d_OK;
			}
							
			break;
		case d_TIMEOUT:
			vdDisplayMessageBox(1, 8, "", "TIME OUT", "", MSG_TYPE_TIMEOUT);
			inRet = d_NO;
			break;
	}

	// Check for voided -- sidumili
	if(srTransRec.byVoided == TRUE)
    {
        vdSetErrorMessage("RECORD VOIDED");
        return d_NO;
    }
	
	CTOS_Beep();
	CTOS_Delay(1000);

	return inRet;
}

void vdXMLFormatDateEndTime(char* szFormattedDateTime)
{
	char szTemp1[d_LINE_SIZE + 1];
	char szTemp2[d_LINE_SIZE + 1];
	char szStr[d_LINE_SIZE + 1];
	ULONG ulHH=0L;
    char szAMPM[2+1];
	char szStrLeft[d_LINE_SIZE + 1];
	char szStrRight[d_LINE_SIZE + 1];
	
	memset(szTemp1, 0x00, d_LINE_SIZE);
	memset(szTemp2, 0x00, d_LINE_SIZE);	
	printDateTime2(szTemp1, szTemp2, FALSE);
	memset(szStr, 0x00, d_LINE_SIZE);

	vdDebug_LogPrintf("--vdXMLFormatDateEndTime--");

	ulHH=atol(szTemp2)/10000;
	vdDebug_LogPrintf("ulHH=[%ld], szTemp1[%s], szTemp2[%s]", ulHH, szTemp1, szTemp2);
	if (ulHH==0&& ulHH!=12)
	{
		ulHH=12;
		strcpy(szAMPM,"AM"); 
	}
	else if (ulHH==12 && ulHH!=0)
	{
		ulHH=12;
		strcpy(szAMPM,"PM"); 
	}
	else if(ulHH<12 && ulHH!=0)
	{
		strcpy(szAMPM,"AM"); 
	}
	else if(ulHH>12 && ulHH!=0)
	{
		ulHH-=12;
		strcpy(szAMPM,"PM"); 
	}			

	//-->> DATE/TIME
	memset(szStrLeft, 0x00, d_LINE_SIZE);
	sprintf(szStrLeft, "%s", szTemp1);
	memset(szStrRight, 0x00, d_LINE_SIZE);
	sprintf(szStrRight, "%02lu:%02lu:%02lu%s", ulHH,atol(szTemp2)%10000/100, atol(szTemp2)%100, szAMPM);

	memset(szFormattedDateTime, 0x00, sizeof(szFormattedDateTime));
	sprintf(szFormattedDateTime, "%s %s", szStrLeft, szStrRight);
}

char* SHA256String(char* szString)
{
	BYTE HashLength;
	BYTE HashData[64 + 1];
	int inSize = 0;
	int inECRAscData = 0;
	BYTE uszInputDataSHA256[XML_SEND_SIZE+1]={0};
	BYTE szBase64HexData[XML_SEND_SIZE+1]={0};
	BYTE pbtBuff[XML_SEND_SIZE+1]={0};
	CTOS_SHA256_CTX ctx;

	vdDebug_LogPrintf("--SHA256String--");
	memcpy(uszInputDataSHA256, (BYTE *)szString, strlen(szString));

	vdDebug_LogPrintf("uszSendDataSHA256[%s], len[%d]", uszInputDataSHA256,  strlen(uszInputDataSHA256));
	
	CTOS_SHA256Init(&ctx);
    CTOS_SHA256Update(&ctx, uszInputDataSHA256, strlen(uszInputDataSHA256));
    CTOS_SHA256Final(&ctx, HashData);

	DebugAddHEX("HashData=",HashData,32);

	memset(pbtBuff, 0x00, sizeof(pbtBuff));
	wub_hex_2_str(HashData, pbtBuff, 32);

	vdDebug_LogPrintf("wub_hex_2_str, pbtBuff=[%s]", pbtBuff);

	//inSize = 32;
	//vdDebug_LogPrintf("HashData[%s] len[%d]",HashData,inSize);
	
	//inECRAscData = inSize;
	//base64_encode((char *)HashData,inECRAscData,&szBase64HexData);
	//vdDebug_LogPrintf("BASE 64 ENCODED %s",szBase64HexData);

	//memset(pbtBuff, 0x00, sizeof(pbtBuff));
	//memcpy(&pbtBuff[0], szBase64HexData,strlen(szBase64HexData));

	return pbtBuff;
	
}

int base64_encode(char *binStr, unsigned len, char *psz_Encoded) 
{
    char buf[2048+1];
    int buflen = 0;
    int c1, c2, c3;
    int maxbuf;
	static const char basis_64[] =
   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#ifdef RUBBISH
    maxbuf = len*4/3 + 1;  /* size after expantion */
#endif
    maxbuf = len*2 + 20;  /* size after expantion */

	memset(buf, 0x00, 2048+1);

    while (len) {
    c1 = (unsigned char)*binStr++;
    buf[buflen++] = basis_64[c1>>2];

    if (--len == 0) c2 = 0;
    else c2 = (unsigned char)*binStr++;
    buf[buflen++] = basis_64[((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4)];

    if (len == 0) {
        buf[buflen++] = '=';
        buf[buflen++] = '=';
        break;
    }

    if (--len == 0) c3 = 0;
    else c3 = (unsigned char)*binStr++;

    buf[buflen++] = basis_64[((c2 & 0xF) << 2) | ((c3 & 0xC0) >>6)];
    if (len == 0) {
        buf[buflen++] = '=';

        break;
    }

    --len;
    buf[buflen++] = basis_64[c3 & 0x3F];

    }

    buf[buflen]=0;

	strcpy(psz_Encoded, buf);

    return 0;
}

int iGetQRPresentedType(void)
{
	int iType = 0;
	//vdDebug_LogPrintf("--iGetQRPresentedType--");
	//vdDebug_LogPrintf("srTransRec.inQRPresented=[%d]", srTransRec.inQRPresented);

	strGBLVar.inGBLvQRPresented = srTransRec.inQRPresented;
	
	if (strGBLVar.inGBLvQRPresented == 0)		
		iType = MPQR_TYPE; // Merchant presented
	else
		iType = CPQR_TYPE; // Customer presented

	//vdDebug_LogPrintf("iType=[%d]", iType);
	
	return iType;
}


int iRetrieveUpdateData(void)
{
	int inResult = d_NO;
	RETRIEVE_DATA_TABLE sLocalData;
	CTOS_RTC SetRTC;
    BYTE szCurrentDate[20];
	BYTE szCurrentTime[20];
	BYTE szDate[DATE_BCD_SIZE+1];     /* BCD MMDD        */
    BYTE szTime[TIME_BCD_SIZE+1];     /* BCD hhmmss        */
	BYTE szCurrentDateTime[40];

	vdDebug_LogPrintf("--iRetrieveUpdateData--");

	if (!strGBLVar.fGBLvRetrieveList)
		return d_OK;
	

	CTOS_RTCGet(&SetRTC);
    memset(szCurrentDate, 0x00, sizeof(szCurrentDate));
	memset(szDate, 0x00, sizeof(szDate));
	memset(szTime, 0x00, sizeof(szTime));

	
    //sprintf(szCurrentDate,"20%02d%02d%02d",SetRTC.bYear, SetRTC.bMonth, SetRTC.bDay);
    
    //memset(szCurrentTime, 0x00, sizeof(szCurrentTime));
    //sprintf(szCurrentTime,"%02d%02d%02d", SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);

	memset(szCurrentDateTime, 0x00, sizeof(szCurrentDateTime));
	sprintf(szCurrentDateTime, "%s%s", srTransRec.szDate, srTransRec.szTime);
	
	memset(&sLocalData, 0x00, sizeof(RETRIEVE_DATA_TABLE));

	sLocalData.HDTid = srTransRec.HDTid;
	sLocalData.MITid = srTransRec.MITid;
	sLocalData.IITid = srTransRec.IITid;
	strcpy(sLocalData.szHostLabel, srTransRec.szHostLabel);
	strcpy(sLocalData.szIssuerLabel, srTransRec.szIssuerLabel);
	memcpy(sLocalData.szTotalAmount, srTransRec.szTotalAmount, AMT_BCD_SIZE);
	memcpy(sLocalData.szBaseAmount, srTransRec.szBaseAmount, AMT_BCD_SIZE);
	memcpy(sLocalData.szDate, srTransRec.szDate, DATE_BCD_SIZE);
	memcpy(sLocalData.szTime, srTransRec.szTime, TIME_BCD_SIZE);
	strcpy(sLocalData.szXMLlocal_total_fee, srTransRec.szXMLtotalfee);
	strcpy(sLocalData.szXMLout_trade_no, strDigitalWallet.szout_trade_no);
	strcpy(sLocalData.szXMLcode_url, srTransRec.szXMLcode_url);
	strcpy(sLocalData.szXMLtime_end, szCurrentDateTime);
	sLocalData.byVoided = 0x31;
	sLocalData.ulSavedIndex = srTransRec.ulSavedIndex;
	
	memset(sLocalData.szInvoiceNo, 0x00, sizeof(sLocalData.szInvoiceNo));
	memcpy(sLocalData.szInvoiceNo, srTransRec.szInvoiceNo, INVOICE_BCD_SIZE);
	strcpy(sLocalData.szXMLout_refund_no, strDigitalWallet.szout_refund_no);
	sLocalData.inQRPresented = srTransRec.inQRPresented;
	
	vdDebug_LogPrintf("MITid=[%d], HDTid=[%d], IITid[%d]", sLocalData.MITid, sLocalData.HDTid, sLocalData.IITid);
	vdDebug_LogPrintf("szTotalAmount=[%s], szBaseAmount=[%s]", sLocalData.szTotalAmount, sLocalData.szBaseAmount);
	vdDebug_LogPrintf("szDate=[%s], szTime=[%s]", sLocalData.szDate, sLocalData.szTime);
	vdDebug_LogPrintf("szXMLlocal_total_fee=[%s]", sLocalData.szXMLlocal_total_fee);
	vdDebug_LogPrintf("szXMLout_trade_no=[%s]", sLocalData.szXMLout_trade_no);
	vdDebug_LogPrintf("szXMLcode_url=[%s]", sLocalData.szXMLcode_url);
	vdDebug_LogPrintf("szXMLtime_end=[%s]", sLocalData.szXMLtime_end);	
	vdDebug_LogPrintf("byVoided=[%d]",sLocalData.byVoided);	
	vdDebug_LogPrintf("ulSavedIndex=[%d]",sLocalData.ulSavedIndex);	
	vdDebug_LogPrintf("szInvoiceNo:[%02X%02X%02X]",sLocalData.szInvoiceNo[0], sLocalData.szInvoiceNo[1], sLocalData.szInvoiceNo[2]);
	vdDebug_LogPrintf("szXMLout_refund_no=[%s]", sLocalData.szXMLout_refund_no);
	vdDebug_LogPrintf("inQRPresented=[%d]",sLocalData.inQRPresented);

	//inResult = inDatabase_RetrieveBatchUpdate(&sLocalData);
	inResult = inDatabase_RetrieveBatchUpdate2(&sLocalData, sLocalData.szInvoiceNo, sLocalData.szXMLout_trade_no);
	vdDebug_LogPrintf("inDatabase_RetrieveBatchUpdate2, inResult=[%d]", inResult);

	if (inResult != d_OK)
		vdDebug_LogPrintf("inDatabase_RetrieveBatchUpdate2 failed");
	else
		vdDebug_LogPrintf("inDatabase_RetrieveBatchUpdate2 success");
	return d_OK;

}

int iXMLCheckResponseOutRefundNo(char* szXMLReceiveData)
{
	BOOL fValid = FALSE;
	BYTE szTemp[256 + 1] = {0};
	BYTE szXMLValue[256 + 1] = {0};
	BOOL fTagExist = FALSE;
	BYTE szErroMsg[256 + 1] = {0};
	int iWidth = 16;
	int i;

	vdDebug_LogPrintf("--iXMLCheckResponseOutRefundNo--");

	srTransRec.inXMLresult_code = d_NO;

	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_OUT_REFUND_NO, szXMLValue);

	if (strlen(szXMLValue) > 0)
	{
		memset(strDigitalWallet.szout_refund_no, 0x00, sizeof(strDigitalWallet.szout_refund_no));
		memset(srTransRec.szXMLout_refund_no, 0x00, sizeof(srTransRec.szXMLout_refund_no));
		
		strcpy(strDigitalWallet.szout_refund_no, szXMLValue);
		strcpy(srTransRec.szXMLout_refund_no, szXMLValue);
		
	}

	vdDebug_LogPrintf("szXMLValue=[%s]", szXMLValue);
	
	return d_OK;
}

int inCTOS_SelectQRPresented(void)
{
	//BYTE key;
	int inRet;
	char szDebug[40 + 1]={0};
	char szChoiceMsg[30 + 1];
	char szHeaderString[24+1];
	int bHeaderAttr = 0x01+0x04, key=0; 
	
	vdDebug_LogPrintf("saturn inCTOS_SelectQRPresented[START]");

	vdDebug_LogPrintf("-->>SELECT ACNT byTransType[%d]", srTransRec.byTransType);

    memset(szHeaderString, 0x00, sizeof(szHeaderString));
	memset(szChoiceMsg, 0x00, sizeof(szChoiceMsg));

	//if (2 == strTCT.byTerminalType)
	{
		strcpy(szHeaderString, "SELECT QRCODE PRESENTED");
		strcat(szChoiceMsg,"SCAN QRCODE \n");
		strcat(szChoiceMsg,"GENERATE QRCODE");
		key = MenuDisplay(szHeaderString,strlen(szHeaderString), bHeaderAttr, 1, 1, szChoiceMsg, TRUE);

		
		if (key > 0)
		{
			if (key == 1)
			{
				srTransRec.inQRPresented = CPQR_TYPE;
				return d_OK;
			}
			if (key == 2)
			{
				//vdDisplayMessageStatusBox(1, 8, "PROCESSING", MSG_PLS_WAIT, MSG_TYPE_PROCESS);
				srTransRec.inQRPresented = MPQR_TYPE;
				return d_OK;
			}
			
			if (key == d_KBD_CANCEL)
			{
			    if(fGetECRTransactionFlg())
                {
                     vdSetECRResponse(ECR_UNKNOWN_ERR);
                     //inMultiAP_ECRSendSuccessResponse();
                }
				return -1;
			}

			if (key == 0xFF)
			{
				vdDisplayMessageBox(1, 8, "", "TIME OUT", "", MSG_TYPE_TIMEOUT);
				CTOS_Beep();
				CTOS_Delay(1500);
                if(fGetECRTransactionFlg())
                {
                     vdSetECRResponse(ECR_TIMEOUT_ERR);
                     //inMultiAP_ECRSendSuccessResponse();
                }
				return d_NO;
			}

			return d_NO;
			
		}
	}
	
	return inRet;
}

int iXMLCheckResponseRefundStatus(char* szXMLReceiveData)
{
	BOOL fValid = FALSE;
	BYTE szTemp[256 + 1] = {0};
	BYTE szXMLValue[256 + 1] = {0};
	BOOL fTagExist = FALSE;
	BYTE szErroMsg[256 + 1] = {0};
	int iWidth = 16;
	int i;

	vdDebug_LogPrintf("--iXMLCheckResponseRefundStatus--");

	srTransRec.inXMLstatus = d_NO;
	srTransRec.fXMLTradeStateOK = FALSE;
	if (iXMLCheckResponseStatus((char*)szXMLReceiveData) == d_OK && iXMLCheckResponseResultCode((char*)szXMLReceiveData) == d_OK)
	{
		srTransRec.inXMLstatus = 0;
		srTransRec.fXMLTradeStateOK = TRUE;
		return d_OK;
	}
	else
	{
		memset(szXMLValue, 0x00, sizeof(szXMLValue));
		vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_ERR_MSG, szXMLValue);
		
		if (strlen(szXMLValue) > 0)
			iXMLDisplayResponse(szXMLValue, iWidth);
		else	
			iXMLDisplayResponse("ERROR RESPONSE", iWidth);
		
		return d_NO;
	}

	#if 0
    //check result code
	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_REFUND_STATUS, szXMLValue);
	if (strcmp(szXMLValue, "SUCCESS") == 0)
	{
		srTransRec.inXMLstatus = 0;
		srTransRec.fXMLTradeStateOK = TRUE;
	}else{
		memset(szXMLValue, 0x00, sizeof(szXMLValue));
		vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_ERR_MSG, szXMLValue);
		
		if (strlen(szXMLValue) > 0)
			iXMLDisplayResponse(szXMLValue, iWidth);
		else	
			iXMLDisplayResponse("ERROR RESPONSE", iWidth);
		
		return d_NO;

	}
	#endif
	
	return d_OK;
}

int vdXMLProcessReversal(void)
{
	int inRet = d_NO;
	
	vdDebug_LogPrintf("--vdXMLProcessReversal--");
	vdDebug_LogPrintf("srTransRec.fStateTimeOut=[%d]", srTransRec.fStateTimeOut);
	
	if (srTransRec.fStateTimeOut)
	{
		if (iGetQRPresentedType() == CPQR_TYPE)
		{
			vdDebug_LogPrintf("CPQR: Process reverasal");
			vdDisplayMessageStatusBox(1, 8, "SENDING REVERSAL INTERFACE", MSG_PLS_WAIT, MSG_TYPE_SEND);
			CTOS_Delay(2000);
			inRet = inXML_CommsInit();
			if (inRet != d_OK)
			{
				vdDisplayMessageBox(1, 8, "SENDING", "REVERSAL INTERFACE", "FAILED", MSG_TYPE_ERROR);
			}
			else
			{
				srTransRec.iXMLType = WECHAT_ALIPAY_REVERSAL;
				srTransRec.byPackType = REVERSAL;
			
				inXMLCreateRequestData();
				inRet = inXMSendAndReceiveRequestMessage();
				if (inRet != d_OK)
					vdDisplayMessageBox(1, 8, "SENDING", "REVERSAL INTERFACE", "FAILED", MSG_TYPE_ERROR);
				else
					vdDisplayMessageBox(1, 8, "SENDING", "REVERSAL INTERFACE", "SUCCESS", MSG_TYPE_SUCCESS);

				inRet = d_NO;
				
			}	

			CTOS_Beep();
			CTOS_Delay(1000);
		}
	}

	return inRet;
		
}

void vdXMLSelectedRetrieve(void)
{
	memset(strDigitalWallet.szout_trade_no, 0x00, sizeof(strDigitalWallet.szout_trade_no));
	memset(srTransRec.szXMLout_trade_no, 0x00, sizeof(srTransRec.szXMLout_trade_no));

	// For out_trade_no
	strcpy(strDigitalWallet.szout_trade_no, strGBLVar.szGBLvOutTradeNo);
	strcpy(srTransRec.szXMLout_trade_no, strGBLVar.szGBLvOutTradeNo);

	// For total fee
	memset(strDigitalWallet.sztotal_fee, 0x00, sizeof(strDigitalWallet.sztotal_fee));
	memset(srTransRec.szXMLtotalfee, 0x00, sizeof(srTransRec.szXMLtotalfee));
	strcpy(strDigitalWallet.sztotal_fee, strGBLVar.szGBLvTotalFee);
	strcpy(srTransRec.szXMLtotalfee, strGBLVar.szGBLvTotalFee);

	// For Refund no
	memset(strDigitalWallet.szout_refund_no, 0x00, sizeof(strDigitalWallet.szout_refund_no));
	memset(srTransRec.szXMLout_refund_no, 0x00, sizeof(srTransRec.szXMLout_refund_no));
	strcpy(strDigitalWallet.szout_refund_no, strGBLVar.szGBLvOutRefundNo);
	strcpy(srTransRec.szXMLout_refund_no, strGBLVar.szGBLvOutRefundNo);

	// For QR Presented
	srTransRec.inQRPresented = strGBLVar.inGBLvQRPresented;
	memset(srTransRec.szInvoiceNo, 0x00, sizeof(srTransRec.szInvoiceNo));

	wub_str_2_hex(strGBLVar.szGBLvInvoiceNo,srTransRec.szInvoiceNo,INVOICE_ASC_SIZE);	
	//memcpy(srTransRec.szInvoiceNo, strGBLVar.szGBLvInvoiceNo, INVOICE_BCD_SIZE);

	srRetrieveRec.byVoided = FALSE;
	srTransRec.byVoided = FALSE;
	if (strcmp(strGBLVar.szGBLvVoided, "VOIDED") == 0)
	{
		srTransRec.byVoided = TRUE;
		srRetrieveRec.byVoided = TRUE;
	}	

	srRetrieveRec.fRetrieveInList = TRUE;
}

// Update Retrieve Data by OutTradeNo -- sidumili
int iRetrieveUpdateDataByOutTradeNo(void)
{
	int inResult = d_NO;
	RETRIEVE_DATA_TABLE sLocalData;

	vdDebug_LogPrintf("--iRetrieveUpdateDataByOutTradeNo--");

	memset(&sLocalData, 0x00, sizeof(RETRIEVE_DATA_TABLE));	
	
	strcpy(sLocalData.szXMLout_trade_no, srTransRec.szXMLout_trade_no);
	strcpy(sLocalData.szXMLout_transaction_id, srTransRec.szXMLout_transaction_id);
	strcpy(sLocalData.szXMLtransaction_id, srTransRec.szXMLtransaction_id);

	strcpy(sLocalData.szXMLout_refund_no, srTransRec.szRefundID);
	
	vdDebug_LogPrintf("szXMLout_trade_no=[%s]", sLocalData.szXMLout_trade_no);
	vdDebug_LogPrintf("szXMLout_transaction_id=[%s]", sLocalData.szXMLout_transaction_id);
	vdDebug_LogPrintf("szXMLtransaction_id=[%s]", sLocalData.szXMLtransaction_id);
	vdDebug_LogPrintf("szXMLout_refund_no=[%s]", sLocalData.szXMLout_refund_no);

	inResult = inDatabase_RetrieveBatchUpdateByOutTradeNo(&sLocalData, sLocalData.szXMLout_trade_no);
	vdDebug_LogPrintf("inDatabase_RetrieveBatchUpdateByOutTradeNo, inResult=[%d]", inResult);

	if (inResult != d_OK)
		vdDebug_LogPrintf("inDatabase_RetrieveBatchUpdateByOutTradeNo failed");
	else
		vdDebug_LogPrintf("inDatabase_RetrieveBatchUpdateByOutTradeNo success");
	return d_OK;

}

// Get Refund ID from response during void
int iXMLCheckResponseOutRefundID(char* szXMLReceiveData)
{
	BOOL fValid = FALSE;
	BYTE szTemp[256 + 1] = {0};
	BYTE szXMLValue[256 + 1] = {0};
	BOOL fTagExist = FALSE;
	BYTE szErroMsg[256 + 1] = {0};
	int iWidth = 16;
	int i;

	vdDebug_LogPrintf("--iXMLCheckResponseOutRefundID--");

	srTransRec.inXMLresult_code = d_NO;

	memset(szXMLValue, 0x00, sizeof(szXMLValue));
	vXMLGetResponseDataByTag(XML_PARSE_STRING, szXMLReceiveData, TAG_REFUND_ID, szXMLValue);

	if (strlen(szXMLValue) > 0)
	{
		memset(srTransRec.szRefundID, 0x00, sizeof(srTransRec.szRefundID));
		strcpy(srTransRec.szRefundID, szXMLValue);
		
	}

	vdDebug_LogPrintf("szXMLValue=[%s]", szXMLValue);
	
	return d_OK;
}




