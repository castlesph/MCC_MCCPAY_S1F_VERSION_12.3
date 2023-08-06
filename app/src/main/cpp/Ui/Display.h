/* 
 * File:   display.h
 * Author: wade
 *
 * Created on 2012年8月11日, 下午 6:08
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DISPLAY_POSITION_LEFT 0
#define DISPLAY_POSITION_CENTER 1
#define DISPLAY_POSITION_RIGHT 2
#define DISPLAY_LINE_SIZE 16

void vdDispErrMsg(IN BYTE *szMsg);
void   vdDispTransTitle(BYTE byTransType);
void vdDispTitleString(BYTE *szTitle);
int    vdDispTransactionInfo(void);
void   setLCDPrint(int line,int position, char *pbBuf);
void   showAmount(IN  BYTE bY, BYTE bStrLen, BYTE *baBuf);
void   szGetTransTitle(BYTE byTransType, BYTE *szTitle);
int   vdDisplayTxnFinishUI(void);
USHORT showBatchRecord(TRANS_DATA_TABLE *strTransData);
void vduiLightOn(void);
void vduiKeyboardBackLight(BOOL fKeyBoardLight);
void vduiPowerOff(void);
void vduiClearBelow(int line);
void vduiDisplayStringCenter(unsigned char  y,unsigned char *sBuf);
void vduiWarningSound(void);
short vduiAskConfirmContinue(void);
BYTE struiGetchWithTimeOut(void); 
void vduiDisplayInvalidTLE(void);
short inuiAskSettlement(void);
void vduiDisplaySignalStrengthBatteryCapacity(void);
USHORT clearLine(int line);

void vdDisplayErrorMsg(int inColumn, int inRow,  char *msg);
void vdDisplayErrorMsg2(int inColumn, int inRow,  char *msg, char *msg2, int msgType);

void vdSetErrorMessage(char *szMessage);
int inGetErrorMessage(char *szMessage);
void vdDisplayLeftRight(USHORT usY, char *ptrLeft, char *ptrRight);

void vdClearNonTitleLines(void);
void setLCDPrint27(int line,int position, char *pbBuf);
void vdDisplayMultiLineMsgAlign(char *msg, char *msg2, char *msg3, int inPosition);
void inDisplayLeftRight(int inLine, unsigned char *strLeft, unsigned char *strRight, int inMode);
void vdClearBelowLine(int inLine);
int inReportSelecion(void);
void vdDisplayMessageBox(int inColumn, int inRow,  char *msg, char *msg2, char *msg3, int msgType);

void vdDisplayMessage(char *szLine1Msg, char *szLine2Msg, char *szLine3Msg);
void vdDisplayErrorMsgResp2 (char *msg, char *msg2, char *msg3);
void DisplayStatusLine(char *szDisplay);
void vdDisplayScreenMessage(char *szDisplay);
int inDisplayMessageBoxWithButton(int inColumn, int inRow,  char *msg, char *msg2, char *msg3, int msgType);
int inGetErrorMessages(char *szMessage, char *szMessage1);


#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_H */

