//
// Created by patri on 6/3/2018.
//

#ifndef S1_MCCPAY_PATRICK_LIB_H
#define S1_MCCPAY_PATRICK_LIB_H

#include <jni.h>
#include <string.h>
//#include "../Includes/ECRTypedef.h"

char * get_dog(void);
//int inPackMessageIdData(int inTransCode, unsigned char *uszPackData, char *szMTI);
//int inPackPCodeData(int inTransCode, unsigned char *uszPackData, char *szPCode);
//void vdModifyBitMapFunc(int inTransCode, int *inBitMap);
//int inCheckIsoHeaderData(char *szSendISOHeader, char *szReceISOHeader);
//int inAnalyseReceiveData(void);
int inAnalyseAdviceData(int inPackType);
int inExeFunction_PackISO(char *INuszFunctionName, unsigned char *uszSendData);
int inExeFunction_UnPackISO(char *INuszFunctionName, unsigned char *uszReceiveData);
//int inPackIsoFunc03(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
//int inPackIsoFunc04(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inCallJAVA_DisplayMultipleMessage(char *pszMsg);
//int inCallJAVA_MenuTransactions(BYTE *pbDispMsg, BYTE *pbOutStr, BYTE *pbOutLen);


#endif //S1_MCCPAY_PATRICK_LIB_H


