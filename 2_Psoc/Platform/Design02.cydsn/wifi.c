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
#if(WIFI_MODULE==ENABLE)
#include "wifi.h"
const char *WifiIniCmd[]={
                          "AT\r\n",   // check connecton responce is ok
                          "AT+CWMODE=3\r\n",  // Access point request, rep =0
                          "AT+RST\r\n",  // reset the card 
                          "AT+CWJAP=""NETGEAR"",""strongviolet962\r\n", // connect to rauter
                          "AT+CIPMUX=1\r\n", // connect to internet
                          "AT+CIPSTART=4,""TCP"",""google.com"",80\r\n",  // run website
                          ""
                         };

static uint8  stWifiModule; // wifi module state machine status
void wifi_module_manager(void)  // called every 100ms
{
   static uint8 CommandNum; 
       switch(stWifiModule)
       {
           case WIFI_IDLE : break; 
           case WIFI_INI: UART_1_PutString(WifiIniCmd[CommandNum]);   // send all required initialization commands every 100 ms 
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
        case WIFI_INI:
                    if(stWifiModule==WIFI_IDLE)
                    {             
                        stWifiModule=WIFI_INI; 
                    }
                     break;
        default:break;
    }
}
#endif
/* [] END OF FILE */
