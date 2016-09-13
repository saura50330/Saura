
#include "os.h" /*private functions*/

const _UBYTE BACKGROUND_TASK=(NO_OF_TASK-1);
Timer_Os Kernel_stTimer [KERNEL_MAX_SW_TIMERS];
// Now initialise the OS perameters
Os Nano_Os=
{
Os_10ms_Task,         // initialise all posible tasks
Os_50ms_Task,
Os_100ms_Task,
Os_200ms_Task,
Os_1000ms_Task,
Os_Back_Ground_Task,
0,                    // start executing from first function in the task
0,
0,
0,
0,
0,
NOT_READY, 			 // Initialise task status
NOT_READY,
NOT_READY,
NOT_READY,
NOT_READY,
READY,                       // back ground task is alwas in READY state
0xFF,      			 // enable all function tasks
0xFF,
0xFF,
0xFF,
0xFF,
0xFF
};
_UBYTE i;
tFuncPtr Os_Task_Ptr_Temp;
void main(void) 
{
   // Initialise system drivers and application
	Kernel_vRunContainer(Kernel_vHWReset);  // calls and executes all function in function pointer array Kernel_vHWReset // harware initialization/Applicatioinitialization/HW ISR call
	
    //TODO // configure watchdog  ... its disabled hear watchdog can be of 20ms as minimum task here is of 10ms
	while(1)
	 {
	   //TODO// reset watch dog  if program do not return here within 20ms watchdog resets      
       
	  // Preemptive sheduler logic
	  Os_Task_Ptr_Temp=0; // set task pinter to null
	   for(i=0;i<NO_OF_TASK;i++) // go through all tasks
	   {
		  //BP_TOTAL_TIME_CO_OP_TASK_START	   // masure this when all tasks are redy ie when 1sec task is ready measure time for i=0 to i= 4
		  //BP_TOTAL_TIME_CO_OP_TASK_END		  
		  if(Nano_Os.Os_ms_Task_Sts[i]==READY)
		   {
				if((*(Nano_Os.Os_ms_Task_Ptr[i]+ Nano_Os.Os_ms_Task_Func_Num[i]))!=0) // check if task function is not empty
			   {
					if((Nano_Os.Os_ms_Task_Func_Mask[i]>>Nano_Os.Os_ms_Task_Func_Num[i])&0x01) // sub task is enabled(max 8 sub tasks _UBYTE), any task can be disabled or enabled by propery configuring the bits
					{
						Os_Task_Ptr_Temp=(*(Nano_Os.Os_ms_Task_Ptr[i] +Nano_Os.Os_ms_Task_Func_Num[i] ));             // address of 1st function in task maped
						  // task funtion selected for execution
					}
					//Nano_OS.Os_ms_Task_Ptr[i]=Nano_OS.Os_ms_Task_Ptr[i]+1;  // execute one task after aother from 10ms container
					Nano_Os.Os_ms_Task_Func_Num[i]++; // next sub task
					break; // go and execute the task
			   }
			   else // if task is empty no task get executed
			   {
					//Nano_OS.Os_ms_Task_Ptr[i]=Nano_OS.Os_ms_Task[i]; // when next timer tick rech 10ms start from begining.
					Nano_Os.Os_ms_Task_Func_Num[i]=0;
					if(i<(BACKGROUND_TASK)) // not a back ground task
					{
						Nano_Os.Os_ms_Task_Sts[i]=NOT_READY; // criticl section
					}
					// all task in 10ms are executed or its empty		
					// note : make timer function as a part of background task
					// make eep periodic function as part of background task
			   }
		   }
		}
		// vaue of variable i vs time can be plted to find which task getting executed
		// as total 3 task are thare 3 bits pin are enough to detect which task getting executed
		if(Os_Task_Ptr_Temp) // if task value is not empty
		{
			(*Os_Task_Ptr_Temp)();  // run the task function which is selected by sheduler
		}	
		//# Preemptive sheduler logic Ends
		//software reset can be written heare as its empys all stack
	 } // while loop ends
 } // main ends
inline void Kernel_Pre_Emptiv_Timer_ISR(void)
{
    // put premptive function here called every 1ms

}
/* below is 10ms timer ISR for OS*/
inline void Kernel_Co_Op_Timer_ISR(void)
{
	static _UBYTE ms10_ctr,ms50_ctr,ms200_ctr;

        /*task states*/
	Nano_Os.Os_ms_Task_Sts[0]=READY;
	if(ms10_ctr==5)  // 
	{
    	ms10_ctr=0;
		Nano_Os.Os_ms_Task_Sts[1]=READY;
		ms50_ctr+=1;
		switch(ms50_ctr)
		{
		  case 4:Nano_Os.Os_ms_Task_Sts[3]=READY;
				 ms50_ctr=0;
				 ms200_ctr+=1;
				 if(ms200_ctr==5)
				{
					ms200_ctr=0;
					Nano_Os.Os_ms_Task_Sts[4]=READY;
				}
		  case 2:Nano_Os.Os_ms_Task_Sts[2]=READY;
				 break;
		  default: break;
		}				
	}
	// timer values update for system timers
	#if(KERNEL_MAX_SW_TIMERS>0)
	for(_UBYTE i = 0;i<KERNEL_MAX_SW_TIMERS;i++)
        {
		 // if task is unlocked     
		 if(Kernel_stTimer[i].Lock == TRUE)
		 {
			// if task is ready to execute		
			if(Kernel_stTimer[i].TimerValue != 0)
			{
				Kernel_stTimer[i].TimerValue--;  // 10 ms resolution
			}
		}
	}
	#endif
	ms10_ctr+=1;		
	// this isr will get overloade every 1 second, as all task status gose to ready state. and all task code get executed.
        

}
void Kernel_StartStop_Task_Func(_UBYTE Task_No , _UBYTE Func_mask)  // use this function care fully as if wrong mask to wrong task will make system idle
{
	if(Task_No<NO_OF_TASK) // task number is valid
	{
		Nano_Os.Os_ms_Task_Func_Mask[Task_No]=Func_mask;
	}
}

void Kernel_vRunContainer(const tFuncPtr *afpContainerArray)
{
   
   while (*afpContainerArray != 0) // not reached till end?
      {
        (*afpContainerArray)();    // execute function
         afpContainerArray++ ;     // next function 
      }    
  }
#if(KERNEL_MAX_SW_TIMERS>0)

void Kernel_bSetTimer( _UBYTE bTimerVal, _UBYTE *bTimerID ,  tFuncPtr cb )
/*
*
* Description           : Timer Value in ms
* Direction             : In
* Modified              : No
* Range                 : 0 - 0xFF
*
*/
 // timer id variable address


/*
*
* Description           : Callback function 
* Direction             : In
* Modified              : No
* Range                 : N.A
*
*/
{

  /***************************************************************************
   * FUNCTION LOCAL DEFINITIONS
   *
   * In this section define
   * - all function local type definitions
   * - all function local variables (auto and static)
   ****************************************************************************/
	 int i;
	 

   /***************************************************************************
   * FUNCTION CODE
   ****************************************************************************/
   
   for(_UBYTE i = 0;i<KERNEL_MAX_SW_TIMERS;i++)  //  run throug all timers
   {
      if(Kernel_stTimer[i].Lock == FALSE) // if timer is empty
      {   
         Kernel_stTimer[i].Lock = TRUE; // enable it
         Kernel_stTimer[i].TimerValue = bTimerVal; // this value will be decremented
         Kernel_stTimer[i].TimerValueBk = bTimerVal; // this is to remember value
         if(cb != 0) // map timer function
         {
            Kernel_stTimer[i].cb = cb;
         }
         //* bTimerID = Kernel_stTimer[i].TimerID; // original
         *bTimerID = i; // timer value
         break;
      }
      else 
      {
          *bTimerID = 0xFF; // no timer allocated
      }

   }
} 
/*******************************************************************************
 *  Interface Description: This fucntion is called by the applciaiton
 *                         modules to stop and kill a Timer. 
 *                           
 *  Return Value (Value Type Range): byte
 *          Range : 1 to 5 - Timer ID
*******************************************************************************/

void  Kernel_boStopTimer( _UBYTE bTimerID)
/*
*
* Description           : Timer ID got through SetTimer.
* Direction             : In
* Modified              : No
* Range                 : 0 - (KERNEL_MAX_SW_TIMERS-1)
*
*/
{
    if(bTimerID<KERNEL_MAX_SW_TIMERS)
	{
	 if(Kernel_stTimer[bTimerID].Lock == TRUE)
	   {
			Kernel_stTimer[bTimerID].Lock = FALSE;
			Kernel_stTimer[bTimerID].cb = 0;
			
       } 
	}
}


/*******************************************************************************
 *  Interface Description: This function is called by the application
 *                         module to reset the timer count value.
 *                           
 *  Return Value (Value Type Range): byte
 *          Range : 1 to 5 - Timer ID
*******************************************************************************/

void Kernel_boResetTimer( _UBYTE bTimerID)
/*
*
* Description           : Timer ID got through SetTimer. 
* Direction             : In
* Modified              : No
* Range                 : 0 - KERNEL_MAX_SW_TIMERS-1
*
*/
{
 if(bTimerID<KERNEL_MAX_SW_TIMERS)
 {
	if(Kernel_stTimer[bTimerID].Lock == TRUE)
	{ 
		Kernel_stTimer[bTimerID].TimerValue = Kernel_stTimer[bTimerID].TimerValueBk ;           
	} 
 }
} 
void Sw_Timer_Updator(void)  // put this in back ground task
 {
		 // manage software timer based tasks
		    for(_UBYTE i = 0;i<KERNEL_MAX_SW_TIMERS;i++)
		   {
			 // if task is unlocked     
			 if(Kernel_stTimer[i].Lock == TRUE)
			  {
			  // if task is ready to execute		
				if(Kernel_stTimer[i].TimerValue == 0)
				{
					// execute the function assigned on ti,e out
					if(Kernel_stTimer[i].cb != 0)
					{   
						(*Kernel_stTimer[i].cb)();
						//(Kernel_stTimer[i].cb)();
					}  		
					Kernel_stTimer[i].TimerValue = Kernel_stTimer[i].TimerValueBk;
				}
			  }
			}
}
//# end of software bsased tasks
// example of timer usage
/*
RISFbits.TRISF5 = 0;	//Configure PIN as Output
                  LATFbits.LATF5 = 1; 	// Make trigger line high  
                  Kernel_bSetTimer(TRIG_MAIN_PULSE_WIDTH, &u8TrigMain_TimerID, &CANH_TrigMain_Timeout_Callback);
				  

void CANH_TrigMain_Timeout_Callback(void)
{
	LATFbits.LATF5 = 0; 	// Make trigger line Low 
	TRISFbits.TRISF5 = 1;	// Configure port F5 as input 
	Kernel_boStopTimer(u8TrigMain_TimerID);
}*/
 
 
#endif

inline void Os_Ini(void)
{
    //--------timer0 used for OS interrupt of 1ms interrut init for 1ms pre emptive tasks----------------
    OPTION_REGbits.PSA=0;
    OPTION_REGbits.PS2=0;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.T0SE=0; //prescaler =16 (last 3 bits) so 2 mhz /16 is the clock input to timer 0.126 MHz (so 126 is the timer value to get 1ms interrupt)
    TMR0=TIMER8_1_MS; // value to get 1 ms delay // Timer T0 counts from//256-126 =131
    T0IE =ENABLE;     // Enable interrupt TMR0       OR T0IE =1 // timer 0 interrupt enale with globeel interrupt bit 1
    T0IF =0; // clear timer0 interrupt flag

    //TIMER 1 to gernerate 10ms interrupt fo cooperative tasks 10ms to 1000ms
    
    TMR1CS=0; // use internal clock fos/4
    T1OSCEN = 0; // do not enable oscillator
    TMR1=TMRER16_10_MS;  // 10ms interrupt
    TMR1ON=ON; //enable timer
    TMR1IE=ENABLE; // enable timer 1 interrupt
    TMR1IF=0;

    //TIMER 2  general purpose timer 2 TODO: not initialised if pwm is disbaled
    #if((HW_PIN_CFG_5==PWM)||(HW_PIN_CFG_6==PWM)||(HW_PIN_CFG_7==PWM)||(HW_PIN_CFG_14==PWM))
    #else
    T2CON=set_bit_position(T2CON,0); // st TMR2ON bits of timer prescalar(4)
    PR2=156;// value of TMR2 when reches 0xFF interrupt occurs
    TMR2ON=ON; //enable timer
    TMR2IE=ENABLE; // enable timer 2 interrupt
    #endif
    PEIE=1;// enable periferal interrupt (used for timer2)
}
