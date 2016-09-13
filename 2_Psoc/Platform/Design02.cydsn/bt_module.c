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
#include <project.h>
#include "common.h"
#include "bt_module.h"
static uint8  stBtModule; // wifi module state machine status
void bt_module_manager(void)  // called every 1second task due to slow responce
{
   static uint8 CommandNum; 
       switch(stBtModule)
       {
           case BT_IDLE : break; 
           case BT_INI: UART_1_PutString(BtIniCmd[CommandNum]);   // send all required initialization commands every 100 ms 
                      CommandNum++;
                      if((*BtIniCmd[CommandNum])==0x00)  //nonly null is end of coomand
                      {
                            stBtModule=BT_IDLE;
                            CommandNum=0;
                      }
                      break;
            default:break;
       }
}
void bt_module_config(uint8 Command)
{
    switch(Command)
    {
        case BT_INI:
                     stBtModule=BT_INI; 
                     break;
        default:break;
    }
}

/* [] END OF FILE */
