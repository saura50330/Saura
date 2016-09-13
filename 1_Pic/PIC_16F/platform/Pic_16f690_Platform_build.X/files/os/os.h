#include "common.h"

#define KERNEL_MAX_SW_TIMERS 2
# define NO_OF_TASK 6
# define NOT_READY 0
# define READY 1

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
// below is Nano_OS structure initialisation
 
// Initialisation task and it uses os container function 
static const tFuncPtr Kernel_vHWReset[] =  // its an array of function pointer of type cont returning void and perameter void
{
	(void (*const)()) Systm_Ini_Proc_1, // configure all harware specific pins and peripherals
	
	#if((HW_PIN_CFG_10==UART_TX)||(HW_PIN_CFG_12==UART_RX))
	//(void (*const)()) Uart_Ini,
	#endif
	
	//(void (*const)()) Eep_Ini,
	//(void (*const)()) Os_Ini,
	//(void (*const)()) App_Ini, // try to make this out of the call
	(void (*const)()) Systm_Ini_Proc_2, // enable all ISR
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 10ms task
static const tFuncPtr Os_10ms_Task[] = 
{
	#if((HW_PIN_CFG_10==UART_TX)||(HW_PIN_CFG_12==UART_RX))
	(void (*const)()) Uart_Manager,
	#endif
	#if((HW_PIN_CFG_19==ADC)||(HW_PIN_CFG_18==ADC)||(HW_PIN_CFG_17==ADC)||(HW_PIN_CFG_16==ADC)||(HW_PIN_CFG_15==ADC)||(HW_PIN_CFG_14==ADC)||(HW_PIN_CFG_13==ADC)||(HW_PIN_CFG_12==ADC)||(HW_PIN_CFG_9==ADC)||(HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_8==ADC)||(HW_PIN_CFG_3==ADC))
        (void (*const)()) adc_updator,
        #endif
	//(void (*const)()) Task1_10ms, 
	//(void (*const)()) Task2_10ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 50ms task
static const tFuncPtr Os_50ms_Task[] = 
{
        #if((HW_PIN_CFG_10==UART_TX)||(HW_PIN_CFG_12==UART_RX))
         (void (*const)()) Api_Uart_Read_Rx,  // manages all communication requests and response
        #endif
         //(void (*const)()) Task1_50ms,
	//(void (*const)()) Task2_50ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 100ms task
static const tFuncPtr Os_100ms_Task[] = 
{
	(void (*const)()) Switch_Manager,
	//(void (*const)()) Task2_100ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 200ms task
static const tFuncPtr Os_200ms_Task[] = 
{
	(void (*const)()) System_Status_Indicator, // syatwm led which blinks at differant frequency to indicate differant system status 0.4hz 1hz 2hz (1hz is normal, 0.5hz is error , 2hz to indicate system is normal and some application event accurd )
	(void (*const)()) Led_Manager,
	(void (*const)()) 0  // void zero is used for ending task 
};

// all functions to be executed in 1000ms task   // vary slow events can be mapped to this function
static const tFuncPtr Os_1000ms_Task[] =
{
	(void (*const)()) usege_timer,
        //(void (*const)()) Task1_1000ms,
	//(void (*const)()) Task2_1000ms, 
	(void (*const)()) 0  // void zero is used for ending task 
};

// back ground task // keep here tasks which need frquent attentions, and no time determint task // this task will always execute when there are no other task are redy.

static const tFuncPtr Os_Back_Ground_Task[] = 
{
	(void (*const)()) Eep_Manager,
	#if(KERNEL_MAX_SW_TIMERS>0) 
	(void (*const)()) Sw_Timer_Updator,
	#endif
	(void (*const)()) 0  // void zero is used for ending task 
};
