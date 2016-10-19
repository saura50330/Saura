

/***************************************************************************
* HEADER-FILES (Only those that are needed in this file)
****************************************************************************/
#include "sd_card.h"
#include <project.h>
#include <Global.h>
#include <stdio.h>

struct SdSTSLST SdInf;

/* START: FUNCTION DEFINITION */

void SDC_InitializeCard(void)
{
	FS_Init();
    SdInf.Init = 1;
}

uint8 SDC_FormatCard(void)
{
	uint8 u8Return =0;		// 0: Success, 1: Failure
	char sdVolName[10];
	
	// Get volume name
	FS_GetVolumeName(0u, &sdVolName[0], 9u);

	if(0 != FS_FormatSD(sdVolName))
	u8Return =1;
	
    return(u8Return);	
}

uint8 SDC_OpenFile(char *sdfile, const uint8 Handle, const char *mode)
{
	uint8 u8Return =0;
	if(Handle < 4)
	SdInf.pFile[Handle] = FS_FOpen(sdfile, mode);
	
	if(SdInf.pFile[Handle] == NULL)
	u8Return = 1; // Indicate failure

	SdInf.u32VFile_Size[Handle] = FS_GetFileSize(SdInf.pFile[Handle]);
    
    if(SdInf.u32VFile_Size[Handle] == 0xFFFFFFFF)
    u8Return = 1; // Indicate failure
 
	return(u8Return);
}

uint8 SDC_CloseFile(uint8 Handle)
{
	// Return 0 : Success, Return 1: Failure , Refer emFile
	uint8 u8Return =0;
	if(Handle < 4)
	u8Return = FS_FClose(SdInf.pFile[Handle]);
	return(u8Return);
}

// 10 ms cyclic call
void SDC_LoadBuffer()
{	
   	
		// Fetch Data from SD and convert it to RSU data format
		// u16BytesRead = FS_Read(pFile[0], u8A_SD_Buff_RSU0, u16V_RSU0_Buf1_cnt);
		
	
}



/*************************************************************************
*   Function 	- SDC_WriteCrashData
*	Description - Write data to SD CARD
*	Input		- 	u8Buff  : Source address of data to be written to SD card
*					u16Size : # of bytes to be written to SDC
*					u8handle: File handle to access the file
**************************************************************************/
void SDC_WriteCrashData(uint8* u8Buff, _UINT16 u16Size, uint8 u8handle)
{
	FS_Write(SdInf.pFile[u8handle],u8Buff,u16Size);
}

/*************************************************************************
*	Function	- SDC_Cnvrt_To_ASCII
*	Description	- Converts Hex value to ASCII
*	Input		- ASCII value
*	Return		- Hex value
*	Note		- None
*************************************************************************/
uint8 SDC_Cnvrt_To_ASCII(uint8 u8Input)
{
	uint8 u8Out =0;
	if(u8Input <=0x09)
	{
		u8Out = 0x30 + u8Input;
	}
	else
	{
		u8Out = 0x37 + u8Input;
	}
	return(u8Out);
}

// 250 ms cyclic call
void SDC_CardDetection(void)
{
    static uint8 u8v_Debounce_Counter = 0;

    
    if(Pin_SD_Detect_Read() == 0)
    {   // Card is inserted
        if(u8v_Debounce_Counter < 5)
		{
			u8v_Debounce_Counter++;
			if((u8v_Debounce_Counter == 4u) && (SdInf.Init == 0))
			{
                FS_DeInit();
                SDC_InitializeCard();   // Re- initialize card.
                u8v_Debounce_Counter = 10u;               
			}
		}
    }
    else
    {	// Card is removed
		if(SdInf.Init == 1u)
		{
			// Turn on RED LED
			u8v_Debounce_Counter = 0;
            SdInf.Init = 0;
		}
    }
}

/* START: FUNCTION DEFINITION */

/* [] END OF FILE */
