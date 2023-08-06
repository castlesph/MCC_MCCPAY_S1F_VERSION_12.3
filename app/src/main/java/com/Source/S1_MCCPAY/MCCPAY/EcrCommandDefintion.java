package com.Source.S1_MCC.MCC;

public class EcrCommandDefintion {
    public static final String ECR_REQ_CMD_NAME                 = "ECR_COMMAND_TYPE";

    public static final String ECR_REQ_CMD_SALE                     = "SALE";
    public static final String ECR_REQ_CMD_VOID                     = "VOID";
    public static final String ECR_REQ_CMD_REFUND                  = "REFUND";
    public static final String ECR_REQ_CMD_SETTLE_ALL              = "SETTLE_ALL";
    public static final String ECR_REQ_CMD_SETTLE                  = "SETTLE";

    public static final String ECR_REQ_PAYMENT_TYPE                 = "ECR_PAYMENT_TYPE"; //get the value from ECR request(TYPE): eg: VMJ -- > Visa/Master,

	public static final String ECR_REQ_CMD_ECHO_TEST                = "ECHOTEST";
	public static final String ECR_REQ_CMD_RETRIEVAL                = "RETRIEVAL";
	public static final String ECR_REQ_CMD_GET_TOTAL                = "TOTAL";
	public static final String ECR_REQ_CMD_GET_LAST_TOTAL           = "GET_LAST_TOTAL";
	public static final String ECR_REQ_CMD_BARCODE                  = "BARCODE";
	public static final String ECR_REQ_CMD_PRINTING                 = "PRINTING";
	public static final String ECR_REQ_CMD_READCARD                 = "READCARD";

    public static final String ECR_REQ_CMD_PREAUTH                  = "PREAUTH";
    public static final String ECR_REQ_CMD_PREAUTH_COMP             = "PREAUTHCOMP";
	public static final String ECR_ECR_REQ_CMD_TIPS                 = "TIPS";
	
    public static final String ECR_REQ_CMD_VOID_PREAUTH             = "VOID_PREAUTH";

	public static final String ECR_REQ_CMD_REPRINT_SETTLE           = "REPRINTSETTLE";
    public static final String ECR_REQ_CMD_REPRINTLAST              = "REPRINTLAST";
    public static final String ECR_REQ_CMD_REPRINTANY               = "REPRINTANY";
}
