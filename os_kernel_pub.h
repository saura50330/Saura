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
// OS related objects
    typedef void (*tFuncPtr)(void); // tFuncPtr is pointer to function which can have void as return type and no parameters passed
//typedef tFuncPtr* TaskFuncPtr; //TaskFuncPtr is a pointer which stores address of function pointer 
extern const tFuncPtr Kernel_vHWIni[];

void Sw_Timer_Updator(void);
inline void Kernel_Co_Op_Timer_ISR(void);
inline void Kernel_Pre_Emptiv_Timer_ISR(void);
inline void kernel(void);
void Os_Ini(void);
void Kernel_vRunContainer(const tFuncPtr *afpContainerArray);

//#######user API
void Kernel_StartStop_Task_Func(_UBYTE Task_No , _UBYTE Func_mask);  // use this function care fully as if wrong mask to wrong task will make system idle

/* [] END OF FILE */
