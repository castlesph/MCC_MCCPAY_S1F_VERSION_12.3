#ifndef __DESFIRE_H__
#define __DESFIRE_H__

#ifdef __cplusplus
extern "C"
{
#endif

//Communication Type
#define d_API_CASTLES					0x00
#define d_API_SAMAV2					0x01

//Diversification Types
#define d_DIV_MODE_MIFARE_PLUS			0x01

//Key Type
#define d_KEY_TYPE_DES					0x00
#define d_KEY_TYPE_3DES					0x01
#define d_KEY_TYPE_3K3DES				0x02
#define d_KEY_TYPE_AES128				0x03

//Key Len
#define d_KEY_LEN_DES					8
#define d_KEY_LEN_3DES					16
#define d_KEY_LEN_3K3DES				24
#define d_KEY_LEN_AES128				16

//File Type
#define d_STANDARD_DATA_FILE			0x00
#define d_BACKUP_FILE					0x01
#define d_VALUE_FILE					0x02
#define d_LINEAR_RECORD_FILE			0x03
#define d_CYCLIC_RECORD_FILE			0x04

//Security related commands define
#define d_CMD_AUTHENTICATE				0x0A
#define d_CMD_AUTHENTICATE_ISO			0x1A
#define d_CMD_AUTHENTICATE_AES			0xAA
#define d_CMD_CHANGE_KEY_SETTINGS		0x54
#define d_CMD_SET_CONFIGURATION			0x5C
#define d_CMD_CHANGE_KEY				0xC4
#define d_CMD_GET_KEY_VERSION			0x64

//PICC level commands define
#define d_CMD_CREATE_APPLICATION		0xCA
#define d_CMD_DELETE_APPLICATION		0xDA
#define d_CMD_GET_APPLICATION_IDS		0x6A
#define d_CMD_FREE_MEMORY				0x6E
#define d_CMD_GET_DF_NAMES				0x6D
#define d_CMD_GET_KEY_SETTINGS			0x45
#define d_CMD_SELECT_APPLICATION		0x5A
#define d_CMD_FORMAT_PICC				0xFC
#define d_CMD_GET_VERSION				0x60
#define d_CMD_GET_CARD_UID				0x51

//Application level commands define
#define d_CMD_GET_FILE_IDS				0x6F
#define d_CMD_GET_FILE_SETTINGS			0xF5
#define d_CMD_CHANGE_FILE_SETTINGS		0x5F
#define d_CMD_CREATE_STD_DATA_FILE		0xCD
#define d_CMD_CREATE_BACKUP_DATA_FILE	0xCB
#define d_CMD_CREATE_VALUE_FILE			0xCC
#define d_CMD_CREATE_LINEAR_RECORD_FILE	0xC1
#define d_CMD_CREATE_CYCLIC_RECORD_FILE	0xC0
#define d_CMD_DELETE_FILE				0xDF
#define d_CMD_GET_ISO_FILE_IDS			0x61

//Data manipulation commands define
#define d_CMD_READ_DATA					0xBD
#define d_CMD_WRITE_DATA				0x3D
#define d_CMD_GET_VALUE					0x6C
#define d_CMD_CREDIT					0x0C
#define d_CMD_DEBIT						0xDC
#define d_CMD_LIMITED_CREDIT			0x1C
#define d_CMD_WRITE_RECORD				0x3B
#define d_CMD_READ_RECORDS				0xBB
#define d_CMD_CLEAR_RECORD_FILE			0xEB
#define d_CMD_COMMIT_TRANSACTION		0xC7
#define d_CMD_ABORT_TRANSACTION			0xA7

//Error Define
#define d_INVALID_AUTH_TYPE				0xFF01
#define d_INVALID_KEYLEN				0xFF02
#define d_INVALID_RN					0xFF03
#define d_INVALID_RESPONSE				0xFF04
#define d_NOT_AUTH_BEFORE				0xFF05
#define d_NOT_SUPPORT_ACCESSRIGHT		0xFF06
#define d_ERROR_KEYNUMBER				0xFF07
#define d_INVALID_PARAMETER				0xFF08
#define d_ERROR_LENGTH					0xFF09
#define d_NOT_SUPPORT_KEY_TYPE			0xFF0A

//Error Status
#define d_OPERATION_OK					0x00
#define d_NO_CHANGES					0x0C
#define d_OUT_OF_EEPROM_ERROR			0x0E
#define d_ILLEGAL_COMMAND_CODE			0x1C
#define d_INTEGRITY_ERROR				0x1E
#define d_NO_SUCH_KEY					0x40
#define d_LENGTH_ERROR					0x7E
#define d_PERMISSION_DENIED				0x9D
#define d_PARAMETER_ERROR				0x9E
#define d_APPLICATION_NOT_FOUND			0xA0
#define d_APPL_INTEGRITY_ERROR			0xA1
#define d_AUTHENTICATION_ERROR			0xAE
#define d_ADDITIONAL_FRAME				0xAF
#define d_BOUNDARY_ERROR				0xBE
#define d_PICC_INTEGRITY_ERROR			0xC1
#define d_COMMAND_ABORTED				0xCA
#define d_PICC_DISABLED_ERROR			0xCD
#define d_COUNT_ERROR					0xCE
#define d_DUPLICATE_ERROR				0xDE
#define d_EEPROM_ERROR					0xEE
#define d_FILE_NOT_FOUND				0xF0
#define d_FILE_INTEGRITY_ERROR			0xF1

//Config define
#define d_MFDF_CONFIG_WRAPPED			0

//Option define
#define d_MFDF_OPTION_COMMUNICATION_PLAIN	0
#define d_MFDF_OPTION_COMMUNICATION_MACD	1
#define d_MFDF_OPTION_COMMUNICATION_ENC		3

//Struct 
typedef struct{
	uint8_t bAID[3];
	uint8_t bFID[2];
	uint8_t bFileName[16];
	uint8_t NameLen;
}FILE_INFO;

typedef struct{
	uint8_t bHwVendorID;
	uint8_t bHwType;
	uint8_t bHwSubType;
	uint8_t bHwMajorVersion;
	uint8_t bHwMinorVersion;
	uint8_t bHwStorageSize;
	uint8_t bHwProtocol;

	uint8_t bSwVendorID;
	uint8_t bSwType;
	uint8_t bSwSubType;
	uint8_t bSwMajorVersion;
	uint8_t bSwMinorVersion;
	uint8_t bSwStorageSize;
	uint8_t bSwProtocol;

	uint8_t abUid[ 7 ];
	uint8_t abBatchNo[ 5 ];
	uint8_t bProductionCW;
	uint8_t bProductionYear;
}VERSION_INFO;

//Diversification  Commands
//*************************************************************************************
uint16_t CTOS_DesfireDiversifyDirectKey(uint16_t Option, uint8_t *InKey, uint16_t KeyType, uint8_t *DivInput, uint16_t DivInputLen, uint8_t *DiversifiedKey);

//DESFire API
//*************************************************************************************
uint16_t CTOS_DesfireAuthenticate(uint8_t KeyNo, uint8_t *InKey, uint16_t KeyLen);
uint16_t CTOS_DesfireAuthenticate_SAM(uint8_t Slot, uint8_t Channel, uint8_t KeyNo, uint8_t SamAv2_KeyNo, uint8_t SamAv2_KeyV);
uint16_t CTOS_DesfireAuthenticateISO(uint8_t KeyNo, uint8_t *InKey, uint16_t KeyLen);
uint16_t CTOS_DesfireAuthenticateISO_SAM(uint8_t Slot, uint8_t Channel, uint8_t KeyNo, uint8_t SamAv2_KeyNo, uint8_t SamAv2_KeyV);
uint16_t CTOS_DesfireAuthenticateAES(uint8_t KeyNo, uint8_t *InKey, uint16_t KeyLen);
uint16_t CTOS_DesfireAuthenticateAES_SAM(uint8_t Slot, uint8_t Channel, uint8_t KeyNo, uint8_t SamAv2_KeyNo, uint8_t SamAv2_KeyV);
uint16_t CTOS_DesfireChangeKeySettings(uint8_t KeySettings);
uint16_t CTOS_DesfireGetKeySettings(uint8_t *bKeySettings, uint8_t *bMaxNoOfKeys);
uint16_t CTOS_DesfireChangeKey(uint8_t bKeyNo, uint8_t *bOKey, uint16_t uOKeyLen, uint8_t *bNKey, uint16_t uNKeyLen);
uint16_t CTOS_DesfireChangeKeySAM(uint8_t bKeyNo, uint8_t SamAv2_OldKeyNo, uint8_t SamAv2_OldKeyVer, uint8_t SamAv2_NewKeyNo, uint8_t SamAv2_NewKeyVer);
uint16_t CTOS_DesfireGetKeyVersion(uint8_t KeyNo, uint8_t *KeyVersion);

//PICC Level Commands
uint16_t CTOS_DesfireCreateApplication(uint8_t *bAID, uint8_t bKeySett1, uint8_t bKeySett2, uint8_t *bISOFID, uint8_t *bDFName, uint16_t uNameLen, uint8_t EnablISO);
uint16_t CTOS_DesfireDeleteApplication(uint8_t *bAID);
uint16_t CTOS_DesfireGetApplicationIDs(uint8_t *bAID, uint16_t *uAIDLen);
uint16_t CTOS_DesfireGetDFNames(FILE_INFO *GetFile, uint16_t *TotalFile);
uint16_t CTOS_DesfireSelectApplication(uint8_t *bAID);
uint16_t CTOS_DesfireFormatPICC(void);
uint16_t CTOS_DesfireGetVersion(VERSION_INFO *GetVersion);
uint16_t CTOS_DesfireFreeMemory(uint32_t *ulSizeOfMemory);
uint16_t CTOS_DesfireSetConfiguration(uint8_t bOption, uint8_t *bData, uint16_t uDataLen);
uint16_t CTOS_DesfireGetCardUID(uint8_t *bUID);

//Application Level Commands
uint16_t CTOS_DesfireGetFileIDs(uint8_t *bFID, uint16_t *uFIDLen);
uint16_t CTOS_DesfireGetISOFileIDs(uint8_t *bFID, uint16_t *uFIDLen);
uint16_t CTOS_DesfireGetFileSettings(uint8_t bFileNo, uint8_t *bSettings, uint16_t *uSettingsLen);
uint16_t CTOS_DesfireChangeFileSettings(uint8_t bFileNo, uint8_t bCommMode, uint16_t uAccessRight, uint8_t bEncrypt);
uint16_t CTOS_DesfireCreateStdDataFile(uint8_t bFileNo, uint8_t *bISOFileID, uint8_t bComSet, uint16_t uAccessRight, uint32_t ulFileSize, uint8_t EnableISO);
uint16_t CTOS_DesfireCreateBackupDataFile(uint8_t bFileNo, uint8_t *bISOFileID, uint8_t bComSet, uint16_t ubAccessRight, uint32_t ulFileSize, uint8_t EnableISO);
uint16_t CTOS_DesfireCreateValueFile(uint8_t bFileNo,uint8_t bComSet,uint16_t bAccessRight, uint32_t ulLowerLimit, uint32_t ulUpperLimit, uint32_t ulValue, uint8_t bLimitedCreditEnabled);
uint16_t CTOS_DesfireCreateLinearRecordFile(uint8_t bFileNo, uint8_t *bISOFID, uint8_t bComSet, uint16_t uAccessRight, uint32_t ulRecordSize, uint32_t ulMaxNumOfRecords, uint8_t EnableISO);
uint16_t CTOS_DesfireCreateCyclicRecordFile(uint8_t bFileNo, uint8_t *bISOFID, uint8_t bComSet, uint16_t uAccessRight, uint32_t ulRecordSize, uint32_t ulMaxNumOfRecords, uint8_t EnableISO);
uint16_t CTOS_DesfireDeleteFile(uint8_t bFileNo);

//Data Manipulation Commands
uint16_t CTOS_DesfireReadData(uint8_t bFileNo, uint32_t ulOffSet, uint32_t ulLength, uint8_t *bGetData, uint32_t *ulDataLen);
uint16_t CTOS_DesfireWriteData(uint8_t bFileNo, uint32_t ulOffSet, uint32_t ulLength, uint8_t *WriteData);
uint16_t CTOS_DesfireGetValue(uint8_t bFileNo, uint32_t *ulGetValue);
uint16_t CTOS_DesfireCredit(uint8_t bFileNo, uint32_t ulCreditValue);
uint16_t CTOS_DesfireDebit(uint8_t bFileNo, uint32_t ulDebitValue);
uint16_t CTOS_DesfireLimitedCredit(uint8_t bFileNo, uint32_t ulCreditValue);
uint16_t CTOS_DesfireWriteRecord(uint8_t bFileNo, uint32_t ulOffSet, uint32_t ulLength, uint8_t *WriteData);
uint16_t CTOS_DesfireReadRecords(uint8_t bFileNo, uint32_t ulOffSet, uint32_t ulLength, uint8_t *bGetData, uint32_t *ulDataLen);
uint16_t CTOS_DesfireClearRecordFile(uint8_t bFileNo);
uint16_t CTOS_DesfireCommitTransaction(void);
uint16_t CTOS_DesfireAbortTransaction(void);

//EV2 Data Manipulation Commands
uint16_t CTOS_DesfireEV2ISOReadData(uint8_t bOption, uint8_t bFileNo, uint32_t ulOffSet, uint32_t ulLength, uint8_t *bGetData, uint32_t *ulDataLen);
uint16_t CTOS_DesfireIsoSelectFile(uint8_t bOption, uint8_t bSelector, uint8_t * pFid, uint8_t * pDFname, uint8_t bDFnameLen, uint8_t * pFCI, uint16_t * pFCILen);
//Config Commands
uint16_t CTOS_DesfireConfigSet(uint8_t ucConfig, uint32_t uiValue);
uint16_t CTOS_DesfireConfigGet(uint8_t ucConfig, uint32_t *puiValue);

uint16_t CTOS_DesfireGetLibraryVersion(uint8_t *strVersion);
void CTOS_3K3DES(uint8_t bType, uint8_t *InKey, uint16_t InKeyLen, uint8_t *InData, uint16_t InDataLen, uint8_t *OutData);

#ifdef __cplusplus
}
#endif
#endif

