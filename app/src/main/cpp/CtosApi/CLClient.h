#ifndef CASTLES_CL_CLIENT_H
#define CASTLES_CL_CLIENT_H

typedef unsigned char uint8_t;
typedef int int32_t;
typedef unsigned int uint32_t;

int CTOS_CLAPDU (uint8_t* baSBuf, int32_t usSLen, uint8_t* baRBuf, int32_t* usRLen);
int CTOS_CLInitComm (uint32_t uiBaudRate);
int CTOS_CLDESELECT (void);
int CTOS_CLTypeAActiveFromIdle (uint8_t bBaudRate, uint8_t* baATQA, uint8_t* bSAK, uint8_t* baCSN, uint8_t* bCSNLen);
int CTOS_CLRATS(uint8_t bAutoBR, uint8_t* baATS, int32_t* bATSLen);
int CTOS_CLTypeBActive(uint8_t* baPUPI);
int CTOS_CLPowerOff (void);
int CTOS_CLPowerOn (void);
#endif

