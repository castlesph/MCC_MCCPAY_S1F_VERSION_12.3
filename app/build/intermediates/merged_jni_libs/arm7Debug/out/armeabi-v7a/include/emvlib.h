#ifndef __EMVLIB_H__
#define __EMVLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	ULONG ulRef;
	BYTE *pbAID;
	BYTE bAIDLen;
    BYTE bApplication_Selection_Indicator;
} DefAppStruct;

typedef struct {
	short iAction;
	BYTE bTermDecision;
    
	char *pcAuthRespCode;
	BYTE *pbIssAutData;
	WORD uiIssAuthDataLen;
	BYTE *pbScript;
	WORD uiScriptLen;
    
	BYTE *pbDefTDOL;
	WORD uiDefTDOLLen;
	BYTE bCryptInfoData;
	WORD uiScriptResLen;
	BYTE *pbScriptRes;
} CompleteFuncPara;

typedef struct {

	void (*CBAPP_ECCardSelectAccount)(void);
	void (*CBAPP_SelectMultipleLanguages)(BYTE *LanguagePreference, BYTE LanguagePreferenceLen);
	
} stCallBackFunction;

typedef struct {
	
	BYTE baEntry[16];
	BYTE bEntryLen;
	BYTE baSupportApp[16];
	BYTE bSupportAppLen;
	
	BYTE bAddToCandidateList;
	BYTE bAppBlock;
	
	BYTE bSelectAIDList;
	BYTE bSelectNext;
	BYTE bAPDUFail;
	BYTE baSW12[2];
	
} stEntryInfo;

typedef struct {
	
	IN ULONG ulVersion;					//fixed to 0x01
	IN BYTE bEntryDataMaxNumber;
	OUT BYTE bTotalEntryNumber;
	OUT BYTE bCandidateEntryNumber;
	OUT BYTE bBlockAppNumber;
	
	OUT stEntryInfo *pstEntryData;

} stGetEntryStatus;

typedef struct {
	
	BYTE baDFName[16];
	BYTE bDFNameLen;
	BYTE baAppLabelStr[33];
	BYTE bAppLabelStrLen;
	//Below items are supported by stGetCandidateListInfo v2 or above
	BYTE bAppLabelStrSource;
	BYTE bAppPriorityIndicator;
	BYTE bIssuerCodeTableIndex;
	BYTE baLangPreference[8];
	BYTE bLangPreferenceLen;
	
} stCandidateInfo;

typedef struct {
	
	IN ULONG ulVersion;					//fixed to 0x02
	IN BYTE bCandidateDataMaxNumber;
	
	OUT BYTE bTotalCandidateNumber; 
	OUT stCandidateInfo *pstCandidateData;

} stGetCandidateListInfo;

typedef struct {

	IN BYTE bVersion;  // Should be 0x01;

	IN USHORT usMaxDataBufSize;
	OUT USHORT usDataLen; 
	OUT BYTE *pbaDataBuf; 
	OUT BYTE bAIDLen;
	OUT BYTE baAID[16];
	OUT BYTE bPANLen;
	OUT BYTE baPAN[10];
	OUT BYTE bIssuerCoutryCodeLen;
	OUT BYTE baIssuerCoutryCode[3];

}stCardAcquisitionData;

typedef struct
{
	BYTE bVersion;           // Should be 0x00
	
	USHORT usLineLeft_X;
	USHORT usLineRight_X;
	USHORT usLinePosition_Y;
	USHORT usTotalTimeout;
	USHORT usNoOperationTimeout;
	BYTE bPINDigitMaxLength;
	BYTE bPINDigitMinLength;
	BOOL IsRightToLeft;
	BOOL IsReverseLine;
	BYTE isEnableNullPIN;
	BYTE isPINBypassByCancelKey;
	BYTE bPINClearButtonBehavior;
	
}stGetPINFuncPara;


//EMV Level2 Data Define

#define d_CANDIDATE_SIZE										50
#define d_SUPPORT_APP_COUNT										80

// Status Codes
#define PP_OK													0
#define PP_DATA_BUFFER_EXCEEDED									1
#define PP_INVALID_PARA											2
#define PP_CRITICAL_MISTAKES									3
#define PP_SELECTION_FAIL										4
#define PP_PIN_BY_PASS											5
#define PP_TERMINAL_DATA_MISSING								6
#define PP_DATA_NOT_FOUND										7
#define PP_GAC1_6985_FALLBACK									8			//TIP
#define PP_ONLY_1_AP_NO_FALLBACK								9			//JIT
#define PP_IAP_9481_FALLBACK									10			//ADVT
#define PP_FORCE_ACCEPTANCE										11
#define PP_FUNCTION_NOT_SUPPORTED 								12			//return 6A81
#define PP_RETURN_KMS_OFFLINE_ENC_PIN							13
#define PP_PIN_PAD_MALFUNCTION									14
#define PP_RETURN_KMS2_OFFLINE_ENC_PIN							15
#define PP_CARD_REMOVED											17
#define PP_OFFLINE_PIN_CANCEL									18
#define PP_OFFLINE_PIN_TIMEOUT									19
#define PP_OFFLINE_PIN_CARD_REMOVED								20

//For PP_iSetPreferredOrder
#define PP_PO_EXCEED_SUPPOUNT_COUNT								100
#define PP_PO_EXCEED_SUPPPORT_DATALEN							101
#define PP_PO_INDATA_LEN_ERROR									102
#define PP_PO_BUF_NOT_ENOUGH									103

#define PP_SET_CDA_MODE_FAIL									120			//For PP_iSetCDAMode
#define PP_STRUCT_VERSION_ERROR									121
#define PP_MCCS_SELECT_PREFER_AID_FAIL							122
#define PP_SET_CARDHOLDER_SELECTION_CONFIRMATION_MODE_FAIL		123
#define PP_DATA_VERSION_ERROR									124


#define PP_SECURE_MODULE_ACCESS_PERMISSION_DENIED				0x7FFE


//AUC Flags
#define USCTRL_CASHBACK											0x01
#define	USCTRL_SERVICES											0x02
#define USCTRL_GOODS											0x04
#define USCTRL_ATM												0x08

//TAA Flags
#define FLG_MF_ONLINE											0x01
#define FLG_EXEC_FILE											0x02
#define FLG_MF_ACCEPT											0x04

//Completion
#define ACT_ONL_APPR											1
#define ACT_ONL_DENY											2
#define ACT_UNAB_ONL											3
#define ACT_ONL_ISSUER_REFERRAL_APPR							4
#define ACT_ONL_ISSUER_REFERRAL_DENY							5
#define ACT_UNAB_ONL_WITH_SPECIFIC_ARC							6

//PIN
#define PINRES_OK												1
#define PINRES_FAIL												2
#define PINRES_BLOCKED											3
#define PINRES_FAILBLOCKED										4

//For PP_SetDebug : bDebugPort
#define d_DEBUG_PORT_USB										0xFF
#define d_DEBUG_PORT_COM1										d_COM1
#define d_DEBUG_PORT_COM2										d_COM2
#define d_DEBUG_PORT_COM3										d_COM3

//CDA Mode
#define d_PP_CDA_MODE_1											0x01
#define d_PP_CDA_MODE_3											0x03

//VERIFY Command Status
#define d_PP_VERIFY_CMD_PERFORMED								0x00
#define d_PP_VERIFY_CMD_NOT_PERFORMED							0x01
#define d_PP_VERIFY_CMD_APDU_FAIL								0x02

//bAppLabelStrSource
#define d_PP_APP_STR_SOURCE_APP_PREFERRED_NAME					0x01
#define d_PP_APP_STR_SOURCE_APP_LABEL							0x02
#define d_PP_APP_STR_SOURCE_DF_NAME								0x03

//Cardholder Selection & Confirmation Mode
#define d_PP_CARDHOLDER_SEL_CONF_SUPPORT						0x00
#define d_PP_CARDHOLDER_SEL_CONF_NOT_SUPPORT					0x11

//PINPad Info
#define d_PP_PINPAD_INFO_GETPIN_OK								0xA5
#define d_PP_PINPAD_INFO_GETPIN_TIMEOUT							0xA6
#define d_PP_PINPAD_INFO_GETPIN_CANCEL							0xA7
#define d_PP_PINPAD_INFO_GETPIN_CANCEL_BY_API					0xA8

//----------------------------------------------------------------------------------------
// PP_iGetLastError Error Codes
#define d_PP_LASTERR_FUNCTION_NOT_SUPPORTED						0x01
#define d_PP_LASTERR_CANDIDATE_LIST_FULL						0x02
#define d_PP_LASTERR_SEND_APDU_CMD_FAIL							0x04
#define d_PP_LASTERR_ONLY_1_AP_BLOCKED							0x26		//JIT FT-18------------------------

#define d_PP_LASTERR_APPLICATION_NOT_ALLOW						0x07
#define d_PP_LASTERR_FIN_SEL_CONFIRM_STOP						0x8A

#define d_PP_LASTERR_IAP_AIP_AFL_ERROR							0x08
#define d_PP_LASTERR_IAP_UNKNOW_SW12							0x09
#define d_PP_LASTERR_IAP_MISS_PAN								0x0A
#define d_PP_LASTERR_IAP_MISS_CDOL1								0x0B
#define d_PP_LASTERR_IAP_MISS_CDOL2								0x0C
#define d_PP_LASTERR_IAP_APPLICATION_NOT_ALLOW					0x0D
#define d_PP_LASTERR_IAP_BAD_PADDING							0x81		//Paypass MChip test------------------
#define d_PP_LASTERR_IAP_FALLBACK_9481							0x88		//ADVT-------------------------------

#define d_PP_LASTERR_ERROR_9F4A_RULE							0x0E
#define d_PP_LASTERR_KEY_NO_FOUND								0x10
#define d_PP_LASTERR_PAN_NOT_SAME								0x11
#define d_PP_LASTERR_DDOL_MISS									0x12
#define d_PP_LASTERR_INTERNAL_AUTHENTICATE_FAIL					0x13
#define d_PP_LASTERR_SDA_LENGTH_NOT_MATCH						0x14
#define d_PP_LASTERR_NO_OFFLINE_DATA_AUTH_MATCH					0x15
#define d_PP_LASTERR_READ_DATA_TAG_NOT_70						0x16

#define d_PP_LASTERR_MISS_APP_EXPIRATION_DATE					0x17

#define d_PP_LASTERR_CARDHOLDER_VER_NOT_SUPP					0x18
#define d_PP_LASTERR_CVM_LIST_MISSING							0x19
#define d_PP_LASTERR_CVM_FINISH									0x1A
#define d_PP_LASTERR_CVM_PLAIN_TEXT								0x1B
#define d_PP_LASTERR_CVM_ENC_PIN_ONLINE							0x1C
#define d_PP_LASTERR_CVM_PLAINTEXT_PIN_SIGNATURE				0x1D
#define d_PP_LASTERR_CVM_ENC_PIN								0x1E
#define d_PP_LASTERR_CVM_ENC_PIN_SIGNATURE_OFFLINE				0x1F
#define d_PP_LASTERR_CVM_SIGNATURE								0x20
#define d_PP_LASTERR_CVM_IDCARD_VERIFY							0x25          //PBOC

#define d_PP_LASTERR_GET_DATA_CMD_ERROR							0x21

#define d_PP_LASTERR_CAS_GAC1_FAIL								0x23
#define d_PP_LASTERR_CAS_MERCHANT_FORCE_APPROVED				0x24

#define d_PP_LASTERR_CAS_OFFLINE_APPROVED						0xC8
#define d_PP_LASTERR_CAS_OFFLINE_DECLINED						0xC9

#define d_PP_LASTERR_CAS_NEED_ONLINE							0x27
#define d_PP_LASTERR_CAS_CARD_INIT_REFERRAL						0x28
#define d_PP_LASTERR_CAS_DDAAC_FAIL								0x29
#define d_PP_LASTERR_CAS_DDAAC_FAIL_NEED_GAC2					0x7B
#define d_PP_LASTERR_CAS_ONLINE_FAIL_APPROVAL					0x2A
#define d_PP_LASTERR_CAS_ONLINE_FAIL_DECLINED					0x78
#define d_PP_LASTERR_CAS_GAC1_6985_FALLBACK						0x7F		//TIP request

#define d_PP_LASTERR_CAS_GAC1_WRONG_CID							0x2C
#define d_PP_LASTERR_CAS_GAC1_FORMAT_1_PADDING					0x36

#define d_PP_LASTERR_CAS_ONLINE_APPROVED						0xCA
#define d_PP_LASTERR_CAS_ONLINE_DECLINED						0xCB
#define d_PP_LASTERR_CAS_ONLINE_DECLINED_REVERSAL				0xCC

#define d_PP_LASTERR_CAS_REFERRAL_APPROVED						0xCD
#define d_PP_LASTERR_CAS_REFERRAL_DECLINED						0xCE

#define d_PP_LASTERR_CAS_GAC2_FAIL								0x2E

#define d_PP_LASTERR_DIS_ADF_UNKNOW_TAG							0x2F
#define d_PP_LASTERR_DIS_ADF_DATA_MISSING						0x30

#define d_PP_LASTERR_DIS_FCI_DATA_ERROR							0x31

#define d_PP_LASTERR_DIS_TLV_TAG_ZERO							0x32
#define d_PP_LASTERR_DIS_TLV_FAIL								0x33
#define d_PP_LASTERR_DIS_TLV_EXCEED_MAX_LEN						0x35

#define d_PP_LASTERR_MISS_AIP_AFL								0x37

#define d_PP_LASTERR_DDOL_NOT_HAVE_9F37							0x38

#define d_PP_LASTERR_DGP_DIS_TLV_FAIL							0x39
#define d_PP_LASTERR_MULTIPLE_OCCURENCE							0x3A
#define d_PP_LASTERR_CVM_DATA_LEN_LESS_8						0x3B
#define d_PP_LASTERR_SFI_TEMPLATE_RULE_WRONG					0x7C

#define d_PP_LASTERR_SDA_DATA_ERROR								0x3C
#define d_PP_LASTERR_SDA_ALGORITHM_NOT_SUPPORT					0x3D

#define d_PP_LASTERR_DDA_DATA_ERROR								0x3E

#define d_PP_LASTERR_DDAAC_DATA_ERROR							0x40
#define d_PP_LASTERR_DDAAC_ALGORITHM_NOT_SUPPORT				0x41
#define d_PP_LASTERR_KEY_LENGTH_ERROR							0x43
#define d_PP_LASTERR_DDAAC_HASH1_ERROR							0x45
#define d_PP_LASTERR_DDAAC_HASH2_ERROR							0x46

#define d_PP_LASTERR_ISSUER_CERT_NOT_EXIST						0x47
#define d_PP_LASTERR_ISSUER_CERT_FORMAT_ERROR					0x48
#define d_PP_LASTERR_ISSUER_CERT_IIN_PAN_NOT_SAME				0x49
#define d_PP_LASTERR_ISSUER_CERT_REVOCATION_FOUND				0x4A
#define d_PP_LASTERR_ISSUER_CERT_ALGORITHM_NOT_SUPPORT			0x4B
#define d_PP_LASTERR_ISSUER_CERT_LENGTH_ERROR					0x4C
#define d_PP_LASTERR_ISSUER_CERT_EXPIRATION_DATE				0x4D
#define d_PP_LASTERR_ISSUER_CERT_HASH_NOT_MATCH					0x4E
#define d_PP_LASTERR_ISSUER_CERT_EXPONENT_NOT_EXIST				0x4F
#define d_PP_LASTERR_ISSUER_CERT_REMAINDER_MISSING				0x50
#define d_PP_LASTERR_ISSUER_CERT_CAPKI_NOT_EXIST				0x79

#define d_PP_LASTERR_ICC_CERT_NOT_EXIST							0x51
#define d_PP_LASTERR_ICC_CERT_FORMAT_ERROR						0x52
#define d_PP_LASTERR_ICC_CERT_ALGORITHM_NOT_SUPPORT				0x53
#define d_PP_LASTERR_ICC_CERT_LENGTH_ERROR						0x54
#define d_PP_LASTERR_ICC_CERT_HASH_NOT_MATCH					0x55
#define d_PP_LASTERR_ICC_CERT_EXPIRATION_DATE					0x56
#define d_PP_LASTERR_ICC_CERT_EXPONENT_NOT_EXIST				0x57
#define d_PP_LASTERR_ICC_ISSUER_PK_NOT_EXIST					0x58
#define d_PP_LASTERR_ICC_CERT_REMAINDER_MISSING					0x59

#define d_PP_LASTERR_PIN_CERT_NOT_EXIST							0x5A
#define d_PP_LASTERR_PIN_CERT_LENGTH_ERROR						0x5B
#define d_PP_LASTERR_PIN_CERT_FORMAT_ERROR						0x5C
#define d_PP_LASTERR_PIN_CERT_ALGORITHM_NOT_SUPPORT				0x5D
#define d_PP_LASTERR_PIN_CERT_HASH_NOT_MATCH					0x5E
#define d_PP_LASTERR_PIN_CERT_EXPIRATION_DATE					0x5F
#define d_PP_LASTERR_PIN_CERT_KEY_LENGTH_ERROR					0x60
#define d_PP_LASTERR_PIN_CERT_EXP_NOT_EXIST						0x85

#define d_PP_LASTERR_READ_PIN_TRY_COUNT_FAIL					0x61

#define d_PP_LASTERR_CVM_PLAIN_TEXT_PIN_NOT_KEYIN				0x62
#define d_PP_LASTERR_CVM_PLAIN_TEXT_PIN_TRY_LIMIT_EXCEEDED		0x63
#define d_PP_LASTERR_CVM_PLAIN_TEXT_PIN_OK						0x64
#define d_PP_LASTERR_CVM_PLAIN_TEXT_PIN_WRONG					0x66
#define d_PP_LASTERR_CVM_PLAIN_TEXT_UNKNOW_SW12					0x67
#define d_PP_LASTERR_CVM_PLAIN_TEXT_PIN_BLOCK					0x7D

#define d_PP_LASTERR_CVM_TERMINAL_NOT_SUPPORT_SPECIFY_CVM		0x68     //No pin pad or terminal cap indicates not support!
#define d_PP_LASTERR_CVM_ENC_PIN_ONLINE_PIN_NOT_KEYIN			0x69

#define d_PP_LASTERR_CVM_ENCIPHERED_PIN_NOT_KEYIN				0x6C
#define d_PP_LASTERR_CVM_ENCIPHERED_PIN_TRY_LIMIT_EXCEEDED		0x6D
#define d_PP_LASTERR_CVM_ENCIPHERED_PIN_UNKNOW_SW12				0x6E
#define d_PP_LASTERR_CVM_ENCIPHERED_PIN_GET_RN_UNKNOW_SW12		0x71
#define d_PP_LASTERR_CVM_ENCIPHERED_PIN_OK						0x6F
#define d_PP_LASTERR_CVM_ENCIPHERED_PIN_WORNG					0x70
#define d_PP_LASTERR_CVM_ENCIPHERED_PIN_BLOCK					0x7E    
                 
#define d_PP_LASTERR_DIS_GAC_UNKNOW_TAG							0x72
#define d_PP_LASTERR_DIS_GAC_ATC_LEN_WRONG						0x83
#define d_PP_LASTERR_DIS_GAC_DATA_MISSING						0x84

#define d_PP_LASTERR_DIS_GAC_AC_LEN_WRONG						0x86
#define d_PP_LASTERR_DIS_GAC_CID_LEN_WRONG						0x87

#define d_PP_LASTERR_MISSING_TERMINAL_DATA						0x75  
#define d_PP_LASTERR_CARD_DATA_BUF_OVERFLOW						0x77

#define d_PP_LASTERR_INVALID_APP_EXPIRATION_DATE				0x0226
#define d_PP_LASTERR_INVALID_APP_EFFECTIVE_DATE					0x0227

#define d_PP_LASTERR_UP_APPLICATION_UNKNOW_SW12					0x0228

//----------------------------------------------------------------------------------------


void PP_EMV_Reset(void);
void PP_EMV_GetVersion(char *ver);
short PP_iGetLastError(void);
short PP_iSCDefApp(DefAppStruct *pstData);
short PP_iGetCandidateList(WORD *puiNumItems, char *vszLabels[], ULONG vulRef[], char *pfConFirm, char bNewSel);
short PP_iSelect_App(short iAppIdx);
short PP_iSelect_AID(short iAppIdx);
short PP_iAuthentication(BYTE *pbDefDDOL, WORD uiDefDDOLLen);
short PP_iProcessingRestrictions(short iAppVerNum, BYTE vvbAppVerList[][2], BYTE bFlags);
short PP_iCardholderVerification(char *pfSignature);
short PP_iRiskManagement(ULONG ulLastAmount, BYTE bTargetPerc, BYTE bMaxTargetPerc, ULONG ulThresholdValue);
short PP_iTermActionAnalysis(BYTE bFlags, BYTE *pbTACDenial, BYTE *pbTACOnline, BYTE *pbTermDecision);
short PP_iTermActionAnalysisEx(BYTE bFlags, BYTE *pbTACDenial, BYTE *pbTACOnline, BYTE *pbTACDefault, BYTE *pbTermDecision);
short PP_iCardActionAnalysis(BYTE bTermDecision, BYTE *pbDefTDOL, WORD uiDefTDOLLen, BYTE *pbCryptInfoData);
short PP_iDefTermActionAnalysis(BYTE *pbTACDefault, BYTE *pbTermDecision);
short PP_iCompletion(CompleteFuncPara *para);
short PP_iEMVGetData(WORD wTag, WORD *puiLen, BYTE *pbVal);
short PP_iEMVSetData(WORD wTag, WORD uiLen, BYTE *pbVal);
short PP_iEMVInitData(void);
void PP_SetDebug(BYTE fDebufEnable, BYTE bDebugPort);
short PP_iSetPreferredOrder(BYTE PONum, BYTE *POData, USHORT PODataLen);
short PP_iGetPreferredOrder(BYTE *PONum, BYTE *POData, USHORT *PODataLen);
void PP_iSetCallBackFunction(stCallBackFunction *CallBackFunction);
void PP_GetVersion(BYTE *baVersionStr);
short PP_iSetCDAMode(BYTE bMode);
short PP_iSubsequentPINBypassSupport(BOOL isSupported);
short PP_iSetCardholderSelectionConfirmation(BYTE bMode);		//0x00 : Support ; 0x11 : Not support
short PP_iGetAllEntryStatus(stGetEntryStatus* pstEntryStatus);
short PP_iGetCandidateListInfo(stGetCandidateListInfo* pstCandidateListInfo);
short PP_iGetVERIFYCommandSW12(BYTE *pbCMDStatus, BYTE *baSW12);
void PP_SpecialEventRegister(IN BYTE EventID, IN void *EventFunc);
short PP_iGetCandidateDirEntry(IN short iAppIdx, OUT BYTE *baDirEntry, OUT USHORT *pusDirEntryLen, IN USHORT usMaxDirEntrySize);
short PP_iGetPSERsp(OUT BYTE *baRspBuf, OUT USHORT *pusRspBufLen, IN USHORT usMaxRspBufSize);
short PP_iSetAppFilteringPara(IN BYTE* baBuf, IN USHORT usBufLen);
short PP_iGetAppFilteringPara(OUT BYTE *baBuf, OUT USHORT *pusBufLen, IN USHORT usMaxBufSize);
short PP_iTxnCardAcquisitionRead(USHORT *pusCandidateNum);
short PP_iTxnCardAcquisitionGet(stCardAcquisitionData *pstAcquisitionBuf, void *RFU);
short PP_iTxnCardAcquisitionSel(USHORT usCandidateIndex, stCardAcquisitionData *pstAcquisitionBuf);

short PP_iGetSelectionFailInfo(BYTE* baSelDFName,BYTE* bSelDFNameLen, BYTE* baSelAppLabel, BYTE* bSelAppLabelLen, ULONG* ulErrCode, BYTE* baSW12);

short PP_iNullPINEnable(void);
short PP_iNullPINDisable(void);
short PP_iVirtualPINCancel(void);
short PP_iPINClearButtonBehaviorSet(BYTE bBehavior);
short PP_iPINClearButtonBehaviorGet(BYTE *pbBehavior);
short PP_iPINTimeoutSet(BYTE bType, BYTE bStatus, USHORT usTimeoutSec);
short PP_iPINTimeoutGet(BYTE bType, BYTE *pbStatus, USHORT *pusTimeoutSec);

short PP_iSetOfflinePinPara(stGetPINFuncPara *pstPINFuncPara);
short PP_iGetOfflinePinPara(stGetPINFuncPara *pstPINFuncPara);

short PP_iSelect_App_Final(short iAppIdx, BYTE *pbSelRsp, USHORT *pusSelRspLen, USHORT usSelRspMaxSize, BOOL *pisSelRspBufExceeded);
short PP_iGetProcessingOptions(void);
short PP_iReadRecord(void);

//----------------------------------------------------------------------------------------
BYTE APP_fCheckRevocation(BYTE *pbRID, BYTE bCAPKIdx, BYTE *pbSerNum);
BYTE APP_fGetCAPK(BYTE *pbRID, BYTE bCAPKIdx, BYTE *pbMod, WORD *puiModLen, BYTE *pbExp, WORD *puiExpLen);
BYTE APP_fGetTermData(WORD wTag, WORD *puiLen, BYTE *pbVal);
short APP_iGetPINOffline(short iRemainingTries, char *pszPIN);
short APP_iGetPINOnline(void);
void APP_PINVerifyResult(short iResult);
void APP_ShowAdvice(BYTE advice);
short APP_iIDCardVerify(void);

//----------------------------------------------------------------------------------------
//For PP_SpecialEventRegister

#define d_PP_EVENTID_APPLICATION_SELECTION_METHOD						0x05
typedef void (*EVENT_APP_ApplicationSelectionMethod)(OUT BYTE *pbMethod);
#define d_EVENT_APP_SELECTION_METHOD_PSE					0x00
#define d_EVENT_APP_SELECTION_METHOD_LIST_AID				0x01

#define d_PP_EVENTID_INTERAC_ASF_MODE									0x08
typedef void (*EVENT_APP_InteracASFMode)(OUT BYTE *pbMethod);
#define d_EVENT_ASF_MODE_NORMAL								0x00
#define d_EVENT_ASF_MODE_NONE								0x01
#define d_EVENT_ASF_MODE_NONE_AND_DROP_INTERAC				0x02

#define d_PP_EVENTID_PIN_TRY_COUNTER									0x09
typedef void (*EVENT_APP_PINTryCounter)(IN BYTE bPTC);

#define d_PP_EVENTID_SHOW_VIRTUAL_PIN									0x0A
typedef USHORT (*EVENT_APP_ShowVirtualPIN)(OUT USHORT *pusPINPadButtonNum, OUT BYTE *pbPINPadButtonInfo, OUT USHORT *pusPINPadButtonInfoLen);

#define d_PP_EVENTID_SHOW_PIN_DIGIT										0x0B
typedef void (*EVENT_APP_ShowPINDigit)(IN BYTE bNoDigits);

#define d_PP_EVENTID_GET_PIN_DONE										0x0C
typedef USHORT (*EVENT_APP_GetPINDone)(void);

#define d_PP_EVENTID_SHOW_PIN_BYPASS									0x0D
typedef void (*EVENT_APP_ShowPINBypass)(void);

#define d_PP_EVENTID_PIN_BYPASS											0x0E
typedef USHORT (*EVENT_APP_PINBypass)(OUT BYTE *pbAction);

#define d_PP_EVENTID_CANCEL_PIN_CONFIRM									0x0F
typedef USHORT (*EVENT_APP_CancelPINConfirm)(IN BYTE bStatus, OUT BYTE *pbAction);

#define d_PP_EVENTID_PINPAD_INFO										0x10
typedef USHORT (*EVENT_APP_PINPadInfo)(IN USHORT usInfo, IN USHORT usInRFU, OUT USHORT *pusOutRFU);

#define d_PP_EVENTID_FINAL_SELECTION_CONFIRM							0x11
typedef USHORT (*EVENT_APP_FinalSelectionConfirm)(IN BYTE *baSelectedAID, IN BYTE bAIDLen, IN BYTE *baSelectRsp, IN USHORT usRspLen, 
		OUT BYTE *pbAction);
#define d_EVENT_FINAL_SELECTION_CONFIRM_ACTION_CONTINUE				0x00
#define d_EVENT_FINAL_SELECTION_CONFIRM_ACTION_STOP					0x01

#define d_PP_EVENTID_PIN_OTHER_KEYS										0x12
typedef USHORT (*EVENT_APP_PINOtherKeys)(IN BYTE bKey, IN USHORT usInRFU, OUT USHORT *pusOutRFU);



#ifdef __cplusplus
}
#endif

#endif

