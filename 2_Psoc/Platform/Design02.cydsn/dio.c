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
#include "dio.h"
// initialise values of dio 
Dio_Con DioCon={
    {LED_OFF},   // {,,,,}  if number of led or swith increases 
    {LED_OFF},
    {System_LED_Write},
    {System_LED_Read},
    {SWITCH_LOW},
    {SWITCH_LOW},
    {0},
    {System_Switch_Read}
};

void Api_Led_Control(_UBYTE led_no,_UBYTE led_status) //led_no = 0 to 7 , led_status= 0:OFF 1 =100ms blink ...255 =ON
{
    if(led_no<MAX_LED)
    {
       // DioCon.Led_u8_st_Led_Status_Comp[led_no]=led_status; // fo checking value
        DioCon.Led_u8_st_Led_Status[led_no]=led_status;// to be decremented
    }
}

// led manager manages led status of system
void Led_Manager(void) // call this every 200ms
{
    _UBYTE u8_temp_led,led_no;
    for(led_no=0;led_no<MAX_LED;led_no++) // go throug all led status
    {
        if(DioCon.Led_u8_st_Led_Status[led_no]==LED_OFF)
        {
            // turn off led
            (*DioCon.led_write[led_no])(0);
            
        }
        else if(DioCon.Led_u8_st_Led_Status[led_no]==LED_ON)
        {
            // turn On LED
           (*DioCon.led_write[led_no])(1);
        }
        else // blink
        {
            if(DioCon.Led_u8_st_Led_Status_Comp[led_no]<=LED_BLINK)
            {
                // toggel LED
                u8_temp_led=(DioCon.led_read[led_no])();
                (*DioCon.led_write[led_no])(!u8_temp_led);// port , pin , value
                DioCon.Led_u8_st_Led_Status_Comp[led_no]=DioCon.Led_u8_st_Led_Status[led_no]; // reload count
            }
            else
            {
                DioCon.Led_u8_st_Led_Status_Comp[led_no]--; // decrement count
            }
        }

    }
}
//Switch_Manager manages switch debounig delays and update debouce switch status
void Switch_Manager(void)  // call this every 100ms
{
    _UBYTE u8_st_temp_switch_ststus, switch_no;
    for(switch_no=0;switch_no<MAX_SWITCH;switch_no++) // go throug all led status
    {
       // read switch value
        u8_st_temp_switch_ststus=(*DioCon.switch_read[switch_no])();
       // compare switch value with old value
        if(u8_st_temp_switch_ststus==DioCon.Switch_u8_st_Current_Status[switch_no])
        {
            // if equal incerment debounce count
            if(DioCon.Switch_u8_st_Deb_Count[switch_no] == MAX_SWITCH_DEB_COUNT)  // if debunce count reaches the final value
            {
                DioCon.Switch_u8_st_Stable_Status[switch_no]=DioCon.Switch_u8_st_Current_Status[switch_no];  // update switch stable status
                DioCon.Switch_u8_st_Deb_Count[switch_no]=0;// reset debounce count
            }
            else
            {
                DioCon.Switch_u8_st_Deb_Count[switch_no]++;
            }

        }
        else
        {
            DioCon.Switch_u8_st_Deb_Count[switch_no]=0;  //if not equal reset debounce count
        }
        // update old value
        DioCon.Switch_u8_st_Current_Status[switch_no]=u8_st_temp_switch_ststus;
    }
}
_UBYTE Api_Get_Switch_Status(_UBYTE switch_no)
{
    // return stable switch status
    if(switch_no<MAX_SWITCH)
    {
        return(DioCon.Switch_u8_st_Stable_Status[switch_no]);
    }
    else
    {
        return(0);
    }
}

/* [] END OF FILE */
