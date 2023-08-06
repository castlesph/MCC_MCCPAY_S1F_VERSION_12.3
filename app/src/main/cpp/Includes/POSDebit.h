
#ifndef ___POS_DEBIT___
#define ___POS_DEBIT___

int inCTOS_DebitSaleFlowProcess(void);
int inCTOS_DebitSale(void);
int inCTOS_DebitBalanceInquiryFlowProcess(void);
int inCTOS_DebitBalInq(void);
int inCTOS_DebitLogonFlowProcess(void);
int inCTOS_DebitLogon(void);

int inCTOS_DebitSelection(void);
int inCTOS_SelectAccountType(void);
int inCTOS_CheckSelectDebit(void);
int InDisplayBalance(void);

//Diag Start
int inCTOS_DebitSale_Diag(void);
int inCTOS_DebitSaleFlowProcess_Diag(void);
int inCTOS_GetTxnBaseAmount_Diag(void);
//Diag End

#endif //end ___POS_DEBIT___

