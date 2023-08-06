/* 
 * File:   Print.h
 * Author: ASUS
 *
 * Created on 2012年8月25日, 上午 1:35
 */

#ifndef PRINT_H
#define	PRINT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "..\Includes\Trans.h"

#define d_LINE_DOT 12
#define d_LINE_SIZE 46

#define d_FIRST_PAGE 1
#define d_SECOND_PAGE 2
#define d_THIRD_PAGE 3

#define TTF_FONT_REGULAR			0
#define TTF_FONT_ITALIC			1
#define TTF_FONT_BOLD				2
#define TTF_FONT_BOLD_ITALIC		3

#define ONE_LINE_DOT	"\n"


#define NORMAL_SIZE	1
#define DOUBLE_SIZE	2

#define PRINT_SALE_REPORT 0
#define PRINT_REFUNT_REPORT 1
#define PRINT_SETTLE_REPORT 2

#define DIVIDING_LINE 1
#define DIVIDING_LINE_DOUBLE 2    
    
#define PRINT_HOST_TOTAL 0
#define PRINT_CARD_TOTAL 1    

#define SETTLE_PRINT 1
#define TOTAL_REPORT 0

#define PRINT_CARD_MASKING_1 0
#define PRINT_CARD_MASKING_2 1
#define PRINT_CARD_MASKING_3 2

#define V3_DESKTOP 1
#define V3_MOBILE 2

#define LENGTH_24 24
#define LENGTH_32 32
#define LENGTH_41 41

#define DB_SIGN_BMP "signtest.bmp"

#define BANK_COPY_RECEIPT 1
#define CUSTOMER_COPY_RECEIPT 2
#define MERCHANT_COPY_RECEIPT 3

short printCheckPaper(void);  
USHORT printDateTime(void);
USHORT ushCTOS_printReceipt(void);
USHORT ushCTOS_PrintAgreement(void);
USHORT batchAllRecord(BOOL isPrint);
void vdPrintEMVTags(void);
void vdPrintCenter(unsigned char *strIn);
USHORT printBatchInvoiceNO(void);
void setTotalCountandAmountCardWise(int i);
void PrintTotalCountandAmountCardWise (int inReportType, USHORT usSaleCount, 
    ULONG ulSaleTotalAmount,    
    USHORT usSaleAloneCount,
    ULONG  ulSaleAloneTotalAmount,
    USHORT usRefundCount,
    ULONG  ulRefundTotalAmount,    
    USHORT usVoidSaleCount,
    ULONG  ulVoidSaleTotalAmount,
    USHORT usOffSaleCount,
    ULONG  ulOffSaleTotalAmount);

void vdPrintTitleCenter(unsigned char *strIn);
void vdPrintTerminalConfig(void);
void printCardHolderName(void);

USHORT ushCTOS_printAll(int page);
USHORT ushCTOS_PrintHeader(int page);
USHORT ushCTOS_PrintBody(int page);
USHORT ushCTOS_PrintFooter(int page);
int inCTOS_rePrintReceipt(void);
int inCTOS_PrintSettleReport(BOOL fManualSettlement, BOOL fCaptureReceipt);

USHORT ushCTOS_printReceipt(void);
int inCTOS_REPRINT_ANY(void);
int inCTOS_REPRINT_LAST(void);
int inCTOS_PRINTF_SUMMARY(void);
int inCTOS_PRINTF_DETAIL(void);
int inCTOS_REPRINTF_LAST_SETTLEMENT(void);


int inCTOS_rePrintLastReceipt(void);
int vdCTOS_PrintSummaryReport(void);
void vdCTOS_PrintDetailReport(void);
USHORT ushCTOS_printErrorReceipt(void);
USHORT ushCTOS_printBatchRecordHeader(void);
USHORT ushCTOS_printBatchRecordFooter(void);
USHORT ushCTOS_GetFontInfor(void);
USHORT ushCTOS_ReprintLastSettleReport(void);
void vdCTOSS_PrinterBMPPic(unsigned int usX,unsigned int y, const char *path);


void vdCTOS_PrintAccumeByHostAndCard (int inReportType,
	USHORT usSaleCount, 
	double ulSaleTotalAmount,    
	USHORT usRefundCount,
	double  ulRefundTotalAmount,    
	USHORT usVoidSaleCount,
	double  ulVoidSaleTotalAmount,
	USHORT usOffSaleCount,
	double  ulOffSaleTotalAmount,
	//10101
	USHORT usReloadCount, 
	double  ulReloadTotalAmount	
	//10101
);



int inCTOS_SelectFont(int inFontMode,int inFontSize ,int inFontStyle,char * szFontName);


int ushCTOS_ePadSignature(void);
int ushCTOS_ePadPrintSignature(void);	
int ushCTOS_ClearePadSignature(void);
int inPrint(unsigned char *strPrint);


/*albert - start - Aug2014 - compute crc*/
void vdCTOS_PrintCRC(void);
void vdComputeCRC(unsigned long lnCRC, char *ptrCRC);
/*albert - end - Aug2014 - compute crc*/
void vdCTOS_HostInfo(void);

USHORT ushCTOS_PrintLoyaltyBody(int page);
USHORT ushCTOS_PrintLoyaltyFooter(int page);
int inPrintLeftRight(unsigned char *strLeft, unsigned char *strRight, int inWidth);
void vdPrintFormattedAmount(unsigned char *prtDisplay, unsigned char *prtAmount, int inWidth);

int vdCTOS_PrintReceiptCopies(BOOL BankCopy, BOOL CustCopy, BOOL MercCopy);
void vdCTOS_PrinterFline(USHORT usLines);

void vdPrintISOOption(unsigned char *pucTitle,unsigned char *uszData, int inLen);
int inPrintISOPacket(unsigned char *pucTitle,unsigned char *pucMessage, int inLen);

void SetReportTime(void);
void vdBackUpReportTime(void);
void vdRestoreReportTime(void);
int isCheckTerminalMP200(void);
void vdComputeGetAppCRC(char *szAppName, char *ptrCRC, BOOL fMP200);
void vdLineFeed(BOOL fFooterLogo);
void vdPutEnvCRC(void);
int inCTOS_DisplayPrintBMP(void);
void vdPrintCRC(void);

// For wechat
int iXMLPrintWechatHeader(void);
int iXMLPrintWechatBody(int page);
int iXMLPrintWechatFooter(int page);
int iXMLPrintWechatCopy(int page);
int iXMLPrintWechatQRCode(void);
USHORT ushCTOS_WechatprintBatchRecordFooter(BOOL fSettle);
int iXMLFormatDateTime(char* szTrxnDateTime, char* szOutTrxnDateTime);
USHORT ushCTOS_WechatprintBatchRecordHeader(void);
int iXMLPrintWechatDateTime(BOOL fSettle, BOOL fPrintLastSettle, int iPage);
int iXMLPrintWechatTIDMID(void);
int inPrintXMLPacket(unsigned char *pucTitle,unsigned char *pucMessage, int inLen, BOOL fRequest);

#define DETAIL_REPORT_TOTAL 2
#define DETAIL_REPORT_GRANDTOTAL 3
int inPrintLeftRight2(unsigned char *strLeft, unsigned char *strRight, int inWidth);
int vdCTOS_SettlementPrintDetailReport(BOOL fAddtoSettleReport, BOOL fLastSettleReport, BOOL fReprintLastSettleHeader);
void vdGetSignatureFilename(char *ptrNewFilename);

void vdPrintTIDMIDVersion(void);
void vdPrintTrxnType(BOOL fPrintCenter);
void vdCTOS_GetROOTFS(char* ucBuffer);
void vdGetTerminalVersion(char* szVersion);
void vdPrintBatchNoInvoiceNo(void);
void vdPrintDateTime(void);
USHORT printDateTime2(char *ptrDate, char *ptrTime, BOOL fTrans);
int inPrintWechatQRCode(void);
int inPrintTrxnTypeAmount(void);
int inPrintFooterLogo(void);
void vdLogISOOption(unsigned char *pucTitle,unsigned char *uszData, int inLen);
int inLogISOPacket(unsigned char *pucTitle,unsigned char *pucMessage, int inLen);
void vdDeleteBMP(void);
int inPrintConfirmation(char* szTitle, BOOL fIncludeLogoHeader);
void vdPrintHeaderLogo(void);
int iPrintPHQRBody(int page);
USHORT printMIDTID(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PRINT_H */

