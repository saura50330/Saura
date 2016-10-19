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
//#include <stdio.h>
// usigned char str[12];
void lcd_ini(void)
{
    LCD1_Start();
	LCD1_Position(0,0);
    
    //LCD1_PrintString("Multi-Port LCD");
    //LCD1_Position(1,0);
	// LCD1_PrintInt8(bargraph);                  /* Show numerical position */
    
    
    //sprintf(str,"%.7f ",variable_name);
    // LCD1_PrintString((str);
}

/* [] END OF FILE */
