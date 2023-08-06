/*******************************************************************************

*******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctosapi.h>
#include <EMVAPLib.h>
#include <EMVLib.h>


#include "..\Includes\Wub_lib.h"

#include "..\Includes\POSTypedef.h"
#include "..\FileModule\myFileFunc.h"

#include "..\Includes\msg.h"
#include "..\Includes\CTOSInput.h"
#include "..\ui\Display.h"

#include "..\Includes\V5IsoFunc.h"
#include "..\Comm\V5Comm.h"
#include "..\Includes\Trans.h"   


#include "..\Includes\CTOSInput.h"


#include "..\debug\debug.h"
#include "..\Accum\Accum.h"

#include "..\Includes\POSMain.h"
#include "..\Includes\POSTrans.h"
#include "..\Includes\POSHost.h"
#include "..\Includes\POSSale.h"
#include "..\Database\DatabaseFunc.h"
#include "..\Includes\POSHost.h"
#include "..\Includes\Wub_lib.h"
#include "..\Includes\myEZLib.h"
#include "..\accum\accum.h"
#include "..\Includes\POSSetting.h"
#include "..\Debug\Debug.h"
#include "..\filemodule\myFileFunc.h"
#include "..\Includes\POSTrans.h"
#include "..\Includes\CTOSInput.h"
#include "..\Ctls\PosWave.h"


#include "..\Includes\MultiApLib.h"
#include "..\Aptrans\MultiAptrans.h"
#include "..\Aptrans\MultiShareEMV.h"
#include "..\POWRFAIL\POSPOWRFAIL.h"
#include "..\print\print.h"


#define d_NOT_RECORD            102

#define d_GETPIN_TIMEOUT        6000
#define d_DUPLICATE_INVOICE     0x0080

extern BOOL fECRTxnFlg;



#if 0
int inCTOS_GetTxnBaseAmount(void)
{
    char szDisplayBuf[30];
    BYTE key;
    BYTE szTemp[20];
    BYTE baAmount[20];
    BYTE bBuf[4+1];
    BYTE bDisplayStr[MAX_CHAR_PER_LINE+1];
    ULONG ulAmount = 0L;

	if (1 == inCTOSS_GetWaveTransType())
	if (inMultiAP_CheckSubAPStatus() == d_OK)
        return d_OK;

	if (memcmp(srTransRec.szBaseAmount, "\x00\x00\x00\x00\x00\x00", 6) != 0)
		return d_OK;
	
    while(1)
    {
        CTOS_LCDTClearDisplay();
        vduiLightOn();
        
        vdDispTransTitle(srTransRec.byTransType);
    
        if(CARD_ENTRY_MANUAL == srTransRec.byEntryMode)
        {
            setLCDPrint(2, DISPLAY_POSITION_LEFT, "CARD NO: ");
            memset(bDisplayStr, 0x00, sizeof(bDisplayStr));
            strcpy(bDisplayStr, srTransRec.szPAN);
            CTOS_LCDTPrintXY(MAX_CHAR_PER_LINE-strlen(bDisplayStr)*2, 3, bDisplayStr);
            setLCDPrint(5, DISPLAY_POSITION_LEFT, "EXPIRY DATE(MM/YY):");
    
            memset(bDisplayStr, 0x00, sizeof(bDisplayStr));
            memset(bBuf, 0x00, sizeof(bBuf));
            wub_hex_2_str(&srTransRec.szExpireDate[1], &bBuf[0], 1);
            memcpy(bDisplayStr, bBuf, 2);
            bDisplayStr[2] = '/';
            memset(bBuf, 0x00, sizeof(bBuf));
            wub_hex_2_str(&srTransRec.szExpireDate[0], &bBuf[0], 1);
            memcpy(bDisplayStr+3, bBuf, 2);
            CTOS_LCDTPrintXY(MAX_CHAR_PER_LINE-strlen(bDisplayStr)*2, 6, bDisplayStr);
        }
        else
        {
    		if (1 != inCTOSS_GetWaveTransType())
    		{
    			inCTOS_DisplayCardTitle(5, 6, TRUE);
    		}
        }
	  
        CTOS_KBDHit(&key);//clear key buffer
        memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
        sprintf(szDisplayBuf, "%s", strCST.szCurSymbol);
        CTOS_LCDTPrintXY(1, 7, "AMOUNT:");
    
        memset(baAmount, 0x00, sizeof(baAmount));
        key = InputAmount2(1, 8, szDisplayBuf, 2, 0x00, baAmount, &ulAmount, d_INPUT_TIMEOUT, 0);
    
        if(d_OK == key)
        {
            memset(szTemp, 0x00, sizeof(szTemp));
            sprintf(szTemp, "%012ld", atol(baAmount));
            wub_str_2_hex(szTemp,srTransRec.szBaseAmount,12);
    
            if(srTransRec.byTransType == SALE && srTransRec.fInstallment == TRUE)
            {
                long amt1=0, amt2=0;	
               
                inTCTRead(1);
                memset(szTemp, 0x00, sizeof(szTemp));
                wub_hex_2_str(strHDT.szMinInstAmt, szTemp,6);
    
    			amt1=atol(baAmount);
    			amt2=atol(szTemp);
    
    			if(amt1 < amt2)
    			{
    				char szMinInstAmt[12+1];
    				
    				memset(szMinInstAmt, 0x00, sizeof(szMinInstAmt));
    				vdFormatAmount(szMinInstAmt, "", strHDT.szMinInstAmt,FALSE);
    				memset(szTemp, 0x00, sizeof(szTemp));
    				sprintf(szTemp,"MIN AMOUNT %s", szMinInstAmt);
    
                    CTOS_LCDTClearDisplay();
    				vdDisplayErrorMsg(1, 8, szTemp);
    				vduiWarningSound();
    				continue;
    			}
				else
                    break;  					
            }	
			else 
                break;				
        }
		else if(d_USER_CANCEL == key)
		{
            vdSetErrorMessage("USER CANCEL");
            return d_NO;
		}
        else if(0xFF == key)
        {
            vdSetErrorMessage("TIME OUT");
            return d_NO;
        }
    }
	
    return key;
}
#endif
int inCTOS_GetTxnBaseAmount(void)
{
    char szDisplayBuf[30];
    BYTE key;
    BYTE szTemp[20];
    BYTE baAmount[20];
    BYTE bBuf[4+1];
    BYTE bDisplayStr[MAX_CHAR_PER_LINE+1];
    ULONG ulAmount = 0L;
	BYTE szXMLTemp[13+1];
	BYTE bFirstKey;
	BYTE szDisplay[128];
	BYTE szTitle[16+1];
	
	vdDebug_LogPrintf("--inCTOS_GetTxnBaseAmount--");
	vdDebug_LogPrintf("byTransType[%d]", srTransRec.byTransType);
	vdDebug_LogPrintf("iXMLType[%d]", srTransRec.iXMLType);

	memset(szTitle, 0x00, sizeof(szTitle));
    szGetTransTitle(srTransRec.byTransType, szTitle);

	/*
    if (srTransRec.iXMLType != WECHAT_ALIPAY_REQUEST_QRCODE && srTransRec.iXMLType != WECHAT_ALIPAY_RETRIEVE_QRCODE) {
		if (inMultiAP_CheckSubAPStatus() == d_OK) 
        	return d_OK;
    }
	*/
	vdDebug_LogPrintf("AANA>>srTransRec.szBaseAmount[%02x%02x%02x%02x%02x%02x] fGetECRTransactionFlg(%d)", srTransRec.szBaseAmount[0],srTransRec.szBaseAmount[1],srTransRec.szBaseAmount[2],srTransRec.szBaseAmount[3],srTransRec.szBaseAmount[4],srTransRec.szBaseAmount[5], fGetECRTransactionFlg());
	if (fGetECRTransactionFlg() == 1)
	{
	    
		if (memcmp(srTransRec.szBaseAmount, "\x00\x00\x00\x00\x00\x00", 6) == 0){
			vdSetErrorMessage("OUT OF RANGE");
			return(ST_ERROR);  
		}


        memset(srTransRec.szXMLtotalfee, 0x00, sizeof(srTransRec.szXMLtotalfee));	
        memset(strDigitalWallet.sztotal_fee, 0x00, sizeof(strDigitalWallet.sztotal_fee));
		memset(szXMLTemp, 0x00, sizeof(szXMLTemp));
		
		wub_hex_2_str(srTransRec.szBaseAmount, szXMLTemp, 6);
		
		strcpy(srTransRec.szXMLtotalfee, szXMLTemp);
		strcpy(strDigitalWallet.sztotal_fee, szXMLTemp);
	}

	if (memcmp(srTransRec.szBaseAmount, "\x00\x00\x00\x00\x00\x00", 6) != 0)
		return d_OK;
	
    //CTOS_LCDTClearDisplay();
    vduiLightOn();
    
    //vdDispTransTitle(srTransRec.byTransType);

    if(CARD_ENTRY_MANUAL == srTransRec.byEntryMode)
    {
        setLCDPrint(2, DISPLAY_POSITION_LEFT, "CARD NO: ");
        memset(bDisplayStr, 0x00, sizeof(bDisplayStr));
        strcpy(bDisplayStr, srTransRec.szPAN);
        CTOS_LCDTPrintXY(MAX_CHAR_PER_LINE-strlen(bDisplayStr)*2, 3, bDisplayStr);
        setLCDPrint(5, DISPLAY_POSITION_LEFT, "EXPIRY DATE(MM/YY):");

        memset(bDisplayStr, 0x00, sizeof(bDisplayStr));
        memset(bBuf, 0x00, sizeof(bBuf));
        wub_hex_2_str(&srTransRec.szExpireDate[1], &bBuf[0], 1);
        memcpy(bDisplayStr, bBuf, 2);
        bDisplayStr[2] = '/';
        memset(bBuf, 0x00, sizeof(bBuf));
        wub_hex_2_str(&srTransRec.szExpireDate[0], &bBuf[0], 1);
        memcpy(bDisplayStr+3, bBuf, 2);
        CTOS_LCDTPrintXY(MAX_CHAR_PER_LINE-strlen(bDisplayStr)*2, 6, bDisplayStr);
    }
    //else
    //{
    //	if (1 != inCTOSS_GetWaveTransType())
    //    	inCTOS_DisplayCardTitle(4, 5);
    //}
    
    CTOS_KBDHit(&key);//clear key buffer
    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    sprintf(szDisplayBuf, "%s", strCST.szCurSymbol);
    CTOS_LCDTPrintXY(1, 7, "AMOUNT:");

	memset(szDisplay, 0x00, sizeof(szDisplay));
	strcpy(szDisplay, szTitle);
	strcat(szDisplay, "|");
	strcat(szDisplay, "Please Enter Amount");
	strcat(szDisplay, "|");
	strcat(szDisplay, strCST.szCurSymbol);
		
    memset(baAmount, 0x00, sizeof(baAmount));
    //key = InputAmount(1, 8, szDisplayBuf, 2, 0x00, baAmount, &ulAmount, d_INPUT_TIMEOUT, 0);
	//key = InputAmount2(1, 8, szDisplayBuf, 2, 0x00, baAmount, &ulAmount, d_INPUT_TIMEOUT, 0);

	vdDebug_LogPrintf("saturn nputAmountEx szDisplay[%s]", szDisplay);

	key = InputAmountEx(1, 8, szDisplay, 2, bFirstKey, baAmount, &ulAmount, 30, 0);

	vdDebug_LogPrintf("saturn InputAmountEx %d", key);

    if(d_OK == key)
    {
        memset(szTemp, 0x00, sizeof(szTemp));
        sprintf(szTemp, "%012ld", atol(baAmount));
        wub_str_2_hex(szTemp,srTransRec.szBaseAmount,12);

		// Hold Amount For Wechat
		memset(szTemp, 0x00, sizeof(szTemp));
		memset(srTransRec.szXMLtotalfee, 0x00, sizeof(srTransRec.szXMLtotalfee));
		sprintf(szXMLTemp, "%d", atoi(baAmount));
		strcpy(srTransRec.szXMLtotalfee, szXMLTemp);
		strcpy(strDigitalWallet.sztotal_fee, szXMLTemp);
		
		sprintf(szTemp, "%012ld", atol(baAmount));
        wub_str_2_hex(szTemp,srTransRec.szBaseAmount,12);
		
        // sidumili: Issue#:000076 [check transaction maximum amount]
		//if (inCTOS_ValidateTrxnAmount()!= d_OK){
		//	vdDebug_LogPrintf("inCTOS_ValidateTrxnAmount result");
		//	return(d_NO);
		//}
    }
        
    if(0xFF == key)
    {
        //vdSetErrorMessage("Amt entry cancelled");
		vdDisplayMessageBox(1, 8, "", "TIME OUT", "", MSG_TYPE_TIMEOUT);
		CTOS_Beep();
		CTOS_Delay(1000);
        return d_NO;
    }

	/*sidumili: [USER press cancel]*/
	if (d_USER_CANCEL == key){
			vdDispTransTitle(srTransRec.byTransType);
			//vdSetErrorMessage("Amt entry cancelled");
			vdDisplayMessageBox(1, 8, "", "AMOUNT ENTRY CANCELLED", "", MSG_TYPE_WARNING);
			CTOS_Beep();
			CTOS_Delay(1000);
			return d_NO;
	}
	/*sidumili: [USER press cancel]*/

    return key;
}


int inCTOS_GetTxnTipAmount(void)
{
    char szDisplayBuf[30];
    BYTE key;
    BYTE szTemp[20];
    BYTE baAmount[20];
    BYTE baBaseAmount[20];
    BYTE bBuf[4+1];
    BYTE bDisplayStr[MAX_CHAR_PER_LINE+1];
    ULONG ulAmount = 0L;

    DebugAddSTR("inCTOS_GetTxnTipAmount","Processing...",20);

    if (d_OK != inCTOS_CheckTipAllowd())
        return d_OK;

	if (1 == inCTOSS_GetWaveTransType())
	if (inMultiAP_CheckSubAPStatus() == d_OK)
        return d_OK;
    
    memset(baBaseAmount, 0x00, sizeof(baBaseAmount));
    wub_hex_2_str(srTransRec.szBaseAmount, baBaseAmount, 6);

    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    sprintf(szDisplayBuf, "%s", strCST.szCurSymbol);
        
    do
    {
        //CTOS_LCDTClearDisplay();
        vdDispTransTitle(srTransRec.byTransType);

        if(CARD_ENTRY_MANUAL == srTransRec.byEntryMode)
        {
            setLCDPrint(2, DISPLAY_POSITION_LEFT, "CARD NO: ");
            memset(bDisplayStr, 0x00, sizeof(bDisplayStr));
            strcpy(bDisplayStr, srTransRec.szPAN);
            CTOS_LCDTPrintXY(MAX_CHAR_PER_LINE-strlen(bDisplayStr)*2, 3, bDisplayStr);
            setLCDPrint(5, DISPLAY_POSITION_LEFT, "EXPIRY DATE(MM/YY):");

            memset(bDisplayStr, 0x00, sizeof(bDisplayStr));
            memset(bBuf, 0x00, sizeof(bBuf));
            wub_hex_2_str(&srTransRec.szExpireDate[1], &bBuf[0], 1);
            memcpy(bDisplayStr, bBuf, 2);
            bDisplayStr[2] = '/';
            memset(bBuf, 0x00, sizeof(bBuf));
            wub_hex_2_str(&srTransRec.szExpireDate[0], &bBuf[0], 1);
            memcpy(bDisplayStr+3, bBuf, 2);
            CTOS_LCDTPrintXY(MAX_CHAR_PER_LINE-strlen(bDisplayStr)*2, 6, bDisplayStr);
        }
        else
        {
        	if (1 != inCTOSS_GetWaveTransType())
        	{
				inCTOS_DisplayCardTitle(5, 6, TRUE);
        	}
        }
        
        CTOS_KBDHit(&key);//clear key buffer
        
        CTOS_LCDTPrintXY(1, 7, "TIP:");

        memset(baAmount, 0x00, sizeof(baAmount));
        key = InputAmount2(1, 8, szDisplayBuf, 2, 0x00, baAmount, &ulAmount, d_INPUT_TIMEOUT, 1);

        if(d_OK == key)
        {
			ULONG ulTipPercent=0;
			
            if(0 == strlen(baAmount)) //by pass TIP
                return d_OK;

            memset(szTemp, 0x00, sizeof(szTemp));
            sprintf(szTemp, "%012ld", atol(baAmount));

                        
			ulTipPercent=(atol(baBaseAmount) * strHDT.ulTipPercent)/100;
			//vdDebug_LogPrintf("atol(szTemp): %ld", atol(szTemp));
			//vdDebug_LogPrintf("strHDT.ulTipPercent: %ld", strHDT.ulTipPercent);
			//vdDebug_LogPrintf("ulTipPercent: %ld", ulTipPercent);
			
			if(atol(szTemp) > ulTipPercent)
			{
                clearLine(8);
                vdDisplayErrorMsg(1, 8, "TOO MUCH TIP");
                clearLine(8);
                continue;				
			}
			
			if(strcmp(szTemp, baBaseAmount) > 0)
            {
                clearLine(8);
                vdDisplayErrorMsg(1, 8, "TOO MUCH TIP");
                clearLine(8);
                continue;
            }
            
            wub_str_2_hex(szTemp,srTransRec.szTipAmount,12);
            
            break;
        }
        else if(d_USER_CANCEL == key)
        {   
            clearLine(8);
            vdSetErrorMessage("USER CANCEL");
            clearLine(8);
            return d_NO;
        }
        else if(0xFF == key)
        {   
            clearLine(8);
            vdSetErrorMessage("TIME OUT");
            clearLine(8);
            return d_NO;
        }
        
        
    }while(1);
    
    return key;
}


int inCTOS_GetTipAfjustAmount(void)
{
    char szDisplayBuf[30];
    BYTE key;
    BYTE szTemp[20];
    BYTE baAmount[20];
    BYTE baBaseAmount[20];
    BYTE baTipAmount[20];
    ULONG ulAmount = 0L;
    BYTE szAmtTmp2[16+1];

    DebugAddSTR("inCTOS_GetTipAfjustAmount","Processing...",20);
    memset(baBaseAmount, 0x00, sizeof(baBaseAmount));
    wub_hex_2_str(srTransRec.szBaseAmount, baBaseAmount, 6);

    memset(baTipAmount, 0x00, sizeof(baTipAmount));
    memset(szAmtTmp2, 0x00, sizeof(szAmtTmp2));
    wub_hex_2_str(srTransRec.szTipAmount, baTipAmount, 6);

    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    sprintf(szDisplayBuf, "%s", strCST.szCurSymbol);

    //CTOS_LCDTClearDisplay();
    vdDispTransTitle(srTransRec.byTransType);

    inCTOS_DisplayCardTitle(3, 4, FALSE);
    
    
    memset(szAmtTmp2, 0x00, sizeof(szAmtTmp2));
    sprintf(szAmtTmp2, "%lu.%02lu", atol(baTipAmount)/100, atol(baTipAmount)%100);
    setLCDPrint(5, DISPLAY_POSITION_LEFT, "ORG TIP");
    setLCDPrint(6, DISPLAY_POSITION_LEFT, szDisplayBuf);
    CTOS_LCDTPrintXY(MAX_CHAR_PER_LINE-(strlen(szAmtTmp2)+1)*2,  6, szAmtTmp2);
    
    CTOS_LCDTPrintXY(1, 7, "TIP");
             
    do
    {
        CTOS_KBDHit(&key);//clear key buffer

        memset(baAmount, 0x00, sizeof(baAmount));
        key = InputAmount2(1, 8, szDisplayBuf, 2, 0x00, baAmount, &ulAmount, d_INPUT_TIMEOUT, 0);

        if(strlen(baAmount) > 0)
        {	
            ULONG ulTipPercent=0;			
			
            memset(szTemp, 0x00, sizeof(szTemp));
            sprintf(szTemp, "%012ld", atol(baAmount));

			ulTipPercent=(atol(baBaseAmount) * strHDT.ulTipPercent)/100;
			//vdDebug_LogPrintf("atol(szTemp): %ld", atol(szTemp));
			//vdDebug_LogPrintf("strHDT.ulTipPercent: %ld", strHDT.ulTipPercent);
			//vdDebug_LogPrintf("ulTipPercent: %ld", ulTipPercent);
			if(atol(szTemp) > ulTipPercent)
			{
                clearLine(8);
                vdDisplayErrorMsg(1, 8, "TOO MUCH TIP");
                clearLine(8);
                continue;				
			}
			
            if(strcmp(szTemp, baBaseAmount) > 0)
            {
                vduiClearBelow(8);
                vdDisplayErrorMsg(1, 8, "TOO MUCH TIP");
                vduiClearBelow(8);
                continue;
            }
            
            wub_str_2_hex(szTemp,srTransRec.szTipAmount,12);
            
            break;
        }
        else if(d_USER_CANCEL == key)
        {   
            vdSetErrorMessage("USER CANCEL");
            return d_NO;
        }
        else if(0xFF == key)
        {   
            vdSetErrorMessage("TIME OUT");
            return d_NO;
        }
        
    }while(1);
    
    return key;
}



int inCTOS_UpdateTxnTotalAmount(void)
{
    BYTE szBaseAmount[20];
    BYTE szTipAmount[20];
    BYTE szTotalAmount[20];
    BYTE   EMVtagVal[64];
    BYTE   szStr[64];
    BYTE  byDataTmp1[32];
    BYTE  byDataTmp2[32];
    BYTE  bPackSendBuf[256];
    USHORT usPackSendLen = 0;
    USHORT ushEMVtagLen;
    ULONG lnTmp;

    memset(szBaseAmount, 0x00, sizeof(szBaseAmount));
    memset(szTipAmount, 0x00, sizeof(szTipAmount));
    memset(szTotalAmount, 0x00, sizeof(szTotalAmount));

    wub_hex_2_str(srTransRec.szTipAmount, szTipAmount, 6);
    wub_hex_2_str(srTransRec.szBaseAmount, szBaseAmount, 6);

    sprintf(szTotalAmount, "%012ld", atol(szBaseAmount) + atol(szTipAmount));
    wub_str_2_hex(szTotalAmount,srTransRec.szTotalAmount,12);
    
    DebugAddSTR(szTotalAmount,szTipAmount,12); 
    DebugAddSTR("Total amount","Tip amount",12); 
    
    DebugAddHEX("Tip amount",srTransRec.szTipAmount,6); 
    DebugAddHEX("Total amount",srTransRec.szTotalAmount,6); 

    if(CARD_ENTRY_ICC == srTransRec.byEntryMode)
    {
		inCTLOS_Updatepowrfail(PFR_EMV_PROCESSING);		
        memset(byDataTmp1, 0x00, sizeof(byDataTmp1));
        memset(byDataTmp2, 0x00, sizeof(byDataTmp2));
        wub_hex_2_str(srTransRec.szTotalAmount, byDataTmp1, 6);
        lnTmp = atol(byDataTmp1);
        wub_long_2_array(lnTmp, byDataTmp2);

        memcpy(&bPackSendBuf[usPackSendLen++], "\x81", 1);
        bPackSendBuf[usPackSendLen++] = 0x04;
        memcpy(&bPackSendBuf[usPackSendLen], byDataTmp2, 4);
        usPackSendLen += 4;
        
        memcpy(srTransRec.stEMVinfo.T9F02, srTransRec.szTotalAmount, 6);
        
        memcpy(&bPackSendBuf[usPackSendLen], "\x9F\x02", 2);
        usPackSendLen += 2;
        bPackSendBuf[usPackSendLen++] = 0x06;
        memcpy(&bPackSendBuf[usPackSendLen], srTransRec.stEMVinfo.T9F02, 6);
        usPackSendLen += 6;

        if(atol(szTipAmount) > 0)
        {
            memcpy(srTransRec.stEMVinfo.T9F03, srTransRec.szTipAmount, 6);
        }
        else
        {
            memset(szTipAmount, 0x00, sizeof(szTipAmount));
            memcpy(srTransRec.stEMVinfo.T9F03, szTipAmount, 6);
        }

        memcpy(&bPackSendBuf[usPackSendLen], "\x9F\x03", 2);
        usPackSendLen += 2;
        bPackSendBuf[usPackSendLen++] = 0x06;
        memcpy(&bPackSendBuf[usPackSendLen], srTransRec.stEMVinfo.T9F03, 6);
        usPackSendLen += 6;

        usCTOSS_EMV_MultiDataSet(usPackSendLen, bPackSendBuf);
    }
    
    return d_OK;
}



int inCTOS_GetOffApproveNO(void)
{
    USHORT usX =1, usY = 6;
    BYTE bShowAttr = 0x02; 
    USHORT szAuthCodeLen = 6;
    BYTE baPIN[6 + 1];
    BYTE   szAuthCode[6+1];
    BYTE bRet;


    memset(szAuthCode, 0x00, sizeof(szAuthCode));


    //CTOS_LCDTClearDisplay();
    vdDispTransTitle(srTransRec.byTransType);
    setLCDPrint(5, DISPLAY_POSITION_LEFT, "Approve NO: ");

    while(TRUE)
    {
        bRet = InputStringAlpha(1, 8, 0x00, 0x02, szAuthCode, &szAuthCodeLen, 0, d_INPUT_TIMEOUT);
        //bRet = InputString(1, 8, 0x00, 0x02, szAuthCode, &szAuthCodeLen, 0, d_INPUT_TIMEOUT);
        if (bRet == d_KBD_CANCEL )
        {
            //CTOS_LCDTClearDisplay();    
            vdDisplayErrorMsg(1, 8, "USER CANCEL");
            return (d_EDM_USER_CANCEL);
        }
        else if (bRet == d_KBD_ENTER ) 
        {
            if(strlen(szAuthCode) >= 6)
            {
                strcpy(srTransRec.szAuthCode, szAuthCode);
                break;
            }
            else
            {
                memset(szAuthCode, 0x00, sizeof(szAuthCode));
                szAuthCodeLen = 6;
            }
        }
		else
            return ST_ERROR;			
    }

    return ST_SUCCESS;
}

int inCTOS_GetInvoice(void)
{

    BYTE key;
    USHORT usX =1, usY = 6;
    BYTE baString[100+1];
    USHORT iStrLen = 6;
    BYTE bShowAttr = 0x02; 
    USHORT usInvoiceLen = 6;
    BYTE baPIN[6 + 1];
    BYTE   szInvNoAsc[6+1];
    BYTE   szInvNoBcd[3];
    BYTE bRet;
    int iLens = 6;
    int  inResult;
    char szBcd[INVOICE_BCD_SIZE+1];
    
    TRANS_DATA_TABLE srTransRecTemp;
    
    DebugAddSTR("inCTOS_GetInvoice","Processing...",20);
	
    if (!strTCT.fManualEntryInv)
    {
        memset(szBcd, 0x00, sizeof(szBcd));
        memcpy(szBcd, strTCT.szInvoiceNo, INVOICE_BCD_SIZE);    
        inBcdAddOne(szBcd, strTCT.szInvoiceNo, INVOICE_BCD_SIZE);
        
        if((inResult = inTCTSave(1)) != ST_SUCCESS)
        {
            vdSetErrorMessage("LOAD TCT ERR");
            return ST_ERROR;
        }

        memcpy(srTransRec.szInvoiceNo,strTCT.szInvoiceNo, 3);
    }
    else
    {
        memset(szInvNoAsc, 0x00, sizeof(szInvNoAsc));
        memset(szInvNoBcd, 0x00, sizeof(szInvNoBcd));
        
        memset((char*)&srTransRecTemp, 0x00, sizeof(TRANS_DATA_TABLE));
        memcpy(&srTransRecTemp, &srTransRec, sizeof(TRANS_DATA_TABLE));
        
        while(TRUE)
        {
            if (strTCT.fECR) // tct
            {
                //If ECR send INV
                if (memcmp(srTransRec.szInvoiceNo, "\x00\x00\x00", 3) != 0)
                {                    
                    inResult = inCTOSS_BatchCheckDuplicateInvoice();
                    vdDebug_LogPrintf("inCTOSS_BatchCheckDuplicateInvoice[%d]", inResult);
                    
                    memcpy(&srTransRec, &srTransRecTemp, sizeof(TRANS_DATA_TABLE));
                    if (d_OK == inResult)
                    {
                        //CTOS_LCDTClearDisplay();    
                        vdDisplayErrorMsg(1, 8, "DUPLICATE INVOICE");

                        return (d_NO);
                    }
                    else
                    {
                        return (d_OK);
                    }
                }
            }
            
            //CTOS_LCDTClearDisplay();
            vdDispTransTitle(srTransRecTemp.byTransType);
            setLCDPrint(5, DISPLAY_POSITION_LEFT, "TRACE NO.: ");

            bRet = InputString(usX, usY, 0x00, bShowAttr, szInvNoAsc, &usInvoiceLen, 1, d_GETPIN_TIMEOUT);
            if (bRet == d_KBD_CANCEL )
            {
                //CTOS_LCDTClearDisplay();    
                vdSetErrorMessage("USER CANCEL");
                memcpy(&srTransRec, &srTransRecTemp, sizeof(TRANS_DATA_TABLE));
                return (d_EDM_USER_CANCEL);
            }
            
            
            if(atoi(szInvNoAsc) != 0)
            {
                inAscii2Bcd(szInvNoAsc, szInvNoBcd, INVOICE_BCD_SIZE);
        
                memcpy(srTransRec.szInvoiceNo,szInvNoBcd,3);
                inResult = inCTOSS_BatchCheckDuplicateInvoice();
                vdDebug_LogPrintf("inCTOSS_BatchCheckDuplicateInvoice[%d]", inResult);
                if (d_OK == inResult)
                {
                    //CTOS_LCDTClearDisplay();    
                    vdDisplayErrorMsg(1, 8, "DUPLICATE INVOICE");
                    memset(szInvNoAsc, 0x00, sizeof(szInvNoAsc));
                    usInvoiceLen = 6;

                    memset(srTransRec.szInvoiceNo, 0x00, sizeof(srTransRec.szInvoiceNo));
                    continue;
                }
                else
                {
                    break;
                }
            }       
        }   

        memcpy(&srTransRec, &srTransRecTemp, sizeof(TRANS_DATA_TABLE));
        memcpy(srTransRec.szInvoiceNo,szInvNoBcd,3);
		
    }
    
    return (d_OK);
}

VS_BOOL fAmountLessThanFloorLimit(void) 
{
    long lnTotalAmount = 0;
    BYTE   szTotalAmt[12+1];
    
    /* If the transaction amount is less than the floor limit,
        Set the transaction type to OFFLINE */

    wub_hex_2_str(srTransRec.szTotalAmount, szTotalAmt, AMT_BCD_SIZE);      
    lnTotalAmount = atol(szTotalAmt);
    
    if (lnTotalAmount < strCDT.InFloorLimitAmount)
        return(d_OK);

    return(d_NO);
}

int inCTOS_CustComputeAndDispTotal(void)
{
	CTOS_RTC SetRTC;
	BYTE	szTotalAmt[12+1];
	BYTE	szTempBuf[12+1];
	BYTE	szTempBuf1[12+1]; 
	BYTE	szDisplayBuf[30];
	BYTE	szStr[45];
	BYTE	szStr1[45];
	
	//CTOS_LCDTClearDisplay();
	vdDispTransTitle(srTransRec.byTransType);

	inCTOS_DisplayCardTitle(5, 6, TRUE);
	
	//setLCDPrint(7, DISPLAY_POSITION_LEFT, "TOTAL AMOUNT :");
	wub_hex_2_str(srTransRec.szTotalAmount, szTotalAmt, AMT_BCD_SIZE);		
	memset(szStr, 0x00, sizeof(szStr));
	memset(szStr1, 0x00, sizeof(szStr1));
	vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szTotalAmt, szStr);
	//setLCDPrint(7, DISPLAY_POSITION_LEFT, strCST.szCurSymbol);
	sprintf(szStr1, "%s %s", "TOTAL", strCST.szCurSymbol);
	setLCDPrint(7, DISPLAY_POSITION_LEFT, szStr1);
	setLCDPrint(7, DISPLAY_POSITION_RIGHT, szStr);

	/*
	if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
	{
		return (d_OK);
	}

	if(srTransRec.byTransType != SALE)
	{
		return (d_OK);
	}
	*/
	
	if (fAmountLessThanFloorLimit() == d_OK)
	{
		srTransRec.shTransResult = TRANS_AUTHORIZED;
		srTransRec.byOffline = CN_TRUE;

		//Read the date and the time //
		CTOS_RTCGet(&SetRTC);  
		/* Set Month & Day*/
		memset(szTempBuf,0,sizeof(szTempBuf));
		sprintf(szTempBuf,"%02d%02d",SetRTC.bMonth,SetRTC.bDay);
		wub_str_2_hex(szTempBuf,srTransRec.szDate,4);
		sprintf(szTempBuf1,"%02d",SetRTC.bYear);
		memcpy(srTransRec.szYear,szTempBuf1,2);
		memset(szTempBuf,0,sizeof(szTempBuf));
		sprintf(szTempBuf,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
		wub_str_2_hex(szTempBuf,srTransRec.szTime,6);
	}

	// Confirmation
	if (inConfirmPAN() != d_OK)
		return d_NO;
	
	return(d_OK);
}




int inCTOS_DisplayResponse(void)
{
    int inMsgid = atoi(srTransRec.szRespCode);
    int inHostIndex = srTransRec.HDTid;
    int inResult = 0;
    char szResponseCode[40], szResponseCode2[40];

    memset(szResponseCode, 0x00, sizeof(szResponseCode2));	
    memset(szResponseCode2, 0x00, sizeof(szResponseCode));
    vdDebug_LogPrintf("inMsgid[%d]inHostIndex[%d]szResponseCode[%s]", inMsgid, inHostIndex, szResponseCode);
    inMSGResponseCodeReadByHostName(szResponseCode, szResponseCode2, inMsgid, strHDT.szAPName);  
    if(0 == strlen(szResponseCode) )
    {
        inMSGResponseCodeRead(szResponseCode, szResponseCode2, inMsgid, 1);
        if(0 != strlen(szResponseCode) )
            vdDisplayErrorMsg2(1, 8, szResponseCode, szResponseCode2, MSG_TYPE_ERROR);
    }
    else
    {
        vdDisplayErrorMsg2(1, 8, szResponseCode, szResponseCode2, MSG_TYPE_ERROR);
    }

    vdDebug_LogPrintf("inMsgid[%d]inHostIndex[%d]szResponseCode[%s]", inMsgid, inHostIndex, szResponseCode);
    return(d_OK);  
}



int inCTOS_GeneralGetInvoice(void)
{
	BYTE key;
	USHORT usX =1, usY = 6;
	BYTE baString[100+1];
	USHORT iStrLen = 6;
	BYTE bShowAttr = 0x02; 
	USHORT usInvoiceLen = 6;
	BYTE baPIN[6 + 1];
	BYTE   szInvNoAsc[6+1];
	BYTE   szInvNoBcd[3];
	BYTE bRet;
	int iLens = 6;
	int  inResult;
	char szBcd[INVOICE_BCD_SIZE+1];

	if (inMultiAP_CheckSubAPStatus() == d_OK)
	{
		return d_OK;
	}

	memset(szInvNoAsc, 0x00, sizeof(szInvNoAsc));
	memset(szInvNoBcd, 0x00, sizeof(szInvNoBcd));
	
	//CTOS_LCDTClearDisplay();
	vdDispTransTitle(srTransRec.byTransType);
	setLCDPrint(5, DISPLAY_POSITION_LEFT, "TRACE NO.: ");
	
	while(TRUE)
	{
		usInvoiceLen = 6;
		vdDebug_LogPrintf("11bRet[%d]atoi(szInvNoAsc)=[%d]usInvoiceLen[%d]",bRet,atoi(szInvNoAsc),usInvoiceLen);
		bRet = InputString(usX, usY, 0x00, bShowAttr, szInvNoAsc, &usInvoiceLen, 1, d_GETPIN_TIMEOUT);
		vdDebug_LogPrintf("bRet[%d]atoi(szInvNoAsc)=[%d]usInvoiceLen[%d]",bRet,atoi(szInvNoAsc),usInvoiceLen);
		if (bRet == d_KBD_CANCEL )
		{
			//CTOS_LCDTClearDisplay();	
			vdSetErrorMessage("USER CANCEL");
			return (d_EDM_USER_CANCEL);
		}
		
		
		if(atoi(szInvNoAsc) != 0)
		{
			inAscii2Bcd(szInvNoAsc, szInvNoBcd, INVOICE_BCD_SIZE);
			DebugAddSTR("INV NUM:",szInvNoAsc,12);	
			memcpy(srTransRec.szInvoiceNo,szInvNoBcd,3);
			break;
		}		
	}

	return ST_SUCCESS;
}



int inCTOS_BatchSearch(void)
{
	int inResult = d_NO;
	char szInvoiceNo[6+1];

	vdDebug_LogPrintf("--inCTOS_BatchSearch--");
	
	memset(szInvoiceNo, 0x00, sizeof(szInvoiceNo));
	wub_hex_2_str(srTransRec.szInvoiceNo, szInvoiceNo, 3);
	vdDebug_LogPrintf("szInvoiceNo=[%s]", szInvoiceNo);
#if 0
	inResult = inDatabase_BancnetBatchSearch(&srTransRec, szInvoiceNo);
	if(inResult != d_OK)
	{
		inResult = inDatabase_BatchSearch(&srTransRec, srTransRec.szInvoiceNo);
	}
#else
vdDebug_LogPrintf("AAA - srTransRec.szTranYear[%s]", srTransRec.szTranYear);
inResult = inDatabase_BatchSearch(&srTransRec, srTransRec.szInvoiceNo);
if(inResult != d_OK)
{
	 if (inMultiAP_CheckSubAPStatus() != d_OK)
		  vdSetErrorMessage("NO RECORD FOUND");
	 return d_NOT_RECORD;
}
#endif
	DebugAddSTR("inCTOS_BatchSearch","Processing...",20);
	
	DebugAddINT("inCTOS_BatchSearch", inResult);

	if(inResult != d_OK)
	{	
		if (inMultiAP_CheckSubAPStatus() != d_OK)
			vdSetErrorMessage("NO RECORD FOUND");
		return d_NOT_RECORD;
	}
	
	memcpy(srTransRec.szOrgDate, srTransRec.szDate, 2);
	memcpy(srTransRec.szOrgTime, srTransRec.szTime, 2);
	
	return inResult;
}


int inCTOS_LoadCDTandIIT(void)
{
	if ( inCDTRead(srTransRec.CDTid) != d_OK)
	{
        vdDebug_LogPrintf("inCTOS_LoadCDTandIIT LOAD CDT ERROR [%d]", srTransRec.CDTid);
		vdSetErrorMessage("LOAD CDT ERROR");
		return(d_NO);
    }
    inGetIssuerRecord(strCDT.IITid);

    return ST_SUCCESS;
}


int inCTOS_CheckTipadjust()
{
    inCDTRead(srTransRec.CDTid);
    if(srTransRec.byTransType == PRE_AUTH || strCDT.inType == DEBIT_CARD
    || srTransRec.fInstallment == TRUE || srTransRec.fDebit == TRUE  || srTransRec.fLoyalty == TRUE)
    {
        vdSetErrorMessage("TRANS NOT ALLWD");
        return d_NO;
    }
	
    if ((srTransRec.byTransType == SALE) || (srTransRec.byTransType == SALE_TIP) || (srTransRec.byTransType == SALE_OFFLINE))
    {
        if((srTransRec.byTransType == SALE) || (srTransRec.byTransType == SALE_OFFLINE))
            srTransRec.byOrgTransType = srTransRec.byTransType;
        
        memcpy(srTransRec.szOrgAmount, srTransRec.szTotalAmount, 6);
        memcpy(srTransRec.szOrgTipAmount, srTransRec.szTipAmount, 6);
    }
    else
    {
        vdSetErrorMessage("TIP NOT ALLWD");
        return d_NO;
    }

    srTransRec.byTransType = SALE_TIP;

    return d_OK;
}

int inCTOS_CheckVOID()
{
    if(srTransRec.byVoided == TRUE)
    {
        vdSetErrorMessage("RECORD VOIDED");
		#if 1
		vdDebug_LogPrintf("srTransRec.byVoided[%d]", srTransRec.byVoided);
        if(get_env_int("ECRPROCESS")==1)
        {
             strcpy(srTransRec.szECRRespCode, ECR_DECLINED_ERR);
             strcpy(srTransRec.szResponseText, ECR_RECORD_VOIDED);
             //inMultiAP_ECRSendSuccessResponse();
        }
		#endif
        return d_NO;
    }

	inCDTRead(srTransRec.CDTid);
    if(srTransRec.byTransType == PRE_AUTH || strCDT.inType == DEBIT_CARD)
    {
        vdSetErrorMessage("TRANS NOT ALLWD");
        return d_NO;
    }
		
    //to find the original transaction of the sale tip to be voided - Meena 26/12/12 - START
    if(srTransRec.byTransType == SALE_TIP)
    {
        szOriginTipTrType = srTransRec.byOrgTransType;
        srTransRec.byOrgTransType = srTransRec.byTransType;
    }
    else //to find the original transaction of the sale tip to be voided - Meena 26/12/12 - END
        srTransRec.byOrgTransType = srTransRec.byTransType;

    srTransRec.byTransType = VOID;

    return d_OK;
}


int inCTOS_VoidSelectHost(void) 
{
    short shGroupId ;
    int inHostIndex;
    short shCommLink;
    int inCurrencyIdx=0;

    inHostIndex = (short) srTransRec.HDTid;
    
     DebugAddSTR("inCTOS_SelectHost","Processing...",20);
     
    if ( inHDTRead(inHostIndex) != d_OK)
    {
        vdSetErrorMessage("HOST SELECTION ERR");
        return(d_NO);
    } 
    else 
    {
        
        srTransRec.HDTid = inHostIndex;

        inCurrencyIdx = strHDT.inCurrencyIdx;

        if (inCSTRead(inCurrencyIdx) != d_OK) {
            
        vdSetErrorMessage("LOAD CST ERR");
        return(d_NO);
        }

        if ( inMMTReadRecord(inHostIndex,srTransRec.MITid) != d_OK)
        {
            vdSetErrorMessage("LOAD MMT ERR");
            return(d_NO);
        }

        if ( inCPTRead(inHostIndex) != d_OK)
        {
            vdSetErrorMessage("LOAD CPT ERR");
            return(d_NO);
        }

        inCTOS_PreConnect();

        return (d_OK);
    }

    
}

int inCTOS_ChkBatchEmpty()
{
    int         inResult;
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;
    
    memset(&srAccumRec,0,sizeof(srAccumRec));
    memset(&strFile,0,sizeof(strFile));
    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    memset(&strFile,0,sizeof(strFile));

	vdDebug_LogPrintf("test lang inCTOS_ChkBatchEmpty %d - %d", strHDT.inHostIndex, srTransRec.MITid);
    vdCTOS_GetAccumName(&strFile, &srAccumRec);

	vdDebug_LogPrintf("test lang filename = %s", strFile.szFileName);

    if((inResult = inMyFile_CheckFileExist(strFile.szFileName)) < 0)
    {
        if(CN_TRUE == strMMT[0].fMustSettFlag)
        {
            strMMT[0].fMustSettFlag = CN_FALSE;
            inMMTSave(strMMT[0].MMTid);
        }
        
        //CTOS_LCDTClearDisplay();
        //vdSetErrorMessage("BATCH EMPTY");
        vdDisplayErrorMsg(1, 8, "BATCH EMPTY");
        return (d_NO);
    }
    
    return (d_OK);
}

int inCTOS_ConfirmInvAmt()
{
    BYTE szAmtTmp1[20+1];
    BYTE szAmtTmp2[16+1];
    char szDisplayBuf[30];
    int  inRet = d_NO;
    USHORT result;
	BYTE szTitle[16+1];
	BYTE szOrgTransTitle[16+1];
	BYTE szTemp[32 + 1] = {0};
	BYTE szDisplayIssuer[40+1];
	BYTE szTotalAmount[12+1];
    BYTE szAmtOut[12+1];
	BYTE szINV[6+1];
	BYTE szDisMsg[30];
	char szTempDate[d_LINE_SIZE + 1];
	char szTemp1[d_LINE_SIZE + 1];
	char szTemp2[d_LINE_SIZE + 1];
	char szTemp4[d_LINE_SIZE + 1];
	char szTemp5[d_LINE_SIZE + 1];
	BYTE szTrxnDateTime[20+1];
	BYTE szConversion[d_LINE_SIZE+1];
	BYTE szStringMsg[1028 + 1] = {0};
	
	vdDebug_LogPrintf("--inCTOS_ConfirmInvAmt--");
	
    //CTOS_LCDTClearDisplay();
    vduiLightOn();

	inIITRead(srTransRec.IITid);
	
    vdDispTransTitle(srTransRec.byTransType);

	memset(szTitle, 0x00, sizeof(szTitle));
	memset(szOrgTransTitle, 0x00, sizeof(szOrgTransTitle));
    //szGetTransTitle(srTransRec.byTransType, szTitle);
	szGetTransTitle(srTransRec.byOrgTransType,szOrgTransTitle);

	//vdDebug_LogPrintf("szGetTransTitle, szTitle=[%s]", szTitle);
	vdDebug_LogPrintf("crashtest szGetTransTitle, szOrgTransTitle=[%s]", szOrgTransTitle);

//fix for crash
#if 0
	memset(szDisplayIssuer,0x00,sizeof(szDisplayIssuer));
	strcpy(szDisplayIssuer, strIIT.szIssuerLabel);


    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    sprintf(szDisplayBuf, "%s %s",szDisplayIssuer,szTitle);//Removed issuer on Batch Review. Issue 667

	CTOS_LCDTPrintXY(1, 2, szDisplayBuf); // TrxnName
		
	memset(szDisplayBuf,0,sizeof(szDisplayBuf));		
	strcpy(szDisplayBuf, srTransRec.szXMLtransaction_id); 


	if (strlen(szDisplayBuf) > 20)
	{
		memset(szTemp, 0x00, sizeof(szTemp));
		strncpy(szTemp, (char*) &szDisplayBuf[0], 20);
		CTOS_LCDTPrintXY(1, 3, szTemp);
			
		memset(szTemp, 0x00, sizeof(szTemp));
		strncpy(szTemp, (char*) &szDisplayBuf[20], strlen(szDisplayBuf) - 20);
		CTOS_LCDTPrintXY(1, 4, szTemp);	
	}
	else
	{
		CTOS_LCDTPrintXY(1, 3, szDisplayBuf);
	}
#endif
    vdDebug_LogPrintf("crash test 1");
	// Amount
	memset(szTotalAmount, 0x00, sizeof(szTotalAmount));
    wub_hex_2_str(srTransRec.szTotalAmount, szTotalAmount, 6);
    vdDebug_LogPrintf("szTotalAmount[%s]", szTotalAmount);
    vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTotalAmount,szAmtOut);
    vdDebug_LogPrintf(" crashtest szAmtOut[%s]", szAmtOut);
    memset(szDisplayBuf,0,sizeof(szDisplayBuf));
    sprintf(szDisplayBuf, "%s%s", strCST.szCurSymbol, szAmtOut);
	strcpy(szAmtTmp1, szDisplayBuf);
	vdDebug_LogPrintf(" crashtest szAmtTmp1[%s]", szAmtTmp1);


//fix for crash
#if 0
	CTOS_LCDTPrintXY(1, 5, szDisplayBuf);
#endif
    vdDebug_LogPrintf("crash test 2");

	// Conversion
	memset(szTemp1, 0x00, sizeof(szTemp1));
	memset(szConversion, 0x00, sizeof(szConversion));
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", srTransRec.szXMLcash_fee, szTemp1);
	sprintf(szConversion,"%s%s",srTransRec.szXMLcash_fee_type, szTemp1);

    vdDebug_LogPrintf("crash test 3");
	
	// Invoice No.
	memset(szINV, 0x00, sizeof(szINV));
	wub_hex_2_str(srTransRec.szInvoiceNo, szINV, INVOICE_BCD_SIZE);

//fix for crash
#if 0
	memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
	sprintf(szDisplayBuf, "TRACE NO.: %ld", atol(szINV));
	CTOS_LCDTPrintXY(1, 6, szDisplayBuf);
#endif
    vdDebug_LogPrintf("crash test 4");

	// Date
	memset(szTrxnDateTime, 0x00, sizeof(szTrxnDateTime));
	iXMLFormatDateTime((char*)srTransRec.szXMLtime_end, szTrxnDateTime);

    vdDebug_LogPrintf("crash test 5");

	// TransName
	memset(szTitle, 0x00, sizeof(szTitle));
	szGetTransTitle(srTransRec.byTransType, szTitle);
	vdDebug_LogPrintf("szGetTransTitle, szTitle=[%s]", szTitle);
	
	memset(szStringMsg, 0x00, sizeof(szStringMsg));
	strcpy(szStringMsg,szTitle);		
	strcat(szStringMsg,"|");
	strcat(szStringMsg,szOrgTransTitle);
	strcat(szStringMsg,"|");
	strcat(szStringMsg,szAmtTmp1);
	strcat(szStringMsg,"|");
	strcat(szStringMsg,szConversion);
	strcat(szStringMsg,"|");
	strcat(szStringMsg,srTransRec.szXMLtransaction_id);
	strcat(szStringMsg,"|");
	strcat(szStringMsg,szINV);
	strcat(szStringMsg,"|");
	strcat(szStringMsg,szTrxnDateTime);
	strcat(szStringMsg,"|");
	strcat(szStringMsg,srTransRec.szHostLabel);
	strcat(szStringMsg,"|");

	vdDebug_LogPrintf("saturn usCTOSS_ConfirmMenu szStringMsg[%s]", szStringMsg);
	
	inRet = usCTOSS_ConfirmInvAndAmt(szStringMsg);

	vdDebug_LogPrintf("usCTOSS_ConfirmInvAndAmt, inRet=[%d]", inRet);

	switch (inRet)
	{
		case d_TIMER_INVALID_PARA:
		case d_TIMEOUT:
			//vdDisplayScreenMessage("TIMEOUT");
			vdDisplayMessageBox(1, 8, "", "TIMEOUT", "", MSG_TYPE_INFO);
			CTOS_Beep();
			if(get_env_int("ECRPROCESS")==1)
           {
                strcpy(srTransRec.szECRRespCode, ECR_TIMEOUT_ERR);
                strcpy(srTransRec.szResponseText, ECR_TIMEOUT_RESP);
                //inMultiAP_ECRSendSuccessResponse();
           }
			CTOS_Delay(1000);
			break;
		case d_USER_CANCEL:
			//vdDisplayScreenMessage("USER CANCEL");
			vdDisplayMessageBox(1, 8, "", "USER CANCEL", "", MSG_TYPE_INFO);
			CTOS_Beep();
           if(get_env_int("ECRPROCESS")==1)
           {
                strcpy(srTransRec.szECRRespCode, ECR_UNKNOWN_ERR);
                strcpy(srTransRec.szResponseText, ECR_OPER_CANCEL_RESP);
                //inMultiAP_ECRSendSuccessResponse();
           }
			CTOS_Delay(1000);
			break;
		case d_OK:
			//vdDisplayScreenMessage("PROCESSING...");
			vdCTOS_DispStatusMessage("PROCESSING");
			break;
		default:
			//vdDisplayScreenMessage("PROCESSING...");
			vdCTOS_DispStatusMessage("PROCESSING");
			break;
	}
	
	return inRet;
	
}

int inCTOS_SettlementSelectAndLoadHost(void)
{
    int key;
    char szBcd[INVOICE_BCD_SIZE+1];
    
    if (inMultiAP_CheckSubAPStatus() == d_OK)
        return d_OK;
    
    key = inCTOS_SelectHostSetting();
    if (key == -1)
    {
        return key;
    }

    /*albert - start - fix stan issue*/
    //memset(szBcd, 0x00, sizeof(szBcd));
    //memcpy(szBcd, strHDT.szTraceNo, INVOICE_BCD_SIZE);    
    //inBcdAddOne(szBcd, strHDT.szTraceNo, INVOICE_BCD_SIZE);
    //srTransRec.ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
    //inHDTSave(strHDT.inHostIndex);
	/*albert - end - fix stan issue*/

    return d_OK;
}

int inCTOS_SettlementClearBathAndAccum(BOOL fManualSettle)
{
    int         inResult;
    BYTE    szBcd[INVOICE_BCD_SIZE+1];
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;

    vdDebug_LogPrintf("inCTOS_SettlementClearBathAndAccum");
    
    memset(szBcd, 0x00, sizeof(szBcd));
    memcpy(szBcd, strMMT[0].szBatchNo, INVOICE_BCD_SIZE);    
    inBcdAddOne(szBcd, strMMT[0].szBatchNo, INVOICE_BCD_SIZE);
    strMMT[0].fMustSettFlag = CN_FALSE;
    inMMTSave(strMMT[0].MMTid);
        
	/*albert - start - 20161202 - Reprint of Detail Report for Last Settlement Report*/
	inDatabase_DeleteDetailReport(srTransRec.HDTid, srTransRec.MITid);
	inDatabase_BackupDetailReport(srTransRec.HDTid, srTransRec.MITid);
	/*albert - end - 20161202 - Reprint of Detail Report for Last Settlement Report*/

	strMMT[0].fBatchNotEmpty = 0;	
	inMMTBatchNotEmptySave(strMMT[0].MMTid); 
	
    inDatabase_BatchDelete();

    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    inCTOS_ReadAccumTotal(&srAccumRec);
    strcpy(srAccumRec.szTID, srTransRec.szTID);
    strcpy(srAccumRec.szMID, srTransRec.szMID);
    memcpy(srAccumRec.szYear, srTransRec.szYear, DATE_BCD_SIZE);
    memcpy(srAccumRec.szDate, srTransRec.szDate, DATE_BCD_SIZE);
    memcpy(srAccumRec.szTime, srTransRec.szTime, TIME_BCD_SIZE);
    memcpy(srAccumRec.szBatchNo, srTransRec.szBatchNo, BATCH_NO_BCD_SIZE);
	srAccumRec.fManualSettle=fManualSettle;
    inCTOS_SaveAccumTotal(&srAccumRec);
    
    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    memset(&strFile,0,sizeof(strFile));
    vdCTOS_GetAccumName(&strFile, &srAccumRec);
    vdDebug_LogPrintf("[strFile.szFileName[%s]", strFile.szFileName);
    vdCTOS_SetBackupAccumFile(strFile.szFileName);
    if((inResult = CTOS_FileDelete(strFile.szFileName)) != d_OK)
    {
        vdDebug_LogPrintf("[inMyFile_SettleRecordDelete]---Delete Record error[%04x]", inResult);
    }

    inDatabase_InvoiceNumDelete(srTransRec.HDTid, srTransRec.MITid);
	
    inMyFile_ReversalDelete();

    inMyFile_AdviceDelete();

    inMyFile_TCUploadDelete();

	// sidumili: Delete retrieve record
	inDatabase_RetrieveBatchDelete();

    return d_OK;
}

int inCTOS_DisplayBatchTotal(void)
{
    int inResult;
    int inTranCardType;
    BYTE szDisplayBuf[40];
    BYTE szAmtBuf[40];
    BYTE szBuf[40];
    USHORT usSaleCount;
    ULONG ulSaleTotalAmount;
    USHORT usRefundCount;
    ULONG  ulRefundTotalAmount;
    ACCUM_REC srAccumRec;

    //CTOS_LCDTClearDisplay();
    vdDispTransTitle(BATCH_TOTAL);
    
    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    if((inResult = inCTOS_ReadAccumTotal(&srAccumRec)) != ST_SUCCESS)
    {
        vdDebug_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
        //CTOS_LCDTClearDisplay();
        vdSetErrorMessage("BATCH EMPTY");
        return ST_ERROR;
    }

    //0 is for Credit type, 1 is for debit type
    inTranCardType = 0;

    usSaleCount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount);
    ulSaleTotalAmount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulSaleTotalAmount);

    usRefundCount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usRefundCount);
    ulRefundTotalAmount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulRefundTotalAmount);
    
    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    strcpy(szDisplayBuf, "Type  Cnt  Amt");
    CTOS_LCDTPrintXY(1, 3, szDisplayBuf);


    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    memset(szAmtBuf, 0x00, sizeof(szAmtBuf));
    memset(szBuf, 0x00, sizeof(szBuf));
    sprintf(szBuf, "%ld", ulSaleTotalAmount);
    vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szBuf,szAmtBuf);
    sprintf(szDisplayBuf, "SALE  %03d  %s", usSaleCount, szAmtBuf);
    CTOS_LCDTPrintXY(1, 5, szDisplayBuf);
    
    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    memset(szAmtBuf, 0x00, sizeof(szAmtBuf));
    memset(szBuf, 0x00, sizeof(szBuf));
    sprintf(szBuf, "%ld", ulRefundTotalAmount);
    vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szBuf,szAmtBuf);
    sprintf(szDisplayBuf, "RFND  %03d  %s", usRefundCount, szAmtBuf);
    CTOS_LCDTPrintXY(1, 6, szDisplayBuf);

    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    memset(szAmtBuf, 0x00, sizeof(szAmtBuf));
    memset(szBuf, 0x00, sizeof(szBuf));
    sprintf(szBuf, "%ld", (ulSaleTotalAmount > ulRefundTotalAmount) ? (ulSaleTotalAmount-ulRefundTotalAmount) : (ulRefundTotalAmount - ulSaleTotalAmount));
    vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szBuf,szAmtBuf);
    memset(szBuf, 0x00, sizeof(szBuf));
    if(ulSaleTotalAmount > ulRefundTotalAmount)
        szBuf[0] = ' ';
    else
        szBuf[0] = '-';
    sprintf(szDisplayBuf, "TOTL  %03d %s%s", (usSaleCount + usRefundCount), szBuf, szAmtBuf);
    CTOS_LCDTPrintXY(1, 8, szDisplayBuf);

    WaitKey(30);

    return d_OK;
}

int inCTOS_DisplaySettleBatchTotal(BYTE byTransType, BOOL fConfirm)
{
	int inResult;
	int inTranCardType;
	BYTE szDisplayBuf[40];
	BYTE szAmtBuf[40];
	BYTE szBuf[40];
	USHORT usSaleCount;
	ULONG ulSaleTotalAmount;
	USHORT usRefundCount;
	ULONG  ulRefundTotalAmount;
	ACCUM_REC srAccumRec;
	BYTE byKeyBuf; 
	USHORT usVoidCount;
	ULONG ulVoidTotalAmount;
	int inRow=2;
	
	
	USHORT usTotalCount;
	ULONG ulTotalAmount;

	 int key=d_NO;
	 BYTE szDispUI[400];
	 
	 BYTE szDisplayBuf1[40];
	 BYTE szDisplayBuf2[40];
	 BYTE szDisplayBuf3[40];
	 BYTE szDisplayBuf4[40];
	 BYTE szDisplayBuf5[40];
	
	BYTE szTitle[20+1];

	int inConfirmTimeOut = 30;

	BYTE szCount[40];
	BYTE szTemp[40];
	BYTE szConfirm[2] = {0};
	BYTE szIssuerLabel[20] = {0};


	vdDebug_LogPrintf("SATURN inCTOS_DisplaySettleBatchTotal");	
	vdDebug_LogPrintf("fAutoSettleFlag=[%d]", srTransRec.fAutoSettleFlag);

	if (srTransRec.fAutoSettleFlag == TRUE)
	{
		fConfirm = FALSE;
		inConfirmTimeOut = 1;
	}

	vdDebug_LogPrintf("fConfirm=[%d]", fConfirm);
	
	//CTOS_LCDTClearDisplay();
	//vdDispTransTitle(byTransType);
	
	memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
	if((inResult = inCTOS_ReadAccumTotal(&srAccumRec)) == ST_ERROR)
	{
		vdDebug_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
		return ST_ERROR;	
	}

	if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		strcpy(szIssuerLabel, "ALIPAY");

	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		strcpy(szIssuerLabel, "WECHAT");

	if (srTransRec.HDTid == GCASH_HOST_INDEX)
		strcpy(szIssuerLabel, "GCASH");

	if (srTransRec.HDTid == GRAB_HOST_INDEX)
		strcpy(szIssuerLabel, "GRAB PAY");

	if (srTransRec.HDTid == UPI_HOST_INDEX)
		strcpy(szIssuerLabel, "UPI");

	
	if (srTransRec.HDTid == PHQR_HOST_INDEX)
		strcpy(szIssuerLabel, "PHQR");
	
	//0 is for Credit type, 1 is for debit type
	inTranCardType = 0;

	usSaleCount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount);
	ulSaleTotalAmount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulSaleTotalAmount);

	usVoidCount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usVoidSaleCount);
	ulVoidTotalAmount = (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulVoidSaleTotalAmount);

	memset(szConfirm, 0x00, sizeof(szConfirm));
	sprintf(szConfirm, "%d", (fConfirm ? TRUE : FALSE));
	
	// Sale
	memset(szDisplayBuf1, 0x00, sizeof(szDisplayBuf1));
	memset(szAmtBuf, 0x00, sizeof(szAmtBuf));
	memset(szBuf, 0x00, sizeof(szBuf));
	sprintf(szBuf, "%ld", ulSaleTotalAmount); 
	vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szBuf,szAmtBuf);
	sprintf(szDisplayBuf1,"%s %s",strCST.szCurSymbol,szAmtBuf);	
	
	vdDebug_LogPrintf("SATURN  inCTOS_DisplaySettleBatchTotal confirm");


	  memset(szDispUI, 0x00, sizeof(szDispUI)); 	  
	  if (srTransRec.byTransType == SETTLE)
	  { 					
		//strcpy(szDispUI, "SETTLEMENT|SALE:|");
		
		memset(szCount, 0x00, sizeof(szCount));
		memset(szTemp, 0x00, sizeof(szTemp));			
		sprintf(szCount, "%03d", usSaleCount);
		sprintf(szTemp, "%s|%s|SETTLEMENT|SALE:|%s|", szConfirm, szIssuerLabel, szCount);
		strcpy(szDispUI, szTemp);
		
	  }
		
	  else
	  {
		//strcpy(szDispUI, "BATCH TOTAL|SALE:|");

		memset(szCount, 0x00, sizeof(szCount));
		memset(szTemp, 0x00, sizeof(szTemp));			
		sprintf(szCount, "%03d", usSaleCount);
		sprintf(szTemp, "%s|%s|BATCH TOTAL|SALE:|%s|", szConfirm, szIssuerLabel, szCount);
		strcpy(szDispUI, szTemp);
	  }
	   
	   strcat(szDispUI, szDisplayBuf1);
	   strcat(szDispUI, "|");

       if (srTransRec.HDTid == PHQR_HOST_INDEX){
		   strcat(szDispUI, " | | |");

       }else{
	   		// For Void
	   		memset(szDisplayBuf4, 0x00, sizeof(szDisplayBuf4));
	   		memset(szAmtBuf, 0x00, sizeof(szAmtBuf));
	   		memset(szBuf, 0x00, sizeof(szBuf));
	   		sprintf(szBuf, "%ld", ulVoidTotalAmount);
	   		vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szBuf,szAmtBuf);	   
	   		sprintf(szDisplayBuf3, "%s%s%s", strCST.szCurSymbol, "-", szAmtBuf);
		
	   		memset(szCount, 0x00, sizeof(szCount));
	   		sprintf(szCount, "%03d", usVoidCount);
	   		sprintf(szTemp, "VOID:|%s|", szCount);    
	   		strcat(szDispUI, szTemp);
	   		strcat(szDispUI, (strlen(szDisplayBuf3) > 0 ? szDisplayBuf3 : "PHP 0.00"));
	   		strcat(szDispUI, "|");
       }
			 	   
	   // Total Count / Amount
	   usTotalCount = usSaleCount;
	   ulTotalAmount = ulSaleTotalAmount;
	   sprintf(szBuf, "%ld", ulTotalAmount);
	   vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szBuf,szAmtBuf);	 
	   sprintf(szDisplayBuf5, "%s%s", strCST.szCurSymbol, szAmtBuf);

	   // For Total
	   memset(szCount, 0x00, sizeof(szCount));
	   memset(szTemp, 0x00, sizeof(szTemp));			
	   sprintf(szCount, "%03d", usTotalCount);
	   sprintf(szTemp, "TOTALS:|%s|", szCount);
	   strcat(szDispUI, szTemp);			
	   strcat(szDispUI, szDisplayBuf5);

	   // sidumili
	   // Eg data: 0|WECHAT|BATCH TOTAL|SALE:|000|PHP 0.00|VOID:|001|PHP 1.00|TOTALS:|000|PHP-1.00
	   vdDebug_LogPrintf("SATURN  inCTOS_DisplaySettleBatchTotal %s inConfirmTimeOut %d", szDispUI, inConfirmTimeOut);
	   
	   key = usCTOSS_Confirm(szDispUI);
	   vdDebug_LogPrintf("usCTOSS_Confirm, key=[%d]", key);

		if (fConfirm)
		{
			if (key == d_USER_CANCEL) /*timeout and key cancel*/
			{					 
				memset(szTitle, 0x00, sizeof(szTitle));
				szGetTransTitle(srTransRec.byTransType, szTitle);

				vdDisplayMessageBox(1, 8, "", "USER CANCEL", "", MSG_TYPE_INFO);
				CTOS_Beep();
				CTOS_Delay(1000);
				return d_NO;
			}

			// sidumili: timeout display message
			if (key == d_TIMEOUT)
			{
				vdDisplayMessageBox(1, 8, "", "TIME OUT", "", MSG_TYPE_TIMEOUT);
				CTOS_Beep();
				CTOS_Delay(2000);
				return d_NO;
			}

			if (key != d_OK)
				return d_NO;
		}
			
	return d_OK;
	   
}



int inCTOS_DisplayBatchRecordDetail(int inType, int inNavRecType)
{
    BYTE szTotalAmount[12+1];
    BYTE szAmtOut[12+1];
    BYTE szINV[6+1];
    BYTE szDisplayBuf[40+1];
    BYTE szTitle[16+1];
	int inPanlen;
	BYTE szTrxnDateTime[20 + 1] = {0};
	BYTE szTemp[32 + 1] = {0};
	BYTE szDisplayIssuer[40+1];
	BYTE inKey = d_NO;   
    BYTE szDisplayBuf2[40+1];
	BYTE szDisplayTransaction_id[32 + 1];
	BYTE szDisplayAmount[40+1];
	BYTE szDisplayInvNo[40+1];
	BYTE szDisplayDateTime[40+1];
	BYTE szDispUI[400];
	BYTE szNavRecType[2+1]={0};
	
   	vdDebug_LogPrintf("--inCTOS_DisplayBatchRecordDetail--");
	vdDebug_LogPrintf("inNavRecType=[%d]", inNavRecType);

	memset(szNavRecType, 0x00, sizeof(szNavRecType));
	sprintf(szNavRecType, "%d", inNavRecType);
	
	//CTOS_LCDTClearDisplay();  

	memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
	
    vdDispTransTitle(inType);

    inIITRead(srTransRec.IITid);
	inCSTRead(1);
    
    memset(szTitle, 0x00, sizeof(szTitle));
    szGetTransTitle(srTransRec.byTransType, szTitle);
	
	memset(szDisplayIssuer,0x00,sizeof(szDisplayIssuer));
	strcpy(szDisplayIssuer, strIIT.szIssuerLabel);
			
    memset(szDisplayBuf, 0x00, sizeof(szDisplayBuf));
    sprintf(szDisplayBuf, "%s %s",szDisplayIssuer,szTitle);//Removed issuer on Batch Review. Issue 667

	// DateTime
	memset(szDisplayDateTime, 0x00, sizeof(szDisplayDateTime));
	iXMLFormatDateTime((char*)srTransRec.szXMLtime_end, szDisplayDateTime);

	//CTOS_LCDTPrintXY(1, 2, szDisplayBuf); // TrxnName

	memset(szDisplayTransaction_id,0,sizeof(szDisplayTransaction_id));		

   // if (srTransRec.HDTid == PHQR_HOST_INDEX)
	//	strcpy(szDisplayTransaction_id, srTransRec.szXMLtransaction_id); 	

   // else
		strcpy(szDisplayTransaction_id, srTransRec.szXMLtransaction_id); 	
    

    memset(szTotalAmount, 0x00, sizeof(szTotalAmount));
    wub_hex_2_str(srTransRec.szTotalAmount, szTotalAmount, 6);
    vdDebug_LogPrintf("szTotalAmount[%s]", szTotalAmount);
    vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTotalAmount,szAmtOut);
    vdDebug_LogPrintf("szAmtOut[%s]", szAmtOut);
    memset(szDisplayAmount,0,sizeof(szDisplayAmount));
    sprintf(szDisplayAmount, "%s%s", strCST.szCurSymbol, szAmtOut);
	//CTOS_LCDTPrintXY(1, 5, szDisplayBuf);

	#if 0
	// Date/Time Trxn
	memset(szTrxnDateTime, 0x00, sizeof(szTrxnDateTime));
	iXMLFormatDateTime((char*)srTransRec.szXMLtime_end, szTrxnDateTime);
	CTOS_LCDTPrintXY(1, 6, szTrxnDateTime);
	#endif


	
    if (srTransRec.HDTid == PHQR_HOST_INDEX){	
		memset(szDisplayInvNo, 0x00, sizeof(szDisplayInvNo));
		strcpy(szDisplayInvNo,srTransRec.szXMLout_transaction_id);

    }else{
		// Invoice No.
		memset(szINV, 0x00, sizeof(szINV));
		wub_hex_2_str(srTransRec.szInvoiceNo, szINV, 3);
		memset(szDisplayInvNo, 0x00, sizeof(szDisplayInvNo));
		sprintf(szDisplayInvNo, "%06ld", atol(szINV));
		//CTOS_LCDTPrintXY(1, 6, szDisplayBuf);
    }

	#if 0
	if ((strTCT.byTerminalType%2) == 0)
	{
		CTOS_LCDTPrintAligned(8, "00->UP", d_LCD_ALIGNLEFT);
		CTOS_LCDTPrintAligned(8, "DOT->DOWN", d_LCD_ALIGNRIGHT);
	}
	else
    vdCTOS_LCDGShowUpDown(1,1);

	if (srTransRec.fBRVoidFlag == TRUE)
	{
		if(srTransRec.byVoided != TRUE)
		{
			if(inType == SALE_TIP)
				vduiDisplayStringCenter(7,"CONFIRM TIP ADJUST");			
			else
				vduiDisplayStringCenter(7,"CONFIRM VOID");
		
			vduiDisplayStringCenter(8,"NO[X] YES[OK]");
		}
		else
		{
			CTOS_LCDTPrintXY(1, 7, "VOIDED");
		}
	}
    #endif

	memset(szDispUI, 0x00, sizeof(szDispUI));
	strcpy(szDispUI, "BATCH REVIEW");	
	strcat(szDispUI, "|");
	strcat(szDispUI, szTitle);
	strcat(szDispUI, "|");
	strcat(szDispUI, szDisplayIssuer);
	strcat(szDispUI, "|");
	strcat(szDispUI, szDisplayDateTime);
	strcat(szDispUI, "|");
	strcat(szDispUI, szDisplayTransaction_id);
	strcat(szDispUI, "|");
	strcat(szDispUI, szDisplayAmount);
	strcat(szDispUI, "|");
	strcat(szDispUI, szDisplayInvNo);
	strcat(szDispUI, "|");
	strcat(szDispUI, szNavRecType);

	vdDebug_LogPrintf("inCTOS_DisplayBatchRecordDetail, szDispUI[%s]", szDispUI);

    if (srTransRec.HDTid == PHQR_HOST_INDEX)
		inKey=usCTOSS_BatchReviewUI_PHQR(szDispUI);
	else
		inKey=usCTOSS_BatchReviewUI(szDispUI);
	
    return inKey;
}

int inCTOSS_DeleteAdviceByINV(BYTE *szInvoiceNo)
{
    int inResult,inUpDateAdviceIndex;
    TRANS_DATA_TABLE srAdvTransTable;
    STRUCT_ADVICE strAdvice;
    
    memset((char *)&srAdvTransTable, 0, sizeof(TRANS_DATA_TABLE));
    memset((char *)&strAdvice, 0, sizeof(strAdvice));
    
    memcpy((char *)&srAdvTransTable, (char *)&srTransRec, sizeof(TRANS_DATA_TABLE));
    
    inResult = ST_SUCCESS;
    inUpDateAdviceIndex = 0;
    while(1)
    {
        inResult = inMyFile_AdviceReadByIndex(inUpDateAdviceIndex,&strAdvice,&srAdvTransTable);
        
        vdDebug_LogPrintf("ADVICE inUpDateAdviceIndex[%d] inMyFile_AdviceRead Rult(%d)(%d)(%d)(%d)", inUpDateAdviceIndex, inResult, srAdvTransTable.ulTraceNum, srAdvTransTable.byPackType, strAdvice.byTransType);
        
        if(inResult == ST_ERROR || inResult == RC_FILE_READ_OUT_NO_DATA)
        {
            inResult = ST_SUCCESS;
            break;
        }
        
        vdDebug_LogPrintf("ulnInvoiceNo[%02X %02X %02X] strAdvice->szInvoiceNo [%02X %02X %02X]", szInvoiceNo[0], szInvoiceNo[1], szInvoiceNo[2], strAdvice.szInvoiceNo[0], strAdvice.szInvoiceNo[1], strAdvice.szInvoiceNo[2]);
        if(0 != memcmp(szInvoiceNo, strAdvice.szInvoiceNo, INVOICE_BCD_SIZE))
        {
            inUpDateAdviceIndex ++;
            continue;
        }
        else
        {
            srAdvTransTable.byUploaded = CN_TRUE;
            inResult = inMyFile_AdviceUpdate(inUpDateAdviceIndex);
            break;
        }
        
    }

    return ST_SUCCESS;
}

int inCTOSS_BatchCheckDuplicateInvoice(void)
{
    int inRet = d_NO;
    
    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPBatchSearch(d_IPC_CMD_CHECK_DUP_INV);

        vdSetErrorMessage("");
        if(d_OK != inRet)
            return inRet;
    }
    else
    {
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetVoid();
            if(d_OK != inRet)
                return inRet;
        }       
        inRet = inCTOS_BatchSearch();
        
        vdSetErrorMessage("");
        if(d_OK != inRet)
            return inRet;
    }

    return inRet;
}

BYTE InputAmount2(USHORT usX, USHORT usY, BYTE *szCurSymbol, BYTE exponent, BYTE first_key, BYTE *baAmount, ULONG *ulAmount, USHORT usTimeOutMS, BYTE bIgnoreEnter)
{
    char szTemp[24+1];
    USHORT inRet;
	int inCtr=0;
	char szAmount[24+1];
	char chAmount=0x00;
	char szDisplay[24+1];
    unsigned char c;
    BOOL isKey;;
	
	memset(szAmount, 0x00, sizeof(szAmount));
	
	while(1)
	{
		memset(szTemp, 0x00, sizeof(szTemp));         	 	
		if(strlen(szAmount) > 0)
		{
		    vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szAmount, szTemp);
		}
		else
		{
		    vdCTOS_FormatAmount("NNN,NNN,NNn.nn", "0", szTemp);
		}

 
        clearLine(usY);
        CTOS_LCDTPrintXY(usX, usY, szCurSymbol);
        int x=0;
        int len, index;
        len=strlen(szTemp);
        for(index=0; index < len; index++)
        {
            if(szTemp[index] == '.')
                x+=1;
            else
                x+=2;
        }
        
        CTOS_LCDTPrintXY(36-x, usY, szTemp);
		
		CTOS_TimeOutSet(TIMER_ID_3,usTimeOutMS);
		while(1)//loop for time out
		{
			CTOS_KBDInKey(&isKey);
			if (isKey){ //If isKey is TRUE, represent key be pressed //
				vduiLightOn();
				//Get a key from keyboard //
				CTOS_KBDGet(&c);
				inRet=c;
				break;
			}
			else if (CTOS_TimeOutCheck(TIMER_ID_3) == d_YES)
			{	   
				return 0xFF;
			}
		}

		if(inRet >= 48 && inRet <= 57)
		{
			if(inCtr < 9)
			{
			    memset(szTemp, 0x00, sizeof(szTemp));
			    sprintf(szTemp, "%c", inRet);
                strcat(szAmount, szTemp);			
                inCtr++; 		
				if(inCtr == 1 && szAmount[0] == 48)
				{
					memset(szAmount, 0x00, sizeof(szAmount));
					inCtr=0;
				}
			}
		}
		else if(inRet == 67) /*cancel key*/
		{
			return d_USER_CANCEL;
		}
		else if(inRet == 65) /*entery key*/
		{
			if(strlen(szAmount) > 0)
			{
				memcpy(baAmount, szAmount, strlen(szAmount));
				return d_OK;
			}
			else
			{
				if(bIgnoreEnter == 1)
					return d_OK;			
			}
		}
		else if(inRet == 82) /*clear key*/
		{		
			if(inCtr > 0)
                inCtr--;
            szAmount[inCtr]=0x00;
		}
	}
}

int inChkAccumEmpty(void)
{
    ACCUM_REC srAccumRec;
    int inResult=ST_SUCCESS;
    
    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    if((inResult = inCTOS_ReadAccumTotal(&srAccumRec)) != ST_SUCCESS)
    {
        vdDebug_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
        //CTOS_LCDTClearDisplay();
        vdSetErrorMessage("BATCH EMPTY");
        return ST_ERROR;    
    }

	
	vdDebug_LogPrintf("inChkAccumEmpty return");
    return ST_SUCCESS;	
}

BYTE InputStringAlpha2(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS)
{
    char szTemp[24+1];
    USHORT inRet;
	int inCtr=0;
	char szAmount[24+1];
	char chAmount=0x00;
	char szDisplay[24+1];
    unsigned char c;
    BOOL isKey;
	int x=0;
	int inLastKey=255;
	int inKey=0;
	int inKey2=0;
    int inMax=*usStrLen;
	
	char KeyPad[10][4]={{'0', '0', '0', '0'},
		               {'1', 'Q', 'Z', '1'},
		               {'2', 'A', 'B', 'C'},
					   {'3', 'D', 'E', 'F'},
					   {'4', 'G', 'H', 'I'},
					   {'5', 'J', 'K', 'L'},
					   {'6', 'M', 'N', '0'},
					   {'7', 'P', 'R', 'S'},
					   {'8', 'T', 'U', 'V'},
					   {'9', 'W', 'X', 'Y'}};
	
	memset(szAmount, 0x00, sizeof(szAmount));
	
	CTOS_TimeOutSet(TIMER_ID_1, 100);
	
	while(1)
	{		
		memset(szTemp, 0x00, sizeof(szTemp));  
         
        clearLine(usY);
        CTOS_LCDTPrintXY(40-(strlen(szAmount)*2), usY, szAmount);
		
		CTOS_TimeOutSet(TIMER_ID_3,usTimeOutMS);
		while(1)//loop for time out
		{
            if (CTOS_TimeOutCheck(TIMER_ID_1) == d_YES)
                inLastKey=255;
			
			CTOS_KBDInKey(&isKey);
			if (isKey){ //If isKey is TRUE, represent key be pressed //
				vduiLightOn();
				//Get a key from keyboard //
				CTOS_KBDGet(&c);
				inRet=c;
				break;
			}
			else if (CTOS_TimeOutCheck(TIMER_ID_3) == d_YES)
			{	   
				return 0xFF;
			}
		}

		if(inRet >= 48 && inRet <= 57)
		{
			char szTemp2[24+1];
			
			memset(szTemp, 0x00, sizeof(szTemp));
			sprintf(szTemp, "%c", inRet);
			inKey=atoi(szTemp);
			
			CTOS_TimeOutSet(TIMER_ID_1, 100);
			if((inCtr < inMax) || (inLastKey == inKey))
			{
                vdDebug_LogPrintf("1. inLastKey:(%d), inKey:(%d), inKey2:(%d),ctr:(%d)", inLastKey, inKey, inKey2, inCtr);
				
                if(inLastKey == inKey)
                {
					inKey2++;
					if(inKey2 > 3)
                       inKey2=0;
                }
                else
					inKey2=0;
				                
                if(inLastKey == inKey)
                    inCtr--;
                    
				szAmount[inCtr]=KeyPad[inKey][inKey2];
				inCtr++;

                vdDebug_LogPrintf("2. inLastKey:(%d), inKey:(%d), inKey2:(%d),ctr:(%d)", inLastKey, inKey, inKey2, inCtr);
				
                inLastKey=inKey;

                if(inKey == 0)
				    inLastKey=255;	
			}
		}
		else if(inRet == 67) /*cancel key*/
		{
			return d_KBD_CANCEL;
		}
		else if(inRet == 65) /*entery key*/
		{
			if(strlen(szAmount) > 0)
			{
				memcpy(pbaStr, szAmount, strlen(szAmount));
				*usStrLen=strlen(szAmount);
				return d_KBD_ENTER;
			}
			
		}
		else if(inRet == 82) /*clear key*/
		{		
			inCtr--;
			if(inCtr <= 0)
                inCtr=0;
            szAmount[inCtr]=0x00;
			inKey2=0;
			inLastKey=255;
		}
	}
}

BYTE InputString2(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS)
{
    char szTemp[24+1];
    USHORT inRet;
	int inCtr=0;
	char szAmount[24+1];
	char chAmount=0x00;
	char szDisplay[24+1];
    unsigned char c;
    BOOL isKey;
	int x=0;
	int inKey=0;
    int inMax=*usStrLen;
	
	memset(szAmount, 0x00, sizeof(szAmount));
	
	while(1)
	{		
		memset(szTemp, 0x00, sizeof(szTemp));  
         
        clearLine(usY);
        CTOS_LCDTPrintXY(40-(strlen(szAmount)*2), usY, szAmount);
		
		CTOS_TimeOutSet(TIMER_ID_3,usTimeOutMS);
		while(1)//loop for time out
		{		
			CTOS_KBDInKey(&isKey);
			if (isKey){ //If isKey is TRUE, represent key be pressed //
				vduiLightOn();
				//Get a key from keyboard //
				CTOS_KBDGet(&c);
				inRet=c;
				break;
			}
			else if (CTOS_TimeOutCheck(TIMER_ID_3) == d_YES)
			{	   
				return 0xFF;
			}
		}

		if(inRet >= 48 && inRet <= 57)
		{
			if(inCtr < inMax)
			{
			    memset(szTemp, 0x00, sizeof(szTemp));
			    sprintf(szTemp, "%c", inRet);
                strcat(szAmount, szTemp);			
                inCtr++; 	
			}
		}
		else if(inRet == 67) /*cancel key*/
		{
			return d_KBD_CANCEL;
		}
		else if(inRet == 65) /*entery key*/
		{
			if(strlen(szAmount) > 0)
			{
				memcpy(pbaStr, szAmount, strlen(szAmount));
				*usStrLen=strlen(szAmount);
				return d_KBD_ENTER;
			}
		}
		else if(inRet == 82) /*clear key*/
		{		
			inCtr--;
			if(inCtr <= 0)
                inCtr=0;
            szAmount[inCtr]=0x00;
		}
	}
}

#if 0
int inCTOS_ValidateTrxnAmount(void)
{
	double dbAmt1 = 0.00, dbAmt2 = 0.00;
	BYTE szBaseAmt[30+1] = {0};
	char *strPTR;

	//inTCTRead(1);
	memset(szBaseAmt, 0x00, sizeof(szBaseAmt));
	wub_hex_2_str(srTransRec.szBaseAmount, szBaseAmt, AMT_BCD_SIZE);
	dbAmt1 = strtod(szBaseAmt, &strPTR);
	dbAmt2 = strtod(strTCT.szMaxTrxnAmt, &strPTR);

	vdDebug_LogPrintf("JEFF::STR TXN AMT: [%s]", szBaseAmt);
	vdDebug_LogPrintf("JEFF::STR MAX AMT: [%s]", strTCT.szMaxTrxnAmt);
	vdDebug_LogPrintf("JEFF::TXN AMT: [%.0f]", dbAmt1);
	vdDebug_LogPrintf("JEFF::MAX AMT: [%.0f]", dbAmt2);

	if((dbAmt1 >= dbAmt2) || (dbAmt1 <= 0))
	{
	    
		vdDebug_LogPrintf("OUT OF RANGE");
		vdSetErrorMessage("OUT OF RANGE");
		return(d_NO);
	}

	return(d_OK);
}
#endif

