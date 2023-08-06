#ifndef __CTOS_QRCODE_H__
#define __CTOS_QRCODE_H__

#ifdef __cplusplus
extern "C"
{
#endif

// Info Header Version
#define QR_INFO_VERSION                     0                   

// Level Value
#define QR_LEVEL_L                          1
#define QR_LEVEL_M                          2
#define QR_LEVEL_Q                          3
#define QR_LEVEL_H                          4

// Version Value
#define QR_VERSION21X21                     1
#define QR_VERSION25X25                     2
#define QR_VERSION29X29                     3
#define QR_VERSION33X33                     4
#define QR_VERSION37X37                     5
#define QR_VERSION41X41                     6 
#define QR_VERSION45X45                     7
#define QR_VERSION49X49                     8
#define QR_VERSION53X53                     9
#define QR_VERSION57X57                     10
#define QR_VERSION61X61                     11
#define QR_VERSION65X65                     12
#define QR_VERSION69X69                     13
#define QR_VERSION73X73                     14
#define QR_VERSION77X77                     15
#define QR_VERSION81X81                     16
#define QR_VERSION85X85                     17
#define QR_VERSION89X89                     18
#define QR_VERSION93X93                     19
#define QR_VERSION97X97                     20
#define QR_VERSION101X101                   21
#define QR_VERSION105X105                   22
#define QR_VERSION109X109                   23
#define QR_VERSION113X113                   24
#define QR_VERSION117X117                   25
#define QR_VERSION121X121                   26
#define QR_VERSION125X125                   27
#define QR_VERSION129X129                   28
#define QR_VERSION133X133                   29
#define QR_VERSION137X137                   30
#define QR_VERSION141X141                   31
#define QR_VERSION145X145                   32
#define QR_VERSION149X149                   33
#define QR_VERSION153X153                   34
#define QR_VERSION157X157                   35
#define QR_VERSION161X161                   36
#define QR_VERSION165X165                   37
#define QR_VERSION169X169                   38
#define QR_VERSION173X173                   39
#define QR_VERSION177X177                   40


// Error Code
#define d_QRCODE_INVALID_PARA              0x6601
#define d_QRCODE_ENCODE_FAILED             0x6602 
#define d_QRCODE_INSUFFICINET_MEMORY       0x6603               
#define d_QRCODE_PRINTER_FAILED            0x6604  

typedef struct
{
    DWORD InfoVersion;
    DWORD Version;
    DWORD Level;
    DWORD Size;
}CTOS_QRCODE_INFO;

// Get Version
USHORT CTOS_LibQRCODEGetVersion(BYTE* version_str);

// Print QRCode by Printer
USHORT CTOS_QRCodePrinter(CTOS_QRCODE_INFO *pInfo, const char *pMessageText, USHORT usXstart);

// Show QRCode on LCD
USHORT CTOS_QRCodeDisplay(CTOS_QRCODE_INFO *pInfo, const char *pMessageText, USHORT X, USHORT Y);

// Put QRcode into Buffer
USHORT CTOS_QRCodePrinterBuffer(CTOS_QRCODE_INFO *pInfo, const char *pMessageText, USHORT usXPos, USHORT usYPos);

#ifdef __cplusplus
}
#endif

#endif

