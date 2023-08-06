/* 
 * File:   display.h
 * Author: 
 *
 * Created on 2014
 */

#ifndef ___V5SLIBPBM_H___
#define	___V5SLIBPBM_H___

#ifdef	__cplusplus
extern "C" {
#endif

void vdConvert2BMP(BYTE *baBuf, ULONG width, ULONG height, BYTE *baName);
void vdCTOSS_TextBufferToBMP(ULONG width, ULONG height,USHORT usXPos, USHORT usYPos,BYTE *baStr,USHORT FontSize,char *szBMPFileName);
void vdCTOSS_TextBufferToBMPEx(USHORT usXPos, USHORT usYPos,BYTE *baStr,USHORT FontSize,char *szBMPFileName);





#ifdef	__cplusplus
}
#endif

#endif	/* ___V5SLIBPBM_H___ */

