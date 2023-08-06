#ifndef CASTLES_CRYPTO_CLIENT_H
#define CASTLES_CRYPTO_CLIENT_H


typedef struct
{
    unsigned long Message_Digest[5]; /* Message Digest (output)          */

    unsigned long Length_Low;        /* Message length in bits           */
    unsigned long Length_High;       /* Message length in bits           */

    unsigned char Message_Block[64]; /* 512-bit message blocks      */
    int Message_Block_Index;    /* Index into message block array   */

    int Computed;               /* Is the digest computed?          */
    int Corrupted;              /* Is the message digest corruped?  */

} SHA_CTX;

int CTOS_DES (uint8_t bEncDec, uint8_t* baKey, uint8_t bKeyLen, uint8_t* baData, uint16_t usDataLen, uint8_t* baResult);
int CTOS_DES_CBC (uint8_t bEncDec, uint8_t* baKey, uint8_t bKeyLen, uint8_t* baICV, uint8_t* baData, uint16_t usDataLen, uint8_t* baResult);
int CTOS_MAC (uint8_t* baKey, uint8_t bKeyLen, uint8_t* baICV, uint8_t* baData, uint16_t usDataLen, uint8_t* baMAC);
void CTOS_SHA1Init(SHA_CTX *pstInfo);
void CTOS_SHA1Update(SHA_CTX *pstInfo, uint8_t* baBuffer, uint16_t usCount);
void CTOS_SHA1Final(uint8_t* baOutput, SHA_CTX *pstInfo);


#endif

