/********************************************************************
 Copyright (c) Castles Technology Co., Ltd. 2006~
Module Name: iccmifare.h
 --------------------------------------------------------------------
 v1.6		2007.07.24	Add ICC SAM & Mifare related functions to iccmifare.c 
            2007.07.25	Add new baudrate index 0x05(BR:9600) in VW_SetBaudRate()
 v1.7		2007.10.29  Modify VW_GetCAPublicKey() and VW_GetJCBPublicKey()
 v2.0		2008.02.21	Add some functions for DLE protocol with payPass transaction use 
                        Modify coding in VisaWave related functions
            2008.03.18	Remove d_SAM_xxxxx definition
                        Remove baudrate index 0x05(BR:9600) from VW_SetBaudRate()
            2008.04.14  Modify return code value of d_HOST_ID_INCORRECT  
 v2.5		2011.03.22	Add a new function VW_MifareAuthEx()
 v2.8		2011.12.07	Add VW_CLPolling and VW_CLActivation function
 v2.9       2011.12.22  Add VW_FelicaPoll and VW_FelicaRW functions
*********************************************************************/
#ifndef ICC_MIFARE_H
#define ICC_MIFARE_H

#ifdef __cplusplus
extern "C" 
{
#endif

ULONG 	VW_ICCheckInsert(BYTE bSocketID);
ULONG 	VW_ICCPowerOn(BYTE bSocketID,BYTE bVoltage,BYTE bEMV,BYTE bPTS,BYTE bIFSD,BYTE *baATR,UINT *uiRLen);
ULONG 	VW_ICCPowerOff(BYTE bSocketID);
ULONG 	VW_ICCSendAPDU(BYTE bSocketID,BYTE *baAPDU,UINT uiSLen,BYTE *baRBuf,UINT *uiRLen);
ULONG 	VW_CLPolling(USHORT usTimeout,BYTE *baCSN,BYTE *bCSNLen);
ULONG 	VW_CLActivation(BYTE *baCSN,BYTE bCSNLen);
ULONG 	VW_PCDInit(void);
ULONG	VW_PCDPowerOn(void);
ULONG	VW_PCDPowerOff(void);
ULONG	VW_MifareActiveHalt(BYTE bBaudRate,BYTE *baCSN,BYTE bCSN_Length,BYTE *baATQA,BYTE *bSAK);
ULONG	VW_MifareHaltA(void);
ULONG	VW_MifareReadE2(USHORT usAddress,BYTE bLen,BYTE *baData);
ULONG	VW_MifareWriteE2(USHORT usAddress,BYTE bLen,BYTE *baData);
ULONG	VW_MifareLoadKeyE2(USHORT usAddress);
ULONG	VW_MifareLoadKey(BYTE *baKey);
ULONG	VW_MifareAuth(BYTE bKeyType,BYTE bNBlock,BYTE *baCSN);
ULONG	VW_MifareAuthEx(BYTE bKeyType,BYTE bNBlock,BYTE bCSN_Length,BYTE *baCSN);
ULONG	VW_MifareReadBlock(BYTE bNBlock,BYTE *baData);
ULONG	VW_MifareWriteBlock(BYTE bNBlock,BYTE *baData);
ULONG	VW_MifareIncrement(BYTE bNBlock,UINT uiValue);
ULONG	VW_MifareDecrement(BYTE bNBlock,UINT uiValue);
ULONG	VW_MifareRestore(BYTE bNBlock);
ULONG	VW_MifareTransfer(BYTE bNBlock);
ULONG   VW_FelicaPoll(BYTE *IDm,BYTE *PMm);
ULONG   VW_FelicaRW(BYTE* baSBuf, USHORT usSLen, BYTE* baRBuf, USHORT* usRLen, USHORT usTimeout);

#ifdef __cplusplus
}
#endif

#endif
