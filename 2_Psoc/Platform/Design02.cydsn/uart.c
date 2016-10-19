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
void Uart_Ini(void)
{
  UART_1_Start();  
  //UART_1_EnableRxInt();
  //UART_1_EnableTxInt();
  
}
void Uart_DeIni(void)
{
  UART_1_Stop();  
  UART_1_DisableRxInt();
  UART_1_DisableTxInt();
}


/* [] END OF FILE */
