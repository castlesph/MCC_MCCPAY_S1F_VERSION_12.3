#ifndef __CTOS_CLAPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------------------------
// Return Code
#define d_CL_OK							0x0000
#define d_CL_HAL_ERROR					0x8301
#define d_CL_TX_TIMEOUT					0x8302
#define d_CL_RX_TIMEOUT					0x8303
#define d_CL_CRC_INCORRECT				0x8304
#define d_CL_BUF_NOT_ENOUGH				0x8305

#define	d_CL_INIT_COM_ERROR				0x8331
#define d_CL_INVALID_HANDLE				0x8332
#define	d_CL_INVALID_PARA				0x8333
#define	d_CL_COMM_FAIL					0x8334
#define	d_CL_RSP_LEN_ERROR				0x8335
#define	d_CL_RSP_CMD_ERROR				0x8336
#define d_CL_OUT_OF_DATA_LEN			0x8337
#define d_CL_SN_INCORRECT				0x8338
#define d_CL_RSP_DATA_LEN_ERROR			0x8339
#define	d_CL_RSP_ID_ERROR				0x833A
#define d_CL_BASE_INDEX_ERROR			0x833B
#define d_CL_COM_ALREADY_OPEN			0x833C

#define d_CL_CMD_ID_INCORRECT			0x8345
#define d_CL_HOST_ID_INCORRECT			0x8346
#define d_CL_READER_ID_INCORRECT		0x8347
#define d_CL_CBN_ERROR					0x8348
#define d_CL_UNKNOWN_PROTOCOL_MODE		0x8349
#define d_CL_SET_DATETIME_ERROR			0x834A
#define d_CL_PENDING					0x8350
#define d_CL_TX_CANCEL					0x8351

// contactless error
#define d_CL_UNDEFINED_ERR				0x83FF
#define d_CL_CRCERR						0x83FE
#define d_CL_AUTHERR					0x83FC
#define d_CL_PARITYERR					0x83FB
#define d_CL_CODEERR					0x83FA
#define d_CL_SERNRERR					0x83F8
                                        
#define d_CL_KEYERR						0x83F7
#define d_CL_NOTAUTHERR					0x83F6
#define d_CL_BITCOUNTERR				0x83F5
#define d_CL_BYTECOUNTERR				0x83F4

#define d_CL_OVFLERR					0x83ED
#define d_CL_FRAMINGERR					0x83EB
#define d_CL_ACCESSERR					0x83EA
#define d_CL_UNKNOWN_COMMAND			0x83E9
#define d_CL_COLLERR					0x83E8
#define d_CL_INITERR					0x83E7
#define d_CL_INTERFACEERR				0x83E6
#define d_CL_ACCESSTIMEOUT				0x83E5
#define d_CL_NOBITWISEANTICOLL			0x83E4
#define d_CL_CODINGERR					0x83E2
                                        
#define d_CL_CASCLEVEX					0x83CC
#define d_CL_BAUDRATE_NOT_SUPPORTED		0x83CA
#define d_CL_SAME_BAUDRATE_REQUIRED		0x83C9
#define d_CL_RESPOND_TOO_EARLY			0x83C8
#define d_CL_WRONG_PARAMETER_VALUE		0x83C4
                                      
#define d_CL_NY_IMPLEMENTED				0x839C
#define d_CL_VALERR						0x8384
                                        
#define d_CL_TCL_ERR					0x8383  
#define d_CL_TCL_TIMEOUT				0x8382  
#define d_CL_TCL_ABORT					0x8381  

//------------------------------------------------------------------------------------------------
// value for Comm Channel baudrate 
#define d_COMM_DEFAULT_BAUDRATE			38400

// value for CL antenna parameter
#define d_CL_TYPE_A   					0
#define d_CL_TYPE_B   					1
#define d_CL_TYPE_C   					2
#define d_CL_BR_106   					0
#define d_CL_BR_212   					1
#define d_CL_BR_424   					2
#define d_CL_BR_848   					3

// value for CL AFI
#define d_CL_AFI_ALL					0x00

// value for NumAntiColSlot 
#define	d_CL_NUMSLOT_x1					0x00	
#define d_CL_NUMSLOT_x2					0x01
#define d_CL_NUMSLOT_x4					0x02
#define d_CL_NUMSLOT_x8					0x03
#define d_CL_NUMSLOT_x16				0x04
#define d_CL_SLOTMARKER_DISABLE				d_CL_NUMSLOT_x1

// value for CL FSDI
#define d_CL_IFDI_16					0x00
#define d_CL_IFDI_24					0x01
#define d_CL_IFDI_32					0x02
#define d_CL_IFDI_40					0x03
#define d_CL_IFDI_48					0x04
#define d_CL_IFDI_64					0x05
#define d_CL_IFDI_96					0x06
#define d_CL_IFDI_128					0x07
#define d_CL_IFDI_256					0x08
#define d_CL_IFDI_DEFAULT				d_CL_IFSDI_256

// value for CL MaxRate
#define d_CL_MAXRATE_106				106
#define d_CL_MAXRATE_212				212
#define d_CL_MAXRATE_424				424
#define d_CL_MAXRATE_848				848
#define d_CL_MAXRATE_DEFAULT			d_CL_MAXRATE_106
#define d_CL_MAXRATE_MAX				d_CL_MAXRATE_848

// value for LED Index
#define d_CL_LED_RED					0x01
#define d_CL_LED_GREEN					0x02
#define d_CL_LED_YELLOW					0x04
#define d_CL_LED_BLUE					0x08
#define d_CL_LED_EXT_RED				0x10
#define d_CL_LED_EXT_GREEN				0x20			
#define d_CL_LED_EXT_BLUE				0x40

// value for SAM Slot ID
#define d_CL_SC_SAM1					1
#define d_CL_SC_MAX_SOCKET				1

// value for SAM Volt
#define d_CL_SC_5V						1
#define d_CL_SC_3V						2
#define d_CL_SC_1_8V					3

// mask for SAM Status
#define MK_SC_PRESENT					0x01
#define MK_SC_ACTIVE					0x02

// value for SAM CardType
#define d_SC_TYPE_T0					0
#define	d_SC_TYPE_T1					1
 
//------------------------------------------------------------------------------------------------
// CL Open/Close Comm Channel
USHORT CTOS_CLInitComm(ULONG uiBaudRate);
void   CTOS_CLCloseComm(void);

// CL General
USHORT CTOS_CLInit(void);
USHORT CTOS_CLPowerOn(void); 
USHORT CTOS_CLPowerOff(void); 
USHORT CTOS_CLVerGet(ULONG* ulVer);
USHORT CTOS_CLSetBaudrate(BOOL fIsTypeA,UCHAR bTxBR,UCHAR bRxBR);
USHORT CTOS_ReadRC(UCHAR bAddr,UCHAR* bValue);
USHORT CTOS_WriteRC(UCHAR bAddr,UCHAR bValue);  
USHORT CTOS_GetAntennaParam(UCHAR bType,UCHAR bRate,UCHAR* baBuf);  
USHORT CTOS_TuneAntennaParam(UCHAR bType, UCHAR bRate,UCHAR* baBuf);
USHORT CTOS_CLSetPowerModeParm(BOOL fAuto, ULONG ulAutoOffTime,BOOL fLEDOn);
USHORT CTOS_CLGetPowerModeParm(BOOL *fAuto, ULONG *ulAutoOffTime,BOOL *fLEDOn);

// CL RC531 specific function
USHORT CTOS_MifareREADE2(UCHAR bAddrMSB,UCHAR bAddrLSB,UCHAR bNumByte,UCHAR* baBuf); 
USHORT CTOS_MifareWRITEE2(UCHAR bAddrMSB,UCHAR bAddrLSB,UCHAR bNumByte,UCHAR* baBuf);
USHORT CTOS_MifareLOADKEYE2(UCHAR bAddrMSB,UCHAR bAddrLSB);

// CL ISO14443-3 Type A
USHORT CTOS_REQA(UCHAR* baATQA);
USHORT CTOS_WUPA(UCHAR* baATQA);
USHORT CTOS_ANTIA(UCHAR bSelCode,UCHAR* baSNR,UCHAR bCnt);                              
USHORT CTOS_SELECTA(UCHAR bSelCode,UCHAR* baSNR,UCHAR* bSAK);      
USHORT CTOS_CLTypeAHalt(void);
USHORT CTOS_CLTypeAActiveFromIdle(BYTE bBaudRate,BYTE* baATQA,BYTE *bSAK,BYTE* baCSN,BYTE* bCSNLen);
USHORT CTOS_CLTypeAActiveFromHalt(BYTE bBaudRate,BYTE* baATQA,BYTE *bSAK,BYTE* baCSN,BYTE bCSNLen);

// CL Mifare function
USHORT CTOS_MifareLOADKEY(UCHAR* baKey);
USHORT CTOS_MifareAUTHEx(UCHAR baKeyType,UCHAR bBlockNr,UCHAR* baCardSN,UCHAR bCardSNLen); 
USHORT CTOS_MifareAUTH2Ex(UCHAR baKeyType,UCHAR bBlockNr,UCHAR* baKey,UCHAR* baCardSN,UCHAR bCardSNLen);
USHORT CTOS_MifareREADBLOCK(UCHAR bBlockNr,UCHAR* baBuf);       
USHORT CTOS_MifareWRITEBLOCK(UCHAR bBlockNr,UCHAR* baBuf);      
USHORT CTOS_MifareINCREASE(UCHAR bBlockNr,UCHAR* bValue); 
USHORT CTOS_MifareDECREASE(UCHAR bBlockNr,UCHAR* bValue);
USHORT CTOS_MifareRESTORE(UCHAR bBlockNr); 
USHORT CTOS_MifareTRANSFER(UCHAR bBlockNr);

// CL ISO14443-3 Type B
USHORT CTOS_REQB(UCHAR bAFI,UCHAR* baATQB,UCHAR* bATQBLen); 
USHORT CTOS_WUPB(UCHAR bAFI,UCHAR* baATQB,UCHAR* bATQBLen);
USHORT CTOS_ATTRIB(UCHAR* baPUPI,UCHAR* bRep);  
USHORT CTOS_HALTB(BYTE *baPUPI);
USHORT CTOS_CLTypeBActive(BYTE* baPUPI); 
USHORT CTOS_CLTypeBActiveEx(BYTE* baPUPI,UCHAR bAFI,BOOL fWUPB,UCHAR bFSDI,UCHAR bNumAntiColSlot,USHORT usMaxRate);

// CL ISO14443-4 T=CL
USHORT CTOS_CLRATS(BYTE bAutoBR,BYTE* baATS,USHORT* bATSLen); 
USHORT CTOS_CLRATSEx(BYTE bAutoBR, BYTE* baATS, USHORT* bATSLen, USHORT usMaxRate, BYTE bFSDI);
USHORT CTOS_CLAPDU(BYTE* baSBuf,USHORT usSLen,BYTE* baRBuf,USHORT* usRLen);
USHORT CTOS_CLDESELECT(void);
USHORT CTOS_CLNAKPOLL(void);

// CL Transparent CL Access
USHORT CTOS_TclTransparent(BYTE* baSBuf,USHORT usSLen,BYTE* baRBuf,USHORT* usRLen);  
USHORT CTOS_TclTransparentEx(BYTE* baSBuf,USHORT usSLen,BYTE* baRBuf,USHORT* usRLen,BOOL fTxCrcEnable,BOOL fRxCrcEnable);  
USHORT CTOS_TclSetTimeout(ULONG ulTimeout);

// CL ISO18092 Felica
USHORT CTOS_FelicaPolling(UCHAR* IDm, UCHAR* PMm);
USHORT CTOS_FelicaPollingEx(UCHAR* Payload, UCHAR* IDm, UCHAR* PMm);
USHORT CTOS_FelicaReadWrite(UCHAR* TxBuf,USHORT TxLen,UCHAR* RxBuf,USHORT* RxLen,USHORT ulTimeout);

// LED
USHORT CTOS_CLLEDSet(BYTE bLEDMask,BYTE bOnOffMask);

// BUZZER
USHORT CTOS_CLSound(USHORT usFreq,USHORT usDuration);

// SAM
USHORT CTOS_CLSCStatus(BYTE bID,BYTE* bStatus);
USHORT CTOS_CLSCReset(BYTE bID,BYTE bVolt,BYTE* baATR,BYTE* bATRLen,BYTE* CardType);
USHORT CTOS_CLSCSendAPDU(BYTE bID,BYTE* baSBuf,USHORT usSLen,BYTE* baRBuf,USHORT* usRLen);
USHORT CTOS_CLSCPowerOff(BYTE bID);

//CLM1000 Secure Communication, support Mifare load key only.
USHORT CTOS_InjectSecureCommKey(IN BYTE *baKey, IN BYTE bKeyLen);
USHORT CTOS_EstablishSecureComm(void);
USHORT CTOS_CloseSecureComm(void);


#ifdef __cplusplus
}
#endif


#endif
