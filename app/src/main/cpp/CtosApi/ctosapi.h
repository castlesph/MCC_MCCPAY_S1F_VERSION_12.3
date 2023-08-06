#ifndef __CTOS_API_H__
#define __CTOS_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <typedef.h>
#include <ctoserr.h>
#include <EMVCL_Client.h>
#include <CryptoClient.h>
#include <emv_cl.h>
#include <cadf_kms2.h>
#include <vwdleapi.h>

#define IN
#define OUT
#define INOUT

#define d_TRUE						1
#define d_FALSE						0

#define d_ON						1
#define d_OFF						0

//=============================================================================================================================
//
// Smart Card  & Memory Card Functions
//
//=============================================================================================================================

// value for bID
#define d_SC_USER							0
#define d_SC_SAM1							1
#define d_SC_SAM2							2
#define d_SC_SAM3							3
#define d_SC_SAM4							4
#define d_SC_MAX_SOCKET						4

// value for bVolt
#define d_SC_5V								1
#define d_SC_3V								2
#define d_SC_1_8V							3

// mask for bStatus
#define d_MK_SC_PRESENT						1
#define d_MK_SC_ACTIVE						2

// value for CardType of CTOS_ATR
#define d_SC_TYPE_T0						0
#define	d_SC_TYPE_T1						1
#define	d_SC_TYPE_MEM						2
#define d_SC_TYPE_I2C						3

USHORT CTOS_SCResetISO(BYTE bID, BYTE bVolt, BYTE* baATR, BYTE* baATRLen, BYTE* baCardType);
USHORT CTOS_SCSendAPDU(BYTE bID, BYTE* baSBuf, USHORT usSLen, BYTE* baRBuf, USHORT* pusRLen);

//=============================================================================================================================
//
// KMS2 Function
//
//=============================================================================================================================

//---------------------------------------------------------------------------------------------
// KeySets

// Reserved Keys for System
#define KMS2_RESERVED_KEY_SETS_0000				0x0000
#define KMS2_RESERVED_KEY_SETS_START			0xFF00
#define KMS2_RESERVED_KEY_SETS_END				0xFFFF

//---------------------------------------------------------------------------------------------

// Key Type
#define KMS2_KEYTYPE_3DES						0x01
#define KMS2_KEYTYPE_3DES_DUKPT					0x02 	// ANS X9.24 - 2004

// User Data Area
#define KMS2_PRIVATE_USER_DATA_MAX_SIZE			(16 * 1024)
#define KMS2_COMMON_USER_DATA_MAX_SIZE			(64 * 1024)

//---------------------------------------------------------------------------------------------
// PIN Block Type
#define KMS2_PINBLOCKTYPE_ANSI_X9_8_ISO_0						0x00
#define KMS2_PINBLOCKTYPE_ISBAN_REVERSIBLE_PIN_4B_MODE			0x01
#define KMS2_PINBLOCKTYPE_ISBAN_IRREVERSIBLE_PIN_4B_MODE		0x02

// PIN Protection Cipher Method
#define KMS2_PINCIPHERMETHOD_ECB									0x00
#define KMS2_PINCIPHERMETHOD_EMV_OFFLINE_PIN			0x02

typedef struct
{
	// Should be 0x00 or 0x01
	IN BYTE Version;

	struct
	{
		IN BYTE BlockType;
		IN BYTE PINDigitMaxLength;
		IN BYTE PINDigitMinLength;
	}PIN_Info;

	struct
	{
		IN USHORT CipherKeySet;
		IN USHORT CipherKeyIndex;
		IN BYTE	CipherMethod;

		// This is used for KeyType is KMS2_KEYTYPE_3DES/KMS2_KEYTYPE_AES
		// If SK_Length is 0, SK will not be calculated and used.
		IN BYTE	SK_Length;
		IN BYTE* pSK;
	}Protection;

	struct
	{
		// This is used for PAN if BlockType is KMS2_PINBLOCKTYPE_ANSI_X9_8_ISO_0.
		// Ths is used for terminal pseudo random if BlockType	is KMS2_PINBLOCKTYPE_ISBAN_REVERSIBLE_PIN_4B_MODE.
		// This is used for PK if BlockType	is KMS2_PINBLOCKTYPE_ISBAN_IRREVERSIBLE_PIN_4B_MODE.
		IN BYTE InLength;
		IN BYTE* pInData;

	}AdditionalData;

	// This is used for KeyType is KMS2_KEYTYPE_3DES_DUKPT
	struct
	{
		IN BOOL IsUseCurrentKey;
	}DUKPT_PARA;

	struct
	{
		INOUT USHORT EncryptedBlockLength;
		OUT	BYTE* pEncryptedBlock;
		OUT BYTE PINDigitActualLength;
	}PINOutput;

	struct
	{
		IN DWORD Timeout;
		IN BYTE AsteriskPositionX;
		IN BYTE AsteriskPositionY;
		IN BYTE NULLPIN;
		IN int (*piTestCancel)(void);
	}Control;

}CTOS_KMS2PINGET_PARA;

typedef struct
{
	// Should be 0x02
	IN BYTE Version;

	struct
	{
		IN BYTE BlockType;
		IN BYTE PINDigitMaxLength;
		IN BYTE PINDigitMinLength;
	}PIN_Info;

	struct
	{
		IN USHORT CipherKeySet;
		IN USHORT CipherKeyIndex;
		IN BYTE	CipherMethod;

		// This is used for KeyType is KMS2_KEYTYPE_3DES/KMS2_KEYTYPE_AES
		// If SK_Length is 0, SK will not be calculated and used.
		IN BYTE	SK_Length;
		IN BYTE* pSK;
	}Protection;

	struct
	{
		// This is used for PAN if BlockType is KMS2_PINBLOCKTYPE_ANSI_X9_8_ISO_0.
		// Ths is used for terminal pseudo random if BlockType	is KMS2_PINBLOCKTYPE_ISBAN_REVERSIBLE_PIN_4B_MODE.
		// This is used for PK if BlockType	is KMS2_PINBLOCKTYPE_ISBAN_IRREVERSIBLE_PIN_4B_MODE.
		IN BYTE InLength;
		IN BYTE* pInData;

	}AdditionalData;

	// This is used for KeyType is KMS2_KEYTYPE_3DES_DUKPT
	struct
	{
		IN BOOL IsUseCurrentKey;
	}DUKPT_PARA;

	struct
	{
		INOUT USHORT EncryptedBlockLength;
		OUT	BYTE* pEncryptedBlock;
		OUT BYTE PINDigitActualLength;
	}PINOutput;

	struct
	{
		IN DWORD Timeout;
		IN BYTE NULLPIN;
	}Control;

	struct
	{
		void (*OnGetPINDigit)(BYTE NoDigits);
		void (*OnGetPINCancel)(void);
		void (*OnGetPINBackspace)(BYTE NoDigits);
	}EventFunction;

}CTOS_KMS2PINGET_PARA_VERSION_2;
//---------------------------------------------------------------------------------------------
#define KMS2_DATAENCRYPTCIPHERMETHOD_ECB							0x00
#define KMS2_DATAENCRYPTCIPHERMETHOD_CBC							0x01

typedef struct
{
	// Should be 0x00 or 0x01
	IN BYTE Version;

	struct
	{
		IN USHORT CipherKeySet;
		IN USHORT CipherKeyIndex;
		IN BYTE	CipherMethod;

		// This is used for KeyType is KMS2_KEYTYPE_3DES/KMS2_KEYTYPE_AES
		// If SK_Length is 0, SK will not be calculated and used.
		IN BYTE	SK_Length;
		IN BYTE* pSK;

	}Protection;

	// This is used for KeyType is KMS2_KEYTYPE_3DES_DUKPT
	struct
	{
		IN BOOL IsUseCurrentKey;
	}DUKPT_PARA;

	struct
	{
		IN USHORT Length;
		IN BYTE* pData;

		IN USHORT ICVLength;
		IN BYTE* pICV;
	}Input;

	// This is not used if CipherMethod is KMS2_DATAENCRYPTCIPHERMETHOD_EXTPIN_ECB
	struct
	{
		OUT USHORT Length;
		OUT BYTE* pData;
	}Output;

}CTOS_KMS2DATAENCRYPT_PARA;

//---------------------------------------------------------------------------------------------
#define KMS2_MACMETHOD_CBC							0x00
#define KMS2_MACMETHOD_X9_19						0x01	// Retail MAC, done in one call

typedef struct
{
	// Should be 0x00 or 0x01
	IN BYTE Version;

	struct
	{
		IN USHORT CipherKeySet;
		IN USHORT CipherKeyIndex;
		IN BYTE	CipherMethod;

		// This is used for KeyType is KMS2_KEYTYPE_3DES/KMS2_KEYTYPE_AES
		// If SK_Length is 0, SK will not be calculated and used.
		IN BYTE	SK_Length;
		IN BYTE* pSK;
	}Protection;

	struct
	{
		BYTE Length;
		BYTE* pData;
	}ICV;

	// This is used for KeyType is KMS2_KEYTYPE_3DES_DUKPT
	struct
	{
		IN BOOL IsUseCurrentKey;
	}DUKPT_PARA;

	struct
	{
		IN USHORT Length;
		IN BYTE* pData;
	}Input;

	struct
	{
		OUT USHORT Length;
		OUT BYTE* pData;
	}Output;

}CTOS_KMS2MAC_PARA;

//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------

typedef struct
{
	// Should be 0x00 or 0x01
	IN BYTE Version;

	struct
	{
		IN USHORT KeySet;

		IN USHORT KeyIndex;

		// Only used for KeyType 3DES/3DES-DUKPT/AES
		IN BYTE CVLen;

		// Only used for KeyType RSA
		IN BYTE HashAlgorithm;

	}Input;

	struct
	{
		OUT BYTE KeyType;
		OUT BYTE KeyVersion;
		OUT DWORD KeyAttribute;
		OUT USHORT KeyLength;

		// Only used for KeyType 3DES/3DES-DUKPT/AES
		OUT BYTE* pCV;

		// Only used for KeyType RSA
		OUT USHORT KeyExponentLength;

		// Only used for KeyType RSA
		// Calculated with the following input data in order:
		//		Modulus Length - 2 bytes, MSB to LSB
		//		Modulus
		//		Exponent Length - 2 bytes, MSB to LSB
		//		Exponent
		OUT BYTE* pHash;
	}Output;

}CTOS_KMS2KEYGETINFO_PARA;

//USHORT CTOS_DES (BYTE bEncDec, BYTE *baKey, BYTE bKeyLen, BYTE *baData, USHORT usDataLen, BYTE *baResult);
USHORT CTOS_KMS2KeyGetInfo(IN CTOS_KMS2KEYGETINFO_PARA *pKeyGetInfoPara);
USHORT CTOS_KMS2KeyCheckAll(void);
//USHORT CTOS_KMS2Init(void);
USHORT CTOS_KMS2DataEncrypt(CTOS_KMS2DATAENCRYPT_PARA *pDataEncPara);
USHORT CTOS_KMS2KeyCheck(IN USHORT KeySet, IN USHORT KeyIndex);
USHORT CTOS_KMS2MAC(CTOS_KMS2MAC_PARA *pMacPara);

//Time parameter
#define TIMER_ID_1		0
#define TIMER_ID_2		1
#define TIMER_ID_3		2
#define TIMER_ID_4		3

USHORT CTOS_KBDGet(BYTE* pbKey);
USHORT CTOS_RNG(BYTE *baResult);
USHORT CTOS_TimeOutSet(BYTE bTID, ULONG ulMSec); // in 10 ms
USHORT CTOS_Beep(void);
USHORT CTOS_KBDBufFlush(void);
USHORT CTOS_KBDInKeyCheck(BYTE *pbKey);
USHORT CTOS_KBDInKey(BOOL* fKeyIn);
USHORT CTOS_KMS2PINGet(CTOS_KMS2PINGET_PARA *pPinGetPara);

//System Wait Event
#define d_EVENT_KBD						0x00000001
#define d_EVENT_SC						0x00000002
#define d_EVENT_MSR						0x00000004
#define d_EVENT_MODEM					0x00000008
#define d_EVENT_ETHERNET				0x00000010
#define d_EVENT_COM1					0x00000020
#define d_EVENT_COM2					0x00000040
#define d_TIME_INFINITE					0xFFFFFFFF


//=============================================================================================================================
//
// RS232 Functions
//
//=============================================================================================================================

#define d_COM1						0
#define d_COM2						1
#define d_COM3                      2
#define d_COM4                      3

typedef struct
{
	// Should be 0x07
	IN BYTE Version;

	struct
	{
		IN BYTE BlockType;
		IN BYTE PINDigitMaxLength;
		IN BYTE PINDigitMinLength;
	}PIN_Info;

	struct
	{
		IN USHORT CipherKeySet;
		IN USHORT CipherKeyIndex;
		IN BYTE	CipherMethod;

		// This is used for KeyType is KMS2_KEYTYPE_3DES/KMS2_KEYTYPE_AES
		// If SK_Length is 0, SK will not be calculated and used.
		IN BYTE	SK_Length;
		IN BYTE* pSK;
	}Protection;

	struct
	{
		// This is used for PAN if BlockType is KMS2_PINBLOCKTYPE_ANSI_X9_8_ISO_0.
		// Ths is used for terminal pseudo random if BlockType	is KMS2_PINBLOCKTYPE_ISBAN_REVERSIBLE_PIN_4B_MODE.
		// This is used for PK if BlockType	is KMS2_PINBLOCKTYPE_ISBAN_IRREVERSIBLE_PIN_4B_MODE.
		IN BYTE InLength;
		IN BYTE* pInData;

	}AdditionalData;

	// This is used for KeyType is KMS2_KEYTYPE_3DES_DUKPT
	struct
	{
		IN BOOL IsUseCurrentKey;
	}DUKPT_PARA;

	struct
	{
		INOUT USHORT EncryptedBlockLength;
		OUT	BYTE* pEncryptedBlock;
		OUT BYTE PINDigitActualLength;
		OUT BYTE KSNLen;
		OUT BYTE* pKSN;
	}PINOutput;

	struct
	{
		IN DWORD Timeout;
		IN BYTE NULLPIN;
		IN int (*piTestCancel)(void);
		IN DWORD FirstKeyEntryTimeout;
	}Control;

	struct
	{
		void (*OnGetPINDigit)(BYTE NoDigits);
		void (*OnGetPINCancel)(void);
		void (*OnGetPINBackspace)(BYTE NoDigits);
		void (*OnGetPINOtherKeys)(BYTE bFunctionKey);
	}EventFunction;

}CTOS_KMS2PINGET_PARA_VERSION_7;


typedef struct
{
	BYTE    bSecond;
	BYTE    bMinute;
	BYTE    bHour;
	BYTE    bDay;
	BYTE    bMonth;
	BYTE    bYear;
	BYTE    bDoW;
} CTOS_RTC;

//=============================================================================================================================
//
// File System Functions
//
//=============================================================================================================================

// Storage Type
#define	d_STORAGE_FLASH					0
#define	d_STORAGE_SRAM					1

// Attribute
#define d_FA_PRIVATE					0
#define d_FA_READ						1		//	Read by other APs
#define d_FA_WRITE						2		//	Written by other APs
#define d_FA_PUBLIC						0xFF

#define d_SEEK_FROM_BEGINNING			0
#define	d_SEEK_FROM_CURRENT				1
#define	d_SEEK_FROM_EOF					2

//For the whole useless blocks in the file system.
#define	d_FORMAT_TYPE_0					0

//For the last ## blocks, the flash FAT size assigned by this AP, in the file system.
#define	d_FORMAT_TYPE_1

// patrick own define
#define ERR_MSG_DELAY			2500
#define OUR_START_TIME      473385600L
#define OUR_START_TIME_EPOCH      788918400L //change to 788918400 as epoch NETS_BASE_DATE. NETS src: 473385600L (this is epoch of 01 Jan 1985)

void CTOS_Delay(ULONG ulMSec);
USHORT CTOS_RTCGet (CTOS_RTC *pstRTC);
USHORT CTOS_TimeOutCheck(BYTE bTID);
USHORT CTOS_SystemWait(DWORD dwTimeout, DWORD dwWaitEvent, DWORD* pdwWakeupEvent);
USHORT CTOS_SCStatus(BYTE bID, BYTE* baStatus);

//=============================================================================================================================
//
// Keyboard Functions
//
//=============================================================================================================================

#define d_KBD_INVALID						0xFF

#define d_KBD_1                                                         '1'
#define d_KBD_2                                                         '2'
#define d_KBD_3                                                         '3'
#define d_KBD_4                                                         '4'
#define d_KBD_5                                                         '5'
#define d_KBD_6                                                         '6'
#define d_KBD_7                                                         '7'
#define d_KBD_8                                                         '8'
#define d_KBD_9                                                         '9'
#define d_KBD_0                                                         '0'

#define d_KBD_K1                                                        'X'
#define d_KBD_K2                                                        'Y'
#define d_KBD_K3                                                        'I'
#define d_KBD_K4                                                        'J'
#define d_KBD_UP							'U'
#define d_KBD_DOWN							'D'

#define d_KBD_CANCEL                                                    'C'
#define d_KBD_CLEAR                                                     'R'
#define d_KBD_ENTER							'A'
#define d_KBD_DOT							'Q'
#define d_KBD_00							'P'

#define d_KBD_X                                                         'Q'
#define d_KBD_COMMA                                                     'P'

#define d_KBD_CREDITCARD                                                'D'
#define d_KBD_SUBTOTAL                                                  'S'
#define d_KBD_POWER                                                     'W'
#define d_KBD_PERCENTAGE                                                'E'
#define d_KBD_PLU                                                       'U'
#define d_KBD_BACK                                                      'B'

#define d_KBD_F1                                                        'X'
#define d_KBD_F2                                                        'Y'
#define d_KBD_F3                                                        'I'
#define d_KBD_F4                                                        'J'
#define d_KBD_F5                                                        'K'
#define d_KBD_F6                                                        'L'
#define d_KBD_F7                                                        'M'
#define d_KBD_F8                                                        'N'

USHORT CTOS_KBDHit(BYTE* pbKey);

#define TCP_GPRS_STATE_ESTABLISHED 		0x00000002
#define TCP_GPRS_STATE_ESTABLISHING		0x00000010
#define TCP_GPRS_STATE_CONNECTING		0x00000020
#define TCP_GPRS_STATE_SENDING			0x00000040
#define TCP_GPRS_STATE_RECEIVING		0x00000080
#define TCP_GPRS_STATE_DISCONNECTING	0x00000100
#define TCP_GPRS_STATE_ONHOOKING		0x00000200

//=============================================================================================================================
//
// Modem/GPRS Setting Functions
//
//=============================================================================================================================

// for ATTRIB of CTOS_PrinterBufferPutString()
typedef struct
{
	USHORT FontSize;
	USHORT X_Zoom;
	USHORT Y_Zoom;
	USHORT X_Space;
	USHORT Y_Space;
} CTOS_FONT_ATTRIB;

ULONG CTOS_TickGet(void); // in 10 ms
USHORT CTOS_KBDBufPut(UCHAR bKey);


#define KMS2_VIRTUALPINPADMETHOD_SCRAMBLING         0x01
#define KMS2_VIRTUALPINPADMETHOD_MOVINGPINPAD       0x02
#define KMS2_VIRTUALPINPADMETHOD_CUSTOM             0x04


typedef struct
{
    USHORT x;
    USHORT y;
    USHORT width;
    USHORT height;
}BUTTON_BASIC;

typedef struct
{
    USHORT x;
    USHORT y;
    USHORT width;
    USHORT height;
    BYTE value;
}BUTTON_EXTERNAL;


typedef struct
{
    // Should be 0x00 or 0x01
    IN BYTE Version;

    struct
    {
        IN BYTE Method;
    }Common;

    struct
    {
        IN USHORT Xtop;
        IN USHORT Ytop;
        IN USHORT Xbottom;
        IN USHORT Ybottom;
        IN USHORT Xoffset;
        IN USHORT Yoffset;
    }MovingRange;

    struct
    {
        BUTTON_BASIC BUTTON_0;
        BUTTON_BASIC BUTTON_1;
        BUTTON_BASIC BUTTON_2;
        BUTTON_BASIC BUTTON_3;
        BUTTON_BASIC BUTTON_4;
        BUTTON_BASIC BUTTON_5;
        BUTTON_BASIC BUTTON_6;
        BUTTON_BASIC BUTTON_7;
        BUTTON_BASIC BUTTON_8;
        BUTTON_BASIC BUTTON_9;
        BUTTON_BASIC BUTTON_ENTER;
        BUTTON_BASIC BUTTON_CANCEL;
        BUTTON_BASIC BUTTON_BACKSPACE;

        BUTTON_EXTERNAL BUTTON_FUNCTIONKEY1;
        BUTTON_EXTERNAL BUTTON_FUNCTIONKEY2;
        BUTTON_EXTERNAL BUTTON_FUNCTIONKEY3;
    }CustomLocation;
}CTOS_KMS2SETVIRTUALPINPAD_PARA;


USHORT CTOS_KMS2SetVirtualPINPad(CTOS_KMS2SETVIRTUALPINPAD_PARA* pSetVirtualPINPad);


//#define ECN_SIZE				12
//#define FUNC_CODE_SIZE          2

//=============================================================================================================================
//
// Encryption/Decryption Functions
//
//=============================================================================================================================

#define d_ENCRYPTION				0
#define	d_DECRYPTION				1

//typedef struct
//{
//	unsigned long Message_Digest[5];	/* Message Digest (output)          */

//	unsigned long Length_Low;					/* Message length in bits           */
//	unsigned long Length_High;				/* Message length in bits           */

//	unsigned char Message_Block[64];	/* 512-bit message blocks      			*/
//	int Message_Block_Index;    			/* Index into message block array   */

//	int Computed; 										/* Is the digest computed?          */
//	int Corrupted;										/* Is the message digest corruped?  */
//}SHA_CTX;

typedef struct
{
	unsigned int tot_len;
	unsigned int len;
	unsigned char block[2 * 64];
	ULONG h[8];
}CTOS_SHA256_CTX;

typedef struct
{
	IN BYTE Version;

	IN USHORT bits;

	OUT USHORT m_len;
	OUT BYTE *m;
	OUT USHORT d_len;
	OUT BYTE *d;
	IN 	USHORT e_len;
	IN 	BYTE *e;

}CTOS_RSA_KEY_GEN;

void	VW_CloseComm(int iCom);

// Back Light parameter
#define d_BKLIT_LCD					0
#define d_BKLIT_KBD					1
#define d_BKLIT_LCD_DEVICE_0		d_BKLIT_LCD
#define d_BKLIT_LCD_DEVICE_1		2

USHORT CTOS_BackLightSetEx(BYTE bDevice, BYTE bOnOff, DWORD dwDuration);

//void CTOS_SHA1Init (SHA_CTX *pstInfo);
//void CTOS_SHA1Update (SHA_CTX *pstInfo, BYTE *baBuffer, USHORT usCount);
//void CTOS_SHA1Final (BYTE *baOutput, SHA_CTX *pstInfo);

// Additional Storage Functions
//USHORT CTOS_KMS2UserDataWrite(IN BOOL IsCommon, IN ULONG Offset, IN BYTE *pData, IN USHORT usLen);
//USHORT CTOS_KMS2UserDataRead(IN BOOL IsCommon, IN ULONG Offset, OUT BYTE *pData, IN USHORT usLen);
//USHORT CTOS_DES_CBC(BYTE bEncDec, BYTE *baKey, BYTE bKeyLen, BYTE *baICV, BYTE *baData, USHORT usDataLen, BYTE *baResult);
//USHORT CTOS_MAC (BYTE *baKey, BYTE bKeyLen, BYTE *baICV, BYTE *baData, USHORT usDataLen, BYTE *baMAC);
USHORT CTOS_KMS2DUKPTIncreaseKSN(IN USHORT KeySet, IN USHORT KeyIndex, IN BYTE Count);

USHORT CTOS_Sound(USHORT usFreq, USHORT usDuration); // in 10 ms

//=============================================================================================================================
//
// Printer Functions
//
//=============================================================================================================================
#define PAPER_X_SIZE			384

// Printer
USHORT CTOS_PrinterPutString(UCHAR* baBuf);

USHORT CTOS_PrinterStatus(void);

//=============================================================================================================================
//
// ULDPM Functions
//
//=============================================================================================================================

#define d_MAX_AP_TABLE				25

#define d_AP_FLAG_DEF_SEL			0x40

#define PROGRAM_UPDATE_WHOLE		0x00
//#define PROGRAM_UPDATE_START		0x01
//#define PROGRAM_UPDATE_CONTINUE		0x02
//#define PROGRAM_UPDATE_FINAL		0x03
//#define PROGRAM_FORCE_REBOOT		0x04

//Should be padding to 128 bytes
typedef struct
{
	BYTE bFlag;
	BYTE bCAPType;
	BYTE baGeneratedDateTime[13];
	BYTE baName[21];
	BYTE baVersion[2];
	BYTE baCompany[21];
	BYTE baExeName[25];
	BYTE RFU[44];
} CTOS_stCAPInfo;

USHORT CTOS_LCDSelectModeEx(BYTE bMode,BOOL fClear);
USHORT CTOS_LCDGShowBMPPic(USHORT usX, USHORT usY, BYTE *baFilename);

USHORT CTOS_GetFactorySN(BYTE *baFactorySN);

unsigned short usCTOS_DES(unsigned char bEncDec, unsigned char *baKey, unsigned char bKeyLen, unsigned char *baData, unsigned short usDataLen, unsigned char *baResult);

//Socket Device
#define d_TCP_DEVICE_ETHERNET			0
#define d_TCP_DEVICE_GPRS 				1
#define d_TCP_DEVICE_MODEM 				2
#define d_TCP_DEVICE_WIFI 				3

typedef struct{
	WORD TimeOut;
	BYTE MaxLEN;
	BYTE MinLEN;
	STR *sLine1Msg;
	STR *sLine2Msg;
	STR *sProcMsg;
	BYTE NULLPIN;
	BYTE LenPIN;
	int (*piTestCancel)(void);
	BYTE AsteriskPositionX;
	BYTE AsteriskPositionY;
	BYTE isRightJustified;
} CTOS_stDefEncOffPINStruc;

//=============================================================================================================================
//
// Font Functions
//
//=============================================================================================================================
// Font language
// ASCII < 0x80 is world-wide same
// ASCII >=0x80 is language dependent
#define d_TTF_C0_CONTROLS										  0x0000
#define d_TTF_BASIC_LATIN	 									  0x0020
#define d_TTF_C1_CONTROLS										  0x0080
#define d_TTF_LATIN_1_SUPPLEMENT                                  0x00A0
#define d_TTF_LATIN_EXTENDED_A                                    0x0100
#define d_TTF_LATIN_EXTENDED_B                                    0x0180
#define d_TTF_IPA_EXTENSIONS                                      0x0250
#define d_TTF_SPACING_MODIFIERS                                   0x02B0
#define d_TTF_COMBINING_DIACRITICS_MARKS                          0x0300
#define d_TTF_GREEK_AND_COPTIC                                    0x0370
#define d_TTF_CYRILLIC                                            0x0400
#define d_TTF_CYRILLIC_SUPPLEMENT                                 0x0500
#define d_TTF_ARMENIAN                                            0x0530
#define d_TTF_HEBREW                                              0x0590
#define d_TTF_ARABIC                                              0x0600
#define d_TTF_SYRIAC                                              0x0700
#define d_TTF_ARABIC_SUPPLEMENT                                   0x0750
#define d_TTF_THAANA                                              0x0780
#define d_TTF_N_KO                                                0x07C0
#define d_TTF_SAMARITAN                                           0x0800
#define d_TTF_MANDAIC                                             0x0840
#define d_TTF_DEVANAGARI                                          0x0900
#define d_TTF_BENGALI                                             0x0980
#define d_TTF_GURMUKHI                                            0x0A00
#define d_TTF_GUJARATI                                            0x0A80
#define d_TTF_ORIYA                                               0x0B00
#define d_TTF_TAMIL                                               0x0B80
#define d_TTF_TELUGU                                              0x0C00
#define d_TTF_KANNADA                                             0x0C80
#define d_TTF_MALAYALAM                                           0x0D00
#define d_TTF_SINHALA                                             0x0D80
#define d_TTF_THAI                                                0x0E00
#define d_TTF_LAO                                                 0x0E80
#define d_TTF_TIBETAN                                             0x0F00
#define d_TTF_MYANMAR                                             0x1000
#define d_TTF_GEORGIAN                                            0x10A0
#define d_TTF_HANGUL_JAMO                                         0x1100
#define d_TTF_ETHIOPIC                                            0x1200
#define d_TTF_ETHIOPIC_SUPPLEMENT                                 0x1380
#define d_TTF_CHEROKEE                                            0x13A0
#define d_TTF_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS               0x1400
#define d_TTF_OGHAM                                               0x1680
#define d_TTF_RUNIC                                               0x16A0
#define d_TTF_TAGALOG                                             0x1700
#define d_TTF_HANUNOO                                             0x1720
#define d_TTF_BUHID                                               0x1740
#define d_TTF_TAGBANWA                                            0x1760
#define d_TTF_KHMER                                               0x1780
#define d_TTF_MONGOLIAN                                           0x1800
#define d_TTF_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED      0x18B0
#define d_TTF_LIMBU                                               0x1900
#define d_TTF_TAI_LE                                              0x1950
#define d_TTF_NEW_TAI_LUE                                         0x1980
#define d_TTF_KHMER_SYMBOLS                                       0x19E0
#define d_TTF_BUGINESE                                            0x1A00
#define d_TTF_TAI_THAM                                            0x1A20
#define d_TTF_BALINESE                                            0x1B00
#define d_TTF_SUNDANESE                                           0x1B80
#define d_TTF_BATAK                                               0x1BC0
#define d_TTF_LEPCHA                                              0x1C00
#define d_TTF_OL_CHIKI                                            0x1C50
#define d_TTF_SUDANESE_SUPPLEMENT                                 0x1CC0
#define d_TTF_VEDIC_EXTENSIONS                                    0x1CD0
#define d_TTF_PHONETIC_EXTENSIONS                                 0x1D00
#define d_TTF_PHONETIC_EXTENSIONS_SUPPLEMENT                      0x1D80
#define d_TTF_COMBINING_DIACRITICS_MARKS_SUPPLEMENT               0x1DC0
#define d_TTF_LATIN_EXTENDED_ADDITIONAL                           0x1E00
#define d_TTF_GREEK_EXTENDED                                      0x1F00
#define d_TTF_GENERAL_PUNCTUATION                                 0x2000
#define d_TTF_SUPERSCRIPTS_AND_SUBSCRIPTS                         0x2070
#define d_TTF_CURRENCY_SYMBOLS                                    0x20A0
#define d_TTF_COMBINING_DIACRITICS_MARKS_FOR_SYMBOLS              0x20D0
#define d_TTF_LETTERLIKE_SYMBOLS                                  0x2100
#define d_TTF_NUMBER_FORM                                         0x2150
#define d_TTF_ARROWS                                              0x2190
#define d_TTF_MATHEMATICAL_OPERATOR                               0x2200
#define d_TTF_MISCELLANEOUS_TECHNICAL                             0x2300
#define d_TTF_CONTROL_PICTURES                                    0x2400
#define d_TTF_OPTICAL_CHARACTER_RECOGNITION                       0x2440
#define d_TTF_ENCLOSED_ALPHANUMERICS                              0x2460
#define d_TTF_BOX_DRAWING                                         0x2500
#define d_TTF_BLOCK_ELEMENT                                       0x2580
#define d_TTF_GEOMETRIC_SHAPES                                    0x25A0
#define d_TTF_MISCELLANEOUS_SYMBOLS                               0x2600
#define d_TTF_DINGBATS                                            0x2700
#define d_TTF_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A                0x27C0
#define d_TTF_SUPPLEMENTAL_ARROWS_A                               0x27F0
#define d_TTF_BRAILLE_PATTERNS                                    0x2800
#define d_TTF_SUPPLEMENTAL_ARROWS_B                               0x2900
#define d_TTF_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B                0x2980
#define d_TTF_SUPPLEMENTAL_MATHEMATICAL_OPERATOR                  0x2A00
#define d_TTF_MISCELLANEOUS_SYMBOLS_AND_ARROWS                    0x2B00
#define d_TTF_GLAGOLITIC                                          0x2C00
#define d_TTF_LATIN_EXTENDED_C                                    0x2C60
#define d_TTF_COPTIC                                              0x2C80
#define d_TTF_GEORGIAN_SUPPLEMENT                                 0x2D00
#define d_TTF_TIFINAGH                                            0x2D30
#define d_TTF_ETHIOPIC_EXTENDED                                   0x2D80
#define d_TTF_SUPPLEMENTAL_PUNCTUATION                            0x2E00
#define d_TTF_CJK_RADICALS_SUPPLEMENT                             0x2E80
#define d_TTF_KANGXI_RADICALS                                     0x2F00
#define d_TTF_IDEOGRAPHIC_DESCRIPTION_CHARACTERS                  0x2FF0
#define d_TTF_CJK_SYMBOLS_AND_PUNCTUATION                         0x3000
#define d_TTF_HIRAGANA                                            0x3040
#define d_TTF_KATAKANA                                            0x30A0
#define d_TTF_BOPOMOFO                                            0x3100
#define d_TTF_HANGUL_COMPATIBILITY_JAMO                           0x3130
#define d_TTF_KANBUN                                              0x3190
#define d_TTF_BOPOMOFO_EXTENDED                                   0x31A0
#define d_TTF_CJK_STROKES                                         0x31C0
#define d_TTF_KATAKANA_PHONETIC_EXTENSIONS                        0x31F0
#define d_TTF_ENCLOSED_CJK_LETTERS_AND_MONTHS                     0x3200
#define d_TTF_CJK_COMPATIBILITY                                   0x3300
#define d_TTF_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A                  0x3400
#define d_TTF_YIJING_HEXAGRAMS_SYMBOLS                            0x4DC0
#define d_TTF_CJK_UNIFIED_IDEOGRAPHS                              0x4E00
#define d_TTF_YI_SYLLABLES                                        0xA000
#define d_TTF_YI_RADICALS                                         0xA490
#define d_TTF_LISU                                                0xA4D0
#define d_TTF_VAI                                                 0xA500
#define d_TTF_CYRILLIC_EXTENDED_B                                 0xA640
#define d_TTF_BAMUM                                               0xA6A0
#define d_TTF_MODIFIER_TONE_LETTERS                               0xA700
#define d_TTF_LATIN_EXTENDED_D                                    0xA720
#define d_TTF_SYLOTI_NAGRI                                        0xA800
#define d_TTF_IND_NO                                              0xA830
#define d_TTF_PHAGS_PA                                            0xA840
#define d_TTF_SAURASHTRA                                          0xA880
#define d_TTF_DEVA_EXT                                            0xA8E0
#define d_TTF_KAYAH_LI                                            0xA900
#define d_TTF_REJANG                                              0xA930
#define d_TTF_JAVANESE                                            0xA980
#define d_TTF_CHAM                                                0xAA00
#define d_TTF_MYANMAR_EXTA                                        0xAA60
#define d_TTF_TAI_VIET                                            0xAA80
#define d_TTF_MEETEI_EXT                                          0xAAE0
#define d_TTF_ETHIOPIC_EXT_A                                      0xAB00
#define d_TTF_MEETEI_MAYEK                                        0xABC0
#define d_TTF_HANGUL_SYLLABLES                                    0xAC00
#define d_TTF_HANGUL_JAMO_EXTENDED_B                              0xD7B0
#define d_TTF_HIGH_HALF_ZONE_OF_UTF_16                            0xD800
#define d_TTF_LOW_HALF_ZONE_OF_UTF_16                             0xDC00
#define d_TTF_PRIVATE_USE_ZONE                                    0xE000
#define d_TTF_CJK_COMPATIBILITY_IDEOGRAPHS                        0xF900
#define d_TTF_ALPHABETIC_PRESENTATION_FORMS      				  0xFB00
#define d_TTF_ARABIC_PRESENTATION_FORMS_A                         0xFB50
#define d_TTF_VARIATION_SELECTOR                                  0xFE00
#define d_TTF_VERTICAL_FORMS                                      0xFE10
#define d_TTF_COMBINING_HALF_MARKS                                0xFE20
#define d_TTF_CJK_COMPATIBILITY_FORMS                             0xFE30
#define d_TTF_SMALL_FORM_VARIANTS                                 0xFE50
#define d_TTF_ARABIC_PRESENTATION_FORMS_B                         0xFE70
#define d_TTF_HALFWIDTH_AND_FULLWIDTH_FORMS                       0xFF00
#define d_TTF_SPECIALS                                            0xFFF0

#define d_FONT_EMPTY   											  0		  //No font
#define d_FONT_CHINESE_TAIWAN   								  1028	  //Chinese(Taiwan)
#define d_FONT_CZECH											  1029	  //Czech
#define d_FONT_JAPANESE											  1041    //Japanese
#define d_FONT_KOREAN											  1042    //Korean
#define d_FONT_PORTUGUESE_BRAZIL        						  1046    //Portuguese(Brazil)
#define d_FONT_RUSSIAN											  1049    //Russian
#define d_FONT_THAI  											  1054    //Thai
#define d_FONT_TURKISH 											  1055    //Turkish
#define d_FONT_FARSI											  1065    //Farsi
#define d_FONT_VIETNAMESE										  1066    //Vietnamese
#define d_FONT_CHINESE_PRC										  2052    //Chinese(PRC)
#define d_FONT_CHINESE_PRC1										  2054    //Simple(Song)
#define d_FONT_PORTUGUESE_PORTUGAL								  2070	  //Portuguese(Portugal)
#define d_FONT_SPANISH 											  3082    //Spanish(Spain)
#define d_FONT_ARABIC_QATAR										  16385   //Arabic(Qatar)
#define d_FONT_CHINESE_TAIWAN_PLUS 								  64507	  //Chinese(Taiwan) with Symbol
#define d_FONT_DEFAULTASCII             						  36864
// Font size
#define d_FONT_8x8												  0x0808
#define d_FONT_8x16												  0x0810
#define d_FONT_16x16											  0x1010
#define d_FONT_12x24											  0x0C18
#define d_FONT_24x24											  0x1818
#define d_FONT_9x9												  0x0909
#define d_FONT_9x18												  0x0912
#define d_FONT_16x30											  0x101E
#define d_FONT_20x40											  0x1428

// for printer font type set
#define d_FONT_NO_SET_TYPE										  0
#define d_FONT_TYPE1											  1
#define d_FONT_TYPE2											  2
#define d_FONT_TYPE3											  3

// for font usage
#define d_FONT_LCD_USE											  0x00
#define d_FONT_PRINTER_USE										  0x01
#define d_FONT_FNT_MODE   										  0
#define d_FONT_TTF_MODE   										  1
#define	d_FONT_DEFAULT_TTF										  "ca_default.ttf"

USHORT CTOS_FileGetSize(BYTE* caFileName, ULONG* pulFileSize);
//USHORT CTOS_FileOpen(BYTE* caFileName, BYTE bStorageType, FILE** pulFileHandle);
//USHORT CTOS_FileSeek(FILE** ulFileHandle, ULONG ulOffset, BYTE bOrigin);
//USHORT CTOS_FileClose(FILE** ulFileHandle);
//USHORT CTOS_FileWrite(FILE** ulFileHandle, BYTE* baBuffer, ULONG ulBufferLength);

//=============================================================================================================================
//
// LCD Functions
//
//=============================================================================================================================

// Graphic mode canvas x/y size
#define d_LCD_CANVAS_X_SIZE 									  128
#define d_LCD_CANVAS_Y_SIZE 									  (64*4)
// Graphic/text mode window x/y size
#define d_LCD_WINDOW_X_SIZE 									  128
#define d_LCD_WINDOW_Y_SIZE 									  64
// value for bMode
#define d_LCD_GRAPHIC_MODE										  0
#define d_LCD_TEXT_MODE											  1
#define d_LCD_GRAPHIC_HIGH_RES_MODE						  		  3
#define d_LCD_TEXT_HIGH_RES_MODE						  		  4

// value for English Size
#define d_LCD_FONT_8x8											  d_FONT_8x8
#define d_LCD_FONT_8x16											  d_FONT_8x16
#define d_LCD_FONT_12x24										  d_FONT_12x24

// value for bFill
#define d_LCD_FILL_0											  0
#define d_LCD_FILL_1											  1
#define d_LCD_FILL_XOR											  2

// for font shift
#define d_LCD_SHIFTLEFT											  0
#define d_LCD_SHIFTRIGHT										  1
#define d_LCD_SHIFTUP											  0
#define d_LCD_SHIFTDOWN											  1
#define d_LCD_WHITE 											  0xFFFFFF
#define d_LCD_BLACK 											  0x000000
#define d_LCD_BLUE 												  0xFF0000
#define d_LCD_GREEN 											  0x00FF00
#define d_LCD_RED 												  0x0000FF

// for show pic mode
#define d_LCD_SHOWPIC_MONO  									  0
#define d_LCD_SHOWPIC_RGB  										  1

#define d_LCD_ALIGNLEFT											  0
#define d_LCD_ALIGNCENTER										  1
#define d_LCD_ALIGNRIGHT										  2

//for LCD attribute
#define d_RESOLUTION_480x320		  ((480 << 16) | 320)
#define d_RESOLUTION_320x480          ((320 << 16) | 480)
#define d_RESOLUTION_320x240          ((320 << 16) | 240)
#define d_RESOLUTION_128x64			  ((128 << 16) | 64)
#define d_RESOLUTION_128x32			  ((128 << 16) | 32)
#define d_COLOR_MONO  			                                  1
#define d_COLOR_262K       		                                  2
#define d_COLOR_16M        		                                  3
#define d_TOUCH_NONE	   		                                  0
#define d_TOUCH_RESISTOR   		                                  1
#define d_TOUCH_CAPACITOR_1P	                                  2
//for LCDGPicInfoGet
#define d_PIC_WIDTH												  1
#define d_PIC_HEIGHT											  2



// For CTOS_LCDGMenu().
enum TAttribute {
	_taNormal  = 0x00,
	_taReverse = 0x01,
	//_taClrOne  = 0x02,
			_taRight   = 0x04,
	_taCenter  = 0x08,
	_taErrCode = 0x10,
	_taDelay   = 0x20
};
// For CTOS_LCDTMenu().
enum TAttribute_T {
	_taTNormal  = 0x00,
	_taTReverse = 0x01,
	//_taTClrOne  = 0x02,
			_taTRight   = 0x04,
	_taTCenter  = 0x08,
	_taTErrCode = 0x10,
	_taTDelay   = 0x20
};

// For CTOS_UIKeypad().
#define d_UI_KEYPAD_ENGLISH {" 0", "qzQZ1", "abcABC2", "defDEF3", "ghiGHI4", "jklJKL5", "mnoMNO6", "prsPRS7", "tuvTUV8", "wxyWXY9", ":;/\\|?,.<>", ".!@#$%^&*()"}
#define d_UI_KEYPAD_NUM_RAD {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "."}
#define d_UI_KEYPAD_NUM {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", ""}


//=============================================================================================================================
//
// Modem Functions
//
//=============================================================================================================================

// Para Mode
#define d_M_MODE_AYNC_NORMAL					1
#define d_M_MODE_AYNC_FAST						2
#define d_M_MODE_SDLC_NORMAL					3
#define d_M_MODE_SDLC_FAST						4
#define d_M_MODE_SDLC_FAST_EX					5

// HANDSHAKE
#define d_M_HANDSHAKE_V90_AUTO_FB				0
#define d_M_HANDSHAKE_V90_ONLY					1
#define d_M_HANDSHAKE_V34_AUTO_FB				2
#define d_M_HANDSHAKE_V34_ONLY					3
#define d_M_HANDSHAKE_V32BIS_AUTO_FB			4
#define d_M_HANDSHAKE_V32BIS_ONLY				5
#define d_M_HANDSHAKE_V22BIS_ONLY				6
#define	d_M_HANDSHAKE_V22_ONLY					7
#define d_M_HANDSHAKE_BELL_212					8
#define d_M_HANDSHAKE_BELL_103					9
#define d_M_HANDSHAKE_V21_ONLY					10
#define d_M_HANDSHAKE_V23						11

// Country Code
#define d_M_COUNTRY_TAIWAN						0
#define d_M_COUNTRY_CHINA						1
#define d_M_COUNTRY_USA							2
#define d_M_COUNTRY_GERMANY						3
#define d_M_COUNTRY_SPAIN						3
#define d_M_COUNTRY_FRANCE						4
#define d_M_COUNTRY_MALAYSIA					5
#define d_M_COUNTRY_ENGLISH						6
#define d_M_COUNTRY_THAILAND					7
#define d_M_COUNTRY_SINGAPORE					8
#define d_M_COUNTRY_JAPEN						9
#define d_M_COUNTRY_TURKEY						10
#define d_M_COUNTRY_RUSSIA						11
#define d_M_COUNTRY_HONGKONG					12
#define d_M_COUNTRY_INDONESIA					13
#define d_M_COUNTRY_AUSTRALIA					14
#define d_M_COUNTRY_BRAZIL						15
#define d_M_COUNTRY_CANADA						16
#define d_M_COUNTRY_SOUTH_AFRICA				17
#define d_M_COUNTRY_SOUTH_KOREA					18
#define d_M_COUNTRY_QATAR						19

// PreDialCheck
#define d_M_PRECHECK_NO_DETECT					0
#define	d_M_PRECHECK_DIAL_TONE_DETECT_ONLY		1
#define	d_M_PRECHECK_BUSY_DETECT_ONLY			2
#define	d_M_PRECHECK_DETECT_BOTH				3
#define	d_M_PRECHECK_DETECT_ALL					4

// CONFIG
#define d_M_CONFIG_DIALING_DURATION				1
#define d_M_CONFIG_MIN_ONHOOK_DURATION			2
#define d_M_CONFIG_PREDIAL_DELAY_TIME			3
#define d_M_CONFIG_DIAL_TONE_DETECT_DURATION	4
#define d_M_CONFIG_DIAL_TONE_MIN_ON_TIME		5
#define d_M_CONFIG_DTMF_ON_TIME					6
#define d_M_CONFIG_DTMF_OFF_TIME				7
#define d_M_CONFIG_BUSY_TONE_MIN_TOTAL_TIME		8
#define d_M_CONFIG_BUSY_TONE_DELTA_TIME			9
#define d_M_CONFIG_BUSY_TONE_MIN_ON_TIME		10
#define d_M_CONFIG_RINGBACK_TONE_MIN_TOTAL_TIME	11
#define d_M_CONFIG_RINGBACK_TONE_DELTA_TIME		12
#define d_M_CONFIG_RINGBACK_TONE_MIN_ON_TIME	13
#define d_M_CONFIG_ANSWER_TONE_WAIT_DURATION	14
#define d_M_CONFIG_BLIND_DIAL_DELAY_TIME		15
#define d_M_CONFIG_CARRIER_PRESENT_TIME			16
#define d_M_CONFIG_TX_POWER_LEVEL				17
#define d_M_CONFIG_RX_POWER_LEVEL				18

#define d_M_POWER_LEVEL_0						0
#define d_M_POWER_LEVEL_1						1
#define d_M_POWER_LEVEL_2						2
#define d_M_POWER_LEVEL_3						3	//default
#define d_M_POWER_LEVEL_4						4
#define d_M_POWER_LEVEL_5						5

// Status
#define d_M_STATUS_MODEM_OPEN					0x00000001
#define d_M_STATUS_MODEM_ONLINE					0x00000002
#define d_M_STATUS_SDLC_MODE					0x00000004
#define d_M_STATUS_SDLC_ONLINE					0x00000008
#define d_M_STATUS_DIALING						0x00000010
#define d_M_STATUS_NO_DIAL_TONE					0x00000020	// Dail up error
#define d_M_STATUS_LINE_BUSY					0x00000040	// Dail up error
#define d_M_STATUS_RING_BACK					0x00000080	// Dail up error
#define d_M_STATUS_TX_BUSY						0x00000100
#define d_M_STATUS_REMOTE_NOT_ANSWER			0x00000200	// Dail up error
#define d_M_STATUS_NO_CARRIER					0x00000400	// Dail up error
#define d_M_STATUS_ALL_DATA_SENT				0x00000800
#define d_M_STATUS_RX_DATA_VALID				0x00001000
#define d_M_STATUS_LISTENING					0x00002000
#define d_M_STATUS_RX_FRAME_ERROR				0x00004000	// Got enough frame error Rx buffer
#define d_M_STATUS_OTHER_ERROR					0x00008000	//Dail up error
#define d_M_STATUS_DATA_SENT_ERROR				0x00010000	//Over USHORT range
#define d_M_STATUS_DATA_RECEIVE_ERROR			0x00020000	//Over USHORT range
#define d_M_STATUS_TIMEOUT						0x00040000	//Over USHORT range

//Socket Device
#define d_TCP_DEVICE_ETHERNET			0
#define d_TCP_DEVICE_GPRS 				1
#define d_TCP_DEVICE_MODEM 				2
#define d_TCP_DEVICE_WIFI 				3

//Device Support
#define d_TCP_DEVICE_CA0				0x00000001
#define d_TCP_DEVICE_PPP0 				0x00000002
#define d_TCP_DEVICE_ETH0 				0x00000004
#define d_TCP_DEVICE_WLAN0 				0x00000008

//=============================================================================================================================
//
// Ethernet Functions
//
//=============================================================================================================================

// ConfigSet/Get
#define d_ETHERNET_CONFIG_IP					1
#define d_ETHERNET_CONFIG_MASK					2
#define d_ETHERNET_CONFIG_GATEWAY				3
#define d_ETHERNET_CONFIG_HOSTIP				4
#define d_ETHERNET_CONFIG_HOSTPORT				8
#define d_ETHERNET_CONFIG_VERSION				10
#define d_ETHERNET_CONFIG_MAC					11
#define d_ETHERNET_CONFIG_DHCP					12
#define d_ETHERNET_CONFIG_AUTOCON				14
#define d_ETHERNET_CONFIG_UPDATE_EXIT			15
#define d_ETHERNET_CONFIG_DNSIP					16
#define d_ETHERNET_CONFIG_HOSTURL				17
#define d_ETHERNET_CONFIG_CONNTIMEOUT			18

// Status
#define d_STATUS_ETHERNET_CONNECTED				0x00000001
#define d_STATUS_ETHERNET_PHYSICAL_ONLINE		0x00000010
#define d_STATUS_ETHERNET_RX_READY				0x00000020
#define d_STATUS_ETHERNET_TX_BUSY				0x00000040
#define d_STATUS_ETHERNET_COMMAND_MODE			0x00000080

#define ETHERNET_MS_CONNECTED_SOCKET0			0x00000100
#define ETHERNET_MS_CONNECTED_SOCKET1			0x00000200
#define ETHERNET_MS_CONNECTED_SOCKET2			0x00000400

// Multi-socket
#define d_ETHERNET_NUM_SOCKET			3
#define d_ETHERNET_MAX_SOCKET			2
#define d_ETHERNET_SOCKET_0				0
#define d_ETHERNET_SOCKET_1				1
#define d_ETHERNET_SOCKET_2				2

#define d_CTOS_WDIOC_SETTIMEOUT	1
#define d_CTOS_WDIOC_GETTIMEOUT	2
// definition for backward compatible
#define d_M_CONFIG_DAILING_DURATION				1
#define d_STATUS_ETHERNET_PHYICAL_ONLINE		0x00000010
#define d_LCD_CAVANS_X_SIZE              		128
#define d_LCD_CAVANS_Y_SIZE              		(64*4)
#define d_BATTERY_EXIST				d_MK_BATTERY_EXIST
#define d_BATTERY_CHARGE			d_MK_BATTERY_CHARGE
#define d_LCD_WINDOW_320x240_X_SIZE				320
#define d_LCD_WINDOW_320x240_Y_SIZE				240
#define d_LCD_GRAPHIC_320x240_MODE				3
#define d_LCD_TEXT_320x240_MODE					4
#define d_LCD_CANVAS_320x240_X_SIZE				320
#define d_LCD_CANVAS_320x240_Y_SIZE				(240*4)
#define ID_KMODEM_KO				6       //OLD
#define ID_CASC_KO				28      //OLD

//=============================================================================================================================
//
// Virtual Functions
// for V3 touch panelN
//
//=============================================================================================================================
typedef struct
{
	USHORT LeftTopX;
	USHORT LeftTopY;
	USHORT RightBottomX;
	USHORT RightBottomY;
}CTOS_VIRTUAL_FUNCTION_KEY_SET_PARA;

USHORT CTOS_VirtualFunctionKeySet(IN CTOS_VIRTUAL_FUNCTION_KEY_SET_PARA* pPara, IN BYTE FuncKeyCount);

//=============================================================================================================================
//
// CTMS Functions
//
//=============================================================================================================================

/** CTMS STATUS***/

#define d_CTMS_UPDATE_UNFINISHED	0x0000
#define d_CTMS_UPDATE_FINISHED 		0x0001
#define d_CTMS_UPDATE_ERR 			0x0002

#define d_CTMS_GPRS_CONFIG			0x01
#define d_CTMS_ETHERNET_CONFIG		0x02
#define d_CTMS_MODEM_CONFIG			0x03
#define d_CTMS_RECOUNT				0x04
#define d_CTMS_LOCALIP				0x05
#define d_CTMS_SERIALNUM			0x06
#define d_CTMS_WIFI_CONFIG			0x07
#define d_CTMS_COMPATIBLE			0x08
#define d_CTMS_NAC_CONFIG			0x09

// Reserve 0x09 for NAC
#define d_CTMS_TRANSFERSIZE			0x0A
#define d_CTMS_DOWNLOAD_CONFIG		0x0B

#define d_CTMS_UPLOAD_ADD			0x01
#define d_CTMS_UPLOAD_DELETE		0x02
#define d_CTMS_UPLOAD_DELETEALL		0x03

/** CTMS Tirgger MODE **/
#define d_CTMS_NORMAL_MODE			0x01
#define d_CTMS_DEFAULT_GPRS			0x02
#define d_CTMS_DEFAULT_MODEM		0x03
#define d_CTMS_DEFAULT_ETHERNET		0x04
#define d_CTMS_DEFAULT_WIFI			0x05

/** CTMS Get Information **/
/** NAC Tirgger MODE **/
#define d_CTMS_NAC_NORMAL				0x21
#define d_CTMS_NAC_DEFAULT_GPRS			0x22
#define d_CTMS_NAC_DEFAULT_MODEM		0x23
#define d_CTMS_NAC_DEFAULT_ETHERNET		0x24
#define d_CTMS_NAC_DEFAULT_WIFI			0x25

#define d_CTMS_INFO_LAST_UPDATE_TIME	0x01
#define d_CTMS_INFO_LAST_UPDATE_COUNT	0x02
#define d_CTMS_INFO_LAST_UPDATE_ITEM	0x03
#define d_CTMS_INFO_LAST_DOWNLOAD_COUNT	0x04
#define d_CTMS_INFO_LAST_DOWNLOAD_ITME	0x05
/** CTMS Upload Info **/
#define d_CTMS_INFO_LAST_UPLOAD_COUNT	0x06
#define d_CTMS_INFO_LAST_UPLOAD_ITEM	0x07

/** CTMS Compatible Mode **/
#define d_CTMS_COMPATIBLE_V1_4		0x00
#define d_CTMS_COMPATIBLE_V2_0		0x01

/** Modem Type **/
#define d_CTMS_TCP_MODEM			0x00
#define d_CTMS_MODEM				0x01

/** NAC Length Mode **/
#define d_NAC_HEX_MODE				0
#define d_NAC_BCD_MODE				1

/** CTMS Upload State **/
#define d_CTMS_UL_WAIT				0x00
#define d_CTMS_UL_PROCESSING		0x01
#define d_CTMS_UL_COMPLETE			0x02
#define d_CTMS_UL_FAIL				0x03

/** Download Configuration **/
#define d_CTMS_DLCONFIG_NORMAL		0
#define d_CTMS_DLCONFIG_PRMONLY		510

/** CTMS UPDATE TYPE **/
typedef struct{
	BYTE bNumber;
	BYTE baName[32];
	BYTE baVersion[8];
	BYTE baTypeName[32];
	USHORT usUpdateResult; // refer to CTOS_UpdateFromMMCIEx return code.
}__attribute__((packed)) CTMS_ItemInfo;

typedef struct{
	BYTE bType;	//Bit1:FW, Bit2:AP, Bit3:Share Lib, Bit4:APP Lib, Bit5:Share File, Bit6:APP File, Bit7:PRM File, Bit8:NULL
	BYTE bNums;	//Number of update
	CTOS_RTC stRTC; //Last got update information time
	BYTE RFU[10];
}__attribute__((packed)) CTMS_UpdateInfo;

/** CTMS Upload Count and Info **/
typedef struct{
	BYTE bTotalCount;
	BYTE bUploadCount;
}__attribute__((packed)) CTMS_UploadCount;

typedef struct{
	BYTE bState;
	BYTE baFilePath[64];
	CTOS_RTC stUploadTime;
	BYTE RFU[4];
}__attribute__((packed)) CTMS_UploadInfo;


typedef struct {
	BYTE bSIMSlot; // The SIM slot ID
	BYTE RFU[3];
	STR strAPN[32]; // APN name

	STR strID[32]; // The ID string used for the CHAP authentication
	STR strPW[32]; // The password string used for the CHAP authentication

	STR strRemoteIP[40]; // Remote IP address
	USHORT usRemotePort; // Remote port number
	BYTE RFU1[2];
	STR strLocalIP[40]; // Local IP address

	ULONG ulSIMReadyTimeout; // The timeout value, in millisecond, for checking if SIM is ready
	ULONG ulGPRSRegTimeout; // The timeout value, in millisecond, for checking if GPRS is registered

	USHORT usPPPRetryCounter; // The times of retry in PPP layer
	BYTE RFU2[2];
	ULONG ulPPPTimeout; // The timeout value in millisecond in PPP layer

	USHORT usTCPRetryCounter; // The times of retry in TCP layer
	BYTE RFU3[2];
	ULONG ulTCPConnectTimeout; // The timeout value, in millisecond, for connection in TCP layer
	ULONG ulTCPTxTimeout; // The timeout value, in millisecond, for transmitting in TCP layer
	ULONG ulTCPRxTimeout; // The timeout value, in millisecond, for receiving in TCP layer

	//BYTE*	pbRxStack;					// The pointer to Received stack
} __attribute__((packed)) CTMS_GPRSInfo;

typedef struct {
	BYTE bMode; // The mode of Modem connection
	BYTE bHandShake; // The handshakes of modem connection
	BYTE bCountryCode; // Country Code
	BYTE RFU[1];
	BYTE strRemotePhoneNum[32]; // The dial number
	BYTE strID[32]; // The ID string used for the CHAP authentication
	BYTE strPW[32]; // The password string used for the CHAP authentication

	BYTE strRemoteIP[40]; // Remote IP address
	USHORT usRemotePort; // Remote port number
	BYTE RFU1[2];

	STR strLocalIP[40]; // Local IP address

	ULONG ulDialTimeout; // The timeout value in millisecond for dialing up

	USHORT usPPPRetryCounter; // The times of retry in PPP layer
	BYTE RFU2[2];

	ULONG ulPPPTimeout; // The timeout value in millisecond in PPP layer

	USHORT usTCPRetryCounter; // The times of retry in TCP layer
	BYTE RFU3[2];
	ULONG ulTCPConnectTimeout; // The timeout value, in millisecond, for connection in TCP layer
	ULONG ulTCPTxTimeout; // The timeout value, in millisecond, for transmitting in TCP layer
	ULONG ulTCPRxTimeout; // The timeout value, in millisecond, for receiving in TCP layer
	//Add for NAC(Modem)
	BYTE bType; //d_CTMS_TCP_MODEM, d_CTMS_MODEM
	ULONG	ulBaudRate;
	BYTE	bParity;
	BYTE	bDataBits;
	BYTE	bStopBits;
	USHORT	usDialDuration;				// Dialing maximum total duration in second. Range: 0~255
	USHORT	usDTMFOnTime;				// DTMF on time in ms.
	USHORT	usDTMFOffTime;				// DTMF off time in ms.
	BYTE	bTxPowerLevel; 				// Tx Power Level(refer to Modem Functions)
	BYTE	bRxPowerLevel;				// Rx Power Level(refer to Modem Functions)

	//BYTE*	pbRxStack;					// The pointer to Received stack
}__attribute__((packed)) CTMS_ModemInfo;

typedef struct {
	//BOOL	boSave;						// Save all setting to module or end of information retrieve on Ethernet module

	STR strRemoteIP[40]; // Remote IP address
	USHORT usRemotePort; // Remote port number
	//BYTE RFU[2];
	BYTE bDHCP;
	BYTE RFU;
	STR strLocalIP[40]; // Local IP address

	STR strMask[40]; // Network mask
	STR strGateway[40]; // Gateway address

}__attribute__((packed)) CTMS_EthernetInfo;

typedef struct
{
	BYTE IsAutoConnect;	//if this parameter set to 1 then wifi opne will auto connect to AP
	BYTE IsHidden;
	BYTE baPassword[64];
	BYTE baSSid[36];
	BYTE bProtocal; //Refer Wifi Setting
	BYTE bGroup;	//Refer Wifi Setting
	BYTE bPairwise;	//Refer Wifi Setting
	STR strRemoteIP[40]; // Remote IP address
	USHORT usRemotePort; // Remote port number
	BYTE bDHCP;
	STR strLocalIP[40]; // Local IP address
	STR strMask[40]; // Network mask
	STR strGateway[40]; // Gateway address
	BYTE bSCAN_Mode;
	BYTE RFU[1];

} __attribute__((packed)) CTMS_WIFIInfo;

typedef struct
{
	USHORT GPRS_Size;
	USHORT Modem_Size;
	USHORT Ethernet_Size;

} __attribute__((packed)) CTMS_TransferSize;


typedef struct {

	BYTE bProtocol;
	USHORT usBlockSize;
	BYTE baSourceAddr[2];
	BYTE baDestAddr[2];
	BYTE bLenType;
	BYTE bAddLenFlag;

}__attribute__((packed)) CTMS_NacInfo;

// Hw Settings
#define d_MK_HW_MODEM       0x0001
#define d_MK_HW_ETHERNET    0x0002
#define d_MK_HW_GPRS        0x0004
#define d_MK_HW_CONTACTLESS 0x0008
#define d_MK_HW_ZIGBEE      0x0010
#define d_MK_HW_WIFI		0x0020
#define d_MK_HW_BT		0x0040
#define d_MK_HW_BARCODE     0x0400

//Protocol
#define d_WIFI_PROTOCOL_WEP    			0x01
#define d_WIFI_PROTOCOL_WPA    			0x02
#define d_WIFI_PROTOCOL_WPA2    		0x03

//Pairwise
#define d_WIFI_PAIRWISE_TKIP    		0x01
#define d_WIFI_PAIRWISE_CCMP    		0x02
#define d_WIFI_PAIRWISE_TKIPCCMP    	0x03

//Group
#define d_WIFI_GROUP_TKIP    			0x01
#define d_WIFI_GROUP_CCMP    			0x02
#define d_WIFI_GROUP_TKIPCCMP    		0x03

//=============================================================================================================================
//
// LED & Back Light Functions
//
//=============================================================================================================================

//LED  parameter
#define d_LED1						0	// => RED
#define d_LED2						1	// => GREEN
#define d_LED3						2	// => ORANGE
#define d_LED4						3	// => BLUE

#define d_LED_RED					0
#define d_LED_GREEN					1
#define d_LED_ORANG					2
#define d_LED_BLUE					3

// Back Light parameter
#define d_BKLIT_LCD					0
#define d_BKLIT_KBD					1
#define d_BKLIT_LCD_DEVICE_0		d_BKLIT_LCD
#define d_BKLIT_LCD_DEVICE_1		2

// Power Source
#define d_PWRSRC_DCJACK      			0
#define d_PWRSRC_CRADLE      			1
#define d_PWRSRC_BATTERY     			2
#define d_PWRSRC_USB              3

// definition for battery status
#define d_MK_BATTERY_EXIST				0x00000001
#define d_MK_BATTERY_CHARGE				0x00000002
#define d_MK_BATTERY_FULL				0x00000004

//=============================================================================================================================
//
// Mobile Functions
//
//=============================================================================================================================
#define d_MOBILE_NETWORK_GPRS			0x0001
#define d_MOBILE_NETWORK_UMTS			0x0002
#define d_MOBILE_NETWORK_CDMA			0x0004
#define d_MOBILE_NETWORK_LTE 			0x0008
#define d_MOBILE_NETWORK_AUTO			0x8000

// Packet Service Network Type
#define	d_GSM_NETWORK_GPRS				0
#define	d_GSM_NETWORK_EGPRS				1
#define	d_GSM_NETWORK_WCDMA				2
#define	d_GSM_NETWORK_HSDPA				3
#define	d_GSM_NETWORK_UNKNOWN			4

// for TTF swich display mode
#define d_TTF_MODE_DEFAULT                						  0x0000
#define d_TTF_MODE_VIETNAM                						  0xA001
#define d_TTF_MODE_BURMA									  0xA002

//=============================================================================================================================
//
// USB Functions
//
//=============================================================================================================================
#define d_MK_USB_STATUS_CDCMODE		0x00000001
#define d_MK_USB_STATUS_HOSTMODE	0x00000002

#define d_USB_DEVICE_MODE				0
#define d_USB_HOST_MODE					1

//=============================================================================================================================
//
// Power Saving Functions
//
//=============================================================================================================================
// value for bMode
#define	d_PWR_STANDBY_MODE				0x00
#define	d_PWR_SLEEP_MODE				0x01
#define	d_PWR_REBOOT					0x02
#define	d_PWR_POWER_OFF					0x03

USHORT CTOS_APGet(USHORT usAPIndex, CTOS_stCAPInfo *stInfo);
USHORT CTOS_LCDTPrintXY(USHORT usX, USHORT usY, UCHAR* pbBuf);
USHORT CTOS_LCDTClearDisplay(void);
USHORT CTOS_EthernetOpenEx(void);
USHORT CTOS_EthernetClose(void);
USHORT CTOS_EthernetConfigSet(BYTE bTag, BYTE* baValue, BYTE bLen);
USHORT CTOS_EthernetConfigGet(BYTE bTag, BYTE* baValue, BYTE* pbLen);
USHORT CTOS_EthernetConnectEx(BYTE* baDestIP, BYTE bIPLen, BYTE* baPort, BYTE bPortLen);
USHORT CTOS_EthernetDisconnect(void);
USHORT CTOS_EthernetFlushRxData(void);
USHORT CTOS_EthernetTxReady(void);
USHORT CTOS_EthernetTxData(BYTE* baData, USHORT usLen);
USHORT CTOS_EthernetStatus(DWORD* pdwStatus);
USHORT CTOS_EthernetRxReady(USHORT* pusLen);
USHORT CTOS_EthernetRxData(BYTE* baData, USHORT* pusLen);
USHORT CTOS_ModemOpenEx(BYTE bMode, BYTE bHandShake, BYTE bCountryCode);
USHORT CTOS_ModemATCommand(BYTE* baCmd, USHORT usCmdLen, BYTE* pbResponse, USHORT* pusRespLen);
USHORT CTOS_ModemDialup(BYTE *baNumber, USHORT usLen);
USHORT CTOS_ModemStatus(DWORD* pdwStatus);
USHORT CTOS_ModemHookOn(void);
USHORT CTOS_ModemFlushRxData(void);
USHORT CTOS_ModemTxReady(void);
USHORT CTOS_ModemTxData(BYTE *baData, USHORT usLen);
USHORT CTOS_ModemRxReady(USHORT* pusLen);
USHORT CTOS_ModemRxData(BYTE* baData, USHORT* pusLen);
USHORT CTOS_ModemCloseEx(void);
void CTOS_TCP_GPRSInit(void);
USHORT CTOS_TCP_GPRSClose(void);
USHORT CTOS_TCP_GPRSOpen(BYTE *baIP, STR *strAPN, STR *baID, STR *baPW);
USHORT CTOS_TCP_GPRSGetIP(BYTE *baIP);
USHORT CTOS_TCP_GPRSConnectEx(BYTE *bSocket, BYTE *baIP, USHORT usPort);
USHORT CTOS_TCP_GPRSDisconnect(BYTE bSocket);
USHORT CTOS_TCP_GPRSTx(BYTE bSocket, BYTE *baBuffer, USHORT usLen);
USHORT CTOS_TCP_GPRSRx(BYTE bSocket, BYTE *baBuffer, USHORT *usLen);
USHORT CTOS_USBTxFlush(void);
USHORT CTOS_TCP_GPRSStatus (DWORD* pdwState);
USHORT CTOS_GSMSignalQuality(BYTE *bpStrength);
USHORT CTOS_EthernetOpen(void);
void EMVCL_GetAPIVersion(char *cVer);
void EMVCL_Close(void);
USHORT CTOS_BackLightSet (BYTE bDevice, BYTE bOnOff);
ULONG 	VW_InitializeCommunication(BYTE bKeyType,BYTE bKeyIndex,BYTE *baRNDB,AUTHKEY_DATA *stKeyData);
USHORT CTOS_USBRxFlush(void);
ULONG 	VW_GenerateKeys(AUTHKEY_DATA *stKeyData);
ULONG 	VW_SetSessionKey(BYTE bKeyID,BYTE *baKeyData);
ULONG	VW_InitTransactionEx(BYTE bTagNum, BYTE *baTransData, USHORT usTransDataLen);
ULONG EMVCL_PollTransactionEx(EMVCL_RC_DATA_EX *stRCDataEx,ULONG ulMS);
ULONG	VW_PollTransactionEx(RC_DATA_EX *stRCDataEx,ULONG ulMS);
ULONG EMVCL_SetTimeOut(ULONG ulMS);
ULONG EMVCL_GetDateTime(BYTE *baDateTime);
ULONG	VW_CancelTransaction(void);
void	VW_AnalyzeTransactionEx(RC_DATA_EX *stRCData, RC_DATA_ANALYZE *stRCDataAnalyze);
ULONG	VW_InitComm(int iCom, ULONG uiBaudRate);
void	VW_GetAPIVersion(char *cVer);
USHORT CTOS_LCDTPrintAligned(USHORT usY, UCHAR* pbBuf, BYTE bMode);
USHORT CTOS_USBHostOpen(IN USHORT usVendorID, IN USHORT usProductID);
USHORT CTOS_USBHostTxData(IN BYTE* baSBuf, IN ULONG ulTxLen, IN ULONG ulMSec);
USHORT CTOS_USBHostRxData(OUT BYTE* baRBuf, IN OUT ULONG* ulRxLen, IN ULONG ulMSec);
USHORT CTOS_USBHostClose(void);
USHORT CTOS_KMS2DUKPTGetKSN(IN USHORT KeySet, IN USHORT KeyIndex, OUT BYTE* pKSN, INOUT BYTE* KSNLen);
USHORT CTOS_LCDBackGndColor(ULONG ulColor);
USHORT CTOS_LCDForeGndColor(ULONG ulColor);
USHORT CTOS_LCDFontSelectMode(BYTE bMode);
USHORT CTOS_BatteryGetCapacityByIC(BYTE* bPercentage);
void CTOS_PowerOff(void);
USHORT CTOS_LCDGShowPic(USHORT usX, USHORT usY, BYTE* baPat, ULONG ulPatLen, USHORT usXSize);
USHORT CTOS_LCDTClear2EOL(void);
USHORT CTOS_LCDTGotoXY(USHORT usX,USHORT usY);
USHORT CTOS_LCDTPutchXY (USHORT usX, USHORT usY, UCHAR bChar);
USHORT CTOS_CLInit(void);
USHORT CTOS_DesfireCreateBackupDataFile(BYTE bFileNo, BYTE *bISOFileID, BYTE bComSet, USHORT ubAccessRight, ULONG ulFileSize, BOOL EnableISO);
USHORT CTOS_DesfireReadData(BYTE bFileNo, ULONG ulOffSet, ULONG ulLength, BYTE *bGetData, ULONG *ulDataLen);
USHORT CTOS_LCDTPrint(UCHAR* sBuf);
USHORT CTOS_MifareREADBLOCK(UCHAR bBlockNr,UCHAR* baBuf);
USHORT CTOS_MifareTRANSFER(UCHAR bBlockNr);
USHORT CTOS_MifareDECREASE(UCHAR bBlockNr,UCHAR* bValue);
USHORT CTOS_MifareREADBLOCK(UCHAR bBlockNr,UCHAR* baBuf);
USHORT CTOS_MifareWRITEBLOCK(UCHAR bBlockNr,UCHAR* baBuf);
USHORT CTOS_MifareAUTHEx(UCHAR baKeyType,UCHAR bBlockNr,UCHAR* baCardSN,UCHAR bCardSNLen);
USHORT CTOS_MifareLOADKEY(UCHAR* baKey);
USHORT CTOS_LanguagePrinterFontSize(USHORT usFontSize,USHORT usFontStyle, USHORT usSetType);
USHORT CTOS_PrinterLogo(BYTE* baLogo, USHORT usXstart, USHORT usXsize, USHORT usY8Size);
USHORT CTOS_LCDTSetReverse(BOOL boReverse);
USHORT CTOS_SCResetEMV(BYTE bID, BYTE bVolt, BYTE* baATR, BYTE* baATRLen, BYTE* baCardType);
USHORT CTOS_RSA(BYTE *baModulus, USHORT usModulousLen, BYTE *baExponent, USHORT usExponentLen, BYTE *baData, BYTE *baResult);
USHORT CTOS_FelicaReadWrite(UCHAR* TxBuf,USHORT TxLen,UCHAR* RxBuf,USHORT* RxLen,USHORT ulTimeout);
USHORT CTOS_SCPowerOff(BYTE bID);
USHORT CTOS_CLPowerOn(void);
USHORT CTOS_LEDSet(BYTE bLED, BYTE bOnOff);
USHORT CTOS_USBSelectMode(BYTE bMode);
void CTOS_SystemReset(void);
USHORT CTOS_VirtualFunctionKeySet(IN CTOS_VIRTUAL_FUNCTION_KEY_SET_PARA* pPara, IN BYTE FuncKeyCount);
USHORT CTOS_VirtualFunctionKeyHit(BYTE *key);
USHORT CTOS_DesfireCreateLinearRecordFile(BYTE bFileNo, BYTE *bISOFID, BYTE bComSet, USHORT uAccessRight, ULONG ulRecordSize, ULONG ulMaxNumOfRecords, BOOL EnableISO);
USHORT CTOS_MifareINCREASE(UCHAR bBlockNr,UCHAR* bValue);
USHORT CTOS_FelicaPolling(UCHAR* IDm, UCHAR* PMm);
USHORT CTOS_LCDSetContrast(BYTE bValue);
USHORT CTOS_HWSettingGet(BOOL* fPortable,BOOL* fPCI,USHORT* mkHWSupport);
USHORT CTOS_PrinterTTFSwichDisplayMode(USHORT usMode);
USHORT CTOS_PrinterTTFSelect(BYTE *baFilename, BYTE bIndex);
USHORT CTOS_SetPMEnterPassword(BYTE *baPasswordString, BOOL IsEnable);
USHORT CTOS_SetFunKeyPassword(BYTE *baPasswordString, BOOL IsEnable);
int inCTOSS_GetRemovableStorageStatus(void);
USHORT CTOS_LCDTSelectFontSize(USHORT usFontSize);
USHORT CTOS_BatteryGetCapacity(BYTE* bPercentage);
USHORT CTOS_BatteryStatus(DWORD* pdwStatus);
USHORT CTOS_PrinterBMPPic(USHORT usX, BYTE *baFilename);
USHORT CTOS_PrinterSetHeatLevel(UCHAR bHeatLevel);
USHORT CTOS_PrinterFline(USHORT usLines);
USHORT CTOS_PrinterBufferPutString(BYTE* pbPtr, USHORT usXPos, USHORT usYPos, BYTE *baStr, CTOS_FONT_ATTRIB* ATTRIB);
USHORT CTOS_KeyInjectionPerform(IN void *pRFU_IN, OUT void *pRFU_OUT);
USHORT CTOS_LCDTTFSwichDisplayMode(USHORT usMode);
USHORT CTOS_LCDTTFSelect(BYTE *baFilename, BYTE bIndex);
USHORT CTOS_UIKeypad(USHORT usX, USHORT usY, STR *pcaKeyboardLayout[], UCHAR ucCursorBlinkInterval, UCHAR ucDelayToNextChar, BOOL boEnableCursorMove, BOOL boOneRadixPointOnly, UCHAR ucDigitAfterRadixPoint, BYTE bPasswordMask, STR caData[], UCHAR ucDataLen);
USHORT CTOS_EnterDownloadMode(void);
USHORT CTOS_PowerSource(UCHAR* bSrc);
USHORT CTOS_CTMSSetUploadFile(IN BYTE *baFileName, IN BYTE bMode);
USHORT CTOS_CTMSSaveLog(IN USHORT usApid, IN BYTE bMainStep, IN BYTE bSubStep,IN USHORT usRet,  IN BYTE *baMeg);
void   CTOS_CTMSUtility(IN BYTE bMode);
USHORT CTOS_PrinterBufferOutput(BYTE* pbPtr, USHORT usY8Len);
USHORT CTOS_CTMSGetInfo(IN BYTE Info_ID, IN void *stInfo);
USHORT CTOS_PowerMode(BYTE bMode);
USHORT CTOS_CTMSGetStatus(OUT USHORT *usStatus, OUT USHORT *usRetErr);
USHORT CTOS_CTMSGetConfig(IN BYTE Config_ID, OUT void *stConfig);
USHORT CTOS_MSRRead(BYTE* baTk1Buf, USHORT* pusTk1Len, BYTE* baTk2Buf, USHORT* pusTk2Len, BYTE* baTk3Buf, USHORT* pusTk3Len);
USHORT CTOS_CTMSGetConfig(IN BYTE Config_ID, OUT void *stConfig);
USHORT CTOS_CTMSSetConfig(IN BYTE Config_ID, IN void *stConfig);
USHORT CTOS_SCCommonReset(BYTE bID, BYTE bTA1, BOOL fColdReset, BOOL fEMV, BOOL fPTS, BOOL fIFSD, BYTE bVolt, BYTE* baATR, BYTE* bATRLen, BYTE* bCardType);

//=============================================================================================================================
//
// Printer Buffer Functions
//
//=============================================================================================================================
// for Printer Canvas
#define PB_CANVAS_X_SIZE			384
#define PB_CANVAS_Y_SIZE 			80
#define MAX_PB_CANVAS_NUM			2

#define d_ORIGINAL					0x01
#define d_DOUBLE					0x02
#define d_ONE_AND_HALF				0x13		//Enlarge 1.5

#define d_PRINTER_ALIGNLEFT											  0
#define d_PRINTER_ALIGNCENTER										  1
#define d_PRINTER_ALIGNRIGHT										  2

USHORT CTOS_USBOpen(void);
USHORT CTOS_USBClose(void);
USHORT CTOS_USBTxReady(void);
USHORT CTOS_USBTxData(BYTE* baSBuf, USHORT usSLen);
USHORT CTOS_USBRxReady(USHORT* pusRLen);
USHORT CTOS_USBRxData(BYTE* baRBuf, USHORT* pusRLen);

//=============================================================================================================================
//
// RS232 Functions
//
//=============================================================================================================================

#define d_COM1						0
#define d_COM2						1
#define d_COM3                      2
#define d_COM4                      3

USHORT CTOS_RS232Open(BYTE bPort, ULONG ulBaudRate, BYTE bParity, BYTE bDataBits, BYTE bStopBits);
USHORT CTOS_RS232FlushTxBuffer(BYTE bPort);
USHORT CTOS_RS232FlushRxBuffer(BYTE bPort);
USHORT CTOS_RS232Close(BYTE bPort);
USHORT CTOS_RS232RxReady(BYTE bPort, USHORT* pusLen);
USHORT CTOS_RS232TxReady(BYTE bPort);
USHORT CTOS_RS232TxData(BYTE bPort, BYTE* baBuf, USHORT usLen);
USHORT CTOS_RS232RxData(BYTE bPort, BYTE* baBuf, USHORT* pusLen);

//Device Mode
#define d_MODEL_VEGA9000	1
#define d_MODEL_VEGA7000	2
#define d_MODEL_VEGA5000	3
#define d_MODEL_VEGA5000S	4
#define d_MODEL_VEGA3000	5
#define d_MODEL_VEGA3000P	6
#define d_MODEL_QPAD		7
#define d_MODEL_MP200	8
#define d_MODEL_TR220		100
#define d_MODEL_PPC930		101
#define d_MODEL_CRT1000		102
#define d_MODEL_MPIN10		103
#define d_MODEL_TR300	104

//=============================================================================================================================
//
// GSM Functions
//
//=============================================================================================================================

#define d_GSM_GPRS_STATE_NOT_REG 		0
#define d_GSM_GPRS_STATE_REG     		1
#define d_GSM_GPRS_STATE_TRYING  		2
#define d_GSM_GPRS_STATE_DENY    		3
#define d_GSM_GPRS_STATE_UNKNOW  		4
#define d_GSM_GPRS_STATE_ROAM    		5

#define d_GSM_PWR_WORK					0
#define d_GSM_PWR_STANDBY				1
#define d_GSM_PWR_SLEEP					2

#define d_GPRS_SIM1						0
#define d_GPRS_SIM2						1

#define d_GSM_900_1800					0
#define d_GSM_900_1900					1
#define d_GSM_850_1800					2
#define d_GSM_850_1900					3

#define d_GSM_PIN_PUK_1          		0
#define d_GSM_PIN_PUK_2          		1

#define d_GSM_AUTH_SC            		0
#define d_GSM_AUTH_PS            		1
#define d_GSM_AUTH_FD            		2

//Phonebook
#define d_GSM_PB_FD              		0
#define d_GSM_PB_SM              		1
#define d_GSM_PB_ON              		2
#define d_GSM_PB_ME              		3
#define d_GSM_PB_LD              		4
#define d_GSM_PB_MC              		5
#define d_GSM_PB_RC              		6
#define d_GSM_PB_UNKNOW          		0xFF

//Addr Type
#define d_GSM_ADDR_NUMBER        		145
#define d_GSM_ADDR_CHAR          		209
#define d_GSM_ADDR_OTHER         		129

#define d_GSM_SMS_REC_UNREAD     		0
#define d_GSM_SMS_REC_READ       		1
#define d_GSM_SMS_STO_UNSEND     		2
#define d_GSM_SMS_STO_SEND       		3
#define d_GSM_SMS_ALL            		4

//Unicode Type
#define d_GSM_SMS_UTF8					0
#define d_GSM_SMS_UCS2					1

// Packet Service Network Type
#define	d_GSM_NETWORK_GPRS				0
#define	d_GSM_NETWORK_EGPRS				1
#define	d_GSM_NETWORK_WCDMA				2
#define	d_GSM_NETWORK_HSDPA				3
#define	d_GSM_NETWORK_UNKNOWN			4

// Manufacturer Information
#define	d_GSM_MANUFACTURER_TELIT		0x01	// Telit
#define	d_GSM_MANUFACTURER_SIMCOM		0x02	// SIMCOM

// Model Information
// Telit
#define	d_GSM_MODEL_GC864_QUAD			0x01	// 2G GSM/GPRS
#define	d_GSM_MODEL_GC864_QUAD_V2		0x02	// 2G GSM/GPRS
#define	d_GSM_MODEL_CC864_SR			0x03	// 3G CDMA
#define	d_GSM_MODEL_CC864_DUAL			0x04	// 3G CDMA
#define	d_GSM_MODEL_UC864_E				0x05	// 3G UMTS
#define	d_GSM_MODEL_UC864_E_DUAL		0x06	// 3G UMTS
#define	d_GSM_MODEL_UC864_G				0x07	// 3G UMTS

// SIMCOM
#define	d_GSM_MODEL_SIM900				0x21	// 2G GSM/GPRS

// Country Code
#define d_GSM_COUNTRY_TAIWAN			0
#define d_GSM_COUNTRY_SPAN				20

// GPIO control
#define d_GSM_IO_POWER		0
#define d_GSM_IO_RESET		1
#define d_GSM_IO_SIM		2
#define d_GSM_IO_VGSM		3
#define d_GSM_IO_DTR		4
#define d_GSM_IO_USBEN		5

// Module operate mode
#define d_GSM_MODULE_CMD_MODE         0x00
#define d_GSM_MODULE_DATA_MODE        0x01

#ifdef __cplusplus
}
#endif

#endif

