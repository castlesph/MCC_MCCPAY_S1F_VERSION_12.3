
#ifndef _POSXMLFUNCTION_H
#define	_POSXMLFUNCTION_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_HEADERS             10
#define MAX_HEADER_VAL_SIZE     512

#define MAX_BODY_VARS           60
#define MAX_BODY_VAL_SIZE       512

#define MAX_KV_KEY_SIZE         512
#define MAX_KV_VALUE_SIZE       XML_DATA_SIZE
#define MAX_KV_PAIRS            50

#define MAX_REQUEST_LINE_SIZE   160

#define MAX_LINE                150
#define MAX_HEADER_SIZE         255
#define MAX_FILE_SIZE			1500
#define MAX_BODY_SIZE			800

#define CRLF_STR                "\r\n"
#define CRLFCRLF_STR            "\r\n\r\n"
#define AMPERSAND_STR           "&"
#define EQUAL_STR               "="
#define COLON_STR               ":"
#define SEMICOLON_STR           ";"

// Tags
#define TAG_SERVICE				"service"
#define TAG_ATTACH				"attach"
#define TAG_BODY				"body"
#define TAG_DEVICE_INFO			"device_info"
#define TAG_MCH_ID				"mch_id"
#define TAG_NONCE_STR			"nonce_str"
#define TAG_OUT_TRADE_NO		"out_trade_no"
#define TAG_SPBILL_CREATE_IP    "spbill_create_ip"
#define TAG_TOTAL_FEE			"total_fee"
#define TAG_SIGN				"sign"
#define TAG_STATUS				"status"
#define TAG_MESSAGE				"message"
#define TAG_APPID				"appid"
#define TAG_RESULT_CODE			"result_code"
#define TAG_OPENID				"openid"
#define TAG_IS_SUBSCRIBE        "is_subscribe"
#define TAG_TRADE_TYPE			"trade_type"
#define TAG_BANK_TYPE			"bank_type"
#define TAG_COUPON_FEE			"coupon_fee"
#define TAG_FEE_TYPE			"fee_type"
#define TAG_TRANSACTION_ID		"transaction_id"
#define TAG_TIME_EXPIRE			"time_expire"
#define TAG_TIME_END			"time_end"
#define TAG_TIME_START			"time_start"
#define TAG_ERR_CODE			"err_code"
#define TAG_ERR_CODE_DESC		"err_code_des"
#define TAG_ERR_MSG				"err_msg"
#define TAG_MCH_CREATE_IP		"mch_create_ip"
#define TAG_NOTIFY_URL			"notify_url"
#define TAG_CHARSET				"charset"
#define TAG_CODE_IMG_URL		"code_img_url"
#define TAG_CODE_URL			"code_url"
#define TAG_SIGN_TYPE			"sign_type"
#define TAG_UUID				"uuid"
#define TAG_VERSION				"version"
#define TAG_CASH_FEE			"cash_fee"
#define TAG_CASH_FEE_TYPE		"cash_fee_type"
#define TAG_LOCAL_FEE_TYPE		"local_fee_type"
#define TAG_LOCAL_TOTAL_FEE		"local_total_fee"
#define TAG_KEY					"key"
#define TAG_OP_USER_ID			"op_user_id"
#define TAG_GOODS_TAG			"goods_tag"
#define TAG_PRODUCT_ID			"product_id"
#define TAG_LIMIT_CREDIT_PAY	"limit_credit_pay"
#define TAG_ORDER_FEE			"order_fee"
#define TAG_OUT_TRANSACTION_ID	"out_transaction_id"
#define TAG_PLATFORM_RATE		"platform_rate"
#define TAG_RATE				"rate"
#define TAG_TRADE_STATE			"trade_state"
#define TAG_TRADE_STATE_DESC	"trade_state_desc"
#define TAG_OUT_REFUND_NO		"out_refund_no"
#define TAG_REFUND_FEE			"refund_fee"
#define TAG_OP_USER_ID			"op_user_id"
#define TAG_REFUND_ID			"refund_id"
#define TAG_AUTH_CODE			"auth_code"
#define TAG_WTIHHOLDING_TAX		"withholding_tax"
#define TAG_REFUND_CHANNEL		"refund_channel"
#define TAG_REFUND_STATUS		"refund_status_0"


#define TAG_NULL				"null"

// Constant
#define szPostLine	        	"https://pay.wepayez.com/pay/gateway"
#define szPostLine2	        	" HTTP/1.1"
#define szContentLine	  		"application/xml"
#define szHost	  				"pay.wepayez.com"
#define szCacheControl  		"no-cache"
#define szConnectionMsg 		"keep-alive"
#define szStartCDataSection 	"<![CDATA["
#define szEndCDataSection 		"]]>"
#define szBadRequestMsg         "Bad Request"
#define szNotFoundMsg           "Not Found"
#define szOKMsg 	            "200 OK"
#define szForbiddenMsg     	    "HTTP/1.1 403"
#define szContinueMsg     	    "HTTP/1.1 100 Continue\r\n"
#define szSuccessMsg            "HTTP/1.1 200 OK\r\n"
#define szRedirectMsg           "HTTP/1.1 303 See Other\r\n"          /* Was "302 Found" */
#define szNotFoundMsg           "HTTP/1.1 404 File not found\r\n\n"   /* Extra "\n" at end for blank line */
#define szNotImplementedMsg     "HTTP/1.1 501 Not Implemented\r\n\n"  /* Extra "\n" at end for blank line */
#define szTagXMLStart         	"<xml>"
#define szTagXMLEnd         	"</xml>"
#define szTagStatusEnd			"</status>"

#define TRADE_STATE_SUCCESS     "SUCCESS"
#define TRADE_STATE_NOTPAY      "NOTPAY"


// XML Parse Type
#define XML_PARSE_NUMERIC			1
#define XML_PARSE_STRING			2

// Structure Definitions
typedef struct tagKVPair
{
    char szKey[MAX_KV_KEY_SIZE+1];
    char szValue[MAX_KV_VALUE_SIZE+1];
} KEY_VALUE_PAIR;

/* Struct for holding a key-value pair collection */
typedef struct tagKVPairCollection
{
    int             iCount;                 /* Number of valid key-value pairs in kvArray[] */
    KEY_VALUE_PAIR  kvArray[MAX_KV_PAIRS];  /* Range 0..MAX_KV_PAIRS-1 */
} KEY_VALUE_PAIR_COL;

typedef struct tagHTTPRequest
{
	char szHeader[MAX_HEADER_SIZE+1];
	KEY_VALUE_PAIR_COL	srKVPairCol;
}HTTP_REQUEST;

HTTP_REQUEST srResponse;
HTTP_REQUEST srRequest;	

int inXMLCreateHTTPpostHeader(char *pszHeader, int inContentLength);
int inXMLCreateRequestData(void);
int inInitKVPairCol(KEY_VALUE_PAIR_COL *pKeyValPair);
int inAddKeyValuePairToCol(KEY_VALUE_PAIR_COL *pKeyValPair, char *pszKey, char *pszValue);
int URLencode(char *szBuffer);
char* strlwr(char* s);
char* strupr(char* s);
int iXMLAnalyzeResponse(char* szXMLData);
void vXMLParseAndStoreToCollection(HTTP_REQUEST *srResponse, int iParseType, char* szResponseData, char* szXMLTag);
void iGetSignatureKey(int iMessageType);
char *getXMLVal(char* szXMLData, char* szTag);
char *getXMLCdataVal(char* szXMLCData, char* szTag);
int iWechatDisplayQRCode(BOOL fConfirm);
BOOL fXMLBadRequestMessage(char* szXMLData);
BOOL fXMLNotOKMessage(char* szXMLData);
BOOL fBadRequest(char* szXMLData);
BOOL fOKMsg(char* szXMLData);
BOOL fXMLForbiddenMessage(char* szXMLData);
BOOL fForbiddenRequest(char* szXMLData);
BOOL fCDataSection(char* szTag);
void iXMLGetDateTime(char* szCurrentDate, char* szCurrentTime);
int iGetKeyValuePairToCol(char* szTag, char* szValue);
void iXMLDisplayResponse(char* szMessage, int iLen);
BOOL fXMLCheckTagExist(char* szXMLData, char* szTag);
int iXMLCheckResponseStatus(char* szXMLReceiveData);
int iXMLCheckResponseResultCode(char* szXMLReceiveData);
int iXMLCheckTradeState(char* szXMLReceiveData);
int iXMLCheckOKStatus(char* szXMLReceiveData);
void vXMLGetResponseDataByTag(int iParseType, char* szXMLReceiveData, char* szXMLTag, char* szXMLValue);
int vXMLGetNonce_str(char* szNonce_str);
void vXMLGenerateNonce_Str(char* szNonce_str);
void vXMLGetOut_trade_no(char* szOut_trade_no);
int iXMLQRDisplayComplete(void);
int iXMLConfirmQRCode(void);
BYTE iXMLGetchWithTimeOut(short Sec);
void vXMLGetService(char* szService);
int iXMLSetValueByTag(char* szTag, char* szValue);
char* iWord_Wrap (char* szOutBuffer, char* szInString, int line_width);
int GetDelimitedString(char *buf, int fldnum, unsigned char sep, char *dest);
int iXMLTransactionIDEntry(void);
int inCTOS_XMLSelectHost(void);
int inXML_CommsInit(void);
int inXMSendAndReceiveRequestMessage(void);
int inXMSendAndReceiveRetrieveMessage(BOOL fRetrieveOnce);
int inXML_CommsReInit(void);
void iSetDummyFieldForERM(void);
int inXMSendAndReceiveVoidMessage(void);
void szGetDigitalWalletTransTitle(BYTE byTransType, BYTE *szTitle);
BOOL fValidXMLEnd(char* szXMLData);
BOOL fValidTagStatusEnd(char* szXMLData);
int inXMSendAndReceiveCloseQRCodeMessage(void);

int iXMLCaptureWechatQRCode(void);
int iGetXMLEndIndex(char* szXMLData);

void iGeTotalFormattedAmount(char* szFormattedAmt);
void iGetConvertedAmount(char* szFormattedAmt);

int iRetrieveReadData(void);
int iRetrieveSaveData(void);

int iXMLRetrieveListView(void);
void vdXMLFormatDateEndTime(char* szFormattedDateTime);

char* SHA256String(char* szString);
int base64_encode(char *binStr, unsigned len, char *psz_Encoded);

int iGetQRPresentedType(void);
int iRetrieveUpdateData(void);

int iXMLCheckResponseOutRefundNo(char* szXMLReceiveData);

int inCTOS_SelectQRPresented(void);

int iXMLCheckResponseRefundStatus(char* szXMLReceiveData);

int vdXMLProcessReversal(void);

void vdXMLSelectedRetrieve(void);

int iRetrieveUpdateDataByOutTradeNo(void); // Update Retrieve Data by OutTradeNo -- sidumili

int iXMLCheckResponseOutRefundID(char* szXMLReceiveData); // Get Refund ID from response during void

#ifdef	__cplusplus
}
#endif

#endif

