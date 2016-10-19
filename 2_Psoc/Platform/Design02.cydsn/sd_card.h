

#ifndef SDCARD_H
#define SDCARD_H

    
//#include <device.h>
#include "type_def.h"
#include <FS.h>
    
struct SdSTSLST
    {
        uint32 u32VFile_Size[2];  // can hold two file sizes
        uint8 Init;   // initialisation status
        FS_FILE * pFile[2];
    };
// CONSTANTS AND MASKS

#define C_SDC_Read_Cnt    	0x100u
#define C_RSU_Sample_Cnt	0x40u
#define C_Buf_Fill_Ref		0x01u
#define C_Buf_Swtch_Ref		0x40u
    

//------- u8VSDC_CAN_Flags -----------
#define M_File1_Opened		0x01u
#define M_File2_Opened		0x02u

extern uint8 u8A_LoadCrash_to_SD[1024u];

// OWN MODULE API DECLARATION
// void SDC_InitializeCard(void);
uint8 SDC_FormatCard(void);
uint8 SDC_OpenFile(char *sdfile, const uint8 Handle, const char *mode);
uint8 SDC_CloseFile(uint8 Handle);

// void SDC_LoadBuffer();
//void SDC_WriteCrashData(uint8* u8Buff, _UINT16 u16Size, uint8 u8handle);

//void SDC_Atoh(uint8 *num);
//void SDC_ASCIIToRSU1Packet(uint8 *u8Source, uint8 u8BufIndex);
//void SDC_ASCIIToRSU2Packet(uint8 *u8Source, uint8 u8BufIndex);

uint8 SDC_Cnvrt_To_ASCII(uint8 u8Input);
//void SDC_ASCIIToRSUPacket(uint8 *u8Source, uint16 *u16Destination, uint16 u16size);
//void SDC_ASCIIToRSU0_Buf1Packet();
//void SDC_ASCIIToRSU0_Buf2Packet();
//void SDC_ASCIIToRSU1_Buf1Packet();
//void SDC_ASCIIToRSU1_Buf2Packet();
void SDC_CardDetection(void);

#endif
/* [] END OF FILE */
