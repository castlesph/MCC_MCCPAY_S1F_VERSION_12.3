/*============================================================================
 *
 *      typedef.h
 *      
 *==========================================================================*/
/*============================================================================
 * 
 * Author(s):    Peril Chen
 * Date:          
 * Description:	
 * 
 *==========================================================================*/
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#define IN
#define OUT
#define INOUT

#define CHAR	int8_t//char
#define UCHAR	uint8_t//unsigned char
#define BYTE	uint8_t//unsigned char

#ifndef STR
	#define STR		uint8_t//unsigned char	
#endif


#define SHORT	int16_t//short				// 2byte
#define USHORT	uint16_t//unsigned short		// 2byte
#define WORD	uint16_t//unsigned short		// 2byte
#define INT		int32_t//int					// 4byte	
#define UINT	uint32_t//unsigned int		// 4byte
#define DWORD	uint32_t//unsigned int		// 4byte
#define BOOL	uint8_t//unsigned char		// 1byte
#define LONG	int32_t//long				// 4byte
//#define ULONG	uint32_t//unsigned long		// 4byte
#define ULONG	unsigned long		// 4byte

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

#ifndef NULL
#define NULL	0
#endif

#define IN
#define OUT

#define K_STATUS		BYTE

#endif

