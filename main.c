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
#include "os_kernel_pub.h" /*private functions*/
int main(void)
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    
    // Initialise system drivers and application
	Kernel_vRunContainer(Kernel_vHWIni);  // calls and executes all function in function pointer array Kernel_vHWReset // harware initialization/Applicatioinitialization/HW ISR call
	CyGlobalIntEnable;
    kernel();  // karnel hendles every thing!
        /* Place your application code here. */ 
    return(0);
}
/* [] END OF FILE */
