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

void app_test_led_uart_switch(void)
{
   
    
    if(Api_Get_Switch_Status(SWITCH0))
    {
       UART_1_WriteTxData(0x55);
       Api_Led_Control(LED0,BLINK_500);
    }
    else
    {
        UART_1_WriteTxData(0xBB);
        Api_Led_Control(LED0,LED_OFF);
    }
}

/* [] END OF FILE */
