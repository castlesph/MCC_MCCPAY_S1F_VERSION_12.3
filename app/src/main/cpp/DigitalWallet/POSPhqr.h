
#ifndef _POSPHQR_H
#define	_POSPHQR_H

#ifdef	__cplusplus
extern "C" {
#endif

#if 0
int inCTOS_WechatQRRequest(void);
int inCTOS_WechatQRRequestFlowProcess(void);
int inCTOS_WechatQRRetrieve(void);
int inCTOS_WechatQRRetrieveFlowProcess(void);
int inCTOS_WechatQRDisplayTest(void);
int iXMLSelectSearchType(void);
int inCTOS_Wechat_Alipay_QRVoid(void);
int inCTOS_Wechat_ALipay_QRVoidFlowProcess(void);
#endif
int inPHQR_Initialize_Logon(int inTransType);
//int inPHQRSendAndReceive(void);
void vdPHQRCreatePOSTData(char *szBuffer, int inType);
void vdGet_ISO8601_Clock(char *szClock);
//int inPHQRSend_request(struct MemoryStruct *contents,struct MemoryStruct *response, int TransType); 
size_t read_phqr_callback(void *dest, size_t size, size_t nmemb, void *userp);
size_t write_phqr_callback(void *contents, size_t size, size_t nmemb, void *userp);




#define PHQR_INITIALIZE 1
#define PHQR_LOGON      2
#define PHQR_GENERATE_QR      3
#define PHQR_CHECK_STATUS     4



typedef struct MemoryStruct {
	char *memory;
	char *function;
	size_t size;
}MemoryStruct;

bool verifySignatureEx(char *plainText, char* signatureBase64);
//RSA* createRSAWithFilename(char * filename,int public);
void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length);
void vdFormatSignature(char *plainText, unsigned char *szConvertText);
int inPHQR_GenerateQR(void);
int inPHQR_Initialize_SendAndReceive(void);
int inExtractField(unsigned char *uszRecData, char *szField, char *szSearchString);
int inDisplaResultcode(int inCode);
int inPHQR_GenerateQR_SendAndReceive(void);

int inPHQR_Check_Status_SendAndReceive(void);


int inCTOS_PHQRRetrieveFlowProcess(void);
int iPHQRRetrieveReadData(long dSerarchRefnum);
int inPrintPHQRCode(void);


int inCurl_CommsInit(void);


#ifdef	__cplusplus
}
#endif

#endif

