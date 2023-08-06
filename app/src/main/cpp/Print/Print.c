#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <ctype.h>
#include <EMVAPLib.h>
#include <EMVLib.h>

#include "../Includes/wub_lib.h"
#include "../Includes/myEZLib.h"
#include "../Includes/msg.h"

#include "Print.h"
#include "../FileModule/myFileFunc.h"
#include "../UI/Display.h"
#include "../Includes/POSTypedef.h"
#include "..\Includes\CTOSInput.h"
#include "../accum/accum.h"
#include "../DataBase/DataBaseFunc.h"
#include "..\debug\debug.h"
#include "..\Includes\Showbmp.h"
#include "..\Includes\POSHost.h"
#include "..\Includes\POSTrans.h"
#include "..\Includes\POSSetting.h"
#include "..\Includes\MultiApLib.h"
#include "..\Aptrans\MultiAptrans.h"
#include "..\Aptrans\MultiShareEMV.h"
#include "..\Includes\epad.h"
#include "..\Includes\PosLoyalty.h"
#include "..\POWRFAIL\POSPOWRFAIL.h"

// For Wechat
#include "..\DigitalWallet\POSWechat.h" // For Wechat -- sidumili
#include "..\DigitalWallet\POSAlipay.h" // For Alipay -- sidumili
#include <ctos_qrcode.h>


const BYTE baPrinterBufferLogo_Single[]={ //Width=60, Height=49
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                                         
                                                                                       
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x60,0x30,0x18,0x18,0x8C,0xC4,0xC6,      
    0x66,0x66,0x22,0x22,0x22,0x22,0x22,0x62,0x66,0x46,0xC4,0x84,0x8C,0x18,0x10,0x30,      
    0x60,0xC0,0xC0,0xE0,0x38,0x0E,0x07,0x1E,0x38,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                                         
                                                                                       
    0x00,0x00,0x00,0x00,0xC0,0xF8,0x0E,0x03,0x81,0xF0,0x1C,0x0E,0x03,0xC1,0xF0,0xF8,      
    0x7C,0x3C,0x1C,0x1E,0x1E,0x1E,0xFE,0xFC,0xFC,0xFC,0xF8,0xF0,0xC1,0x83,0xCE,0x7C,      
    0x1C,0x07,0x81,0xE0,0x70,0x1C,0x0E,0x1C,0x70,0xC0,0x83,0x07,0x1C,0x70,0xE0,0x80,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                                         
                                                                                      
    0x00,0x00,0x00,0x00,0x0F,0x7F,0xC0,0x00,0x03,0x3F,0xF0,0x80,0x00,0x0F,0x3F,0x7F,  
    0xFF,0xFC,0xFC,0xFC,0xFC,0xFE,0xF3,0xFB,0xFF,0xFF,0x7F,0x3F,0x0F,0x03,0x81,0xE0,  
    0x38,0x0E,0x03,0xC0,0xF0,0x1C,0x1C,0x38,0xE0,0x81,0x03,0x0E,0x38,0x70,0xC0,0x03,  
    0x0E,0x1C,0x70,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,  
    0xF0,0x78,0x18,0x18,0x0C,0x0C,0x0C,0x0C,0x08,0x18,0x38,0x30,0x00,0x00,0x00,0x00,  
    0x80,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x80,0xC0,  
    0xC0,0xC0,0xC0,0x80,0x00,0x00,0xC0,0xFC,0xFC,0xFC,0xC0,0xC0,0x00,0xFC,0xFC,0xFC,  
    0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,  
    0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,  
    0x0C,0xFC,0xFC,0xFC,0x0C,0x0C,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,  
    0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,  
    0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,  
    0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,  
    0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x00,0x00,0x00,  
    0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,  
    0xC0,0xC0,0xC0,0x80,0x80,0xC0,0xC0,0xC0,0x00,0x00,0x40,0xC0,0xC0,0x80,0x00,0x00,  
    0x00,0x00,0x80,0xC0,0xC0,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,  
    0xF0,0x78,0x18,0x18,0x0C,0x0C,0x0C,0x0C,0x0C,0x18,0x38,0x30,0x00,0x00,0x00,0x00,  
    0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0xC0,0xC0,0xFC,0xFC,0xFC,0xC0,0x00,0x00,  
    0x00,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                                     
                                                                                      
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x0E,0x18,0x30,0x61,0x63,0xC6,0x8C,0x88,  
    0x98,0x98,0x11,0x11,0x11,0x11,0x11,0x11,0x90,0x98,0x88,0x8C,0xC6,0x63,0x61,0x30,  
    0x18,0x0C,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x1C,0x70,0xE0,0x81,0x07,  
    0x0E,0x38,0xE0,0xC1,0x07,0x0E,0x38,0x70,0xC0,0x00,0x00,0x00,0x00,0x00,0x0F,0x3F,  
    0x7F,0xF0,0xC0,0xC0,0x80,0x80,0x80,0x80,0xC0,0xC0,0xE0,0x60,0x00,0x00,0x00,0x3F,  
    0xFF,0xFF,0xC1,0x80,0x80,0x80,0xC1,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x03,0xCF,0x8F,  
    0x9E,0x9C,0xFC,0xF8,0x20,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xFF,0xFF,0xFF,  
    0x00,0x00,0x3E,0x7F,0xFF,0xCD,0x8C,0x8C,0x8C,0x8C,0xCF,0xCF,0x0E,0x08,0x00,0x03,  
    0xC7,0x8F,0x8E,0x9C,0xFC,0xF9,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x3E,0x7F,0xFF,0xCD,0xCC,0x8C,0x8C,0x8C,  
    0xCF,0xCF,0x0F,0x0C,0x00,0x00,0x1C,0x7F,0xFF,0xE3,0xC1,0x80,0x80,0x80,0xC1,0xC1,  
    0x41,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x01,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,  
    0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x08,0x3E,0x7F,0xFF,  
    0xC1,0x80,0x80,0x80,0xC1,0xFF,0x7F,0x3E,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x3E,  
    0x7F,0xFF,0xC1,0x80,0x80,0x80,0xC1,0xE3,0x7F,0x3F,0x08,0x00,0x0C,0x7F,0xFF,0xE3,  
    0xC1,0x80,0x80,0x80,0xC1,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x03,0x0F,0x3F,0xFC,0xF0,  
    0xF8,0x7E,0x1F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x3F,  
    0x7F,0xF0,0xC0,0xC0,0x80,0x80,0x80,0x80,0xC0,0xC0,0xE0,0x60,0x00,0x00,0x08,0x3E,  
    0x7F,0xFF,0xC1,0x80,0x80,0x80,0xC1,0xFF,0x7F,0x3E,0x00,0x00,0x80,0xC0,0xC0,0x00,  
    0x00,0x00,0x00,0x00,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0xFF,0xFF,0xFF,0x80,0x80,0x80,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x1C,  
    0x7F,0xFF,0xE1,0xC0,0x80,0x80,0xC0,0xE1,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xC0,0xC0,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                                     
                                                                                      
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,  
    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,  
    0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,  
    0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,  
    0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x0E,0x0E,  
    0x0D,0x09,0x09,0x08,0x0C,0x0F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x0C,0x0F,0x0F,  
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,  
    0x00,0x00,0x00,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                                     
};

TRANS_TOTAL stBankTotal;
BOOL	fRePrintFlag = FALSE; 
BOOL 	fERMTransaction = FALSE;

static CTOS_FONT_ATTRIB stgFONT_ATTRIB;

BYTE szgTempDate[DATE_BCD_SIZE+1];
BYTE szgTempTime[TIME_BCD_SIZE+1];

BOOL fPrinterStart;

extern int printcopies_cntr;
BOOL fReprintLastSettleFlag = 0;

void vdSetGolbFontAttrib(USHORT FontSize, USHORT X_Zoom, USHORT Y_Zoom, USHORT X_Space, USHORT Y_Space)
{
    memset(&stgFONT_ATTRIB, 0x00, sizeof(stgFONT_ATTRIB));
    
    stgFONT_ATTRIB.FontSize = FontSize;      // Font Size = 12x24
	stgFONT_ATTRIB.X_Zoom = X_Zoom;		    // The width magnifies X_Zoom diameters
	stgFONT_ATTRIB.Y_Zoom = Y_Zoom;		    // The height magnifies Y_Zoom diameters

    stgFONT_ATTRIB.X_Space = X_Space;      // The width of the space between the font with next font
    stgFONT_ATTRIB.Y_Space = Y_Space;      // The Height of the space between the font with next font      
    
}

short printCheckPaper(void)
{
	unsigned short inRet;
	unsigned char key;
	int inLine = 3;

	BYTE szTitle[20+1];
	BYTE szDisMsg[256];
	int inKey=0;

	if( (strTCT.byTerminalType%2) == 0 )
		inLine=2;

	//CTOS_LCDTClearDisplay();

	while(1)
	{
		inRet = CTOS_PrinterStatus();
		vdDebug_LogPrintf("saturn  - printCheckPaper inRet[%02x]",inRet );
		if (inRet==d_OK)
			return d_OK;
		else if(inRet==d_PRINTER_PAPER_OUT)
		{
	          #if 0
			vduiClearBelow(inLine);
			vduiWarningSound();
			vduiDisplayStringCenter(3,"PRINTER OUT OF");
			vduiDisplayStringCenter(4,"PAPER, INSERT");
			vduiDisplayStringCenter(5,"PAPER AND PRESS");
			vduiDisplayStringCenter(6,"ANY TO PRINT.");
			//vduiDisplayStringCenter(8,"[X] CANCEL PRINT");

			CTOS_KBDGet(&key);
			/*
			if(key==d_KBD_CANCEL)
			{
				CTOS_LCDTClearDisplay();
				return -1;
			}
			*/
		#else
			//CTOS_LCDTClearDisplay();
			memset(szTitle, 0x00, sizeof(szTitle));
			szGetTransTitle(srTransRec.byTransType, szTitle);
			
			memset(szDisMsg, 0x00, sizeof(szDisMsg));
			strcpy(szDisMsg, szTitle);
			strcat(szDisMsg, "|");
			strcat(szDisMsg, "PRINTER OUT OF PAPER");	
			strcat(szDisMsg, "|");
			strcat(szDisMsg, "INSERT PAPER AND PRESS OK");	
			strcat(szDisMsg, "|");
			strcat(szDisMsg, "TO PRINT");	
						
			CTOS_Beep();
			//usCTOSS_ConfirmOK(szDisMsg);
			inDisplayMessageBoxWithButton(1,8,"PRINTER OUT OF PAPER","INSERT PAPER AND PRESS OK","TO PRINT", MSG_TYPE_WARNING); // sidumili: added

		#endif
		}
		if(isCheckTerminalMP200() == d_OK)
			return d_OK;
	}
}




void cardMasking(char *szPan, int style)
{
    int num;
    int i;
	char szTemp[30+1];
	
    if (style == PRINT_CARD_MASKING_1)
    {
        num = strlen(szPan) - 10;
        if (num > 0)
        {
            for (i = 0; i < num; i++)
            {
                szPan[6+i] = '*';
            }
        }
    }
    else if (style == PRINT_CARD_MASKING_2)
    {
        num = strlen(szPan) - 12;
        if (num > 0)
        {
            for (i = 0; i < num; i++)
            {
                szPan[12+i] = '*';
            }
        }
    }
    else if (style == PRINT_CARD_MASKING_3)
    {
        memset(szTemp, 0x00, sizeof(szTemp));
        strcpy(szTemp,"****************");
        num = strlen(szPan) - 4;
        memcpy(&szTemp[num],&szPan[num],4);
		szTemp[strlen(szPan)]=0x00;
        strcpy(szPan,szTemp);
    }
}

void vdCTOS_FormatDate(char *pchDate)
 {
	char szLocal[6 + 1] = {0};
	char szMonthNames[36 + 1] = {0};
	short Month = 0;

	strcpy(szLocal, pchDate);
	Month = (szLocal[2] - '0') * 10 + (szLocal[3] - '0') - 1;
	strcpy(szMonthNames, "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC");
	sprintf(pchDate, "%.3s.%.2s,20%.2s", &(szMonthNames[Month * 3]), &(szLocal[4]), szLocal);

	pchDate[13] = ((char) 0);
}

USHORT printDateTime(void)
{
	BYTE   EMVtagVal[64];
	USHORT EMVtagLen; 
	int result;
	char szStr[35 + 1];
	CTOS_RTC SetRTC;
	char szYear[3];
	char szTempDate[d_LINE_SIZE + 1];
	char szTemp[d_LINE_SIZE + 1];
	char szTemp2[d_LINE_SIZE + 1];
	char szTemp3[d_LINE_SIZE + 1];
	char szTemp4[d_LINE_SIZE + 1];
	char szTermSerialNum[15+1]; // print terminal serial number on all txn receipt - mfl
	BYTE baTemp[PAPER_X_SIZE * 64];

	CTOS_RTCGet(&SetRTC);
	sprintf(szYear ,"%02d",SetRTC.bYear);
	memcpy(srTransRec.szYear,szYear,2);

	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp2, 0x00, sizeof(szTemp2));
	memset(szTemp3, 0x00, sizeof(szTemp3));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	wub_hex_2_str(srTransRec.szDate, szTemp,DATE_BCD_SIZE);
	wub_hex_2_str(srTransRec.szTime, szTemp2,TIME_BCD_SIZE);
	memset(szTempDate, 0x00, sizeof(szTempDate));
	sprintf(szTempDate, "%02lu%02lu%02lu", atol(srTransRec.szYear), atol(szTemp)/100, atol(szTemp)%100);
	vdCTOS_FormatDate(szTempDate);
	sprintf(szTemp3, "DATE: %s", szTempDate);
	sprintf(szTemp4, "TIME: %02lu:%02lu:%02lu", atol(szTemp2)/10000,atol(szTemp2)%10000/100, atol(szTemp2)%100);
	inPrintLeftRight(szTemp3, szTemp4, 46);

	return(ST_SUCCESS);
	
}


USHORT printTIDMID(void)
{
	char szStr[d_LINE_SIZE + 1];
	char szStr1[d_LINE_SIZE + 1];
	
	USHORT result;
	BYTE baTemp[PAPER_X_SIZE * 64];
	
	memset(szStr, ' ', d_LINE_SIZE);
	sprintf(szStr, "TID: %s", srTransRec.szTID);

	memset(szStr1, ' ', d_LINE_SIZE);
	sprintf(szStr1, "MID: %s", srTransRec.szMID);

	inPrintLeftRight(szStr, szStr1, 46);
	
	return d_OK;

}

USHORT printMIDTID(void)
{
	char szStr[d_LINE_SIZE + 1];
	char szStr1[d_LINE_SIZE + 1];
	
	USHORT result;
	BYTE baTemp[PAPER_X_SIZE * 64];
	
	memset(szStr, ' ', d_LINE_SIZE);
	sprintf(szStr, "| TID: %s", srTransRec.szTID);

	memset(szStr1, ' ', d_LINE_SIZE);
	sprintf(szStr1, "MID: %s", srTransRec.szMID);

	inPrintLeftRight(szStr1, szStr, 46);
	
	return d_OK;

}



USHORT printBatchInvoiceNO(void)
{
	char szStr[d_LINE_SIZE + 1];
	char szStr1[d_LINE_SIZE + 1];
	
	char szTemp1[d_LINE_SIZE + 1];
	char szTemp2[d_LINE_SIZE + 1];
	USHORT result;
	BYTE baTemp[PAPER_X_SIZE * 64];
		
	memset(szStr, ' ', d_LINE_SIZE);
	memset(szTemp1, ' ', d_LINE_SIZE);
	memset(szTemp2, ' ', d_LINE_SIZE);
	
	wub_hex_2_str(srTransRec.szBatchNo,szTemp1,3);
	wub_hex_2_str(srTransRec.szInvoiceNo, szTemp2, INVOICE_BCD_SIZE);
	vdMyEZLib_LogPrintf("invoice no: %s",szTemp2);
	memset(szStr, 0x00, d_LINE_SIZE);
	sprintf(szStr, "BATCH NUM: %s", szTemp1);
	//result=inPrint(szStr);

	memset(szStr1, 0x00, d_LINE_SIZE);
	sprintf(szStr1, "TRACE NO.: %s", szTemp2);
	//result=inPrint(szStr);

	inPrintLeftRight(szStr, szStr1, 46);
	
	return d_OK;
}


USHORT printBatchNO(void)
{
    char szStr[d_LINE_SIZE + 1];
    char szTemp[d_LINE_SIZE + 1];
    BYTE baTemp[PAPER_X_SIZE * 64];
    USHORT result;
    
    memset(szStr, ' ', d_LINE_SIZE);
    memset(szTemp, ' ', d_LINE_SIZE);
	
	wub_hex_2_str(srTransRec.szBatchNo,szTemp,3);
    sprintf(szStr, "BATCH NO: %s", szTemp);

    result = inPrint(szStr);
    return(result);
}

USHORT printHostLabel(void)
{
    char szStr[d_LINE_SIZE + 1];
    BYTE baTemp[PAPER_X_SIZE * 64];
    USHORT result;

    memset(szStr, 0x00, sizeof(szStr));
    sprintf(szStr, "HOST: %s", srTransRec.szHostLabel);

    result = inPrint(szStr);
    return(result);
}


void vdPrintCenter(unsigned char *strIn)
{
	unsigned char tucPrint [24*4+1];
	short i,spacestring;
    USHORT usCharPerLine = 32;
    BYTE baTemp[PAPER_X_SIZE * 64];

	//vdDebug_LogPrintf("vdPrintCenter | fERMTransaction[%d]", fERMTransaction);

	if ((isCheckTerminalMP200() == d_OK) && (isERMMode() == TRUE))
	{
		usCharPerLine = 46;
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
	else
	{
		if(d_FONT_24x24 == stgFONT_ATTRIB.FontSize && NORMAL_SIZE == stgFONT_ATTRIB.X_Zoom)
        usCharPerLine = 32;
	    else if(d_FONT_24x24 == stgFONT_ATTRIB.FontSize && DOUBLE_SIZE == stgFONT_ATTRIB.X_Zoom)
	        usCharPerLine = 16;
	    else if(d_FONT_16x16 == stgFONT_ATTRIB.FontSize && NORMAL_SIZE == stgFONT_ATTRIB.X_Zoom)
	        usCharPerLine = 48;
	    else if(d_FONT_16x16 == stgFONT_ATTRIB.FontSize && DOUBLE_SIZE == stgFONT_ATTRIB.X_Zoom)
	        usCharPerLine = 24;
	    else
	        usCharPerLine = 32;
	}    
        
    i = strlen(strIn);
	spacestring=(usCharPerLine-i)/2;
				
	memset(tucPrint,0x20,55);
	memcpy(tucPrint+spacestring,strIn,usCharPerLine);	
	
	tucPrint[i+spacestring]=0;
    memset (baTemp, 0x00, sizeof(baTemp));
      
	inCCTOS_PrinterBufferOutput(tucPrint,&stgFONT_ATTRIB, fERMTransaction);
}

void vdPrintTitleCenter(unsigned char *strIn)
{
	unsigned char tucPrint [24*4+1];
	short i,spacestring;
    USHORT usCharPerLine = 24;
    BYTE baTemp[PAPER_X_SIZE * 64];

	//vdDebug_LogPrintf("vdPrintTitleCenter | fERMTransaction[%d]", fERMTransaction);
	
    if ((isCheckTerminalMP200() == d_OK) && (strTCT.byERMMode == 2))
	{
		usCharPerLine = 46;
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
	else
    	vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
    
    i = strlen(strIn);
	spacestring=(usCharPerLine-i)/2;
				
	memset(tucPrint,0x20,30);
	
	memcpy(tucPrint+spacestring,strIn,usCharPerLine);	
	
	tucPrint[i+spacestring]=0;
        
	inPrint(tucPrint);

    vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
    
}

USHORT printDividingLine(int style)
{
    if (style == DIVIDING_LINE_DOUBLE)
        return(inPrint("============================================="));
    else
        return(inPrint("---------------------------------------------"));
}

void vdCTOSS_GetEMVTagsDescription(USHORT usTag, char *szValue)
{
    switch(usTag)
    {
        case TAG_50:
            strcpy(szValue, "Appl Label");
            break;

        case TAG_57:
            strcpy(szValue, "Track2 Data");
            break;

        case TAG_5F2A_TRANS_CURRENCY_CODE:
            strcpy(szValue, "Txn CurrCode");
            break;

        case TAG_5F34_PAN_IDENTFY_NO:
            strcpy(szValue, "PAN Seq Num");
            break;

        case TAG_82_AIP:
            strcpy(szValue, "AIP");
            break;

        case TAG_8A_AUTH_CODE:
            strcpy(szValue, "Auth Code");
            break;

        case TAG_8F:
            strcpy(szValue, "CAPK Index");
            break;

        case TAG_91_ARPC:
            strcpy(szValue, "ARPC");
            break;

        case TAG_95:
            strcpy(szValue, "TVR");
            break;

        case TAG_9A_TRANS_DATE:
            strcpy(szValue, "Txn Date");
            break;

        case TAG_9B:
            strcpy(szValue, "TSI");
            break;

        case TAG_9C_TRANS_TYPE:
            strcpy(szValue, "Txn Type");
            break;

        case TAG_9F02_AUTH_AMOUNT:
            strcpy(szValue, "Txn Amt");
            break;

        case TAG_9F03_OTHER_AMOUNT:
            strcpy(szValue, "Other Amt");
            break;

        case TAG_9F10_IAP:
            strcpy(szValue, "IAP");
            break;

        case TAG_9F12:
            strcpy(szValue, "Appl Pref Name");
            break;

        case TAG_9F1A_TERM_COUNTRY_CODE:
            strcpy(szValue, "Term CurrCode");
            break;

        case TAG_9F1B_TERM_FLOOR_LIMIT:
            strcpy(szValue, "FloorLimit");
            break;

        case TAG_9F26_EMV_AC:
            strcpy(szValue, "AC");
            break;

        case TAG_9F27:
            strcpy(szValue, "CID");
            break;

        case TAG_9F33_TERM_CAB:
            strcpy(szValue, "Term Cab");
            break;

        case TAG_9F34_CVM:
            strcpy(szValue, "CVM");
            break;

        case TAG_9F36_ATC:
            strcpy(szValue, "ATC");
            break;

        case TAG_9F37_UNPREDICT_NUM:
            strcpy(szValue, "Unpredict Num");
            break;

        case TAG_9F5B:
            strcpy(szValue, "Script Result");
            break;

        case TAG_71:
            strcpy(szValue, "Issuer Script1");
            break;

        case TAG_72:
            strcpy(szValue, "Issuer Script2");
            break;

        default :
            szValue[0] = 0x00;
            break;
    }
}

void vdPrintEMVTags(void)
{
    BYTE   EMVtagVal[64];
    USHORT EMVtagLen;
    USHORT usPrintTag;
    USHORT usPrintTagLen;
    USHORT usOffset;
    char szStr[d_LINE_SIZE + 1];
    char szTagDesp[50];
    char szEMVTagList[512];
    char szEMVTagListHex[256];
    BYTE szOutEMVData[2048];
    USHORT inTagLen = 0;
    int shHostIndex, inRet=0;
	
    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;

	//CTOS_LCDTClearDisplay();

    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_PRINT_EMV_TAGS);
        if(d_OK != inRet)
              return ;
    }
	else
	{
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return ;
            
            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return ;
        }
    }    
	
    if( printCheckPaper()==-1)
    	return ;

    vdCTOSS_PrinterStart(100);
    CTOS_PrinterSetHeatLevel(4);  
	
    inCTOS_DisplayPrintBMP();
	
    CTOS_LanguagePrinterFontSize(d_FONT_16x16, 0, TRUE);	
	
	vdCTOSS_PrinterBMPPic(0, 0, "logo.bmp");
	
    vdPrintTitleCenter("EMV TAGS DATA");

    memset(szOutEMVData,0x00,sizeof(szOutEMVData));

    usCTOSS_EMV_MultiDataGet(PRINT_EMV_TAGS_LIST, &inTagLen, szOutEMVData);
    DebugAddHEX("PRINT_EMV_TAGS_LIST",szOutEMVData,inTagLen);

    memset(szEMVTagList,0x00,sizeof(szEMVTagList));
    memset(szEMVTagListHex,0x00,sizeof(szEMVTagListHex));
    strcpy(szEMVTagList, PRINT_EMV_TAGS_LIST);
    wub_str_2_hex(szEMVTagList, szEMVTagListHex, strlen(szEMVTagList));

    usOffset = 0;
    while(szEMVTagListHex[usOffset] != 0x00)
    {
        vdCTOS_PrinterFline(1);
        
        memset(szStr, 0x00, sizeof(szStr));
        if ((szEMVTagListHex[usOffset] & 0x1F) == 0x1F)	// If the least 5 bits of the first byte are set, it is a two byte Tag
    	{
    		usPrintTag = szEMVTagListHex[usOffset];
    		usPrintTag = ((usPrintTag << 8) | szEMVTagListHex[usOffset+1]);
    		usPrintTagLen = 2;		
    	}
    	else
    	{
    		usPrintTag = szEMVTagListHex[usOffset];
            usPrintTag = usPrintTag & 0x00FF;
            usPrintTagLen = 1;
    	}
        memcpy(szStr, &szEMVTagList[usOffset*2], usPrintTagLen*2);
        usOffset += usPrintTagLen;

        memset(szTagDesp,0x00,sizeof(szTagDesp));
        vdCTOSS_GetEMVTagsDescription(usPrintTag, szTagDesp);
        vdDebug_LogPrintf("szStr[%s] usPrintTag[%X] szTagDesp[%s]", szStr, usPrintTag, szTagDesp);  
        vdMyEZLib_Printf("%s (%s)", szStr,szTagDesp);
        memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
        EMVtagLen = 0;
        usCTOSS_FindTagFromDataPackage(usPrintTag, EMVtagVal, &EMVtagLen, szOutEMVData, inTagLen);
        memset(szStr, ' ', d_LINE_SIZE);
        if(EMVtagLen == 0)
            memcpy(szStr,"No DATA" , 7);
        else    
            wub_hex_2_str(EMVtagVal, szStr, EMVtagLen);
        vdMyEZLib_Printf("Len:%d val:%s", EMVtagLen, szStr);
    }
        
    vdCTOS_PrinterFline(1);
    vdPrintTitleCenter("*** END OF REPORT ***");
	vdLineFeed(FALSE);
	vdCTOSS_PrinterEnd();
}

void vdPrintTerminalConfig(void)
{
    int inResult;
    int shHostIndex = 1;
    int inNum = 0;
    int inNumOfHost = 0;
    int inNumOfMerchant = 0;
    int inLoop =0 ;
    BYTE szStr[50];
    BYTE szBuf[50];
    BYTE baTemp[PAPER_X_SIZE * 64];

    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

	vdCTOSS_PrinterStart(100);
    CTOS_PrinterSetHeatLevel(4);  

	ushCTOS_PrintHeader(0);

    vdCTOS_PrinterFline(1);
            
    memset(szStr, 0x00, sizeof(szStr));
    memset(szBuf, 0x00, sizeof(szBuf));
    wub_hex_2_str(strTCT.szInvoiceNo, szBuf, 3);
    sprintf(szStr, "INVOICE NO: %s", szBuf);                        
    inPrint(szStr);
	
    memset(szStr, 0x00, sizeof(szStr));
    sprintf(szStr, "PABX: %s", strTCT.szPabx);                        
    
    inPrint(szStr);
	
    //check host num
    inNumOfHost = inHDTNumRecord();
    
    vdDebug_LogPrintf("[inNumOfHost]-[%d]", inNumOfHost);
    for(inNum =1 ;inNum <= inNumOfHost; inNum++)
    {
        if(inHDTRead(inNum) == d_OK)
        {                
            inCPTRead(inNum);

            vdCTOS_PrinterFline(1);
            
            memset(szStr, 0x00, sizeof(szStr));
            sprintf(szStr, "HOST: %s", strHDT.szHostLabel);                        
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            memset(szBuf, 0x00, sizeof(szBuf));
            wub_hex_2_str(strHDT.szTPDU, szBuf, 5);
            sprintf(szStr, "TPDU: %s", szBuf);                        
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            if(strCPT.inCommunicationMode == ETHERNET_MODE)
            {   
                sprintf(szStr, "COMM TYPE: %s", "ETHERNET");           
            }
            else if(strCPT.inCommunicationMode == DIAL_UP_MODE)
            {
                sprintf(szStr, "COMM TYPE: %s", "DIAL_UP");         
            }       
            else if(strCPT.inCommunicationMode == GPRS_MODE)
            {
                sprintf(szStr, "COMM TYPE: %s", "GPRS");                 
            }
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            sprintf(szStr, "PRI NUM : %s", strCPT.szPriTxnPhoneNumber);                        
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            sprintf(szStr, "SEC NUM : %s", strCPT.szSecTxnPhoneNumber);                        
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            sprintf(szStr, "PRI IP  : %s", strCPT.szPriTxnHostIP);                        
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            sprintf(szStr, "PRI PORT: %d", strCPT.inPriTxnHostPortNum);                        
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            sprintf(szStr, "SEC IP  : %s", strCPT.szSecTxnHostIP);                        
            inPrint(szStr);
			
            memset(szStr, 0x00, sizeof(szStr));
            sprintf(szStr, "SEC PORT: %d", strCPT.inSecTxnHostPortNum);                        
            inPrint(szStr);
			
            inMMTReadNumofRecords(strHDT.inHostIndex,&inNumOfMerchant);
        
            vdDebug_LogPrintf("[inNumOfMerchant]-[%d]strHDT.inHostIndex[%d]", inNumOfMerchant,strHDT.inHostIndex);
            for(inLoop=1; inLoop <= inNumOfMerchant;inLoop++)
            {
                if((inResult = inMMTReadRecord(strHDT.inHostIndex,strMMT[inLoop-1].MITid)) !=d_OK)
                {
                    vdDebug_LogPrintf("[read MMT fail]-Mitid[%d]strHDT.inHostIndex[%d]inResult[%d]", strMMT[inLoop-1].MITid,strHDT.inHostIndex,inResult);
                    continue;
                    //break;
                }
                else 
                {
                    if(strMMT[0].fMMTEnable)
                    {                        
                        memset(szStr, 0x00, sizeof(szStr));
                        sprintf(szStr, "MERCHANT:%s", strMMT[0].szMerchantName);                        
                        inPrint(szStr);
						
                        memset(szStr, 0x00, sizeof(szStr));
                        sprintf(szStr, "TID : %s", strMMT[0].szTID);                        
                        inPrint(szStr);
						
                        memset(szStr, 0x00, sizeof(szStr));
                        sprintf(szStr, "MID : %s", strMMT[0].szMID);                        
                        inPrint(szStr);
						
                        memset(szStr, 0x00, sizeof(szStr));
                        memset(szBuf, 0x00, sizeof(szBuf));
                        wub_hex_2_str(strMMT[0].szBatchNo, szBuf, 3);
                        sprintf(szStr, "BATCH NO: %s", szBuf);                        
                        inPrint(szStr);
                    }                    

                }
            }
        }
        else
            continue;

    }

    vdLineFeed(FALSE);
    vdCTOSS_PrinterEnd();
    return ;
}

void printCardHolderName(void)
{
    USHORT shLen;
    char szStr[d_LINE_SIZE + 1];
    
    shLen = strlen(srTransRec.szCardholderName);

    if(shLen > 0)
    {
        memset(szStr, 0x00, d_LINE_SIZE);
        strcpy(szStr, srTransRec.szCardholderName);
        szStr[31]=0x00;
        //while(0x20 == szStr[--shLen] && shLen >= 0)
            //szStr[shLen] = 0x00;

    	//vdPrintCenter(szStr);	
    	inPrint(szStr);
    }
}

void vdPrintFormattedAmount(unsigned char *prtDisplay, unsigned char *prtAmount, int inWidth)
{
    char szAmtBuf[24+1] = { 0 };
	char szStr[24+1] = { 0 };

	memset(szStr, 0, sizeof(szStr));
	memset(szAmtBuf, 0, sizeof(szAmtBuf));
	if(srTransRec.byVoided == TRUE)
	{
		szAmtBuf[0]='-';
	    vdCTOS_FormatAmount("NNN,NNN,NNn.nn", prtAmount, &szAmtBuf[1]);
	}
	else
	    vdCTOS_FormatAmount("NNN,NNN,NNn.nn", prtAmount, szAmtBuf);
	
	sprintf(szStr, "%s%s", prtDisplay, strCST.szCurSymbol);
	inPrintLeftRight(szStr, szAmtBuf, inWidth);
}

USHORT ushCTOS_PrintDemo(void)
{
    BYTE strIn[40];
    unsigned char tucPrint [24*4+1];
    short i,spacestring;
    USHORT usCharPerLine = 24;
    BYTE baTemp[PAPER_X_SIZE * 64];

	vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);

    memset(strIn, 0x00, sizeof(strIn));
    strcpy(strIn, "DEMO");
    i = strlen(strIn);
    spacestring=(usCharPerLine-i)/2;
                
    memset(tucPrint,0x20,30);
    
    memcpy(tucPrint+spacestring,strIn,usCharPerLine);   
    
    tucPrint[i+spacestring]=0;

    vdCTOS_PrinterFline(1);
        
    inPrint(tucPrint);
    
    vdCTOS_PrinterFline(1);

	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

    return d_OK;
    
}



USHORT ushCTOS_PrintHeader(int page)
{	
    vdDebug_LogPrintf("saturn ushCTOS_PrintHeader-1");
	vdDebug_LogPrintf("strHDT.szHeaderLogoName[%s]", strHDT.szHeaderLogoName);
	vdDebug_LogPrintf("fRePrintFlag[%d] fReprintLastSettleFlag[%d]", fRePrintFlag, fReprintLastSettleFlag);

	// sidumili:Comment not to print a diplicate header logo
	#if 0
	//print Logo	
	if(strlen(strHDT.szHeaderLogoName))
		vdCTOSS_PrinterBMPPic(0, 0, strHDT.szHeaderLogoName);
	else
		vdCTOSS_PrinterBMPPic(0, 0, "logo.bmp");
	#endif

	if (fReprintLastSettleFlag==1)
		vdCTOSS_PrinterBMPPic(0, 0, strHDT.szHeaderLogoName);
	
	if(fRePrintFlag == TRUE)	
    {   
        vdDebug_LogPrintf("srTransRec.HDTid[%d] srTransRec.MITid[%d]",srTransRec.HDTid, srTransRec.MITid);
		if ( inMMTReadRecord(srTransRec.HDTid,srTransRec.MITid) != d_OK)
        {
            vdSetErrorMessage("LOAD MMT ERR");
            return(d_NO);
        }
        vdDebug_LogPrintf("srTransRec.HDTid[%d] srTransRec.MITid[%d] strMMT[0].szRctHdr1[%s]",srTransRec.HDTid, srTransRec.MITid, strMMT[0].szRctHdr1);
    }

	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
	//merhcant name and address
	if(strlen(strMMT[0].szRctHdr1) > 0)
		vdPrintCenter(strMMT[0].szRctHdr1);
	if(strlen(strMMT[0].szRctHdr2) > 0)
		vdPrintCenter(strMMT[0].szRctHdr2);
	if(strlen(strMMT[0].szRctHdr3) > 0)
    	vdPrintCenter(strMMT[0].szRctHdr3);
	if(strlen(strMMT[0].szRctHdr4) > 0)
    	vdPrintCenter(strMMT[0].szRctHdr4);
	if(strlen(strMMT[0].szRctHdr5) > 0)
    	vdPrintCenter(strMMT[0].szRctHdr5);

    //vdCTOS_PrinterFline(1);
	
    if(VS_TRUE == strTCT.fDemo)
    {
        ushCTOS_PrintDemo();
    }

    if(fRePrintFlag == TRUE)
    {
        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
        vdPrintCenter("DUPLICATE");
        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
    }
    
	vdCTOS_PrinterFline(1);
    
    return d_OK;
}

USHORT ushCTOS_PrintInstallmentDetails(void)
{	
	char szTemp[d_LINE_SIZE+1];
	char szStr[d_LINE_SIZE+1];
    char szTempAmount[12+1];
	char szTempAmountHex[6+1];
	
	vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
	memset(szTemp, 0x00, d_LINE_SIZE);
	wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
	vdPrintFormattedAmount("TOTAL :", szTemp, 24);
		
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);

    //inPrintLeftRight("PROMO               :", srTransRec.szHostLabel, 46);
	
    inPrintLeftRight("TERMS/MOS           :", srTransRec.szTerms, 46);
	
	memset(szTemp, 0x00, d_LINE_SIZE);
	sprintf(szTemp, "%012ld", atol(srTransRec.szTotalInstAmnt));
	vdPrintFormattedAmount("GROSS AMT           :", szTemp, 46);

	memset(szStr, 0x00, d_LINE_SIZE);
	sprintf(szStr, "%.1s.%s", srTransRec.szFactorRate, &srTransRec.szFactorRate[1]);
    inPrintLeftRight("RATE                :", szStr, 46);

	memset(szTemp, 0x00, d_LINE_SIZE);
	sprintf(szTemp, "%012ld", atol(srTransRec.szMoAmort));
	vdPrintFormattedAmount("MONTHLY AMORTIZATION:", szTemp, 46);	
}

USHORT ushCTOS_PrintBody(int page)
{	
    char szStr[d_LINE_SIZE + 1];
    char szTemp[d_LINE_SIZE + 1];
    char szTemp1[d_LINE_SIZE + 1];
    char szTemp3[d_LINE_SIZE + 1];
    char szTemp4[d_LINE_SIZE + 1];
    char szTemp5[d_LINE_SIZE + 1];
    char szFormatPAN[strlen(srTransRec.szPAN) + 7];//to account for space as in PAN format and to include additional PAN no. 7 has been added -Meena 08/01/2012
    int inFmtPANSize;
    BYTE baTemp[PAPER_X_SIZE * 64];
    USHORT result;
    int num,i,inResult;
    unsigned char tucPrint [24*4+1];	
    BYTE   EMVtagVal[64];
    USHORT EMVtagLen; 
    short spacestring;
    BYTE   key;

	char szStrLeft[d_LINE_SIZE + 1];
	char szStrRight[d_LINE_SIZE + 1];

    vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
	if(CUSTOMER_COPY_RECEIPT == page)
	{
    	//memset(szTemp1, ' ', d_LINE_SIZE);
    	//sprintf(szTemp1,"%s",srTransRec.szHostLabel);
    	//vdPrintCenter(szTemp1);
		
		printDateTime();

        printTIDMID(); 
		
		printBatchInvoiceNO();

		memset(szStr, ' ', d_LINE_SIZE);
		if(srTransRec.fInstallment)
            if (srTransRec.byTransType == VOID)
				sprintf(szStr, "VOID %s", srTransRec.szHostLabel);
			else
				strcpy(szStr, srTransRec.szHostLabel);
		else
    	    szGetTransTitle(srTransRec.byTransType, szStr);     
		
  		vdPrintTitleCenter(szStr);    

        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
		EMVtagLen = 0;
		memset(szStr, ' ', d_LINE_SIZE);
		vdMyEZLib_LogPrintf("CDT index: %d",srTransRec.CDTid);

		sprintf(szStr, "%s", srTransRec.szCardLable);
		vdMyEZLib_LogPrintf("Card label: %s",srTransRec.szCardLable);
		inPrint(szStr);

		//cardMasking(srTransRec.szPAN, PRINT_CARD_MASKING_1);
		//strcpy(szTemp4, srTransRec.szPAN);
		//memset (baTemp, 0x00, sizeof(baTemp));
		//CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szTemp4, &stgFONT_ATTRIB);
		//CTOS_PrinterBufferOutput((BYTE *)baTemp, 4);		

        memset (baTemp, 0x00, sizeof(baTemp));
		vdCTOS_FormatPAN(strIIT.szMaskCustomerCopy, srTransRec.szPAN, baTemp, strIIT.fMaskCustCopy);
        inPrint(baTemp);
		
        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);

		printCardHolderName();
		
		//Exp date and Entry mode
		memset(szStr, ' ', d_LINE_SIZE);
		memset(szTemp, 0, sizeof(szTemp));
		memset(szTemp1, 0, sizeof(szTemp1));
		memset(szTemp4, 0, sizeof(szTemp4));
		memset(szTemp5, 0, sizeof(szTemp5));
		wub_hex_2_str(srTransRec.szExpireDate, szTemp,EXPIRY_DATE_BCD_SIZE);
		DebugAddSTR("EXP",szTemp,12);  
		
		for (i =0; i<4;i++)
			szTemp[i] = '*';
		memcpy(szTemp4,&szTemp[0],2);
		memcpy(szTemp5,&szTemp[2],2);

		if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
			memcpy(szTemp1,"Chip",4);
		else if(srTransRec.byEntryMode==CARD_ENTRY_MANUAL)
			memcpy(szTemp1,"Manual",6);
		else if(srTransRec.byEntryMode==CARD_ENTRY_MSR)
			memcpy(szTemp1,"MSR",3);
		else if(srTransRec.byEntryMode==CARD_ENTRY_FALLBACK)
			memcpy(szTemp1,"Fallback",8);
		else if(srTransRec.byEntryMode==CARD_ENTRY_WAVE)
		{
			if ('4' ==srTransRec.szPAN[0])
				memcpy(szTemp1,"PayWave",7);
			if ('5' ==srTransRec.szPAN[0])
				memcpy(szTemp1,"PayPass",7);
		}

		DebugAddSTR("ENT:",baTemp,12);  
		//sprintf(szTemp,"%s%s/%s          %s%s","EXP: ",szTemp4,szTemp5,"ENT:",szTemp1);		
        //inPrint(szTemp);

		// Print Expiry Date / Entry Mode
		memset(szStrLeft, 0x00, sizeof(szStrLeft));
		memset(szStrRight, 0x00, sizeof(szStrRight));
		sprintf(szStrLeft, "EXP: %s/%s", szTemp4, szTemp5);
		sprintf(szStrRight, "ENT: %s", szTemp1);		
		inPrintLeftRight(szStrLeft, szStrRight, 46);

		/*
        //Reference num
        if(strCDT.inType != DEBIT_CARD)
        {
            memset(szStr, ' ', d_LINE_SIZE);
            memset (baTemp, 0x00, sizeof(baTemp));					
            memset(szStr, ' ', d_LINE_SIZE);
            sprintf(szStr, "REF NO.   : %s", srTransRec.szRRN);
            inPrint(szStr);
        }
		
		//Auth response code
		memset(szStr, ' ', d_LINE_SIZE);
		memset(szStr, ' ', d_LINE_SIZE);
		sprintf(szStr, "APPR. CODE: %s", srTransRec.szAuthCode);
		inPrint(szStr);
		*/

		// Print Ref No / Appr Code
		memset(szTemp3, 0x00, sizeof(szTemp3));
		memset(szTemp4, 0x00, sizeof(szTemp4));
		sprintf(szTemp3, "REF NO.   : %s", srTransRec.szRRN);
		sprintf(szTemp4, "APPR. CODE: %s", srTransRec.szAuthCode);		
		inPrintLeftRight(szTemp3, szTemp4, 46);
		
		memset(szStr, ' ', d_LINE_SIZE);
		memset(szTemp, ' ', d_LINE_SIZE);
        if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
        {
            if(srTransRec.byTransType != REFUND && srTransRec.byOrgTransType != REFUND)
            {
                //AC
                wub_hex_2_str(srTransRec.stEMVinfo.T9F26, szTemp, 8);
                sprintf(szStr, "APP CRYPT : %s", szTemp);
                inPrint(szStr);
                //TVR
                EMVtagLen = 5;
                memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
                memcpy(EMVtagVal, srTransRec.stEMVinfo.T95, EMVtagLen);
                memset(szStr, ' ', d_LINE_SIZE);
                sprintf(szStr, "TVR VALUE : %02x%02x%02x%02x%02x", EMVtagVal[0], EMVtagVal[1], EMVtagVal[2], EMVtagVal[3], EMVtagVal[4]);
                inPrint(szStr);
            }
			
			//AID
			memset(szStr, ' ', d_LINE_SIZE);
            EMVtagLen = srTransRec.stEMVinfo.T84_len;
			memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
			memcpy(EMVtagVal, srTransRec.stEMVinfo.T84, EMVtagLen);
			memset(szTemp, ' ', d_LINE_SIZE);
			wub_hex_2_str(EMVtagVal, szTemp, EMVtagLen);
			sprintf(szStr, "AID       : %s",szTemp);
			inPrint(szStr);
			memset(szStr, ' ', d_LINE_SIZE);
			sprintf(szStr, "APP LABEL : %s", srTransRec.stEMVinfo.szChipLabel);
			inPrint(szStr);
		}
		
		memset(szStr, ' ', d_LINE_SIZE);
		memset(szTemp, ' ', d_LINE_SIZE);
		memset(szTemp1, ' ', d_LINE_SIZE);
		memset(szTemp3, ' ', d_LINE_SIZE);
		wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
		wub_hex_2_str(srTransRec.szBaseAmount, szTemp1, AMT_BCD_SIZE);

        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		if (srTransRec.byTransType == SALE || srTransRec.byTransType == SALE_OFFLINE || srTransRec.byTransType == VOID)
		{
            if(srTransRec.fInstallment)
            {
                ushCTOS_PrintInstallmentDetails();				
            }
			else
			{
                //Tip amount
                memset(szStr, ' ', d_LINE_SIZE);
                sprintf(szStr,"%s    %s","TIP",strCST.szCurSymbol);
                memset (baTemp, 0x00, sizeof(baTemp));
                memset (szTemp4, 0x00, sizeof(szTemp4));
                wub_hex_2_str(srTransRec.szTipAmount, szTemp4, AMT_BCD_SIZE);

                DebugAddSTR("TIP:",szTemp4,12);
                if (TRUE ==strHDT.fTipAllowFlag)
                {
                    if(atol(szTemp4) > 0)
                    {
						//Base amount
						vdPrintFormattedAmount("AMOUNT:", szTemp1, 24);

                        vdPrintFormattedAmount("TIP   :", szTemp4, 24);
                        
                        //Total amount
                        vdPrintFormattedAmount("TOTAL :", szTemp, 24);
                    }
                    else
                    {
						if(srTransRec.byTransType == VOID)
						{
                            vdPrintFormattedAmount("TOTAL :", szTemp1, 24);							
						}
						else
						{
							//Base amount
							vdPrintFormattedAmount("AMOUNT:", szTemp1, 24);

                            memset(szStr, ' ', d_LINE_SIZE);
                            
                            inPrint(szStr);
							inPrintLeftRight(" ","______________",24);
							
                            //Total amount
                            memset(szStr, ' ', d_LINE_SIZE);
                            sprintf(szStr, "TOTAL :%s", strCST.szCurSymbol);
                            inPrint(szStr);
							inPrintLeftRight(" ","______________",24);
						}
                    }
                }
				else
                    vdPrintFormattedAmount("TOTAL :", szTemp1, 24);					
			}
			
		}
		else if (srTransRec.byTransType == SALE_TIP)
		{
			memset(szTemp3, ' ', d_LINE_SIZE);
			wub_hex_2_str(srTransRec.szTipAmount, szTemp3, AMT_BCD_SIZE);
			vdPrintFormattedAmount("AMOUNT:", szTemp1, 24);
			vdPrintFormattedAmount("TIPS  :", szTemp3, 24);
			vdPrintFormattedAmount("TOTAL :", szTemp, 24);
			
		}
		else
		{			
            vdPrintFormattedAmount("AMOUNT:", szTemp, 24);
		}

        //vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
	else if(MERCHANT_COPY_RECEIPT == page || BANK_COPY_RECEIPT == page)
	{
    	//memset(szTemp1, ' ', d_LINE_SIZE);
    	//sprintf(szTemp1,"%s",srTransRec.szHostLabel);
    	//vdPrintCenter(szTemp1);
		
		printDateTime();
		inResult = printTIDMID(); 
		
		printBatchInvoiceNO(); // pat confirm hang
		
		memset(szStr, ' ', d_LINE_SIZE);
		if(srTransRec.fInstallment)
            if (srTransRec.byTransType == VOID)
				sprintf(szStr, "VOID %s", srTransRec.szHostLabel);
			else
				strcpy(szStr, srTransRec.szHostLabel);
		else
    	    szGetTransTitle(srTransRec.byTransType, szStr); 
		
  		vdPrintTitleCenter(szStr);    

        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
	    memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
		EMVtagLen = 0;
		memset(szStr, ' ', d_LINE_SIZE);
		vdMyEZLib_LogPrintf("CDT index: %d",srTransRec.CDTid);
		
		sprintf(szStr, "%s",srTransRec.szCardLable);
		vdMyEZLib_LogPrintf("Card label: %s",srTransRec.szCardLable);
		inPrint(szStr);
		
		//cardMasking(srTransRec.szPAN, PRINT_CARD_MASKING_1);
		//strcpy(szTemp4, srTransRec.szPAN);
		//memset (baTemp, 0x00, sizeof(baTemp));
		//CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szTemp4, &stgFONT_ATTRIB);
		//CTOS_PrinterBufferOutput((BYTE *)baTemp, 4);		

        memset (baTemp, 0x00, sizeof(baTemp));
		vdCTOS_FormatPAN(strIIT.szMaskMerchantCopy, srTransRec.szPAN, baTemp, strIIT.fMaskMerchCopy);
        inPrint(baTemp);

        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);

		printCardHolderName();
        
		//Exp date and Entry mode
		memset(szStr, ' ', d_LINE_SIZE);
		memset(szTemp, 0, sizeof(szTemp));
		memset(szTemp1, 0, sizeof(szTemp1));
		memset(szTemp4, 0, sizeof(szTemp4));
		memset(szTemp5, 0, sizeof(szTemp5));
		wub_hex_2_str(srTransRec.szExpireDate, szTemp,EXPIRY_DATE_BCD_SIZE);
		DebugAddSTR("EXP",szTemp,12);  
		
		for (i =0; i<4;i++)
			szTemp[i] = '*';
		memcpy(szTemp4,&szTemp[0],2);
		memcpy(szTemp5,&szTemp[2],2);

		if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
			memcpy(szTemp1,"Chip",4);
		else if(srTransRec.byEntryMode==CARD_ENTRY_MANUAL)
			memcpy(szTemp1,"Manual",6);
		else if(srTransRec.byEntryMode==CARD_ENTRY_MSR)
			memcpy(szTemp1,"MSR",3);
		else if(srTransRec.byEntryMode==CARD_ENTRY_FALLBACK)
			memcpy(szTemp1,"Fallback",8);
		
		DebugAddSTR("ENT:",szTemp1,12);  
		//sprintf(szTemp,"%s%s/%s          %s%s","EXP: ",szTemp4,szTemp5,"ENT:",szTemp1);		
        //inPrint(szTemp);

		// Print Expiry Date / Entry Mode
		memset(szStrLeft, 0x00, sizeof(szStrLeft));
		memset(szStrRight, 0x00, sizeof(szStrRight));
		sprintf(szStrLeft, "EXP: %s/%s", szTemp4, szTemp5);
		sprintf(szStrRight, "ENT: %s", szTemp1);		
		inPrintLeftRight(szStrLeft, szStrRight, 46);

		/*
        //Reference num
        if(strCDT.inType != DEBIT_CARD)
        {
            memset(szStr, ' ', d_LINE_SIZE);
            sprintf(szStr, "REF NO.   : %s", srTransRec.szRRN);
            inPrint(szStr);
        }

		//Auth response code
		memset(szStr, ' ', d_LINE_SIZE);
		sprintf(szStr, "APPR. CODE: %s", srTransRec.szAuthCode);
		inPrint(szStr);
		*/

		// Print Ref No / Appr Code
		memset(szTemp3, 0x00, sizeof(szTemp3));
		memset(szTemp4, 0x00, sizeof(szTemp4));
		sprintf(szTemp3, "REF NO.   : %s", srTransRec.szRRN);
		sprintf(szTemp4, "APPR. CODE: %s", srTransRec.szAuthCode);		
		inPrintLeftRight(szTemp3, szTemp4, 46);
		
		memset(szStr, ' ', d_LINE_SIZE);
		memset(szTemp, ' ', d_LINE_SIZE);
        if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
        {
            if(srTransRec.byTransType != REFUND && srTransRec.byOrgTransType != REFUND)
            {
                //AC
                wub_hex_2_str(srTransRec.stEMVinfo.T9F26, szTemp, 8);
                sprintf(szStr, "APP CRYPT : %s", szTemp);
                inPrint(szStr);
				
                //TVR
                EMVtagLen = 5;
                memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
                memcpy(EMVtagVal, srTransRec.stEMVinfo.T95, EMVtagLen);
                memset(szStr, ' ', d_LINE_SIZE);
                sprintf(szStr, "TVR VALUE : %02x%02x%02x%02x%02x", EMVtagVal[0], EMVtagVal[1], EMVtagVal[2], EMVtagVal[3], EMVtagVal[4]);
                inPrint(szStr);
            }
			
            //AID
            memset(szStr, ' ', d_LINE_SIZE);
            EMVtagLen = srTransRec.stEMVinfo.T84_len;
            memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
            memcpy(EMVtagVal, srTransRec.stEMVinfo.T84, EMVtagLen);
            memset(szTemp, ' ', d_LINE_SIZE);
            wub_hex_2_str(EMVtagVal, szTemp, EMVtagLen);
            sprintf(szStr, "AID       : %s",szTemp);
            inPrint(szStr);
			
            /* EMV: Get Application Label - start -- jzg */
            memset(szStr, ' ', d_LINE_SIZE);
            sprintf(szStr, "APP LABEL : %s", srTransRec.stEMVinfo.szChipLabel);
            inPrint(szStr);
            /* EMV: Get Application Label - end -- jzg */
		}
		
		memset(szStr, ' ', d_LINE_SIZE);
		memset(szTemp, ' ', d_LINE_SIZE);
		memset(szTemp1, ' ', d_LINE_SIZE);
		memset(szTemp3, ' ', d_LINE_SIZE);
		wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
		wub_hex_2_str(srTransRec.szBaseAmount, szTemp1, AMT_BCD_SIZE);

        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
        
		if (srTransRec.byTransType == SALE || srTransRec.byTransType == SALE_OFFLINE || srTransRec.byTransType == VOID)
		{
			//Base amount
            if(srTransRec.fInstallment)
            {
                ushCTOS_PrintInstallmentDetails();				
            }
			else
			{
    			//Tip amount
    			memset(szStr, ' ', d_LINE_SIZE);
    			sprintf(szStr,"%s    %s","TIP",strCST.szCurSymbol);
    			memset (baTemp, 0x00, sizeof(baTemp));
    			memset (szTemp4, 0x00, sizeof(szTemp4));
    			wub_hex_2_str(srTransRec.szTipAmount, szTemp4, AMT_BCD_SIZE);
    
    			DebugAddSTR("TIP:",szTemp4,12);
                if (TRUE ==strHDT.fTipAllowFlag)
                {
                    if(atol(szTemp4) > 0)
                    {
						//Base amount
						vdPrintFormattedAmount("AMOUNT:", szTemp1, 24);

                        vdPrintFormattedAmount("TIP   :", szTemp4, 24);
                        
                        //Total amount
                        vdPrintFormattedAmount("TOTAL :", szTemp, 24);
                    }
                    else
                    {
						if(srTransRec.byTransType == VOID)
						{
                            vdPrintFormattedAmount("TOTAL :", szTemp1, 24);							
						}
						else
						{
							//Base amount
							vdPrintFormattedAmount("AMOUNT:", szTemp1, 24);

                            memset(szStr, ' ', d_LINE_SIZE);
                            sprintf(szStr, "TIP   :%s", strCST.szCurSymbol);
                            
                            inPrint(szStr);
							inPrintLeftRight(" ","______________",24);
                            
                            //Total amount
                            memset(szStr, ' ', d_LINE_SIZE);
                            sprintf(szStr, "TOTAL :%s", strCST.szCurSymbol);
                            inPrint(szStr);
							inPrintLeftRight(" ","______________",24);
						}
                    }
                }
				else
    		        vdPrintFormattedAmount("TOTAL :", szTemp1, 24);
			}
		}
		else if (srTransRec.byTransType == SALE_TIP)
		{
			memset(szTemp3, ' ', d_LINE_SIZE);
			wub_hex_2_str(srTransRec.szTipAmount, szTemp3, AMT_BCD_SIZE);
			vdPrintFormattedAmount("AMOUNT:", szTemp1, 24);

			vdPrintFormattedAmount("TIPS  :", szTemp3, 24);
			vdPrintFormattedAmount("TOTAL :", szTemp, 24);
		}
		else
		{
            vdPrintFormattedAmount("TOTAL :", szTemp, 24);
		}

        //vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
	
	return d_OK;	
	
}
USHORT ushCTOS_PrintAgreement()
{
    if(strCDT.inType != DEBIT_CARD)
    {
        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
        vdPrintCenter("I AGREE TO PAY THE ABOVE TOTAL AMOUNT");
        vdPrintCenter("ACCORDING TO THE CARD ISSUER AGREEMENT");
		vdCTOS_PrinterFline(1);
        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
    }
}

USHORT ushCTOS_PrintFooter(int page)
{		
    BYTE   EMVtagVal[64];
    USHORT EMVtagLen; 
	
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
	if(page == CUSTOMER_COPY_RECEIPT)
	{
		memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
		EMVtagLen = 0;
		
		DebugAddINT("ushCTOS_PrintFooter,mode",srTransRec.byEntryMode);  
		if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
		{
		    EMVtagLen = 3;
            memcpy(EMVtagVal, srTransRec.stEMVinfo.T9F34, EMVtagLen);
			if((EMVtagVal[2]& 0x0F) == 2)
            {   
                vdCTOS_PrinterFline(2);
                vdPrintCenter("*****NO SIGNATURE REQUIRED*****");
                if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
                    vdPrintCenter("(PIN VERIFY SUCCESS)");
            }
            else
            {
                //CTOS_PrinterFline(d_LINE_DOT * 3);
            }
		}
        else
        {
            //CTOS_PrinterFline(d_LINE_DOT * 3);
        }

		ushCTOS_ePadPrintSignature();
		
		//CTOS_PrinterPutString("SIGN:_______________________________________");
		//printCardHolderName();
		vdCTOS_PrinterFline(1);
		
		ushCTOS_PrintAgreement();

		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
		if(strlen(strMMT[0].szRctFoot1) > 0)
			vdPrintCenter(strMMT[0].szRctFoot1);
		if(strlen(strMMT[0].szRctFoot2) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot2);
		if(strlen(strMMT[0].szRctFoot3) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot3);

		if(strHDT.fFooterLogo == TRUE)	
		{
			if(strlen(strHDT.szFooterLogoName))
				vdCTOSS_PrinterBMPPic(0, 0, strHDT.szFooterLogoName);
			else	
                vdCTOSS_PrinterBMPPic(0, 0, "footer.bmp");
		}
		
		vdCTOS_PrinterFline(1); 
		vdPrintCenter("***** CUSTOMER COPY *****");
		vdCTOS_PrinterFline(2);
	}
	else if(page == MERCHANT_COPY_RECEIPT)
	{
		memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
		EMVtagLen = 0;
		
		DebugAddINT("ushCTOS_PrintFooter,mode",srTransRec.byEntryMode);  
		if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
		{
			EMVtagLen = 3;
            memcpy(EMVtagVal, srTransRec.stEMVinfo.T9F34, EMVtagLen);
			if((EMVtagVal[2]& 0x0F) == 2)
            {         
                vdCTOS_PrinterFline(2);
                vdPrintCenter("*****NO SIGNATURE REQUIRED*****");
                if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
                    vdPrintCenter("(PIN VERIFY SUCCESS)");
            }
            else
            {				
				if(strCDT.inType != DEBIT_CARD)
				{
					vdCTOS_PrinterFline(2);
					inPrint("SIGN:_______________________________________");
				}
            }
		}
        else
        {
			if(strCDT.inType != DEBIT_CARD)
			{
				vdCTOS_PrinterFline(2);
				inPrint("SIGN:_______________________________________");
			}
        }

        ushCTOS_ePadPrintSignature();
        
        //if(strCDT.inType != DEBIT_CARD)
            //CTOS_PrinterPutString("SIGN:_______________________________________");
		//printCardHolderName();
		vdCTOS_PrinterFline(1);
		
		ushCTOS_PrintAgreement();
        
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
        if(strlen(strMMT[0].szRctFoot1) > 0)
			vdPrintCenter(strMMT[0].szRctFoot1);
		if(strlen(strMMT[0].szRctFoot2) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot2);
		if(strlen(strMMT[0].szRctFoot3) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot3);
		
        if(strHDT.fFooterLogo == TRUE)	
        {
        if(strlen(strHDT.szFooterLogoName))
            vdCTOSS_PrinterBMPPic(0, 0, strHDT.szFooterLogoName);
        else    
            vdCTOSS_PrinterBMPPic(0, 0, "footer.bmp");
        }
		vdCTOS_PrinterFline(1); 
		vdPrintCenter("***** MERCHANT COPY *****");
		vdCTOS_PrinterFline(2);
	}
	else if(page == BANK_COPY_RECEIPT)
	{
		memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
		EMVtagLen = 0;
		
		DebugAddINT("ushCTOS_PrintFooter,mode",srTransRec.byEntryMode);  
		if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
		{
			EMVtagLen = 3;
            memcpy(EMVtagVal, srTransRec.stEMVinfo.T9F34, EMVtagLen);
			if((EMVtagVal[2]& 0x0F) == 2)
            {         
                vdCTOS_PrinterFline(2);
                vdPrintCenter("*****NO SIGNATURE REQUIRED*****");
                if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
                    vdPrintCenter("(PIN VERIFY SUCCESS)");
            }
            else
            {				
				if(strCDT.inType != DEBIT_CARD)
				{
                    vdCTOS_PrinterFline(2);
                    inPrint("SIGN:_______________________________________");
				}
            }
		}
        else
        {
			if(strCDT.inType != DEBIT_CARD)
			{
               vdCTOS_PrinterFline(2);
               inPrint("SIGN:_______________________________________");
			}
        }

        ushCTOS_ePadPrintSignature();
        
        //if(strCDT.inType != DEBIT_CARD)
            //CTOS_PrinterPutString("SIGN:_______________________________________");
		//printCardHolderName();
		vdCTOS_PrinterFline(1);
		
		ushCTOS_PrintAgreement();
		
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
        if(strlen(strMMT[0].szRctFoot1) > 0)
			vdPrintCenter(strMMT[0].szRctFoot1);
		if(strlen(strMMT[0].szRctFoot2) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot2);
		if(strlen(strMMT[0].szRctFoot3) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot3);
		
        if(strHDT.fFooterLogo == TRUE)	
        {
        if(strlen(strHDT.szFooterLogoName))
            vdCTOSS_PrinterBMPPic(0, 0, strHDT.szFooterLogoName);
        else    
            vdCTOSS_PrinterBMPPic(0, 0, "footer.bmp");
        }
		vdCTOS_PrinterFline(1); 
		vdPrintCenter("***** BANK COPY *****");
		vdCTOS_PrinterFline(2);
	}

	
	vdLineFeed((strHDT.fFooterLogo == TRUE)?TRUE:FALSE); // added -- sidumili

return d_OK;	

}



USHORT ushCTOS_printAll(int page)
{	
	int inResult, print_counter = 0;
    int inKey = d_NO;
    BYTE szLogoPath[256 + 1];
    BYTE szMsg[256];
    BYTE szPrintMsg[512];
	BYTE szTitle[20+1];
	BYTE szDisMsg[100];
	BYTE szCopy[256];

	vdDebug_LogPrintf("--ushCTOS_printAll--");
	//vdDebug_LogPrintf("fERMTransaction[%d]", fERMTransaction);
	
    if( printCheckPaper()==-1)
        return -1;

	//tine/sidumili: android - set ui thread to display nothing
	//DisplayStatusLine(" ");
	
    inCTOS_DisplayPrintBMP();

    
	#if 0 /*albert (ecr tct update fail) - save last invoice number, moved to vdSaveLastInvoiceNo*/
    if(VS_FALSE == fRePrintFlag)
	    memcpy(strTCT.szLastInvoiceNo,srTransRec.szInvoiceNo,INVOICE_BCD_SIZE);
	
	if((inResult = inTCTSave(1)) != ST_SUCCESS)
    {
		vdDisplayErrorMsg(1, 8, "Update TCT fail");
    }
	#endif
    
	
    //vdCTOSS_PrinterStart(100);
    //CTOS_PrinterSetHeatLevel(4);  

	vdDebug_LogPrintf("ushCTOS_printAll-2");
	
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
    

	#if 0
		ushCTOS_PrintHeader(page);

		if(srTransRec.fLoyalty==TRUE && (srTransRec.byTransType == LOY_BAL_INQ || srTransRec.byTransType == LOY_REDEEM_5050
		|| srTransRec.byTransType == LOY_REDEEM_VARIABLE || srTransRec.byTransType == VOID))
		{
	 	    ushCTOS_PrintLoyaltyBody(page);	
		    ushCTOS_PrintLoyaltyFooter(page);
		}
		else
		{	
	  	    ushCTOS_PrintBody(page);	
		    ushCTOS_PrintFooter(page);
		}
	#endif


	
	inHDTRead(srTransRec.HDTid);

	if (srTransRec.HDTid == PHQR_HOST_INDEX){

		iXMLPrintWechatHeader();
	
		vdDebug_LogPrintf("ushCTOS_printAll-3");
	
		iPrintPHQRBody(page);	
		iXMLPrintWechatFooter(page);	


	}else{
		inMCCWechatRead(1);
		
		iXMLPrintWechatHeader();
		
		vdDebug_LogPrintf("ushCTOS_printAll-3");
		
		//iXMLPrintWechatCopy(page);
		iXMLPrintWechatBody(page);	
		iXMLPrintWechatFooter(page);	
	}

	memset(szLogoPath, 0x00, sizeof(szLogoPath));
    memset(szMsg, 0x00, sizeof(szMsg));
	memset(szCopy, 0x00, sizeof(szCopy));
    memset(szPrintMsg, 0x00, sizeof(szPrintMsg));
    sprintf(szLogoPath, "%s%s", LOGO_PATH, strHDT.szHeaderLogoName);

    if (page == BANK_COPY_RECEIPT)
    	{
    		strcpy(szMsg, "Print Bank Copy?");
			strcpy(szCopy, "Bank Copy");
    	}        
    else if (page == CUSTOMER_COPY_RECEIPT)
    	{
    		strcpy(szMsg, "Print Customer Copy?");
			strcpy(szCopy, "Customer Copy");
    	}        
    else if (page == MERCHANT_COPY_RECEIPT)
    	{	
    		strcpy(szMsg, "Print Merchant Copy?");
			strcpy(szCopy, "Merchant Copy");
    	}

	// sidumili: Capture receipt bmp
	vdDebug_LogPrintf("Capturing erm report...");
	vdDeleteBMP();
	vdCTOSS_PrinterEnd_CConvert2BMP("/data/data/pub/Print_BMP.bmp");
	CTOS_KBDBufFlush();
	inCTOSS_ERM_Form_Receipt(0);

	if (strTCT.feReceiptEnable == 1) 
	{

	    strcpy(szPrintMsg, szLogoPath);
	    strcat(szPrintMsg, "|");
	    strcat(szPrintMsg, szMsg);

	    if (strTCT.byTerminalModel == 1) {
	        //vdCTOSS_PrinterStart(200);
	        //ushCTOS_printAll(BANK_COPY_RECEIPT);
	        
	        if (printcopies_cntr == 0) {
	            //inKey = PrintFirstReceiptUI(szLogoPath);
	            memset(szTitle, 0x00, sizeof(szTitle));
				szGetTransTitle(srTransRec.byTransType, szTitle);
				strcpy(szDisMsg, szTitle);
				strcat(szDisMsg, "|");
				strcat(szDisMsg, "PRINTING...");
				usCTOSS_LCDDisplay(szDisMsg);
				//vdDisplayMessageBox(1, 8, "PRINTING", szCopy, MSG_PLS_WAIT, MSG_TYPE_PRINT);

	            vdCTOSS_PrinterBMPPicEx(0, 0, szLogoPath);
	            vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
	            vdCTOSS_PrinterEnd();
	            return d_OK;
	        } else {
	        	inKey = PrintReceiptUI(szPrintMsg);
				
				if (inKey == d_OK || inKey == 0xFF) {
	                vdDebug_LogPrintf("vdCTOSS_PrinterEnd() - START");
					memset(szTitle, 0x00, sizeof(szTitle));
					szGetTransTitle(srTransRec.byTransType, szTitle);
					strcpy(szDisMsg, szTitle);
					strcat(szDisMsg, "|");
					strcat(szDisMsg, "PRINTING...");
					usCTOSS_LCDDisplay(szDisMsg);
					//vdDisplayMessageBox(1, 8, "PRINTING", szCopy, MSG_PLS_WAIT, MSG_TYPE_PRINT);
				
	                vdCTOSS_PrinterBMPPicEx(0, 0, szLogoPath);
	                vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
	                vdCTOSS_PrinterEnd();
	                return d_OK;
	                //print_counter = 1;
	                //goto INPRINTRECEIPT;
	            }
	            if (inKey == d_USER_CANCEL)
	            {
					return d_OK;		
	            }
	                		
	        }            
	                      
	    } else {
	        vdDebug_LogPrintf("strTCT.byTerminalModel = [%d]", strTCT.byTerminalModel);
	        inKey = EliteReceiptUI(szLogoPath);
	    }
	} 
	else 
	{
        //sprintf(szLogoPath, "%s%s", LOCAL_PATH, strHDT.szHeaderLogoName);  
        memset(szTitle, 0x00, sizeof(szTitle));
		szGetTransTitle(srTransRec.byTransType, szTitle);
		strcpy(szDisMsg, szTitle);
		strcat(szDisMsg, "|");
		strcat(szDisMsg, "PRINTING...");
		usCTOSS_LCDDisplay(szDisMsg);
		//vdDisplayMessageBox(1, 8, "PRINTING", szCopy, MSG_PLS_WAIT, MSG_TYPE_PRINT);
					
        vdCTOSS_PrinterBMPPicEx(0, 0, szLogoPath);
		vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
        vdCTOSS_PrinterEnd();

        return d_OK;

    }
		
    return d_OK;
 
}

int inCTOS_rePrintReceipt(void)
{
	int   inResult;
	BYTE  szInvNoAsc[6+1];
	BYTE  szInvNoBcd[3];	
	BOOL   needSecond = TRUE;	
	BYTE   key;

	
	CTOS_PrinterSetHeatLevel(4);
	if( printCheckPaper()==-1)
		return d_NO;
	
	inResult = inCTOS_GeneralGetInvoice();
	if(d_OK != inResult)
	{
		return inResult;
	}

	if (inMultiAP_CheckMainAPStatus() == d_OK)
	{
		inResult = inCTOS_MultiAPBatchSearch(d_IPC_CMD_REPRINT_ANY);
		if(d_OK != inResult)
			return inResult;
	}
	else
	{
		if (inMultiAP_CheckSubAPStatus() == d_OK)
		{
			inResult = inCTOS_MultiAPGetVoid();
			if(d_OK != inResult)
				return inResult;
		}		
		inResult = inCTOS_BatchSearch();
		if(d_OK != inResult)
			return inResult;
	}

	if(d_OK != inResult)
	{
		return inResult;
	}

    inIITRead(srTransRec.IITid);
    inCDTRead(srTransRec.CDTid);
    inHDTRead(srTransRec.HDTid);

	// Revise for printer start mp200 -- sidumili
	if (!fPrinterStart)
		vdCTOSS_PrinterStart(100);

	vdCTOS_PrintReceiptCopies(strTCT.fPrintBankCopy, TRUE, strTCT.fPrintMerchCopy);

	return d_OK;
}

int vdCTOS_PrintSummaryReport()
{
	ACCUM_REC srAccumRec;
	unsigned char chkey;
	short shHostIndex;
	int inResult,inRet;
	int inTranCardType;
	int inReportType;
	int inIITNum , i;
	char szStr[d_LINE_SIZE + 1];
    BYTE baTemp[PAPER_X_SIZE * 64];

	int inPADSize,x;
	char szSPACE[40];
	char szBuff[46];
	char szBuff2[46];
	int inIITCount = 0;

	vdDebug_LogPrintf("--vdCTOS_PrintSummaryReport--");
	
	vdSetGlobalVariable();

	strGBLVar.fGBLvConfirmPrint = FALSE; // sidumili: disable confirmation instead print directly
	
	//for MP200 no need print
	if ((strTCT.byTerminalType == 5) || (strTCT.byTerminalType == 6))
    	return (d_OK);
	
	if( printCheckPaper()==-1)
		return d_NO;

	//inRet = inCTOS_TEMPCheckAndSelectMutipleMID();
		
	//by host and merchant
	shHostIndex = inCTOS_SelectHostSetting();
	if (shHostIndex == -1)
		return d_NO;
	strHDT.inHostIndex = shHostIndex;

	
    
	DebugAddINT("summary host Index",shHostIndex);
	inCSTRead(strHDT.inCurrencyIdx);

	if (inMultiAP_CheckMainAPStatus() == d_OK)
	{
		inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_SUMMARY_REPORT);
		if(d_OK != inRet)
			return d_NO;
	}
	else
	{
		if (inMultiAP_CheckSubAPStatus() == d_OK)
		{
			inRet = inCTOS_MultiAPGetData();
			if(d_OK != inRet)
				return d_NO;

			inRet = inCTOS_MultiAPReloadHost();
			if(d_OK != inRet)
				return d_NO;
		}
	}

	inCTOS_CheckAndSelectMutipleMID();
	if(d_OK != inRet)
		return inRet;

	vdDebug_LogPrintf("HDTid[%d].MITid[%d]",srTransRec.HDTid,srTransRec.MITid);
	inMMTReadRecord(srTransRec.HDTid,srTransRec.MITid);

	inResult = inCTOS_ChkBatchEmpty();
	if(d_OK != inResult)
	{
		return d_NO;
	}

    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    if((inResult = inCTOS_ReadAccumTotal(&srAccumRec)) == ST_ERROR)
    {
        vdMyEZLib_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
        return d_NO;
    }

	//vdSetPrintThreadStatus(1); /*BDO: Display printing image during report -- sidumili*/
	//vdThreadDisplayPrintBmp(0,0, "Print1.bmp", "Print2.bmp", "Print3.bmp", NULL, NULL);
	//vdPrintReportDisplayBMP();
	inCTOS_DisplayPrintBMP();
	
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);

    
    vdCTOSS_PrinterStart(100);
	CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);

	vdPrintHeaderLogo();
	ushCTOS_PrintHeader(0);	
    
    //vdPrintTitleCenter("SUMMARY REPORT");
    //vdCTOS_PrinterFline(1);
	//CTOS_PrinterFline(d_LINE_DOT * 1);
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);

    //printHostLabel();

	if (srTransRec.HDTid == WECHAT_HOST_INDEX || srTransRec.HDTid == ALIPAY_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX || srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == UPI_HOST_INDEX || srTransRec.HDTid == PHQR_HOST_INDEX)
	{
		vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		vdPrintCenter("SUMMARY REPORT");
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);
		
		printHostLabel();
		iXMLPrintWechatTIDMID();
		iXMLPrintWechatDateTime(FALSE, FALSE, 0);
		printDividingLine(DIVIDING_LINE);

		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);
	
		vdDebug_LogPrintf("szMID[%s]", srTransRec.szMID);
		vdDebug_LogPrintf("szTID[%s]", srTransRec.szTID);
		vdDebug_LogPrintf("szBatchNo[%02X%02X%02X]", srTransRec.szBatchNo[0], srTransRec.szBatchNo[1], srTransRec.szBatchNo[2]);

		// Print Summary	
		inIITCount = inIITNumRecord() + 1;
		for(inTranCardType = 0; inTranCardType < 1 ;inTranCardType ++)
		{
			BOOL fPrintTotal=FALSE;
			inReportType = PRINT_CARD_TOTAL;
			inTranCardType = inTranCardType; 
			
			if(inReportType == PRINT_CARD_TOTAL)
			{
				for(i= 0; i < inIITCount; i ++ )
				{
					vdDebug_LogPrintf("--Count[%d]", i);
					if((srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usSaleCount == 0)
					&&(srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usOffSaleCount == 0)
					&&(srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usRefundCount == 0)
					&&(srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usVoidSaleCount == 0)			
					&&(srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usReloadCCCount == 0))
						continue;
					
					
					vdDebug_LogPrintf("Count[%d]", i); 
					inIITRead(i);				
					vdCTOS_PrintAccumeByHostAndCard (inReportType, 
													srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usSaleCount - srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usOffSaleCount,   
													(srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].ulSaleTotalAmount) - (srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].ulOffSaleTotalAmount),												
													srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usRefundCount, (srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].ulRefundTotalAmount), 												
													srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usVoidSaleCount, (srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].ulVoidSaleTotalAmount), 
													srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usOffSaleCount, (srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].ulOffSaleTotalAmount),
													srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].usReloadCCCount, (srAccumRec.stBankTotal[inTranCardType].stCardTotal[i].ulReloadCCTotalAmount));	
					//fPrintTotal=TRUE; // sidumili: Remove Grand Total priting
				}
				
				//after print issuer total, then print host toal
				if(fPrintTotal == TRUE)
				{
					memset(szStr, ' ', d_LINE_SIZE);
					memset (baTemp, 0x00, sizeof(baTemp));
					strcpy(szStr,"GRAND TOTAL");
					inPrint(szStr);
					
					inReportType = DETAIL_REPORT_TOTAL;
					vdCTOS_PrintAccumeByHostAndCard (inReportType, 
														srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount - srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usOffSaleCount,   
														(srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulSaleTotalAmount) - (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulOffSaleTotalAmount),													
														srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usRefundCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulRefundTotalAmount), 													
														srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usVoidSaleCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulVoidSaleTotalAmount), 
														srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usOffSaleCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulOffSaleTotalAmount),
														srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usReloadCCCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulReloadCCTotalAmount));		
				}				
				
			}
			else
			{
			
				memset(szStr, ' ', d_LINE_SIZE);
				memset (baTemp, 0x00, sizeof(baTemp));
				strcpy(szStr,srTransRec.szHostLabel);
				inPrint(szStr);
				vdCTOS_PrintAccumeByHostAndCard (inReportType, 
													srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usSaleCount - srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usOffSaleCount,   
													(srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulSaleTotalAmount) - (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulOffSaleTotalAmount),												
													srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usRefundCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulRefundTotalAmount), 												
													srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usVoidSaleCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulVoidSaleTotalAmount), 
													srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usOffSaleCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulOffSaleTotalAmount),
													srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.usReloadCCCount, (srAccumRec.stBankTotal[inTranCardType].stHOSTTotal.ulReloadCCTotalAmount));		
			}
		}
			
		vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		vdPrintCenter("END OF REPORT");	
		vdCTOS_PrinterFline(2);
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
		vdLineFeed(5);
	}
	    
	//print space one line
	vdLineFeed(FALSE);
	
	//vdCTOSS_PrinterEnd(); // Commented to support print confirmation -- sidumili

	// Capture report -- sidumili
	vdDebug_LogPrintf("Capturing erm report...");
	vdDeleteBMP();
	vdCTOSS_PrinterEnd_CConvert2BMP("/data/data/pub/Print_BMP.bmp");
	CTOS_KBDBufFlush();
	inCTOSS_ERM_Form_Receipt(0);
	
	if (strGBLVar.fGBLvConfirmPrint)
	{
		// Print confirmation -- sidumili
		if (inPrintConfirmation("SUMMARY REPORT", FALSE) == d_OK)
		{
			vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
			vdCTOSS_PrinterEnd();
		}
		else
		{			
			vdCTOSS_PrinterEnd();
		}
	}
	else
	{
		//usCTOSS_LCDDisplay("SUMMARY REPORT|PRINTING...");
		vdDisplayMessageBox(1, 8, "", "PRINTING...", "", MSG_TYPE_PRINT);
		vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
		vdCTOSS_PrinterEnd();
	}

	
	return d_OK;	
}
void vdCTOS_PrintDetailReport()
{
	unsigned char chkey;
	short shHostIndex;
	int inResult,i,inCount,inRet;
	int inTranCardType;
	int inReportType;
	int inBatchNumOfRecord;
	int *pinTransDataid;
	ACCUM_REC srAccumRec;
	char szStr[d_LINE_SIZE + 1];
    BYTE baTemp[PAPER_X_SIZE * 64];

	vdDebug_LogPrintf("--vdCTOS_PrintDetailReport--");
	
	//vdSetGlobalVariable();

	strGBLVar.fGBLvConfirmPrint = TRUE;
	
	if( printCheckPaper()==-1)
		return;
		
	//by host and merchant
	shHostIndex = inCTOS_SelectHostSetting();
	if (shHostIndex == -1)
		return;

	strHDT.inHostIndex = shHostIndex;
	DebugAddINT("detail host Index",shHostIndex);

	if (inMultiAP_CheckMainAPStatus() == d_OK)
	{
		inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_DETAIL_REPORT);
		if(d_OK != inRet)
			return ;
	}
	else
	{
		if (inMultiAP_CheckSubAPStatus() == d_OK)
		{
			inRet = inCTOS_MultiAPGetData();
			if(d_OK != inRet)
				return ;

			inRet = inCTOS_MultiAPReloadHost();
			if(d_OK != inRet)
				return ;
		}
	}

	inResult = inCTOS_CheckAndSelectMutipleMID();
	DebugAddINT("detail MITid",srTransRec.MITid);
	if(d_OK != inResult)
		return;

    inResult = inCTOS_ChkBatchEmpty();
    if(d_OK != inResult)
        return;

    inResult = inChkAccumEmpty();
    if(d_OK != inResult)
        return;

	vdDebug_LogPrintf("1. tid=%d mid=%d batchnum=%s", strMMT[0].HDTid,strMMT[0].MITid,strMMT[0].szBatchNo);
	CTOS_Delay(2500);

	vdDebug_LogPrintf("before read batch");



	inBatchNumOfRecord = inBatchNumRecord();

	
	vdDebug_LogPrintf("after  read batch");
	
	DebugAddINT("batch record",inBatchNumOfRecord);
	if(inBatchNumOfRecord <= 0)
	{
		vdDisplayErrorMsg(1, 8, "BATCH EMPTY");
		return;
	}

	//vdCTOSS_PrinterStart(100);
    vdCTOSS_PrinterStart(2000);
    CTOS_PrinterSetHeatLevel(4);  
	
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
    
    inCTOS_DisplayPrintBMP();
	if (srTransRec.HDTid == WECHAT_HOST_INDEX || srTransRec.HDTid == ALIPAY_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX || srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == UPI_HOST_INDEX || srTransRec.HDTid == PHQR_HOST_INDEX)
	{
	    
		vdDebug_LogPrintf("hdtid wechat/alipay");
		vdPrintHeaderLogo();
		ushCTOS_PrintHeader(0);

		vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		vdPrintCenter("DETAIL REPORT");
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);

		printHostLabel();

		iXMLPrintWechatTIDMID();

		iXMLPrintWechatDateTime(FALSE, FALSE, 0);

		printDividingLine(DIVIDING_LINE);

		// Print Detail Report
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);

		vdDebug_LogPrintf("szMID[%s]", srTransRec.szMID);
		vdDebug_LogPrintf("szTID[%s]", srTransRec.szTID);
		vdDebug_LogPrintf("szBatchNo[%02X%02X%02X]", srTransRec.szBatchNo[0], srTransRec.szBatchNo[1], srTransRec.szBatchNo[2]);

		ushCTOS_WechatprintBatchRecordHeader();

		if (!strGBLVar.fGBLvConfirmPrint)
			vdCTOSS_PrinterEnd();
			
		pinTransDataid = (int*)malloc(inBatchNumOfRecord  * sizeof(int));
		inCount = 0;		
		inBatchByMerchandHost(inBatchNumOfRecord, srTransRec.HDTid, srTransRec.MITid, srTransRec.szBatchNo, pinTransDataid);
		for(i=0; i<inBatchNumOfRecord; i++)
		{
			inResult = inDatabase_BatchReadByTransId(&srTransRec, pinTransDataid[inCount]);
		    inCount ++;

			if (!strGBLVar.fGBLvConfirmPrint)
			{
				vdCTOSS_PrinterStart(100);
				CTOS_PrinterSetHeatLevel(4);
			}
			
			ushCTOS_WechatprintBatchRecordFooter(FALSE);	

			if (!strGBLVar.fGBLvConfirmPrint)
				vdCTOSS_PrinterEnd();
		}

		 free(pinTransDataid);

		printDividingLine(DIVIDING_LINE);

		vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		vdPrintCenter("END OF REPORT");
		vdCTOS_PrinterFline(2);
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

		vdCTOS_PrinterFline(4);

		//vdLineFeed(strHDT.fFooterLogo);

	}

    vdLineFeed(FALSE);
	
    //vdCTOSS_PrinterEnd(); // Commented to support print confirmation -- sidumili
	
	// Capture report -- sidumili		
	vdDebug_LogPrintf("Capturing erm report...");
	vdDeleteBMP();
	vdCTOSS_PrinterEnd_CConvert2BMP("/data/data/pub/Print_BMP.bmp");
	CTOS_KBDBufFlush();
	inCTOSS_ERM_Form_Receipt(0);

	/*
	if (strGBLVar.fGBLvConfirmPrint)
	{
		// Print confirmation -- sidumili
		if (inPrintConfirmation("DETAIL REPORT", FALSE) == d_OK)
		{
			vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
			vdCTOSS_PrinterEnd();
		}
		else
		{			
			vdCTOSS_PrinterEnd();
		}
	}
	else
	*/
	{
		vdDisplayMessageBox(1, 8, "", "PRINTING...", "", MSG_TYPE_PRINT);
		vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
		vdCTOSS_PrinterEnd();
	}
	
	return;	
}




int inCTOS_rePrintLastReceipt(void)
{
	int   inResult;
	BYTE  szInvNoAsc[6+1];
	BYTE  szInvNoBcd[3];	
    BOOL   needSecond = TRUE;	
    BYTE   key;

	
	CTOS_PrinterSetHeatLevel(4);
	if( printCheckPaper()==-1)
		return d_NO;
	
	memcpy(srTransRec.szInvoiceNo,strTCT.szLastInvoiceNo,INVOICE_BCD_SIZE);
	DebugAddHEX("last invoice", strTCT.szLastInvoiceNo,3 );

	if (inMultiAP_CheckMainAPStatus() == d_OK)
	{
		inResult = inCTOS_MultiAPBatchSearch(d_IPC_CMD_REPRINT_LAST);
		if(d_OK != inResult)
			return inResult;
	}
	else
	{
		if (inMultiAP_CheckSubAPStatus() == d_OK)
		{
			inResult = inCTOS_MultiAPGetVoid();
			if(d_OK != inResult)
				return inResult;
		}		
		inResult = inCTOS_BatchSearch();
		if(d_OK != inResult)
			return inResult;
	}

    inIITRead(srTransRec.IITid);
    inCDTRead(srTransRec.CDTid);
	inHDTRead(srTransRec.HDTid);

	// Revise for printer start mp200 -- sidumili
	if (!fPrinterStart)
		vdCTOSS_PrinterStart(100);

	vdCTOS_PrintReceiptCopies(strTCT.fPrintBankCopy, TRUE, strTCT.fPrintMerchCopy);

	return d_OK;
}

USHORT ushCTOS_printReceipt(void)
{
    USHORT result;
    BYTE   key;
    BOOL   needSecond = TRUE;
    
    if( printCheckPaper()==-1)
    	return -1;

	// Revise for printer start mp200 -- sidumili
	if (!fPrinterStart)
		vdCTOSS_PrinterStart(100);

	vdCTOS_PrintReceiptCopies(strTCT.fPrintBankCopy, TRUE, strTCT.fPrintMerchCopy);
	
    return (d_OK);
}



USHORT ushCTOS_printErrorReceipt(void)
{
    USHORT result;
    BYTE   key;
    BOOL   needSecond = TRUE;
	
	char szStr[d_LINE_SIZE + 1];
   char szTemp[d_LINE_SIZE + 1];
   char szTemp1[d_LINE_SIZE + 1];
   char szTemp3[d_LINE_SIZE + 1];
   char szTemp4[d_LINE_SIZE + 1];
   char szTemp5[d_LINE_SIZE + 1];
   char szFormatPAN[strlen(srTransRec.szPAN) + 7];//to account for space as in PAN format and to include additional PAN no. 7 has been added -Meena 08/01/2012
   int inFmtPANSize;
   BYTE baTemp[PAPER_X_SIZE * 64];
   CTOS_FONT_ATTRIB stFONT_ATTRIB;
   int num,i,inResult;
   unsigned char tucPrint [24*4+1];    
   BYTE   EMVtagVal[64];
   USHORT EMVtagLen; 
   short spacestring;

    
    if( printCheckPaper()==-1)
    	return -1;

	memcpy(strTCT.szLastInvoiceNo,srTransRec.szInvoiceNo,INVOICE_BCD_SIZE);
	
	if((inResult = inTCTSave(1)) != ST_SUCCESS)
    {
		vdDisplayErrorMsg(1, 8, "Update TCT fail");
    }
	
	DebugAddHEX("LastInvoiceNum", strTCT.szLastInvoiceNo,3);

	inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
    
    CTOS_PrinterSetHeatLevel(4);  
	DebugAddSTR("ushCTOS_printAll","print...",20);
		

	ushCTOS_PrintHeader(d_FIRST_PAGE);

	memset(szTemp1, ' ', d_LINE_SIZE);
	sprintf(szTemp1,"%s",srTransRec.szHostLabel);
	vdPrintCenter(szTemp1);
	
	printDateTime();

    printTIDMID(); 
	
	printBatchInvoiceNO();
	
	szGetTransTitle(srTransRec.byTransType, szStr);     
		vdPrintTitleCenter(szStr);    
    if(srTransRec.byTransType == SETTLE)
    {
        DebugAddSTR("settle","print...",20);
    }
    else
    {
        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
    	memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
    	EMVtagLen = 0;
    	memset(szStr, ' ', d_LINE_SIZE);
    	vdMyEZLib_LogPrintf("CDT index: %d",srTransRec.CDTid);

    	sprintf(szStr, "%s", srTransRec.szCardLable);
    	vdMyEZLib_LogPrintf("Card label: %s",srTransRec.szCardLable);
    	
    	memset (baTemp, 0x00, sizeof(baTemp));
    	CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
    	CTOS_PrinterBufferOutput((BYTE *)baTemp, 4); 

    	cardMasking(srTransRec.szPAN, PRINT_CARD_MASKING_1);
    	strcpy(szTemp4, srTransRec.szPAN);
    	memset (baTemp, 0x00, sizeof(baTemp));
    	CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szTemp4, &stgFONT_ATTRIB);
    	CTOS_PrinterBufferOutput((BYTE *)baTemp, 4);		

        vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
        
    	//Exp date and Entry mode
    	memset(szStr, ' ', d_LINE_SIZE);
    	memset(szTemp, 0, sizeof(szTemp));
    	memset(szTemp1, 0, sizeof(szTemp1));
    	memset(szTemp4, 0, sizeof(szTemp4));
    	memset(szTemp5, 0, sizeof(szTemp5));
    	wub_hex_2_str(srTransRec.szExpireDate, szTemp,EXPIRY_DATE_BCD_SIZE);
    	DebugAddSTR("EXP",szTemp,12);  
    	
    	for (i =0; i<4;i++)
    		szTemp[i] = '*';
    	memcpy(szTemp4,&szTemp[0],2);
    	memcpy(szTemp5,&szTemp[2],2);

    	if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
    		memcpy(szTemp1,"Chip",4);
    	else if(srTransRec.byEntryMode==CARD_ENTRY_MANUAL)
    		memcpy(szTemp1,"Manual",6);
    	else if(srTransRec.byEntryMode==CARD_ENTRY_MSR)
    		memcpy(szTemp1,"MSR",3);
    	else if(srTransRec.byEntryMode==CARD_ENTRY_FALLBACK)
    		memcpy(szTemp1,"Fallback",8);
    	
    	memset (baTemp, 0x00, sizeof(baTemp));
    	sprintf(szTemp,"%s%s/%s          %s%s","EXP: ",szTemp4,szTemp5,"ENT:",szTemp1);
    	
    	DebugAddSTR("ENT:",baTemp,12);  
    	CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szTemp, &stgFONT_ATTRIB);
    	CTOS_PrinterBufferOutput((BYTE *)baTemp, 4);

    		
     	memset (baTemp, 0x00, sizeof(baTemp));
    	stFONT_ATTRIB.FontSize = 0x1010;
        stFONT_ATTRIB.X_Zoom = DOUBLE_SIZE;       // The width magnifies X_Zoom diameters
        stFONT_ATTRIB.Y_Zoom = DOUBLE_SIZE;       // The height magnifies Y_Zoom diameters

        stFONT_ATTRIB.X_Space = 0;      // The width of the space between the font with next font

    }

	memset(szStr, ' ', d_LINE_SIZE);
	memset(szTemp, ' ', d_LINE_SIZE);
	memset(szTemp1, ' ', d_LINE_SIZE);
	sprintf(szStr, "%s", "TRANS NOT SUC.");
	
	memset (baTemp, 0x00, sizeof(baTemp));
	CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stFONT_ATTRIB);
	CTOS_PrinterBufferOutput((BYTE *)baTemp, 4); 

	memset(szStr, ' ', d_LINE_SIZE);
	memset(szTemp, ' ', d_LINE_SIZE);
	memset(szTemp1, ' ', d_LINE_SIZE);
	
	sprintf(szStr, "%s", "PLS TRY AGAIN");
	
	memset (baTemp, 0x00, sizeof(baTemp));
	CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stFONT_ATTRIB);
	CTOS_PrinterBufferOutput((BYTE *)baTemp, 4); 

	vdLineFeed(FALSE);
	
	vdCTOSS_PrinterEnd();
	//inSetTextMode();
    return (d_OK);
}


int inCTOS_REPRINT_ANY()
{
    char szErrMsg[30+1];
    int   inResult;
	
	fRePrintFlag = TRUE;
    vdCTOS_SetTransType(REPRINT_ANY);
	inResult = inCTOS_rePrintReceipt();

    memset(szErrMsg,0x00,sizeof(szErrMsg));
    if (inGetErrorMessage(szErrMsg) > 0)
    {
        vdDisplayErrorMsg(1, 8, szErrMsg);
        vdSetErrorMessage("");
    }
    
    memset( &srTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
    CTOS_KBDBufFlush();
	fRePrintFlag = FALSE;
	return inResult;
}

int inCTOS_REPRINT_LAST()
{
    char szErrMsg[30+1];
	int   inResult;
    
	fRePrintFlag = TRUE;
	inResult = inCTOS_rePrintLastReceipt();	

    memset(szErrMsg,0x00,sizeof(szErrMsg));
    if (inGetErrorMessage(szErrMsg) > 0)
    {
        vdDisplayErrorMsg(1, 8, szErrMsg);
        vdSetErrorMessage("");
    }
    
    memset( &srTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
    CTOS_KBDBufFlush();
	fRePrintFlag = FALSE;
	
	return inResult;
}

int inCTOS_PRINTF_SUMMARY()
{
    char szErrMsg[30+1];
    
	vdCTOS_PrintSummaryReport();	

    memset(szErrMsg,0x00,sizeof(szErrMsg));
    if (inGetErrorMessage(szErrMsg) > 0)
    {
        vdDisplayErrorMsg(1, 8, szErrMsg);
        vdSetErrorMessage("");
    }
    
    memset( &srTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
    CTOS_KBDBufFlush();
    
	return d_OK;
	
}

int inCTOS_PRINTF_DETAIL()
{
    char szErrMsg[30+1];
    
	vdCTOS_PrintDetailReport();	

    memset(szErrMsg,0x00,sizeof(szErrMsg));
    if (inGetErrorMessage(szErrMsg) > 0)
    {
        vdDisplayErrorMsg(1, 8, szErrMsg);
        vdSetErrorMessage("");
    }
    
    memset( &srTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
    CTOS_KBDBufFlush();
    
	return d_OK;
	
}

int inCTOS_REPRINTF_LAST_SETTLEMENT()
{
    char szErrMsg[30+1];
	
	ushCTOS_ReprintLastSettleReport();
    
    memset(szErrMsg,0x00,sizeof(szErrMsg));
    if (inGetErrorMessage(szErrMsg) > 0)
    {
		//CTOS_LCDTClearDisplay();
        vdDisplayErrorMsg(1, 8, szErrMsg);
        vdSetErrorMessage("");
    }
    memset( &srTransRec, 0x00, sizeof(TRANS_DATA_TABLE));
	fRePrintFlag = FALSE;
    CTOS_KBDBufFlush();

    return d_OK;
}

void vdCTOS_PrintAccumeByHostAndCard (int inReportType,
	USHORT usSaleCount, 
	double ulSaleTotalAmount,    
	USHORT usRefundCount,
	double  ulRefundTotalAmount,    
	USHORT usVoidSaleCount,
	double  ulVoidSaleTotalAmount,
	USHORT usOffSaleCount,
	double  ulOffSaleTotalAmount,
	//10101
	USHORT usReloadCount, 
	double  ulReloadTotalAmount)
	//10101
{
	char szStr[d_LINE_SIZE + 3] = {0};
	char szTemp[d_LINE_SIZE + 3] = {0}, 
	szTemp1[d_LINE_SIZE + 3] = {0}, 
	szTemp2[d_LINE_SIZE + 3] = {0};
	USHORT usTotalCount = 0;
	double  ulTotalAmount = 0.00;
	BYTE baTemp[PAPER_X_SIZE * 64] = {0};

	char szBuff[47] = {0};
	int inPADSize = 0,
	x = 0;
	char szSPACE[40] = {0};

	CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);  
	memset (baTemp, 0x00, sizeof(baTemp));	
	
	//0720
	if((inReportType == DETAIL_REPORT_TOTAL) || (inReportType == DETAIL_REPORT_GRANDTOTAL)){

		if (srTransRec.HDTid == WECHAT_HOST_INDEX || srTransRec.HDTid == ALIPAY_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX || srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == UPI_HOST_INDEX || srTransRec.HDTid == PHQR_HOST_INDEX)
		{
			usTotalCount = usSaleCount + usRefundCount + usVoidSaleCount;
			memset(baTemp, 0x00, sizeof(baTemp));
			
			ulTotalAmount = (ulSaleTotalAmount) - ulRefundTotalAmount;
			memset(szTemp1, 0x00, sizeof(szTemp1)); 
			memset(szTemp2, 0x00, sizeof(szTemp2));				
			sprintf(szTemp1,"%.0f", ulTotalAmount); 	
			vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);// patrick add code 20141216		
			sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
			sprintf(szStr,"  TOTAL    %04d",usSaleCount);
			inPrintLeftRight2(szStr,szBuff,41);

			printDividingLine(DIVIDING_LINE_DOUBLE);
					
			return;
		}
		
		usTotalCount = usSaleCount + usOffSaleCount + usRefundCount + usReloadCount;
		memset(baTemp, 0x00, sizeof(baTemp));
			
		ulTotalAmount = (ulSaleTotalAmount+ulOffSaleTotalAmount+ulReloadTotalAmount) - ulRefundTotalAmount;
		memset(szTemp1, 0x00, sizeof(szTemp1)); 
		memset(szTemp2, 0x00, sizeof(szTemp2));
			
		sprintf(szTemp1,"%.0f", ulTotalAmount); 	
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);// patrick add code 20141216		
		sprintf(szTemp,"%s%s",strCST.szCurSymbol,szTemp2);

		memset(szTemp1, 0x00, sizeof(szTemp1));
		szStr[47]=0x00;
	
		sprintf(szTemp1,"%d",usTotalCount);
		memset(szStr, 0x20, sizeof(szStr));
				
		if (inReportType == DETAIL_REPORT_TOTAL)
			memcpy(&szStr[0],strIIT.szIssuerLabel, strlen(strIIT.szIssuerLabel));
		else
			memcpy(&szStr[0],"TOTAL", 5);
				
		memcpy(&szStr[18],szTemp1, strlen(szTemp1));
		//memcpy(&szStr[24],szTemp, strlen(szTemp));
		memcpy(&szStr[42-strlen(szTemp)],szTemp, strlen(szTemp));

		memset(baTemp, 0x00, sizeof(baTemp));
		//CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
		//CTOS_PrinterBufferOutput((BYTE *)baTemp, 3);
        inPrint(szStr);
		
	    return;
	}
	//0720

	//types
	/*		memset(szStr, ' ', d_LINE_SIZE);
	strcpy(szStr,"TYPES  COUNT  CUR            AMT");
	memset (baTemp, 0x00, sizeof(baTemp));
	CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
	CTOS_PrinterBufferOutput((BYTE *)baTemp, 3);
	CTOS_PrinterFline(d_LINE_DOT * 1);
	*/		

	if (srTransRec.HDTid == WECHAT_HOST_INDEX || srTransRec.HDTid == ALIPAY_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX || srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == UPI_HOST_INDEX)
	{		
		memset(szStr, 0x00, d_LINE_SIZE);
		strcpy(szStr,"TYPES      COUNT                      AMT");
		inPrint(szStr);
		printDividingLine(DIVIDING_LINE);
		
		//sale
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulSaleTotalAmount+ulOffSaleTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  SALE     %04d",usSaleCount+usOffSaleCount+usVoidSaleCount);
		inPrintLeftRight2(szStr,szBuff,41);

		//refund
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulRefundTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  REFUND   %04d",usRefundCount);
		inPrintLeftRight2(szStr,szBuff,41);

		//void
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulVoidSaleTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"-%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  VOID     %04d",usVoidSaleCount);
		inPrintLeftRight2(szStr,szBuff,41);

		usTotalCount = usSaleCount + usVoidSaleCount;
	}
	else if (srTransRec.HDTid == PHQR_HOST_INDEX){
		memset(szStr, 0x00, d_LINE_SIZE);
		strcpy(szStr,"TYPES      COUNT                      AMT");
		inPrint(szStr);
		printDividingLine(DIVIDING_LINE);
		
		//sale
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulSaleTotalAmount+ulOffSaleTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  SALE     %04d",usSaleCount+usOffSaleCount+usVoidSaleCount);
		inPrintLeftRight2(szStr,szBuff,41);
		
		usTotalCount = usSaleCount;

	}
	else
	{
		//sale
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulSaleTotalAmount+ulOffSaleTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  SALE     %04d",usSaleCount+usOffSaleCount);
		inPrintLeftRight2(szStr,szBuff,41);


		//reload
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));	
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulReloadTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  RELOAD   %04d",usReloadCount);
		inPrintLeftRight2(szStr,szBuff,41);


		//offline sale
		//memset(szStr, ' ', d_LINE_SIZE);
		//sprintf(szStr,"OFFLINE  %3d  %s  %10lu.%02lu", usOffSaleCount, strCST.szCurSymbol,ulOffSaleTotalAmount/100, ulOffSaleTotalAmount%100);
		//memset (baTemp, 0x00, sizeof(baTemp));
		//CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
		//CTOS_PrinterBufferOutput((BYTE *)baTemp, 3);		

		//void
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulVoidSaleTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  VOID     %04d",usVoidSaleCount);
		inPrintLeftRight2(szStr,szBuff,41);


		//refund
		memset(szStr, 0x00, sizeof(szStr));
		memset(szBuff, 0x00, sizeof(szBuff));
		memset(szTemp1, 0x00, sizeof(szTemp1));	
		memset(szTemp2, 0x00, sizeof(szTemp2));
		sprintf(szTemp1,"%.0f", ulRefundTotalAmount);		
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
		sprintf(szBuff,"%s%s",strCST.szCurSymbol,szTemp2);
		sprintf(szStr,"  REF.     %04d",usRefundCount);
		inPrintLeftRight2(szStr,szBuff,41);

		//CTOS_PrinterPutString("----------------------------------------------");

		usTotalCount = usSaleCount + usOffSaleCount + usRefundCount + usReloadCount;
	
	}
	
	memset(szStr, ' ', d_LINE_SIZE);
	memset(szTemp, ' ', d_LINE_SIZE);
	if (inReportType == PRINT_HOST_TOTAL)
	{
		printDividingLine(DIVIDING_LINE);
		
		if(ulRefundTotalAmount > (ulSaleTotalAmount+ulOffSaleTotalAmount+ulReloadTotalAmount))
		{
			ulTotalAmount = ulRefundTotalAmount - (ulSaleTotalAmount+ulOffSaleTotalAmount+ulReloadTotalAmount);
			memset(szTemp1, 0x00, sizeof(szTemp1)); 
			memset(szTemp2, 0x00, sizeof(szTemp2));
			sprintf(szTemp1,"%.0f", ulTotalAmount);		
			vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
			sprintf(szTemp,"-%s%s",strCST.szCurSymbol,szTemp2);
			sprintf(szStr,"  TOT.     %04d", usTotalCount);	
			inPrintLeftRight2(szStr,szTemp,41);
		}
		else
		{
			ulTotalAmount = (ulSaleTotalAmount+ulOffSaleTotalAmount+ulReloadTotalAmount) - ulRefundTotalAmount;
			memset(szTemp1, 0x00, sizeof(szTemp1)); 
			memset(szTemp2, 0x00, sizeof(szTemp2));
			sprintf(szTemp1,"%.0f", ulTotalAmount);		
			vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
			sprintf(szTemp,"%s%s",strCST.szCurSymbol,szTemp2);
			sprintf(szStr,"  TOT.     %04d", usTotalCount);							
			inPrintLeftRight2(szStr,szTemp,41);
		}
	}
	else if(inReportType == PRINT_CARD_TOTAL)
	{
		if (srTransRec.HDTid == WECHAT_HOST_INDEX || srTransRec.HDTid == ALIPAY_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX || srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == UPI_HOST_INDEX || srTransRec.HDTid == PHQR_HOST_INDEX)
		{
			printDividingLine(DIVIDING_LINE);
			
			if(ulRefundTotalAmount > ulSaleTotalAmount)
			{
				ulTotalAmount = ulRefundTotalAmount - (ulSaleTotalAmount);
				memset(szTemp1, 0x00, sizeof(szTemp1)); 
				memset(szTemp2, 0x00, sizeof(szTemp2));
				sprintf(szTemp1,"%.0f", ulTotalAmount);		
				vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
				sprintf(szTemp,"-%s%s",strCST.szCurSymbol,szTemp2);
				sprintf(szStr,"  TOTAL    %04d", usTotalCount);	
				inPrintLeftRight2(szStr,szTemp,41);
			}
			else
			{
				ulTotalAmount = (ulSaleTotalAmount) - ulRefundTotalAmount;
				memset(szTemp1, 0x00, sizeof(szTemp1)); 
				memset(szTemp2, 0x00, sizeof(szTemp2));
				sprintf(szTemp1,"%.0f", ulTotalAmount);		
				vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
				sprintf(szTemp,"%s%s",strCST.szCurSymbol,szTemp2);
				sprintf(szStr,"  TOTAL    %04d", usTotalCount);							
				inPrintLeftRight2(szStr,szTemp,41);
			}
			
		}
		else
		{
			printDividingLine(DIVIDING_LINE);
			
			if(ulRefundTotalAmount > (ulSaleTotalAmount+ulOffSaleTotalAmount+ulReloadTotalAmount))
			{
				ulTotalAmount = ulRefundTotalAmount - (ulSaleTotalAmount+ulOffSaleTotalAmount+ulReloadTotalAmount);
				memset(szTemp1, 0x00, sizeof(szTemp1)); 
				memset(szTemp2, 0x00, sizeof(szTemp2));
				sprintf(szTemp1,"%.0f", ulTotalAmount);		
				vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
				sprintf(szTemp,"-%s%s",strCST.szCurSymbol,szTemp2);
				sprintf(szStr,"  TOT.     %04d", usTotalCount);	
				inPrintLeftRight2(szStr,szTemp,41);
			}
			else
			{
				ulTotalAmount = (ulSaleTotalAmount+ulOffSaleTotalAmount+ulReloadTotalAmount) - ulRefundTotalAmount;
				memset(szTemp1, 0x00, sizeof(szTemp1)); 
				memset(szTemp2, 0x00, sizeof(szTemp2));
				sprintf(szTemp1,"%.0f", ulTotalAmount);		
				vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp1,szTemp2);
				sprintf(szTemp,"%s%s",strCST.szCurSymbol,szTemp2);
				sprintf(szStr,"  TOT.     %04d", usTotalCount);							
				inPrintLeftRight2(szStr,szTemp,41);
			}
		}			 
	}

	//memset (baTemp, 0x00, sizeof(baTemp));
	//CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
	//CTOS_PrinterBufferOutput((BYTE *)baTemp, 3); 

    vdCTOS_PrinterFline(1);
	//CTOS_PrinterFline(d_LINE_DOT * 1);
}

int inCTOS_SelectFont(int inFontMode,int inFontSize ,int inFontStyle,char * szFontName)
{
	if(inFontMode == d_FONT_TTF_MODE)
	{
		CTOS_PrinterFontSelectMode(d_FONT_TTF_MODE);	//set the printer with TTF Mode
		CTOS_PrinterTTFSelect("times.ttf", inFontStyle);
	}
	else
	{
	
		CTOS_PrinterFontSelectMode(d_FONT_FNT_MODE);	//set the printer with default Mode
		CTOS_LanguagePrinterFontSize(inFontSize, 0, TRUE);		
	}
	return d_OK;
	
}

int inCTOS_PrintSettleReport(BOOL fManualSettlement, BOOL fCaptureReceipt)
{
	ACCUM_REC srAccumRec;
	unsigned char chkey;
	short shHostIndex;
	int inResult;
	int inTranCardType;
	int inReportType;
	int i;		
	char szStr[d_LINE_SIZE + 1];
	BYTE baTemp[PAPER_X_SIZE * 64];	
	int inTempTransType;

    BYTE        szTempDate[DATE_BCD_SIZE+1];     
    BYTE        szTempTime[TIME_BCD_SIZE+1];  

	int inBatchNumOfRecord;
	int *pinTransDataid;
	int inIITCount = 0;
	int inCount = 0;
	BOOL fPrintSettleDetailReport = (VS_BOOL)strTCT.fPrintSettleDetailReport;
	
	vdDebug_LogPrintf("--inCTOS_PrintSettleReport--");
	vdDebug_LogPrintf("fCaptureReceipt[%d]", fCaptureReceipt);
	vdDebug_LogPrintf("strTCT.byERMMode[%d]", strTCT.byERMMode);
	vdDebug_LogPrintf("fAutoSettleFlag=[%d]", srTransRec.fAutoSettleFlag);
	
	vdSetGlobalVariable();

	strGBLVar.fGBLvConfirmPrint = FALSE; // sidumili: disable confirmation instead print directly

	// sidumili: do not prompt for print confirmation instead print directly
	if (srTransRec.fAutoSettleFlag)
	{
		if (strGBLVar.fGBLvConfirmPrint)
			strGBLVar.fGBLvConfirmPrint = FALSE;
	}
	
	if( printCheckPaper()==-1)
		return d_NO;

	//srTransRec.fManualSettlement=fManualSettlement; 

	inResult = inCTOS_ChkBatchEmpty();
	if(d_OK != inResult)
	{
		return d_NO;
	}

	if (fFSRMode() == TRUE || (isCheckTerminalMP200() == d_OK))
		fPrintSettleDetailReport = FALSE;
	
    if (fManualSettlement != TRUE)
		inCTLOS_Updatepowrfail(PFR_BATCH_SETTLEMENT_PRINT);//1204

	//save Orig Trans type before detail report
	inTempTransType = srTransRec.byTransType;

	vdCTOSS_PrinterStart(100);

	SetReportTime();
	//save date and time for retrieval later-after printing of details report
    memset(szTempDate, 0x00, sizeof(szTempDate));
    memset(szTempTime, 0x00, sizeof(szTempTime));
	memcpy(szTempDate, srTransRec.szDate, DATE_BCD_SIZE);
	memcpy(szTempTime, srTransRec.szTime, TIME_BCD_SIZE);

	if (srTransRec.HDTid != WECHAT_HOST_INDEX && srTransRec.HDTid != ALIPAY_HOST_INDEX && srTransRec.HDTid != GCASH_HOST_INDEX && srTransRec.HDTid != GRAB_HOST_INDEX && srTransRec.HDTid != UPI_HOST_INDEX && srTransRec.HDTid != PHQR_HOST_INDEX)
	{
		/* BDO: Include detailed report to settlement receipt - start -- jzg */
	    //if(srTransRec.byTransType == MANUAL_SETTLE || fAUTOManualSettle == TRUE)
	    if(srTransRec.byTransType == MANUAL_SETTLE )
			vdCTOS_SettlementPrintDetailReport(TRUE, FALSE, FALSE);
	    //else if(strTCT.fPrintSettleDetailReport == TRUE)
	    else
			vdCTOS_SettlementPrintDetailReport(TRUE, FALSE, FALSE);
	}
	
	//Reload back Original Trans Type
	srTransRec.byTransType = inTempTransType;
	
	memcpy(srTransRec.szDate, szTempDate, DATE_BCD_SIZE);
	memcpy(srTransRec.szTime, szTempTime,  TIME_BCD_SIZE);

	inBatchNumOfRecord = inBatchNumRecord();	
	DebugAddINT("batch record",inBatchNumOfRecord);
	if(inBatchNumOfRecord <= 0)
	{
		vdDisplayErrorMsg(1, 8, "BATCH EMPTY");
		return d_NO;
	}	
		
	memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    if((inResult = inCTOS_ReadAccumTotal(&srAccumRec)) == ST_ERROR)
    {
        vdMyEZLib_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
		vdSetErrorMessage("Read Accum Error");
        return ST_ERROR;    
    }

   	//vdDisplayAnimateBmp(0,0, "Print1.bmp", "Print2.bmp", "Print3.bmp", NULL, NULL); /*BDO: Display printing image during report -- sidumili*/
	//vdPrintReportDisplayBMP();
	inCTOS_DisplayPrintBMP();
	
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);

	CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);

	//SetReportTime();  
	vdPrintHeaderLogo();
	ushCTOS_PrintHeader(0);	
	
	if (srTransRec.HDTid == WECHAT_HOST_INDEX || srTransRec.HDTid == ALIPAY_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX || srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == UPI_HOST_INDEX || srTransRec.HDTid == PHQR_HOST_INDEX)
	{
		vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		vdPrintCenter("SETTLEMENT REPORT");
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);
		
		printHostLabel();
		iXMLPrintWechatTIDMID();
		iXMLPrintWechatDateTime(TRUE, FALSE, 0);
		printDividingLine(DIVIDING_LINE);

		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);				
	
		vdDebug_LogPrintf("szMID[%s]", srTransRec.szMID);
		vdDebug_LogPrintf("szTID[%s]", srTransRec.szTID);
		vdDebug_LogPrintf("szBatchNo[%02X%02X%02X]", srTransRec.szBatchNo[0], srTransRec.szBatchNo[1], srTransRec.szBatchNo[2]);

		// Print Summary
		//inPrint("TERMINAL TOTAL:");	
		inIITCount = inIITNumRecord() + 1;
		for(inTranCardType = 0; inTranCardType < 1 ;inTranCardType ++)
		{
			BOOL fPrintTotal=FALSE;
			inReportType = PRINT_CARD_TOTAL;

			if(inReportType == PRINT_CARD_TOTAL)
			{
				for(i= 0; i < inIITCount; i ++ )
				{
					vdDebug_LogPrintf("--Count[%d]", i);
					if((srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usSaleCount == 0)
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount == 0)
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usRefundCount == 0)
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usVoidSaleCount == 0)			
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usReloadCCCount == 0))
						continue;
					
					
					vdDebug_LogPrintf("Count[%d]", i); 
					inIITRead(i);
					vdCTOS_PrintAccumeByHostAndCard (inReportType, 
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usSaleCount - srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount,   
													(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulSaleTotalAmount) - (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),												
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usRefundCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulRefundTotalAmount), 												
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usVoidSaleCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulVoidSaleTotalAmount), 
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usReloadCCCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulReloadCCTotalAmount));
					//fPrintTotal=TRUE; // sidumili: Remove Grand Total priting
				}

				//after print issuer total, then print host toal
				if(fPrintTotal == TRUE)
				{
					memset(szStr, ' ', d_LINE_SIZE);
					strcpy(szStr,"GRAND TOTAL:");
					inPrint(szStr);
	                vdCTOS_PrintAccumeByHostAndCard (inReportType, 
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usSaleCount - srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount,   
													(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulSaleTotalAmount) - (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),												
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usRefundCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulRefundTotalAmount), 												
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usVoidSaleCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulVoidSaleTotalAmount), 
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usReloadCCCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulReloadCCTotalAmount));
				}
			}		
		}

		vdCTOS_PrinterFline(1);

		// Flag to print detail report during settlement
		if (fPrintSettleDetailReport)
		{
			// Print Detail Report
			inPrint("DETAIL REPORT:");		
			vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);
			vdDebug_LogPrintf("szMID[%s]", srTransRec.szMID);
			vdDebug_LogPrintf("szTID[%s]", srTransRec.szTID);
			vdDebug_LogPrintf("szBatchNo[%02X%02X%02X]", srTransRec.szBatchNo[0], srTransRec.szBatchNo[1], srTransRec.szBatchNo[2]);

			ushCTOS_WechatprintBatchRecordHeader();
				
			pinTransDataid = (int*)malloc(inBatchNumOfRecord  * sizeof(int));
			inCount = 0;		
			inBatchByMerchandHost(inBatchNumOfRecord, srTransRec.HDTid, srTransRec.MITid, srTransRec.szBatchNo, pinTransDataid);
			for(i=0; i<inBatchNumOfRecord; i++)
			{
				inResult = inDatabase_BatchReadByTransId(&srTransRec, pinTransDataid[inCount]);
			    inCount ++;
				ushCTOS_WechatprintBatchRecordFooter(FALSE);	
			}

			 free(pinTransDataid);

			printDividingLine(DIVIDING_LINE);

			vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
			vdPrintCenter("END OF REPORT");
			vdCTOS_PrinterFline(2);
			vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

			vdCTOS_PrinterFline(4);
		}		

		vdSetPrintThreadStatus(0);

		vdPrintTitleCenter("SETTLEMENT CONFIRMED");	
	    vdCTOS_PrinterFline(2);
		//print space one line
		vdLineFeed(FALSE);
		
		//vdCTOSS_PrinterEnd(); // Commented to support print confirmation -- sidumili
	
		// Capture report -- sidumili
		vdDebug_LogPrintf("Capturing erm report...");
		vdDeleteBMP();
		vdCTOSS_PrinterEnd_CConvert2BMP("/data/data/pub/Print_BMP.bmp");
		CTOS_KBDBufFlush();
		inCTOSS_ERM_Form_Receipt(0);
		
		if (strGBLVar.fGBLvConfirmPrint)
		{
			// Print confirmation -- sidumili
			if (inPrintConfirmation("SETTLEMENT", FALSE) == d_OK)
			{
				vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
				vdCTOSS_PrinterEnd();
			}
			else
			{			
				vdCTOSS_PrinterEnd();
			}
		}
		else
		{
			//usCTOSS_LCDDisplay("SETTLEMET REPORT|PRINTING...");
			vdDisplayMessageBox(1, 8, "", "PRINTING...", "", MSG_TYPE_PRINT);
			vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
			vdCTOSS_PrinterEnd();
		}
	}
	
	return d_OK;	
}


USHORT ushCTOS_printBatchRecordHeader(void)
{
	USHORT result;
	BYTE baTemp[PAPER_X_SIZE * 64];
	BYTE szLogoPath[256 + 1];

	memset(szLogoPath, 0x00, sizeof(szLogoPath));	

    sprintf(szLogoPath, "%s%s", LOGO_PATH, strHDT.szHeaderLogoName);
	vdCTOSS_PrinterBMPPicEx(0, 0, szLogoPath);
	
    ushCTOS_PrintHeader(0);	
    
    vdPrintTitleCenter("DETAIL REPORT");
	vdCTOS_PrinterFline(1);

	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
    printHostLabel();
    
	printTIDMID();
	
	vdCTOS_SetDateTime();
	 
	printDateTime();
    
	printBatchNO();
    
    vdCTOS_PrinterFline(1);
    inPrint("CARD NAME           CARD NUMBER");
    inPrint("EXP DATE            TRACE NO.");
    inPrint("TRANSACTION         AMOUNT");
    inPrint("APPR. CODE          ENTRY MODE");
	
	printDividingLine(DIVIDING_LINE_DOUBLE);
	return d_OK;
}


USHORT ushCTOS_printBatchRecordFooter(void)
{
	BYTE baTemp[PAPER_X_SIZE * 64];		
	char szStr[d_LINE_SIZE + 1];
	char szPrintBuf[d_LINE_SIZE + 1];
	char szTempBuf1[d_LINE_SIZE + 1];
	char szTempBuf2[d_LINE_SIZE + 1];
	char szTempBuf3[d_LINE_SIZE + 1];

	int i;

	DebugAddSTR("ushCTOS_printBatchRecordFooter", srTransRec.szPAN, 10);
	memset(szTempBuf1,0,sizeof(szTempBuf1));
	memset(szTempBuf2, 0, sizeof(szTempBuf2));
	strcpy(szTempBuf1,srTransRec.szPAN);    
    inIITRead(srTransRec.IITid);
	strcpy(szTempBuf2,strIIT.szIssuerLabel);
	if(strHDT.fMaskDetail == TRUE)
	    cardMasking(szTempBuf1, PRINT_CARD_MASKING_3);

    memset (szPrintBuf, 0x00, sizeof(szPrintBuf));
    strcpy(szPrintBuf, szTempBuf2);
    memset(&szPrintBuf[strlen(szPrintBuf)], 0x20, 20-strlen(szTempBuf2));
    strcat(szPrintBuf, szTempBuf1);
	inPrintNoErm(szPrintBuf);

	//Exp date and inv num
	memset(szTempBuf1, 0, sizeof(szTempBuf1));
	memset(szTempBuf2, 0, sizeof(szTempBuf2));
	memset(szTempBuf3, 0, sizeof(szTempBuf3));
	
	wub_hex_2_str(srTransRec.szExpireDate, szTempBuf1,EXPIRY_DATE_BCD_SIZE);
	DebugAddSTR("detail--EXP",szTempBuf1,12);  

	
    if(strHDT.fMaskDetail == TRUE)
    {
       for (i =0; i<4;i++)
       szTempBuf1[i] = '*';
    }
	memcpy(szTempBuf2,&szTempBuf1[0],2);
	memcpy(szTempBuf3,&szTempBuf1[2],2);

	memset(szStr, 0,sizeof(szStr));
	wub_hex_2_str(srTransRec.szInvoiceNo, szStr, INVOICE_BCD_SIZE);
		
	memset (szPrintBuf, 0x00, sizeof(szPrintBuf));
    strcpy(szPrintBuf, szTempBuf2);
    strcat(szPrintBuf, szTempBuf3);
    memset(&szPrintBuf[strlen(szPrintBuf)], 0x20, 20-strlen(szTempBuf2)-strlen(szTempBuf3));
    strcat(szPrintBuf, szStr);
	inPrintNoErm(szPrintBuf);
 
    memset(szStr, 0,sizeof(szStr));
    memset(szTempBuf1, 0,sizeof(szTempBuf1));
    memset(szTempBuf2, 0,sizeof(szTempBuf2));
	if(srTransRec.byTransType == LOY_REDEEM_5050 || srTransRec.byTransType == LOY_REDEEM_VARIABLE)
	    strcpy(szStr, "REDEEM");
	else
        szGetTransTitle(srTransRec.byTransType, szStr);
    wub_hex_2_str(srTransRec.szTotalAmount, szTempBuf1, AMT_BCD_SIZE);
	vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szTempBuf1, szTempBuf2);

    memset(szPrintBuf, 0x20, sizeof(szPrintBuf));
	memcpy(szPrintBuf, szStr, strlen(szStr));
	memcpy(&szPrintBuf[20], szTempBuf2, strlen(szTempBuf2));
	szPrintBuf[46]=0x00;
	inPrintNoErm(szPrintBuf);
	
	memset(szTempBuf2, 0,sizeof(szTempBuf2));
	if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
		memcpy(szTempBuf2,"Chip",4);
	else if(srTransRec.byEntryMode==CARD_ENTRY_MANUAL)
		memcpy(szTempBuf2,"Manual",6);
	else if(srTransRec.byEntryMode==CARD_ENTRY_MSR)
		memcpy(szTempBuf2,"MSR",3);
	else if(srTransRec.byEntryMode==CARD_ENTRY_FALLBACK)
		memcpy(szTempBuf2,"Fallback",8);


    memset (szPrintBuf, 0x00, sizeof(szPrintBuf));
    strcpy(szPrintBuf, srTransRec.szAuthCode);
    memset(&szPrintBuf[strlen(szPrintBuf)], 0x20, 20-strlen(srTransRec.szAuthCode));
    strcat(szPrintBuf, szTempBuf2);
	inPrintNoErm(szPrintBuf);
	
	
	vdCTOS_PrinterFline(1);

	
}

USHORT ushCTOS_GetFontInfor(void)
{
	USHORT usASCIIFontID;
	USHORT usFontSize;
	USHORT usFontStyle;

	CTOS_LanguagePrinterGetFontInfo( &usASCIIFontID, &usFontSize, &usFontStyle );   
	vdDebug_LogPrintf("usASCIIFontID[%d]usFontSize[%d]usFontStyle[%d]",usFontSize,usFontStyle );
}

USHORT ushCTOS_ReprintLastSettleReport(void)
{
	ACCUM_REC srAccumRec;
	unsigned char chkey;
	short shHostIndex;
	int inResult,inRet;
	int inTranCardType;
	int inReportType;
	int inIITNum , i;
	char szStr[d_LINE_SIZE + 1];
	BYTE baTemp[PAPER_X_SIZE * 64];

	int inIITCount = 0;
	int inCount = 0;
	int inBatchNumOfRecord;
	int *pinTransDataid;
	BOOL fPrintSettleDetailReport = (VS_BOOL)strTCT.fPrintSettleDetailReport;

    /*do not print detail report during Last Settlement*/
    fPrintSettleDetailReport=0;

	vdDebug_LogPrintf("--ushCTOS_ReprintLastSettleReport--");
	
	vdSetGlobalVariable();
	
	fRePrintFlag=TRUE;

	//for MP200 no need print
	if ((strTCT.byTerminalType == 5) || (strTCT.byTerminalType == 6))
    	return (d_OK);
	
	if( printCheckPaper()==-1)
		return d_NO;

    inRet = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inRet)
        return inRet;
		
	//by host and merchant
	shHostIndex = inCTOS_SelectHostSetting();
	if (shHostIndex == -1)
		return d_NO;
	strHDT.inHostIndex = shHostIndex;
	DebugAddINT("summary host Index",shHostIndex);
	inCSTRead(strHDT.inCurrencyIdx);

	if (inMultiAP_CheckMainAPStatus() == d_OK)
	{
		inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_REPRINT_LAST_SETT);
		if(d_OK != inRet)
			return inRet;
	}
	else
	{
		if (inMultiAP_CheckSubAPStatus() == d_OK)
		{
			inRet = inCTOS_MultiAPGetData();
			if(d_OK != inRet)
				return inRet;

			inRet = inCTOS_MultiAPReloadHost();
			if(d_OK != inRet)
				return inRet;
		}
	}

	//inMMTReadRecord(srTransRec.HDTid,srTransRec.MITid);
	inResult = inCTOS_CheckAndSelectMutipleMID();
	DebugAddINT("summary MITid",srTransRec.MITid);
	if(d_OK != inResult)
		return d_NO;

	memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
	inResult = inCTOS_ReadBKAccumTotal(&srAccumRec,strHDT.inHostIndex,srTransRec.MITid);
	if(inResult == ST_ERROR)
	{
	    vdSetErrorMessage("NO RECORD FOUND");
		vdMyEZLib_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
		return ST_ERROR;	
	}		 
	else if(inResult == RC_FILE_READ_OUT_NO_DATA)
	{
	    vdSetErrorMessage("NO RECORD FOUND");
		return d_NO;
	}	 
    /*albert - start - 20161202 - Reprint of Detail Report for Last Settlement Report*/   

	vdCTOSS_PrinterStart(100);
	CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);

	fRePrintFlag = TRUE;

	if (srTransRec.HDTid == WECHAT_HOST_INDEX || srTransRec.HDTid == ALIPAY_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX || srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == UPI_HOST_INDEX || srTransRec.HDTid == PHQR_HOST_INDEX)
	{
		inBatchNumOfRecord = inBackupDetailReportNumRecord();
	
		DebugAddINT("batch record",inBatchNumOfRecord);
		if(inBatchNumOfRecord <= 0)
		{
			vdDisplayErrorMsg(1, 8, "BATCH EMPTY");
			return d_NO;
		}
	
		memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
		inResult = inCTOS_ReadBKAccumTotal(&srAccumRec,strHDT.inHostIndex,srTransRec.MITid);
		if(inResult == ST_ERROR)
		{
		    vdSetErrorMessage("NO RECORD FOUND");
			vdMyEZLib_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
			return ST_ERROR;	
		}		 
		else if(inResult == RC_FILE_READ_OUT_NO_DATA)
		{
		    vdSetErrorMessage("NO RECORD FOUND");
			return d_NO;
		}	 
	
		//vdPrintReportDisplayBMP();
		inCTOS_DisplayPrintBMP();
	
		inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_16x16,0," ");
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);

		vdPrintHeaderLogo();
		ushCTOS_PrintHeader(0);	

		vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
		vdPrintCenter("SETTLEMENT REPORT");
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);
		
		printHostLabel();

		iXMLPrintWechatTIDMID();
		
		iXMLPrintWechatDateTime(TRUE, TRUE, 0);

		printDividingLine(DIVIDING_LINE);
			
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);

		vdDebug_LogPrintf("szMID[%s]", srTransRec.szMID);
		vdDebug_LogPrintf("szTID[%s]", srTransRec.szTID);
		vdDebug_LogPrintf("szBatchNo[%02X%02X%02X]", srTransRec.szBatchNo[0], srTransRec.szBatchNo[1], srTransRec.szBatchNo[2]);

		// Print Summary
		//inPrint("TERMINAL TOTAL:");	
		inIITCount = inIITNumRecord() + 1;
		for(inTranCardType = 0; inTranCardType < 1 ;inTranCardType ++)
		{
			inReportType = PRINT_CARD_TOTAL;
			
			if(inReportType == PRINT_CARD_TOTAL)
			{
				for(i= 0; i < inIITCount; i ++ )
				{
					vdDebug_LogPrintf("--Count[%d]", i);
					if((srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usSaleCount == 0)
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount == 0)
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usRefundCount == 0)
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usVoidSaleCount == 0)			
					&&(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usReloadCCCount == 0))
						continue;
					
					
					vdDebug_LogPrintf("Count[%d]", i); 
					inIITRead(i);
					vdCTOS_PrintAccumeByHostAndCard (inReportType, 
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usSaleCount - srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount,   
													(srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulSaleTotalAmount) - (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),												
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usRefundCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulRefundTotalAmount), 												
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usVoidSaleCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulVoidSaleTotalAmount), 
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usOffSaleCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),
													srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].usReloadCCCount, (srAccumRec.stBankTotal[CREDIT_CARD].stCardTotal[i].ulReloadCCTotalAmount));	
				}			
			}		
		}

		// Flag to print detail report during settlement
		if (fPrintSettleDetailReport)
		{
			vdCTOS_PrinterFline(1);
			// Print Detail
			inPrint("DETAIL REPORT:");

			// Header
			inPrintLeftRight("TYPE", "AMOUNT", 41);
			inPrintLeftRight("ORDER ID", " ", 41);
			printDividingLine(DIVIDING_LINE);

			pinTransDataid = (int*)malloc(inBatchNumOfRecord  * sizeof(int));
			inCount = 0;	
			inBatchByDetailReport(inBatchNumOfRecord, srTransRec.HDTid, srTransRec.MITid, pinTransDataid);
			for(i=0; i<inBatchNumOfRecord; i++)
			{
				inResult = inDatabase_ReadDetailReport(&srTransRec, pinTransDataid[inCount]);
				inCount ++;
				ushCTOS_WechatprintBatchRecordFooter(TRUE);
			}

			 free(pinTransDataid);

            #if 0
			printDividingLine(DIVIDING_LINE);
		
			vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
			vdPrintCenter("END OF REPORT");
			vdCTOS_PrinterFline(2);
			vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
			#endif
		}		

        printDividingLine(DIVIDING_LINE);
        
        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
        vdPrintCenter("END OF REPORT");
        vdCTOS_PrinterFline(2);
        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
		
	//print space one line
    vdCTOS_PrinterFline(6);
	
	vdCTOSS_PrinterEnd();

	// For Wechat / Alipay, do not print -- sidumili
	if (srTransRec.HDTid == WECHAT_HOST_INDEX && srTransRec.HDTid == ALIPAY_HOST_INDEX && srTransRec.HDTid == GCASH_HOST_INDEX && srTransRec.HDTid == GRAB_HOST_INDEX && srTransRec.HDTid == UPI_HOST_INDEX)
	{
		// Do nothing...
	}
	//else
	//{
	//	if(srAccumRec.fManualSettlement == TRUE)
	//      vdCTOS_PrintManualPosting();
	//}
	
	return d_OK;	
}


void vdCTOSS_PrinterBMPPic(unsigned int usX,unsigned int y, const char *path)
{
	char szBmpfile[100+1];
	
	memset(szBmpfile, 0x00, sizeof(szBmpfile));
	sprintf(szBmpfile,"%s%s", LOGO_PATH, path);

	vdDebug_LogPrintf("szBmpfile=[%s]", szBmpfile);
	
	vdCTOSS_PrinterBMPPicEx(0, 0, szBmpfile);
	return ;
}

int inPrint(unsigned char *strPrint) 
{
	char szStr[46+1] = {0}; 

	//vdDebug_LogPrintf("inPrint | fERMTransaction[%d]", fERMTransaction);
	
	memset(szStr, 0x00, sizeof(szStr));
	memcpy(szStr, strPrint, strlen(strPrint));

	inCCTOS_PrinterBufferOutput(szStr,&stgFONT_ATTRIB, fERMTransaction);
	return d_OK;
}

int inPrintNoErm(unsigned char *strPrint) 
{
	char szStr[46+1] = {0}; 
	//BYTE baTemp[PAPER_X_SIZE * 64] = {0};

	//vdDebug_LogPrintf("inPrint | fERMTransaction[%d]", fERMTransaction);
	
	memset(szStr, 0x00, sizeof(szStr));
	memcpy(szStr, strPrint, strlen(strPrint));

	//memset (baTemp, 0x00, sizeof(baTemp));
	//CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
	//return CTOS_PrinterBufferOutput((BYTE *)baTemp, 4);
	inCCTOS_PrinterBufferOutput(szStr,&stgFONT_ATTRIB, fERMTransaction);
	return d_OK;
}

int inPrintLeftRight(unsigned char *strLeft, unsigned char *strRight, int inWidth) 
{
	char szStr[64+1] = {0}; 
	BYTE baTemp[PAPER_X_SIZE * 64] = {0};

	//vdDebug_LogPrintf("inPrintLeftRight | fERMTransaction[%d]", fERMTransaction);

	if ((isCheckTerminalMP200() == d_OK) && (strTCT.byERMMode == 2))
	{
		inWidth = 46;
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
	
    inWidth--;
	
	memset(szStr, 0x20, sizeof(szStr));
    memcpy(szStr, strLeft, strlen(strLeft));
    memcpy(&szStr[inWidth-strlen(strRight)], strRight, strlen(strRight));
	szStr[inWidth]=0x00;
	
	inCCTOS_PrinterBufferOutput(szStr,&stgFONT_ATTRIB, fERMTransaction);
	return d_OK;
}

/*albert - start - Aug2014 - compute crc*/
void vdCTOS_PrintCRC(void)
{
    int shHostIndex = 1, inNumOfMerchant=0;
    int inResult,inRet;
    unsigned char szCRC[8+1];
    char ucLineBuffer[d_LINE_SIZE];
    BYTE baTemp[PAPER_X_SIZE * 64];
	char szStr[d_LINE_SIZE + 1];
	
    vduiLightOn();                

    //fix for issue #00092
    //Must display "Out of Paper..."
    if( printCheckPaper()==-1)
      return;
	
    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;

    //CTOS_LCDTClearDisplay();

    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_PRINT_CRC);
        if(d_OK != inRet)
            return ;
    }
    else
    {
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return ;

            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return ;
        }
    }
		
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

    vdCTOSS_PrinterStart(100);
    CTOS_PrinterSetHeatLevel(4);  
    inCTOS_DisplayPrintBMP();

    fRePrintFlag = FALSE;
	
	//print Logo	
	if(strlen(strHDT.szHeaderLogoName))
		vdCTOSS_PrinterBMPPic(0, 0, strHDT.szHeaderLogoName);
	else
		vdCTOSS_PrinterBMPPic(0, 0, "logo.bmp");

    vdPrintTitleCenter("CRC REPORT");
	vdCTOS_PrinterFline(1);

	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
	vdCTOS_SetDateTime();
	
    printDateTime();
	vdCTOS_PrinterFline(1);

    memset(baTemp, 0x00, sizeof(baTemp));
    sprintf(baTemp, "HOST: %s", strHDT.szHostLabel);
	inPrint(baTemp);

	// Print CRC
	memset(szCRC, 0x00, sizeof(szCRC));
	vdComputeGetAppCRC(strHDT.szAPName, szCRC, FALSE);
	inPrint(szCRC);

	// Print CRC
	//vdPrintCRC();
	
    vdCTOS_PrinterFline(7);
	vdCTOSS_PrinterEnd();

}
/*albert - end - Aug2014 - compute crc*/

void vdCTOS_HostInfo(void){
    int shHostIndex = 1, inNumOfMerchant=0;
    int inResult,inRet;
    unsigned char szCRC[8+1];
    char ucLineBuffer[d_LINE_SIZE];
    BYTE baTemp[PAPER_X_SIZE * 64];
    int inLoop = 0;
    char szBillerName[1024] = {0};
    char szBillerCode[1024] = {0};
    char szStr[d_LINE_SIZE + 1];
    BYTE szTempBuf[12+1];
    BYTE szBuf[50];
    char szSerialNum[16 + 1];
	
	vdDebug_LogPrintf("--vdCTOS_HostInfo--");
		
	vdSetGlobalVariable();


vduiLightOn();								
    if( printCheckPaper()==-1)
        return;

shHostIndex = inCTOS_SelectHostSetting();
if (shHostIndex == -1)
	return;

//CTOS_LCDTClearDisplay();

if (inMultiAP_CheckMainAPStatus() == d_OK)
{
	inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_PRINT_HOST_INFO);
	if(d_OK != inRet)
		  return ;
}
else
{
	if (inMultiAP_CheckSubAPStatus() == d_OK)
	{
		inRet = inCTOS_MultiAPGetData();
		if(d_OK != inRet)
			return ;
		
		inRet = inCTOS_MultiAPReloadHost();
		if(d_OK != inRet)
			return ;
	}
}	 

//inRet = inCTOS_MultiAPReloadHost();
//if(d_OK != inRet)
	//return ;

//tine: capture correct values of headers for the chosen host.
//inCTOS_CheckAndSelectMutipleMID();
		
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

    vdCTOSS_PrinterStart(100);
    CTOS_PrinterSetHeatLevel(4);  
    inCTOS_DisplayPrintBMP();
    //vdSetPrintThreadStatus(1); //##00129
    
    //print Logo	
    if(strlen(strHDT.szHeaderLogoName))
        vdCTOSS_PrinterBMPPic(0, 0, strHDT.szHeaderLogoName);
    else
        vdCTOSS_PrinterBMPPic(0, 0, "logo.bmp");

	ushCTOS_PrintHeader(0);
    vdPrintTitleCenter("HOST INFO REPORT");
    vdCTOS_PrinterFline(1);

    vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
    
    vdCTOS_SetDateTime();
    
    printDateTime();
    
    inPrint("---------------------------------------------");
    
    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "HOST        : %s", (char *)strHDT.szHostLabel);
    inPrint(szStr);
    
    memset(szTempBuf, 0x00, sizeof(szTempBuf));
    wub_hex_2_str(strHDT.szTPDU,szTempBuf,5);
    sprintf(szStr, "TPDU        : %s", szTempBuf);
    inPrint(szStr);
    
    memset(szTempBuf, 0x00, sizeof(szTempBuf));
    wub_hex_2_str(strHDT.szNII,szTempBuf,2);
    sprintf(szStr, "NII         : %s", szTempBuf);
    inPrint(szStr);
    
    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "PRI TEL NUM : %s", (char *)strCPT.szPriTxnPhoneNumber);
    inPrint(szStr);
    
    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "SEC TEL NUM : %s", (char *)strCPT.szSecTxnPhoneNumber);
    inPrint(szStr);
    
    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "PRI IP      : %s", (char *)strCPT.szPriTxnHostIP);
    inPrint(szStr);

    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "PRI IP PORT : %04ld", strCPT.inPriTxnHostPortNum);
    inPrint(szStr);
    
    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "SEC IP      : %s", (char *)strCPT.szSecTxnHostIP);
    inPrint(szStr);

    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "SEC IP PORT : %04ld", strCPT.inSecTxnHostPortNum);
    inPrint(szStr);
    
    memset(szSerialNum, 0, sizeof(szSerialNum));
    //CTOS_GetFactorySN(szSerialNum);
    usGetSerialNumber(szSerialNum);
    szSerialNum[15]=0;
    memset(szStr, 0x00, d_LINE_SIZE);
    sprintf(szStr, "SERIAL NO   : %s", szSerialNum);
    inPrint(szStr);

// Print CRC
vdPrintCRC();

inPrint("---------------------------------------------");

inMMTReadNumofRecords(strHDT.inHostIndex,&inNumOfMerchant);

vdDebug_LogPrintf("[inNumOfMerchant]-[%d]strHDT.inHostIndex[%d]", inNumOfMerchant,strHDT.inHostIndex);
for(inLoop=1; inLoop <= inNumOfMerchant;inLoop++)
{
	 if((inResult = inMMTReadRecord(strHDT.inHostIndex,inLoop)) !=d_OK)
	 {
			 vdDebug_LogPrintf("[read MMT fail]-Mitid[%d]strHDT.inHostIndex[%d]inResult[%d]", inLoop,strHDT.inHostIndex,inResult);
			 continue;
			 //break;
	 }
	 else 
	 {
			 if(strMMT[0].fMMTEnable)
			 {	
					 memset(szStr, 0x00, sizeof(szStr));
					 sprintf(szStr, "  MERCHANT : %s", strMMT[0].szMerchantName); 											 
					 inPrint(szStr);

					 memset(szStr, 0x00, sizeof(szStr));
					 sprintf(szStr, "  TID      : %s", strMMT[0].szTID); 											 
					 inPrint(szStr);

					 memset(szStr, 0x00, sizeof(szStr));
					 sprintf(szStr, "  MID      : %s", strMMT[0].szMID); 											 
					 inPrint(szStr);

					 memset(szStr, 0x00, sizeof(szStr));
					 memset(szBuf, 0x00, sizeof(szBuf));
					 wub_hex_2_str(strMMT[0].szBatchNo, szBuf, 3);
					 sprintf(szStr, "  BATCH NO : %s", szBuf); 											 
					 inPrint(szStr);
					 vdCTOS_PrinterFline(1);

			 }										

	 }
}

    vdLineFeed(FALSE);
	
	//vdCTOSS_PrinterEnd(); // Commented to support print confirmation -- sidumili
	
	// Capture report -- sidumili		
	vdDebug_LogPrintf("Capturing erm report...");
	vdDeleteBMP();
	vdCTOSS_PrinterEnd_CConvert2BMP("/data/data/pub/Print_BMP.bmp");
	CTOS_KBDBufFlush();
	inCTOSS_ERM_Form_Receipt(0);
			
	if (strGBLVar.fGBLvConfirmPrint)
	{
		// Print confirmation -- sidumili
		if (inPrintConfirmation("HOST REPORT", FALSE) == d_OK)
		{
			vdCTOSS_PrinterBMPPicEx(0, 0, "/data/data/pub/Print_BMP.bmp");
			vdCTOSS_PrinterEnd();
		}
		else
		{			
			vdCTOSS_PrinterEnd();
		}
	}
	else
	{
		usCTOSS_LCDDisplay("HOST REPORT|PRINTING...");
		vdCTOSS_PrinterEnd();
	}
	
    vdDebug_LogPrintf("-->>vdCTOS_HostInfo[END]");
}

int ushCTOS_ePadSignature(void)
{
	if (FALSE ==strTCT.fSignatureFlag)
		return d_OK;
	
	ePad_SignatureCaptureLib();
	return d_OK;
}

int ushCTOS_ePadPrintSignature(void)
{
	if (FALSE ==strTCT.fSignatureFlag)
		return d_OK;
	
	ePad_PrintSignatureCaptureLib();
	return d_OK;
}


int ushCTOS_ClearePadSignature(void)
{
	vdDebug_LogPrintf("--ushCTOS_ClearePadSignature--");
	vdDebug_LogPrintf("fSignatureFlag[%d]", strTCT.fSignatureFlag);
		

	if (FALSE ==strTCT.fSignatureFlag)
		return d_OK;
	
	ePad_ClearSignatureCaptureLib();
	return d_OK;
}

USHORT ushCTOS_PrintLoyaltyBody(int page)
{	
    char szStr[d_LINE_SIZE + 1];
    char szTemp[d_LINE_SIZE + 1];
    char szTemp1[d_LINE_SIZE + 1];
    char szTemp3[d_LINE_SIZE + 1];
    char szTemp4[d_LINE_SIZE + 1];
    char szTemp5[d_LINE_SIZE + 1];
    char szFormatPAN[strlen(srTransRec.szPAN) + 7];//to account for space as in PAN format and to include additional PAN no. 7 has been added -Meena 08/01/2012
    int inFmtPANSize;
    BYTE baTemp[PAPER_X_SIZE * 64];
    USHORT result;
    int num,i,inResult;
    unsigned char tucPrint [24*4+1];	
    BYTE   EMVtagVal[64];
    USHORT EMVtagLen; 
    short spacestring;
    BYTE   key;
    struct LOYALTY_DATA stLoyData;
    int inIndex=0;

    if(strTCT.fDemo == 1)
    {
        memset(srTransRec.szLoyaltyData, 0x00, sizeof(srTransRec.szLoyaltyData));
        memcpy(srTransRec.szLoyaltyData, "0000000011110000000022220000000033330000000044400000000555000000006660000000000770000000088800000000999 R", 105);
	}

    memset(&stLoyData, 0x00, sizeof(struct LOYALTY_DATA));
    vdExtractLoyaltyData(&stLoyData, &srTransRec.szLoyaltyData);

	if(CUSTOMER_COPY_RECEIPT == page)
	{
        vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0); 
		if(srTransRec.byTransType == LOY_BAL_INQ)
			vdPrintCenter("POINTS INQUIRY");
		else if(srTransRec.byTransType == LOY_REDEEM_5050 || srTransRec.byTransType == LOY_REDEEM_VARIABLE)
			vdPrintCenter("REAL TIME REWARDS REDEMPT.");
		else if(srTransRec.byTransType == VOID)
			vdPrintCenter("VOID REAL TIME REWARDS REDEMPT.");

        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);	

		vdCTOS_PrinterFline(1);

	    memset (szTemp, 0x00, sizeof(szTemp));
        vdCTOS_FormatPAN(strIIT.szMaskCustomerCopy, srTransRec.szPAN, szTemp, strIIT.fMaskCustCopy);
         inPrintLeftRight("CARD NUMBER", szTemp, 46);
		
 	    TrimTrail(srTransRec.szCardholderName);
        inPrintLeftRight("CARDHOLDER NAME", srTransRec.szCardholderName, 46);
		
		vdCTOS_PrinterFline(1);

		if(srTransRec.byTransType == LOY_BAL_INQ)
		{
            memset(szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szPointBalance, szTemp);
            //sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szSaleAmount)/100), (atol(stLoyData.szSaleAmount)%100)); 
            inPrintLeftRight("POINTS BALANCE", szTemp, 46);
    		
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szEPurseBalance, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemAmountAmount)/100), (atol(stLoyData.szRedeemAmountAmount)%100));
            inPrintLeftRight("PESO VALUE OF POINTS", szTemp, 46);			
		}
		else if(srTransRec.byTransType == LOY_REDEEM_5050 || srTransRec.byTransType == LOY_REDEEM_VARIABLE)
		{
            memset(szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szSaleAmount, szTemp);
            //sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szSaleAmount)/100), (atol(stLoyData.szSaleAmount)%100)); 
            inPrintLeftRight("TOTAL AMOUNT DUE", szTemp, 46);
    		
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemAmountAmount, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemAmountAmount)/100), (atol(stLoyData.szRedeemAmountAmount)%100));
            inPrintLeftRight("AMOUNT CHARGE TO POINTS", szTemp, 46);
    
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szNetSalesAmount, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szNetSalesAmount)/100), (atol(stLoyData.szNetSalesAmount)%100));
            inPrintLeftRight("AMOUNT CHARGE TO C. CARD", szTemp, 46);
     
    		vdCTOS_PrinterFline(1);
    
    		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szBeginPointBalance, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szBeginPointBalance)/100), (atol(stLoyData.szBeginPointBalance)%100));
     		inPrintLeftRight("BEGINNING BALANCE POINTS", szTemp, 46);
    
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemPoints, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemPoints)/100), (atol(stLoyData.szRedeemPoints)%100));
      		inPrintLeftRight("POINTS USED FOR PAYMENT", szTemp, 46);
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            //vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szPointBalance, szTemp);	
    		sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szPointBalance)/100), (atol(stLoyData.szPointBalance)%100));
       		inPrintLeftRight("ENDING BALANCE: POINTS", szTemp, 46);
		}
		else if(srTransRec.byTransType == VOID)
		{
            memset(szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szSaleAmount, szTemp);
            //sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szSaleAmount)/100), (atol(stLoyData.szSaleAmount)%100)); 
            inPrintLeftRight("AMOUNT REVERSED", szTemp, 46);
    		
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemAmountAmount, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemAmountAmount)/100), (atol(stLoyData.szRedeemAmountAmount)%100));
            inPrintLeftRight("AMOUNT REVERSED TO POINTS", szTemp, 46);
    
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szNetSalesAmount, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szNetSalesAmount)/100), (atol(stLoyData.szNetSalesAmount)%100));
            inPrintLeftRight("AMOUNT REVERSED TO CARD", szTemp, 46);
     
    		vdCTOS_PrinterFline(1);
    
    		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szBeginPointBalance, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szBeginPointBalance)/100), (atol(stLoyData.szBeginPointBalance)%100));
     		inPrintLeftRight("BEGINNING BALANCE POINTS", szTemp, 46);
    
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemPoints, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemPoints)/100), (atol(stLoyData.szRedeemPoints)%100));
      		inPrintLeftRight("POINTS REVERSED", szTemp, 46);
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            //vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szPointBalance, szTemp);	
    		sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szPointBalance)/100), (atol(stLoyData.szPointBalance)%100));
       		inPrintLeftRight("ENDING BALANCE: POINTS", szTemp, 46);	
		}
        vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
	else if(MERCHANT_COPY_RECEIPT == page)
	{
        vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0); 
		if(srTransRec.byTransType == LOY_BAL_INQ)
			vdPrintCenter("POINTS INQUIRY");
		else if(srTransRec.byTransType == LOY_REDEEM_5050 || srTransRec.byTransType == LOY_REDEEM_VARIABLE)
			vdPrintCenter("REAL TIME REWARDS REDEMPT.");
		else if(srTransRec.byTransType == VOID)
			vdPrintCenter("VOID REAL TIME REWARDS REDEMPT.");

        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);	

		vdCTOS_PrinterFline(1);

	    memset (szTemp, 0x00, sizeof(szTemp));
        vdCTOS_FormatPAN(strIIT.szMaskMerchantCopy, srTransRec.szPAN, szTemp, strIIT.fMaskMerchCopy);
         inPrintLeftRight("CARD NUMBER", szTemp, 46);
		
 	    TrimTrail(srTransRec.szCardholderName);
        inPrintLeftRight("CARDHOLDER NAME", srTransRec.szCardholderName, 46);
		
        vdCTOS_PrinterFline(1);

		if(srTransRec.byTransType == LOY_BAL_INQ)
		{
            memset(szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szPointBalance, szTemp);
            //sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szSaleAmount)/100), (atol(stLoyData.szSaleAmount)%100)); 
            inPrintLeftRight("POINTS BALANCE", szTemp, 46);
    		
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szEPurseBalance, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemAmountAmount)/100), (atol(stLoyData.szRedeemAmountAmount)%100));
            inPrintLeftRight("PESO VALUE OF POINTS", szTemp, 46);			
		}
		else if(srTransRec.byTransType == LOY_REDEEM_5050 || srTransRec.byTransType == LOY_REDEEM_VARIABLE)
		{
            memset(szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szSaleAmount, szTemp);
            //sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szSaleAmount)/100), (atol(stLoyData.szSaleAmount)%100)); 
            inPrintLeftRight("TOTAL AMOUNT DUE", szTemp, 46);
    		
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemAmountAmount, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemAmountAmount)/100), (atol(stLoyData.szRedeemAmountAmount)%100));
            inPrintLeftRight("AMOUNT CHARGE TO POINTS", szTemp, 46);
    
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szNetSalesAmount, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szNetSalesAmount)/100), (atol(stLoyData.szNetSalesAmount)%100));
            inPrintLeftRight("AMOUNT CHARGE TO C. CARD", szTemp, 46);
     
    		vdCTOS_PrinterFline(1);
    
    		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szBeginPointBalance, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szBeginPointBalance)/100), (atol(stLoyData.szBeginPointBalance)%100));
     		inPrintLeftRight("BEGINNING BALANCE POINTS", szTemp, 46);
    
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemPoints, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemPoints)/100), (atol(stLoyData.szRedeemPoints)%100));
      		inPrintLeftRight("POINTS USED FOR PAYMENT", szTemp, 46);
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            //vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szPointBalance, szTemp);	
    		sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szPointBalance)/100), (atol(stLoyData.szPointBalance)%100));
       		inPrintLeftRight("ENDING BALANCE: POINTS", szTemp, 46);
		}
		else if(srTransRec.byTransType == VOID)
		{
            memset(szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szSaleAmount, szTemp);
            //sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szSaleAmount)/100), (atol(stLoyData.szSaleAmount)%100)); 
            inPrintLeftRight("AMOUNT REVERSED", szTemp, 46);
    		
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemAmountAmount, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemAmountAmount)/100), (atol(stLoyData.szRedeemAmountAmount)%100));
            inPrintLeftRight("AMOUNT REVERSED TO POINTS", szTemp, 46);
    
            memset(szTemp, 0x00, sizeof(szTemp));        
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szNetSalesAmount, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szNetSalesAmount)/100), (atol(stLoyData.szNetSalesAmount)%100));
            inPrintLeftRight("AMOUNT REVERSED TO CARD", szTemp, 46);
     
           vdCTOS_PrinterFline(1);
    
    		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szBeginPointBalance, szTemp);	
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szBeginPointBalance)/100), (atol(stLoyData.szBeginPointBalance)%100));
     		inPrintLeftRight("BEGINNING BALANCE POINTS", szTemp, 46);
    
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szRedeemPoints, szTemp);
    		//sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szRedeemPoints)/100), (atol(stLoyData.szRedeemPoints)%100));
      		inPrintLeftRight("POINTS REVERSED", szTemp, 46);
    
     		memset (szTemp, 0x00, sizeof(szTemp));
            //vdCTOS_FormatAmount("NNN,NNN,NNn.nn", stLoyData.szPointBalance, szTemp);	
    		sprintf(szTemp, "%10lu.%02lu", (atol(stLoyData.szPointBalance)/100), (atol(stLoyData.szPointBalance)%100));
       		inPrintLeftRight("ENDING BALANCE: POINTS", szTemp, 46);	
		}
        vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);	
	}
	return d_OK;	
	
}

USHORT ushCTOS_PrintLoyaltyFooter(int page)
{		
    BYTE   EMVtagVal[64];
    USHORT EMVtagLen; 
	int result;
    char szStr[d_LINE_SIZE + 1];
  	CTOS_RTC SetRTC;
	char szYear[3];
	char szTempDate[d_LINE_SIZE + 1];
    char szTemp[d_LINE_SIZE + 1];
    char szTemp2[d_LINE_SIZE + 1];
    char szTemp3[d_LINE_SIZE + 1];
    char szTemp4[d_LINE_SIZE + 1];

	CTOS_RTCGet(&SetRTC);
	sprintf(szYear ,"%02d",SetRTC.bYear);
	memcpy(srTransRec.szYear,szYear,2);
	
	if(page == CUSTOMER_COPY_RECEIPT)
	{
		memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
		EMVtagLen = 0;
		
		DebugAddINT("ushCTOS_PrintFooter,mode",srTransRec.byEntryMode);  
		if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
		{
		    EMVtagLen = 3;
            memcpy(EMVtagVal, srTransRec.stEMVinfo.T9F34, EMVtagLen);
			if((EMVtagVal[2]& 0x0F) == 2)
            {   
                vdCTOS_PrinterFline(2);
                vdPrintCenter("*****NO SIGNATURE REQUIRED*****");
                if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
                    vdPrintCenter("(PIN VERIFY SUCCESS)");
            }
            else
            {
                //CTOS_PrinterFline(d_LINE_DOT * 3);
            }
		}
        else
        {
            //CTOS_PrinterFline(d_LINE_DOT * 3);
        }

		ushCTOS_ePadPrintSignature();
		
		//CTOS_PrinterPutString("SIGN:_______________________________________");
		//printCardHolderName();
		vdCTOS_PrinterFline(1);
		
		//ushCTOS_PrintAgreement();
		
		if(strlen(strMMT[0].szRctFoot1) > 0)
			vdPrintCenter(strMMT[0].szRctFoot1);
		if(strlen(strMMT[0].szRctFoot2) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot2);
		if(strlen(strMMT[0].szRctFoot3) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot3);

        vdCTOS_PrinterFline(1);

		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);	
		
	    memset(szTemp, 0x00, sizeof(szTemp));
        memset(szTemp2, 0x00, sizeof(szTemp2));
	    memset(szTemp3, 0x00, sizeof(szTemp3));
        memset(szTemp4, 0x00, sizeof(szTemp4));
        wub_hex_2_str(srTransRec.szDate, szTemp,DATE_BCD_SIZE);
        wub_hex_2_str(srTransRec.szTime, szTemp2,TIME_BCD_SIZE);
        memset(szTempDate, 0x00, sizeof(szTempDate));
        sprintf(szTempDate, "%02lu%02lu%02lu", atol(srTransRec.szYear), atol(szTemp)/100, atol(szTemp)%100);
        vdCTOS_FormatDate(szTempDate);
        sprintf(szTemp3, "DATE: %s", szTempDate);
		sprintf(szTemp4, "TIME: %02lu:%02lu:%02lu", atol(szTemp2)/10000,atol(szTemp2)%10000/100, atol(szTemp2)%100);
		inPrintLeftRight(szTemp3, szTemp4, 46);
 
        memset(szTemp, 0x00, sizeof(szTemp));
        memset(szTemp2, 0x00, sizeof(szTemp2));
        sprintf(szTemp, "TID: %s", srTransRec.szTID);
        sprintf(szTemp2, "MID: %s", srTransRec.szMID);
	    inPrintLeftRight(szTemp, szTemp2, 46);
			
	    memset(szTemp, 0x00, sizeof(szTemp));
        memset(szTemp2, 0x00, sizeof(szTemp2));
	    memset(szTemp3, 0x00, sizeof(szTemp3));
        memset(szTemp4, 0x00, sizeof(szTemp4));
		wub_hex_2_str(srTransRec.szBatchNo,szTemp,3);
		wub_hex_2_str(srTransRec.szInvoiceNo, szTemp2, INVOICE_BCD_SIZE);
		sprintf(szTemp3, "BATCH NUM: %s", szTemp);
		sprintf(szTemp4, "TRACE NO.: %s", szTemp2);
	    inPrintLeftRight(szTemp3, szTemp4, 46);
		
	    memset(szStr, 0x00, d_LINE_SIZE);
        sprintf(szStr, "REF NO.   : %s", srTransRec.szRRN);
	    result=inPrint(szStr);
	
	    memset(szStr, 0x00, d_LINE_SIZE);
        sprintf(szStr, "APPR. CODE: %s", srTransRec.szAuthCode);
	    result=inPrint(szStr);

        if(strTCT.fProductCode == TRUE)
        {
            memset(szStr, 0x00, d_LINE_SIZE);
            sprintf(szStr, "PRODUCT CODE: %s", srTransRec.szProductCode);
            result=inPrint(szStr);
        }

        if(strTCT.fCustomerNo == TRUE)
        {
	        memset(szStr, 0x00, d_LINE_SIZE);
            sprintf(szStr, "CUSTOMER NUMBER: %s", srTransRec.szCustomerNo);
	        result=inPrint(szStr);
        }
		
        vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

		if(strHDT.fFooterLogo == TRUE)	
            vdCTOSS_PrinterBMPPic(0, 0, "footer.bmp");
			
		vdCTOS_PrinterFline(1); 
		vdPrintCenter("***** CUSTOMER COPY *****");
	}
	else
	{
		memset(EMVtagVal, 0x00, sizeof(EMVtagVal));
		EMVtagLen = 0;
		
		DebugAddINT("ushCTOS_PrintFooter,mode",srTransRec.byEntryMode);  
		if(srTransRec.byEntryMode==CARD_ENTRY_ICC)
		{
			EMVtagLen = 3;
            memcpy(EMVtagVal, srTransRec.stEMVinfo.T9F34, EMVtagLen);
			if((EMVtagVal[2]& 0x0F) == 2)
            {         
                vdCTOS_PrinterFline(2);
                vdPrintCenter("*****NO SIGNATURE REQUIRED*****");
                if(srTransRec.byEntryMode == CARD_ENTRY_ICC)
                    vdPrintCenter("(PIN VERIFY SUCCESS)");
            }
            else
            {
				if(strCDT.inType != DEBIT_CARD)
				{
					vdCTOS_PrinterFline(2);
					inPrint("SIGN:_______________________________________");
					TrimTrail(srTransRec.szCardholderName);
					vdPrintCenter(srTransRec.szCardholderName); 
				}
            }
		}
        else
        {
			if(strCDT.inType != DEBIT_CARD)
			{
				vdCTOS_PrinterFline(2);
				inPrint("SIGN:_______________________________________");
				TrimTrail(srTransRec.szCardholderName);
				vdPrintCenter(srTransRec.szCardholderName); 
			}
        }

        ushCTOS_ePadPrintSignature();
        
        //CTOS_PrinterPutString("SIGN:_______________________________________");
		//printCardHolderName();

	
		vdCTOS_PrinterFline(1);
		
		//ushCTOS_PrintAgreement();

        if(strlen(strMMT[0].szRctFoot1) > 0)
			vdPrintCenter(strMMT[0].szRctFoot1);
		if(strlen(strMMT[0].szRctFoot2) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot2);
		if(strlen(strMMT[0].szRctFoot3) > 0)
	    	vdPrintCenter(strMMT[0].szRctFoot3);

        vdCTOS_PrinterFline(1);

		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);	
		
	    memset(szTemp, 0x00, sizeof(szTemp));
        memset(szTemp2, 0x00, sizeof(szTemp2));
	    memset(szTemp3, 0x00, sizeof(szTemp3));
        memset(szTemp4, 0x00, sizeof(szTemp4));
        wub_hex_2_str(srTransRec.szDate, szTemp,DATE_BCD_SIZE);
        wub_hex_2_str(srTransRec.szTime, szTemp2,TIME_BCD_SIZE);
        memset(szTempDate, 0x00, sizeof(szTempDate));
        sprintf(szTempDate, "%02lu%02lu%02lu", atol(srTransRec.szYear), atol(szTemp)/100, atol(szTemp)%100);
        vdCTOS_FormatDate(szTempDate);
        sprintf(szTemp3, "DATE: %s", szTempDate);
		sprintf(szTemp4, "TIME: %02lu:%02lu:%02lu", atol(szTemp2)/10000,atol(szTemp2)%10000/100, atol(szTemp2)%100);
		inPrintLeftRight(szTemp3, szTemp4, 46);
 
        memset(szTemp, 0x00, sizeof(szTemp));
        memset(szTemp2, 0x00, sizeof(szTemp2));
        sprintf(szTemp, "TID: %s", srTransRec.szTID);
        sprintf(szTemp2, "MID: %s", srTransRec.szMID);
	    inPrintLeftRight(szTemp, szTemp2, 46);
			
	    memset(szTemp, 0x00, sizeof(szTemp));
        memset(szTemp2, 0x00, sizeof(szTemp2));
	    memset(szTemp3, 0x00, sizeof(szTemp3));
        memset(szTemp4, 0x00, sizeof(szTemp4));
		wub_hex_2_str(srTransRec.szBatchNo,szTemp,3);
		wub_hex_2_str(srTransRec.szInvoiceNo, szTemp2, INVOICE_BCD_SIZE);
		sprintf(szTemp3, "BATCH NUM: %s", szTemp);
		sprintf(szTemp4, "TRACE NO.: %s", szTemp2);
	    inPrintLeftRight(szTemp3, szTemp4, 46);
		
	    memset(szStr, 0x00, d_LINE_SIZE);
        sprintf(szStr, "REF NO.   : %s", srTransRec.szRRN);
	    result=inPrint(szStr);
	
	    memset(szStr, 0x00, d_LINE_SIZE);
        sprintf(szStr, "APPR. CODE: %s", srTransRec.szAuthCode);
	    result=inPrint(szStr);

        if(strTCT.fProductCode == TRUE)
        {
            memset(szStr, 0x00, d_LINE_SIZE);
            sprintf(szStr, "PRODUCT CODE: %s", srTransRec.szProductCode);
            result=inPrint(szStr);
        }

        if(strTCT.fCustomerNo == TRUE)
        {
	        memset(szStr, 0x00, d_LINE_SIZE);
            sprintf(szStr, "CUSTOMER NUMBER: %s", srTransRec.szCustomerNo);
	        result=inPrint(szStr);
        }
		
        vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

		if(strHDT.fFooterLogo == TRUE)	
            vdCTOSS_PrinterBMPPic(0, 0, "footer.bmp");

		vdCTOS_PrinterFline(1); 
		vdPrintCenter("***** MERCHANT COPY *****");
	}

	vdLineFeed(FALSE);
	
return d_OK;	

}

void vdCTOS_PrinterFline(USHORT usLines)
{
	while(usLines > 0)
	{
		inCCTOS_PrinterBufferOutput(ONE_LINE_DOT,&stgFONT_ATTRIB,1);
		usLines--;
	}
}

#if 0
int vdCTOS_PrintReceiptCopies(BOOL BankCopy, BOOL CustCopy, BOOL MercCopy)
{
    BOOL fFirstReceipt=TRUE; 
    int inReceiptCtr=0;
    BYTE szPrintOption[3+1];
    BYTE chPrintOption=0x00;
	BYTE  key;
	
    memset(szPrintOption, 0x00, sizeof(szPrintOption)); 
    memcpy(szPrintOption, strTCT.szPrintOption, 3); 
	inReceiptCtr=0;
	fFirstReceipt=TRUE;

	if (isCheckTerminalMP200() == d_OK)
	{
		inCTOS_LoadCDTandIIT();
		inMMTReadRecord(srTransRec.HDTid,srTransRec.MITid);
		
		// Minimize the white screend display -- sidumili
		vdCTOS_DispStatusMessage("PROCESSING...");

		vdCTOSS_PrinterStart(100);
		CTOS_PrinterSetHeatLevel(4);
		ushCTOS_printAll(BANK_COPY_RECEIPT);
		vdCTOSS_PrinterEnd(); // For fast printing																	
    	CTOS_KBDBufFlush();//cleare key buffer
		inCTOSS_ERM_Form_Receipt(0);	

		return d_OK;
	}
	
    do
		    {
		        chPrintOption=(unsigned char)szPrintOption[inReceiptCtr];
		        if((chPrintOption == '1') && (BankCopy == TRUE)) /*bank copy*/
		        {
					if(fFirstReceipt == TRUE)
					{
						vdCTOSS_PrinterStart(100);
						CTOS_PrinterSetHeatLevel(4);
						ushCTOS_printAll(BANK_COPY_RECEIPT);
						CTOS_KBDBufFlush();	
						fFirstReceipt=FALSE;
						if(strTCT.fPrintReceipt == TRUE)
						{
							inCTOS_DisplayPrintBMP();
							vdCTOSS_PrinterEnd();
						}
					}
					else
					{
						CTOS_TimeOutSet(TIMER_ID_1, UI_TIMEOUT);  
						//clearLine(7);
						CTOS_LCDTClearDisplay();
						setLCDPrint(7, DISPLAY_POSITION_CENTER, "PRINT BANK COPY?");
						setLCDPrint(8, DISPLAY_POSITION_CENTER, "NO[X] YES[OK]");
		                while(1)
		                { 
		                	if(strTCT.fPrintReceipt != TRUE)
	                        break;
							
		                    //if (strTCT.fPrintWarningSound == TRUE)
		                    vduiWarningSound();
		                    
		                    CTOS_KBDHit(&key);
		                    if(key == d_KBD_ENTER)
		                    {
		                    	vdCTOSS_PrinterStart(100);
								CTOS_PrinterSetHeatLevel(4);
		                        ushCTOS_printAll(BANK_COPY_RECEIPT);
		                        CTOS_KBDBufFlush();//cleare key buffer
		                        if(strTCT.fPrintReceipt == TRUE)
		                        {
		                            inCTOS_DisplayPrintBMP();
		                            vdCTOSS_PrinterEnd();
		                        }
		                        break;
		                    }
		                    else if((key == d_KBD_CANCEL))
		                    {
		                        CTOS_LCDTClearDisplay();
		                        break;
		                    }
		                    if(CTOS_TimeOutCheck(TIMER_ID_1) == d_YES)
		                    {
		                    	vdCTOSS_PrinterStart(100);
								CTOS_PrinterSetHeatLevel(4);
								ushCTOS_printAll(BANK_COPY_RECEIPT);
								if(strTCT.fPrintReceipt == TRUE)
		                        {
		                            inCTOS_DisplayPrintBMP();
		                            vdCTOSS_PrinterEnd();
		                        }
		                        break;
		                    }
		                }
					}
		        }
		        else if((chPrintOption == '2') && (CustCopy == TRUE)) /*customer copy*/
		        {
					if(fFirstReceipt == TRUE)
					{
						vdCTOSS_PrinterStart(100);
						CTOS_PrinterSetHeatLevel(4);
						ushCTOS_printAll(CUSTOMER_COPY_RECEIPT);
						CTOS_KBDBufFlush();//cleare key buffer	
						fFirstReceipt=FALSE;
						if(strTCT.fPrintReceipt == TRUE)
						{
							inCTOS_DisplayPrintBMP();
							vdCTOSS_PrinterEnd();
						}
					}
					else
					{
						CTOS_TimeOutSet(TIMER_ID_1, UI_TIMEOUT);  
						//clearLine(7);
						CTOS_LCDTClearDisplay();
						setLCDPrint(7, DISPLAY_POSITION_CENTER, "PRINT CUSTOMER COPY?");
						setLCDPrint(8, DISPLAY_POSITION_CENTER, "NO[X] YES[OK]");
		                while(1)
		                { 
		                	if(strTCT.fPrintReceipt != TRUE)
	                        break;	
							
		                    //if (strTCT.fPrintWarningSound == TRUE)
		                    vduiWarningSound();
		                    
		                    CTOS_KBDHit(&key);
		                    if(key == d_KBD_ENTER)
		                    {
		                    	vdCTOSS_PrinterStart(100);
								CTOS_PrinterSetHeatLevel(4);
		                        ushCTOS_printAll(CUSTOMER_COPY_RECEIPT);
		                        CTOS_KBDBufFlush();//cleare key buffer
		                        if(strTCT.fPrintReceipt == TRUE)
		                        {
		                            inCTOS_DisplayPrintBMP();
		                            vdCTOSS_PrinterEnd();
		                        }
		                        break;
		                    }
		                    else if((key == d_KBD_CANCEL))
		                    {
		                        CTOS_LCDTClearDisplay();
		                        break;
		                    }
		                    if(CTOS_TimeOutCheck(TIMER_ID_1) == d_YES)
		                    {
		                    	vdCTOSS_PrinterStart(100);
								CTOS_PrinterSetHeatLevel(4);
								ushCTOS_printAll(CUSTOMER_COPY_RECEIPT);
								if(strTCT.fPrintReceipt == TRUE)
		                        {
		                            inCTOS_DisplayPrintBMP();
		                            vdCTOSS_PrinterEnd();
		                        }
		                        break;
		                    }
		                }
					}
					
		        }  	
		        else if((chPrintOption == '3') && (MercCopy== TRUE)) /*merchant copy*/
		        {
					if(fFirstReceipt == TRUE)
					{
						vdCTOSS_PrinterStart(100);
						CTOS_PrinterSetHeatLevel(4);
						ushCTOS_printAll(MERCHANT_COPY_RECEIPT);
						CTOS_KBDBufFlush();//cleare key buffer	
						fFirstReceipt=FALSE;
						if(strTCT.fPrintReceipt == TRUE)
                        {
                            inCTOS_DisplayPrintBMP();
                            vdCTOSS_PrinterEnd();
                        }
					}
					else
					{
						CTOS_TimeOutSet(TIMER_ID_1, UI_TIMEOUT);  
						//clearLine(7);				
						CTOS_LCDTClearDisplay();
						setLCDPrint(7, DISPLAY_POSITION_CENTER, "PRINT MERCHANT COPY?");
						setLCDPrint(8, DISPLAY_POSITION_CENTER, "NO[X] YES[OK]");
		                while(1)
		                { 
		                	if(strTCT.fPrintReceipt != TRUE)
	                        break;	
							
		                    //if (strTCT.fPrintWarningSound == TRUE) /*BDO PHASE 2: [Warning sound for printing flag] -- sidumili*/
		                    vduiWarningSound();
		                    
		                    CTOS_KBDHit(&key);
		                    if(key == d_KBD_ENTER)
		                    {
		                    	vdCTOSS_PrinterStart(100);
								CTOS_PrinterSetHeatLevel(4);
		                        ushCTOS_printAll(MERCHANT_COPY_RECEIPT);
		                        CTOS_KBDBufFlush();//cleare key buffer
		                        if(strTCT.fPrintReceipt == TRUE)
		                        {
		                            inCTOS_DisplayPrintBMP();
		                            vdCTOSS_PrinterEnd();
		                        }
		                        break;
		                    }
		                    else if((key == d_KBD_CANCEL))
		                    {
		                        CTOS_LCDTClearDisplay();
		                        break;
		                    }
		                    if(CTOS_TimeOutCheck(TIMER_ID_1) == d_YES)
		                    {
		                    	vdCTOSS_PrinterStart(100);
								CTOS_PrinterSetHeatLevel(4);
								ushCTOS_printAll(MERCHANT_COPY_RECEIPT);
								if(strTCT.fPrintReceipt == TRUE)
		                        {
		                            inCTOS_DisplayPrintBMP();
		                            vdCTOSS_PrinterEnd();
		                        }
		                        break;
		                    }
		                }
					}
		        }  		
				inReceiptCtr++; 
		    }while(inReceiptCtr < 3);

	vdCheckERMMode(); // Enhancement for printing speed -- sidumili
	
}
#else
int vdCTOS_PrintReceiptCopies(BOOL BankCopy, BOOL CustCopy, BOOL MercCopy)
{
    BOOL fFirstReceipt=TRUE; 
    int inReceiptCtr=0;
    BYTE szPrintOption[3+1];
    BYTE chPrintOption=0x00;
	BYTE  key;
	int inKey;
	
	BYTE szTitle[20+1];
	BYTE szPrintDisplay[100];
	BOOL fOrientation = (BOOL)get_env_int("#ORIENTATION");
	BOOL fNextCopy = TRUE;

	vdDebug_LogPrintf("TINE: S1_BANCNET --vdCTOS_PrintReceiptCopies--");
	vdDebug_LogPrintf("--vdCTOS_PrintReceiptCopies--");
	vdDebug_LogPrintf("BankCopy[%d], CustCopy[%d], MercCopy[%d]", BankCopy, CustCopy, MercCopy);
	vdDebug_LogPrintf("byTerminalType[%d], byTerminalModel[%d]", strTCT.byTerminalType, strTCT.byTerminalModel);
	vdDebug_LogPrintf("byERMMode[%d]", strTCT.byERMMode);
	vdDebug_LogPrintf("fOrientation[%d]", fOrientation);

	memset(szTitle, 0x00, sizeof(szTitle));
    szGetTransTitle(srTransRec.byTransType, szTitle);
	
    memset(szPrintOption, 0x00, sizeof(szPrintOption)); 
    memcpy(szPrintOption, strTCT.szPrintOption, 3); 
	inReceiptCtr=0;
	fFirstReceipt=TRUE;
	
	if (isCheckTerminalMP200() == d_OK)
	{
		// Minimize the white screend display -- sidumili
		vdCTOS_DispStatusMessage("PROCESSING");
		
		ushCTOS_printAll(BANK_COPY_RECEIPT);
		vdCTOSS_PrinterEnd(); // For fast printing																	
    	CTOS_KBDBufFlush();//cleare key buffer
		inCTOSS_ERM_Form_Receipt(0);	
	}

	if (isCheckTerminalMP200() != d_OK && fOrientation == TRUE)
	{
		vdCTOS_DispStatusMessage("PROCESSING");
		
		fERMTransaction = TRUE;
		vdCTOSS_PrinterStart(500);
		ushCTOS_printAll(BANK_COPY_RECEIPT);
		vdCTOSS_PrinterEnd_CConvert2BMP("/data/data/pub//Print_BMP.bmp");
    	CTOS_KBDBufFlush();
		inCTOSS_ERM_Form_Receipt(0);
	}	
	//else
	{
	    do
		    {
		        chPrintOption=(unsigned char)szPrintOption[inReceiptCtr];
		        if((chPrintOption == '1') && (BankCopy == TRUE)) /*bank copy*/
		        {
					if(fFirstReceipt == TRUE)
					{
					    vdDebug_LogPrintf("saturn print bank");
						ushCTOS_printAll(BANK_COPY_RECEIPT);
						CTOS_KBDBufFlush();	
						fFirstReceipt=FALSE;
						//CTOS_Delay(1000);
					}
					else
					{
						CTOS_TimeOutSet(TIMER_ID_1, UI_TIMEOUT);  
						//clearLine(7);
						vdDebug_LogPrintf("saturn print bank copy");						
		                while(1) {
                            if (strTCT.fPrintReceipt != TRUE)
                                break;

                            if (strTCT.feReceiptEnable == 0) {
                                memset(szPrintDisplay, 0x00, sizeof(szPrintDisplay));
                                strcat(szPrintDisplay, "BANK COPY");

                                fNextCopy = usCTOSS_Confirm(szPrintDisplay);
                                vdDebug_LogPrintf("next copy %d", fNextCopy);

                                vduiWarningSound();

                                //if(inKey == d_OK)
                                if (fNextCopy == d_OK) {
                                    ushCTOS_printAll(BANK_COPY_RECEIPT);
                                    CTOS_KBDBufFlush();//cleare key buffer
                                    //CTOS_Delay(1000);
                                    break;
                                } else if ((fNextCopy == d_USER_CANCEL)) {
                                    //CTOS_LCDTClearDisplay();
                                    break;
                                }
                                if (CTOS_TimeOutCheck(TIMER_ID_1) == d_YES) {
                                    ushCTOS_printAll(BANK_COPY_RECEIPT);
									//CTOS_Delay(1000);
                                    break;
                                }
                            } else {
                                ushCTOS_printAll(BANK_COPY_RECEIPT);
                                CTOS_KBDBufFlush();
								//CTOS_Delay(1000);
                                break;
                            }
                        }
					}
		        }
		        else if((chPrintOption == '2') && (CustCopy == TRUE)) /*customer copy*/
		        {
					if(fFirstReceipt == TRUE)
					{
                        printcopies_cntr = 0;
					    ushCTOS_printAll(CUSTOMER_COPY_RECEIPT);
						CTOS_KBDBufFlush();//cleare key buffer	
						fFirstReceipt=FALSE;
						//CTOS_Delay(1000);
					}
					else
					{
						CTOS_TimeOutSet(TIMER_ID_1, UI_TIMEOUT);  
						vdDebug_LogPrintf("saturn print customer");						
		                while(1)
		                { 
		                	if(strTCT.fPrintReceipt != TRUE)
	                        break;

                            if(strTCT.feReceiptEnable == 0)
                            {
                                memset(szPrintDisplay,0x00,sizeof(szPrintDisplay));
                                strcat(szPrintDisplay, "CUSTOMER COPY");

                                fNextCopy = usCTOSS_Confirm(szPrintDisplay);

                                vduiWarningSound();

                                if(fNextCopy == d_OK)
                                {
                                    ushCTOS_printAll(CUSTOMER_COPY_RECEIPT);
                                    CTOS_KBDBufFlush();//cleare key buffer
                                    //CTOS_Delay(1000);
                                    break;
                                }
                                else if((fNextCopy == d_USER_CANCEL))
                                {
                                    //CTOS_LCDTClearDisplay();
                                    break;
                                }
                                if(CTOS_TimeOutCheck(TIMER_ID_1) == d_YES)
                                {
                                    ushCTOS_printAll(CUSTOMER_COPY_RECEIPT);
									//CTOS_Delay(1000);
                                    break;
                                }
                            }
                            else
                            {
                                ushCTOS_printAll(CUSTOMER_COPY_RECEIPT);
                                CTOS_KBDBufFlush();//cleare key buffer
                                //CTOS_Delay(1000);
                                break;
                            }

							
		                }
					}
					
		        }  	
		        else if((chPrintOption == '3') && (MercCopy== TRUE)) /*merchant copy*/
		        {
					if(fFirstReceipt == TRUE)
					{
						ushCTOS_printAll(MERCHANT_COPY_RECEIPT);
						CTOS_KBDBufFlush();//cleare key buffer	
						fFirstReceipt=FALSE;
						//CTOS_Delay(1000);
					}
					else
					{
					  	CTOS_TimeOutSet(TIMER_ID_1, UI_TIMEOUT);  
					    vdDebug_LogPrintf("saturn print merchant copy");
		                while(1)
		                { 
		                	if(strTCT.fPrintReceipt != TRUE)
	                        break;

                            if(strTCT.feReceiptEnable == 0)
                            {
                                memset(szPrintDisplay,0x00,sizeof(szPrintDisplay));
                                strcat(szPrintDisplay, "MERCHANT COPY");
                                fNextCopy = usCTOSS_Confirm(szPrintDisplay);
                                vduiWarningSound();

                                //CTOS_KBDHit(&key);
                                if (fNextCopy == d_OK) {
                                    ushCTOS_printAll(MERCHANT_COPY_RECEIPT);
                                    CTOS_KBDBufFlush();//cleare key buffer
                                    //CTOS_Delay(1000);
                                    break;
                                } else if ((fNextCopy == d_USER_CANCEL)) {
                                    //CTOS_LCDTClearDisplay();
                                    break;
                                }
                                if (CTOS_TimeOutCheck(TIMER_ID_1) == d_YES) {
                                    ushCTOS_printAll(MERCHANT_COPY_RECEIPT);
									//CTOS_Delay(1000);
                                    break;
                                }
                            }
                            else
                            {
                                ushCTOS_printAll(MERCHANT_COPY_RECEIPT);
                                CTOS_KBDBufFlush();//cleare key buffer
                                //CTOS_Delay(1000);
                                break;
                            }
		                }
					}
		        }
                usCTOSS_LCDDisplay(" ");
		        inReceiptCtr++;
                printcopies_cntr = inReceiptCtr;
		    }while(inReceiptCtr < 3);
	}

	vdCheckERMMode(); // Enhancement for printing speed -- sidumili
	
}

#endif

void vdPrintISOOption(unsigned char *pucTitle,unsigned char *uszData, int inLen)
{
	BYTE szTitle[16 + 1] = {0};
	
	if(strTCT.inPrintISOOption == 1)
	{
		if(memcmp(pucTitle,"RX",2) == 0 && strCPT.inCommunicationMode == DIAL_UP_MODE)
			inPrintISOPacket(pucTitle , uszData, inLen-2);
		else
			inPrintISOPacket(pucTitle , uszData, inLen);
	}
	else if(strTCT.inPrintISOOption == 2)
	{
		szGetTransTitle(srTransRec.byTransType, szTitle);
		vdPrintParseISO(szTitle,pucTitle ,uszData+5);
	}
	else if(strTCT.inPrintISOOption == 3)
	{
		if(memcmp(pucTitle,"RX",2) == 0 && strCPT.inCommunicationMode == DIAL_UP_MODE)
			inPrintISOPacket(pucTitle , uszData, inLen-2);
		else
			inPrintISOPacket(pucTitle , uszData, inLen);

		szGetTransTitle(srTransRec.byTransType, szTitle);
		vdPrintParseISO(szTitle,pucTitle ,uszData+5);
		
	}

}

int inPrintISOPacket(unsigned char *pucTitle,unsigned char *pucMessage, int inLen)
{
    char ucLineBuffer[d_LINE_SIZE+4];
    unsigned char *pucBuff;
    int inBuffPtr = 0;
    BYTE baTemp[PAPER_X_SIZE * 64];
    char szStr[d_LINE_SIZE + 4];

	//vdDebug_LogPrintf("inPrintISOPacket | fERMTransaction[%d]", fERMTransaction);

    if (inLen <= 0)
        return(ST_SUCCESS);

	vdCTOSS_PrinterStart(100);
    CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);
	
	//CTOS_PrinterSetWorkTime(50000,1000);
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_16x16,0," ");
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
    
    memset(szStr, 0x00, sizeof(szStr));
	memset(baTemp, 0x00, sizeof(baTemp));
    sprintf(szStr,"[%s] [%d] \n", pucTitle, inLen);
	inCCTOS_PrinterBufferOutput(szStr,&stgFONT_ATTRIB, fERMTransaction);

	vdCTOS_PrinterFline(1);
    
    pucBuff = pucMessage + inLen;
    while (pucBuff > pucMessage)
    {
    	memset(ucLineBuffer,0x00, sizeof(ucLineBuffer));
	    for (inBuffPtr = 0; (inBuffPtr < 44) && (pucBuff > pucMessage); inBuffPtr += 3)
	    {
		    sprintf(&ucLineBuffer[inBuffPtr], "%02X ", *pucMessage);
		    pucMessage++;
	    }
	    ucLineBuffer[44] = '\n';
	    memset (baTemp, 0x00, sizeof(baTemp));		
		inCCTOS_PrinterBufferOutput(ucLineBuffer,&stgFONT_ATTRIB, fERMTransaction);
    
    } 

	vdCTOS_PrinterFline(1);
    
    vdCTOSS_PrinterEnd();
    return (ST_SUCCESS);
}

void SetReportTime(void)
{
	BYTE szCurrTime[7] = {0};
	CTOS_RTC rtcClock;
	CTOS_RTC SetRTC;

	CTOS_RTCGet(&SetRTC);

	sprintf(szCurrTime,"%02d%02d%02d", SetRTC.bHour, SetRTC.bMinute, SetRTC.bSecond);

	wub_str_2_hex(szCurrTime,srTransRec.szTime,6);
		 
}

void vdBackUpReportTime(void)
{
	SetReportTime();
    memset(szgTempDate, 0x00, sizeof(szgTempDate));
    memset(szgTempTime, 0x00, sizeof(szgTempTime));
	memcpy(szgTempDate, srTransRec.szDate, DATE_BCD_SIZE);
	memcpy(szgTempTime, srTransRec.szTime, TIME_BCD_SIZE);

}

void vdRestoreReportTime(void)
{
	memcpy(srTransRec.szDate, szgTempDate, DATE_BCD_SIZE);
	memcpy(srTransRec.szTime, szgTempTime,  TIME_BCD_SIZE);
}

int isCheckTerminalMP200(void) 
{
    if ((strTCT.byTerminalType == 5) || (strTCT.byTerminalType == 6))
        return d_OK;
    return d_NO;
}

void vdComputeGetAppCRC(char *szAppName, char *ptrCRC, BOOL fMP200)
{
	char szStr[40 + 1];	
	FILE* file;
	int nLen;
	unsigned char chBuffer[1024];
	BOOL fAppExist = FALSE;

	vdDebug_LogPrintf("--vdComputeGetAppCRC--");
	
	memset(szStr, 0x00, sizeof(szStr));
	memset(chBuffer, 0, 1024);

	strcpy(szAppName, "V5S_MCCPAY");
	
	if ((file = fopen (szAppName, "rb")) != NULL)
	{
		fAppExist = TRUE;
		
	    while (nLen = fread (chBuffer, 1, 256, file))
	    {
	    }

	    sprintf(szStr, "%02x%02x%02x%02x", wub_lrc((unsigned char *) &chBuffer[0], 64),
	    wub_lrc((unsigned char *) &chBuffer[63], 64),
	    wub_lrc((unsigned char *) &chBuffer[127], 64),
	    wub_lrc((unsigned char *) &chBuffer[191], 64));	            
	    fclose (file);
	}

	// hardcode crc
	//strcpy(szStr, "43767a70");

	// Put CRC to ENV
	put_env_char(szAppName, szStr);
	
	if (fAppExist)
	{
		if (fMP200)
			sprintf(ptrCRC, "%s", szStr);
		else	
			sprintf(ptrCRC, "CRC: %s", szStr);
	}
	else
		strcpy(ptrCRC, "N/A");

	vdDebug_LogPrintf("szAppName[%s] | ptrCRC[%s] | fAppExist[%d] | fMP200[%d]", szAppName, ptrCRC, fAppExist, fMP200);
	
}

void vdLineFeed(BOOL fFooterLogo)
{
	int inLineFeed;
/*
	if(strTCT.byTerminalModel == V3_DESKTOP)
		inLineFeed=((fFooterLogo)?6:7);
	else if(strTCT.byTerminalModel == V3_MOBILE)
		inLineFeed=((fFooterLogo)?2:3);
	else
		inLineFeed=((fFooterLogo)?4:5);//for V5
*/	
	inLineFeed=((fFooterLogo)?3:4);
	vdCTOS_PrinterFline(inLineFeed);
}


// Put CRC value to ENV -- sidumili
void vdPutEnvCRC(void)
{
    unsigned char szCRC[8+1];	

	memset(szCRC, 0x00, sizeof(szCRC));
	
    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
    	vdGetCRC();
        inCTOS_MultiAPALLAppEventID(d_IPC_CMD_PUT_ENV_CRC);       
    }
    else
    {
        vdGetCRC();
    }
}

int inCTOS_DisplayPrintBMP(void)
{

	// sidumili: Cleanup for S1 terminal which not application
	return d_OK;
	
	vdDebug_LogPrintf("--inCTOS_DisplayPrintBMP--");
	vdDebug_LogPrintf("fImageAnimated[%d]", strTCT.fImageAnimated);
	vdDebug_LogPrintf("byTerminalType[%d]", strTCT.byTerminalType);

	// Do not display printing image -- sidumili
	if (isCheckTerminalMP200() == d_OK && isERMMode() == TRUE)
		return d_OK;
	
	if (strTCT.fImageAnimated == TRUE)
	{
		if ((strTCT.byTerminalType % 2) == 0)
			vdDisplayAnimateBmp(0, 0, "Print1.bmp", "Print2.bmp", "Print3.bmp", NULL, NULL);
		else
			vdDisplayAnimateBmp(0, 0, "Print4.bmp", "Print5.bmp", "Print6.bmp", NULL, NULL);
	}
	else
	{
		if ((strTCT.byTerminalType % 2) == 0)
			displayAppbmpDataEx(0, 0, "Print1.bmp", TRUE);
		else
			displayAppbmpDataEx(0, 0, "Print4.bmp", TRUE);
	}

	vdCTOS_DispStatusMessage("PRINTING...");
		
}

void vdPrintCRC(void)
{
	BYTE szCRC[50];
	BYTE szStr[50];
	BYTE szAPName[50];
	BYTE szPrintCRC[50] = {0};
	 
	memset(szCRC, 0x00, sizeof(szCRC));
	memset(szStr, 0x00, sizeof(szStr));
	memset(szPrintCRC, 0x00, sizeof(szPrintCRC));
	memset(szAPName, 0x00, sizeof(szAPName));

	//inCTOSS_GetEnvDB("V5S_MCCPAY_CRC", szCRC);
#if 1//Comment to harcode crc
	getAppPackageInfo("com.Source.S1_MCCPAY.MCCPAY", szCRC);
#else
    
	memset(szCRC, 0x00, sizeof(szCRC));
    strcpy(szCRC, "0065A19CD");
#endif

	vdDebug_LogPrintf("szCRC[%s]", szCRC);

	strcpy(szAPName, "QR PAY      ");
	memset(szPrintCRC, 0x00, sizeof(szPrintCRC));
	sprintf(szPrintCRC, "%s: %s", szAPName, szCRC);
	//sprintf(szPrintCRC, "%s: %s", szAPName, szCRC);
	inPrint(szPrintCRC);

}




int iXMLPrintWechatHeader(void)
{
	char szLogoPath[50+1];
	
	vdDebug_LogPrintf("--iXMLPrintWechatHeader--");
	
	// sidumili:Comment not to print a diplicate header logo
	#if 0
	//print Logo	
	if(strlen(strHDT.szHeaderLogoName))
		vdCTOSS_PrinterBMPPic(0, 0, strHDT.szHeaderLogoName);
	else
		vdCTOSS_PrinterBMPPic(0, 0, "logo.bmp");
	#endif

	if (fReprintLastSettleFlag==1)
		vdCTOSS_PrinterBMPPic(0, 0, strHDT.szHeaderLogoName);
			
	if(fRePrintFlag == TRUE)
	{	
		vdDebug_LogPrintf("srTransRec.HDTid[%d] srTransRec.MITid[%d]",srTransRec.HDTid, srTransRec.MITid);
		if ( inMMTReadRecord(srTransRec.HDTid,srTransRec.MITid) != d_OK)
		{
			vdSetErrorMessage("LOAD MMT ERR");
			return(d_NO);
		}
		vdDebug_LogPrintf("srTransRec.HDTid[%d] srTransRec.MITid[%d] strMMT[0].szRctHdr1[%s]",srTransRec.HDTid, srTransRec.MITid, strMMT[0].szRctHdr1);
	}
	
	if(strlen(strMMT[0].szRctHdr1) <= 0)
		inMMTReadRecord(1,1);
		
	//merhcant name and address
    //vdPrintHeaderText();
    vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
	//merhcant name and address
	if(strlen(strMMT[0].szRctHdr1) > 0)
		vdPrintCenter(strMMT[0].szRctHdr1);
	if(strlen(strMMT[0].szRctHdr2) > 0)
		vdPrintCenter(strMMT[0].szRctHdr2);
	if(strlen(strMMT[0].szRctHdr3) > 0)
    	vdPrintCenter(strMMT[0].szRctHdr3);
	if(strlen(strMMT[0].szRctHdr4) > 0)
    	vdPrintCenter(strMMT[0].szRctHdr4);
	if(strlen(strMMT[0].szRctHdr5) > 0)
    	vdPrintCenter(strMMT[0].szRctHdr5);

	//issue-00420
	//if (strlen(strTCT.szAppVersionHeader) > 0)
	//	vdPrintCenter(strTCT.szAppVersionHeader);
  	
	//vdCTOS_PrinterFline(1);

	#if 0
	vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
    vdPrintCenter("RECEIPT");
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);
	#endif

	if(fRePrintFlag == TRUE)
    {
        vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
        vdPrintCenter("DUPLICATE");
        vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 1, 0);
    }
	
	vdCTOS_PrinterFline(1);
	
	return d_OK;
}

int iXMLPrintWechatBody(int page)
{
	char szTemp[d_LINE_SIZE + 1] = {0};
	char szTemp4[d_LINE_SIZE + 1] = {0};
	char szTemp5[d_LINE_SIZE + 1] = {0};
	char szAmountBuff[d_LINE_SIZE + 1] = {0};
	char szTrxnDateTime[20 + 1] = {0};
	
	vdDebug_LogPrintf("--iXMLPrintWechatBody--");
	vdDebug_LogPrintf("page[%d]", page);
	vdDebug_LogPrintf("srTransRec.HDTid[%d]", srTransRec.HDTid);

	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);

	#if 0
	// Merchant Name
	memset(szTemp, 0x00, sizeof(szTemp));
	inPrint("MERCHANT NAME:");
	//memset(&strSingleNMT,0x00,sizeof(STRUCT_NMT));
	//inNMTReadRecord(srTransRec.MITid);
	sprintf(szTemp, "%s", strMMT[0].szRctHdr1);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	// Issuer ID
	//memset(szTemp, 0x00, sizeof(szTemp));
	//sprintf(szTemp, "ISSUER ID: %d", strHDT.inIssuerID);
	//inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);
	
	// MID
	memset(szTemp, 0x00, sizeof(szTemp));
	sprintf(szTemp, "MID: %s", srTransRec.szMID);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	// TID
	//memset(szTemp, 0x00, sizeof(szTemp));
	//sprintf(szTemp, "TID: %s", srTransRec.szTID);
	//inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	// InvoiceNo
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	wub_hex_2_str(srTransRec.szInvoiceNo, szTemp4, INVOICE_BCD_SIZE);	
	sprintf(szTemp, "INVOICE NO.: %s", szTemp4);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);
	
	// Transaction Time
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTrxnDateTime, 0x00, sizeof(szTrxnDateTime));
	inPrint("TRANSACTION TIME:");
	iXMLFormatDateTime((char*)srTransRec.szXMLtime_end, szTrxnDateTime);
	sprintf(szTemp, "%s", szTrxnDateTime);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);
	
	// Order ID
	//inPrint("ORDER ID:");
	//memset(szTemp, 0x00, sizeof(szTemp));
	//sprintf(szTemp, "%s", srTransRec.szXMLout_trade_no);
	//inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	// Wechat Transaction ID
	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		inPrint("ORDER ID:");

	if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		inPrint("PLATFORM ORDER ID:");
	
	memset(szTemp, 0x00, sizeof(szTemp));
	sprintf(szTemp, "%s", srTransRec.szXMLtransaction_id);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);
	
	// Wechat Pay Order ID
	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		inPrint("WECHAT PAY ORDER ID:");

	if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		inPrint("ALIPAY ORDER ID:");
	
	memset(szTemp, 0x00, sizeof(szTemp));
	sprintf(szTemp, "%s", srTransRec.szXMLout_transaction_id);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);	

	// Payment Method
	memset(szTemp, 0x00, sizeof(szTemp));
	inPrint("PAYMENT METHOD:");

	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		inPrint("WECHAT-QR PAY");

	if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		inPrint("TRANSACTION QR CODE");
			
	// Status of Transaction
	inPrint("STATUS OF TRANSACTION:");
	if (srTransRec.inXMLstatus != -1)
		inPrint("SUCCESSFUL");

	vdDebug_LogPrintf("szHostLabel[%s].szXMLcash_fee_type[%s].szXMLcash_fee[%s]", srTransRec.szHostLabel, srTransRec.szXMLcash_fee_type, srTransRec.szXMLcash_fee);
	vdDebug_LogPrintf("szHostLabel[%s].szXMLlocal_fee_type[%s].szXMLlocal_total_fee[%s]", srTransRec.szHostLabel, srTransRec.szXMLlocal_fee_type, srTransRec.szXMLlocal_total_fee);
		
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	memset(szAmountBuff, 0x00, sizeof(szAmountBuff));

	// Total	
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	memset(szAmountBuff, 0x00, sizeof(szAmountBuff));
	wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTemp4);
	sprintf(szAmountBuff,"%s%s",srTransRec.szXMLlocal_fee_type, szTemp4);
	sprintf(szTemp, "TOTAL: %s", szAmountBuff);			
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);
	
	// Conversion
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", srTransRec.szXMLcash_fee, szTemp4);
	sprintf(szTemp,"%s%s",srTransRec.szXMLcash_fee_type, szTemp4);		
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);
	
	// Print QRCode
	#endif
	
	vdPrintTIDMIDVersion();
		
    //vdCTOS_PrinterFline(1); // request to remove extra newline - sidumili

	vdPrintTrxnType(FALSE);

	vdPrintBatchNoInvoiceNo();
	
	vdPrintDateTime();

	//vdCTOS_PrinterFline(1); // request to remove extra newline -- sidumili
	
	// Wechat Transaction ID
	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		inPrint("ORDER ID:");
	else if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		inPrint("PLATFORM ORDER ID:");
	else
		inPrint("ORDER ID:");
	
	memset(szTemp, 0x00, sizeof(szTemp));
	sprintf(szTemp, "%s", srTransRec.szXMLtransaction_id);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	// Wechat Pay Order ID
	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		inPrint("WECHAT PAY ORDER ID:");

	if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		inPrint("ALIPAY ORDER ID:");

	if (srTransRec.HDTid == GCASH_HOST_INDEX)
		inPrint("GCASH ORDER ID:");

	if (srTransRec.HDTid == GRAB_HOST_INDEX)
		inPrint("GRAB ORDER ID:");

	if (srTransRec.HDTid == UPI_HOST_INDEX)
		inPrint("UPI ORDER ID:");
	
	memset(szTemp, 0x00, sizeof(szTemp));
	sprintf(szTemp, "%s", srTransRec.szXMLout_transaction_id);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	if (srTransRec.byTransType == VOID)
	{
		inPrint("REFUND ID:");
		inPrint(srTransRec.szRefundID);
	}
	
	vdCTOS_PrinterFline(1);

	vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
	// Total	
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	memset(szTemp5, 0x00, sizeof(szTemp5));
	memset(szAmountBuff, 0x00, sizeof(szAmountBuff));
	wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTemp4);
	//sprintf(szAmountBuff,"%s %s",srTransRec.szXMLlocal_fee_type, szTemp4);

	if(srTransRec.byTransType == SALE)
		sprintf(szTemp5, "%s%s", srTransRec.szXMLlocal_fee_type, szTemp4);

	if(srTransRec.byTransType == VOID)
		sprintf(szTemp5, "%s-%s", srTransRec.szXMLlocal_fee_type, szTemp4);

	inPrintLeftRight("TOTAL:",szTemp5,24);

	// Print when has value of conversion -- sidumili
	if (strlen(srTransRec.szXMLcash_fee) > 0 && strlen(srTransRec.szXMLcash_fee_type) > 0)
	{
		// Conversion
		memset(szTemp, 0x00, sizeof(szTemp));
		memset(szTemp4, 0x00, sizeof(szTemp4));
		memset(szTemp5, 0x00, sizeof(szTemp5));
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", srTransRec.szXMLcash_fee, szTemp4);
		//sprintf(szTemp,"%s %s",srTransRec.szXMLcash_fee_type, szTemp4);

		if(srTransRec.byTransType == SALE)
			sprintf(szTemp5, "%s%s", srTransRec.szXMLcash_fee_type, szTemp4);

		if(srTransRec.byTransType == VOID)
			sprintf(szTemp5, "%s-%s", srTransRec.szXMLcash_fee_type, szTemp4);
			
		inPrintLeftRight(" ",szTemp5,24);
	}
	
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
	
	return d_OK;
}

int iXMLPrintWechatFooter(int page)
{	
	vdDebug_LogPrintf("--iXMLPrintWechatFooter--");	
	vdDebug_LogPrintf("byTerminalType[%d]", strTCT.byTerminalType);
	vdDebug_LogPrintf("fFooterLogo[%d]", strHDT.fFooterLogo);
	vdDebug_LogPrintf("szFooterLogoName[%s]", strHDT.szFooterLogoName);

	vdCTOS_PrinterFline(1);

    if (srTransRec.HDTid == PHQR_HOST_INDEX){
		
		vdPrintCenter("THIS SERVES AS YOUR ACKNOWLEDGEMENT RECEIPT");
		vdPrintCenter("THANK YOU");

    }else{
		vdPrintCenter("THIS SERVES AS YOUR");
		vdPrintCenter("ACKNOWLEDGEMENT RECEIPT");	
		vdPrintCenter("THANK YOU");
	}

	//printDividingLine(DIVIDING_LINE);	

	vdCTOS_PrinterFline(1);
	
	//iXMLPrintWechatDateTime(FALSE, FALSE, page);
	
	//vdCTOS_PrinterFline(2);

	if (strDigitalWallet.fSignatureLine == TRUE)
	{
		inPrint("SIGNATURE:");
		vdCTOS_PrinterFline(2);
		printDividingLine(DIVIDING_LINE);
	}

	switch (page)
	{
		case CUSTOMER_COPY_RECEIPT:
			vdPrintCenter("***** CUSTOMER COPY *****");
			break;
		case MERCHANT_COPY_RECEIPT:
			vdPrintCenter("***** MERCHANT COPY *****");
			break;
		case BANK_COPY_RECEIPT:
			vdPrintCenter("***** BANK COPY *****");
			break;
	}	

	vdCTOS_PrinterFline(1);


    //if (srTransRec.HDTid != PHQR_HOST_INDEX){

	if (isCheckTerminalMP200() != d_OK && strHDT.fFooterLogo == TRUE)
	{
		if(strlen(strHDT.szFooterLogoName) > 0)
		    vdCTOSS_PrinterBMPPic(0, 0, strHDT.szFooterLogoName);
		else
		    vdCTOSS_PrinterBMPPic(0, 0, "footer.bmp");
	}
    //}

	vdLineFeed((strHDT.fFooterLogo == TRUE)?TRUE:FALSE); // added -- sidumili

    if (srTransRec.HDTid != PHQR_HOST_INDEX){

	if (strHDT.fFooterLogo == TRUE)
		vdCTOS_PrinterFline(2);
    }
	
	return d_OK;
}

int iXMLPrintWechatCopy(int page)
{
	vdDebug_LogPrintf("--iXMLPrintWechatCopy--");
	
	switch (page)
	{
		case CUSTOMER_COPY_RECEIPT:
			inPrint("CUSTOMER COPY");
			break;
		case MERCHANT_COPY_RECEIPT:
			inPrint("MERCHANT COPY");
			break;
		case BANK_COPY_RECEIPT:
			inPrint("BANK COPY");
			break;
	}

	inPrint("PLEASE KEEP RECEIPT");

	printDividingLine(DIVIDING_LINE_DOUBLE);
	
	return d_OK;
}

int iXMLPrintWechatQRCode(void)
{	
	BYTE key;
	USHORT res;
	USHORT usRet = d_OK;
	//CTOS_QRCODE_INFO qrcodeInfo;	
	CTOS_QRCODE_INFO_S1 qrcode_info;
	USHORT usXstart = 0;

	vdDebug_LogPrintf("--iXMLPrintWechatQRCode--");
	vdDebug_LogPrintf("srTransRec.szXMLcode_url=[%s]", srTransRec.szXMLcode_url);

    memset(&qrcode_info, 0x0, sizeof(qrcode_info));
    //qrcode_info.InfoVersion = QR_INFO_VERSION;
	//qrcode_info.Size = 6;
    qrcode_info.Version = QR_VERSION21X21;
    qrcode_info.Level = QR_ECLEVEL_L;

	vdCTOS_PrinterFline(1);
	
	//vdCTOSS_PrinterBufferQRCode(&qrcodeInfo, "weixin://wxpay/bizpayurl?pr=4RWYcPh", 100, 60);
	//vdCTOSS_PrinterBufferQRCode(&qrcodeInfo, srTransRec.szXMLcode_url, 100, 60);

	if (srTransRec.HDTid == GRAB_HOST_INDEX || srTransRec.HDTid == GCASH_HOST_INDEX)
	{
		qrcode_info.Level = QR_ECLEVEL_Q;
		usXstart = 0;
	}		
	else
	{
		qrcode_info.Level = QR_ECLEVEL_L;
		usXstart = 80;
	}		
	
	usRet = CTOS_QRCodePrinter(&qrcode_info, srTransRec.szXMLcode_url, usXstart);
	
	vdDebug_LogPrintf("CTOS_QRCodePrinter, usRet=[%d]", usRet);
		
	return d_OK;	
}

USHORT ushCTOS_WechatprintBatchRecordFooter(BOOL fSettle)
{
	char szStr[d_LINE_SIZE + 1] = {0};
	char szBuff[d_LINE_SIZE + 1] = {0};
	char szTemp1[d_LINE_SIZE + 1] = {0};
	char szTemp2[d_LINE_SIZE + 1] = {0};
	
	char szPrintBuf[d_LINE_SIZE + 1] = {0};
	char szTempBuf1[d_LINE_SIZE + 1] = {0};
	char szTempBuf2[d_LINE_SIZE + 1] = {0};
	char szTemp[d_LINE_SIZE + 3] = {0};
	char szTrxnDateTime[20 + 1] = {0};
	char szConversion[d_LINE_SIZE + 1] = {0};
	int i;
	BYTE szTitle[d_LINE_SIZE + 1] = {0};

	if (fSettle)
	{
		// Print TransName / Amount
		memset(szTemp, 0x00, sizeof(szTemp));	
		memset(szTempBuf1, 0, sizeof(szTempBuf1));
		memset(szTempBuf2, 0, sizeof(szTempBuf2));
		wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTempBuf1);
		sprintf(szTempBuf2,"%s%s",strCST.szCurSymbol, szTempBuf1);

		if (srTransRec.byTransType == SALE)
			inPrintLeftRight("SALE", szTempBuf2, 41);

		// Print Transaction ID
		memset(szTemp, 0,sizeof(szTemp));
		strcpy(szTemp, srTransRec.szXMLtransaction_id);	
		inPrint(szTemp);
	}
	else
	{
		// Print TransName / Amount
		memset(szTemp, 0x00, sizeof(szTemp));	
		memset(szTempBuf1, 0, sizeof(szTempBuf1));
		memset(szTempBuf2, 0, sizeof(szTempBuf2));
		wub_hex_2_str(srTransRec.szTotalAmount, szTemp, AMT_BCD_SIZE);
		vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTempBuf1);

		if (srTransRec.byTransType == SALE)
			sprintf(szTempBuf2,"%s%s",strCST.szCurSymbol, szTempBuf1);
		else if (srTransRec.byTransType == VOID)	
			sprintf(szTempBuf2,"-%s%s",strCST.szCurSymbol, szTempBuf1);
		
		// Get and print transaction title -- sidumili
		memset(szTitle,0x00, sizeof(szTitle));
		szGetDigitalWalletTransTitle(srTransRec.byTransType, szTitle); 
		inPrintLeftRight(szTitle, szTempBuf2, 41);
			
		#if 0
		if (srTransRec.HDTid == WECHAT_HOST_INDEX)
		{
			if (srTransRec.byTransType == SALE)
				inPrintLeftRight("Wechat Pay", szTempBuf2, 41);
			else if (srTransRec.byTransType == VOID)
				inPrintLeftRight("Wechat Pay Void", szTempBuf2, 41);	
		}
			
		if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
		{
			if (srTransRec.byTransType == SALE)
				inPrintLeftRight("ALIPAY Pay", szTempBuf2, 41);
			else if (srTransRec.byTransType == VOID)
				inPrintLeftRight("ALIPAY Pay Void", szTempBuf2, 41);
		}

		if (srTransRec.HDTid == GCASH_HOST_INDEX)
		{
			if (srTransRec.byTransType == SALE)
				inPrintLeftRight("GCASH Pay", szTempBuf2, 41);
			else if (srTransRec.byTransType == VOID)
				inPrintLeftRight("GCASH Pay Void", szTempBuf2, 41);
		}

		if (srTransRec.HDTid == GRAB_HOST_INDEX)
		{
			if (srTransRec.byTransType == SALE)
				inPrintLeftRight("GRAB Pay", szTempBuf2, 41);
			else if (srTransRec.byTransType == VOID)
				inPrintLeftRight("GRAB Pay Void", szTempBuf2, 41);
		}

		if (srTransRec.HDTid == UPI_HOST_INDEX)
		{
			if (srTransRec.byTransType == SALE)
				inPrintLeftRight("UPI Pay", szTempBuf2, 41);
			else if (srTransRec.byTransType == VOID)
				inPrintLeftRight("UPI Pay Void", szTempBuf2, 41);
		}
		#endif
			

		// Print Trxn Date/Time / Status
		memset(szTemp, 0x00, sizeof(szTemp));
		iXMLFormatDateTime((char*)srTransRec.szXMLtime_end, szTrxnDateTime);
		sprintf(szTemp, "%s", szTrxnDateTime);
		inPrintLeftRight(szTemp, "Successful", 41);


        if (srTransRec.HDTid == PHQR_HOST_INDEX){
			
			// txn ref num
			memset(szTemp, 0x00, sizeof(szTemp));		
			sprintf(szTemp, "%s", srTransRec.szXMLtransaction_id);
			inPrint(szTemp);

			
			// txn bancnet invoice
			memset(szTemp, 0x00, sizeof(szTemp));		
			sprintf(szTemp, "%s", srTransRec.szXMLout_transaction_id);
			inPrint(szTemp);

        }else{
			// Print Order ID
			memset(szTemp, 0x00, sizeof(szTemp));		
			sprintf(szTemp, "%s", srTransRec.szXMLtransaction_id);

			// Conversion
			memset(szTemp1, 0x00, sizeof(szTemp1));
			memset(szConversion, 0x00, sizeof(szConversion));
			vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", srTransRec.szXMLcash_fee, szTemp1);

			if (srTransRec.byTransType == SALE)
				sprintf(szConversion,"%s%s",srTransRec.szXMLcash_fee_type, szTemp1);
			else if (srTransRec.byTransType == VOID)
				sprintf(szConversion,"-%s%s",srTransRec.szXMLcash_fee_type, szTemp1);
		
			if (strlen(szConversion) > 11)
			{
				inPrint(szTemp);
				inPrintLeftRight("", szConversion, 41);
			
			}
			else
			{
				inPrintLeftRight(szTemp, szConversion, 41);
			}

			// Print Pay Order ID / Invoice
			memset(szTemp, 0,sizeof(szTemp));
			memset(szStr, 0,sizeof(szStr));
			strcpy(szTemp, srTransRec.szXMLout_transaction_id);	
			wub_hex_2_str(srTransRec.szInvoiceNo, szStr, INVOICE_BCD_SIZE);	
			inPrintLeftRight(szTemp, szStr, 41);

			// Print Transaction ID
			//memset(szTemp, 0,sizeof(szTemp));
			//strcpy(szTemp, srTransRec.szXMLtransaction_id);	
			//inPrint(szTemp);	
        }
	}

	vdCTOS_PrinterFline(1);
}

int iXMLFormatDateTime(char* szTrxnDateTime, char* szOutTrxnDateTime)
{
	char szTrxnDate[10 + 1] = {0};
	char szTrxnTime[8 + 1] = {0};
	char szTemp[5 + 1] = {0};
	
	// 20180719151607
	memset(szOutTrxnDateTime, 0x00, sizeof(szOutTrxnDateTime));

	// Format Date
	memset(szTemp, 0x00, sizeof(szTemp));
	strncpy(szTemp, (char*) &szTrxnDateTime[0], 4);
	strcpy(szTrxnDate, szTemp);
	strcat(szTrxnDate, "-");
	memset(szTemp, 0x00, sizeof(szTemp));
	strncpy(szTemp, (char*) &szTrxnDateTime[4], 2);
	strcat(szTrxnDate, szTemp);
	strcat(szTrxnDate, "-");
	strncpy(szTemp, (char*) &szTrxnDateTime[6], 2);
	strcat(szTrxnDate, szTemp);

	// Format Time
	memset(szTemp, 0x00, sizeof(szTemp));
	strncpy(szTemp, (char*) &szTrxnDateTime[8], 2);
	strcpy(szTrxnTime, szTemp);
	strcat(szTrxnTime, ":");
	memset(szTemp, 0x00, sizeof(szTemp));
	strncpy(szTemp, (char*) &szTrxnDateTime[10], 2);
	strcat(szTrxnTime, szTemp);
	strcat(szTrxnTime, ":");
	strncpy(szTemp, (char*) &szTrxnDateTime[12], 2);
	strcat(szTrxnTime, szTemp);

	sprintf(szOutTrxnDateTime, "%s %s", szTrxnDate, szTrxnTime);

	vdDebug_LogPrintf("szOutTrxnDateTime[%s]", szOutTrxnDateTime);
	
}

USHORT ushCTOS_WechatprintBatchRecordHeader(void)
{    
	inPrintLeftRight("TRANSACTION", "AMOUNT", 41);
	inPrintLeftRight("DATE/TIME", "STATUS", 41);

	if (srTransRec.HDTid == WECHAT_HOST_INDEX)
	{
		inPrintLeftRight("ORDER ID", "CONVERSION", 41);
		inPrintLeftRight("WECHAT PAY ORDER ID", "INVOICE NO", 41);
	}

	if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
	{
		inPrintLeftRight("PLATFORM ORDER ID", "CONVERSION", 41);
		inPrintLeftRight("ALIPAY ORDER ID", "INVOICE NO", 41);
	}

	if (srTransRec.HDTid == GCASH_HOST_INDEX)
	{
		inPrintLeftRight("ORDER ID", "CONVERSION", 41);
		inPrintLeftRight("GCASH ORDER ID", "INVOICE NO", 41);
	}

	if (srTransRec.HDTid == GRAB_HOST_INDEX)
	{
		inPrintLeftRight("ORDER ID", "CONVERSION", 41);
		inPrintLeftRight("GRAB ORDER ID", "INVOICE NO", 41);
	}

	if (srTransRec.HDTid == UPI_HOST_INDEX)
	{
		inPrintLeftRight("ORDER ID", "CONVERSION", 41);
		inPrintLeftRight("UPI ORDER ID", "INVOICE NO", 41);
	}

	
	if (srTransRec.HDTid == PHQR_HOST_INDEX)
	{
		inPrintLeftRight("TXN REFERENCE #", " ", 41);
		inPrintLeftRight("BANCNET INVOICE", " ", 41);
	}
			
	printDividingLine(DIVIDING_LINE);
	
	return d_OK;
}

int iXMLPrintWechatDateTime(BOOL fSettle, BOOL fPrintLastSettle, int iPage)
{	
	
	CTOS_RTC SetRTC;
	char szYear[4+1] = {0};
	char szMonth[2+1] = {0};
	char szDay[2+1] = {0};
	char szTime[6+1] = {0};
	char szCurrentDate[8 + 1] = {0};
	char szCurrentTime[6 + 1] = {0};
	char szDateTime[20 + 1] = {0};
	char szPrintDateTime[50 + 1] = {0};
	char szTrxnDateTime[20 + 1] = {0};
	int inRet;
	BOOL fNull = FALSE;
	
	vdDebug_LogPrintf("--iXMLPrintWechatDateTime--");
	vdDebug_LogPrintf("fSettle[%d].fPrintLastSettle[%d].iPage[%d]", fSettle, fPrintLastSettle, iPage);
	vdDebug_LogPrintf("HDTid[%d].MITid[%d]", srTransRec.HDTid, srTransRec.MITid);
	vdDebug_LogPrintf("fRePrintFlag[%d]", fRePrintFlag);
	DebugAddHEX("szInvoiceNo", srTransRec.szInvoiceNo, 3);
	
	CTOS_RTCGet(&SetRTC);

	sprintf(szYear ,"20%02d",SetRTC.bYear);
	sprintf(szMonth ,"%02d",SetRTC.bMonth);	
	sprintf(szDay ,"%02d",SetRTC.bDay);	
	sprintf(szTime ,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);

	//wub_hex_2_str(srTransRec.szTime, szTime,TIME_BCD_SIZE);

	vdDebug_LogPrintf("szYear[%s]", szYear);
	vdDebug_LogPrintf("szMonth[%s]", szMonth);
	vdDebug_LogPrintf("szDay[%s]", szDay);
	vdDebug_LogPrintf("szTime[%s]", szTime);

	sprintf(szCurrentDate, "%s%s%s", szYear, szMonth, szDay);
	sprintf(szCurrentTime, "%s", szTime);

	vdDebug_LogPrintf("szCurrentDate[%s]", szCurrentDate);
	vdDebug_LogPrintf("szCurrentTime[%s]", szCurrentTime);		
	
	sprintf(szDateTime, "%s%s", szCurrentDate, szCurrentTime);
	iXMLFormatDateTime(szDateTime, szTrxnDateTime);

	if (fSettle)
	{
		if (fPrintLastSettle)
		{
			if (srTransRec.HDTid == WECHAT_HOST_INDEX)
				inCTOSS_GetEnvDB("WCSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
				inCTOSS_GetEnvDB("ALSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == GCASH_HOST_INDEX)
				inCTOSS_GetEnvDB("GCSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == GRAB_HOST_INDEX)
				inCTOSS_GetEnvDB("GRSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == UPI_HOST_INDEX)
				inCTOSS_GetEnvDB("UPSETTLEDATETIME", szTrxnDateTime);
		}
			
		else
		{
			if (srTransRec.HDTid == WECHAT_HOST_INDEX)
				inCTOSS_PutEnvDB("WCSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == ALIPAY_HOST_INDEX)
				inCTOSS_PutEnvDB("ALSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == GCASH_HOST_INDEX)
				inCTOSS_PutEnvDB("GCSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == GRAB_HOST_INDEX)
				inCTOSS_PutEnvDB("GRSETTLEDATETIME", szTrxnDateTime);

			if (srTransRec.HDTid == UPI_HOST_INDEX)
				inCTOSS_PutEnvDB("UPSETTLEDATETIME", szTrxnDateTime);
		}
			
	}
	
	vdDebug_LogPrintf("szTrxnDateTime[%s]", szTrxnDateTime);
	
	memset(szPrintDateTime, 0x00, sizeof(szPrintDateTime));
	sprintf(szPrintDateTime, "PRINT TIME: %s", szTrxnDateTime);
	inPrint(szPrintDateTime);

	return d_OK;
}

int iXMLPrintWechatTIDMID(void)
{
	char szTemp1[d_LINE_SIZE + 1] = {0};
	char szTemp2[d_LINE_SIZE + 1] = {0};
	
	memset(szTemp1, 0x00, sizeof(szTemp1));
    memset(szTemp2, 0x00, sizeof(szTemp2));
	//sprintf(szTemp1,"TID: %s", srTransRec.szTID);
	//inPrint(szTemp1);
	sprintf(szTemp2,"MID: %s", srTransRec.szMID);
	inPrint(szTemp2);
}

int inPrintXMLPacket(unsigned char *pucTitle,unsigned char *pucMessage, int inLen, BOOL fRequest)
{
    char ucLineBuffer[d_LINE_SIZE+4];
    unsigned char *pucBuff;
    BYTE baTemp[PAPER_X_SIZE * 64];
    char szStr[d_LINE_SIZE + 4];
	BOOL fLoop = TRUE;
	int i = 0;
	int iLimit = 48;
	BYTE ch = 0;

	//vdDebug_LogPrintf("--inPrintXMLPacket--");
	
    if (inLen <= 0)
        return(ST_SUCCESS);

	if( printCheckPaper()==-1)
	return -1;
	
	vdCTOSS_PrinterStart(100);
    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_16x16,0," ");
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
    CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);  

	if (fRequest)	
		inCCTOS_PrinterBufferOutput("[REQUEST-XML]", &stgFONT_ATTRIB, FALSE);
	else
		inCCTOS_PrinterBufferOutput("[RETRIEVE-XML]", &stgFONT_ATTRIB, FALSE);
	
    memset(szStr, 0x00, sizeof(szStr));
	memset(baTemp, 0x00, sizeof(baTemp));
    sprintf(szStr,"[%s] [%d] \n", pucTitle, inLen);
	inCCTOS_PrinterBufferOutput(szStr, &stgFONT_ATTRIB, FALSE);
	vdCTOS_PrinterFline(1);		

	do
	{
		memset(szStr, 0x00, sizeof(szStr));
		memcpy(szStr, &pucMessage[i], iLimit);
		pucMessage[i+1] = '\0';
		purge_chr(szStr, '\n');
		//vdDebug_LogPrintf("i[%d].Len[%d].szStr[%s]", i, strlen(szStr), szStr);	
		inCCTOS_PrinterBufferOutput(szStr, &stgFONT_ATTRIB, FALSE);
			
		i+=iLimit;
		if (i > inLen) break;

		
	}while (fLoop);	
	
	vdCTOS_PrinterFline(5);
	
    vdCTOSS_PrinterEnd();

	//vdDebug_LogPrintf("--Exit--");
	
    return d_OK;
}

// For Wechat -- sidumili


int inPrintLeftRight2(unsigned char *strLeft, unsigned char *strRight, int inWidth) 
{
	char szStr[64+1]; 
    int inLength=0, inSize=0;
    BYTE baTemp[PAPER_X_SIZE * 64];
	
	if ((isCheckTerminalMP200() == d_OK) && (strTCT.byERMMode == 2))
	{
		inWidth = 46;
		vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	}
	
	inWidth--;
	
	memset(szStr, 0x20, sizeof(szStr));
    memcpy(szStr, strLeft, strlen(strLeft));
    memcpy(&szStr[inWidth-strlen(strRight)], strRight, strlen(strRight));
	szStr[inWidth]=0x00;
	
	inCCTOS_PrinterBufferOutput(szStr, &stgFONT_ATTRIB,fERMTransaction);
    return d_OK;
	
}

int vdCTOS_SettlementPrintDetailReport(BOOL fAddtoSettleReport, BOOL fLastSettleReport, BOOL fReprintLastSettleHeader)
{
	unsigned char chkey;
	short shHostIndex;
	int inResult,i,inCount,inRet;
	int inTranCardType;
	int inReportType;
	int inBatchNumOfRecord;
	int *pinTransDataid;

	//0720
	ACCUM_REC srAccumRec;
	BYTE baTemp[PAPER_X_SIZE * 64];
	char szStr[d_LINE_SIZE + 1];
	//0720

	//for MP200 no need print
		if ((strTCT.byTerminalType == 5) || (strTCT.byTerminalType == 6))
			return (d_OK);

	if( printCheckPaper()==-1)
		return d_NO;

	//inRet = inCTOS_TEMPCheckAndSelectMutipleMID();
    inRet = inCTOS_CheckAndSelectMutipleMID();
	if(d_OK != inRet)
		return inRet;
		
	//by host and merchant
	shHostIndex = inCTOS_SelectHostSetting();
	if (shHostIndex == -1)
		return d_NO;

	//strHDT.inHostIndex = shHostIndex;
	DebugAddINT("summary host Index",shHostIndex);


    //inResult = inCTOS_CheckAndSelectMutipleMID();
    //if(d_OK != inResult)
    //    return inResult;
    inMMTReadRecord(srTransRec.HDTid,srTransRec.MITid);

#if 0//tempremove
    if(fLastSettleReport == TRUE)
    {
        inBatchNumOfRecord = inBackupDetailReportNumRecord();
    }
    else
#endif
    {
		inResult = inCTOS_ChkBatchEmpty();
		if(d_OK != inResult)
			return d_NO;
        inBatchNumOfRecord = inBatchNumRecord();
    }
	
	DebugAddINT("batch record",inBatchNumOfRecord);
	if(inBatchNumOfRecord <= 0)
	{
		vdDisplayErrorMsg(1, 8, "BATCH EMPTY");
		return d_NO;
	}

    //vdDisplayAnimateBmp(0,0, "Print1.bmp", "Print2.bmp", "Print3.bmp", NULL, NULL);
    //vdPrintReportDisplayBMP();
    inCTOS_DisplayPrintBMP();

    inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

#if 0//tempremove
	if(fReprintLastSettleHeader == TRUE)
		ushCTOS_ReprintBatchRecordHeader();// Get ACCUMS data to get correct batch number and date
	else
#endif
		ushCTOS_printBatchRecordHeader();

	
	pinTransDataid = (int*)malloc(inBatchNumOfRecord  * sizeof(int));
	inCount = 0;	
#if 0 //tempremove
    if(fLastSettleReport == TRUE)
    {
		inBatchByDetailReport(inBatchNumOfRecord, srTransRec.HDTid, srTransRec.MITid, pinTransDataid);
		for(i=0; i<inBatchNumOfRecord; i++)
		{
			inResult = inDatabase_ReadDetailReport(&srTransRec, pinTransDataid[inCount]);
			inCount ++;
			ushCTOS_printBatchRecordFooter();	
		}		
    }
    else
#endif

    {
        inBatchByMerchandHost(inBatchNumOfRecord, srTransRec.HDTid, srTransRec.MITid, srTransRec.szBatchNo, pinTransDataid);
        for(i=0; i<inBatchNumOfRecord; i++)
        {
            inResult = inDatabase_BatchReadByTransId(&srTransRec, pinTransDataid[inCount]);
            inCount ++;
            ushCTOS_printBatchRecordFooter();	
        }
    }

    free(pinTransDataid);

	//0720
    memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
    if(fLastSettleReport == TRUE)
    {
        inResult = inCTOS_ReadBKAccumTotal(&srAccumRec,strHDT.inHostIndex,srTransRec.MITid);
    }
    else
        inResult = inCTOS_ReadAccumTotal(&srAccumRec);
        
	if(inResult == ST_ERROR)
	{
		vdMyEZLib_LogPrintf("[vdUpdateAmountTotal]---Read Total Rec. error");
		return d_NO; 	 
	}
	

	    vdCTOS_PrinterFline(2);
		//CTOS_PrinterFline(d_LINE_DOT * 1);
		//CTOS_PrinterFline(d_LINE_DOT * 1);

	    memset(baTemp, 0x00, sizeof(baTemp));
		//sprintf(szStr,"CARD TYPE        COUNT         AMT");
		sprintf(szStr,"CARD TYPE        COUNT                 AMT");
		//CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
		//CTOS_PrinterBufferOutput((BYTE *)baTemp, 3);
        inPrint(szStr);
		
		for(inTranCardType = 0; inTranCardType < 1 ;inTranCardType ++)
		{
				inReportType = DETAIL_REPORT_TOTAL;
		
				for(i= 0; i <20; i ++ )
				{
					vdDebug_LogPrintf("--Count[%d]", i);
					if((srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usSaleCount == 0)
					&&(srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usOffSaleCount == 0)
					&&(srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usRefundCount == 0)
					&&(srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usVoidSaleCount == 0)
					&&(srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usReloadCCCount == 0))
						continue;
				
					vdDebug_LogPrintf("Count[%d]", i); 
					inIITRead(i);
					memset(szStr, ' ', d_LINE_SIZE);
					memset (baTemp, 0x00, sizeof(baTemp));
	
				
					vdCTOS_PrintAccumeByHostAndCard (inReportType, 
					srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usSaleCount - srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usOffSaleCount,	(srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].ulSaleTotalAmount) - (srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),
																
					srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usRefundCount, (srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].ulRefundTotalAmount), 
																
					srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usVoidSaleCount, (srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].ulVoidSaleTotalAmount), 
				
					srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usOffSaleCount, (srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].ulOffSaleTotalAmount),
				
					srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].usReloadCCCount, (srAccumRec.stBankTotal[DEBIT_CARD].stCardTotal[i].ulReloadCCTotalAmount));	
				}
	
				
				printDividingLine(DIVIDING_LINE);
				
				inReportType = DETAIL_REPORT_GRANDTOTAL;
				
				vdCTOS_PrintAccumeByHostAndCard (inReportType, 
				srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.usSaleCount - srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.usOffSaleCount,   (srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.ulSaleTotalAmount) - (srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.ulOffSaleTotalAmount),
																		
				srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.usRefundCount, (srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.ulRefundTotalAmount), 
																		
				srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.usVoidSaleCount, (srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.ulVoidSaleTotalAmount), 
					
				srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.usOffSaleCount, (srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.ulOffSaleTotalAmount),
					//1010
				srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.usReloadCCCount, (srAccumRec.stBankTotal[DEBIT_CARD].stHOSTTotal.ulReloadCCTotalAmount));
					//1010
	}
	
	
	//0720


    vdCTOS_PrinterFline(10);
    //CTOS_PrinterFline(d_LINE_DOT * 10);

	
	return d_OK;	
}

void vdGetSignatureFilename(char *ptrNewFilename)
{
	char szNewFileName[24+1];
    char szInvoiceNo[6+1];
    
    memset(szInvoiceNo, 0x00, sizeof(szInvoiceNo));
    wub_hex_2_str(srTransRec.szInvoiceNo, szInvoiceNo, INVOICE_BCD_SIZE);
    
    memset(szNewFileName, 0x00, sizeof(szNewFileName));
    sprintf(szNewFileName, "signERM%06ld.bmp", atol(szInvoiceNo));
	memcpy(ptrNewFilename, szNewFileName, strlen(szNewFileName));
}

void vdPrintTIDMIDVersion(void)
{
	char szStr[d_LINE_SIZE + 1];
    char szRootFS[d_LINE_SIZE + 1];
	char szStrLeft[d_LINE_SIZE + 1];
	char szStrRight[d_LINE_SIZE + 1];
	char szAppVersion[d_LINE_SIZE + 1];
	char szTerminalVersion[d_LINE_SIZE + 1];

	//-->> TID / MID

	if(srTransRec.HDTid == PHQR_HOST_INDEX){
		memset(szStrRight, 0x00, d_LINE_SIZE);
		sprintf(szStrRight, "MID: %s", srTransRec.szMID);
		
		memset(szStrLeft, 0x00, d_LINE_SIZE);
		sprintf(szStrLeft, "TID: %s", srTransRec.szTID);
		inPrintLeftRight(szStrRight, szStrLeft, 46);

	}else{
		memset(szStrLeft, 0x00, d_LINE_SIZE);
		sprintf(szStrLeft, "TID: %s", srTransRec.szTID);
		memset(szStrRight, 0x00, d_LINE_SIZE);
		sprintf(szStrRight, "MID: %s", srTransRec.szMID);
		inPrintLeftRight(szStrLeft, szStrRight, 46);
	}

	#if 0
	//-->>  OS VERSION / MODEL VERSION
	vdCTOS_GetROOTFS(szRootFS);
	szRootFS[6]=0x00;
	memset(szStrLeft, 0x00, d_LINE_SIZE);
	sprintf(szStrLeft, "OS VER: %s", szRootFS);
	memset(szStrRight, 0x00, d_LINE_SIZE);
	memset(szTerminalVersion, 0x00, sizeof(szTerminalVersion));
	vdGetTerminalVersion((char*)szTerminalVersion);
	sprintf(szStrRight, "MODEL VER: %s", szTerminalVersion);
	inPrintLeftRight(szStrLeft, szStrRight, 46);	
	#endif

}

void vdPrintTrxnType(BOOL fPrintCenter)
{
	char szTemp[d_LINE_SIZE + 1];
	char szStr[d_LINE_SIZE + 1];
	char szStrLeft[d_LINE_SIZE + 1];	
	char szTitle[d_LINE_SIZE + 1];
	
	//-->> TRAN TYPE
	memset(szStrLeft, 0x00, d_LINE_SIZE);
	sprintf(szStrLeft, "%s", "TRAN TYPE:");
	memset(szTitle, 0x00, d_LINE_SIZE);	

	// Get Trxn Title
	szGetDigitalWalletTransTitle(srTransRec.byTransType, szTitle); 

	if (fPrintCenter)
		vdPrintCenter(szTitle);
	else
		inPrintLeftRight(szStrLeft, szTitle, 46);
}

void vdCTOS_GetROOTFS(char* ucBuffer)
{
	int inResult;

	memset(ucBuffer, 0x00, sizeof(ucBuffer));
	//inResult = CTOS_GetSystemInfo (ID_ROOTFS, ucBuffer);

	return;
}

void vdGetTerminalVersion(char* szVersion)
{
	char szType[20 + 1] = {0};
	char szModel[20 + 1] = {0};

	switch (strTCT.byTerminalType)
	{		
		case 1:
			strcpy(szType, "V5");
			break;
		case 2:			
		case 4: 
			strcpy(szType, "V3");
			break;			
		case 3:
			strcpy(szType, "V3 NT");
			break;
		case 5:
			strcpy(szType, "MP200");
			break;
		default:
			strcpy(szType, "N/A");
			break;			
	}

	switch (strTCT.byTerminalModel)
	{		
		case V3_DESKTOP:
			strcpy(szModel, "DESKTOP");
			break;
		case V3_MOBILE:			
			strcpy(szModel, "MOBILE");
			break;
		default:
			strcpy(szModel, "N/A");
			break;
	}

	memset(szVersion, 0x00, sizeof(szVersion));
	sprintf(szVersion, "%s-%s", szType, szModel);
}

void vdPrintBatchNoInvoiceNo(void)
{
	char szTemp1[d_LINE_SIZE + 1];
	char szTemp2[d_LINE_SIZE + 1];
	char szStr[d_LINE_SIZE + 1];
	BYTE baTemp[PAPER_X_SIZE * 64];
	char szStrLeft[d_LINE_SIZE + 1];
	char szStrRight[d_LINE_SIZE + 1];

	// -->> BATCH NUM / INVOICE NUM
	//srTransRec.szBancnetBatchNo[6]=0x00;
	wub_hex_2_str(srTransRec.szBatchNo,szTemp1,3); // sidumili: revert to its original function
	memset(szStrLeft, 0x00, d_LINE_SIZE);
	sprintf(szStrLeft, "BATCH NUM: %s", szTemp1);

	memset(szTemp2, 0x00, d_LINE_SIZE);
	wub_hex_2_str(srTransRec.szInvoiceNo, szTemp2, INVOICE_BCD_SIZE);
	memset(szStrRight, 0x00, d_LINE_SIZE);

	if (srTransRec.HDTid == PHQR_HOST_INDEX)	
		sprintf(szStrRight, "INVOICE NO.: %s", szTemp2);
	else
		sprintf(szStrRight, "TRACE NO.: %s", szTemp2);
	inPrintLeftRight(szStrLeft, szStrRight, 46);
	
}

void vdPrintDateTime(void)
{
	char szTemp1[d_LINE_SIZE + 1];
	char szTemp2[d_LINE_SIZE + 1];
	char szStr[d_LINE_SIZE + 1];
	ULONG ulHH=0L;
    char szAMPM[2+1];
	char szStrLeft[d_LINE_SIZE + 1];
	char szStrRight[d_LINE_SIZE + 1];
	
	memset(szTemp1, 0x00, d_LINE_SIZE);
	memset(szTemp2, 0x00, d_LINE_SIZE);
	printDateTime2(szTemp1, szTemp2, FALSE);
	memset(szStr, 0x00, d_LINE_SIZE);

	ulHH=atol(szTemp2)/10000;
	if (ulHH==0&& ulHH!=12)
	{
		ulHH=12;
		strcpy(szAMPM,"AM"); 
	}
	else if (ulHH==12 && ulHH!=0)
	{
		ulHH=12;
		strcpy(szAMPM,"PM"); 
	}
	else if(ulHH<12 && ulHH!=0)
	{
		strcpy(szAMPM,"AM"); 
	}
	else if(ulHH>12 && ulHH!=0)
	{
		ulHH-=12;
		strcpy(szAMPM,"PM"); 
	}			

	//-->> DATE/TIME
	memset(szStrLeft, 0x00, d_LINE_SIZE);
	sprintf(szStrLeft, "DATE: %s", szTemp1);
	memset(szStrRight, 0x00, d_LINE_SIZE);
	sprintf(szStrRight, "TIME: %02lu:%02lu:%02lu%s", ulHH,atol(szTemp2)%10000/100, atol(szTemp2)%100, szAMPM);
	inPrintLeftRight(szStrLeft, szStrRight, 46);
}

USHORT printDateTime2(char *ptrDate, char *ptrTime, BOOL fTrans)
{
	char szStr[d_LINE_SIZE + 1];
	char szTemp[d_LINE_SIZE + 1];
	char szTemp1[d_LINE_SIZE + 1];
	USHORT result;
	BYTE baTemp[PAPER_X_SIZE * 64];
	CTOS_RTC SetRTC;
	char szYear[3];
	char szTempDate[d_LINE_SIZE + 1];
	BYTE szTime[6];     /* BCD hhmmss        */
	
	vdDebug_LogPrintf("--printDateTime2--");
	vdDebug_LogPrintf("szTranYear[%s]", srTransRec.szTranYear);
	vdDebug_LogPrintf("fTrans[%d]", fTrans);
	
	memset(szStr, ' ', d_LINE_SIZE);
	memset(szTemp, ' ', d_LINE_SIZE);
	
	CTOS_RTCGet(&SetRTC);
	sprintf(szYear ,"%02d",SetRTC.bYear);
	memcpy(srTransRec.szYear,szYear,2);
	vdDebug_LogPrintf("year[%s],date[%02x][%02x]",srTransRec.szYear,srTransRec.szDate[0],srTransRec.szDate[1]);
	//wub_hex_2_str(srTransRec.szDate, szTemp,DATE_BCD_SIZE);
	wub_hex_2_str(srRetrieveRec.szDate, szTemp,DATE_BCD_SIZE);

	// From response
	memset(szTime, 0x00, sizeof(szTime));
	memcpy(szTime,&srTransRec.szXMLtime_end[8],6);
	memset(srTransRec.szTime,0x00,sizeof(srTransRec.szTime));
	wub_str_2_hex(szTime,srTransRec.szTime,6);
	vdDebug_LogPrintf("szXMLtime_end=[%s], szTime[%s]", srTransRec.szXMLtime_end, szTime);
	
	wub_hex_2_str(srTransRec.szTime, szTemp1,TIME_BCD_SIZE);
	vdDebug_LogPrintf("date[%s],time[%s]atol(szTemp)=[%d](atol(szTemp1)=[%d]",szTemp,szTemp1,atol(szTemp),atol(szTemp1) );	

	if(atol(szTemp) == 0)
	{
		sprintf(szTemp ,"%02d%02d",SetRTC.bMonth,SetRTC.bDay); //aaronnino mcc bancnet fix on issue #0002 Incomplete Date in Settlement  and Last settlement	
	}
	if(atol(szTemp1) == 0)
	{
		sprintf(szTemp1 ,"%02d%02d%02d",SetRTC.bHour,SetRTC.bMinute,SetRTC.bSecond);	

	}
	vdDebug_LogPrintf("date[%s],time[%s]",szTemp,szTemp1);
	
	memset(szTempDate, 0x00, sizeof(szTempDate));

	if (fTrans)
	{
		if (strlen(srTransRec.szTranYear) > 0)
			sprintf(szTempDate, "%02lu%02lu%02lu", atol(srTransRec.szTranYear), atol(szTemp)/100, atol(szTemp)%100);
		else
			sprintf(szTempDate, "%02lu%02lu%02lu", atol(srTransRec.szYear), atol(szTemp)/100, atol(szTemp)%100);
	}		
	else
	{
		sprintf(szTempDate, "%02lu%02lu%02lu", atol(srTransRec.szYear), atol(szTemp)/100, atol(szTemp)%100);
	}
	
	vdCTOS_FormatDate(szTempDate);
	//if(strCDT.inType == DEBIT_CARD)
		//sprintf(szStr,"DATE %s	  TIME %02lu:%02lu:%02lu",szTempDate,atol(szTemp1)/10000,atol(szTemp1)%10000/100, atol(szTemp1)%100);
	//else	
		//sprintf(szStr,"DATE/TIME: %s %02lu:%02lu:%02lu",szTempDate,atol(szTemp1)/10000,atol(szTemp1)%10000/100, atol(szTemp1)%100);

	//inPrint(szStr);
	memcpy(ptrDate, szTempDate, strlen(szTempDate));
	memcpy(ptrTime, szTemp1, strlen(szTemp1));

	vdDebug_LogPrintf("ptrDate[%s].ptrTime[%s]", ptrDate, ptrTime);
	
	return(result);
	 
}

int inPrintWechatQRCode(void)
{
	vdDebug_LogPrintf("--inPrintWechatQRCode--");
	BOOL fJavaCall = TRUE;

	if(inFLGGet("fPrintQRCode") != TRUE)
		return d_OK;

	if(isCheckTerminalMP200() == d_OK)
		return d_OK;
	
	if( printCheckPaper()==-1)
        return -1;

	inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
    vdSetGolbFontAttrib(d_FONT_24x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);

	inHDTRead(srTransRec.HDTid);

	vdCTOSS_PrinterStart(100);
	CTOS_PrinterSetHeatLevel(4);
	
	inCTOS_DisplayPrintBMP();
	
	iXMLPrintWechatHeader();
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);

	printDividingLine(DIVIDING_LINE);
	
	inPrintTrxnTypeAmount();
	
	printDividingLine(DIVIDING_LINE);

	if (fJavaCall)
		usCTOSS_PrintQRCodeUI(srTransRec.szXMLcode_url);
	else
		iXMLPrintWechatQRCode();

	vdPrintCenter("SCAN TO PAY");
	
	printDividingLine(DIVIDING_LINE);

	inPrintFooterLogo();
	
	vdCTOS_PrinterFline(1);

	vdCTOSS_PrinterEnd();

	//CTOS_LCDTClearDisplay();
	
	return d_OK;
}

int inPrintTrxnTypeAmount(void)
{
	char szTemp[d_LINE_SIZE + 1] = {0};
	char szTemp4[d_LINE_SIZE + 1] = {0};
	char szTemp5[d_LINE_SIZE + 1] = {0};
	char szTitle[d_LINE_SIZE + 1] = {0};

	// Get Trxn Title
	memset(szTitle, 0x00, sizeof(szTitle));
	szGetDigitalWalletTransTitle(srTransRec.byTransType, szTitle); 

	// Format Amount
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	memset(szTemp5, 0x00, sizeof(szTemp5));
	wub_hex_2_str(srTransRec.szBaseAmount, szTemp, AMT_BCD_SIZE);
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTemp4);
	
	if(srTransRec.byTransType == SALE)
		sprintf(szTemp5, "%s%s", strCST.szCurSymbol, szTemp4);

	inPrintLeftRight(szTitle, szTemp5, 46);
	
	return d_OK;
}

int inPrintFooterLogo(void)
{
	if (isCheckTerminalMP200() != d_OK && strHDT.fFooterLogo == TRUE)
	{
		if(strlen(strHDT.szFooterLogoName) > 0)
		    vdCTOSS_PrinterBMPPic(0, 0, strHDT.szFooterLogoName);
		else
		    vdCTOSS_PrinterBMPPic(0, 0, "footer.bmp");
	}

	vdLineFeed((strHDT.fFooterLogo == TRUE)?TRUE:FALSE); // added -- sidumili
	
	return d_OK;
}

void vdLogISOOption(unsigned char *pucTitle,unsigned char *uszData, int inLen)
{
	//vdDebug_LogPrintf2("============================================");
	
	if(memcmp(pucTitle,"RX",2) == 0 && strCPT.inCommunicationMode == DIAL_UP_MODE)
		inLogISOPacket(pucTitle , uszData, inLen-2);
	else
		inLogISOPacket(pucTitle , uszData, inLen);

	vdLogParseISO(srTransRec.byTransType,pucTitle ,uszData+5);

	//vdDebug_LogPrintf2("============================================");

}

int inLogISOPacket(unsigned char *pucTitle,unsigned char *pucMessage, int inLen)
{
    char ucLineBuffer[d_LINE_SIZE+4];
    unsigned char *pucBuff;
    int inBuffPtr = 0;
    BYTE baTemp[PAPER_X_SIZE * 64];
    char szStr[d_LINE_SIZE + 4];

    if (inLen <= 0)
        return(ST_SUCCESS);

	//if (isCheckTerminalMP200() != d_OK)
	//	vdCTOSS_PrinterStart(100); // For fast printing
		
    //CTOS_PrinterSetHeatLevel(strTCT.inPrinterHeatLevel);
	
	//CTOS_PrinterSetWorkTime(50000,1000);
    //inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_16x16,0," ");
	//vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
    
    memset(szStr, 0x00, sizeof(szStr));
	memset(baTemp, 0x00, sizeof(baTemp));
    sprintf(szStr,"[%s] [%d] \n", pucTitle, inLen);
    //CTOS_PrinterBufferPutString((BYTE *)baTemp, 1, 1, szStr, &stgFONT_ATTRIB);
    //CTOS_PrinterBufferOutput((BYTE *)baTemp, 3);
	//inCCTOS_PrinterBufferOutput(szStr,&stgFONT_ATTRIB,1);

	//if (isCheckTerminalMP200() == d_OK)		
	//	vdDebug_LogPrintf2(szStr);

		
    //CTOS_PrinterFline(d_LINE_DOT); 
	  //vdCTOS_PrinterFline(1);
    
    
    pucBuff = pucMessage + inLen;
    while (pucBuff > pucMessage)
    {
    memset(ucLineBuffer,0x00, sizeof(ucLineBuffer));
    for (inBuffPtr = 0; (inBuffPtr < 44) && (pucBuff > pucMessage); inBuffPtr += 3)
    {
    sprintf(&ucLineBuffer[inBuffPtr], "%02X ", *pucMessage);
    pucMessage++;
    }

	//if (isCheckTerminalMP200() == d_OK) 	
	//	vdDebug_LogPrintf2(ucLineBuffer);

    ucLineBuffer[44] = '\n';
    memset (baTemp, 0x00, sizeof(baTemp));		
    //inCCTOS_PrinterBufferOutput(ucLineBuffer, &stgFONT_ATTRIB,1);
    }
	//vdCTOS_PrinterFline(2);	
    //CTOS_PrinterFline(d_LINE_DOT * 2); 
   	//if (isCheckTerminalMP200() != d_OK)
    //	vdCTOSS_PrinterEnd();
    return (ST_SUCCESS);
}

// Delete bmp file -- sidumili
void vdDeleteBMP(void)
{
	char szSystemCmdPath[128+1];    
    char szNewFileName[24+1];

	vdDebug_LogPrintf("--vdDeleteBMP--");
	
    memset(szSystemCmdPath, 0x00, sizeof(szSystemCmdPath));
    sprintf(szSystemCmdPath, "rm %s", "/data/data/pub/Print_BMP.bmp");
    system(szSystemCmdPath);

	vdDebug_LogPrintf("szSystemCmdPath[%s]", szSystemCmdPath);
}

int inPrintConfirmation(char* szTitle, BOOL fIncludeLogoHeader)
{
	int inKey = d_NO;
	BYTE szLogoPath[512];
    BYTE szMsg[512];
    BYTE szPrintMsg[512];
	BYTE szDisMsg[512];

	vdDebug_LogPrintf("--inPrintConfirmation--");
	vdDebug_LogPrintf("feReceiptEnable=[%d]", strTCT.feReceiptEnable);
	vdDebug_LogPrintf("szHeaderLogoName=[%s]", strHDT.szHeaderLogoName);
	
	if (strTCT.feReceiptEnable != 1)
		return d_NO;

	memset(szMsg, 0x00, sizeof(szMsg));
    memset(szPrintMsg, 0x00, sizeof(szPrintMsg));
	memset(szLogoPath, 0x00, sizeof(szLogoPath));	
	
	if (fIncludeLogoHeader)
	{
		if (strlen(strHDT.szHeaderLogoName) > 0)
			sprintf(szLogoPath, "%s%s", LOGO_PATH, strHDT.szHeaderLogoName);
		else
			sprintf(szLogoPath, "%s%s", LOGO_PATH, "logo.bmp");
	}
	else
	{
		sprintf(szLogoPath, "%s%s", LOGO_PATH, "blank.bmp");
	}

	vdDebug_LogPrintf("szLogoPath=[%s]", szLogoPath);
	
    strcpy(szMsg, "Print Confirmation");

	strcpy(szPrintMsg, szLogoPath);
	strcat(szPrintMsg, "|");
	strcat(szPrintMsg, szMsg);
		
	inKey = PrintReceiptUI(szPrintMsg);
	vdDebug_LogPrintf("inKey=[%d]", inKey);
				
	if (inKey == d_OK) 
	{
		// sidumili: check printer when selecting "PRINT"
		if( printCheckPaper()==-1)
    		return d_NO;
		
		memset(szDisMsg, 0x00, sizeof(szDisMsg));
		strcpy(szDisMsg, szTitle);
		strcat(szDisMsg, "|");
		strcat(szDisMsg, "PRINTING...");
		usCTOSS_LCDDisplay(szDisMsg);
	        
        return d_OK;
    }
	
    if (inKey == 0xFF) // timeout
    {
		vdDisplayMessageBox(1, 8, "", "TIME OUT", "", MSG_TYPE_TIMEOUT);
		CTOS_Beep();
		CTOS_Delay(1000);
		
		return d_NO;		
    }
	
    if (inKey == d_USER_CANCEL)
    {
		vdDisplayMessageBox(1, 8, "", "USER CANCEL", "", MSG_TYPE_INFO);
		CTOS_Beep();
		CTOS_Delay(1000);
		return d_NO;		
    }
}

void vdPrintHeaderLogo(void)
{
	BYTE szLogoPath[256 + 1];

	vdDebug_LogPrintf("--vdPrintHeaderLogo--");
	
	memset(szLogoPath, 0x00, sizeof(szLogoPath));		
    sprintf(szLogoPath, "%s%s", LOGO_PATH, strHDT.szHeaderLogoName);
	vdCTOSS_PrinterBMPPicEx(0, 0, szLogoPath);
}

extern BYTE szQRString[2048+1];

void vdPrintQRCode(void){

    char szTemp1[d_LINE_SIZE + 1];
    char szAmtBuf[24+1] = { 0 };
	char szStr[24+1] = { 0 };


	vdCTOS_DispStatusMessage("PROCESSING");
	vdCTOSS_PrinterStart(100);


	vdCTOSS_PrinterBMPPic(0, 0, strHDT.szHeaderLogoName);

	vdDebug_LogPrintf("ushCTOS_printAll-2");

	inCTOS_SelectFont(d_FONT_FNT_MODE,d_FONT_24x24,0," ");
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);	

	//ushCTOS_PrintHeader(0);
	
	if(strlen(strMMT[0].szRctHdr1) > 0)
		vdPrintCenter(strMMT[0].szRctHdr1);
	//printTIDMID(); 
	printMIDTID();

    vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	vdCTOS_PrinterFline(1);
	
	vdCTOSS_PrinterBMPPic(0, 0, "phqr.bmp");
	vdCTOSS_PrinterEnd();

	vdCTOSS_PrinterStart(100);

	usCTOSS_PrintQRCodeUI(szQRString);
	//vdCTOS_PrinterFline(2);

	vdCTOSS_PrinterEnd();

	
	vdCTOSS_PrinterStart(100);

	//vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
	vdSetGolbFontAttrib(d_FONT_12x24, NORMAL_SIZE, NORMAL_SIZE, 0, 0);
	
	if(strlen(strMMT[0].szRctHdr1) > 0)
		vdPrintCenter(strMMT[0].szRctHdr1);
	
	if(strlen(strMMT[0].szRctHdr2) > 0)
		vdPrintCenter(strMMT[0].szRctHdr2);

	wub_hex_2_str(srTransRec.szBaseAmount, szTemp1, AMT_BCD_SIZE);


	memset(szStr, 0, sizeof(szStr));
	memset(szAmtBuf, 0, sizeof(szAmtBuf));

	vdCTOS_FormatAmount("NNN,NNN,NNn.nn", szTemp1, szAmtBuf);
	
	sprintf(szStr, "%s %s", strCST.szCurSymbol, szAmtBuf);

    vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
	
	vdCTOS_PrinterFline(1);

	vdPrintCenter(szStr);

	
	vdCTOS_PrinterFline(1);

	//inPrintLeftRight(szStr, szAmtBuf, inWidth);

	
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);	
	
	vdPrintCenter("Pay QR via Instapay P2M");
	
	vdPrintCenter("Show this QR CODE to the customer to");

	
	vdPrintCenter("accept payment");
	
	
	vdCTOS_PrinterFline(4);

	vdCTOSS_PrinterEnd();


}

int iPrintPHQRBody(int page)
{
	char szTemp[d_LINE_SIZE + 1] = {0};
	char szTemp4[d_LINE_SIZE + 1] = {0};
	char szTemp5[d_LINE_SIZE + 1] = {0};
	char szAmountBuff[d_LINE_SIZE + 1] = {0};
	char szTrxnDateTime[20 + 1] = {0};
	
	vdDebug_LogPrintf("--iXMLPrintWechatBody--");
	vdDebug_LogPrintf("page[%d]", page);
	vdDebug_LogPrintf("srTransRec.HDTid[%d]", srTransRec.HDTid);


	vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);

	inPrint("P2M QRPH SALE");

	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);

	//vdPrintTrxnType(FALSE);
	
	vdPrintTIDMIDVersion();
	vdPrintDateTime();
		
    //vdCTOS_PrinterFline(1); // request to remove extra newline - sidumili


	vdPrintBatchNoInvoiceNo();
	

	//vdCTOS_PrinterFline(1); // request to remove extra newline -- sidumili
	
	// Wechat Transaction ID
	
	
	memset(szTemp, 0x00, sizeof(szTemp));
	sprintf(szTemp, "TXN REFERENCE #: %s", srTransRec.szXMLtransaction_id);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	
	memset(szTemp, 0x00, sizeof(szTemp));
	sprintf(szTemp, "BANCNET INVOICE #: %s", srTransRec.szXMLout_transaction_id);
	inCCTOS_PrinterBufferOutput(szTemp,&stgFONT_ATTRIB, fERMTransaction);

	
	vdCTOS_PrinterFline(1);

	vdSetGolbFontAttrib(d_FONT_16x16, DOUBLE_SIZE, DOUBLE_SIZE, 0, 0);
	// Total	
	memset(szTemp, 0x00, sizeof(szTemp));
	memset(szTemp4, 0x00, sizeof(szTemp4));
	memset(szTemp5, 0x00, sizeof(szTemp5));
	memset(szAmountBuff, 0x00, sizeof(szAmountBuff));
	wub_hex_2_str(srTransRec.szBaseAmount, szTemp, AMT_BCD_SIZE);
	vdCTOS_FormatAmount("NN,NNN,NNN,NNn.nn", szTemp, szTemp4);
	//sprintf(szAmountBuff,"%s %s",srTransRec.szXMLlocal_fee_type, szTemp4);

	sprintf(szTemp5, "%s%s", strCST.szCurSymbol, szTemp4);

	inPrintLeftRight("TOTAL:",szTemp5,24);


	
	vdSetGolbFontAttrib(d_FONT_16x16, NORMAL_SIZE, DOUBLE_SIZE, 0, 0);
	
	return d_OK;
}

