#ifndef CASTLES_KMS2_CLIENT_H
#define CASTLES_KMS2_CLIENT_H

#include <stdbool.h>

int CTOS_KMS2Init (void);
int CTOS_KMS2UserDataWrite(bool isCommon, uint32_t offset, uint8_t* baInBuf, uint32_t bufLen);
int CTOS_KMS2UserDataRead(bool isCommon, uint32_t offset, uint8_t* baOutBuf, uint32_t bufLen);

#endif
