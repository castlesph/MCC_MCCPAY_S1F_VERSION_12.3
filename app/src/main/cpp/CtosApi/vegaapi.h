#ifndef __VEGA_API_H__
#define __VEGA_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <ctosapi.h>
#include <epad.h>

BYTE MenuDisplay(BYTE *sHeaderString, BYTE iHeaderStrLen, BYTE bHeaderAttr, BYTE iCol, BYTE center_x, BYTE *sItemsString, BYTE isReCount);
void vdCTOSS_FormatAmount(char *szFmt,char* szInAmt,char* szOutAmt);
int inMultiAP_SetSubAPStatus(void);
int inMultiAP_CheckSubAPStatus(void);
int inMultiAP_CheckMainAPStatus(void);
int inCTOSS_FormERMReceipt(char *szFileName);
BYTE InputStringAlphaEx2(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS);
int inCTOSS_ResetDUKPTKSN(IN BYTE *i_szKSN);
unsigned int CTOSS_strlen (char *s);
BYTE bGMenuDisplay(BYTE *sHeaderString, BYTE iHeaderStrLen, BYTE bHeaderAttr, BYTE iCol, BYTE center_x, BYTE *sItemsString, BYTE isReCount);
int inMultiAP_ForkTaskEx(void);
int inMultiAP_ForkSharlsAp(void);
int inMultiAP_ForkTask(void);
int inSetLanguageDatabase(char* szLanguangeDatabaseName);
int inCTOSS_SetERMFontType(char *szFontName,int inFontStyle);
void vdCTOSS_CombineMenuBMP(char *szBKBMPname);
void vdCTOSS_DelBackupS3DB(void);
int ePad_ClearSignatureCaptureLib(void);
int ePad_SignatureCaptureLibEex(DISPLAY_REC *szDisplayRec);
void vdCTOSS_TextBufferToBMP(ULONG width, ULONG height,USHORT usXPos, USHORT usYPos,BYTE *baStr,USHORT FontSize,char *szBMPFileName);
USHORT CTOS_LanguagePrinterGetFontInfo(USHORT* usASCIIFontID,USHORT* usFontSize,USHORT* usFontStyle);
void vdCTOS_InputTMSReSet(void);
void vdCTOS_InputTMSSetting(void);
int inCTOSS_EditTalbe(char *szTableName,char *szDatabaseName);
BYTE InputStringAlphaEx(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS);
int inCTOS_KillALLSubAP(void);
int inCTOSS_HKLMInjectDUKPTKey(int inKeySet, int inKeyIndex);
void vdBackUpWhiteBMP(char *szBMPname,char *szOrgBMP);
BYTE InputStringAlpha(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS);
void vdCTOSS_RestoreMemory(int limitRAM);
USHORT usCTOSS_SystemMemoryStatus(ULONG* ulUsedDiskSize ,ULONG* ulTotalDiskSize ,ULONG* ulUsedRamSize ,ULONG* ulTotalRamSize);
void vdCTOS_LCDGShowUpDown(BYTE isUP, BYTE isDOWN);
BYTE InputString(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS);
USHORT usCTOSS_ConfirmMenu(void);
USHORT usCTOSS_VirtualNumberGet(USHORT usDisplayX, USHORT usDisplayY, BYTE MaxNumLen, BYTE *pNum);
BYTE InputAmountEx(USHORT usX, USHORT usY, BYTE *szCurSymbol, BYTE exponent, BYTE first_key, BYTE *baAmount, ULONG *ulAmount, USHORT usTimeOutMS, BYTE bIgnoreEnter);
USHORT usCTOSS_VirtualAmountGet(USHORT usDisplayX, USHORT usDisplayY, BYTE *szAmtPrompt, BYTE *szCurSymbol, BYTE exponent, BYTE first_key, BYTE *baAmount, ULONG *ulAmount, USHORT usTimeOutMS, BYTE bIgnoreEnter, BYTE *szAmountFmt);
void vdCTOSS_SaveBMPForERM(char *szFileName);
int ePad_SignatureCaptureLibEx(void);
int inCCTOS_PrinterBufferOutputAligned(unsigned char* pchTemp,CTOS_FONT_ATTRIB* ATTRIB,BYTE bMode, BYTE IsNextline, int ERMCapture);
void vdSetsysPowerLowSpeed(void);
void vdSetsysPowerHighSpeed(void);
BYTE InputStringEx(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS);
int inMTAClient (void);
void vdCTOSS_getNameByPid(pid_t pid, char *szAPname);

int find_pid_of(const char *process_name);

int inMultiAP_SendChild(BYTE *inbuf, USHORT inlen);
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

// MTA
#define d_IPC_MSG_TYPE			0xFFFF

#define d_MAX_APP				50
#define	d_AP_FLAG_DEF_SEL_EX	0xC0

#define d_MAX_IPC_BUFFER		8192*2

struct theMsgBuf {
    long msgType;
    char msgText[d_MAX_IPC_BUFFER];
} ApmsgBuffer;

#ifdef __cplusplus
}
#endif

#endif

