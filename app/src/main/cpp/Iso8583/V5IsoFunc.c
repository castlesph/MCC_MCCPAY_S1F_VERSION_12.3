#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <EMVAPLib.h>
#include <EMVLib.h>
#include <emv_cl.h>

#include "../Includes/wub_lib.h"
#include "../Includes/Encryption.h"
#include "../Includes/msg.h"
#include "../Includes/myEZLib.h"

#include "../Includes/V5IsoFunc.h"
#include "../Includes/POSTypedef.h"
#include "../Comm/V5Comm.h"
#include "../FileModule/myFileFunc.h"
#include "../UI/Display.h"
#include "../Includes/Trans.h"
#include "../UI/Display.h"
#include "../Accum/Accum.h"
#include "../POWRFAIL/POSPOWRFAIL.h"
#include "../DataBase/DataBaseFunc.h"
#include "../Includes/POSTrans.h"
#include "..\Debug\Debug.h"
#include "..\Includes\POSTrans.h"
#include "..\Includes\Showbmp.h"
#include "..\Includes\POSHost.h"
#include "..\Includes\ISOEnginee.h"
#include "..\Includes\EFTSec.h"

#include "../ctls/POSCtls.h"

#include <emvaplib.h>

int inDataCnt;
BYTE TempKey;
int inFinalSend;
BYTE szDataForMAC[512];
int  inMacMsgLen;


BYTE byField_02_ON;
BYTE byField_14_ON;
BYTE byField_35_ON;
BYTE byField_45_ON;
BYTE byField_48_ON;

ISO_FUNC_TABLE srIsoFuncTable[] =
{
    {
        inPackMessageIdData,/*inPackMTI*/
		inPackPCodeData,/*inPackPCode*/
        vdModifyBitMapFunc,/*vdModifyBitMap*/
        inCheckIsoHeaderData,/*inCheckISOHeader*/
        inAnalyseReceiveData,/*inTransAnalyse*/
        inAnalyseAdviceData  /*inAdviceAnalyse*/
    },
};

static TRANS_DATA_TABLE *ptsrISOEngTransData = NULL;

void vdSetISOEngTransDataAddress(TRANS_DATA_TABLE *srTransPara)
{
    ptsrISOEngTransData= srTransPara;
}

TRANS_DATA_TABLE* srGetISOEngTransDataAddress(void)
{
    return ptsrISOEngTransData;
}



void vdDispTextMsg(char *szTempMsg)
{    
//    CTOS_LCDUClearDisplay();
//    CTOS_LCDTPutchXY(1,8,"TEST");
//    CTOS_KBDGet(&TempKey);
}

void vdDecideWhetherConnection(TRANS_DATA_TABLE *srTransPara)
{
    vdMyEZLib_LogPrintf("**vdDecideWhetherConnection START**");
    
    vdMyEZLib_LogPrintf(". BefTrnsType(%d) UpLoad(%d) Offline (%d)",srTransPara->byTransType,
                                                              srTransPara->byUploaded,
                                                              srTransPara->byOffline);
    
    if(srTransPara->byTransType != VOID)
    {    
        if(srTransPara->byTransType == SALE_TIP || srTransPara->byTransType == SALE_ADJUST)
        {
            srTransPara->byOffline = CN_TRUE;
           
        }
        else if(srTransPara->byTransType == SALE_OFFLINE)
        {
            srTransPara->byUploaded = CN_FALSE;
            srTransPara->byOffline = CN_TRUE;
        }
        else if(srTransPara->byTransType == SALE &&
            srTransPara->byEntryMode == CARD_ENTRY_ICC &&
            srTransPara->shTransResult == TRANS_AUTHORIZED &&
            0 == memcmp(&srTransPara->szAuthCode[0], "Y1", 2) && 
            0x40 == srTransPara->stEMVinfo.T9F27)
        {
            srTransPara->byUploaded = CN_FALSE;
            srTransPara->byOffline = CN_TRUE;
        }
        else if(srTransPara->byTransType == SALE &&
            srTransPara->byEntryMode != CARD_ENTRY_ICC &&
            srTransPara->shTransResult == TRANS_AUTHORIZED &&
            srTransPara->byOffline == CN_TRUE &&
            fAmountLessThanFloorLimit() == d_OK )
        {
            srTransPara->byUploaded = CN_FALSE;
            srTransPara->byOffline = CN_TRUE;
        }
        else
        {
            srTransPara->byUploaded = CN_TRUE;
            srTransPara->byOffline = CN_FALSE;
        }
    }
    else
    {
        //Start Should be Online void the Intial SALE amount.
        if(srTransPara->byOrgTransType == SALE)
        {
            if(srTransPara->byUploaded == CN_FALSE) //Y1 or below floor limit, and not upload yet
                srTransPara->byOffline = CN_TRUE;
            else
                srTransPara->byOffline = CN_FALSE;
        }
        else if(srTransPara->byOrgTransType == SALE_OFFLINE)
        {
            if(srTransPara->byUploaded == CN_FALSE)
                srTransPara->byOffline = CN_TRUE;
            else
                srTransPara->byOffline = CN_FALSE; //piggy back and upload to host already
        }
        else if(srTransPara->byOrgTransType == SALE_TIP)
        {
            if(srTransPara->byUploaded == CN_FALSE) //Y1 or below floor limit, and not upload yet
                srTransPara->byOffline = CN_TRUE;
            else
                srTransPara->byOffline = CN_FALSE;
        }
        //End Should be Online void the Intial SALE amount.
    }
    vdDebug_LogPrintf(". AftTrnsType(%d) srTransPara->byOrgTransType[%d]byEntryMode[%d] shTransResult[%d] szAuthCode[%s] 9F27[%02X] UpLoad(%d) Offline (%d)",
                                                              srTransPara->byTransType,
                                                              srTransPara->byOrgTransType,
                                                              srTransPara->byEntryMode,
                                                              srTransPara->shTransResult,
                                                              srTransPara->szAuthCode,
                                                              srTransPara->stEMVinfo.T9F27,
                                                              srTransPara->byUploaded,
                                                              srTransPara->byOffline);
    
    vdMyEZLib_LogPrintf("**vdDecideWhetherConnection END**");
    return;
}


/************************************************************************
Function Name: inBuildAndSendIsoData()
Description:
    To generate iso data and send to host
Parameters:
    [IN] srTransPara

Return: ST_SUCCESS  
        ST_ERROR
        TRANS_COMM_ERROR
        ST_SEND_DATA_ERR
        ST_UNPACK_DATA_ERR
    
************************************************************************/
int inBuildAndSendIsoData(void)
{
    int inResult,i;
    char szErrorMessage[30+1];
    char szBcd[INVOICE_BCD_SIZE+1];
    TRANS_DATA_TABLE *srTransPara;
    TRANS_DATA_TABLE srTransParaTmp;
    TRANS_DATA_TABLE srTransTemp;

    /*albert - start - fix stan issue*/ 
    //memset(szBcd, 0x00, sizeof(szBcd));
	//memcpy(szBcd, strHDT.szTraceNo, INVOICE_BCD_SIZE);    
	//inBcdAddOne(szBcd, strHDT.szTraceNo, INVOICE_BCD_SIZE);
	/*albert - end - fix stan issue*/
	
    srTransRec.ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
    //if ((VOID != srTransRec.byTransType) && (SALE_TIP != srTransRec.byTransType))
        //srTransRec.ulOrgTraceNum = srTransRec.ulTraceNum;
    inHDTSave(strHDT.inHostIndex);
    
    srTransPara = &srTransRec;

	vdDebug_LogPrintf("srTransRec.ulTraceNum: %d", srTransRec.ulTraceNum);
	vdDebug_LogPrintf("srTransPara->ulTraceNum: %d", srTransPara->ulTraceNum);
	vdDebug_LogPrintf("strHDT.inHostIndex: %d", strHDT.inHostIndex);

	vdDebug_LogPrintf("**inBuildAndSendIsoData V02**");
	inCTLOS_Updatepowrfail(PFR_BEGIN_SEND_ISO);

    strHDT.fSignOn = CN_TRUE; //notsupport SignON
        
    if(strHDT.fSignOn == CN_FALSE)
    {
        if( (srTransRec.byTransType == SALE)    ||  // SALE
            (srTransRec.byTransType == REFUND)  ||  // REFUND
            (srTransRec.byTransType == PRE_AUTH)||  // PREAUTH
            (srTransRec.byTransType == SETTLE)    // SETTLE
          )
        {
            i = srTransPara->HDTid;
            inHDTRead(srTransPara->HDTid);
            memcpy(&srTransParaTmp, srTransPara, sizeof(TRANS_DATA_TABLE));
            memset( &srTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
            //CTOS_LCDTClearDisplay();
			
			DebugAddSTR("Sign on false","---",12);  
            //SignOnTrans(i);            
            if(strHDT.fSignOn == CN_FALSE)
            {
                srTransRec.shTransResult = TRANS_TERMINATE;
				vdSetErrorMessage("TRANS TERMINATE");
                inCTOS_inDisconnect();
                return ST_ERROR;
            }
            memset( &srTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
            //CTOS_LCDTClearDisplay();
            memcpy(srTransPara, &srTransParaTmp, sizeof(TRANS_DATA_TABLE));
            inHDTRead(srTransPara->HDTid);
            srTransPara->ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
        }
    }
    //CTOS_LCDTClearDisplay();
    //vdDispTransTitle(srTransRec.byTransType);

	#if 0
	if ((strTCT.byTerminalType % 2) == 0)
		setLCDPrint(V3_ERROR_LINE_ROW, DISPLAY_POSITION_LEFT, "Processing...        ");
	else	
    	setLCDPrint(8, DISPLAY_POSITION_LEFT, "Processing...        ");  
	#else
		//vdCTOS_DispStatusMessage("PROCESSING");
	#endif
	
    vdDecideWhetherConnection(srTransPara);
    
    if (CN_FALSE == srTransPara->byOffline)
    {        
        ULONG ulTraceNum; 		

		if(srTransPara->byTransType == SETTLE)
		{
			strMMT[0].fMustSettFlag = CN_TRUE;
			inMMTSave(strMMT[0].MMTid);
		}

		/*save and restore transaction after doing reversal*/
        memset(&srTransTemp, 0x00, sizeof(TRANS_DATA_TABLE));
        memcpy(&srTransTemp, &srTransRec, sizeof(TRANS_DATA_TABLE));			
		
        if(VS_TRUE == strTCT.fDemo)
        {
            vdDebug_LogPrintf("DEMO Call Connect!!");
        }
        else
        {
            vdDebug_LogPrintf("Call Connect!!sharecom[%d]",strTCT.fShareComEnable);

			if(srTransPara->fLogon == TRUE)
                srTransPara->fLogon=FALSE;
			else
			{
                if (srCommFuncPoint.inConnect(&srTransRec) != ST_SUCCESS)
                {
                    if(srTransPara->shTransResult == 0)
                        srTransPara->shTransResult = TRANS_COMM_ERROR;
                    inCTOS_inDisconnect();
                    vdSetErrorMessage("CONNECT FAILED");

					if(fGetECRTransactionFlg())
                    {
                         vdSetECRResponse(ECR_COMMS_ERR);
                         //inMultiAP_ECRSendSuccessResponse();
                    }
					
                    if(strCPT.fCommBackUpMode == CN_TRUE && strCPT.inCommunicationMode != DIAL_UP_MODE)
                    {
                        if(inCTOS_CommsFallback(strHDT.inHostIndex) != d_OK)
                            return ST_ERROR;
                        
                        if (srCommFuncPoint.inConnect(&srTransRec) != ST_SUCCESS)
                        {
                            if(srTransPara->shTransResult == 0)
                                srTransPara->shTransResult = TRANS_COMM_ERROR;
							
							CTOS_Beep();
							CTOS_Delay(1500);

                            inCTOS_inDisconnect();
                            vdSetErrorMessage("CONNECT FAILED"); 

                            if(fGetECRTransactionFlg())
                            {
                                 vdSetECRResponse(ECR_COMMS_ERR);
                                 //inMultiAP_ECRSendSuccessResponse();
                            }
							return ST_ERROR;
                        }						
                    }
					else
                        return ST_ERROR;    
                }		
			}
        }
        
        if (inProcessReversal(srTransPara) != ST_SUCCESS)            
        {
            inCTOS_inDisconnect();
			//vdSetErrorMessage("Reversal Error");
			vdSetErrorMessage("NO RESPONSE FROM HOST");
            return ST_ERROR;
        }
        
        if(srTransPara->byTransType == SETTLE)
        {
            if ((inResult = inProcessEMVTCUpload(srTransPara, -1)) != ST_SUCCESS)
            {
                vdDebug_LogPrintf(". inProcessAdviceTrans(%d) ADV_ERROR!!", inResult);
                inCTOS_inDisconnect();
				vdSetErrorMessage("TC Upload Error");
                return ST_ERROR;
            }
            
            if ((inResult = inProcessAdviceTrans(srTransPara, -1)) != ST_SUCCESS)
            {
                vdDebug_LogPrintf(". inProcessAdviceTrans(%d) ADV_ERROR!!", inResult);
                inCTOS_inDisconnect();
				vdSetErrorMessage("Advice Error");
                return ST_ERROR;
            }
        }

		/*save and restore transaction after doing reversal*/
		ulTraceNum=srTransPara->ulTraceNum;
        memcpy(srTransPara, &srTransTemp, sizeof(TRANS_DATA_TABLE));
		srTransPara->ulTraceNum=ulTraceNum;
    }
    else
    {
        CTOS_RTC SetRTC;
        BYTE szCurrentTime[20];
    
        CTOS_RTCGet(&SetRTC);
	    sprintf(szCurrentTime,"%02d%02d",SetRTC.bMonth, SetRTC.bDay);
        wub_str_2_hex(szCurrentTime,srTransPara->szDate,DATE_ASC_SIZE);
        sprintf(szCurrentTime,"%02d%02d%02d", SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
        wub_str_2_hex(szCurrentTime,srTransPara->szTime,TIME_ASC_SIZE);
    }
    
    if(srTransPara->byTransType == SETTLE)
    {
        strMMT[0].fMustSettFlag = CN_TRUE;
        inMMTSave(strMMT[0].MMTid);
    }
    else
    {
        /*for TLE field 57*/
        byField_02_ON = 0;
        byField_14_ON = 0;
        byField_35_ON = 0;
        byField_45_ON = 0;
        byField_48_ON = 0;
		
        if ((VOID != srTransPara->byTransType) && (SALE_TIP != srTransPara->byTransType))
            srTransPara->ulOrgTraceNum = srTransPara->ulTraceNum;
    }
    
    srTransPara->byContinueTrans = CN_FALSE;
    do
    {
        if (CN_FALSE == srTransPara->byOffline)
        {
            if(srTransPara->byTransType == SETTLE)
            {
                vdCTOS_SetDateTime();
                memcpy(srTransPara->szDate, srTransRec.szDate, DATE_BCD_SIZE);
                memcpy(srTransPara->szTime, srTransRec.szTime, TIME_BCD_SIZE);
            }
			
            if ((inResult = inBuildOnlineMsg(srTransPara)) != ST_SUCCESS)
            {
            	if (ST_RESP_MATCH_ERR == inResult)
            	{
            		vdDebug_LogPrintf("inBuildOnlineMsg %d",inResult);
                    return inResult;
            	}
                if ((srTransPara->byEntryMode == CARD_ENTRY_ICC) &&
                    (srTransPara->byTransType == SALE || srTransPara->byTransType == PRE_AUTH))
                { 
                    if((inResult == ST_SEND_DATA_ERR) ||
                       (inResult == ST_UNPACK_DATA_ERR))
                    {
                        srTransPara->shTransResult = TRANS_COMM_ERROR;
                    }
                    vdDebug_LogPrintf("CARD_ENTRY_ICC Flow");
                } 
                else
                {
                    vdDebug_LogPrintf("inBuildOnlineMsg %d",inResult);
                    return inResult;
                }
            }
        }
        else
        {
            /* If the transaction is completed, 
               offline transaction do not need to analyze any information*/
            if (inProcessOfflineTrans(srTransPara) != ST_SUCCESS)
            {
                vdDebug_LogPrintf("inProcessOfflineTrans Err");
                inCTOS_inDisconnect();
                return ST_ERROR;
            }
            else
                break;
        }


        if (inAnalyseIsoData(srTransPara) != ST_SUCCESS)
        {
            vdDebug_LogPrintf("inAnalyseIsoData Err byTransType[%d]shTransResult[%d]srTransPara->szRespCode[%s]", srTransPara->byTransType, srTransRec.shTransResult, srTransPara->szRespCode);

            if((srTransPara->byTransType == SETTLE) && (!memcmp(srTransPara->szRespCode,"95",2)))
            {
                inResult=inPorcessTransUpLoad(srTransPara); 				
                if(inResult != ST_SUCCESS)
                {
						srTransRec.shTransResult = TRANS_COMM_ERROR;
						inCTOS_inDisconnect();

						/* BDO: Display the correct error message if batch upload error or settlement recon err -- jzg */
						if(inResult == ST_CLS_BATCH_ERR)
							vdSetErrorMessage("SETTLE FAILED");
						else
							vdSetErrorMessage("BATCH UPLOAD ERROR");
						
						return ST_ERROR;
                }
            }else
            {
                if((strHDT.fReversalEnable == CN_TRUE) && (srTransPara->byTransType != SETTLE) && (srTransRec.shTransResult == TRANS_REJECTED || srTransRec.shTransResult == TRANS_CALL_BANK))
                {
                    inMyFile_ReversalDelete();
                }
                
                inCTOS_inDisconnect();
                
                if((inGetErrorMessage(szErrorMessage) > 0) || (srTransRec.shTransResult == TRANS_REJECTED))
                {
                    vdDebug_LogPrintf("2nd AC failed or Host reject");
                }
                //else
				//    vdSetErrorMessage("Analyse Iso Error");
                
                return ST_ERROR;
            }

            
        }
        else
        {
        	
			vdCTOS_SyncHostDateTime();        	
            if((strHDT.fReversalEnable == CN_TRUE) && (srTransPara->byTransType != SETTLE))
            {
                inMyFile_ReversalDelete();
            }
        }
    
    } while (srTransPara->byContinueTrans);

	DebugAddSTR("inBuildAndSendIsoData","end",20);

    return ST_SUCCESS;
    
}

int inProcessLogon(void)
{
    int inResult,i;
    char szErrorMessage[30+1];
    char szBcd[INVOICE_BCD_SIZE+1];
    TRANS_DATA_TABLE *srTransPara;
    TRANS_DATA_TABLE srTransParaTmp;
	
    srTransRec.ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
    if ((VOID != srTransRec.byTransType) && (SALE_TIP != srTransRec.byTransType))
        srTransRec.ulOrgTraceNum = srTransRec.ulTraceNum;
    inHDTSave(strHDT.inHostIndex);

    srTransPara = &srTransRec;

	if (2 == strTCT.byTerminalType)
		setLCDPrint(V3_ERROR_LINE_ROW, DISPLAY_POSITION_LEFT, "Processing...        ");
	else	
    	setLCDPrint(8, DISPLAY_POSITION_LEFT, "Processing...        ");  
	
	srTransPara->byOffline = CN_FALSE;
	
    if (srCommFuncPoint.inConnect(&srTransRec) != ST_SUCCESS)
    {
        if(srTransPara->shTransResult == 0)
            srTransPara->shTransResult = TRANS_COMM_ERROR;
        inCTOS_inDisconnect();
        vdSetErrorMessage("CONNECT FAILED");
        return ST_ERROR;
    }	

    vdIncSTAN(srTransPara);
	
	if ((inResult = inBuildOnlineMsg(srTransPara)) != ST_SUCCESS)
	{
		inTCTSave(1);
		if (ST_RESP_MATCH_ERR == inResult)
		{
			vdDebug_LogPrintf("inBuildOnlineMsg %d",inResult);
			return inResult;
		}
		memset(strTCT.szLogonDate, 0x00, sizeof(strTCT.szLogonDate));
		return inResult;
	}
    else
    {
        if (2 == strTCT.byTerminalType)
            setLCDPrint(V3_ERROR_LINE_ROW, DISPLAY_POSITION_LEFT, "LOGON SUCCESSFUL");
        else	
            setLCDPrint(8, DISPLAY_POSITION_LEFT, "LOGON SUCCESSFUL");  
		CTOS_Delay(1000);
    }
	
    return ST_SUCCESS;
}

void vdDecSTAN(TRANS_DATA_TABLE *srTransPara)
{
    char szStr[46 + 1];
	
    memset(szStr, 0, sizeof(szStr));
    srTransPara->ulTraceNum--;
    sprintf(szStr, "%06ld", srTransPara->ulTraceNum);
    inAscii2Bcd(szStr, strHDT.szTraceNo, 3);
    inHDTSave(srTransPara->HDTid);
}	

void vdIncSTAN(TRANS_DATA_TABLE *srTransPara)
{
    char szStr[46 + 1];
	
    memset(szStr, 0, sizeof(szStr));
    srTransPara->ulTraceNum++;
    sprintf(szStr, "%06ld", srTransPara->ulTraceNum);
    inAscii2Bcd(szStr, strHDT.szTraceNo, 3);
    inHDTSave(srTransPara->HDTid);
}	

/************************************************************************
Function Name: inSnedReversalToHost()
Description:
    Send Reversal Data To Host
Parameters:
    [IN] srTransPara
         inTransCode

Return: ST_SUCCESS  
        ST_ERROR
        TRANS_COMM_ERROR
        ST_SEND_DATA_ERR
        ST_UNPACK_DATA_ERR
************************************************************************/
int inSnedReversalToHost(TRANS_DATA_TABLE *srTransPara, int inTransCode)
{
    int inResult;
    int inSendLen, inReceLen;
    BYTE uszSendData[ISO_SEND_SIZE + 1], uszReceData[ISO_REC_SIZE + 1];
    CHAR szFileName[d_BUFF_SIZE];
    
    memset(szFileName,0,sizeof(szFileName));
    sprintf(szFileName, "%s%02d%02d.rev"
                        , strHDT.szHostLabel
                        , strHDT.inHostIndex
                        , srTransRec.MITid);
    
	DebugAddSTR("inSnedReversalToHost",szFileName,12);  
    
    vdDebug_LogPrintf("Rever Name %s",szFileName);
    
    if((inResult = inMyFile_CheckFileExist(szFileName)) < 0)
    {
        vdDebug_LogPrintf("inMyFile_CheckFileExist <0");
        return ST_SUCCESS;
    }
    
    inSendLen = inResult;
    
    vdDebug_LogPrintf("inMyFile_ReversalRead(%d)",inResult);
    if((inResult = inMyFile_ReversalRead(&uszSendData[0],sizeof(uszSendData))) == ST_SUCCESS)
    {        
        DebugAddHEX("Reversal orig", uszSendData, inSendLen);
        inCTOSS_ISOEngCheckEncrypt(srTransPara->HDTid, uszSendData, &inSendLen);
        DebugAddHEX("Reversal Encrypt", uszSendData, inSendLen);
		
		/*
        if((strTCT.fPrintISOMessage == VS_TRUE) && (strTCT.fDemo == VS_FALSE))
        {
            //inPrintISOPacket(VS_TRUE , uszSendData, inSendLen);
            vdMyEZLib_HexDump("Send ISO Packet", uszSendData, inSendLen);
            vdPrintParseISO(uszSendData+5);
        }
        */
        
        if ((inReceLen = inSendAndReceiveFormComm(srTransPara,
                             (unsigned char *)uszSendData,
                             inSendLen,
                             (unsigned char *)uszReceData)) <= ST_SUCCESS)
        {
            vdDebug_LogPrintf("inSnedReversalToHost Send Err");
            srTransRec.shTransResult = TRANS_COMM_ERROR;
            return ST_SEND_DATA_ERR;
        }

		/*
        if ((strTCT.fPrintISOMessage == VS_TRUE) && (strTCT.fDemo == VS_FALSE))
        {
            //inPrintISOPacket(VS_TRUE , uszReceData, inSendLen);
            vdMyEZLib_HexDump("Receive ISO Packet", uszReceData, inReceLen);
            vdPrintParseISO(uszReceData+5);
        }
        */
        
        vdSetISOEngTransDataAddress(srTransPara);
        inResult = inCTOSS_UnPackIsodata(srTransPara->HDTid,
                                     (unsigned char *)uszSendData,
                                     inSendLen,
                                     (unsigned char *)uszReceData,
                                     inReceLen);
        if (inResult != ST_SUCCESS)
        {
            vdDebug_LogPrintf("inSnedReversalToHost inCTOSS_UnPackIsodata Err");
            return ST_UNPACK_DATA_ERR;
        }else
        {   
            if (memcmp(srTransPara->szRespCode, "00", 2))
    		{
    		    vdDebug_LogPrintf(". inSnedReversalToHost Resp Err %02x%02x",srTransPara->szRespCode[0],srTransPara->szRespCode[1]);
                inCTOS_inDisconnect();

                return ST_ERROR;
    		}
    		else
    		{
                inResult = CTOS_FileDelete(szFileName);
                if (inResult != d_OK)
    			{
    			    vdDebug_LogPrintf(". inSnedReversalToHost %04x",inResult);
                    inCTOS_inDisconnect();
                    return ST_ERROR;
    			} 
                else
                    vdDebug_LogPrintf("rev. file deelted succesfully after send rev to host");
                
    		}
        }
    }
    
    vdDebug_LogPrintf("**inSnedReversalToHost END**");
    
    return ST_SUCCESS;
}

/************************************************************************
Function Name: inSaveReversalFile()
Description:
    Save Reversal Data into file
Parameters:
    [IN] srTransPara
         inTransCode

Return: ST_SUCCESS  
        ST_ERROR
        ST_BUILD_DATD_ERR
************************************************************************/
int inSaveReversalFile(TRANS_DATA_TABLE *srTransPara, int inTransCode)
{
    int inResult = ST_SUCCESS;
    int inSendLen, inReceLen;
    unsigned char uszSendData[ISO_SEND_SIZE + 1], uszReceData[ISO_REC_SIZE + 1];

    
    vdDebug_LogPrintf("**inSaveReversalFile START TxnType[%d]Orig[%d]**", srTransPara->byTransType, srTransPara->byOrgTransType);

    if(REFUND == srTransPara->byTransType)
        srTransPara->byPackType = REFUND_REVERSAL;
    else if(VOID == srTransPara->byTransType && REFUND == srTransPara->byOrgTransType)
        srTransPara->byPackType = VOIDREFUND_REVERSAL;
    else if(VOID == srTransPara->byTransType)
        srTransPara->byPackType = VOID_REVERSAL;        
    else if(PRE_AUTH == srTransPara->byTransType)
        srTransPara->byPackType = PREAUTH_REVERSAL;
	else if(srTransPara->byTransType == LOY_REDEEM_5050 || srTransPara->byTransType == LOY_REDEEM_VARIABLE)
        srTransPara->byPackType = LOY_REDEEM_REVERSAL;		
    else
        srTransPara->byPackType = REVERSAL;

    vdDebug_LogPrintf(" byPackType(%d)",srTransPara->byPackType);   

    vdSetISOEngTransDataAddress(srTransPara);
	if ((inSendLen = inCTOSS_PackIsoDataNoEncryp(srTransPara->HDTid, strHDT.szTPDU, uszSendData, srTransPara->byPackType)) <= ST_SUCCESS)
    {  
		inCTOS_inDisconnect();
        vdDebug_LogPrintf(" inSave_inSendLen %d",inSendLen);
        vdDebug_LogPrintf("**inSaveReversalFile ST_BUILD_DATD_ERR**");
        srTransPara->byPackType = inTransCode;
        inResult = ST_BUILD_DATD_ERR;
    }
    
    vdDebug_LogPrintf(". inSaveReversalFile Send(%02x)(%02x)(%02x)(%02x)(%02x)(%02x)(%02x)(%02x)(%02x)(%02x)",uszSendData[0],uszSendData[1],uszSendData[2],uszSendData[3],uszSendData[4],uszSendData[5],uszSendData[6],uszSendData[7],uszSendData[8],uszSendData[9]);

    vdDebug_LogPrintf(". inSaveReversalFile Send Len(%d)",inSendLen);

    if((inResult = inMyFile_ReversalSave(&uszSendData[0], inSendLen)) != ST_SUCCESS)
    {
        vdDebug_LogPrintf(". inSave_inMyFile_ReversalSave(%04x)",inResult);
        inCTOS_inDisconnect();
        inResult = ST_ERROR;
    }
    
    srTransPara->byPackType = inTransCode;
    
    return inResult;
}


/************************************************************************
Function Name: inProcessReversal()
Description:
    Processing Reversal the transaction flow
Parameters:
    [IN] srTransPara

Return: ST_SUCCESS  
        ST_ERROR
************************************************************************/

int inProcessReversal(TRANS_DATA_TABLE *srTransPara)
{    
    int inResult,inTransCode;
    
    /*for TLE field 57*/
    byField_02_ON = 0;
    byField_14_ON = 0;
    byField_35_ON = 0;
    byField_45_ON = 0;
    byField_48_ON = 0;
    
    vdDebug_LogPrintf("**inProcessReversal START**");
    
    inResult = ST_SUCCESS;    
    inTransCode = srTransPara->byTransType;    
    srTransPara->byPackType = REVERSAL;
    vdDebug_LogPrintf(". transtype %d",srTransPara->byTransType);
    vdDebug_LogPrintf(". pack type %d",srTransPara->byPackType);

    /* Check reversal flag, If flag is true then open reversal file and send data to host */    
    if ((inResult = inSnedReversalToHost(srTransPara,inTransCode)) != ST_SUCCESS)
    {
        vdDebug_LogPrintf(". Process_inSnedReversalToHost %d",inResult);
		if(inExceedMaxTimes_ReversalDelete() != ST_SUCCESS)
		{
			vdDisplayErrorMsg(1, 8, "Delete REV Fail...");
		}
    }
    
    if(inResult == ST_SUCCESS)
    {
        inMyFile_ReversalDelete();
        /*albert - start - fix stan issue*/        		
		vdIncSTAN(srTransPara);
        /*albert - end - fix stan issue*/
 		
        /* Check reversal flag, If flag is true then create reversal file */
        if ((strHDT.fReversalEnable == CN_TRUE) && (srTransRec.byTransType != SETTLE) && (srTransRec.byTransType != SIGN_ON)
			/*&& (srTransRec.byTransType != PRE_AUTH)*/ && (srTransRec.byTransType != BAL_INQ) && (srTransRec.byTransType != LOY_BAL_INQ)
			&& (srTransRec.byTransType != CARD_VERIFY))
        {
            vdDebug_LogPrintf("inSaveReversalFile START");
            if ((inResult = inSaveReversalFile(srTransPara,inTransCode)) != ST_SUCCESS)
            {
                vdDebug_LogPrintf(". Process_inSaveReversalFile %04x",inResult);
            }
        }
    }
    
    srTransPara->byTransType = inTransCode; 
    
    vdDebug_LogPrintf("**inProcessReversal TYPE(%d) Rest(%d)END**",srTransPara->byTransType,
                                                                     inResult);
    
    return inResult;
}

int inProcessAdviceTrans(TRANS_DATA_TABLE *srTransPara, int inAdvCnt)
{
    int inResult,inUpDateAdviceIndex;
    int inCnt;
    TRANS_DATA_TABLE srAdvTransTable;
    ISO_FUNC_TABLE srAdviceFunc;
    STRUCT_ADVICE strAdvice;

	//add to do not do any advice for Y1, advice will be sent on the next online transaction
	if(stRCDataAnalyze.usTransResult == d_EMV_CHIP_OFF_APPROVAL) 
	    return d_OK;

    memset((char *)&srAdvTransTable, 0, sizeof(TRANS_DATA_TABLE));
    memset((char *)&strAdvice, 0, sizeof(strAdvice));
    
    memcpy((char *)&srAdvTransTable, (char *)srTransPara, sizeof(TRANS_DATA_TABLE));
    memcpy((char *)&srAdviceFunc, (char *)&srIsoFuncTable[0], sizeof(ISO_FUNC_TABLE));
    
    inResult = ST_SUCCESS;
        
    vdDebug_LogPrintf("inProcessAdviceTrans byPackType(%d)byTransType(%d)", srAdvTransTable.byPackType, strAdvice.byTransType);
    while(1)
    {
        inResult = inMyFile_AdviceRead(&inUpDateAdviceIndex,&strAdvice,&srAdvTransTable);
        
        vdDebug_LogPrintf("ADVICE inUpDateAdviceIndex[%d] inMyFile_AdviceRead Rult(%d)(%d)(%d)(%d)", inUpDateAdviceIndex, inResult, srAdvTransTable.ulTraceNum, srAdvTransTable.byPackType, strAdvice.byTransType);
        
        if(inResult == ST_ERROR || inResult == RC_FILE_READ_OUT_NO_DATA)
        {
            inResult = ST_SUCCESS;
            break;
        }
        
        if(inResult == ST_SUCCESS)
        {
            vdDebug_LogPrintf("srTransPara->HDTid = [%d] CVV2[%s]Tip[%02X%02X%02X%02X%02X%02X]", srTransPara->HDTid,srAdvTransTable.szCVV2, srAdvTransTable.szTipAmount[0], srAdvTransTable.szTipAmount[1], srAdvTransTable.szTipAmount[2], srAdvTransTable.szTipAmount[3], srAdvTransTable.szTipAmount[4], srAdvTransTable.szTipAmount[5]);
            //advice need add traceNum
            //srAdvTransTable.ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
            //inMyFile_HDTTraceNoAdd(srTransPara->HDTid);
            //inHDTRead(srTransPara->HDTid);
            
            /*albert - start - fix stan issue*/
            srAdvTransTable.ulTraceNum=srAdvTransTable.ulOrgTraceNum=srTransPara->ulTraceNum;
			/*albert - end - fix stan issue*/
			
            inResult = inPackSendAndUnPackData(&srAdvTransTable, strAdvice.byTransType);

            /*albert - start - fix stan issue*/
            vdIncSTAN(srTransPara);
            srAdvTransTable.ulTraceNum=srTransPara->ulTraceNum;
            /*albert - end - fix stan issue*/
			
            vdDebug_LogPrintf(". inProcessAdviceTrans Rult(%d)srAdvTransTable.byTCFailUpCnt[%d]", inResult, srAdvTransTable.byTCFailUpCnt);
            if (memcmp(srAdvTransTable.szRespCode, "00", 2))
                inResult = ST_ERROR;
                
            if ((inResult == ST_SUCCESS))
            {
                if ((srAdviceFunc.inAdviceAnalyse != 0x00))
                {
                    vdSetISOEngTransDataAddress(&srAdvTransTable);
                    inResult = srAdviceFunc.inAdviceAnalyse(CN_FALSE);
                }
                
                if (inResult == ST_SUCCESS)
                {
                    vdDebug_LogPrintf(". inAdviceAnalyse Rult(%d)", inResult);

                    srAdvTransTable.byUploaded = CN_TRUE;

                    //Should be Online void the Intial SALE amount.
                    //use szStoreID to store how much amount fill up in DE4 for VOID
                    memcpy(srAdvTransTable.szStoreID, srAdvTransTable.szTotalAmount, 6);  
                    
                    if((inResult = inMyFile_BatchSave(&srAdvTransTable,DF_BATCH_UPDATE)) == ST_SUCCESS)
                    {
                        inResult = inMyFile_AdviceUpdate(inUpDateAdviceIndex);
                        vdDebug_LogPrintf(". inProcessAdviceTrans Update Rult(%d)**", inResult);
                    }

                    if (inResult != ST_SUCCESS)
                    {
                        vdDebug_LogPrintf(". inProcessAdviceTrans Err(%d)**", inResult);
                        break;
                    }
                }
            }
        }
        
        if(inResult != ST_SUCCESS)
        {
            if(srTransPara->byTransType == SETTLE)
            {
                srTransRec.shTransResult = TRANS_COMM_ERROR;
                inCTOS_inDisconnect();
                return ST_ERROR;
            }
            else
                return ST_SUCCESS;
        }

        
        if(inAdvCnt != -1)
        {
            inAdvCnt --;
            if(inAdvCnt == 0)
                break;
        }
    }
    
    vdDebug_LogPrintf("**inProcessAdviceTrans(%d) END**", inResult);
	return (inResult);
}

int inProcessAdviceTCTrail(TRANS_DATA_TABLE *srTransPara)
{
	int inRet=d_NO, inUpDateAdviceIndex;
	TRANS_DATA_TABLE srAdvTransTable;
	STRUCT_ADVICE strAdvice;

	inCTLOS_Updatepowrfail(PFR_IDLE_STATE);

    inUpDateAdviceIndex=0;
    memset((char *)&srAdvTransTable, 0, sizeof(TRANS_DATA_TABLE));
    memset((char *)&strAdvice, 0, sizeof(strAdvice));	
	
    if(srTransPara->byOffline == CN_FALSE)
    {
        if(inMyFile_AdviceRead(&inUpDateAdviceIndex,&strAdvice,&srAdvTransTable) == d_OK)
            vdIncSTAN(srTransPara);

	    //CTOS_LCDTClearDisplay();
	    
        inRet=inProcessAdviceTrans(srTransPara, strHDT.inNumAdv);
        if(d_OK != inRet)
            return inRet;
        else
            vdSetErrorMessage("");
    }	

    inUpDateAdviceIndex=0;
    memset((char *)&srAdvTransTable, 0, sizeof(TRANS_DATA_TABLE));
    memset((char *)&strAdvice, 0, sizeof(strAdvice));	

    if( CARD_ENTRY_ICC == srTransPara->byEntryMode)
    {
        if(inMyFile_TCUploadFileRead(&inUpDateAdviceIndex,&strAdvice,&srAdvTransTable) == d_OK)
            vdIncSTAN(srTransPara);

	    //CTOS_LCDTClearDisplay();
		        
        inRet = inProcessEMVTCUpload(srTransPara, -1);
        if(d_OK != inRet)
            return inRet;
        else
            vdSetErrorMessage("");
    }
	
	return inRet;
}

int inProcessEMVTCUpload(TRANS_DATA_TABLE *srTransPara, int inAdvCnt)
{
    int inResult,inUpDateAdviceIndex;
    int inCnt;
    TRANS_DATA_TABLE srOrigTransFromBatch;
    TRANS_DATA_TABLE srAdvTransTable;
    ISO_FUNC_TABLE srAdviceFunc;
    STRUCT_ADVICE strAdvice;

    memset((char *)&srAdvTransTable, 0, sizeof(TRANS_DATA_TABLE));
    memset((char *)&strAdvice, 0, sizeof(strAdvice));
    
    memcpy((char *)&srAdvTransTable, (char *)srTransPara, sizeof(TRANS_DATA_TABLE));
    memcpy((char *)&srAdviceFunc, (char *)&srIsoFuncTable[0], sizeof(ISO_FUNC_TABLE));
    
    inResult = ST_SUCCESS;
        
    vdDebug_LogPrintf("inProcessEMVTCUpload byPackType(%d)byTransType(%d)", srAdvTransTable.byPackType, strAdvice.byTransType);
    while(1)
    {
        inResult = inMyFile_TCUploadFileRead(&inUpDateAdviceIndex,&strAdvice,&srAdvTransTable);
        
        if(strAdvice.byTransType == TC_UPLOAD)
            srAdvTransTable.byPackType = TC_UPLOAD;
        
        if(inResult == ST_ERROR || inResult == RC_FILE_READ_OUT_NO_DATA)
        {
            inResult = ST_SUCCESS;
            break;
        }

        memcpy(&srOrigTransFromBatch, &srAdvTransTable, sizeof(TRANS_DATA_TABLE));
        if(inResult == ST_SUCCESS)
        {
            //vdDebug_LogPrintf("srTransPara->HDTid = [%d] ", srTransPara->HDTid);
            //inMyFile_HDTTraceNoAdd(srTransPara->HDTid);
            //inHDTRead(srTransPara->HDTid);

            /*albert - start - fix stan issue*/
            srAdvTransTable.ulTraceNum=srTransPara->ulTraceNum;
			/*albert - end - fix stan issue*/
            
            inResult = inPackSendAndUnPackData(&srAdvTransTable, strAdvice.byTransType);

            /*albert - start - fix stan issue*/
            vdIncSTAN(srTransPara);
            srAdvTransTable.ulTraceNum=srTransPara->ulTraceNum;
            /*albert - end - fix stan issue*/
			
            vdDebug_LogPrintf(". inProcessEMVTCUpload Rult(%d)srAdvTransTable.byTCFailUpCnt[%d]srTransPara->szRespCode[%s]", inResult, srAdvTransTable.byTCFailUpCnt,srAdvTransTable.szRespCode);

  
            if(srAdvTransTable.byTCFailUpCnt >= 2)
            {
              srAdvTransTable.byTCuploaded = CN_TRUE;
              srAdvTransTable.byUploaded = CN_TRUE;
              inResult = inMyFile_TCUploadFileUpdate(inUpDateAdviceIndex);
              vdDebug_LogPrintf(". inProcessEMVTCUpload Update Rult(%d)**", inResult);
                      
              if (inResult != ST_SUCCESS)
              {
                  vdDebug_LogPrintf(". inProcessEMVTCUpload Err(%d)**", inResult);
                  break;
              }
            }
            if (inResult == ST_SUCCESS)
            {
                if(memcmp(srAdvTransTable.szRespCode,"00",2) != 0)
                {
                    vdDebug_LogPrintf(". resp not succ(%s)**srTransPara->byPackType[%d]strAdvice.byTransType[%d]", srAdvTransTable.szRespCode,srTransPara->byPackType,strAdvice.byTransType);
                    if( strAdvice.byTransType == TC_UPLOAD )
                    {
                        srOrigTransFromBatch.byTCFailUpCnt++;                    
                        inMyFile_BatchSave(&srOrigTransFromBatch,DF_BATCH_UPDATE);
                        inCTOS_inDisconnect();
                        return ST_ERROR;
                    }
                }
                else
                {
                    srAdvTransTable.byTCuploaded = CN_TRUE;
                    srAdvTransTable.byUploaded = CN_TRUE;
                    inResult = inMyFile_TCUploadFileUpdate(inUpDateAdviceIndex);
                    vdDebug_LogPrintf(". inProcessEMVTCUpload Update Rult(%d)**", inResult);
                          
                    if (inResult != ST_SUCCESS)
                    {
                      vdDebug_LogPrintf(". inProcessEMVTCUpload Err(%d)**", inResult);
                      break;
                    }
                }    
            }
            else
            {
                if(strAdvice.byTransType == TC_UPLOAD)
                {
                    srOrigTransFromBatch.byTCFailUpCnt++; 
                    inMyFile_BatchSave(&srOrigTransFromBatch,DF_BATCH_UPDATE);
                }
            }
            
        }
        
        if(inResult != ST_SUCCESS)
        {
            if(srTransPara->byTransType == SETTLE)
            {
                srTransRec.shTransResult = TRANS_COMM_ERROR;
                inCTOS_inDisconnect();
                return ST_ERROR;
            }
            else
                return ST_SUCCESS;
        }

        
        if(inAdvCnt != -1)
        {
            inAdvCnt --;
            if(inAdvCnt == 0)
                break;
        }
    }
    
    vdDebug_LogPrintf("**inProcessEMVTCUpload(%d) END**", inResult);
	return (inResult);
}


/************************************************************************
Function Name: inPackSendAndUnPackData()
Description:
    Composed upload host information.
    Analysis of host return message.
Parameters:
    [IN] srTransPara
         inTransCode
Return: ST_SUCCESS  
        ST_ERROR
        ST_BUILD_DATD_ERR
        ST_SEND_DATA_ERR
        ST_UNPACK_DATA_ERR
************************************************************************/

int inPackSendAndUnPackData(TRANS_DATA_TABLE *srTransPara, int inTransCode)
{
    int inResult;
    int inSendLen, inReceLen;
    unsigned char uszSendData[ISO_SEND_SIZE + 1], uszReceData[ISO_REC_SIZE + 1];
    
    vdDebug_LogPrintf("**inPackSendAndUnPackData START**");
    
    memset(uszSendData, 0x00, sizeof(uszSendData));
    memset(uszReceData, 0x00, sizeof(uszReceData));

    inResult = ST_SUCCESS;

    vdSetISOEngTransDataAddress(srTransPara);
	if ((inSendLen = inCTOSS_PackIsoData(srTransPara->HDTid, strHDT.szTPDU, uszSendData, inTransCode)) <= ST_SUCCESS)
    {
		inCTOS_inDisconnect();    
        vdDebug_LogPrintf(" inSendLen %d",inSendLen);
        vdDebug_LogPrintf("**inPackSendAndUnPackData ST_BUILD_DATD_ERR**");
        return ST_BUILD_DATD_ERR;
    }

	inResult = inCTOSS_CheckBitmapSetBit(5);
    vdDebug_LogPrintf("inCTOSS_CheckSetBit 5 [%ld]", inResult);
	inResult = inCTOSS_CheckBitmapSetBit(11);
    vdDebug_LogPrintf("inCTOSS_CheckSetBit 11 [%ld]", inResult);
	
    vdDebug_LogPrintf(". inPackData Send Len(%d)",inSendLen);
    
    if ((inReceLen = inSendAndReceiveFormComm(srTransPara,
                         (unsigned char *)uszSendData,
                         inSendLen,
                         (unsigned char *)uszReceData)) <= 0)
    {
        vdDebug_LogPrintf(". inPackData ST_SEND_DATA_ERR");
        return ST_SEND_DATA_ERR;
    }
    
    vdDebug_LogPrintf(". inPackData Rec Len(%d)",inReceLen);

    vdSetISOEngTransDataAddress(srTransPara);
    inResult = inCTOSS_UnPackIsodata(srTransPara->HDTid,
                                 (unsigned char *)uszSendData,
                                 inSendLen,
                                 (unsigned char *)uszReceData,
                                 inReceLen);
    vdDebug_LogPrintf("**inPackSendAndUnPackData inResult=[%d]srTransPara->szRespCode[%s]",inResult,srTransPara->szRespCode);

    if (inResult != ST_SUCCESS)
    {
        vdSetErrorMessage("INVALID RESPONSE");
        inResult = ST_UNPACK_DATA_ERR;
		inCTOS_inDisconnect();
    }

    vdDebug_LogPrintf("**srTransPara->byPackType: %d", srTransPara->byPackType);
    vdDebug_LogPrintf("**srTransPara->byTransType: %d", srTransPara->byTransType);

    if(srTransPara->byPackType == LOG_ON)
    {
        if(inCTOSS_CheckBitmapSetBit(53) == 0)
        {
            vdSetErrorMessage("LOGON FAILED");
            inResult = ST_UNPACK_DATA_ERR;
            inCTOS_inDisconnect();
        }
    }
    else if((srTransPara->byPackType == SALE) && (srTransPara->fInstallment == TRUE))
    {
        if(inCTOSS_CheckBitmapSetBit(61) == 0)
        {
            vdSetErrorMessage("INVALID RESPONSE");
            inResult = ST_UNPACK_DATA_ERR;
            inCTOS_inDisconnect();
        }
    }

    if(strTCT.fDemo == TRUE)
        inResult=ST_SUCCESS;
        
    vdDebug_LogPrintf("**inPackSendAndUnPackData END**");
    return inResult;
}





/************************************************************************
Function Name: inBuildOnlineMsg()
Description:
   To handle online messages and check the response code and authorization code
Parameters:
    [IN] srTransPara

Return: ST_SUCCESS  
        ST_ERROR
        ST_UNPACK_DATA_ERR
        ST_SEND_DATA_ERR
************************************************************************/
int inBuildOnlineMsg(TRANS_DATA_TABLE *srTransPara)
{
    int inResult;
	TRANS_DATA_TABLE srTransParaTmp;
    
    vdDebug_LogPrintf("**inBuildOnlineMsg START TxnType[%d]Orig[%d]Uploaded[%d]**", srTransPara->byTransType, srTransPara->byOrgTransType, srTransPara->byUploaded);

    if(VOID == srTransPara->byTransType && REFUND == srTransPara->byOrgTransType)
        srTransPara->byPackType = VOID_REFUND;	
    else
        srTransPara->byPackType = srTransPara->byTransType;

	memset(&srTransParaTmp, 0x00, sizeof(TRANS_DATA_TABLE));
	memcpy(&srTransParaTmp, srTransPara, sizeof(TRANS_DATA_TABLE));
    vdDebug_LogPrintf(" byPackType(%d)",srTransPara->byPackType);    

    inResult = inPackSendAndUnPackData(srTransPara, srTransPara->byPackType);
    
    if (inResult == ST_BUILD_DATD_ERR)
    {  
        vdDebug_LogPrintf("**inBuildOnlineMsg TRANS_COMM_ERROR**");
        srTransPara->shTransResult = TRANS_COMM_ERROR;
        vdSetErrorMessage("BUILD DATA ERR");
        inCTOS_inDisconnect();
        return ST_ERROR;
    }
    else if (inResult == ST_SEND_DATA_ERR || inResult == ST_UNPACK_DATA_ERR)
    {
        srTransRec.shTransResult = TRANS_COMM_ERROR;
        vdDebug_LogPrintf("**inBuildOnlineMsg ST_SEND_DATA_ERR**");
        return inResult;
    }else
    {
        srTransPara->shTransResult = inCheckHostRespCode(srTransPara);
        vdDebug_LogPrintf(". shTransResult %d",srTransPara->shTransResult);
        
        if (srTransPara->shTransResult == TRANS_AUTHORIZED)
        {
            if (ST_SUCCESS != inBaseRespValidation(&srTransParaTmp,srTransPara))
				return ST_RESP_MATCH_ERR;
        }
        else if (srTransPara->shTransResult == ST_UNPACK_DATA_ERR)
        {
            vdDebug_LogPrintf("**inBuildOnlineMsg shTransResult UNPACK_ERR**");
            srTransPara->shTransResult = TRANS_COMM_ERROR;// for not delete reversal file

            vdSetErrorMessage("RESP ERROR");
            return ST_UNPACK_DATA_ERR;
        }

    }
    
    vdDebug_LogPrintf("**inBuildOnlineMsg END**");
    return ST_SUCCESS;
}

/************************************************************************
Function Name: inSetBitMapCode()
Description:
    Use the transaction code to generate the corresponding bitmap code
Parameters:
    [IN] srTransPara
         srPackFunc
         inTempBitMapCode
Return: ST_SUCCESS  
        inBitMapArrayIndex
************************************************************************/

int inSetBitMapCode(TRANS_DATA_TABLE *srTransPara, int inTransCode)
{
    int    inBitMapIndex = -1;

    inBitMapIndex = inTransCode;
    
    return inBitMapIndex;
}

/************************************************************************
Function Name: inPackMessageIdData()
Description:
    Pack message id data
Parameters:
    [IN] srTransPara
         inTransCode
         uszPackData
Return: inPackLen
************************************************************************/
int inPackMessageIdData(int inTransCode, unsigned char *uszPackData, char *szMTI)
{
    int    inPackLen;
    TRANS_DATA_TABLE *srTransPara;
    
    inPackLen = 0;
    srTransPara = srGetISOEngTransDataAddress();

    wub_str_2_hex(szMTI, (char *)&uszPackData[inPackLen], MTI_ASC_SIZE);

    if(srTransPara->byPackType != BATCH_UPLOAD && srTransPara->byPackType != TC_UPLOAD)
    {   
       wub_str_2_hex(szMTI, (char *)srTransPara->szMassageType, MTI_ASC_SIZE);
    }

    inPackLen += MTI_BCD_SIZE;
    
    return inPackLen;
}

/************************************************************************
Function Name: inPackPCodeData()
Description:
    Pack message id data
Parameters:
    [IN] srTransPara
         inTransCode
         uszPackData
Return: inPackLen
************************************************************************/
int inPackPCodeData(int inTransCode, unsigned char *uszPackData, char *szPCode)
{
    char szTempFile03[10];
    int    inPackLen;
    TRANS_DATA_TABLE *srTransPara;
	
    inPackLen = 0;
    srTransPara = srGetISOEngTransDataAddress();

    
	vdDebug_LogPrintf(". inPackPCodeData [%02X] %s", inTransCode, szPCode);

	if(srTransPara->byPackType == BATCH_UPLOAD)
	{
		memset(szTempFile03,0x00,sizeof(szTempFile03));
		memcpy(szTempFile03,srTransPara->szIsoField03,PRO_CODE_BCD_SIZE);
	}

	wub_str_2_hex(szPCode, srTransPara->szIsoField03, PRO_CODE_ASC_SIZE);

    if (srTransPara->byTransType == SALE || srTransPara->byTransType == BAL_INQ)
    {
        if (srTransPara->inAccountType == SAVINGS_ACCOUNT)
            srTransPara->szIsoField03[1] += 0x10;
        else if (srTransPara->inAccountType == CURRENT_ACCOUNT)
            srTransPara->szIsoField03[1] += 0x20;
    }

	if(srTransPara->byPackType == BATCH_UPLOAD)
	{
		memcpy(srTransPara->szIsoField03,szTempFile03,PRO_CODE_BCD_SIZE);
		
		if(inFinalSend == CN_TRUE)
			srTransPara->szIsoField03[2] = 0x00;
	}

    inPackLen += PRO_CODE_BCD_SIZE;

	vdDebug_LogPrintf(". szPCode %s [%02X%02X%02X]", szPCode, srTransPara->szIsoField03[0], srTransPara->szIsoField03[1], srTransPara->szIsoField03[2]);


	return inPackLen;
}

/************************************************************************
Function Name: vdModifyBitMapFunc()
Description:
    Modify bitmap array
Parameters:
    [IN] srTransPara
         inTransCode
         inBitMap
Return: 
************************************************************************/
void vdModifyBitMapFunc(int inTransCode, int *inBitMap)
{
    BYTE szTipAmount[20];
    TRANS_DATA_TABLE *srTransPara;

    srTransPara = srGetISOEngTransDataAddress();
    
    if(inTransCode == SIGN_ON)
    {
        return;
    }

    vdDebug_LogPrintf(". vdModifyBitMapFunc inTransCode[%d]byEntryMode[%d]byTransType[%d]szCVV2[%s]", inTransCode, srTransPara->byEntryMode, srTransPara->byTransType, srTransPara->szCVV2);
    
    if((inTransCode != BATCH_UPLOAD) && (inTransCode != REVERSAL) && (inTransCode != SETTLE) && (inTransCode != CLS_BATCH))
    {
        if(srTransPara->byEntryMode == CARD_ENTRY_MSR ||
           srTransPara->byEntryMode == CARD_ENTRY_ICC)
        {
            {
                vdMyEZLib_LogPrintf(". usTrack1Len>0");
                vdCTOSS_SetBitMapOff(inBitMap, 45);
            }
        }
        
        if ((srTransPara->byEntryMode == CARD_ENTRY_ICC || 
			(srTransPara->byEntryMode == CARD_ENTRY_WAVE &&
			(srTransPara->bWaveSID == d_VW_SID_VISA_WAVE_2 ||
			 srTransPara->bWaveSID == d_VW_SID_VISA_WAVE_QVSDC ||
			 srTransPara->bWaveSID == d_VW_SID_PAYPASS_MCHIP))) && 
            (  srTransPara->byTransType == SALE
            || srTransPara->byTransType == SALE_OFFLINE
            || srTransPara->byTransType == PRE_AUTH
            /*|| srTransPara->byTransType == REFUND*/
            || srTransPara->byTransType == SALE_TIP
            || srTransPara->byTransType == VOID))
        {

            vdDebug_LogPrintf("Transcation set DE55 T5F34_len[%d]", srTransPara->stEMVinfo.T5F34_len);
            vdCTOSS_SetBitMapOn(inBitMap, 55);
            
            if(srTransPara->stEMVinfo.T5F34_len > 0)
                vdCTOSS_SetBitMapOn(inBitMap, 23); 
        }
    
        if (srTransPara->byEntryMode == CARD_ENTRY_MANUAL)
        {
            vdMyEZLib_LogPrintf(". byEntryMode CN_TRUE");
            vdCTOSS_SetBitMapOn(inBitMap, 2);
            vdCTOSS_SetBitMapOn(inBitMap, 14);

            vdCTOSS_SetBitMapOff(inBitMap, 35);
        }


        memset(szTipAmount, 0x00, sizeof(szTipAmount));
        wub_hex_2_str(srTransPara->szTipAmount, szTipAmount, 6);
        DebugAddSTR("Tip", szTipAmount, 12);
        if(atol(szTipAmount) > 0)
        {
            vdCTOSS_SetBitMapOn(inBitMap, 54);

            //Should be Online void the Intial SALE amount.
            //use szStoreID to store how much amount fill up in DE4 for VOID
            if(srTransPara->byTransType == VOID && 0 == memcmp(srTransPara->szStoreID, srTransPara->szBaseAmount, 6))
            {
                vdCTOSS_SetBitMapOff(inBitMap, 54);
            }
        }

        if(strlen(srTransPara->szCVV2) > 0)
        {
            vdCTOSS_SetBitMapOn(inBitMap, 48);
        }

    }
    else if(inTransCode == BATCH_UPLOAD || inTransCode == REVERSAL || inTransCode == VOID_REVERSAL || inTransCode == PREAUTH_REVERSAL)
    {
        if(srTransPara->byEntryMode == CARD_ENTRY_ICC || 
			(srTransPara->byEntryMode == CARD_ENTRY_WAVE &&
			(srTransPara->bWaveSID == d_VW_SID_VISA_WAVE_2 ||
			 srTransPara->bWaveSID == d_VW_SID_VISA_WAVE_QVSDC ||
			 srTransPara->bWaveSID == d_VW_SID_PAYPASS_MCHIP)))
        {
            vdDebug_LogPrintf(("BATCH_UPLOAD set DE55"));
            vdCTOSS_SetBitMapOn(inBitMap, 55);

            if(srTransPara->stEMVinfo.T5F34_len > 0)
                vdCTOSS_SetBitMapOn(inBitMap, 23); 
        }

        memset(szTipAmount, 0x00, sizeof(szTipAmount));
        wub_hex_2_str(srTransPara->szTipAmount, szTipAmount, 6);
        DebugAddSTR("Tip", szTipAmount, 12);
        if(atol(szTipAmount) > 0)
        {
            vdCTOSS_SetBitMapOn(inBitMap, 54);
        }

        if(strlen(srTransPara->szCVV2) > 0)
        {
            vdCTOSS_SetBitMapOn(inBitMap, 48);
        }
    }
    else if((inTransCode == SETTLE) && (inTransCode == CLS_BATCH))
    {
        vdDebug_LogPrintf(("Settlement modify field"));
    }

    /*albert - start - do not send de55 and de23 on refund*/
    if(srTransPara->byTransType == REFUND || srTransPara->byOrgTransType == REFUND)
    {
       vdCTOSS_SetBitMapOff(inBitMap, 55);
       vdCTOSS_SetBitMapOff(inBitMap, 23);
    }
    /*albert - end - do not send de55 and de23 on refund*/	
}

/************************************************************************
Function Name: inSendAndReceiveFormComm()
Description:
    Call function to send and receive data
Parameters:
    [IN] srTransPara
         uszSendData
         inSendLen
         uszReceData
Return: inResult --> Receive data len;
        ST_ERROR
************************************************************************/

int inSendAndReceiveFormComm(TRANS_DATA_TABLE* srTransPara,
                             unsigned char* uszSendData,
                             int inSendLen,
                             unsigned char* uszReceData)
{
    int inResult;
    BYTE key;
	char uszSendEcnryptedData[1024+1];
    
    vdDebug_LogPrintf("**inSendAndReceiveFormComm START**");
	vdDebug_LogPrintf("fTPSec[%d]", strTCT.fTPSec);
	vdDebug_LogPrintf("inHostIndex[%d] . szHostLabel[%s] . fHostTPSec[%d]", strHDT.inHostIndex, strHDT.szHostLabel, strHDT.fHostTPSec);
	
    //vdMyEZLib_LogPrintff(uszSendData,inSendLen);
    DebugAddHEX("send da",uszSendData,inSendLen);

    if(VS_TRUE == strTCT.fDemo)
    {
        vdDebug_LogPrintf("DEMO Call inSendData!!");
        inResult = d_OK;
    }
    else
    {
    	if(strTCT.fTPSec == TRUE && strHDT.fHostTPSec == TRUE)
		{
		 	if(inCheckTPSecKey() != d_OK)
				return ST_SEND_DATA_ERR;

			memset(uszSendEcnryptedData, 0, sizeof(uszSendEcnryptedData));
		    inSendLen=inEncryptTPSec(uszSendData, inSendLen, &uszSendEcnryptedData);
            DebugAddHEX("uszSendEcnryptedData",uszSendEcnryptedData,inSendLen); 			
			inResult = srCommFuncPoint.inSendData(srTransPara,uszSendEcnryptedData,inSendLen);
		}
		else
        	inResult = srCommFuncPoint.inSendData(srTransPara,uszSendData,inSendLen);
		
		// Parse ISO
		if(inResult == d_OK && strTCT.fPrintISOMessage == VS_TRUE)
			vdPrintISOOption("TX",uszSendData, inSendLen);
		
    }
    
    vdDebug_LogPrintf("**inSendAndReceiveFormComm [%d]",inResult);
    if (inResult != d_OK)
    {
        if( srTransPara->byPackType != TC_UPLOAD )
        {
            //vdSetErrorMessage("SEND ERR");
            vdSetErrorMessage("NO RESPONSE FROM HOST");
        }
        
		inSetTextMode();				
        inCTOS_inDisconnect();
        return ST_ERROR;
    }else
    {
        //vdDisplayAnimateBmp(0,0, "Comms1.bmp", "Comms2.bmp", "Comms3.bmp", "Comms4.bmp", NULL);
        if(VS_TRUE == strTCT.fDemo)
        {
            vdDebug_LogPrintf("DEMO Call inRecData!!");
            inResult = inCTOS_PackDemoResonse(srTransPara,uszReceData);
        }
        else
        {          
            inResult = srCommFuncPoint.inRecData(srTransPara,uszReceData);
        }
        
        if (inResult > 0)
        {   
        	DebugAddHEX("recv data",uszReceData,inResult);
            
			if(strTCT.fTPSec == TRUE && strHDT.fHostTPSec == TRUE)
			{
                if(strCPT.inCommunicationMode == ETHERNET_MODE)
                    inResult-=2;
				inResult=inDecryptTPSec(uszReceData, inResult, uszReceData);
				DebugAddHEX("decrypted recv data",uszReceData,inResult);
			}
			
            // Parse ISO
			if(strTCT.fPrintISOMessage == VS_TRUE){
				if (strCPT.inCommunicationMode == DIAL_UP_MODE)
					vdPrintISOOption("RX",uszReceData, inResult);
				else
					vdPrintISOOption("RX",uszReceData, inResult);
			}
			
        	vdMyEZLib_LogPrintff(uszReceData,inResult);
        }
        else
        {            
            inCTOS_inDisconnect();

            if( srTransPara->byPackType != TC_UPLOAD )
            {
            /*
                CTOS_LCDTClearDisplay();
                if( srTransPara->byPackType == REVERSAL )
                      CTOS_LCDTPrintXY(1,1,"      REVERSAL");      
                else
                    vdDispTransTitle(srTransPara->byTransType);
                CTOS_Sound(1000, 50);
                CTOS_LCDTPrintXY(1,8,"NO RESP.FRM HOST");

                CTOS_KBDHit(&key);//clear key buffer
    		    key=struiGetchWithTimeOut();
    		    */

			if(fGetECRTransactionFlg())
                {
                     vdSetECRResponse(ECR_COMMS_ERR);
                     //inMultiAP_ECRSendSuccessResponse();
                }	
			
    		    //vdSetErrorMessage("NO RESP.FRM HOST");
    		    vdSetErrorMessage("NO RESPONSE FROM HOST");
                return ST_ERROR;
            }
                        
        }
    }
    vdDebug_LogPrintf("**inSendAndReceiveFormComm END**");
//	inSetTextMode(); // patrick temp remark 20140421				
    return inResult;
}

/************************************************************************
Function Name: inCheckIsoHeaderData()
Description:
    Check message id value
Parameters:
    [IN] srTransPara
         szSendISOHeader
         szReceISOHeader

Return: ST_SUCCESS
        ST_ERROR
************************************************************************/
int inCheckIsoHeaderData(char *szSendISOHeader, char *szReceISOHeader)
{
    int    inCnt = 0;
    TRANS_DATA_TABLE *srTransPara;

    srTransPara = srGetISOEngTransDataAddress();

    inCnt += TPDU_BCD_SIZE;

    szSendISOHeader[inCnt + 1] += 0x10;
    if (memcmp(&szSendISOHeader[inCnt], &szReceISOHeader[inCnt], MTI_BCD_SIZE))
    {
        if(VS_TRUE == strTCT.fDemo)
        {
            return ST_SUCCESS;
        }
        
        vdMyEZLib_LogPrintf("**ISO header data Error**");
        inCTOS_inDisconnect();
        return ST_ERROR;
    }

    return ST_SUCCESS;
}


/************************************************************************
Function Name: inProcessOfflineTrans()
Description:
    Setup and save the file offline transactions need
Parameters:
    [IN] srTransPara
         szSendISOHeader
         szReceISOHeader

Return: ST_SUCCESS
        ST_ERROR
************************************************************************/
int inProcessOfflineTrans(TRANS_DATA_TABLE *srTransPara)
{
    int inResult;

    vdDebug_LogPrintf("**inProcessOfflineTrans START TxnType[%d]Orig[%d]Uploaded[%d]**", srTransPara->byTransType, srTransPara->byOrgTransType, srTransPara->byUploaded);
	
    if(srTransPara->byTransType == SALE_TIP && srTransPara->byOrgTransType == SALE_OFFLINE
    && srTransPara->byUploaded == FALSE)
        srTransPara->byPackType = SALE_OFFLINE;
	else
	    srTransPara->byPackType = srTransPara->byTransType;
	
    if(!memcmp(srTransRec.szAuthCode, "Y1", 2))// for save trans as Y1 TC UPLOAD format
        srTransRec.byTransType = SEND_ADVICE; //put to change 320 Msg type to 0220
    return ST_SUCCESS;
}    



/************************************************************************
Function Name: inAnalyseIsoData()
Description:
    Analysis of the host to send back information
Parameters:
    [IN] srTransPara
         
Return: ST_SUCCESS
        ST_ERROR
************************************************************************/
int inAnalyseIsoData(TRANS_DATA_TABLE *srTransPara)
{
    int inResult;
    ISO_FUNC_TABLE srPackFunc;
        
    inResult = ST_SUCCESS;
    
    if (srTransPara->byTransType == SALE &&
        srTransPara->byEntryMode == CARD_ENTRY_ICC &&
        srTransPara->shTransResult == TRANS_AUTHORIZED &&
        !memcmp(&srTransPara->szAuthCode[0], "Y1", 2))
    {
        return ST_SUCCESS;
    }

    if (srTransPara->byOffline == CN_TRUE)
    {
        return inResult;
    }

    memset((char *)&srPackFunc, 0x00, sizeof(srPackFunc));
    memcpy((char *)&srPackFunc, (char *)&srIsoFuncTable[0], sizeof(srPackFunc));

    if (srPackFunc.inTransAnalyse != 0x00)
    {
        vdSetISOEngTransDataAddress(srTransPara);
        inResult = srPackFunc.inTransAnalyse();
    }
	vdDebug_LogPrintf("inAnalyseIsoData[%d]", inResult);

	if(VS_TRUE == strTCT.fDemo)
		CTOS_LCDTPrintXY(1, 8, "APPROVE        ");

    return inResult;
}

/************************************************************************
Function Name: inCheckHostRespCode()
Description:
    Check the host response code
Parameters:
    [IN] srTransPara
         
Return: TRANS_AUTHORIZED
        TRANS_COMM_ERROR
        TRANS_AUTHORIZED
        TRANS_CALL_BANK
        TRANS_CANCELLED
        ST_UNPACK_DATA_ERR
************************************************************************/
int inCheckHostRespCode(TRANS_DATA_TABLE *srTransPara)
{
    int    inResult = TRANS_COMM_ERROR;
    
    vdDebug_LogPrintf("inCheckHostRespCode %s",srTransPara->szRespCode);
    if (!memcmp(srTransPara->szRespCode, "00", 2)
    || !memcmp(srTransPara->szRespCode, "08", 2)
    || !memcmp(srTransPara->szRespCode, "10", 2)
    || !memcmp(srTransPara->szRespCode, "11", 2)
    || !memcmp(srTransPara->szRespCode, "16", 2))
    {
        inResult = TRANS_AUTHORIZED;
		srTransPara->shTransResult = TRANS_AUTHORIZED;
		strcpy(srTransPara->szRespCode, "00");
		DebugAddSTR("txn approval",srTransPara->szAuthCode ,6);  
    }
    else
    {
        if((srTransPara->szRespCode[0] >= '0' && srTransPara->szRespCode[0] <= '9') &&
           (srTransPara->szRespCode[1] >= '0' && srTransPara->szRespCode[1] <= '9'))
        {
            inResult = TRANS_REJECTED;
            if((srTransRec.byTransType == SETTLE) && (memcmp(srTransPara->szRespCode,"95",2)))
            {
                //vdDispErrMsg("SETTLE FAILED");
                vdDisplayErrorMsg(1, 8, "SETTLE FAILED");
            }
            else
            {
                //if(memcmp(srTransPara->szRespCode,"95",2))
                    inCTOS_DisplayResponse();
                
            }
                
        }
        else
            inResult = ST_UNPACK_DATA_ERR;
    }

    return (inResult);
}

int inBaseRespValidation(TRANS_DATA_TABLE *srOrgTransPara,TRANS_DATA_TABLE *srTransPara)
{
	vdDebug_LogPrintf("inBaseRespValidation ulTraceNum=[%ld][%ld]",srOrgTransPara->ulTraceNum,srTransPara->ulTraceNum);
	if (srOrgTransPara->ulTraceNum != srTransPara->ulTraceNum)
	{
		vdSetErrorMessage("TID Not Match");
		return ST_RESP_MATCH_ERR;
	}

	vdDebug_LogPrintf("inBaseRespValidation szTID=[%s][%s]",srOrgTransPara->szTID,srTransPara->szTID);
	if (memcmp(srOrgTransPara->szTID,srTransPara->szTID,TERMINAL_ID_BYTES) != 0)
	{
		vdSetErrorMessage("STAN Not Match");
		return ST_RESP_MATCH_ERR;
	}

	return ST_SUCCESS;
}

/************************************************************************
Function Name: inCheckTransAuthCode()
Description:
    Check the host authorization code
Parameters:
    [IN] srTransPara
         
Return: ST_SUCCESS
        ST_ERROR
      
************************************************************************/

int inCheckTransAuthCode(TRANS_DATA_TABLE *srTransPara)
{
    int inResult = ST_SUCCESS;

    if(srTransPara->byTransType != SETTLE && srTransPara->byTransType != CLS_BATCH)
    {
        if (!memcmp(&srTransPara->szAuthCode[0], "000000", 6) || 
            !memcmp(&srTransPara->szAuthCode[0], "      ", 6))
        {
            if(srTransPara->byTransType != VOID) //Synergy host does not return Auth.code for void sale
                inResult = ST_ERROR;
        }
    }

    return (inResult);
}


int inAnalyseChipData(TRANS_DATA_TABLE *srTransPara)
{
    int	inResult;
    USHORT inlen=0;
    unsigned char stScript[512];
    
//    vduiClearBelow(8); // patrick fix code 20140421
    
	memset(stScript,0,sizeof(stScript));

    vdDebug_LogPrintf("tag71[%d] tag72[%d]", srTransPara->stEMVinfo.T71Len, srTransPara->stEMVinfo.T72Len);  
    if( srTransPara->stEMVinfo.T71Len>0)
    {
    	memcpy(&stScript[inlen], srTransPara->stEMVinfo.T71, srTransPara->stEMVinfo.T71Len );
    	inlen=srTransPara->stEMVinfo.T71Len;    	
    }
    if( srTransPara->stEMVinfo.T72Len>0)
    {
    	memcpy(&stScript[inlen], srTransPara->stEMVinfo.T72, srTransPara->stEMVinfo.T72Len );
      inlen= inlen + 	srTransPara->stEMVinfo.T72Len;      
    }	
    DebugAddHEX("inAnalyseChipData ", stScript, inlen);
	inResult = shCTOS_EMVSecondGenAC(stScript, inlen);

	vdDebug_LogPrintf("inAnalyseChipData[%d] srTransPara->shTransResult[%d] srTransPara->byOffline[%d]", inResult, srTransPara->shTransResult, srTransPara->byOffline);

    if(inResult == PP_OK)
    {
        vdDisplayTxnFinishUI();
        //if(0 != memcmp(srTransRec.szAuthCode, "Y3", 2))
        {
            inMyFile_ReversalDelete();

            #if 0
            if (srTransPara->byOffline == CN_FALSE)
    		{		
    		    if(strHDT.inNumAdv > 0)
                {
                    inCTLOS_Updatepowrfail(PFR_BEGIN_BATCH_UPDATE);
    			    inProcessAdviceTrans(srTransPara, strHDT.inNumAdv);
                }
    		}
			#endif
        }
        
        
        inResult = ST_SUCCESS;
        
    }
    else
    {
        if (srTransPara->shTransResult == TRANS_AUTHORIZED || srTransPara->shTransResult == TRANS_COMM_ERROR)
        {
        }
        else
        {
       
            vdDebug_LogPrintf(". Resp Err");
        }
        inResult = ST_ERROR;
        
    }
    
    return inResult;
}

int inAnalyseNonChipData(TRANS_DATA_TABLE *srTransPara)
{
	int	inResult = ST_SUCCESS;

	vdDebug_LogPrintf("**inAnalyseNonChipData(TxnResult = %d) [%d] byTransType[%d] START**", srTransPara->shTransResult, srTransPara->byPackType, srTransPara->byTransType);
    if(srTransPara->byPackType == SETTLE || srTransPara->byPackType == CLS_BATCH)
    {
        if(srTransPara->shTransResult != TRANS_AUTHORIZED)
            inResult = ST_ERROR;
        
    }
    else if (srTransPara->shTransResult == TRANS_AUTHORIZED)
	{
	    vdDisplayTxnFinishUI();

        //Should be Online void the Intial SALE amount.
        if(srTransPara->byTransType == VOID)
        {
            inCTOSS_DeleteAdviceByINV(srTransPara->szInvoiceNo);
        }
        #if 0
		if(srTransPara->byOffline == CN_FALSE)
		{		
		    if(strHDT.inNumAdv > 0)
            {     
                inCTLOS_Updatepowrfail(PFR_BEGIN_BATCH_UPDATE);
			    inProcessAdviceTrans(srTransPara, strHDT.inNumAdv);
            }
		}
		#endif
	}
	else if (srTransPara->shTransResult == TRANS_CANCELLED)
	{		
		inResult = ST_ERROR;
	}
	else
	{		
		inResult = ST_ERROR;
	}

	vdDebug_LogPrintf("**inAnalyseNonChipData(%d) END**", inResult);
	return inResult;
}

int inAnalyseReceiveData(void)
{
	int	inResult;
    TRANS_DATA_TABLE* srTransPara;

    srTransPara = srGetISOEngTransDataAddress();

	if ((srTransPara->byEntryMode == CARD_ENTRY_ICC) && 
            ((srTransPara->byTransType == SALE) || (srTransPara->byTransType == PRE_AUTH) /*|| (srTransPara->byTransType == REFUND)*/)
           )
	{
		inResult = inAnalyseChipData(srTransPara);
	}
	else
	{
		inResult = inAnalyseNonChipData(srTransPara);
	}

	return inResult;
}


int inAnalyseAdviceData(int inPackType)
{   
    TRANS_DATA_TABLE *srTransPara;
    
    srTransPara = srGetISOEngTransDataAddress();
    
    if(srTransPara->byTransType == SALE_OFFLINE)
    {
        srTransPara->byUploaded = CN_FALSE;
        
    }else if(srTransPara->byTransType == SALE_TIP || srTransPara->byTransType == SALE_ADJUST)
    {
           if(srTransPara->byOffline == CN_TRUE)
            srTransPara->byUploaded = CN_FALSE;
    }
    
    return ST_SUCCESS;
}



int inPorcessTransUpLoad(TRANS_DATA_TABLE *srTransPara)
{
    int inSendCount,inTotalCnt,inFileMaxLen;
    int inResult;
    int inBatchRecordNum = 0;
    int i;
    TRANS_DATA_TABLE srUploadTransRec,srTransParaTmp;
    STRUCT_FILE_SETTING strFile;
    int  *pinTransDataid = NULL;
    int  *pinTransDataidSend = NULL;

    inResult = ST_SUCCESS;
    
    memset(&srUploadTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
    memset(&strFile, 0x00, sizeof(STRUCT_FILE_SETTING));

    inBatchRecordNum = inBatchNumRecord();

    vdDebug_LogPrintf("BatchUpload totaltxn[%d]",inBatchRecordNum);
    if(inBatchRecordNum > 0)
    {
        pinTransDataid = (int*)malloc(inBatchRecordNum * sizeof(int));
        pinTransDataidSend = (int*)malloc(inBatchRecordNum * sizeof(int));

	    inBatchByMerchandHost(inBatchRecordNum, srTransRec.HDTid, srTransRec.MITid, srTransRec.szBatchNo, pinTransDataid);

        inTotalCnt = 0;
        for(i=0; i<inBatchRecordNum; i++)
        {
            inDatabase_BatchReadByTransId(&srUploadTransRec, pinTransDataid[i]);

            vdDebug_LogPrintf("BatchUpload curren[%d] Void?[%d]",pinTransDataid[i], srUploadTransRec.byVoided);
            if((srUploadTransRec.byVoided != TRUE) && (srUploadTransRec.byTransType != PRE_AUTH))
            {
                pinTransDataidSend[inTotalCnt] = pinTransDataid[i];
                inTotalCnt ++;
            }
        }

        //in case all is void
        inResult = ST_SUCCESS; 
        inFinalSend = CN_TRUE;

        vdDebug_LogPrintf("BatchUpload total None void txn[%d]",inTotalCnt);
        for (inSendCount = 0; inSendCount < inTotalCnt; inSendCount ++)
        {
            if(((inSendCount + 1) == inTotalCnt))
                inFinalSend = CN_TRUE;
            else
                inFinalSend = CN_FALSE;

            vdDebug_LogPrintf("Before HDTid[%d]MITid[%d]AMT[%02X%02X%02X%02X%02X%02X]", srTransPara->HDTid, srTransPara->MITid, srTransPara->szTotalAmount[0]
                                                                                                                            , srTransPara->szTotalAmount[1]
                                                                                                                            , srTransPara->szTotalAmount[2]
                                                                                                                            , srTransPara->szTotalAmount[3]
                                                                                                                            , srTransPara->szTotalAmount[4]
                                                                                                                            , srTransPara->szTotalAmount[5]);
            inDatabase_BatchReadByTransId(&srUploadTransRec, pinTransDataidSend[inSendCount]);
            
            vdDebug_LogPrintf("After HDTid[%d]MITid[%d]AMT[%02X%02X%02X%02X%02X%02X]", srUploadTransRec.HDTid, srUploadTransRec.MITid, srUploadTransRec.szTotalAmount[0]
                                                                                                                            , srUploadTransRec.szTotalAmount[1]
                                                                                                                            , srUploadTransRec.szTotalAmount[2]
                                                                                                                            , srUploadTransRec.szTotalAmount[3]
                                                                                                                            , srUploadTransRec.szTotalAmount[4]
                                                                                                                            , srUploadTransRec.szTotalAmount[5]);

            if(srUploadTransRec.byTransType == PRE_AUTH)
            {
                continue;
            }
            
            srUploadTransRec.byPackType =  BATCH_UPLOAD;
            
            vdDebug_LogPrintf(". Bef Add szTraceNo = %02x%02x%02x",strHDT.szTraceNo[0],
                                                                strHDT.szTraceNo[1],
                                                                strHDT.szTraceNo[2]);
            
            inMyFile_HDTTraceNoAdd(srUploadTransRec.HDTid);
            
            vdDebug_LogPrintf(". Aft Add szTraceNo = %02x%02x%02x",strHDT.szTraceNo[0],
                                                                strHDT.szTraceNo[1],
                                                                strHDT.szTraceNo[2]);
            
            srUploadTransRec.ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
            vdDebug_LogPrintf(". UploadSendTracNum(%d) [%s]",srUploadTransRec.ulTraceNum,srUploadTransRec.szTID);
			memcpy(&srTransParaTmp,&srUploadTransRec,sizeof(TRANS_DATA_TABLE));
            
            if ((inResult = inPackSendAndUnPackData(&srUploadTransRec, srUploadTransRec.byPackType) != ST_SUCCESS))
            {
                vdDebug_LogPrintf(". inPorcessTransUpLoad(%d)_Err",inResult);
                vdDebug_LogPrintf(". byTransType %d",srUploadTransRec.byTransType);
                inResult = ST_UNPACK_DATA_ERR;
                break;
            }
            else
            {
                if (memcmp(srUploadTransRec.szRespCode, "00", 2))
                {
                    vdDebug_LogPrintf(". inPorcessTransUpLoad(%s) BatchUpload Fail", srUploadTransRec.szRespCode);
                    inResult = ST_ERROR;
                    break;
                }
				else
				{
					if (ST_SUCCESS != inBaseRespValidation(&srTransParaTmp,&srUploadTransRec))
					{
						inResult = ST_UNPACK_DATA_ERR;
						break;
					}
				}
            
                vdDebug_LogPrintf(". inPorcessTransUpLoad(%d)BatchUpload OK", inResult);
            }
        }

        free(pinTransDataid);
        free(pinTransDataidSend);
    
    }
    else
    {
        inResult = ST_ERROR;
        vdDebug_LogPrintf("No bath record found");
    }

    /*albert - fix stan issue*/
    //inMyFile_HDTTraceNoAdd(srTransPara->HDTid);
    //srUploadTransRec.ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
    //vdMyEZLib_LogPrintf(". Aft Upload TraceNum(%d)",srUploadTransRec.ulTraceNum);
    
    if(inResult == ST_SUCCESS && inFinalSend == CN_TRUE)
    {
        srTransPara->byPackType = CLS_BATCH; /*albert - fix stan issue*/
        inMyFile_HDTTraceNoAdd(srTransPara->HDTid);
        srTransPara->ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
        vdMyEZLib_LogPrintf(". Aft CLS_BATCH TraceNum(%d)",srTransPara->ulTraceNum);
        if ((inResult = inPackSendAndUnPackData(srTransPara, srTransPara->byPackType) != ST_SUCCESS))
        {
            vdMyEZLib_LogPrintf(". FinalSettle(%d)_Err",inResult);
            vdMyEZLib_LogPrintf(". byTransType %d",srTransPara->byTransType);
            inResult = ST_UNPACK_DATA_ERR;
        }

		/* BDO: Check settlement reconciliation response code - start -- jzg */
		if(memcmp(srTransPara->szRespCode, "00", 2) != 0)
		{
			vdDebug_LogPrintf("JEFF::ST_CLS_BATCH_ERR");
			inResult = ST_CLS_BATCH_ERR;
		}
		/* BDO: Check settlement reconciliation response code - end -- jzg */
        //inMyFile_HDTTraceNoAdd(srTransPara->HDTid);
        //srTransPara->ulTraceNum = wub_bcd_2_long(strHDT.szTraceNo,3);
        //vdMyEZLib_LogPrintf(". Aft CLS_BATCH TraceNum(%d)",srTransPara->ulTraceNum);
    }

    return inResult;
}


int inPackIsoFunc02(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    int inPANLen;
    char szTempPan[PAN_SIZE+1];

    vdDebug_LogPrintf("**inPackIsoFunc02 START**");
    
    inDataCnt = 0;
    inPANLen = 0;

    memset(szTempPan, 0x00, sizeof(szTempPan));   
    inPANLen = strlen(srTransPara->szPAN);
    memcpy(szTempPan,srTransPara->szPAN,inPANLen);
    
    uszSendData[inDataCnt ++] = (inPANLen / 10 * 16) + (inPANLen % 10);
        
    if (inPANLen % 2)
        szTempPan[inPANLen ++] = '0';
    
#ifdef TLE
    memset(&uszSendData[inDataCnt], 0x00, (inPANLen+1) / 2);
    byField_02_ON = TRUE;
#else
    wub_str_2_hex(szTempPan, (char *)&uszSendData[inDataCnt], inPANLen);
#endif    
    inDataCnt += (inPANLen / 2);
    
    vdDebug_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdDebug_LogPrintf("**inPackIsoFunc02 END**");
    
    return (inDataCnt);

}

int inPackIsoFunc03(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    inDataCnt = 0;
    
    vdDebug_LogPrintf("**inPackIsoFunc03 START**");
    
    memcpy(&uszSendData[inDataCnt], &srTransPara->szIsoField03[0], PRO_CODE_BCD_SIZE);
    inDataCnt += PRO_CODE_BCD_SIZE;
    
    if((BATCH_UPLOAD == srTransPara->byPackType))
    {
        //fix Tip adjust Sale , batch upload process code is 02000x, should be 00000x
        uszSendData[0] &= 0xF0;
        
        if(inFinalSend != CN_TRUE)
            uszSendData[2] |= 0x01;
    }

	//if(srTransPara->byPackType != TC_UPLOAD)
	//{
		//if(srTransPara->byEntryMode == CARD_ENTRY_ICC && srTransPara->byTransType == SALE)
			//uszSendData[2] |= 0x01;
	//}
		
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc03 END**");
    return (inDataCnt);
}

int inPackIsoFunc04(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    inDataCnt = 0;

    //Should be Online void the Intial SALE amount.
    if(srTransPara->byTransType == VOID)
    {
        //use szStoreID to store how much amount fill up in DE4 for VOID
        memcpy((char *)&uszSendData[inDataCnt], srTransPara->szStoreID, 6);
    }
    else
        memcpy((char *)&uszSendData[inDataCnt], srTransPara->szTotalAmount, 6);
    
    inDataCnt += 6;
    return (inDataCnt);
}

int inPackIsoFunc11(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    char szSTAN[6 + 1];
    
    inDataCnt = 0;    
    memset(szSTAN, 0x00, sizeof(szSTAN));
    
    
    if(srTransPara->byTransType == SETTLE)// 20121204
    {
        sprintf(szSTAN, "%06ld", srTransPara->ulTraceNum);
        wub_str_2_hex(&szSTAN[0], (char *)&uszSendData[inDataCnt], 6);
        vdDebug_LogPrintf("sys trace %d",uszSendData[0], uszSendData[1],uszSendData[2]);        
    }
     else                  
    {
        if((srTransPara->byPackType == TC_UPLOAD) || srTransPara->byTransType == CLS_BATCH)
        {
            vdDebug_LogPrintf("**inPackIsoFunc11 %d**", srTransPara->ulTraceNum);
            sprintf(szSTAN, "%06ld", (srTransPara->ulTraceNum+1));
        }
        else
            sprintf(szSTAN, "%06ld", srTransPara->ulTraceNum);
        
        wub_str_2_hex(&szSTAN[0], (char *)&uszSendData[inDataCnt], 6);
    }
    
    inDataCnt += 3;
    
    vdDebug_LogPrintf("  PACK_LEN[%d] %d [%02X%02X%02X] inFinalSend[%d]byPackType[%d]",inDataCnt, srTransPara->byPackType, uszSendData[0], uszSendData[1], uszSendData[2], inFinalSend, srTransPara->byPackType);
    return (inDataCnt);
}

void vdGetTimeDate(TRANS_DATA_TABLE *srTransPara)
{
    if(srTransPara->byPackType == SALE || srTransPara->byPackType == PRE_AUTH || srTransPara->byPackType == LOG_ON 
	|| srTransPara->byPackType == BAL_INQ || srTransPara->byPackType == CARD_VERIFY || srTransPara->byPackType == REFUND)		 
    {
        CTOS_RTC SetRTC;
        BYTE szCurrentTime[20];
        
        CTOS_RTCGet(&SetRTC);
		memset(szCurrentTime, 0, sizeof(szCurrentTime));
        sprintf(szCurrentTime,"%02d%02d",SetRTC.bMonth, SetRTC.bDay);
        wub_str_2_hex(szCurrentTime,srTransPara->szDate,DATE_ASC_SIZE);

		memset(szCurrentTime, 0, sizeof(szCurrentTime));
        sprintf(szCurrentTime,"%02d%02d%02d", SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
        wub_str_2_hex(szCurrentTime,srTransPara->szTime,TIME_ASC_SIZE);
    }
}


int inPackIsoFunc12(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    char szTempTime[6];

	vdGetTimeDate(srTransPara);
		
    vdMyEZLib_LogPrintf("**inPackIsoFunc12 START**");
    inDataCnt = 0;
    
    if(srTransPara->byPackType == SALE_TIP || srTransPara->byPackType == SALE_ADJUST || srTransPara->byPackType == SALE_TIP)
        memcpy((char *)&uszSendData[inDataCnt], srTransPara->szOrgTime, 2);
    else
       memcpy((char *)&uszSendData[inDataCnt], srTransPara->szTime, 3);
    inDataCnt += 3;
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc12 END**");
    return (inDataCnt);
}

int inPackIsoFunc13(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    char szTempDate[6];
    
    vdMyEZLib_LogPrintf("**inPackIsoFunc13 START**");
    inDataCnt = 0;
    
    if(srTransPara->byPackType == SALE_TIP || srTransPara->byPackType == SALE_ADJUST || srTransPara->byPackType == SALE_TIP)
        memcpy((char *)&uszSendData[inDataCnt], srTransPara->szOrgDate, 2);
    else
        memcpy((char *)&uszSendData[inDataCnt], srTransPara->szDate, 2);
    inDataCnt += 2;
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc13 END**");
    return (inDataCnt);
}

int inPackIsoFunc14(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    char szTempExpDate[6];
    vdMyEZLib_LogPrintf("**inPackIsoFunc14 START**");
    inDataCnt = 0;

#ifdef TLE 
    memcpy((char *)&uszSendData[inDataCnt], "\x00\x00", 2);
    byField_14_ON = TRUE;
#else
    memcpy((char *)&uszSendData[inDataCnt], srTransPara->szExpireDate, 2);
#endif
    inDataCnt += 2;
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc14 END**");
    return (inDataCnt);
}

int inPackIsoFunc22(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    char szTempEnterMode[5];
    vdMyEZLib_LogPrintf("**inPackIsoFunc22 START**");
    
    inDataCnt = 0;
    
   
    memset(szTempEnterMode,0,sizeof(szTempEnterMode));
    
    if (srTransPara->byEntryMode == CARD_ENTRY_ICC)
    {
        if(srTransPara->byPINEntryCapability == 0)
            strcpy(szTempEnterMode,"0052");
        else
            strcpy(szTempEnterMode,"0051");			
    }
    else if (srTransPara->byEntryMode == CARD_ENTRY_FALLBACK)
    {
        //sprintf(szTempEnterMode,"080%d",srTransPara->byPINEntryCapability);
		strcpy(szTempEnterMode,"0802");
    }
     else if (srTransPara->byEntryMode == CARD_ENTRY_MSR)
    {
        //sprintf(szTempEnterMode,"002%d",srTransPara->byPINEntryCapability);
        strcpy(szTempEnterMode,"0022");
    }
    else if (srTransPara->byEntryMode  == CARD_ENTRY_MANUAL)
    {        
         //sprintf(szTempEnterMode,"001%d",srTransPara->byPINEntryCapability);
         strcpy(szTempEnterMode,"0012");
    }
	else if (srTransPara->byEntryMode  == CARD_ENTRY_WAVE)
    {        
         //sprintf(szTempEnterMode,"007%d",srTransPara->byPINEntryCapability);
         strcpy(szTempEnterMode,"0072");
		 if (srTransPara->bWaveSID == d_VW_SID_PAYPASS_MAG_STRIPE)
		 	//sprintf(szTempEnterMode,"091%d",srTransPara->byPINEntryCapability);
		 	strcpy(szTempEnterMode,"0912");
    }
    
    wub_str_2_hex(szTempEnterMode, (char *)&uszSendData[inDataCnt], 4);
    
    inDataCnt +=2 ; //+= 3; 
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc22 END**");
    return (inDataCnt);
}

int inPackIsoFunc23(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc23 START**");
    inDataCnt = 0;
    
    uszSendData[inDataCnt] = 0x00;
    uszSendData[inDataCnt+1] = srTransPara->stEMVinfo.T5F34;
    
    inDataCnt += 2;
        
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc23 END**");
    return (inDataCnt);
}


int inPackIsoFunc24(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    inDataCnt = 0;

    DebugAddHEX("inPackIsoFunc24", strHDT.szNII, 2);
    
    memcpy((char *)&uszSendData[inDataCnt], strHDT.szNII, 2);
    
    inDataCnt += 2;
            
    return (inDataCnt);
}

int inPackIsoFunc25(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc25 START**");
    inDataCnt = 0;
    
    //if(srTransRec.byTransType == PRE_AUTH)
        //wub_str_2_hex("06", (char *)&uszSendData[inDataCnt], 2);
    //else
        wub_str_2_hex("00", (char *)&uszSendData[inDataCnt], 2);
    
    inDataCnt += 1;
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc25 END**");
    return (inDataCnt);
}

int inPackIsoFunc35(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    char szTrack2Data[40];
    int inLen;
    int i;
    
    vdMyEZLib_LogPrintf("**inPackIsoFunc35 START**");
    inDataCnt = 0;
    inLen = 0;
    
    memset(szTrack2Data,0x00,sizeof(szTrack2Data));   
    strcpy(szTrack2Data, srTransPara->szTrack2Data);

    for (i = 0; i < strlen(szTrack2Data); i ++)
    {
        if (szTrack2Data[i] == '=')
            szTrack2Data[i] = 'D';
    }

    inLen = strlen(szTrack2Data);
    
    /* Data Length */
			
    uszSendData[inDataCnt ++] = (inLen / 10 * 16) + (inLen % 10);

    if (inLen % 2)
    {
        inLen ++;
        strcat(szTrack2Data, "0");
    }
#ifdef TLE
    memset((char *)&uszSendData[inDataCnt], 0x00, (inLen/2)+1);
    byField_35_ON = TRUE;
#else
    wub_str_2_hex(&szTrack2Data[0], (char *)&uszSendData[inDataCnt], inLen);
#endif
    inDataCnt += (inLen / 2);
    
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc35 END**");
    
    return (inDataCnt);
}

int inPackIsoFunc37(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc37 START**");
    inDataCnt = 0;

    memcpy((char *)&uszSendData[inDataCnt], srTransPara->szRRN, 12);
    inDataCnt += 12;
    
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc37 END**");
    return inDataCnt;
}

int inPackIsoFunc38(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc38 START**");

    inDataCnt = 0;
    memcpy((char *)&uszSendData[inDataCnt], srTransPara->szAuthCode, 6);

    inDataCnt += 6;
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc38 END**");
    return inDataCnt;
}

int inPackIsoFunc39(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc39 START**");
    inDataCnt = 0;
    memcpy((char *)&uszSendData[inDataCnt], srTransPara->szRespCode, 2);
    
    inDataCnt += 2;
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc39 END**");
    return inDataCnt;
}

int inPackIsoFunc41(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    BYTE szTID[TERMINAL_ID_BYTES+1];

    inDataCnt = 0;    

    memset(szTID, 0x00, sizeof(szTID));
    memset(szTID, 0x20, TERMINAL_ID_BYTES);
    memcpy(szTID, srTransPara->szTID, strlen(srTransPara->szTID));
    memcpy((char *)&uszSendData[inDataCnt], szTID, TERMINAL_ID_BYTES);
    inDataCnt += TERMINAL_ID_BYTES;
    vdDebug_LogPrintf(" TID[%s] PACK_LEN[%d]",szTID, inDataCnt);
    
    return inDataCnt;
}

int inPackIsoFunc42(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    BYTE szMID[MERCHANT_ID_BYTES+1];
    
    inDataCnt = 0;

    memset(szMID, 0x00, sizeof(szMID));
    memset(szMID, 0x20, MERCHANT_ID_BYTES);
    memcpy(szMID, srTransPara->szMID, strlen(srTransPara->szMID));
    memcpy((char *)&uszSendData[inDataCnt], szMID, MERCHANT_ID_BYTES);
    inDataCnt += MERCHANT_ID_BYTES;
    vdDebug_LogPrintf(" MID[%s] PACK_LEN[%d]",szMID, inDataCnt);
    
    return inDataCnt;
}

int inPackIsoFunc45(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{

    vdMyEZLib_LogPrintf("**inPackIsoFunc45 START**");
    inDataCnt = 0;
    uszSendData[inDataCnt++] = (srTransPara->usTrack1Len%100)/10*16+
                               (srTransPara->usTrack1Len%100)%10;
    vdMyEZLib_LogPrintf("  45Len%02x",uszSendData[0]);
#ifdef TLE
    memset((char *)&uszSendData[inDataCnt], 0x00, srTransPara->usTrack1Len);
    byField_45_ON = TRUE;
#else
    memcpy((char *)&uszSendData[inDataCnt], srTransPara->szTrack1Data, srTransPara->usTrack1Len);
#endif
    inDataCnt += srTransPara->usTrack1Len;
    
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc45 END**");
    
    return inDataCnt;
}

int inPackIsoFunc48(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    int inCVV2Len;
    BYTE szBuf[20];
    vdMyEZLib_LogPrintf("**inPackIsoFunc48 START**");
    inDataCnt = 0;
        
#ifdef TLE
    memset(&uszSendData[inDataCnt], 0x00, 2);
    byField_48_ON = TRUE;
    inDataCnt += 2;
#else
    DebugAddSTR("inPackIsoFunc48", srTransPara->szCVV2, 4);
    inCVV2Len = strlen(srTransPara->szCVV2);
    DebugAddINT("LEN", inCVV2Len);
    memset(szBuf, 0x00, sizeof(szBuf));
    sprintf(szBuf, "%04d", inCVV2Len);
    DebugAddSTR("inPackIsoFunc48", szBuf, 4);
    wub_str_2_hex(szBuf, (char *)&uszSendData[inDataCnt], 4);
    inDataCnt += 2;
    memcpy((char *)&uszSendData[inDataCnt], srTransPara->szCVV2, inCVV2Len);
    inDataCnt += inCVV2Len;
#endif

     return inDataCnt;
}


int inPackIsoFunc52(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    char szTempPin[17];
    vdMyEZLib_LogPrintf("**inPackIsoFunc52 START**");
    inDataCnt = 0;
    
    //wub_str_2_hex((BYTE *)&uszSendData[inDataCnt], szTempPin, 16);
    memcpy((char *)uszSendData, srTransPara->szPINBlock, 8);
    
    inDataCnt += 8;
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc52 END**");
    return inDataCnt;
}



int inPackIsoFunc54(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    int inPacketCnt = 0;
    char szAscBuf[4 + 1], szBcdBuf[2 + 1];
    char szPacket[20 + 1];
    char szBaseAmt[20 + 1];
    char szVoidTotalAmt[20 + 1];

    inDataCnt = 0;
    DebugAddHEX("inPackIsoFunc54",srTransPara->szTipAmount,6);
    
    memset(szPacket, 0x00, sizeof(szPacket));
    
     //Should be Online void the Intial SALE amount.
    if(srTransPara->byTransType == VOID)
    {
        //use szStoreID to store how much amount fill up in DE4 for VOID
        memset(szBaseAmt, 0x00, sizeof(szBaseAmt));
        memset(szVoidTotalAmt, 0x00, sizeof(szVoidTotalAmt));
        
        wub_hex_2_str(srTransPara->szBaseAmount, szBaseAmt, 6);
        wub_hex_2_str(srTransPara->szStoreID, szVoidTotalAmt, 6);
        sprintf(szPacket, "%012ld", atol(szVoidTotalAmt) - atol(szBaseAmt));
    }
    else
    {
        wub_hex_2_str(srTransPara->szTipAmount, szPacket, 6);
    }

    inPacketCnt = strlen(szPacket);
    memset(szAscBuf, 0x00, sizeof(szAscBuf));
    sprintf(szAscBuf, "%04d", inPacketCnt);
	wub_str_2_hex(szAscBuf, szBcdBuf, 4);
	memcpy((char *)&uszSendData[inDataCnt], &szBcdBuf[0], 2);
	inDataCnt += 2;

    DebugAddHEX("inPackIsoFunc54", szBcdBuf, 2);
    
	/* Packet Data */
	memcpy((char *)&uszSendData[inDataCnt], &szPacket[0], inPacketCnt);
	inDataCnt += inPacketCnt;

    DebugAddSTR("inPackIsoFunc54", szPacket, 12);
	
	vdMyEZLib_LogPrintf("**inPackIsoFunc54 START**");
	return inDataCnt;
}


int inPackIsoFunc55(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc55 START**");
    
    inDataCnt = 0;

	if (srTransPara->byEntryMode == CARD_ENTRY_ICC)
    	inDataCnt = inPackISOEMVData(srTransPara, uszSendData);
	if (srTransPara->byEntryMode == CARD_ENTRY_WAVE)
	{
		if (srTransPara->bWaveSID == d_VW_SID_PAYPASS_MCHIP)
			inDataCnt = inPackISOPayPassData(srTransPara, uszSendData);

		if (srTransPara->bWaveSID == d_VW_SID_VISA_WAVE_2 ||
			srTransPara->bWaveSID == d_VW_SID_VISA_WAVE_QVSDC )
			inDataCnt = inPackISOPayWaveData(srTransPara, uszSendData);
	}

    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc55 END**");
    return inDataCnt;
}

int inPackIsoFunc56(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc56 START**");
    inDataCnt = 0;
    
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc56 END**");
    return inDataCnt;
}

int inPackIsoFunc57(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    int	           in_FieldLen = 0, inPacketCnt = 0, inPacketLen=0;
    BYTE           szAscBuf[4 + 1], szBcdBuf[2 + 1];
    BYTE           szPacket[512 + 1];
    BYTE           szPacketASC[512 + 1];
    unsigned short usLen, usRetVal;
    BYTE           btTrack2[20];
    int            inBitMapIndex;
    BYTE           szDataBuf[255], szTrack2Data[40], szTempPan[19];
    short   i,in_Field35Len=0, in_Field02Len=0;
    char szAscBuf1[4 + 1];
    
        
    inTLERead(1);
    vdMyEZLib_LogPrintf("**inPackIsoFunc57 START**");
    memset(szPacket, 0, sizeof(szPacket));
    inDataCnt = 0; 

 		if(srTransPara->byTransType == SETTLE || srTransPara->byTransType == CLS_BATCH)
 		{
    		inPacketCnt=18;    		   	    
   	    sprintf(szAscBuf, "%04d", inPacketCnt);
		    memset(szBcdBuf, 0, sizeof(szBcdBuf));
		    wub_str_2_hex(szAscBuf, szBcdBuf, 4);
		    memcpy((char *)&uszSendData[inDataCnt], &szBcdBuf[0], 2);		    
		    inDataCnt += 2;
		    memcpy(&uszSendData[inDataCnt], stTLE.szVERSION, 2);
    		inDataCnt += 2;    		
    		
    		memset(szPacketASC, 0x30, 16);    
    		vdTripleDES_CBC(d_ENCRYPTION, szPacketASC, 16, szDataBuf);;     		
			  memcpy((char *)&uszSendData[inDataCnt], &szDataBuf[0], 16);
		    inDataCnt += 16;
				
				vdMyEZLib_LogPrintff(uszSendData,inDataCnt );
				    
 			  return (inDataCnt);
 		} 	        
    else if(srTransPara->byTransType == SIGN_ON)
    {
    		memcpy(&szPacket[inPacketCnt], stTLE.szVERSION, 2);
    		inPacketCnt = inPacketCnt + 2; 
    
        memcpy(&szPacket[inPacketCnt], stTLE.szTMKRefNum, 8);
        inPacketCnt = inPacketCnt + 8;
        /* Packet Data Length */
		    memset(szAscBuf, 0, sizeof(szAscBuf));
		    sprintf(szAscBuf, "%04d", inPacketCnt);
		    memset(szBcdBuf, 0, sizeof(szBcdBuf));
		    wub_str_2_hex(szAscBuf, szBcdBuf, 4);
		    memcpy((char *)&uszSendData[inDataCnt], &szBcdBuf[0], 2);
		    inDataCnt += 2;
			  memcpy((char *)&uszSendData[inDataCnt], &szPacket[0], inPacketCnt);
		    inDataCnt += inPacketCnt;
    
        return (inDataCnt);
    }
    else
    {
        inBitMapIndex = inPacketCnt;
    
        inPacketCnt = inPacketCnt + 8; // skip bit map
        if (byField_02_ON == CN_TRUE)
        {
	          szPacket[inBitMapIndex+0] = 0x40;
	
						strcpy(szTempPan, srTransPara->szPAN);
						in_Field02Len = strlen(srTransPara->szPAN);
						if (in_Field02Len % 2)
						{	
							in_FieldLen= in_Field02Len + 1;
        			szTempPan[in_FieldLen] = '0';        			
            }
            else            		          
             in_FieldLen= in_Field02Len;
    				
    				vdisoTwoOne(srTransPara->szPAN, in_FieldLen, &szPacket[inPacketCnt]);
            
            vdMyEZLib_LogPrintf("**PAN**");
            vdMyEZLib_LogPrintff(&szPacket[inPacketCnt], in_FieldLen/2);
            inPacketCnt += in_FieldLen/2;
        }

    
        if (byField_14_ON == CN_TRUE)
        {
            szPacket[inBitMapIndex + 1] = szPacket[inBitMapIndex + 1] | 0x04;
        
            memcpy((char *)&szPacket[inPacketCnt], srTransPara->szExpireDate, 2);
            vdMyEZLib_LogPrintf("**Expired**");
            vdMyEZLib_LogPrintff(&szPacket[inPacketCnt], 2);
            inPacketCnt += 2;
        }
    
        if (byField_35_ON == CN_TRUE && srTransPara->byPackType != TC_UPLOAD)
        {
            szPacket[inBitMapIndex + 4] = szPacket[inBitMapIndex + 4] | 0x20;
                        
            strcpy(szTrack2Data, srTransPara->szTrack2Data);

				    for (i = 0; i < strlen(szTrack2Data); i ++)
				    {
				        if (szTrack2Data[i] == '=')
				            szTrack2Data[i] = 'D';
				    }
            in_Field35Len = strlen(szTrack2Data);
    				if (in_Field35Len % 2)
    				{
        				in_FieldLen= in_Field35Len + 1;
        				strcat(szTrack2Data, "0");
    				} 
    				else
    					in_FieldLen=  in_Field35Len; 					       
         		vdisoTwoOne(szTrack2Data, in_FieldLen, &szPacket[inPacketCnt]);
            vdMyEZLib_LogPrintf("**Track 2**");
            vdMyEZLib_LogPrintff(&szPacket[inPacketCnt], in_FieldLen/2);
            inPacketCnt += (in_FieldLen/2);
        }

        if (byField_45_ON == CN_TRUE && srTransPara->byPackType != TC_UPLOAD)
        {
            szPacket[inBitMapIndex + 5] = szPacket[inBitMapIndex + 5] | 0x08;
	
            in_FieldLen = strlen(srTransPara->szTrack1Data);	
        
            memcpy((char *)&szPacket[inPacketCnt], srTransPara->szTrack1Data, in_FieldLen);
            inPacketCnt += in_FieldLen;
        }

        if(strTCT.fCVVEnable)
        {
        if (byField_48_ON == CN_TRUE)
        {
            szPacket[inBitMapIndex + 5] = szPacket[inBitMapIndex + 5] | 0x01;
	    
                        sprintf(szAscBuf1,"%s","%04s");
        		sprintf(szAscBuf,szAscBuf1,(char *)srTransPara->szCVV2);
                        
         		vdisoTwoOne(szAscBuf, 4, szBcdBuf);
        		
        		vdMyEZLib_LogPrintf("**CVV**  %s",srTransPara->szCVV2);        
        		vdMyEZLib_LogPrintff(szBcdBuf,2);
        		
            memcpy((char *)&szPacket[inPacketCnt], szBcdBuf, 2);
            inPacketCnt += 2;
        }
        }
    }
    
    memset(szPacketASC, 0x30, sizeof(szPacketASC));
    vdMyEZLib_LogPrintf("**before**  %d",inPacketCnt);
		vdMyEZLib_LogPrintff(szPacket,inPacketCnt );
		
    vdisoOneTwo(szPacket, szPacketASC, inPacketCnt);        
    inPacketCnt = inPacketCnt*2;    
    
    vdMyEZLib_LogPrintf("**before**  %d",inPacketCnt);
		vdMyEZLib_LogPrintf(szPacketASC,inPacketCnt );
		
    if(inPacketCnt%8!=0)
    {
        szPacketASC[inPacketCnt] = 0x38;
        inPacketCnt = inPacketCnt + (8-(inPacketCnt%8));
    }
    
    memset(szDataBuf, 0, sizeof(szDataBuf));
    vdTripleDES_CBC(d_ENCRYPTION, szPacketASC, inPacketCnt, szDataBuf);
    
	/* Packet Data Length */
	  inPacketLen= inPacketCnt+2;  // add 2 for stTLE.szVERSION
    memset(szAscBuf, 0, sizeof(szAscBuf));
    sprintf(szAscBuf, "%04d", inPacketLen);
    memset(szBcdBuf, 0, sizeof(szBcdBuf));
    wub_str_2_hex(szAscBuf, szBcdBuf, 4);
    memcpy((char *)&uszSendData[inDataCnt], &szBcdBuf[0], 2);
    inDataCnt += 2;
    memcpy(&uszSendData[inDataCnt], stTLE.szVERSION, 2);
    inDataCnt += 2;
    
	/* Packet Data */
    memcpy((char *)&uszSendData[inDataCnt], &szDataBuf[0], inPacketCnt);
    inDataCnt += inPacketCnt;
	
	  vdMyEZLib_LogPrintf("**57 data**");
		vdMyEZLib_LogPrintff(uszSendData,inDataCnt );
    vdMyEZLib_LogPrintf("**inPackIsoFunc57 START**");
    return (inDataCnt);
}

int inPackIsoFunc60(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    int inPacketCnt = 0;
    char szAscBuf[4 + 1], szBcdBuf[2 + 1];
    char szPacket[100 + 1];
    vdDebug_LogPrintf("**inPackIsoFunc60 byPackType[%d]ulOrgTraceNum[%ld]szMassageType[%02X%02X]**", srTransPara->byPackType, srTransPara->ulOrgTraceNum, srTransPara->szMassageType[0], srTransPara->szMassageType[1]);
    inDataCnt = 0;
    
    memset(szPacket, 0x00, sizeof(szPacket));
    
    if((srTransPara->byPackType == BATCH_UPLOAD) || (srTransPara->byPackType == TC_UPLOAD))
    {   
        /* Load the Original Data Message in field 60 */
        /* Field 60 contains 4 digits of MID, 6 digits of STAN
           and 12 digits of Reserved space set to spaces.
           */
        wub_hex_2_str(srTransPara->szMassageType,szPacket,2);
        inPacketCnt += 4;

        sprintf(&szPacket[inPacketCnt], "%06ld", srTransPara->ulOrgTraceNum);
        inPacketCnt += 6;
        
        memcpy(&szPacket[inPacketCnt],srTransPara->szRRN,RRN_BYTES);
        inPacketCnt += RRN_BYTES;  
    }
    else if(srTransPara->byTransType == SETTLE || srTransPara->byTransType == CLS_BATCH)
    {
        //wub_hex_2_str(srTransRec.szBatchNo,szPacket,3);
        wub_hex_2_str(srTransPara->szBatchNo,szPacket,3);
        
        inPacketCnt += 6;
    }
    else
    {
        if(srTransPara->byUploaded == TRUE)
            wub_hex_2_str(srTransPara->szOrgAmount,szPacket,6);
        else			
            wub_hex_2_str(srTransPara->szBaseAmount,szPacket,6);
        inPacketCnt += 12;
    }
   
    /* Packet Data Length */
	memset(szAscBuf, 0, sizeof(szAscBuf));
	sprintf(szAscBuf, "%04d", inPacketCnt);
	memset(szBcdBuf, 0, sizeof(szBcdBuf));
	wub_str_2_hex(szAscBuf, szBcdBuf, 4);
	memcpy((char *)&uszSendData[inDataCnt], &szBcdBuf[0], 2);
	inDataCnt += 2;
	/* Packet Data */
	memcpy((char *)&uszSendData[inDataCnt], &szPacket[0], inPacketCnt);
	inDataCnt += inPacketCnt;
	
	vdDebug_LogPrintf(". Pack Len(%d)",inDataCnt);
	vdDebug_LogPrintf("**inPackIsoFunc60 END**");
	return inDataCnt;

}


int inPackIsoFunc61(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoFunc61 START**");
    inDataCnt = 0;
    uszSendData[inDataCnt++] = 0x00;
    uszSendData[inDataCnt++] = 0x06;
    sprintf((char *)&uszSendData[inDataCnt], "%06ld", wub_bcd_2_long(srTransPara->szInvoiceNo,3));
    inDataCnt += 6;
    
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc61 END**");
    return inDataCnt;
}

int inPackIsoInstallment61(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    vdMyEZLib_LogPrintf("**inPackIsoInstallment61 START**");
    char szPacket[100 + 1];
    char szTemp[12+1], szTemp2[12+1];
	char szAscBuf[4 + 1], szBcdBuf[2 + 1];
    int inPacketCnt = 0;
    	
    inDataCnt = 0;

    if(srTransPara->fInstallment == TRUE)
    {
        memset(szPacket, 0, sizeof(szPacket));
        if(srTransPara->byTransType == VOID || srTransPara->byPackType == BATCH_UPLOAD
        || srTransPara->byPackType == TC_UPLOAD)
        {
    		memcpy(szPacket, srTransPara->szInstallmentData, 81);
            inPacketCnt=81;
        }
        else if(srTransPara->byTransType == SALE)
        {
            /*installment plan*/
            memset(szTemp, 0, sizeof(szTemp));
            sprintf(szTemp, "%03d", atol(srTransPara->szPromo));
            memcpy(szPacket, szTemp, 3); inPacketCnt+=3;
            
            /*installment term*/		
            memset(szTemp, 0, sizeof(szTemp));
            sprintf(szTemp, "%02d", atol(srTransPara->szTerms));
            memcpy(&szPacket[inPacketCnt], szTemp, 2); inPacketCnt+=2;
            
            /*compute method*/
            memcpy(&szPacket[inPacketCnt], "0", 1); inPacketCnt+=1;
            
            /*interest rate - 6 bytes*/ 
            memcpy(&szPacket[inPacketCnt], "000000", 6); inPacketCnt+=6;
            
            /*interest free months - 2 bytes*/ 
            memcpy(&szPacket[inPacketCnt], "00", 2); inPacketCnt+=2;
            
            /*first pay - 9 bytes*/ 
            memcpy(&szPacket[inPacketCnt], "000000000", 9); inPacketCnt+=9;
            
            /*last pay - 9 bytes*/ 
            memcpy(&szPacket[inPacketCnt], "000000000", 9); inPacketCnt+=9;
            
            /*monthly installment amount - 9 bytes*/
            memcpy(&szPacket[inPacketCnt], "000000000", 9); inPacketCnt+=9;
            
            /*total installment amount - 9 bytes*/
            memcpy(&szPacket[inPacketCnt], "000000000", 9); inPacketCnt+=9;
            
            /*gross amount*/ 
            memset(szTemp, 0, sizeof(szTemp));
            memset(szTemp2, 0, sizeof(szTemp2));
            wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
            sprintf(szTemp2, "%09ld", atol(szTemp));
            memcpy(&szPacket[inPacketCnt], szTemp2, 9); inPacketCnt+=9;
            
            /*out interest - 9 bytes*/
            memcpy(&szPacket[inPacketCnt], "000000000", 9); inPacketCnt+=9;
            
            /*handling fee - 9 bytes*/
            memcpy(&szPacket[inPacketCnt], "000000000", 9); inPacketCnt+=9;
            
            /*wave fr mos - 2 bytes*/
            memcpy(&szPacket[inPacketCnt], "00", 2); inPacketCnt+=2;
            
            /*wave fr mos - 2 bytes*/
            memcpy(&szPacket[inPacketCnt], "00", 2); inPacketCnt+=2;
        }
    }
	
	/* Packet Data Length */
    memset(szAscBuf, 0, sizeof(szAscBuf));
    sprintf(szAscBuf, "%04d", inPacketCnt);
    memset(szBcdBuf, 0, sizeof(szBcdBuf));
    wub_str_2_hex(szAscBuf, szBcdBuf, 4);
    memcpy((char *)&uszSendData[inDataCnt], &szBcdBuf[0], 2);
    inDataCnt += 2;
    /* Packet Data */
    memcpy((char *)&uszSendData[inDataCnt], &szPacket[0], inPacketCnt);
    inDataCnt += inPacketCnt;
	
    vdDebug_LogPrintf(". Pack Len(%d)",inDataCnt);
    vdDebug_LogPrintf("**inPackIsoFunc61 END**");
    return inDataCnt;
}


int inPackIsoFunc62(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
	  char szPacket[7];

      DebugAddHEX("inPackIsoFunc62", srTransPara->szInvoiceNo, 3);

    inDataCnt = 0;
    uszSendData[inDataCnt++] = 0x00;
    uszSendData[inDataCnt++] = 0x06;
    if(srTransPara->byTransType == SETTLE || srTransPara->byTransType == CLS_BATCH)
    {
    	memcpy((char *)&uszSendData[inDataCnt],"000000",6);
    }
    else
   	{   		
   		sprintf((char *)&uszSendData[inDataCnt], "%06ld", wub_bcd_2_long(srTransPara->szInvoiceNo,3));   		
   	}
    inDataCnt += 6;
    
    vdMyEZLib_LogPrintf("  PACK_LEN%d",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackIsoFunc62 END**");
    return inDataCnt;
}


int inPackIsoFunc63(TRANS_DATA_TABLE *srTransPara, unsigned char *uszSendData)
{
     
	int inPacketCnt = 0;
    int inResult;
    int inTranCardType;
    ACCUM_REC srAccumRec;
	char szAscBuf[4 + 1], szBcdBuf[2 + 1];
	char szTemplate[100], szPacket[128 + 1];
    char szTemp[12+1];
	
	memset(szPacket, 0, sizeof(szPacket));
	inDataCnt = 0;

    vdDebug_LogPrintf("Test1111");
    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    if((inResult = inCTOS_ReadAccumTotal(&srAccumRec)) == ST_ERROR)
	{
		vdDebug_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
		return ST_ERROR;	
	}		
    vdDebug_LogPrintf("Test2222");
    //0 is for Credit type, 1 is for debit type
    inTranCardType = 0;
    vdDebug_LogPrintf("**inPackIsoFunc63 START**byTransType[%d]Sale[%d]Refund[%d]", srTransPara->byTransType,srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount, srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount);
    
    if(srTransPara->byTransType == SETTLE || srTransPara->byTransType == CLS_BATCH)
    {
        if((srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount) == 0 && (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usRefundCount) == 0)
        {
            strcpy(szPacket, "000000000000000");
            strcat(szPacket, "000000000000000");
            //strcat(szPacket, "000");
            inPacketCnt += 30;
			
        }else
        {
            /* SALE */
            memset(szTemplate, 0x00, sizeof(szTemplate));
            sprintf(szTemplate, "%03d", srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount);
            strcpy(szPacket, szTemplate);
            inPacketCnt += 3;
            sprintf(szTemplate, "%012ld", (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulSaleTotalAmount));
            strcat(szPacket, szTemplate);
            inPacketCnt += 12;
            /* REFUND */
            memset(szTemplate, 0x00, sizeof(szTemplate));
            sprintf(szTemplate, "%03d", srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usRefundCount);
            strcat(szPacket, szTemplate);
            inPacketCnt += 3;
            sprintf(szTemplate, "%012ld", (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulRefundTotalAmount));
            strcat(szPacket, szTemplate);
            inPacketCnt += 12;
            /*
            memset(szTemplate, 0x00, sizeof(szTemplate));
            sprintf(szTemplate, "%03d", srAccumRec.stBankTotal[inTranCardType].usEMVTCCount);
            strcat(&szPacket[inPacketCnt], szTemplate);
            inPacketCnt += 3;
               */
        }
    }
	else
	{
		if(srTransPara->fInstallment == TRUE)
		{
			memset(szPacket, 0, sizeof(szPacket));
			if(srTransPara->byPackType == BATCH_UPLOAD)
			{
                memcpy(&szPacket[inPacketCnt], "\x00\x29", 2); inPacketCnt+=2;
				memcpy(&szPacket[inPacketCnt], "\x35\x39", 2); inPacketCnt+=2;
				/*installment term*/		
				memset(szTemp, 0, sizeof(szTemp));
				sprintf(szTemp, "%03d", atol(srTransPara->szTerms));
				memcpy(&szPacket[inPacketCnt], szTemp, 3); inPacketCnt+=3;
				/*monthly amortization*/
				memset(szTemp, 0, sizeof(szTemp));
				sprintf(szTemp, "%012ld", atol(srTransPara->szMoAmort));
				memcpy(&szPacket[inPacketCnt], szTemp, 12); inPacketCnt+=12;
				/*monthly amortization*/
				memset(szTemp, 0, sizeof(szTemp));
				sprintf(szTemp, "%012ld", atol(srTransPara->szTotalInstAmnt));
				memcpy(&szPacket[inPacketCnt], szTemp, 12); inPacketCnt+=12;
			}
			else if(srTransPara->byTransType == SALE || srTransPara->byTransType == VOID)
			{				
                memcpy(&szPacket[inPacketCnt], "\x00\x05", 2); inPacketCnt+=2;
				memcpy(&szPacket[inPacketCnt], "\x35\x37", 2); inPacketCnt+=2;
				
				/*installment term*/		
				memset(szTemp, 0, sizeof(szTemp));
				sprintf(szTemp, "%03d", atol(srTransPara->szTerms));
				memcpy(&szPacket[inPacketCnt], szTemp, 3); inPacketCnt+=3;
			}
		}
	}
	
	/* Packet Data Length */
    memset(szAscBuf, 0, sizeof(szAscBuf));
    sprintf(szAscBuf, "%04d", inPacketCnt);
    memset(szBcdBuf, 0, sizeof(szBcdBuf));
    wub_str_2_hex(szAscBuf, szBcdBuf, 4);
    memcpy((char *)&uszSendData[inDataCnt], &szBcdBuf[0], 2);
    inDataCnt += 2;
    /* Packet Data */
    memcpy((char *)&uszSendData[inDataCnt], &szPacket[0], inPacketCnt);
    inDataCnt += inPacketCnt;
	
    vdDebug_LogPrintf(". Pack Len(%d)",inDataCnt);
    vdDebug_LogPrintf("**inPackIsoFunc63 END**");
    return inDataCnt;
	
}

int inPackIsoFunc64(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData)
{
    BYTE           szSHABinTemp[20];
    BYTE           szSHAAscTemp[50];
    BYTE           szTempMacResutl[16];
    SHA_CTX        stSHA;
    BYTE           szInitVictor[8];
    BYTE szClearTMK[33], szClearTAK[33], temp[17], ClearTMK[17];   
           
    inTLERead(1);
     
    vdMyEZLib_LogPrintf("**inPackIsoFunc64 START**");
    
    GET_KEY( szClearTMK , szClearTAK);

   
    vdMyEZLib_LogPrintf("**MAC DATA**");
    vdMyEZLib_LogPrintff(szDataForMAC,inMacMsgLen);
    
    CTOS_SHA1Init(&stSHA);   //Perform the SHA1 algorithm with the input data //                                                     
    CTOS_SHA1Update(&stSHA, szDataForMAC, inMacMsgLen);     //Finalize the SHA1 operation and retrun the result //                                                   
    CTOS_SHA1Final(szSHABinTemp,&stSHA);     //vdSHA1Generate(szDataForMAC, inMacMsgLen, szSHABinTemp); //wrong code          
    vdMyEZLib_LogPrintff(szSHABinTemp,20);   vdMyEZLib_LogPrintf("**SHA Generate**");    
    wub_hex_2_str(szSHABinTemp, szSHAAscTemp, 20);    
    memset(szInitVictor, 0x00, sizeof(szInitVictor));
    memset(szTempMacResutl, 0x00, sizeof(szTempMacResutl));
    
    if(srTransPara->byTransType == SIGN_ON)
    {
    	vdMyEZLib_LogPrintf("**SIGN ON CLEAR TAK**");
    	CTOS_MAC (szClearTAK, 16, szInitVictor, szSHAAscTemp, 40, szTempMacResutl);
    }    
    else 
    {
    	   //hard code key
	    memset( temp, 0x00, 17);	   
			vdMyEZLib_LogPrintf("**MAC KEY");	
		 	memcpy(szClearTAK,stTLE.szMACKey,16);	
    	CTOS_MAC (szClearTAK, 16, szInitVictor, szSHAAscTemp, 40, szTempMacResutl);
    }
    inDataCnt = 0;
    
    memcpy((char *)&uszSendData[inDataCnt], szTempMacResutl, 8);
    inDataCnt += 8;
    
    vdMyEZLib_LogPrintf("**inPackIsoFunc64 END**");
    return inDataCnt;
}

int inUnPackIsoFunc04(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
	if(srTransPara->byTransType == BAL_INQ)
	{
        memset(srTransPara->szTotalAmount, 0x00, sizeof(srTransPara->szTotalAmount));
        memcpy(srTransPara->szTotalAmount,(char *)uszUnPackBuf,12);
	}
	
    return ST_SUCCESS;	
}

int inUnPackIsoUnknown(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{	
    return ST_SUCCESS;	
}

int inUnPackIsoFunc11(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
	unsigned char szSTAN[6 + 1];
    
    memset(szSTAN, 0x00, sizeof(szSTAN));

	wub_hex_2_str(uszUnPackBuf, szSTAN, 3);
	
    srTransPara->ulTraceNum = atol(szSTAN);
	
	vdDebug_LogPrintf("inUnPackIsoFunc11(%s) [%d]", szSTAN,srTransPara->ulTraceNum);
    return ST_SUCCESS;
}

int inUnPackIsoFunc12(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    memcpy(srTransPara->szTime,(char *)uszUnPackBuf,TIME_BCD_SIZE);
    return ST_SUCCESS;
}

int inUnPackIsoFunc13(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    memcpy(srTransPara->szDate,(char *)uszUnPackBuf,DATE_BCD_SIZE);
    return ST_SUCCESS;
}

int inUnPackIsoFunc37(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    memcpy(srTransPara->szRRN, (char *)uszUnPackBuf, 12);
    return ST_SUCCESS;
}

int inUnPackIsoFunc38(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    memcpy(srTransPara->szAuthCode, (char *)uszUnPackBuf, AUTH_CODE_DIGITS);
    return ST_SUCCESS;
}

int inUnPackIsoFunc39(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    memcpy(srTransPara->szRespCode, (char *)uszUnPackBuf, RESP_CODE_SIZE);
	vdDebug_LogPrintf("inUnPackIsoFunc39(%s)", srTransPara->szRespCode);
    return ST_SUCCESS;
}

int inUnPackIsoFunc41(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    memcpy(srTransPara->szTID, (char *)uszUnPackBuf, TERMINAL_ID_BYTES);
	vdDebug_LogPrintf("inUnPackIsoFunc42(%s)", srTransPara->szTID);
    return ST_SUCCESS;
}

int inUnPackIsoFunc53(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
	if(srTransPara->byTransType == LOG_ON)
	{		
		unsigned char szPINKey[16 + 1];
		int inIndex;
        CTOS_RTC SetRTC;
        BYTE szCurrentTime[20];
        
		memset(szCurrentTime, 0, sizeof(szCurrentTime));
        memset(szPINKey, 0x00, sizeof(szPINKey));
        memcpy(szPINKey, uszUnPackBuf, 16);
	
        vdDebug_LogPrintf("inUnPackIsoFunc53(%s)", szPINKey);
        vdDebug_LogPrintf("srTransPara->HDTid(%d)", srTransPara->HDTid);
         
        for(inIndex = 0; inIndex < 16; inIndex++)
        {
            switch(szPINKey[inIndex])
            {
                case ':' :  
                    szPINKey[inIndex] = 'A'; 
                    break;
                case ';' :  
                    szPINKey[inIndex] = 'B'; 
                    break;
                case '<' :  
                    szPINKey[inIndex] = 'C'; 
                    break;
                case '=' :  
                    szPINKey[inIndex] = 'D'; 
                    break;
                case '>' :  
                    szPINKey[inIndex] = 'E'; 
                    break;
                case '?' :  
                    szPINKey[inIndex] = 'F'; 
                    break;
                default:   
                    break;			
            }
        }

        inDCTRead(srTransPara->HDTid);
        memcpy(strDCT.szPINKey, szPINKey, 16);
        inDCTSave(srTransPara->HDTid);
				 
		inTCTRead(1);
        CTOS_RTCGet(&SetRTC);
		memset(szCurrentTime, 0, sizeof(szCurrentTime));
        sprintf(szCurrentTime,"%02d%02d",SetRTC.bMonth, SetRTC.bDay);
        wub_str_2_hex(szCurrentTime,strTCT.szLogonDate,DATE_ASC_SIZE);
		
        inTCTSave(1);
	}
		
    return ST_SUCCESS;
}

int inUnPackIsoFunc55(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    int inLen, inTotalLen, inTagLen;
	unsigned short usTag;

    inLen =((uszUnPackBuf[0] / 16 * 10) + uszUnPackBuf[0] % 16) *100;
    inLen += (uszUnPackBuf[1] / 16 * 10) + uszUnPackBuf[1] % 16;

    
    vdDebug_LogPrintf("**inEDC_EMV_UnPackData55(%d) START** uszUnPackBuf[%02X %02X]", inLen, uszUnPackBuf[0], uszUnPackBuf[1]);
    DebugAddHEX("DE 55", uszUnPackBuf, inLen+2);

    if (inLen > 0)
    {
    	memset(srTransPara->stEMVinfo.T8A, 0x00, sizeof(srTransPara->stEMVinfo.T8A));
    	memcpy(srTransPara->stEMVinfo.T8A, srTransPara->szRespCode, strlen(srTransPara->szRespCode));

        for (inTotalLen = 2; inTotalLen < inLen;)
        {
            usTag = (unsigned short)uszUnPackBuf[inTotalLen] * 256;
    
    	    if ((uszUnPackBuf[inTotalLen ++] & 0x1F) == 0x1F)
                usTag += ((unsigned short)uszUnPackBuf[inTotalLen ++]);

            vdDebug_LogPrintf("usTag[%X]", usTag);
            switch (usTag)
            {
                case 0x9100 :
                    memset(srTransPara->stEMVinfo.T91, 0x00, sizeof(srTransPara->stEMVinfo.T91));
                    srTransPara->stEMVinfo.T91Len = (unsigned short)uszUnPackBuf[inTotalLen ++];
                    memcpy(srTransPara->stEMVinfo.T91, (char *)&uszUnPackBuf[inTotalLen], srTransPara->stEMVinfo.T91Len);
                    inTotalLen += srTransPara->stEMVinfo.T91Len;
                    vdDebug_LogPrintf(". 91Len(%d)",srTransPara->stEMVinfo.T91Len);
                    DebugAddHEX("Tag 91", srTransPara->stEMVinfo.T91, srTransPara->stEMVinfo.T91Len);
                    ushCTOS_EMV_NewTxnDataSet(TAG_91_ARPC, srTransPara->stEMVinfo.T91Len, srTransPara->stEMVinfo.T91);
                    break;
                case 0x7100 :
                    memset(srTransPara->stEMVinfo.T71, 0x00, sizeof(srTransPara->stEMVinfo.T71));
                    srTransPara->stEMVinfo.T71Len = (unsigned short)uszUnPackBuf[inTotalLen ++];
                    srTransPara->stEMVinfo.T71Len+=2;
                    memcpy(&srTransPara->stEMVinfo.T71[0], (char *)&uszUnPackBuf[inTotalLen-2], srTransPara->stEMVinfo.T71Len);
                    inTotalLen += srTransPara->stEMVinfo.T71Len-2;
                    vdDebug_LogPrintf(". 71Len(%d)",srTransPara->stEMVinfo.T71Len);
                    DebugAddHEX("Tag 71", srTransPara->stEMVinfo.T71, srTransPara->stEMVinfo.T71Len);
                    ushCTOS_EMV_NewTxnDataSet(TAG_71, srTransPara->stEMVinfo.T71Len, srTransPara->stEMVinfo.T71);
                    break;
                case 0x7200 :
                    memset(srTransPara->stEMVinfo.T72, 0x00, sizeof(srTransPara->stEMVinfo.T72));
                    srTransPara->stEMVinfo.T72Len = (unsigned short)uszUnPackBuf[inTotalLen ++];
                    srTransPara->stEMVinfo.T72Len+=2;
                    memcpy(&srTransPara->stEMVinfo.T72[0], (char *)&uszUnPackBuf[inTotalLen-2], srTransPara->stEMVinfo.T72Len);
                    inTotalLen += srTransPara->stEMVinfo.T72Len-2;
                    vdDebug_LogPrintf(". 72Len(%d)",srTransPara->stEMVinfo.T72Len);
                    DebugAddHEX("Tag 72", srTransPara->stEMVinfo.T72, srTransPara->stEMVinfo.T72Len);
                    ushCTOS_EMV_NewTxnDataSet(TAG_72, srTransPara->stEMVinfo.T72Len, srTransPara->stEMVinfo.T72);
                    break;
                 default :
                    vdDebug_LogPrintf("**inEDC_EMV_UnPackData55(%X) Err**", usTag);
                    inTagLen = (unsigned short)uszUnPackBuf[inTotalLen ++];
                    inTotalLen += inTagLen;
                    vdDebug_LogPrintf("**inTagLen(%d) inTotalLen[%d] Err**", inTagLen, inTotalLen);
                    break;
    	    }
    	}    
    }
    else
    {
        inCTOS_inDisconnect();
        return (ST_ERROR);
    }

    return ST_SUCCESS;
}

int inUnPackIsoFunc57(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    int        inIndex = 0;
    BYTE       szTmp[32];
    int        inLen;
		BYTE szClearTMK[17] , szClearTAK[17];

    if(srTransPara->byTransType != SIGN_ON)
    {
        return ST_SUCCESS;
    }
    
    inLen = ((uszUnPackBuf[0] / 16 * 10) + uszUnPackBuf[0] % 16) * 100;
    inLen += (uszUnPackBuf[1] / 16 * 10) + uszUnPackBuf[1] % 16;
    vdMyEZLib_LogPrintf("**inEDC_EMV_UnPackData57(%d) START**", inLen);
    inIndex = 2;
    
    inIndex += 2;//skip version
    inIndex += 8;//skip tmk refercence
		
	inTLERead(1);
		
    if (inLen > 10)
    {
        memset(szTmp, 0x00, sizeof(szTmp));
        GET_KEY( szClearTMK , szClearTAK);        
        
        Decrypt3Des(&uszUnPackBuf[inIndex], szClearTMK, szTmp);
        memcpy(stTLE.szTermPinEncryuptKey, szTmp, 8);
        Decrypt3Des(&uszUnPackBuf[inIndex+8], szClearTMK, szTmp);
        memcpy(&stTLE.szTermPinEncryuptKey[8], szTmp, 8);
        inIndex = inIndex +16;//TPK
        
        memset(szTmp, 0x00, sizeof(szTmp));
                
        vdMyEZLib_LogPrintff(&uszUnPackBuf[inIndex],16);
                
        Decrypt3Des(&uszUnPackBuf[inIndex], szClearTMK, szTmp);
        memcpy(stTLE.szMACKey, szTmp, 8);
        Decrypt3Des(&uszUnPackBuf[inIndex+8], szClearTMK, szTmp); 
        memcpy(&stTLE.szMACKey[8], szTmp, 8);       
        inIndex = inIndex +16;//TPK
        
        vdMyEZLib_LogPrintf("szCLEar MAC**");
	
       
        vdMyEZLib_LogPrintff(&uszUnPackBuf[inIndex],16); 
        memset(szTmp, 0x00, sizeof(szTmp));
        Decrypt3Des(&uszUnPackBuf[inIndex], szClearTMK, szTmp);
        memcpy(stTLE.szLineEncryptKey, szTmp, 8);
        Decrypt3Des(&uszUnPackBuf[inIndex+8], szClearTMK, szTmp);  
        memcpy(&stTLE.szLineEncryptKey[8], szTmp, 8);
        inIndex = inIndex +16;//TPK
        
        vdMyEZLib_LogPrintf("szCLEar TPK**");
    }
    else
    {
        inCTOS_inDisconnect();
        return (ST_ERROR);
    }
    
    inTLESave(1);
    vdMyEZLib_LogPrintf("**inEDC_EMV_UnPackData57(%d) END**", inLen);
    return ST_SUCCESS;
}

int inUnPackIsoInstallment61(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    int inIndex=2, inLen=0;

    if(srTransPara->byPackType == SALE && srTransPara->fInstallment == TRUE)
    {
        inLen = ((uszUnPackBuf[0] / 16 * 10) + uszUnPackBuf[0] % 16) * 100;
        inLen += (uszUnPackBuf[1] / 16 * 10) + uszUnPackBuf[1] % 16;
        
        if(inLen<81)
            return (ST_ERROR);
        
		// Parse ISO
		if(strTCT.fPrintISOMessage == TRUE)
			vdPrintISOOption("inUnPackIsoInstallment61",uszUnPackBuf, 81+2);
      
        memset(srTransPara->szInstallmentData, 0, sizeof(srTransPara->szInstallmentData));
        memcpy(srTransPara->szInstallmentData, uszUnPackBuf+inIndex, 81);
        
        memcpy(srTransPara->szPromo, (char *)uszUnPackBuf+inIndex, 3); inIndex+=3;		
        memcpy(srTransPara->szTerms, (char *)uszUnPackBuf+inIndex, 2); inIndex+=2;
        inIndex+=1; /*skip compute method*/
        memcpy(srTransPara->szFactorRate, (char *)uszUnPackBuf+inIndex, 6); inIndex+=6;
        inIndex+=2; /*interest free months - 2 bytes*/ 
        inIndex+=9; /*first pay - 9 bytes*/
        inIndex+=9; /*last pay - 9 bytes*/ 
        memcpy(srTransRec.szMoAmort, (char *)uszUnPackBuf+inIndex, 9); inIndex+=9;
        memcpy(srTransRec.szTotalInstAmnt, (char *)uszUnPackBuf+inIndex, 9); inIndex+=9;
    }
	
    return ST_SUCCESS;
}

/*Diners Installment - DE63*/
int inUnPackIsoFunc63(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
    int inIndex=2, inLen=0;

    if(srTransPara->byPackType == SALE && srTransPara->fInstallment == TRUE)
    {
        inLen = ((uszUnPackBuf[0] / 16 * 10) + uszUnPackBuf[0] % 16) * 100;
        inLen += (uszUnPackBuf[1] / 16 * 10) + uszUnPackBuf[1] % 16;
        
        if(inLen<28)
            return (ST_ERROR);
		
        // Parse ISO
		if(strTCT.fPrintISOMessage == TRUE)
			vdPrintISOOption("inUnPackIsoFunc63",uszUnPackBuf, 28+2);
        
        memset(srTransPara->szInstallmentData, 0, sizeof(srTransPara->szInstallmentData));
        memcpy(srTransPara->szInstallmentData, uszUnPackBuf+inIndex, 28);
        
        memset(srTransPara->szMoAmort, 0, sizeof(srTransPara->szMoAmort));
        memset(srTransPara->szTotalInstAmnt, 0, sizeof(srTransPara->szTotalInstAmnt));
        
        memcpy(srTransPara->szMoAmort, (char *)uszUnPackBuf+(16+2), 12);
        memcpy(srTransPara->szTotalInstAmnt, (char *)uszUnPackBuf+(4+2), 12);
    }
	
    return ST_SUCCESS;
}

int inPackISOEMVData(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
     
	int inPacketCnt = 0, inTagLen;
	unsigned short usLen, usRetVal;
	BYTE btTrack2[20];
	char szAscBuf[4 + 1], szBcdBuf[2 + 1];
	char szPacket[512 + 1];
    char szTmp[32+1];
	USHORT ushEMVtagLen = 0;
	BYTE   EMVtagVal[64];
	int inRet;

    
	DebugAddSTR("load f55","emv",2);	

	vdMyEZLib_LogPrintf("**inPackISOEMVData START**");
	memset(szPacket, 0, sizeof(szPacket));
	inDataCnt = 0;

    {	
	   
        szPacket[inPacketCnt ++] = 0x5F;
        szPacket[inPacketCnt ++] = 0x2A;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T5F2A, 2);
        inPacketCnt += 2;
        

        DebugAddINT("5F34 Len",srTransPara->stEMVinfo.T5F34_len );
        if(srTransPara->stEMVinfo.T5F34_len > 0)
        {
	        szPacket[inPacketCnt ++] = 0x5F;
	        szPacket[inPacketCnt ++] = 0x34;
	        szPacket[inPacketCnt ++] = 1;
	        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T5F34;
			DebugAddSTR("EMV tag","5f34--finish--",2);
        }

        
        szPacket[inPacketCnt ++] = 0x82;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T82, 2);
        inPacketCnt += 2;

        
        szPacket[inPacketCnt ++] = 0x84;		
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T84_len;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T84, srTransPara->stEMVinfo.T84_len);
        inPacketCnt += srTransPara->stEMVinfo.T84_len;

       
        szPacket[inPacketCnt ++] = 0x95;
        szPacket[inPacketCnt ++] = 5;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T95, 5);
        inPacketCnt += 5;

        
        szPacket[inPacketCnt ++] = 0x9A;
        szPacket[inPacketCnt ++] = 3;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9A, 3);
        inPacketCnt += 3;

        
        szPacket[inPacketCnt ++] = 0x9C;
        szPacket[inPacketCnt ++] = 1;
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9C;  // SL check again  //spec said 2 byte

       
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x02;
        szPacket[inPacketCnt ++] = 6;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F02, 6);
        inPacketCnt += 6;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x03;
        szPacket[inPacketCnt ++] = 6;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F03, 6);
        inPacketCnt += 6;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x09;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F09, 2);
        inPacketCnt += 2;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x10;
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9F10_len;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F10, srTransPara->stEMVinfo.T9F10_len);
        inPacketCnt += srTransPara->stEMVinfo.T9F10_len;
        
        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x1A;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F1A, 2);
        inPacketCnt += 2;
        
        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x1E;
        szPacket[inPacketCnt ++] = 8;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F1E, 8);
        inPacketCnt += 8;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x26;
        szPacket[inPacketCnt ++] = 8;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F26, 8);
        inPacketCnt += 8;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x27;
        szPacket[inPacketCnt ++] = 1;
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9F27;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x33;
        szPacket[inPacketCnt ++] = 3;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F33, 3);
        inPacketCnt += 3;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x34;
        szPacket[inPacketCnt ++] = 3;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F34, 3);
        inPacketCnt += 3;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x35;
        szPacket[inPacketCnt ++] = 1;
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9F35;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x36;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F36, 2);
        inPacketCnt += 2;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x37;
        szPacket[inPacketCnt ++] = 4;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F37, 4);
        inPacketCnt += 4;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x41;
        szPacket[inPacketCnt ++] = 3;

        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F41, 3);// get chip transaction counter
        inPacketCnt += 3;        
        					
    }
	
    /* Packet Data Length */
    memset(szAscBuf, 0, sizeof(szAscBuf));
    sprintf(szAscBuf, "%04d", inPacketCnt);
    memset(szBcdBuf, 0, sizeof(szBcdBuf));
    wub_str_2_hex(szAscBuf, szBcdBuf, 4);
    memcpy((char *)&uszUnPackBuf[inDataCnt], &szBcdBuf[0], 2);
    inDataCnt += 2;
    /* Packet Data */
    memcpy((char *)&uszUnPackBuf[inDataCnt], &szPacket[0], inPacketCnt);
    inDataCnt += inPacketCnt;
    
    vdMyEZLib_LogPrintf(". Pack Len(%d)",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackISOEMVData END**");
    return (inDataCnt);
}


int inPackISOPayWaveData(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{
     
	int inPacketCnt = 0, inTagLen;
	unsigned short usLen, usRetVal;
	BYTE btTrack2[20];
	char szAscBuf[4 + 1], szBcdBuf[2 + 1];
	char szPacket[512 + 1];
    char szTmp[32+1];
	USHORT ushEMVtagLen = 0;
	BYTE   EMVtagVal[64];
	int inRet;

    
	DebugAddSTR("load f55","emv",2);	

	vdMyEZLib_LogPrintf("**inPackISOPayWaveData START**");
	memset(szPacket, 0, sizeof(szPacket));
	inDataCnt = 0;

    {	
	    szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x02;
        szPacket[inPacketCnt ++] = 6;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F02, 6);
        inPacketCnt += 6;

        
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x03;
        szPacket[inPacketCnt ++] = 6;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F03, 6);
        inPacketCnt += 6;

		szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x26;
        szPacket[inPacketCnt ++] = 8;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F26, 8);
        inPacketCnt += 8;
		
        szPacket[inPacketCnt ++] = 0x82;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T82, 2);
        inPacketCnt += 2;
	
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x36;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F36, 2);
        inPacketCnt += 2;
		
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x10;
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9F10_len;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F10, srTransPara->stEMVinfo.T9F10_len);
        inPacketCnt += srTransPara->stEMVinfo.T9F10_len;
		
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x33;
        szPacket[inPacketCnt ++] = 3;
        memcpy(&szPacket[inPacketCnt], "\xE0\xB0\xC8", 3);
        inPacketCnt += 3;
		
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x1A;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F1A, 2);
        inPacketCnt += 2;

        szPacket[inPacketCnt ++] = 0x95;
        szPacket[inPacketCnt ++] = 5;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T95, 5);
        inPacketCnt += 5;

        szPacket[inPacketCnt ++] = 0x5F;
        szPacket[inPacketCnt ++] = 0x2A;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T5F2A, 2);
        inPacketCnt += 2;

        szPacket[inPacketCnt ++] = 0x9A;
        szPacket[inPacketCnt ++] = 3;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9A, 3);
        inPacketCnt += 3;

        szPacket[inPacketCnt ++] = 0x9C;
        szPacket[inPacketCnt ++] = 1;
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9C;  // SL check again  //spec said 2 byte

        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x37;
        szPacket[inPacketCnt ++] = 4;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F37, 4);
        inPacketCnt += 4;
    }
	
    /* Packet Data Length */
    memset(szAscBuf, 0, sizeof(szAscBuf));
    sprintf(szAscBuf, "%04d", inPacketCnt);
    memset(szBcdBuf, 0, sizeof(szBcdBuf));
    wub_str_2_hex(szAscBuf, szBcdBuf, 4);
    memcpy((char *)&uszUnPackBuf[inDataCnt], &szBcdBuf[0], 2);
    inDataCnt += 2;
    /* Packet Data */
    memcpy((char *)&uszUnPackBuf[inDataCnt], &szPacket[0], inPacketCnt);
    inDataCnt += inPacketCnt;
    
    vdMyEZLib_LogPrintf(". Pack Len(%d)",inDataCnt);
    vdMyEZLib_LogPrintf("**inPackISOPayWaveData END**");
    return (inDataCnt);
}


int inPackISOPayPassData(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf)
{ 
	int inPacketCnt = 0, inTagLen;
	unsigned short usLen, usRetVal;
	BYTE btTrack2[20];
	char szAscBuf[4 + 1], szBcdBuf[2 + 1];
	char szPacket[512 + 1];
	char szTmp[32+1];
	USHORT ushEMVtagLen = 0;
	BYTE   EMVtagVal[64];
	int inRet;

	
	DebugAddSTR("load f55","emv",2);	

	vdMyEZLib_LogPrintf("**inPackISOPayWaveData START**");
	memset(szPacket, 0, sizeof(szPacket));
	inDataCnt = 0;

	{
		szPacket[inPacketCnt ++] = 0x9F;
		szPacket[inPacketCnt ++] = 0x26;
		szPacket[inPacketCnt ++] = 8;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F26, 8);
		inPacketCnt += 8;
	
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x27;
        szPacket[inPacketCnt ++] = 1;
        szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9F27;
		
		szPacket[inPacketCnt ++] = 0x9F;
		szPacket[inPacketCnt ++] = 0x10;
		szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9F10_len;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F10, srTransPara->stEMVinfo.T9F10_len);
		inPacketCnt += srTransPara->stEMVinfo.T9F10_len;

        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x34;
        szPacket[inPacketCnt ++] = 3;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F34, 3);
        inPacketCnt += 3;
	
		szPacket[inPacketCnt ++] = 0x9F;
		szPacket[inPacketCnt ++] = 0x37;
		szPacket[inPacketCnt ++] = 4;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F37, 4);
		inPacketCnt += 4;

        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x36;
        szPacket[inPacketCnt ++] = 2;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F36, 2);
        inPacketCnt += 2;
		
		szPacket[inPacketCnt ++] = 0x95;
		szPacket[inPacketCnt ++] = 5;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T95, 5);
		inPacketCnt += 5;

		szPacket[inPacketCnt ++] = 0x9A;
		szPacket[inPacketCnt ++] = 3;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9A, 3);
		inPacketCnt += 3;

		szPacket[inPacketCnt ++] = 0x9C;
		szPacket[inPacketCnt ++] = 1;
		szPacket[inPacketCnt ++] = srTransPara->stEMVinfo.T9C;	// SL check again  //spec said 2 byte

        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x02;
        szPacket[inPacketCnt ++] = 6;
        memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F02, 6);
        inPacketCnt += 6;

		szPacket[inPacketCnt ++] = 0x5F;
		szPacket[inPacketCnt ++] = 0x2A;
		szPacket[inPacketCnt ++] = 2;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T5F2A, 2);
		inPacketCnt += 2;

		szPacket[inPacketCnt ++] = 0x82;
		szPacket[inPacketCnt ++] = 2;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T82, 2);
		inPacketCnt += 2;
		
		szPacket[inPacketCnt ++] = 0x9F;
		szPacket[inPacketCnt ++] = 0x1A;
		szPacket[inPacketCnt ++] = 2;
		memcpy(&szPacket[inPacketCnt], srTransPara->stEMVinfo.T9F1A, 2);
		inPacketCnt += 2;
		
		szPacket[inPacketCnt ++] = 0x9F;
		szPacket[inPacketCnt ++] = 0x33;
		szPacket[inPacketCnt ++] = 3;
		memcpy(&szPacket[inPacketCnt], "\xE0\xB0\xC8", 3);
		inPacketCnt += 3;
		
		
        szPacket[inPacketCnt ++] = 0x9F;
        szPacket[inPacketCnt ++] = 0x03;
        szPacket[inPacketCnt ++] = 6;
        memcpy(&szPacket[inPacketCnt], "\x00\x00\x00\x00\x00\x00", 6);
        inPacketCnt += 6;
	}
	
	/* Packet Data Length */
	memset(szAscBuf, 0, sizeof(szAscBuf));
	sprintf(szAscBuf, "%04d", inPacketCnt);
	memset(szBcdBuf, 0, sizeof(szBcdBuf));
	wub_str_2_hex(szAscBuf, szBcdBuf, 4);
	memcpy((char *)&uszUnPackBuf[inDataCnt], &szBcdBuf[0], 2);
	inDataCnt += 2;
	/* Packet Data */
	memcpy((char *)&uszUnPackBuf[inDataCnt], &szPacket[0], inPacketCnt);
	inDataCnt += inPacketCnt;
	
	vdMyEZLib_LogPrintf(". Pack Len(%d)",inDataCnt);
	vdMyEZLib_LogPrintf("**inPackISOPayWaveData END**");
	return (inDataCnt);
}



int inCTOS_PackDemoResonse(TRANS_DATA_TABLE *srTransPara,unsigned char *uszRecData)
{
    int inPackLen;
    BYTE szSTAN[6+1];
    BYTE szTID[TERMINAL_ID_BYTES+1];
    BYTE szMTI[MTI_BCD_SIZE+1];
    BYTE szBitMap[8+1];
    BYTE szCurrentTime[20];
    CTOS_RTC SetRTC;

    //default response turn on 3, 11, 12, 13, 24, 37, 38, 39, 41
    memset(szBitMap, 0x00, sizeof(szBitMap));
    memcpy(szBitMap, "\x20\x38\x01\x00\x0E\x80\x00\x00", 8);

    inPackLen = 0;
    
    memcpy(&uszRecData[inPackLen], "\x60\x00\x01\x00\x00",TPDU_BCD_SIZE);
    inPackLen += TPDU_BCD_SIZE;

    memcpy(szMTI, "\x02\x10", MTI_BCD_SIZE);
    szMTI[1] |= 0x10;
    memcpy(&uszRecData[inPackLen], szMTI, MTI_BCD_SIZE);
    inPackLen += MTI_BCD_SIZE;

    memcpy(&uszRecData[inPackLen], szBitMap, 8);
    inPackLen += 8;

    //DE 3
    memcpy(&uszRecData[inPackLen], "\x00\x00\x00", PRO_CODE_BCD_SIZE);
    inPackLen += PRO_CODE_BCD_SIZE;

    //DE 11
    sprintf(szSTAN, "%06ld", srTransPara->ulTraceNum);
    wub_str_2_hex(&szSTAN[0], (char *)&uszRecData[inPackLen], 6);
    inPackLen += 3;

    //DE 12
    CTOS_RTCGet(&SetRTC);
    memset(szCurrentTime, 0x00, sizeof(szCurrentTime));
    sprintf(szCurrentTime,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);
    wub_str_2_hex(&szCurrentTime[0], (char *)&uszRecData[inPackLen], 6);
    inPackLen += 3;

    //DE 13
    memset(szCurrentTime, 0x00, sizeof(szCurrentTime));
    sprintf(szCurrentTime,"%02d%02d",SetRTC.bMonth,SetRTC.bDay);
    wub_str_2_hex(&szCurrentTime[0], (char *)&uszRecData[inPackLen], 4);
    inPackLen += 2;

    //DE 24
    memcpy((char *)&uszRecData[inPackLen], strHDT.szNII, 2);
    inPackLen += 2;

    //DE 37
    memcpy((char *)&uszRecData[inPackLen], "111111111111", 12);
    inPackLen += 12;

    //DE 38
    if(0 == strlen(srTransPara->szAuthCode))
        memcpy((char *)&uszRecData[inPackLen], "123456", 6);
    else
        memcpy((char *)&uszRecData[inPackLen], srTransPara->szAuthCode, 6);
    inPackLen += 6;

    //DE 39
    memcpy((char *)&uszRecData[inPackLen], "00", 2);
    inPackLen += 2;

    //DE 41
    memset(szTID, 0x00, sizeof(szTID));
    memset(szTID, 0x20, TERMINAL_ID_BYTES);
    memcpy(szTID, srTransPara->szTID, strlen(srTransPara->szTID));
    memcpy((char *)&uszRecData[inPackLen], szTID, TERMINAL_ID_BYTES);
    inPackLen += TERMINAL_ID_BYTES;
    
    return inPackLen;
}

void vdInitialISOFunction(ISO_FUNC_TABLE *srPackFunc)
{
	/* Choose ISO_FUNC_TABLE Array */
	memcpy((char *)srPackFunc, (char *)&srIsoFuncTable[0], sizeof(ISO_FUNC_TABLE));
}

