/********************************************************************
 Copyright (c) Castles Technology Co., Ltd. 2006~
 Module Name: contactless.h
 --------------------------------------------------------------------
 v1.6		2007.07.24	Add contactless card related functions to tclcard.c	
		2007.07.25	Add new baudrate index 0x05(BR:9600) in VW_SetBaudRate()
 v1.7		2007.10.29      Modify VW_GetCAPublicKey() and VW_GetJCBPublicKey()
 v2.0		2008.02.21	Add some functions for DLE protocol with payPass transaction use 
					Modify coding in VisaWave related functions
		2008.03.18	Remove d_SAM_xxxxx definition
					Remove baudrate index 0x05(BR:9600) from VW_SetBaudRate()
		2008.04.14      Modify return code value of d_HOST_ID_INCORRECT          		
 v2.2		2010.01.20	Add new function VW_TypeB_ActiveIdleEx()	
		2010.01.29      Add new function VW_TypeA_RATSEx()	         		
*********************************************************************/
#ifndef TCLCARD_H
#define TCLCARD_H

#ifdef __cplusplus
extern "C" 
{
#endif

// Used for VW_TypeB_ActiveIdleEx()
#define d_USE_REQB      0
#define d_USE_WUPB      1
 
#define d_FSDI_16		0   
#define d_FSDI_24		1
#define d_FSDI_32		2
#define d_FSDI_40		3
#define d_FSDI_48		4
#define d_FSDI_64		5
#define d_FSDI_96		6
#define d_FSDI_128 		7
#define d_FSDI_256 		8
 
#define d_NUM_SLOT_x1  	0 
#define d_NUM_SLOT_x2  	1
#define d_NUM_SLOT_x4  	2
#define d_NUM_SLOT_x8  	3
#define d_NUM_SLOT_x16 	4
 
#define d_MAXRATE_106	106
#define d_MAXRATE_212	212
#define d_MAXRATE_424	424

ULONG 	VW_TypeA_ActiveIdle(BYTE bBaudRate,BYTE *baATQA,BYTE *bSAK,BYTE *bCSN_Length,BYTE *baCSN);
ULONG 	VW_TypeA_RATS(BYTE bAutoPPS,BYTE *baATS,UINT *uiRLen);
ULONG	VW_TypeA_RATSEx(BYTE bAutoPPS,USHORT usMaxRate,BYTE bFSDI,BYTE *baATS,UINT *uiRLen);
ULONG 	VW_TCL_APDU(BYTE *baAPDU,UINT uiSLen,BYTE *baRBuf,UINT *uiRLen);
ULONG 	VW_TCL_DESELECT(void);
ULONG 	VW_TypeB_PowerOn(BYTE *baPUPI);
ULONG 	VW_TypeB_ActiveIdleEx(BYTE bAFI, BYTE bWUPB, BYTE bFSDI, BYTE bNumAntiColSlot, USHORT usMaxRate, BYTE *baPUPI);
ULONG	VW_NAKPollStart(void);
ULONG	VW_NAKPollStop(void);
ULONG	VW_NAKPollSet(BYTE bOnOff,BYTE bInterval);


#ifdef __cplusplus
}
#endif

#endif
