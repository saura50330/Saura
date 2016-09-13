/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
// this moule uses the uar as base for tx and rx
#include <project.h>
#include "common.h"
#include "wifi.h"
static uint8  stWifiModule; // wifi module state machine status
void wifi_module_manager(void)  // called every 100ms
{
   static uint8 CommandNum; 
       switch(stWifiModule)
       {
           case WIFI_IDLE : break; 
           case WFI_INI: UART_1_PutString(WifiIniCmd[CommandNum]);   // send all required initialization commands every 100 ms 
                      CommandNum++;
                      if((*WifiIniCmd[CommandNum])==0x00)  //nonly null is end of coomand
                      {
                            stWifiModule=WIFI_IDLE;
                            CommandNum=0;
                      }
                      break;
            default:break;
       }
}
void wifi_module_config(uint8 Command)
{
    switch(Command)
    {
        case WFI_INI:
                     stWifiModule=WFI_INI; 
                     break;
        default:break;
    }
}

/* [] END OF FILE */
