#ifndef CTOSKIA_H
#define	CTOSKIA_H

#ifdef	__cplusplus
extern "C" {
#endif

#define d_RKI_ERR_PATH_ILLEGAL				0x2B0D
#define d_RKI_INVALID_PARA                  0x2B0E		
#define d_RKI_ERR_FTOK_FAIL					0x2B0F
#define d_RKI_ERR_SHMGET_FAIL				0x2B10
#define d_RKI_ERR_IPC_FAIL					0x2B11
#define d_RKI_ERR_UNSUPPORTED				0x2B12
#define d_RKI_ERR_KIA_NOT_EXIST				0x2B13
#define d_RKI_ERR_SHMAT_FAIL				0x2B14    
    
USHORT CTOS_KeyInjectionPerform(IN void *pRFU_IN, OUT void *pRFU_OUT);


#ifdef	__cplusplus
}
#endif

#endif	/* CTOSKIA_H */

