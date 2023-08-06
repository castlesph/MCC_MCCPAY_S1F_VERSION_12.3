#include <string.h>
#include <stdio.h>
#include <ctosapi.h>
#include <stdlib.h>
#include <stdarg.h>


#include "../Includes/msg.h"
#include "../Includes/wub_lib.h"
#include "../Includes/myEZLib.h"
#include "../Includes/POSTypedef.h"
#include "../Includes/POSTrans.h"

#include "display.h"
#include "../FileModule/myFileFunc.h"
#include "../print/Print.h"
#include "../Includes/CTOSinput.h"
#include "../UI/Display.h"
#include "../Comm/V5Comm.h"
#include "..\Debug\Debug.h"
#include "../UIapi.h"

extern USHORT GPRSCONNETSTATUS;

#define ERRORLEN 30
#define ERRORLEN1 30

static char szErrorMessage[ERRORLEN+1];
static char szErrorMessage1[ERRORLEN1+1];
static char szErrorMessage2[ERRORLEN1+1];


void setLCDReverse(int line,int position, char *pbBuf)
{
    int iInitX = 0;
    int lens = 0;

    //Set the reverse attribute of the character //
    CTOS_LCDTSetReverse(TRUE);  //the reverse enable // 
    
    switch(position)
    {
        case DISPLAY_POSITION_LEFT:
            CTOS_LCDTPrintXY(1, line, pbBuf);
            break;
        case DISPLAY_POSITION_CENTER:
            lens = strlen(pbBuf);
            iInitX = (16 - lens) / 2 + 1;
            CTOS_LCDTPrintXY(iInitX, line, pbBuf);
            break;
        case DISPLAY_POSITION_RIGHT:
            lens = strlen(pbBuf);
            iInitX = 16 - lens + 1;
            CTOS_LCDTPrintXY(iInitX, line, pbBuf);
            break;
    }

    //Set the reverse attribute of the character //
    CTOS_LCDTSetReverse(FALSE); //the reverse enable //     
}

void setLCDPrint(int line,int position, char *pbBuf)
{
    int iInitX = 0;
    int lens = 0;
    
    CTOS_LCDFontSelectMode(d_FONT_FNT_MODE);
    switch(position)
    {
        case DISPLAY_POSITION_LEFT:
            CTOS_LCDTPrintXY(1, line, pbBuf);
        break;

        case DISPLAY_POSITION_CENTER:
            lens = strlen(pbBuf);
            iInitX = ((30 - lens) / 2) + 1;
            CTOS_LCDTPrintXY(iInitX, line, pbBuf);
        break;

        case DISPLAY_POSITION_RIGHT:
            lens = strlen(pbBuf);
            iInitX = 20 - lens + 1;
            CTOS_LCDTPrintXY(iInitX, line, pbBuf);
        break;
    }
    CTOS_LCDFontSelectMode(d_FONT_TTF_MODE);
}

void showAmount(IN  BYTE bY, BYTE bStrLen, BYTE *baBuf)
{
    int i;
    
    if(bStrLen > 2)
    {
        CTOS_LCDTPrintXY(13, bY, "0.00");
        for(i = 0;i < bStrLen; i++)
        {
            if ((16 - bStrLen + 1 + i) > 14)
                CTOS_LCDTPutchXY(16 - bStrLen + 1 + i, bY, baBuf[i]);
            else
                CTOS_LCDTPutchXY(16 - bStrLen + i, bY, baBuf[i]);
        }
    }
    else
    {
        CTOS_LCDTPrintXY(13, bY, "0.00");
        for(i = 0;i < bStrLen; i++)
        {
            CTOS_LCDTPutchXY(16 - bStrLen + 1 + i, bY, baBuf[i]);
        }
    }
}

void vduiDisplayInvalidTLE(void)
{
    
    vduiClearBelow(2);
    vduiWarningSound();
    vduiDisplayStringCenter(3,"INVALID SESSION");
    vduiDisplayStringCenter(4,"KEY, PLEASE");
    vduiDisplayStringCenter(5,"DWD SESSION KEY");
    vduiDisplayStringCenter(6,"--INSTRUCTION---");
    CTOS_LCDTPrintXY(1,7,"PRESS [F2] THEN");
    CTOS_LCDTPrintXY(1,8,"PRESS [3]");
    
    CTOS_Delay(2500);
}


void szGetTransTitle(BYTE byTransType, BYTE *szTitle)
{    
    int i;
    szTitle[0] = 0x00;

	//vdDebug_LogPrintf("**szGetTransTitle START byTransType[%d]Orig[%d]**", byTransType, srTransRec.byOrgTransType);
    //vdDebug_LogPrintf("--szGetTransTitle--");
	//vdDebug_LogPrintf("srTransRec.IITid=[%d]", srTransRec.IITid);
	//vdDebug_LogPrintf("strIIT.szIssuerLabel=[%s]", strIIT.szIssuerLabel);
	//vdDebug_LogPrintf("strIIT.szIssuerAbbrev=[%s]", strIIT.szIssuerAbbrev);
	
    switch(byTransType)
    {
        case SALE:
			if(srTransRec.fInstallment == TRUE)
				strcpy(szTitle, "INSTALLMENT");
			else	
                strcpy(szTitle, "SALE");

			
			//if (srTransRec.HDTid == PHQR_HOST_INDEX)			
				sprintf(szTitle, "%s %s", "SALE", strIIT.szIssuerLabel);
			//else
			//	sprintf(szTitle, "%s<%s>", "SALE", strIIT.szIssuerAbbrev);
            break;
        case PRE_AUTH:
            //strcpy(szTitle, "RETRIEVE");

			//sprintf(szTitle, "%s<%s>", "RETRIEVE", strIIT.szIssuerAbbrev);
			sprintf(szTitle, "%s %s ", "RETRIEVE", strIIT.szIssuerLabel);
            break;
        case PRE_COMP:
            strcpy(szTitle, "AUTH COMP");
            break;
        case REFUND:
            strcpy(szTitle, "REFUND");
            break;
        case VOID:
            if(srTransRec.byOrgTransType == REFUND)
                strcpy(szTitle, "VOID REFUND");
			else if(srTransRec.fInstallment == TRUE) 
				strcpy(szTitle, "VOID INSTALLMENT");
			else if(srTransRec.byOrgTransType == LOY_REDEEM_5050 || srTransRec.byOrgTransType == LOY_REDEEM_VARIABLE)
				strcpy(szTitle, "VOID REDEEM");
            else
                //strcpy(szTitle, "VOID");
                sprintf(szTitle, "%s %s", "VOID", strIIT.szIssuerLabel);

			//sprintf(szTitle, "%s<%s>", "VOID", strIIT.szIssuerAbbrev);
			sprintf(szTitle, "%s %s", "VOID", strIIT.szIssuerLabel);
            break;
        case SALE_TIP:
			if(srTransRec.byOrgTransType == SALE)
                strcpy(szTitle, "SALE TIP ADJ");
			else if(srTransRec.byOrgTransType == SALE_OFFLINE)
                strcpy(szTitle, "OFFLINE TIP ADJ");
			else
				strcpy(szTitle, "TIP ADJUST");
            break;
        case SALE_OFFLINE:
            strcpy(szTitle, "OFFLINE");
            break;
        case SALE_ADJUST: 
            strcpy(szTitle, "ADJUST");
            break;
        case SETTLE:
            strcpy(szTitle, "SETTLE");
            break;
        case SIGN_ON:
            strcpy(szTitle, "SIGN ON");
            break;
        case BATCH_REVIEW:
            strcpy(szTitle, "BATCH REVIEW");
            break;
        case BATCH_TOTAL:
            strcpy(szTitle, "BATCH TOTAL");
            break;
        case REPRINT_ANY:
            strcpy(szTitle, "REPRINT RECEIPT");
            break;
        case BAL_INQ:
           strcpy(szTitle, "BALANCE INQUIRY");
           break;
        case LOG_ON:
           strcpy(szTitle, "LOGON");
           break;	
        case LOY_BAL_INQ:
            strcpy(szTitle, "INQUIRY");		
            break;
        case LOY_REDEEM_5050:
            strcpy(szTitle, "50/50 REDEEM");	   
            break;
			
        case LOY_REDEEM_VARIABLE:
            strcpy(szTitle, "VARIABLE REDEEM");	  
            break;
			
        case CARD_VERIFY:
            strcpy(szTitle, "CARD VERIFY");	  
            break;			
			
        default:
            strcpy(szTitle, "");
            break;
    }
    i = strlen(szTitle);
    szTitle[i]=0x00;
    return ;
}

void vdDispTransTitle(BYTE byTransType)
{
    BYTE szTitle[16+1];
    BYTE szTitleDisplay[MAX_CHAR_PER_LINE+1];
    int iInitX = 1;

 #if 0
    inSetTextMode();
	
    memset(szTitle, 0x00, sizeof(szTitle));
    szGetTransTitle(byTransType, szTitle);
    iInitX = (MAX_CHAR_PER_LINE - strlen(szTitle)*2) / 2 ;
    memset(szTitleDisplay, 0x00, sizeof(szTitleDisplay));
    memset(szTitleDisplay, 0x20, MAX_CHAR_PER_LINE);
    memcpy(&szTitleDisplay[iInitX], szTitle, strlen(szTitle));
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, szTitleDisplay);
    CTOS_LCDTSetReverse(FALSE);
	//vdClearNonTitleLines();
	vdClearBelowLine(4); // Do not clear logo
#endif
}

void vdDispTitleString(BYTE *szTitle)
{
    BYTE szTitleDisplay[MAX_CHAR_PER_LINE+1];
    int iInitX = 1;
       
    iInitX = (MAX_CHAR_PER_LINE - strlen(szTitle)*2) / 2;
    memset(szTitleDisplay, 0x00, sizeof(szTitleDisplay));
    memset(szTitleDisplay, 0x20, MAX_CHAR_PER_LINE);
    memcpy(&szTitleDisplay[iInitX], szTitle, strlen(szTitle));
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, szTitleDisplay);
    CTOS_LCDTSetReverse(FALSE);
}

USHORT clearLine(int line)
{
    CTOS_LCDTGotoXY(1,line);
    CTOS_LCDTClear2EOL();
}

int vdDisplayTxnFinishUI(void)
{
	unsigned char key;
	char szTemp1[24+1], szTemp2[24+1], szTemp3[24+1], szTotalAmt[24+1];

	if (srTransRec.byTransType == BAL_INQ) 
	return d_OK;
	
	if(isCheckTerminalMP200() == d_OK)
	{
		vdClearBelowLine(2);
		vdDispTransTitle(srTransRec.byTransType);

		if (srTransRec.byTransType == SALE_OFFLINE || srTransRec.byTransType == SALE_TIP)
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TRANSACTION DETAILS");
		else
			setLCDPrint(3, DISPLAY_POSITION_LEFT, "TRANSACTION APPROVED");
		
		memset(szTemp1, 0, sizeof(szTemp1));
		wub_hex_2_str(srTransRec.szInvoiceNo, szTemp1, INVOICE_BCD_SIZE);
		memset(szTemp2, 0, sizeof(szTemp2));
		sprintf(szTemp2, "TRACE NO.: %s", szTemp1);
		setLCDPrint(4, DISPLAY_POSITION_LEFT, szTemp2);   

		memset(szTemp1, 0, sizeof(szTemp1));
		sprintf(szTemp1, "APPR. CODE: %s", srTransRec.szAuthCode);
		setLCDPrint(5, DISPLAY_POSITION_LEFT, szTemp1);   

		if (srTransRec.byTransType == SALE_TIP)
		{
			// Tip Amount
			memset(szTemp1, 0, sizeof(szTemp1));
			memset(szTemp2, 0, sizeof(szTemp2));
			memset(szTotalAmt, 0, sizeof(szTotalAmt));
			wub_hex_2_str(srTransRec.szTipAmount, szTotalAmt, AMT_BCD_SIZE);
			vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szTotalAmt, szTemp1);
			sprintf(szTemp2, "TIP AMOUNT: %s%s", strCST.szCurSymbol, szTemp1);					
			setLCDPrint(6, DISPLAY_POSITION_LEFT, szTemp2);

			// Total Amount
			memset(szTemp1, 0, sizeof(szTemp1));
			memset(szTemp2, 0, sizeof(szTemp2));
			memset(szTotalAmt, 0, sizeof(szTotalAmt));
			wub_hex_2_str(srTransRec.szTotalAmount, szTotalAmt, AMT_BCD_SIZE);
			vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szTotalAmt, szTemp1);
			sprintf(szTemp2, "AMOUNT: %s%s", strCST.szCurSymbol, szTemp1);					
			setLCDPrint(7, DISPLAY_POSITION_LEFT, szTemp2); 
		}
		else
		{
			// Total Amount
			memset(szTemp1, 0, sizeof(szTemp1));
			memset(szTemp2, 0, sizeof(szTemp2));
			memset(szTotalAmt, 0, sizeof(szTotalAmt));
			wub_hex_2_str(srTransRec.szTotalAmount, szTotalAmt, AMT_BCD_SIZE);
			vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szTotalAmt, szTemp1);
			sprintf(szTemp2, "AMOUNT: %s%s", strCST.szCurSymbol, szTemp1);					
			setLCDPrint(6, DISPLAY_POSITION_LEFT, szTemp2); 
		}

		vduiDisplayStringCenter(8,"PLEASE PRESS ENTER");
		
		while(1)
		{
			key=WaitKey(1);
			if(key == d_KBD_ENTER)
				break;
			
			CTOS_Beep();
			CTOS_Delay(300);
			CTOS_Beep();
		}
	}
	else
	{
	#if 0
		CTOS_LCDTClearDisplay();
		setLCDPrint(3, DISPLAY_POSITION_CENTER, "Transaction");
		setLCDPrint(4, DISPLAY_POSITION_CENTER, "Approved");
		setLCDPrint(5, DISPLAY_POSITION_CENTER, srTransRec.szAuthCode);  
		
	#else //TINE:  android	
	strGBLVar.fGBLApprovedDisplay = (get_env_int("APPROVEDDISPLAY") > 0 ? TRUE : FALSE);

          if(strGBLVar.fGBLApprovedDisplay ==TRUE)
          {
               vdDisplayMessageBox(1, 8, "APPROVED", "Transaction", "", MSG_TYPE_SUCCESS);
               CTOS_Beep();
               CTOS_Delay(2000);
          }
		  
	#endif
	}	
}


void vdDispErrMsg(IN BYTE *szMsg)
{
    char szDisplayMsg[40];
    BYTE byKeyBuf;
    
    //CTOS_LCDTClearDisplay();
    if(srTransRec.byTransType != 0)
        vdDispTransTitle(srTransRec.byTransType);

    memset(szDisplayMsg, 0x00, sizeof(szDisplayMsg));
    strcpy(szDisplayMsg, szMsg);
    vduiClearBelow(8);
    setLCDPrint(8, DISPLAY_POSITION_LEFT, szDisplayMsg);
    CTOS_TimeOutSet (TIMER_ID_2 , 2*100);
    CTOS_Sound(1000, 50);
    
    while (1)
    {        
        CTOS_KBDHit  (&byKeyBuf);
        if (byKeyBuf == d_KBD_CANCEL ||byKeyBuf == d_KBD_ENTER)
        {
            CTOS_KBDBufFlush ();
            return ;
        }
    }
}


int vdDispTransactionInfo(void)
{
    BYTE byKeyBuf;
    BYTE szTmp1[16+1];
    BYTE szTmp2[16+1];
    
    //CTOS_LCDTClearDisplay();
    vdDispTransTitle(srTransRec.byTransType);
    
    setLCDPrint(2, DISPLAY_POSITION_LEFT, "Card NO.");
    setLCDPrint(3, DISPLAY_POSITION_LEFT, srTransRec.szPAN);
    memset(szTmp1, 0x00, sizeof(szTmp1));
    memset(szTmp2, 0x00, sizeof(szTmp2));
    wub_hex_2_str(srTransRec.szInvoiceNo, szTmp1, 3);
    sprintf(szTmp2,"Inv No:%s", szTmp1);
    setLCDPrint(4, DISPLAY_POSITION_LEFT, szTmp2);
    
    wub_hex_2_str(srTransRec.szTotalAmount, szTmp1, 6);
    setLCDPrint(5, DISPLAY_POSITION_LEFT, "Amount:");
    sprintf(szTmp2,"SGD%7lu.%02lu", (atol(szTmp1)/100), (atol(szTmp1)%100));
    setLCDPrint(6, DISPLAY_POSITION_RIGHT, szTmp2);  
    setLCDPrint(8, DISPLAY_POSITION_LEFT, "PRS ENTR TO CONF");
    CTOS_TimeOutSet (TIMER_ID_2 , 30*100);
    
    while (1)
    {
        if(CTOS_TimeOutCheck(TIMER_ID_2 )  == d_OK)
            return  READ_CARD_TIMEOUT;
        
        CTOS_KBDHit  (&byKeyBuf);
        if (byKeyBuf == d_KBD_CANCEL)
        {
            CTOS_KBDBufFlush ();
            return USER_ABORT;
        }
        else if (byKeyBuf == d_KBD_ENTER)
        {
            CTOS_KBDBufFlush ();
            return d_OK;
        }
    }
}

USHORT showBatchRecord(TRANS_DATA_TABLE *strTransData)
{
    char szStr[DISPLAY_LINE_SIZE + 1];
    char szTemp[DISPLAY_LINE_SIZE + 1];
    BYTE byKeyBuf;
    //CTOS_LCDTClearDisplay();
    memset(szStr, ' ', DISPLAY_LINE_SIZE);
    sprintf(szStr, "%s", strTransData->szPAN);
    setLCDPrint(1, DISPLAY_POSITION_LEFT, "Card NO:");
    setLCDPrint(2, DISPLAY_POSITION_LEFT, szStr);
    
    memset(szStr, ' ', DISPLAY_LINE_SIZE);
    memset(szTemp, ' ', DISPLAY_LINE_SIZE);
    wub_hex_2_str(strTransData->szBaseAmount, szTemp, AMT_BCD_SIZE);
    sprintf(szStr, "%lu.%lu", atol(szTemp)/100, atol(szTemp)%100);
    setLCDPrint(3, DISPLAY_POSITION_LEFT, "Amount:");
    setLCDPrint(4, DISPLAY_POSITION_LEFT, szStr);

    
    memset(szStr, ' ', DISPLAY_LINE_SIZE);
    sprintf(szStr, "%s", strTransData->szAuthCode);
    setLCDPrint(5, DISPLAY_POSITION_LEFT, "Auth Code:");
    setLCDPrint(6, DISPLAY_POSITION_LEFT,  szStr);


    memset(szStr, ' ', DISPLAY_LINE_SIZE);
    memset(szTemp, ' ', DISPLAY_LINE_SIZE);
    wub_hex_2_str(strTransData->szInvoiceNo, szTemp, INVOICE_BCD_SIZE);
    sprintf(szStr, "%s", szTemp);
    setLCDPrint(7, DISPLAY_POSITION_LEFT, "TRACE NO.:");
    setLCDPrint(8, DISPLAY_POSITION_LEFT, szTemp);
     
    CTOS_TimeOutSet (TIMER_ID_2 , 30*100);   
    while (1)
    {
        if(CTOS_TimeOutCheck(TIMER_ID_2 )  == d_OK)
        {
            //CTOS_LCDTClearDisplay();
            return  READ_CARD_TIMEOUT;
        }
        CTOS_KBDHit  (&byKeyBuf);
        if (byKeyBuf == d_KBD_CANCEL)
        {
            CTOS_KBDBufFlush ();
            //CTOS_LCDTClearDisplay();
            return USER_ABORT;
        }
        else if (byKeyBuf == d_KBD_ENTER)
        {
            CTOS_KBDBufFlush ();
            //CTOS_LCDTClearDisplay();
            return d_OK;
        }
    }
}

void vduiLightOn(void)
{
    if (strTCT.fHandsetPresent)  
        CTOS_BackLightSetEx(d_BKLIT_LCD,d_ON,80000);
    else
        CTOS_BackLightSet (d_BKLIT_LCD, d_ON);
}

void vduiKeyboardBackLight(BOOL fKeyBoardLight)
{
    if (strTCT.fHandsetPresent) 
    {
        if(VS_TRUE == fKeyBoardLight)
        {
            
            CTOS_BackLightSetEx(d_BKLIT_KBD,d_ON,0xffffff);
            CTOS_BackLightSetEx(d_BKLIT_LCD,d_ON,0xffffff);
        }
        else
        {
            CTOS_BackLightSetEx(d_BKLIT_KBD,d_OFF,100);
            CTOS_BackLightSetEx(d_BKLIT_LCD,d_OFF,3000);
        }

    }
    else
    {
        if(VS_TRUE == fKeyBoardLight)
            CTOS_BackLightSetEx(d_BKLIT_KBD,d_ON,0xffffff);
        else
            CTOS_BackLightSetEx(d_BKLIT_KBD,d_OFF,100);
    }
}

void vduiPowerOff(void)
{
    BYTE block[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    USHORT ya,yb,xa,xb;
    unsigned char c;
        
    vduiClearBelow(1);
    vduiDisplayStringCenter(3,"ARE YOU SURE");
    vduiDisplayStringCenter(4,"WANT TO POWER");
    vduiDisplayStringCenter(5,"OFF TERMINAL");
    CTOS_LCDTPrintXY(1,7,"NO[X]   YES[OK] ");
    c=WaitKey(60);
    
    if(c!=d_KBD_ENTER)
    {
            return;
    }    
    
    for(ya =1; ya<5; ya++)
    {
        CTOS_Delay(100);
        CTOS_LCDTGotoXY(1,ya);
        CTOS_LCDTClear2EOL();
    }
    for(yb=8; yb>4; yb--)
    {
        CTOS_Delay(100);
        CTOS_LCDTGotoXY(1,yb);
        CTOS_LCDTClear2EOL();
    }
    CTOS_LCDTPrintXY(1,4,"----------------");
    for(xa=1; xa<8; xa++)
    {
        CTOS_Delay(25);
        CTOS_LCDTPrintXY(xa,4," ");
    }
    for(xb=16; xb>7; xb--)
    {
        CTOS_Delay(25);
        CTOS_LCDTPrintXY(xb,4," ");
    }
            
    CTOS_LCDGShowPic(58, 6, block, 0, 6);
    CTOS_Delay(250);
    CTOS_LCDTGotoXY(7,4);
    CTOS_LCDTClear2EOL();
    CTOS_Delay(250);

    CTOS_PowerOff();
}

void vduiDisplayStringCenter(unsigned char  y,unsigned char *sBuf)
{	
    //CTOS_LCDTPrintXY(1,y,"                ");
	if (isCheckTerminalMP200() == d_OK)
		setLCDPrint(y, DISPLAY_POSITION_CENTER, sBuf);    
	else		
	    CTOS_LCDTPrintXY((30-strlen(sBuf))/2+1,y,sBuf);
}

void vduiClearBelow(int line)
{
	int i = 0,
	inNumOfLine = 8;

	inTCTRead(1);
	if((strTCT.byTerminalType % 2) == 0)
		inNumOfLine = 16;

	for(i=line; i<=inNumOfLine; i++)
		clearLine(i);
}


void vduiWarningSound(void)
{
    CTOS_LEDSet(d_LED1, d_ON);
    CTOS_LEDSet(d_LED2, d_ON);
    CTOS_LEDSet(d_LED3, d_ON);
    
    CTOS_Beep();
    CTOS_Delay(300);
    CTOS_Beep();
    
    CTOS_LEDSet(d_LED1, d_OFF);
    CTOS_LEDSet(d_LED2, d_OFF);
    CTOS_LEDSet(d_LED3, d_OFF);
}


void vdDisplayErrorMsg(int inColumn, int inRow,  char *msg)
{
	int inRowtmp;
    BYTE szTitle[20+1];
    BYTE szDisplay[200];
	int iMsgType;
	

#if 0
    if (2 == strTCT.byTerminalType)
		inRowtmp = V3_ERROR_LINE_ROW;
	else
        inRowtmp = inRow;
			
    CTOS_LCDTPrintXY(inColumn, inRowtmp, "                                        ");
    CTOS_LCDTPrintXY(inColumn, inRowtmp, msg);
    CTOS_Beep();
    CTOS_Delay(1500);
//#else
	memset(szTitle, 0x00, sizeof(szTitle));
	szGetTransTitle(srTransRec.byTransType, szTitle);

	strcpy(szDisMsg, szTitle);
	strcat(szDisMsg, "|");
	strcat(szDisMsg, msg);
	usCTOSS_LCDDisplay(szDisMsg);
	CTOS_Beep();
	CTOS_Delay(1500);
	CTOS_Beep();
	
#else

	iMsgType = MSG_TYPE_ERROR;
	if (strcmp(msg, "USER CANCEL") == 0)
		iMsgType = MSG_TYPE_INFO;
	if ((strcmp(msg, "TIMEOUT") == 0) || (strcmp(msg, "TIME OUT") == 0))
		iMsgType = MSG_TYPE_TIMEOUT;
	
	vdDisplayMessageBox(1, 8, "", msg, "", iMsgType);
	CTOS_Beep();
	CTOS_Delay(1500);
	CTOS_Beep();
	


#endif
}

void vdDisplayErrorMsg2(int inColumn, int inRow,  char *msg, char *msg2, int msgType)
{
	int inRowtmp;
	BYTE szDisMsg[200];
    BYTE szDisMsg1[200];
    BYTE szDisplay[200];

#if 0	
    if ((strTCT.byTerminalType % 2) == 0)
		inRowtmp = V3_ERROR_LINE_ROW;
	else
        inRowtmp = inRow;
			
    CTOS_LCDTPrintXY(inColumn, inRowtmp-1, "                                        ");
    CTOS_LCDTPrintXY(inColumn, inRowtmp-1, msg);
    CTOS_LCDTPrintXY(inColumn, inRowtmp, "                                        ");
    CTOS_LCDTPrintXY(inColumn, inRowtmp, msg2);	
    CTOS_Beep();
    CTOS_Delay(1500);

#else
	//memset(szDisMsg, 0x00, sizeof(szDisMsg));
	//strcpy(szDisMsg, msg);
	//strcat(szDisMsg, "|");
    //strcat(szDisMsg, msg2);
	//usCTOSS_DisplayErrorMsg2(szDisMsg);
	vdDisplayMessageBox(1, 8, msg, msg2, "", msgType);
    CTOS_Beep();
    CTOS_Delay(1500);
    CTOS_Beep();

#endif
}


/* functions for loyalty - Meena 15/01/2012 - start*/
short vduiAskConfirmContinue(void)
{
    unsigned char key;
  
    vduiClearBelow(2);
    vduiDisplayStringCenter(3,"ARE YOU SURE");
    vduiDisplayStringCenter(4,"YOU WANT TO");
    vduiDisplayStringCenter(5,"CONTINUE?");
    CTOS_LCDTPrintXY(1,7," NO[X]  YES[OK] ");
        
    while(1)
    {
        key = struiGetchWithTimeOut();
        if (key==d_KBD_ENTER)
            return d_OK;
        else if (key==d_KBD_CANCEL)
            return -1;
        else
            vduiWarningSound();
    }
    
}

short vduiAskConfirmation(char *szHeaderString)
{
    char szChoiceMsg[30 + 1];
    int bHeaderAttr = 0x01+0x04, key=0;
    char szHeaderString2[30 + 1];	

    //CTOS_LCDTClearDisplay();
	memset(szChoiceMsg, 0x00, sizeof(szChoiceMsg));
	memset(szHeaderString2, 0x00, sizeof(szHeaderString2));

  //#00136 - No header in Delete Batch & Clear Reversal
    if(strcmp(szHeaderString, "DELETE BATCH?") == 0)	
    		strcpy(szHeaderString2, "DELETE BATCH?");	
    else if(strcmp(szHeaderString, "CLEAR REVERSAL?") == 0)	
    		strcpy(szHeaderString2, "CLEAR REVERSAL?");	
    else if(strcmp(szHeaderString, "RESTART?") == 0)	
    		strcpy(szHeaderString2, "RESTART?");	
		
		
    strcat(szChoiceMsg,"YES \n");
    strcat(szChoiceMsg,"NO");
    key = MenuDisplay(szHeaderString2,strlen(szHeaderString2), bHeaderAttr, 1, 1, szChoiceMsg, TRUE);
    //key = MenuDisplay(szHeaderString,strlen(szHeaderString), bHeaderAttr, 1, 1, szChoiceMsg, TRUE);	
    //key = MenuDisplay(0,0, bHeaderAttr, 1, 1, szChoiceMsg, TRUE);
    
	if (key > 0)
	{
		if (key == 1) 
		{
			return d_OK;
		}
				
		if (key == 2 || key == d_KBD_CANCEL)
		{
			return -1;
		}	
	}
	
    return -1;
}

BYTE struiGetchWithTimeOut(void)
{
    unsigned char c;
    BOOL isKey;
    CTOS_TimeOutSet(TIMER_ID_3,3000);
    
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

/* functions for loyalty - Meena 15/01/2012 - End*/

short inuiAskSettlement(void)
{
    unsigned char key;
    while(1) 
    {
        vduiClearBelow(2);
        vduiDisplayStringCenter(2,"DAILY SETTLEMENT");
        vduiDisplayStringCenter(3,"NOTIFICATION");

        vduiDisplayStringCenter(5,"PERFORM");
        vduiDisplayStringCenter(6,"SETTLEMENT?");
        vduiDisplayStringCenter(8,"NO[X] YES[OK]");

        CTOS_KBDGet(&key);
        if(key==d_KBD_ENTER)
            return d_OK;
        else if(key==d_KBD_CANCEL)
            return d_KBD_CANCEL;
        else if(key==d_KBD_F1)
            vduiPowerOff();
    }
        
}

void vduiDisplaySignalStrengthBatteryCapacity(void)
{
    
    BYTE bCapacity, msg2[50];
    USHORT dwRet;
    short insign;
    
    
    if(GPRSCONNETSTATUS== d_OK && strTCT.inMainLine == GPRS_MODE)
    {
        insign=incommSignal();
        if(insign==-1)
        {
            CTOS_LCDTPrintXY (9,1, "SIGNAL:NA");
        }
        else
        {           
            if(insign/6 == 0)
                CTOS_LCDTPrintXY (9,1, "NO SIGNAL");
            else if(insign/6 == 1)
            {                               
               CTOS_LCDTPrintXY (9,1, "S:l____"); 
            }
            else if(insign/6 == 2)
            {                               
               CTOS_LCDTPrintXY (9,1, "S:ll___"); 
            }
            else if(insign/6 == 3)
            {                               
               CTOS_LCDTPrintXY (9,1, "S:lll__"); 
            }
            else if(insign/6 == 4)
            {                               
               CTOS_LCDTPrintXY (9,1, "S:llll_"); 
            }
            else if(insign/6 == 5)
            {                               
               CTOS_LCDTPrintXY (9,1, "S:lllll"); 
            }
            
        }
    }
    
    dwRet= CTOS_BatteryGetCapacityByIC(&bCapacity);  
    if(dwRet==d_OK)
    {
        sprintf(msg2, "B:%d%% ", bCapacity);
        CTOS_LCDTPrintXY (3,1, msg2);
    }
                
}

void vdSetErrorMessage(char *szMessage)
{
    int inErrLen=0;

    inErrLen = strlen(szMessage);
    memset(szErrorMessage,0x00,sizeof(szErrorMessage));
    
    if (inErrLen > 0)
    {
        if (inErrLen > ERRORLEN)
            inErrLen = ERRORLEN;
        
        memcpy(szErrorMessage,szMessage,inErrLen);
    }
}

int inGetErrorMessage(char *szMessage)
{
    int inErrLen=0;

    inErrLen = strlen(szErrorMessage);

    if (inErrLen > 0)
    {       
        memcpy(szMessage,szErrorMessage,inErrLen);
    }
    
    return inErrLen;
}

void vdClearNonTitleLines(void)
{
	setLCDPrint(2, DISPLAY_POSITION_LEFT, "                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    ");
}

//void vdClearBelowLine(int inLine)
//{
	//setLCDPrint(inLine, DISPLAY_POSITION_LEFT, "                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    ");
	//setLCDPrint(inLine, DISPLAY_POSITION_LEFT, "                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    \n                    ");

//}

void vdClearBelowLine(int inLine)
{
    int i=0, inIndex=0;
    int inNoLine=8;
	char szTemp[360+1];

    memset(szTemp, 0, sizeof(szTemp));
	
	if ((strTCT.byTerminalType % 2) == 0)
        inNoLine=16;


	inNoLine-=inLine;
	inNoLine--;

	memset(szTemp, 0, sizeof(szTemp));
    for(i=0; i<=inNoLine; i++)
    {
        memcpy(&szTemp[inIndex], "                    \n", 21);
	    inIndex+=21;
    }

    memcpy(&szTemp[inIndex], "                     ", 21);
	
    //vdDebug_LogPrintf("vdClearBelowLine");
	//vdDebug_LogPrintf("szTemp:[%s]", szTemp);
	//vdDebug_LogPrintf("inNoLine:[%d]", inNoLine);
	//vdDebug_LogPrintf("inLine:[%d]", inLine);

	setLCDPrint(inLine, DISPLAY_POSITION_LEFT, szTemp);
	
}

void setLCDPrint27(int line,int position, char *pbBuf)
{
	int iInitX = 0;
	int lens = 0;

	CTOS_LCDFontSelectMode(d_FONT_FNT_MODE);

	if (isCheckTerminalMP200() == d_OK || isCheckTerminalNonTouch() == d_OK)
		CTOS_LCDTSelectFontSize(d_FONT_16x30);
	
	switch(position)
	{
	case DISPLAY_POSITION_LEFT:
		CTOS_LCDTPrintXY(1, line, pbBuf);
		break;
	case DISPLAY_POSITION_CENTER:
		lens = strlen(pbBuf);
		iInitX = (20 - lens) / 2 + 1;
		CTOS_LCDTPrintXY(iInitX, line, pbBuf);
		break;
	case DISPLAY_POSITION_RIGHT:
		lens = strlen(pbBuf);
		iInitX = 20 - lens + 1;
		CTOS_LCDTPrintXY(iInitX, line, pbBuf);
		break;
	}

	CTOS_LCDFontSelectMode(d_FONT_TTF_MODE);
}

void vdDisplayMultiLineMsgAlign(char *msg, char *msg2, char *msg3, int inPosition)
{
   int inAlign = 0;
   
   //CTOS_LCDTClearDisplay();
   vdDispTransTitle(srTransRec.byTransType);

   switch (inPosition)
   {
   	 case DISPLAY_POSITION_LEFT:
	 	inAlign = d_LCD_ALIGNLEFT;
	 	break;
	 case DISPLAY_POSITION_RIGHT:
	 	inAlign = d_LCD_ALIGNRIGHT;
	 	break;
	 case DISPLAY_POSITION_CENTER:
	 	inAlign = d_LCD_ALIGNCENTER;
	 	break;
	  default:
	  	inAlign = d_LCD_ALIGNCENTER;
	  	break;
   }
   
	if ((strTCT.byTerminalType % 2) == 0)
   {	
   		if (strlen(msg) > 0)
			CTOS_LCDTPrintAligned(6, msg, inAlign);

		if (strlen(msg2) > 0)
			CTOS_LCDTPrintAligned(7, msg2, inAlign);

		if (strlen(msg3) > 0)
			CTOS_LCDTPrintAligned(8, msg3, inAlign);
   }
   else 
   {
   		if (strlen(msg) > 0)
			setLCDPrint27(4, inAlign, msg);

		if (strlen(msg2) > 0)
			setLCDPrint27(5, inAlign, msg2);

		if (strlen(msg3) > 0)
			setLCDPrint27(6, inAlign, msg3);
   }
	 
   CTOS_Beep();
   CTOS_Delay(1000);
   vduiClearBelow(2);
   
}

// Left to Right display -- sidumili
void inDisplayLeftRight(int inLine, unsigned char *strLeft, unsigned char *strRight, int inMode)
{
	char szStr[48+1]; 
    int inLength=0, inSize=0;
	inLength=inMode;	
	memset(szStr, 0x20, sizeof(szStr));
	inSize=strlen(strRight);
    memcpy(&szStr[inLength-inSize], strRight, inSize);
	inSize=strlen(strLeft);
    memcpy(szStr, strLeft, strlen(strLeft));

	if (isCheckTerminalMP200() == d_OK)
		setLCDPrint27(inLine, d_LCD_ALIGNLEFT, szStr);
	else
		CTOS_LCDTPrintAligned(inLine, szStr, d_LCD_ALIGNLEFT);
}


int inReportSelecion(void)
{
    char szChoiceMsg[30 + 1];
    int bHeaderAttr = 0x01+0x04, key=0;
    char szHeaderString2[30 + 1];	

    //CTOS_LCDTClearDisplay();
	memset(szChoiceMsg, 0x00, sizeof(szChoiceMsg));
	memset(szHeaderString2, 0x00, sizeof(szHeaderString2));
	strcpy(szHeaderString2, "REPORT");

    strcat(szChoiceMsg,"SUMMARY \n");
    strcat(szChoiceMsg,"DETAIL ");
	//strcat(szChoiceMsg,"HOST INFO \n");
	//strcat(szChoiceMsg,"CRC \n");
	//strcat(szChoiceMsg,"RS232 \n");
	//strcat(szChoiceMsg,"IP \n");
	//strcat(szChoiceMsg,"ISO LOG REPORT");
	
	
    key = MenuDisplay(szHeaderString2,strlen(szHeaderString2), bHeaderAttr, 1, 1, szChoiceMsg, TRUE);
    //key = MenuDisplay(szHeaderString,strlen(szHeaderString), bHeaderAttr, 1, 1, szChoiceMsg, TRUE);	
    //key = MenuDisplay(0,0, bHeaderAttr, 1, 1, szChoiceMsg, TRUE);
    
	if (key > 0)
	{
		if (key == 1)   //SUMMARY
		{
			//return d_OK;
			inCTOS_PRINTF_SUMMARY();
		}
				
		if (key == 2)   //DETAIL
		{
			inCTOS_PRINTF_DETAIL();
		}

/*
		if (key == 3)   //HOST INFO
		{
			vdCTOS_HostInfo();
		}

		if (key == 4)   //CRC
		{
			vdCTOS_PrintCRC();
		}

		if (key == 5)   //RS232
		{
			vdCTOS_PrintRS232Report();
		}

		if (key == 6)   //IP
		{
			vdCTOS_PrintIP();
		}

		if (key == 7)   //ISO LOG REPORT
		{
			vdPrintISOLog();
		}
*/
	}
	
    return -1;
}

USHORT CTOS_LCDTClearDisplay(void)
{
	return d_OK;
}

USHORT CTOS_LCDTPrintXY(USHORT usX, USHORT usY, UCHAR* pbBuf)
{
	return d_OK;
}

//sidumili: display message
void vdDisplayMessage(char *szLine1Msg, char *szLine2Msg, char *szLine3Msg)
{
	#if 0
    CTOS_LCDTClearDisplay();
		vduiClearBelow(2);

		if ((strTCT.byTerminalType%2) == 0){
			CTOS_LCDTPrintAligned(8, szLine1Msg, d_LCD_ALIGNCENTER);	
			CTOS_LCDTPrintAligned(9, szLine2Msg, d_LCD_ALIGNCENTER);	
			CTOS_LCDTPrintAligned(10, szLine3Msg, d_LCD_ALIGNCENTER);
		}else{
			CTOS_LCDTPrintAligned(4, szLine1Msg, d_LCD_ALIGNCENTER);	
			CTOS_LCDTPrintAligned(5, szLine2Msg, d_LCD_ALIGNCENTER);	
			CTOS_LCDTPrintAligned(6, szLine3Msg, d_LCD_ALIGNCENTER);
		}
		//vduiDisplayStringCenter(4, szLine1Msg);
		//vduiDisplayStringCenter(5, szLine2Msg);
		//vduiDisplayStringCenter(6, szLine3Msg);
		WaitKey(1);
	#else
		vdDisplayMessageBox(1, 8, szLine1Msg, szLine2Msg, szLine3Msg, MSG_TYPE_INFO);
	#endif
}
//sidumili

void vdDisplayErrorMsgResp2 (char *msg, char *msg2, char *msg3)
{
   //CTOS_LCDTClearDisplay();
   
   #if 0	
   if ((strTCT.byTerminalType % 2) == 1) 
   {
      vduiDisplayStringCenter(3,msg);
      vduiDisplayStringCenter(4,msg2);
      vduiDisplayStringCenter(5,msg3);		
   }
   else 
   {
      vduiDisplayStringCenter(6, msg);
      vduiDisplayStringCenter(7, msg2);
      vduiDisplayStringCenter(8, msg3);
   }
   #else
   	vdDisplayMessageBox(1, 8, msg, msg2, msg3, MSG_TYPE_ERROR);
   #endif
	 
   CTOS_Beep();
   CTOS_Delay(1500);
   //CTOS_LCDTClearDisplay();
}

/*Display message with image -- sidumili*/
void vdDisplayMessageBox(int inColumn, int inRow,  char *msg, char *msg2, char *msg3, int msgType)
{
	BYTE szDisMsg[200 + 1] = {0};
	BYTE szMsgType[40 + 1] = {0};
    int i=0;
	
	vdDebug_LogPrintf("--vdDisplayMessageBox--");
	
	memset(szDisMsg, 0x00, sizeof(szDisMsg));
	strcpy(szDisMsg, msg);
	strcat(szDisMsg, "|");
	strcat(szDisMsg, msg2);
	strcat(szDisMsg, "|");
	strcat(szDisMsg, msg3);
	strcat(szDisMsg, "|");

    for(i=0;i<=strlen(szDisMsg);i++)
	{  
		if(szDisMsg[i]==0xe6)
		{
		   if(szDisMsg[i+1]==0x96)
		   {
		      szDisMsg[i]=0x20;
			  i++;
			  szDisMsg[i]=0x20;
		   }
	    }
		else if(szDisMsg[i]==0x99)
		{
			szDisMsg[i]=0x20;
		}
		else if(szDisMsg[i]==0xe4)
		{
			szDisMsg[i]=0x20;
		}
	}

	memset(szMsgType, 0x00, sizeof(szMsgType));
	inGetBoxMessageType(msgType, szMsgType);
	strcat(szDisMsg, szMsgType);

	vdDebug_LogPrintf("szDisMsg=[%s], msgType=[%d]", szDisMsg, msgType);
	
	usCTOSS_DisplayBox(szDisMsg);
}

/*Display message with image -- sidumili*/
void vdDisplayMessageStatusBox(int inColumn, int inRow,  char *msg, char *msg2, int msgType)
{
	BYTE szDisMsg[200 + 1] = {0};
	BYTE szMsgType[40 + 1] = {0};

	vdDebug_LogPrintf("--vdDisplayMessageStatusBox--");
	
	memset(szDisMsg, 0x00, sizeof(szDisMsg));
	strcpy(szDisMsg, msg);
	strcat(szDisMsg, "|");
	strcat(szDisMsg, msg2);
	strcat(szDisMsg, "|");

	memset(szMsgType, 0x00, sizeof(szMsgType));
	inGetBoxMessageType(msgType, szMsgType);
	strcat(szDisMsg, szMsgType);

	vdDebug_LogPrintf("szDisMsg=[%s], msgType=[%d]", szDisMsg, msgType);
	
	usCTOSS_DisplayStatusBox(szDisMsg);
}

// sidumili: Get message type
int inGetBoxMessageType(int msgType, char* szMsgType)
{
	vdDebug_LogPrintf("--inGetBoxMessageType--");
	
	switch (msgType)
	{
		case MSG_TYPE_SUCCESS:
			strcpy(szMsgType, "success");
		break;
		case MSG_TYPE_ERROR:
			strcpy(szMsgType, "error");
		break;
		case MSG_TYPE_WARNING:
			strcpy(szMsgType, "warning");
		break;
		case MSG_TYPE_INFO:
			strcpy(szMsgType, "info");
		break;
		case MSG_TYPE_QUESTION:
			strcpy(szMsgType, "question");
		break;
		case MSG_TYPE_PRINT:
			strcpy(szMsgType, "print");
		break;			
		case MSG_TYPE_PROCESS:
			strcpy(szMsgType, "process");
		break;
		case MSG_TYPE_SEND:
			strcpy(szMsgType, "send");
		break;	
		case MSG_TYPE_RECEIVE:
			strcpy(szMsgType, "receive");
		break;
		case MSG_TYPE_WAIT:
			strcpy(szMsgType, "wait");
		break;
		case MSG_TYPE_NONE:
			strcpy(szMsgType, "none");
		break;
		case MSG_TYPE_TIMEOUT:
			strcpy(szMsgType, "timeout");
		break;
		default:
			strcpy(szMsgType, "none");
		break;	
	}

	vdDebug_LogPrintf("szMsgType=[%s]", szMsgType);
	
	return d_OK;
}

void DisplayStatusLine(char *szDisplay) 
{
 	BYTE szTitle[20+1];
	BYTE szDisMsg[200];

	memset(szTitle, 0x00, sizeof(szTitle));
    szGetTransTitle(srTransRec.byTransType, szTitle); 

    strcpy(szDisMsg, szTitle);
    strcat(szDisMsg, "|");
    strcat(szDisMsg, szDisplay);
    usCTOSS_LCDDisplay(szDisMsg);
     
}

void vdDisplayScreenMessage(char *szDisplay)
{
	BYTE szTitle[20+1];
	BYTE szDisMsg[200];

	memset(szTitle, 0x00, sizeof(szTitle));
    szGetTransTitle(srTransRec.byTransType, szTitle); 

	memset(szDisMsg, 0x00, sizeof(szDisMsg));
	strcpy(szDisMsg, szTitle);
	strcat(szDisMsg, "|");
	strcat(szDisMsg, szDisplay);
	usCTOSS_LCDDisplay(szDisMsg);
	CTOS_Beep();
	CTOS_Delay(1500);
}

// sidumili: for display message with button
int inDisplayMessageBoxWithButton(int inColumn, int inRow,  char *msg, char *msg2, char *msg3, int msgType)
{
	BYTE szDisMsg[200 + 1] = {0};
	BYTE szMsgType[40 + 1] = {0};
	int inRet = d_NO;

	vdDebug_LogPrintf("--inDisplayMessageBoxWithButton--");
	
	memset(szDisMsg, 0x00, sizeof(szDisMsg));
	strcpy(szDisMsg, msg);
	strcat(szDisMsg, "|");
	strcat(szDisMsg, msg2);
	strcat(szDisMsg, "|");
	strcat(szDisMsg, msg3);
	strcat(szDisMsg, "|");

	memset(szMsgType, 0x00, sizeof(szMsgType));
	inGetBoxMessageType(msgType, szMsgType);
	strcat(szDisMsg, szMsgType);

	vdDebug_LogPrintf("szDisMsg=[%s], msgType=[%d]", szDisMsg, msgType);
	
	inRet = usCTOSS_ConfirmOK(szDisMsg);

	return inRet;

}

int inGetErrorMessages(char *szMessage1, char *szMessage2)
{
    int inErrLen1=0, inErrLen2=0;

    inErrLen1 = strlen(szErrorMessage1);
	inErrLen2 = strlen (szErrorMessage2);
      
        memcpy(szMessage1,szErrorMessage1,inErrLen1);
		memcpy(szMessage2,szErrorMessage2,inErrLen2);

    return inErrLen2;
}


