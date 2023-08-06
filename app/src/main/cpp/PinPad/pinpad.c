#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
//#include <sbdf_kms2.h>


#include "../Database/DatabaseFunc.h"
#include "pinpad.h"
#include "../Includes/wub_lib.h"
#include "../Includes/POSTypedef.h"
#include "..\Debug\Debug.h"
#include "../Ui/Display.h"
#include "..\Includes\CTOSInput.h"



#define PIN_POSITION_X	30
#define PIN_POSITION_Y	10

int inInitializePinPad(void)
{
		CTOS_KMS2Init();
		//TEST_Write3DES_Plaintext();
		return d_OK;
}
//For testing and development only, hardcode the key
void TEST_Write3DES_Plaintext(void)
{
	USHORT KeySet;
	USHORT KeyIndex;
	CTOS_KMS2KEYWRITE_PARA para;
	USHORT ret;
	BYTE KeyData[16];
	BYTE str[17];
	BYTE key;
	BYTE szInitialVector[8], szDataIn[100], szMAC[8];


	
	vdDebug_LogPrintf("saturn TEST_Write3DES_Plaintext");
	KeySet = 0XC000;
	KeyIndex = 0x0001;
	memcpy(KeyData, "\x11\x11\x11\x11\x22\x22\x22\x22\x33\x33\x33\x33\x44\x44\x44\x44", 16);

	memset(&para, 0x00, sizeof(CTOS_KMS2KEYWRITE_PARA));
	para.Version = 0x01;
	para.Info.KeySet = KeySet;
	para.Info.KeyIndex = KeyIndex;
	para.Info.KeyType = KMS2_KEYTYPE_3DES;
	para.Info.KeyVersion = 0x07;
	para.Info.KeyAttribute = SBKMS2_KEYATTRIBUTE_PIN | SBKMS2_KEYATTRIBUTE_MAC;
	para.Protection.Mode = KMS2_KEYPROTECTIONMODE_PLAINTEXT;
	para.Value.pKeyData = KeyData;
	para.Value.KeyLength = IPP_TDES_KEY_SIZE;
	CTOS_KMS2KeyWrite(&para);

    #if 0
	strcpy(srTransRec.szPAN,"1234567890123456");
	srTransRec.HDTid = 6;
	srTransRec.byTransType = SALE;
	strcpy(strCST.szCurSymbol,"SGD");
	strcpy(srTransRec.szTotalAmount,"1");
	inGetIPPPin();
	memset(szDataIn, 'A', 100);
	memset(szInitialVector, 0, 8);
	memset(szMAC, 0, 8);
	inIPPGetMAC(szDataIn, 8, szInitialVector, szMAC);
	#endif
}
	
void inCTOS_DisplayCurrencyAmount(BYTE *szAmount, int inLine)
{
	char szDisplayBuf[30];
	BYTE baAmount[20];
    BYTE szStr[30];
	
	DebugAddSTR("inCTOS_DisplayCurrencyAmount","Processing...",20);
	
	memset(baAmount, 0x00, sizeof(baAmount));
	wub_hex_2_str(szAmount, baAmount, 6);

	memset(szStr, 0x00, sizeof(szStr));
	vdCTOS_FormatAmount("NNN,NNN,NNn.nn",baAmount, szStr);
	sprintf(szDisplayBuf, "%s %s", strCST.szCurSymbol, szStr);
	vdDebug_LogPrintf("AAA - inCTOS_DisplayCurrencyAmount szDisplayBuf:%s",szDisplayBuf);
	//setLCDPrint(inLine, DISPLAY_POSITION_LEFT, szDisplayBuf);
	setLCDPrint(inLine, DISPLAY_POSITION_CENTER, szDisplayBuf);
	
	
}

void OnGetPINDigit(BYTE NoDigits)
{
	BYTE i;
	
	for(i=0;i<NoDigits;i++)
		CTOS_LCDTPrintXY(PIN_POSITION_X+i, PIN_POSITION_Y,"*");
		
	for(i=NoDigits;i<12;i++)
		CTOS_LCDTPrintXY(PIN_POSITION_X+i, PIN_POSITION_Y," ");
	
   DebugAddINT("OnGetPINDigit", NoDigits);

}

void OnGetPINCancel(void)
{
   DebugAddINT("OnGetPINCancel", 1);

}

void OnGetPINBackspace(BYTE NoDigits)
{
	BYTE i;
	
	for(i=0;i<NoDigits;i++)
		CTOS_LCDTPrintXY(PIN_POSITION_X+i, PIN_POSITION_Y,"*");
		
	for(i=NoDigits;i<12;i++)
		CTOS_LCDTPrintXY(PIN_POSITION_X+i, PIN_POSITION_Y," ");
   DebugAddINT("OnGetPINBackspace", NoDigits);
}

void vdDisplayErrorMessage(USHORT errorID)
{
	char szErr[50+1];

	memset(szErr,0x00,sizeof(szErr));
	switch(errorID)
	{
		case d_KMS2_GET_PIN_TIMEOUT:
			strcpy(szErr,"TIME OUT");
			break;
		case d_KMS2_GET_PIN_ABORT:
			strcpy(szErr,"GET PIN ABORT");
			break;
		default:
			strcpy(szErr,"GET PIN ABORT");
			break;
	}
	vdDisplayErrorMsg(1, 8, szErr);
}
int inGetIPPPin(void)
{
	CTOS_KMS2PINGET_PARA_VERSION_7 stPinGetPara;
    int inRet;
	
	BYTE str[40],key;
	USHORT ret;



	
	usCTOSS_GetPinUI();

  
  	//CTOS_LCDTClearDisplay();

	DebugAddSTR("inGetIPPPin","Processing...",20);
	inDCTRead(srTransRec.HDTid);

	vdDebug_LogPrintf("saturn GETIPPpIN");

    /*check for keys if injected -- sidumili*/
    //inRet = inCheckKeys(strDCT.usKeySet, strDCT.usKeyIndex);
    //if (inRet != d_OK)
    //    return(inRet);
    /*check for keys if injected -- sidumili*/

    CTOS_LCDFontSelectMode(d_FONT_TTF_MODE);
    CTOS_LCDTSelectFontSize(d_FONT_20x40);

    //vdDispTransTitle(srTransRec.byTransType);
	
	if(srTransRec.byTransType != BAL_INQ)
	    inCTOS_DisplayCurrencyAmount(srTransRec.szTotalAmount, 5);
	    
	//setLCDPrint(4, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine1);
	//setLCDPrint(5, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine2);
	
	setLCDPrint(7, DISPLAY_POSITION_CENTER, strDCT.szDisplayLine1);
	setLCDPrint(8, DISPLAY_POSITION_CENTER, strDCT.szDisplayLine2);
	
	memset(&stPinGetPara, 0x00, sizeof(CTOS_KMS2PINGET_PARA_VERSION_7));
	stPinGetPara.Version = 0x07;
	stPinGetPara.PIN_Info.BlockType = KMS2_PINBLOCKTYPE_ANSI_X9_8_ISO_0;
	stPinGetPara.PIN_Info.PINDigitMinLength = strDCT.inMinPINDigit;
	stPinGetPara.PIN_Info.PINDigitMaxLength = strDCT.inMaxPINDigit;

	stPinGetPara.Protection.CipherKeyIndex = strDCT.usKeyIndex;
	stPinGetPara.Protection.CipherKeySet = strDCT.usKeySet;
	stPinGetPara.Protection.SK_Length = IPP_TDES_KEY_SIZE;
	stPinGetPara.Protection.pSK = strDCT.szPINKey;
	stPinGetPara.Protection.CipherMethod = KMS2_PINCIPHERMETHOD_ECB;
	stPinGetPara.AdditionalData.InLength = strlen(srTransRec.szPAN);
	stPinGetPara.AdditionalData.pInData = (BYTE*)srTransRec.szPAN;
        
	stPinGetPara.Control.Timeout = 30;
	stPinGetPara.Control.NULLPIN = FALSE;
	stPinGetPara.PINOutput.EncryptedBlockLength = 8;
	stPinGetPara.PINOutput.pEncryptedBlock = srTransRec.szPINBlock;
	stPinGetPara.EventFunction.OnGetPINBackspace = OnGetPINBackspace;
	stPinGetPara.EventFunction.OnGetPINCancel = OnGetPINCancel;
	stPinGetPara.EventFunction.OnGetPINDigit = OnGetPINDigit;

	//CTOS_LCDSelectModeEx(d_LCD_TEXT_320x240_MODE, FALSE); // Fix for undisplay of * during pin -- sidumili	
	ret = CTOS_KMS2PINGet((CTOS_KMS2PINGET_PARA *)&stPinGetPara);
	
	vdDebug_LogPrintf("SATURN CTOS_KMS2PINGet RET %d",ret);
	//aaronnino BANCNET bancnet fix on issue #0006 Unexpected "InProcessing=290F" appears in canceling Pin number start
	if(ret != d_OK)
	{
		//CTOS_LCDTClearDisplay();
		//vdDispTransTitle(srTransRec.byTransType);

		switch (ret)
		{
			case 10511:
				vdSetErrorMessage("USER CANCEL");
				vdDisplayErrorMsg(1, 8, "USER CANCEL");
				inRet = IPP_CANCEL;
				break; 
			case 10512:
				vdSetErrorMessage("PIN TIMEOUT");
				vdDisplayErrorMsg(1, 8, "PIN TIMEOUT");
				inRet = IPP_TIMEOUT;
				break; 
			default:
				vdSetErrorMessage("PIN ERROR");
			    vdDisplayErrorMsg(1, 8, "PIN ERROR");
				inRet = IPP_ERROR;
				break;
		}

		vdDebug_LogPrintf("CTOS_KMS2PINGet::ret[%d] | inRet[%d]", ret, inRet);
		
		return(inRet);
	}

	vdDebug_LogPrintf("saturn CTOS_KMS2PINGet::SUCCESS!!!");
	
	//aaronnino BANCNET bancnet fix on issue #0006 Unexpected "InProcessing=290F" appears in canceling Pin number end
	return d_OK;
}

int inIPPGetMAC(BYTE *szDataIn, int inLengthIn, BYTE *szInitialVector, BYTE *szMAC)
{
	CTOS_KMS2MAC_PARA para;
	USHORT ret;
	BYTE key,str[40];
	
	//CTOS_LCDTClearDisplay();
	DebugAddSTR("inGetIPPMAC","Processing...",20);    
	
	memset(&para, 0x00, sizeof(CTOS_KMS2MAC_PARA));
	para.Version = 0x01;
	para.Protection.CipherKeySet = strDCT.usKeySet;
	para.Protection.CipherKeyIndex = strDCT.usKeyIndex;
	para.Protection.CipherMethod = KMS2_MACMETHOD_CBC;
	para.Protection.SK_Length = IPP_TDES_KEY_SIZE;
	para.Protection.pSK = strDCT.szMACKey;
	para.ICV.Length = 8;
	para.ICV.pData = szInitialVector;
	para.Input.Length = inLengthIn;
	para.Input.pData = szDataIn;
	para.Output.pData = szMAC;
	
	ret = CTOS_KMS2MAC(&para);
	if(ret != d_OK)
		return ret;
	return d_OK;		
}

int inCalculateMAC(BYTE *szDataIn, int inLengthIn, BYTE *szMAC)
{
	BYTE szInitialVector[8];

	inIPPGetMAC(szDataIn, inLengthIn,  szInitialVector, szMAC);
}
	
int inCheckKeys(USHORT ushKeySet, USHORT ushKeyIndex)
{
    USHORT rtn;
    
    vdDebug_LogPrintf("SET[%04X] IDX[%04X]", ushKeySet, ushKeyIndex);
    
    rtn = CTOS_KMS2KeyCheck(ushKeySet, ushKeyIndex);
    if (rtn != d_OK)
        vdDisplayErrorMsg(1, 8, "PLEASE INJECT KEY");
    
    return rtn;
}

int htoi(char s[])
{
    int i;
    int n = 0;
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))
    {
        i = 2;
    }
    else
    {
        i = 0;
    }
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)
    {
        if (tolower(s[i]) > '9')
        {
            n = 16 * n + (10 + tolower(s[i]) - 'a');
        }
        else
        {
            n = 16 * n + (tolower(s[i]) - '0');
        }
    }
    return n;
}

int GetPIN_With_3DESDUKPT(void)
{
	CTOS_KMS2PINGET_PARA stPinGetPara;
	USHORT ret;
	BYTE str[17];
	BYTE key;
	BYTE PINBlock[16];
	BYTE *pCipherKey;
	BYTE CipherKeyLength;
	BYTE DecipherPINBlock[16];
	BYTE ExpectPINBlock[16];
	BYTE ksn[10];
	BYTE ksn_Len;
	int inRet;
	
	DebugAddSTR("inGetIPPPin","Processing...",20);
	inDCTRead(srTransRec.HDTid);

    /*check for keys if injected -- sidumili*/
    inRet = inCheckKeys(strDCT.usKeySet, strDCT.usKeyIndex);
    if (inRet != d_OK)
        return(inRet);
    /*check for keys if injected -- sidumili*/

	//CTOS_LCDTClearDisplay();
	vdDispTransTitle(srTransRec.byTransType);

	if(srTransRec.byTransType != BAL_INQ)
	    inCTOS_DisplayCurrencyAmount(srTransRec.szTotalAmount, 3);
	setLCDPrint(4, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine1);
	setLCDPrint(5, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine2);

		
	memset(&stPinGetPara, 0x00, sizeof(CTOS_KMS2PINGET_PARA));
	stPinGetPara.Version = 0x01;
	stPinGetPara.PIN_Info.BlockType = KMS2_PINBLOCKTYPE_ANSI_X9_8_ISO_0;
	stPinGetPara.PIN_Info.PINDigitMinLength = 4;
	stPinGetPara.PIN_Info.PINDigitMaxLength = 12;
	stPinGetPara.Protection.CipherKeySet = 0x1010;
	stPinGetPara.Protection.CipherKeyIndex = 0x0001;
	stPinGetPara.Protection.CipherMethod = KMS2_PINCIPHERMETHOD_ECB;
	stPinGetPara.Protection.SK_Length = 0;
	//stPinGetPara.AdditionalData.InLength = 16;
	//stPinGetPara.AdditionalData.pInData = (BYTE*)TestPAN;
	stPinGetPara.AdditionalData.InLength = strlen(srTransRec.szPAN);
	stPinGetPara.AdditionalData.pInData = (BYTE*)srTransRec.szPAN;
	stPinGetPara.DUKPT_PARA.IsUseCurrentKey = FALSE;
	stPinGetPara.PINOutput.EncryptedBlockLength = 8;
	stPinGetPara.PINOutput.pEncryptedBlock = PINBlock;
	stPinGetPara.Control.Timeout = 10;
	stPinGetPara.Control.AsteriskPositionX = 1;
	stPinGetPara.Control.AsteriskPositionY = 2;
	stPinGetPara.Control.NULLPIN = FALSE;
	stPinGetPara.Control.piTestCancel = NULL;
	ret = CTOS_KMS2PINGet(&stPinGetPara);
	if(ret != d_OK)
	{
		sprintf(str, "ret = 0x%04X", ret);
		CTOS_LCDTPrintXY(1, 8, str);
		goto __Leave;
	}
	CTOS_LCDTPrintXY(1, 5, "Get PIN OK");
	
	ksn_Len = 10;
	ret = CTOS_KMS2DUKPTGetKSN(stPinGetPara.Protection.CipherKeySet, stPinGetPara.Protection.CipherKeyIndex, ksn, &ksn_Len);
	if(ret != d_OK)
	{
		sprintf(str, "ret = 0x%04X", ret);
		CTOS_LCDTPrintXY(1, 8, str);
		goto __Leave;
	}
	CTOS_LCDTPrintXY(1, 6, "Get KSN OK");
		
	CTOS_LCDTPrintXY(1, 8, "GetPIN Done");
__Leave:
	CTOS_KBDGet(&key);			
}


USHORT inCTOS_KMS2PINGetEx3Des(USHORT KeySet,  USHORT KeyIndex,  BYTE* pInData, BYTE* szPINBlock, BYTE* szKSN, USHORT pinBypassAllow)
{

    BYTE szIndex[2]={0};
    USHORT inRet = 0;

    vdDebug_LogPrintf("saturn inGetPIN_With_3DESDUKPTEx Start KeySet[%d] KeyIndex[%d]", KeySet, KeyIndex);
    vdDebug_LogPrintf("pInData[%s]", pInData);
    inCallJAVA_inGetPIN_With_3DESDUKPTEx(KeySet, KeyIndex, pInData, szPINBlock, szKSN,  szIndex, pinBypassAllow);
    vdDebug_LogPrintf("szIndex[%c]", szIndex[0]);
    vdDebug_LogPrintf("inGetPIN_With_3DESDUKPTEx End szIndex[%c]", szIndex[0]);

    inRet = atoi(szIndex);

    if(inRet == 0)
    {
        return d_OK;
    }
    else
    {
        inRet = htoi(szKSN);
        return inRet;
    }
}

#if 0
int inGetIPPPinEx(void)
{
    int inRet;
	
	BYTE str[40],key;
	USHORT ret;
	BYTE PINBlockStr[20];
    BYTE ksn[20];
    BYTE ksnStr[20];;

		
	vdDebug_LogPrintf("saturn --inGetIPPPinEx--");
	inDCTRead(srTransRec.HDTid);

	vdDebug_LogPrintf("saturn CTOS_KMS2Init");
	CTOS_KMS2Init();


	CTOS_LCDTClearDisplay();
	vdDispTransTitle(srTransRec.byTransType);

	if(srTransRec.byTransType != BAL_INQ)
	    inCTOS_DisplayCurrencyAmount(srTransRec.szTotalAmount, 3);
	setLCDPrint(4, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine1);
	setLCDPrint(5, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine2);
	

	vdDebug_LogPrintf("saturn before inCTOS_KMS2PINGetEx3Des");

	
	
	memset(PINBlockStr, 0x00, sizeof(PINBlockStr));
	memset(ksnStr, 0x00, sizeof(ksnStr));
	
	ret = inCTOS_KMS2PINGetEx3Des(strDCT.usKeySet, strDCT.usKeyIndex, srTransRec.szPAN, PINBlockStr, ksnStr, 0);
	
	//wub_str_2_hex(PINBlockStr, srTransRec.szPINBlock, strlen(PINBlockStr));
    vdDebug_LogPrintf("saturn inCTOS_KMS2PINGetExDukpt[%d]", ret);
	vdDebug_LogPrintf("saturn pin block %s", PINBlockStr);
    vdPCIDebug_HexPrintf("saturn GetPIN_With_3DESDUKPTEx srTransRec.szPINBlock", PINBlockStr, 8);


	if(ret != d_OK)
	{
		//if(ret == d_KMS2_GET_PIN_NULL_PIN)
		//{
		//	CTOS_LCDTPrintXY(1, 8, "PIN BYPASSED");
		//	CTOS_Delay(300);
		//	return d_KMS2_GET_PIN_NULL_PIN;
		//}
		if (ret == 10511)
		{
			vdSetErrorMessage("USER CANCEL");
			return(d_NO);
		}
		else if (ret == 10512) /*Timeout --sidumili*/ 
		{
			vdDisplayErrorMsg(1, 8, "PIN TIMEOUT");
			return(d_NO);
		}
		else if (ret == d_KMS2_KEY_NOT_EXIST)
		{
			vdDisplayErrorMsg(1, 8, "PLEASE INJECT KEY");
			return(d_NO);
		}
		else
		{
			/* MCCCLG: Added message if user cancelled during pin entry - start -- jzg*/
			if(ret == d_KMS2_GET_PIN_ABORT)
			{
			vdDisplayErrorMsg(1, 8, "USER CANCEL");
		}
		else
		{
			sprintf(str, "%s=%04X", strDCT.szDisplayProcessing, ret);
			vdDisplayErrorMsg(1, 8, str);
		}
		/* MCCCLG: Added message if user cancelled during pin entry - end -- jzg*/
		return ret;
	}
}

#if 0
	if(ret != d_OK)
	{
		sprintf(str, "%s=%04X", strDCT.szDisplayProcessing, ret);
		CTOS_LCDTPrintXY(1, 8, str);
		CTOS_KBDGet(&key);
		return ret;
	}
#endif
	return d_OK;
}
#else
int inGetIPPPinEx(void)
{
    int inRet;
	BYTE str[40],key;
	USHORT ret;
	BYTE PINBlockStr[20+1];
    BYTE ksn[20+1];
    BYTE ksnStr[20+1];
	BYTE szBuffer[70];
	BYTE strPINKEY[32+1];

	

		
	vdDebug_LogPrintf("saturn --inGetIPPPinEx--");
	inDCTRead(srTransRec.HDTid);

	vdDebug_LogPrintf("CTOS_KMS2Init");
	CTOS_KMS2Init();



	//CTOS_LCDTClearDisplay();
	vdDispTransTitle(srTransRec.byTransType);

	if(srTransRec.byTransType != BAL_INQ)
	    inCTOS_DisplayCurrencyAmount(srTransRec.szTotalAmount, 3);
	setLCDPrint(4, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine1);
	setLCDPrint(5, DISPLAY_POSITION_LEFT, strDCT.szDisplayLine2);
	

	vdDebug_LogPrintf("saturn before ");

	
	
	memset(PINBlockStr, 0x00, sizeof(PINBlockStr));
	memset(ksnStr, 0x00, sizeof(ksnStr));

	vdDebug_LogPrintf("saturn keyset = %d keyindex = %d", strDCT.usKeySet, strDCT.usKeyIndex);
	
	vdDebug_LogPrintf("saturn keyset = %x keyindex = %x", strDCT.usKeySet, strDCT.usKeyIndex);

	memset(szBuffer, 0x00, sizeof(szBuffer));
	strcpy(szBuffer, srTransRec.szPAN);
	strcat(szBuffer,"|");

	memset(strPINKEY, 0x00, sizeof(strPINKEY));
    wub_hex_2_str(strDCT.szPINKey, strPINKEY, 16);
	strcat(szBuffer, strPINKEY);

	vdDebug_LogPrintf("saturn pimdata %s", szBuffer);
	
	//ret = inCTOS_KMS2PINGetEx3Des(strDCT.usKeySet, strDCT.usKeyIndex, srTransRec.szPAN, PINBlockStr, ksnStr, 0);
	ret = inCTOS_KMS2PINGetEx3Des(strDCT.usKeySet, strDCT.usKeyIndex, szBuffer, PINBlockStr, ksnStr, 0);

	vdPCIDebug_HexPrintf("saturn GetPIN_With_3DESDUKPTEx srTransRec.szPINBlock", srTransRec.szPINBlock, 8);
	wub_str_2_hex(PINBlockStr, srTransRec.szPINBlock, strlen(PINBlockStr));

	
    if(ret != d_OK)
    {
        if(ret == d_KMS2_GET_PIN_NULL_PIN)
        {
            CTOS_LCDTPrintXY(1, 8, "PIN BYPASSED");
            CTOS_Delay(300);
            return d_KMS2_GET_PIN_NULL_PIN;
        }
        else if (ret == 10511)
        {
            vdSetErrorMessage("USER CANCEL");
            return(d_NO);
        }
        else if (ret == 10512) /*Timeout --sidumili*/ 
        {
            vdDisplayErrorMsg(1, 8, "PIN TIMEOUT");
            return(d_NO);
        }
        else if (ret == d_KMS2_KEY_NOT_EXIST)
        {
            vdDisplayErrorMsg(1, 8, "PLEASE INJECT KEY");
            return(d_NO);
        }
        else
        {
            /* MCCCLG: Added message if user cancelled during pin entry - start -- jzg*/
            if(ret == d_KMS2_GET_PIN_ABORT)
            {
                vdDisplayErrorMsg(1, 8, "USER CANCEL");
            }
            else
            {
                sprintf(str, "%s=%04X", strDCT.szDisplayProcessing, ret);
                vdDisplayErrorMsg(1, 8, str);
            }
            /* MCCCLG: Added message if user cancelled during pin entry - end -- jzg*/
            return ret;
        }
    }
		
	return d_OK;
}


#endif

