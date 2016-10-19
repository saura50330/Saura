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
   
     #if(BT_MODULE==ENABLE)
        bt_module_config(BT_INI);
     #endif
     
     #if(WIFI_MODULE==ENABLE)
        wifi_module_config(BT_INI);
     #endif
    
     #if(IMU_SENSOR==ENABLE)
         if(IMU_Read_St()==NOT_INIT)
         {
         IMU_Command(IMU_CMD_INIT);
         }
         
         if(IMU_Read_St()==INIT_SUCC)
         {
            IMU_Command(IMU_CMD_START);
         } 
      #endif
    
    Api_Led_Control(LED0,BLINK_500);
    
   /* if(Api_Get_Switch_Status(SWITCH0))
    {
       
    }
    else
    {
        UART_1_WriteTxData(0xBB);
        Api_Led_Control(LED0,LED_OFF);
    }*/
}

/* [] END OF FILE */
