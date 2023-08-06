#ifndef CASTLES_CL_CLIENT_H
#define CASTLES_CL_CLIENT_H


#ifdef	__cplusplus
extern "C" {
#endif
#include "typedef.h"
/*
int CTOS_CLAPDU (uint8_t* baSBuf, int32_t usSLen, uint8_t* baRBuf, int32_t* usRLen);
int CTOS_CLInitComm (uint32_t uiBaudRate);
int CTOS_CLDESELECT (void);
int CTOS_CLTypeAActiveFromIdle (uint8_t bBaudRate, uint8_t* baATQA, uint8_t* bSAK, uint8_t* baCSN, uint8_t* bCSNLen);
int CTOS_CLRATS(uint8_t bAutoBR, uint8_t* baATS, int32_t* bATSLen);
int CTOS_CLTypeBActive(uint8_t* baPUPI);
int CTOS_CLPowerOff (void);
*/


//------------------------------------------------------------------------------
//Data Structure
//------------------------------------------------------------------------------
	
//------------------------------------------------------------------------------
typedef struct 
{
	BYTE	bMessageIdentifier;
	BYTE	bStatus;
	BYTE	baHoldTime[3];
	BYTE	baLanguagePreference[8];
	BYTE	bValueQualifier;
	BYTE	baValue[6];
	BYTE	baCurrencyCode[2];
	
}EMVCL_USER_INTERFACE_REQ_DATA;

typedef struct 
{
	BOOL (*OnCancelTransaction)(void);
	void (*OnShowMessage)(BYTE bKernel, EMVCL_USER_INTERFACE_REQ_DATA *baUserInterfaceRequestData);
	
}EMVCL_EVENT;

typedef struct 
{
	BYTE	bConfigFilenameLen;
	BYTE	*pConfigFilename;
	EMVCL_EVENT stOnEvent;
	
}EMVCL_INIT_DATA;
	
//------------------------------------------------------------------------------
typedef struct 
{
	BYTE	bStart;
	BYTE	bTagNum;
	USHORT	usTransactionDataLen;
	BYTE	*pbaTransactionData;

}EMVCL_ACT_DATA;
	
//------------------------------------------------------------------------------
typedef struct 
{
	BYTE	bSID;
	BYTE	baDateTime[15];
	BYTE	bTrack1Len;
	BYTE	baTrack1Data[100];
	BYTE	bTrack2Len;
	BYTE	baTrack2Data[100];
	USHORT	usChipDataLen;
	BYTE	baChipData[1024];
	USHORT	usAdditionalDataLen;
	BYTE	baAdditionalData[1024];
	
}EMVCL_RC_DATA_EX;

//------------------------------------------------------------------------------
typedef struct
{
	USHORT	usTransResult;			// Transction Result
	BYTE	bCVMAnalysis;			//
	BYTE	baCVMResults[3];		//CVM Result
	BYTE	bVisaAOSAPresent;		//Visa Available Offline Spending Amount(AOSA) Present
	BYTE	baVisaAOSA[6];
	BOOL	bODAFail;				//Offline Data Authentication Fail , ODA FAIL -> TRUE, ODA OK --> FALSE
	BOOL	bCompleteFunRequired;	//Complete Function Required, Yes -> TRUE, No -> FALSE
	
}EMVCL_RC_DATA_ANALYZE;

//------------------------------------------------------------------------------
typedef struct 
{
	IN BYTE			bAIDLen;
	IN BYTE			baAID[16];
	IN BYTE			bKernelID;
	IN BYTE			bTransactionType;
	IN USHORT		usTAGDataLen;
	IN BYTE			*pbaTAGData;

}EMVCL_AID_SET_TAG_DATA;

//------------------------------------------------------------------------------
typedef struct 
{
	IN BYTE			bAIDLen;
	IN BYTE			baAID[16];
	IN BYTE			bKernelID;
	IN BYTE			bTransactionType;
	INOUT USHORT	usTAGDataLen;
	OUT BYTE		*pbaTAGData;
	
}EMVCL_AID_GET_TAG_DATA;

//------------------------------------------------------------------------------
typedef struct
{
	IN UINT uiVersion;				//fixed to 0x01
	OUT UINT uiNo;					//Number of AID Combination List {TransType-AID-KERNEL ID}
	OUT BYTE bAIDLen[256];			//list all of AID Len
	OUT BYTE baAID[256][16];		//list all of AID data
	OUT BYTE bKernelID[256];		//list all of Kernel ID data
	OUT BYTE bTransactionType[256];	//list all of Transaction Type data

	// One AID Combination List {bAIDLen[x] + baAID[x][] + bKernelID[x] + bTransactionType[x]}  (x = 0 ~ uiNo)
}EMVCL_AID_COMBINATION_LIST;

//------------------------------------------------------------------------------
typedef struct
{
	BYTE	bAction;
	BYTE	bIndex;
	UINT	uiModulusLen;
	BYTE	baModulus[248];
	UINT	uiExponentLen;		// Length of Extension
	BYTE	baExponent[3];		// Extension
	BYTE	baHash[20];			// Key Hash (SHA-1) Result

}EMVCL_CA_PUBLIC_KEY;

//------------------------------------------------------------------------------
typedef struct
{
	UINT	uiNoP;				// Number of Parameters
	UINT	uiIndex[100];		// Parameter Index
	UINT	uiLen[100];			// Length of Parameter Data
	BYTE	baData[100][20];	// Parameter Data
	
}EMVCL_PARAMETER_DATA;

//------------------------------------------------------------------------------
typedef struct
{
	BYTE	bNoS;				// Number of Schemes
	BYTE	baID[255];			// Scheme ID
	BYTE	baAction[255];		// Active or deactive
	
}EMVCL_SCHEME_DATA;

//------------------------------------------------------------------------------
typedef struct
{
	IN		BYTE	bTagNum;
	IN		USHORT	usTransactionDataLen;
	IN		BYTE	*pbaTransactionData;
	INOUT	BYTE	bSelectedAIDLen;
	OUT		BYTE	*pbaSelectedAID;
	INOUT	ULONG	ulSelectAIDRspLen;
	OUT		BYTE	*pbaSelectAIDRsp;
	INOUT	ULONG	ulSelectPPSERspLen;
	OUT		BYTE	*pbaSelectPPSERsp;
	
}EMVCL_DETECT_TXN_DATA;

//------------------------------------------------------------------------------
typedef struct
{
	BYTE		bSID;				// Scheme Identifier
	BYTE		baDateTime[15];		// YYYYMMDDHHMMSS format
	BYTE		bTrack1Len;
	BYTE		baTrack1Data[100];	// ANS 
	BYTE		bTrack2Len;
	BYTE		baTrack2Data[100];	// ASCII
	BYTE		bChipDataLen;		// Chip Data
	BYTE		baChipData[256];
	BYTE		bAdditionalDataLen;	// Additional Data
	BYTE		baAdditionalData[256];
	
}EMVCL_RC_DATA;

//------------------------------------------------------------------------------
typedef struct
{
	BYTE		bNoCVM;				// Number of CVM
	BYTE		baCVMID[10];		// CVM ID
	BYTE		baAction[10];		// Active or deactive
	
}EMVCL_CVM_DATA;

//------------------------------------------------------------------------------
typedef struct
{
	IN		UINT	uiVersion;				//fixed to 0x01
	
	IN		USHORT	usTagNum;
	IN		USHORT	usTransactionDataLen;
	IN		USHORT	usRequestedCardTagLen;
	IN		BYTE	*pbaTransactionData;
	IN		BYTE	*pbaRequestedCardTag;
	
	INOUT	USHORT	usSelectedAIDLen;
	INOUT	USHORT	usSelectAIDRspLen;
	INOUT	USHORT	usSelectPPSERspLen;
	INOUT	USHORT	usGotCardDataLen;
	OUT		BYTE	*pbaSelectedAID;
	OUT		BYTE	*pbaSelectAIDRsp;
	OUT		BYTE	*pbaSelectPPSERsp;
	OUT		BYTE	*pbaGotCardData;
	
}EMVCL_GET_CARD_DATA;
//------------------------------------------------------------------------------
typedef struct
{
	UINT	uiVersion;						// fixed to 0x01;
	
	BYTE	baAppLabel[33];
	BYTE	bAIDLen;
	BYTE	baAID[16];
	
}EMVCL_APP_LIST_DATA;
//------------------------------------------------------------------------------
typedef struct
{
	UINT	uiVersion;						// fixed to 0x02;
	
	BYTE	baAppLabel[33];
	BYTE	bAIDLen;
	BYTE	baAID[16];
	BOOL	bIssuerIdentificationNumberPresented;
	BYTE	baIssuerIdentificationNumber[3];
	BOOL	bIssuerCountryCodeAlpha2Presented;
	BYTE	baIssuerCountryCodeAlpha2[2];
	
}EMVCL_APP_LIST_DATA_V2;
//------------------------------------------------------------------------------
typedef struct
{
	IN	UINT	uiVersion;						// fixed to 0x01;
	
	IN	BYTE	bAIDLen;
	IN	BYTE	baAID[16];
	IN	BYTE	bASRPDLen;
	IN	BYTE	baASRPD[252];
	OUT	BYTE	bAction;
	
}EMVCL_APP_SELE_REG_PROPRI_DATA;



//Add for S1
typedef struct
{
	BYTE	bVersion;
	BYTE	bKeyType;
	USHORT	usCipherKeySet;
	USHORT	usCipherKeyIndex;
	BYTE	bCipherMethod;
	BYTE	baICV[32];
	BYTE	bICVLen;
	BYTE	baSK[32];
	BYTE	bSKLen;
	BYTE	bChecksumType;
	BYTE	bPaddingMethod;	
	BYTE	bLRCIncluded;
	BYTE	bSS_ESIncluded;		
	//add
	BYTE bIsKSNFixed;
	
}EMVCL_SECURE_DATA_INFO;



ULONG EMVCL_ShowContactlessSymbol(void *pPara);
ULONG EMVCL_InitTransactionEx(BYTE bTagNum, BYTE *baTransData, USHORT usTransDataLen);
ULONG EMVCL_PerformTransactionEx(EMVCL_RC_DATA_EX *stRCDataEx);
ULONG EMVCL_StartTransactionEx(EMVCL_ACT_DATA *stACTData, EMVCL_RC_DATA_EX *stRCDataEx);
void EMVCL_AnalyzeTransactionEx(EMVCL_RC_DATA_EX *stRCDataEx, EMVCL_RC_DATA_ANALYZE *stRCDataAnalyze);
ULONG EMVCL_CompleteEx(BYTE bAction, BYTE *baARC, UINT uiIADLen, BYTE *baIAD, UINT uiScriptLen, BYTE *baScript, EMVCL_RC_DATA_EX *stRCDataEx);
ULONG EMVCL_StartIdleLEDBehavior(void *pPara);
ULONG EMVCL_StopIdleLEDBehavior(void *pPara);
ULONG EMVCL_SetLED(BYTE bIndex, BYTE bOnOff);
ULONG EMVCL_Initialize(EMVCL_INIT_DATA *pInitData);
ULONG EMVCL_SpecialEventRegister(BYTE bEventID, void *pEventFunc);
void EMVCL_SetDebug(BYTE fDebufEnable, BYTE bDebugPort);
ULONG EMVCL_ShowVirtualLED(void *pPara);
ULONG EMVCL_CancelTransaction(void);
ULONG EMVCL_SetEMVTags(BYTE *baTagsData, UINT uiTagLen, BYTE *baRBuf, UINT *uiRLen);
ULONG EMVCL_SetParameter(EMVCL_PARAMETER_DATA *stPara);
ULONG EMVCL_ListAllCAPKID(BYTE *baRBuf, UINT *uiRLen);
ULONG EMVCL_SetCAPK(BYTE *baRID, EMVCL_CA_PUBLIC_KEY *stCAPubKey);
ULONG EMVCL_SetUIType(BYTE bType);




#ifdef	__cplusplus
}
#endif

#endif
