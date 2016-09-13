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
#include "common.h"
#include "os_kernel_pub.h"
#define KERNEL_MAX_SW_TIMERS 2
#define NO_OF_TASK 6
#define BACKGROUND_TASK (NO_OF_TASK-1)
#define NOT_READY 0
#define READY 1
#define TRUE 1
#define FALSE 0

// complete OS functionality can be controlled using  Nano_OS// RAM : 30 bytes, code : 68 instructions approx including ISR
typedef struct
{
    // task function pointer
	const tFuncPtr *Os_ms_Task_Ptr[NO_OF_TASK];// stores address of function pointer to different tasks(total  tasks) functions
	
	//Task thread counter
	_UBYTE Os_ms_Task_Func_Num[NO_OF_TASK]; // Function counter which tells  which function is running in the task
	
	//task status 
	volatile _UBYTE Os_ms_Task_Sts [NO_OF_TASK]; // tells if task is ready to execute, READY,NOT_READY //volatile is updated by ISR
	
	//function supressor
	 _UBYTE Os_ms_Task_Func_Mask[NO_OF_TASK]; // by seeting respective bits of this any function in task can be supreesed
         
}Os;
typedef struct
{
  _UBYTE Lock;  // to enable or disable timer tasks
  volatile _UBYTE TimerValue;   // execution backup // volatile: updated in isr
  _UBYTE TimerValueBk;  // call back execution period backup
  tFuncPtr  cb; // callback function

}Timer_Os;

// Initialisation task and it uses os container function 
const tFuncPtr Kernel_vHWIni[] =  // its an array of function pointer of type cont returning void and perameter void
{
	//(void (*const)()) Systm_Ini_Proc_1, // configure all harware specific pins and peripherals
	(void (*const)()) Uart_Ini,
    //(void (*const)()) Eep_Ini,
	(void (*const)()) Os_Ini,
	//(void (*const)()) App_Ini, // try to make this out of the call
	//(void (*const)()) Systm_Ini_Proc_2, // enable all ISR
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 10ms task
const tFuncPtr Os_10ms_Task[] = 
{
	
	//(void (*const)()) Uart_Manager,
	//(void (*const)()) adc_updator,
        
	//(void (*const)()) Task1_10ms, 
	//(void (*const)()) Task2_10ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 50ms task
 const tFuncPtr Os_50ms_Task[] = 
{
    // (void (*const)()) Api_Uart_Read_Rx,  // manages all communication requests and response
    //(void (*const)()) Task1_50ms,
	//(void (*const)()) Task2_50ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 100ms task
 const tFuncPtr Os_100ms_Task[] = 
{
	(void (*const)()) Switch_Manager,
    (void (*const)()) app_test_led_uart_switch,
	//(void (*const)()) Task2_100ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 200ms task
const tFuncPtr Os_200ms_Task[] = 
{
	//(void (*const)()) System_Status_Indicator, // syatwm led which blinks at differant frequency to indicate differant system status 0.4hz 1hz 2hz (1hz is normal, 0.5hz is error , 2hz to indicate system is normal and some application event accurd )
	(void (*const)()) Led_Manager,
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 1000ms task   // vary slow events can be mapped to this function
 const tFuncPtr Os_1000ms_Task[] =
{
	//(void (*const)()) usege_timer,
    //(void (*const)()) Task1_1000ms,
	//(void (*const)()) Task2_1000ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// back ground task // keep here tasks which need frquent attentions, and no time determint task // this task will always execute when there are no other task are redy.

 const tFuncPtr Os_Back_Ground_Task[] = 
{
	//(void (*const)()) Eep_Manager,
	#if(KERNEL_MAX_SW_TIMERS>0) 
	(void (*const)()) Sw_Timer_Updator,
	#endif
	(void (*const)()) 0  // void zero is used for ending task 
};




/* [] END OF FILE */
