#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>
#include "../Includes/POSbatch.h"
#include "../Includes/POSTypedef.h"

#include "../FileModule/myFileFunc.h"
#include "../POWRFAIL/POSPOWRFAIL.h"
#include "..\debug\debug.h"
#include "..\Database\DatabaseFunc.h"
#include "..\Includes\myEZLib.h"
#include "..\ui\Display.h"
#include "..\Includes\POSHost.h"
#include "..\debug\debug.h"


int inCTOS_SaveBatchTxn(void)
{
    int inResult ;
    int inSaveType;
	
	BYTE szMessage1[40+1]={0};
	BYTE szMessage2[40+1]={0};
	BYTE szMessage3[40+1]={0};
	BYTE szMessage4[40+1]={0};

	vdSaveLastInvoiceNo();

    vdDebug_LogPrintf("SATURN inCTOS_SaveBatchTxn %d", srTransRec.byTransType);
	inCTLOS_Updatepowrfail(PFR_BEGIN_BATCH_UPDATE);

    
    vdDebug_LogPrintf("saturn inCTOS_SaveBatchTxn byTransType[%d].byOffline[%d].byUploaded[%d]", srTransRec.byTransType, srTransRec.byOffline, srTransRec.byUploaded);
    if(srTransRec.byTransType == VOID && srTransRec.byOffline == CN_TRUE)
    {
        //OFFLINE SALE, then VOID it, the VOID is offline, should delete the OFFLINE SALE Advice
        if(srTransRec.byUploaded == CN_FALSE)
        {
            inCTOSS_DeleteAdviceByINV(srTransRec.szInvoiceNo);
        }
    }

	// Get ECR Message from ENV -- sidumili
	if (srTransRec.fECRTxnFlg == TRUE)
	{
		memset(szMessage1, 0x00, sizeof(szMessage1));
		memset(szMessage2, 0x00, sizeof(szMessage2));
		memset(szMessage3, 0x00, sizeof(szMessage3));
		memset(szMessage4, 0x00, sizeof(szMessage4));
		
		inCTOSS_GetEnvDB("MSG1", szMessage1);
		//strcpy(srTransRec.szECRMessage1, szMessage1);

		inCTOSS_GetEnvDB("MSG2", szMessage2);
		//strcpy(srTransRec.szECRMessage2, szMessage2);

		inCTOSS_GetEnvDB("MSG3", szMessage3);
		//strcpy(srTransRec.szECRMessage3, szMessage3);

		inCTOSS_GetEnvDB("MSG4", szMessage4);
		//trcpy(srTransRec.szECRMessage4, szMessage4);
	}

    //SetTransYear(); // Set Trans Year -- sidumili
	
    if((srTransRec.byTransType == SALE) || (srTransRec.byTransType == REFUND) || (srTransRec.byTransType == SALE_OFFLINE) || (srTransRec.byTransType == PRE_AUTH)
	|| (srTransRec.byTransType == LOY_REDEEM_5050) || (srTransRec.byTransType == LOY_REDEEM_VARIABLE))
    {
		if (srTransRec.byTransType != PRE_AUTH){
	    	strMMT[0].fBatchNotEmpty = TRUE;
			inMMTBatchNotEmptySave(strMMT[0].MMTid);
        }
		
        inSaveType = DF_BATCH_APPEND;

        //Should be Online void the Intial SALE amount.
        if(srTransRec.byOffline == CN_FALSE)
        {
            //use szStoreID to store how much amount fill up in DE4 for VOID
            //either enter tip or not enter tip are ok
            memcpy(srTransRec.szStoreID, srTransRec.szTotalAmount, 6);
        }
    }
    else
    {
        inSaveType = DF_BATCH_UPDATE;
    }
    
    if(srTransRec.byTransType == VOID)
        srTransRec.byVoided = TRUE;


	vdDebug_LogPrintf("SATURN inCTOS_SaveBatchTxn 2 %d", srTransRec.byVoided);
	inResult = inDatabase_BatchSave(&srTransRec, inSaveType);
    
    if(inResult != ST_SUCCESS)
    {
    	vdSetErrorMessage("Batch Save Error");
        return ST_ERROR;
    }
            
    if((memcmp(srTransRec.szAuthCode, "Y3",2))  && 
       (srTransRec.byTransType != SALE_OFFLINE) &&
       (srTransRec.byTransType != SALE_ADJUST)  &&
       (srTransRec.byTransType != SALE_TIP) &&
       ( srTransRec.byOffline != VS_TRUE))
    {
        inMyFile_ReversalDelete();
    }
        
    if((srTransRec.byTransType == SALE_TIP) || (srTransRec.byTransType == SALE_ADJUST) || (srTransRec.byTransType == SALE_OFFLINE))
    {
        //fix send tip adjust advie two times issue
        inCTOSS_DeleteAdviceByINV(srTransRec.szInvoiceNo);
        
        inMyFile_AdviceSave(&srTransRec, srTransRec.byPackType);
    }
    //if original txn is offline, then do a online txn piggy back the offline advice, the offline advice been deleted, then void it, the void is offline, need to create advice for this offline void
    else if(srTransRec.byTransType == VOID && srTransRec.byOffline == CN_TRUE && srTransRec.byUploaded == CN_TRUE)
    {
        vdDebug_LogPrintf("Create Advice for offline VOID");
        inMyFile_AdviceSave(&srTransRec, OFFLINE_VOID);
    }
    else if((srTransRec.byTransType == SALE) && (srTransRec.byEntryMode == CARD_ENTRY_ICC))
    {
       if((srTransRec.byTCuploaded == 0) ||
          (!memcmp(srTransRec.szAuthCode, "Y1",2)) || 
          (!memcmp(srTransRec.szAuthCode, "Y3",2)) 
         )
       {
           if((memcmp(srTransRec.szAuthCode, "Y1",2)) && 
              (memcmp(srTransRec.szAuthCode, "Y3",2)) 
             )
           {
               srTransRec.byPackType = TC_UPLOAD;
               inMyFile_TCUploadFileSave(&srTransRec, srTransRec.byPackType);
           }
           else
           {
               srTransRec.byPackType = SEND_ADVICE;
               inMyFile_AdviceSave(&srTransRec, srTransRec.byPackType);
           }
       }
    }
    else if((/*srTransRec.byTransType == REFUND || */srTransRec.byTransType == PRE_AUTH) && (srTransRec.byEntryMode == CARD_ENTRY_ICC))
    {
       
        srTransRec.byPackType = TC_UPLOAD;
        inMyFile_TCUploadFileSave(&srTransRec, srTransRec.byPackType);
           
    }

    return d_OK;
}



int inCTOS_BatchReviewFlow(void)
{
    int inResult;
    int inReadRecResult;
 	int inCount = 0;
	int inBatchRecordNum = 0;
    BYTE key;
	char szPanDisplay[24+1];
    BYTE szTemp1[30+1];
	int  *pinTransDataid = NULL;
	int inNavRecType = 0; // sidumili: nagivation record type

	/*
	inNavRecType:
	0 - Beginning
	1 - First of Record
	2 - End of Record
	*/

	vdDebug_LogPrintf("SATURN inCTOS_BatchReviewFlow");

	usCTOSS_LCDDisplay(" "); // sidumili: Remove stuck image eg. "Processing"
	
	inBatchRecordNum = inBatchNumRecord();

	pinTransDataid = (int*)malloc(inBatchRecordNum * sizeof(int));
	
	inBatchByMerchandHost(inBatchRecordNum, srTransRec.HDTid, srTransRec.MITid, srTransRec.szBatchNo, pinTransDataid);

	inResult = inDatabase_BatchReadByTransId(&srTransRec, pinTransDataid[inCount]);
	
	if(inResult == d_NO)
	{
	    //vdDisplayErrorMsg(1, 8, "BATCH EMPTY");
		
		//usCTOSS_LCDDisplay("BATCH REVIEW|BATCH EMPTY");
		//CTOS_Beep();
		//CTOS_Delay(1000);
		vdDisplayErrorMsg(1, 8, "BATCH EMPTY");
		usCTOSS_LCDDisplay(" ");
		free(pinTransDataid);
		free(pinTransDataid);
	    return d_OK;
	}

    key = inCTOS_DisplayBatchRecordDetail(BATCH_REVIEW, inNavRecType);
    
    while(1)
	{ 
		inNavRecType = 0;
		
		//CTOS_KBDHit(&key); 
		if (key == d_KBD_CANCEL || key == 0xFF)
		{ 
			free(pinTransDataid);
		    return d_NO; 
		} 
		else if ((key == d_KBD_DOWN) || (key == d_KBD_DOT))
	    {
           inCount++;     
		   		   
		   DebugAddIntX("inCount", inCount);

		    if(inCount >= inBatchRecordNum)
		   {
				//usCTOSS_LCDDisplay("BATCH REVIEW|END OF RECORD");
				CTOS_Beep();
				//CTOS_Delay(1000);
				//usCTOSS_LCDDisplay(" ");
				inCount = inBatchRecordNum - 1;
				inNavRecType = 2;
		   }
		  
		   inResult = inDatabase_BatchReadByTransId(&srTransRec, pinTransDataid[inCount]);
		   
		   key = inCTOS_DisplayBatchRecordDetail(BATCH_REVIEW, inNavRecType);	
		} 
        else if ((key == d_KBD_UP) || (key == d_KBD_00))
        { 
            inCount--;

            inResult = inDatabase_BatchReadByTransId(&srTransRec, pinTransDataid[inCount]);

            if(inResult == d_NO)
            {
                //usCTOSS_LCDDisplay("BATCH REVIEW|FIRST OF RECORD");
				CTOS_Beep();
				//CTOS_Delay(1000); 
				//usCTOSS_LCDDisplay(" ");
                inCount = 0;
                inResult = inDatabase_BatchReadByTransId(&srTransRec, pinTransDataid[inCount]);
				inNavRecType = 1;
            }

            key = inCTOS_DisplayBatchRecordDetail(BATCH_REVIEW, inNavRecType);
		} 
    } 	
    
	free(pinTransDataid);
    return d_OK;
    
}


