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
#include "type_def.h"

#define MAX_LED 1 // maximum LED in system
#define MAX_SWITCH 1 // maximum switches in system
#define MAX_SWITCH_DEB_COUNT 3 // how much switch to be debaunce befor confirming the status

#define SWITCH_LOW 0
#define SWITCH_HIGH 1

typedef void (*dio_write)(_UBYTE);
typedef _UBYTE (*dio_read)(void);

typedef struct
{
    _UBYTE Led_u8_st_Led_Status_Comp[MAX_LED];
    _UBYTE Led_u8_st_Led_Status[MAX_LED];
    const dio_write led_write [MAX_LED];
    const dio_read led_read [MAX_LED];
    
     _UBYTE Switch_u8_st_Current_Status[MAX_SWITCH];
     _UBYTE Switch_u8_st_Stable_Status[MAX_SWITCH];
     _UBYTE Switch_u8_st_Deb_Count[MAX_SWITCH];
     const dio_read switch_read [MAX_SWITCH];
}Dio_Con;

/* [] END OF FILE */
