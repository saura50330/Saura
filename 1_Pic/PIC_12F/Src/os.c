// OS Ini
#include "common.h"
#include "adc.h"
#include "eep_pub.h"
#include "system.h"
#include "os_priv.h" /*private functions*/
#include "os_pub.h" /*private functions*/


/*public functions of other modules nesesory*/

#include "app_process_pub.h" /*all user process function prototypes*/

#include "sevlb_pub.h"


/*task states*/
#define  SUSPENDED 0
#define  READY 1
#define  RUNNING 2

/*task priorities*/
#define TEN_MS_TASK 1
#define HUNDRED_MS_TASK 3
#define THOUSAND_MS_TASK 4
#define NO_TASK_RUNNING 100
//_UINT16 rx_data;
/* this function is called while context switching to switch between takes*/
volatile static _UINT8 task_10ms_state,task_100ms_state,task_1000ms_state;
 
void (*Task_Ptr) ();


/*This function initialises task pointer and task states*/
inline void Os_Ini(void)
{

    Task_Ptr=&Os_10ms_Task;
    task_10ms_state=SUSPENDED;
    task_100ms_state=SUSPENDED;
    task_1000ms_state=SUSPENDED;
    
    //--------timer0 interrut init----------------
    OPTION_REGbits.PSA=0;
    OPTION_REGbits.PS2=0;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.T0SE=0; //prescaler =16 (last 3 bits) so 2 mhz /16 is the clock input to timer 0.126 MHz (so 126 is the timer value to get 1ms interrupt)
    TMR0=TIMER_0_1MS; // value to get 1 ms delay // Timer T0 counts from//256-126 =131
    T0IE =1;     // Enable interrupt TMR0       OR T0IE =1 // timer 0 interrupt enale with globeel interrupt bit 1
    T0IF =0; // clear timer0 interrupt flag
}

inline void app_ini(void)
{
	app_sine_wave_ini();
}
inline void app_speciel(void)  // executes every 0.312 ms , as pwm bit field is 32 bit , so invertor 1/2 cycle complets in 32*0.312 = 10 ms
{
    app_sine_wave();
}

/*this is 10ms task function and have highet priority no other task can context switch this task*/
void Os_10ms_Task(void)
{
	/*set task to running state*/
	task_10ms_state=RUNNING;
	

     #if((HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_6==ADC)||(HW_PIN_CFG_5==ADC)||(HW_PIN_CFG_3==ADC))
	 adc_updator();
	#endif
	/*process 1 code comes hear*/
        //delay_ms(2);
	/*process 2 code comes hear*/

	/*set task state to suspended after all processe are completed*/
	task_10ms_state=SUSPENDED;
	
}

void Os_100ms_Task(void)
{
	//_UBYTE temp;
	task_100ms_state=RUNNING;

    /*process 1 code comes hear*/
     app_sine_wave_control();
     uart_frame_mgr();
      	
	//temp=uart_tx(0x7F);
	task_100ms_state=SUSPENDED;
}
void Os_1000ms_Task(void)
{

        task_1000ms_state=RUNNING;
        
         Charge_Controller();
        //uart_tx(0x7F);
	 usege_timer();
	
	    task_1000ms_state=SUSPENDED;
}

inline void Os_Check(_UINT8 Os_Current_Running_task)  // context switching
{
	/*if current running task is 10 ms task do not context switch as it is of higher priority*/
	if((Os_Current_Running_task==TEN_MS_TASK))  // no co-operation
	{
		return;
	}
	/*if 10ms task is ready suprees other task and switch to 10ms task*/
	else if((task_10ms_state==READY)&&(Os_Current_Running_task>TEN_MS_TASK)) // suppress any task running more then 10 ms
	{
		Task_Ptr=&Os_10ms_Task;
	}
	/*if 100ms task is running and 10ms task is suspended */
	else if((Os_Current_Running_task==HUNDRED_MS_TASK)) // suppress any task running less then 20 ms
	{
		return;
	}
	/*if 100ms task is ready and 10ms task is suspended and 1000 ms task is running*/
	else if((task_100ms_state==READY)&&(Os_Current_Running_task>HUNDRED_MS_TASK)) // suppress any task running more then 100 ms
	{
		Task_Ptr=&Os_100ms_Task;
	}
	/*current task running is 1000ms task*/
	else if(Os_Current_Running_task==THOUSAND_MS_TASK)
	{
		return;
	}
	/*if 1000ms task is ready*/
	else if((task_1000ms_state==READY))
	{
		Task_Ptr=&Os_1000ms_Task;
	}
	/*if all task are suspended*/
	else
	{
		return;
	}	
	/*call for task in case of context switching*/
	(*Task_Ptr) ();
        
}
void main(void)
{
	
	// initialise pins
        systm_ini_proc_1();
       
        delay_ms(3000); // wait for stbility delay
        //initialise timer and isr

        systm_ini_proc_2();/*gpio for fail safe, os timer ,adc, ccp, pwm, interrupt*/

        
	while(1)
	{
		Os_Check(NO_TASK_RUNNING);
	}
}

/* below is 1ms timer ISR for OS*/
inline void Os_Timer_ISR(void)
{
	static _UBYTE ms1_ctr,ms10_ctr,ms100_ctr;
	/*task states*/
        task_10ms_state=((ms1_ctr==10)?READY:task_10ms_state); /* change the state of task to ready only if time is 10ms else retain the state*/
		if(ms1_ctr==10)
		{
		ms1_ctr=0;ms10_ctr=ms10_ctr+1;
		}
	task_100ms_state=((ms10_ctr==10)?READY:task_100ms_state);
		if(ms10_ctr==10)
		{
		ms10_ctr=0;ms100_ctr=ms100_ctr+1;
		}
	task_1000ms_state=((ms100_ctr==10)? READY:task_1000ms_state);
        if(ms100_ctr==10)
		{
		ms100_ctr=0;
		}
	/*Os timer*/
	ms1_ctr=ms1_ctr+1;
        // clear timer registeor
        T0IF=0;// reset timer interrupt flg
        TMR0=TIMER_0_1MS; // reload the timer
}