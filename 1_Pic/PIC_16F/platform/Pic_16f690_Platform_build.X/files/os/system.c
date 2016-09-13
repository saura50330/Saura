#include "common.h"
#include "system.h"
_UBYTE Sys_u8_st_Globle_System_Status=0;
/*ISR routines*/
void interrupt isr(void) // 10ms timer    os timer
{
  
  //GIE=0; // pause interupts during ISR execution // This was big bug and wasted lot of time ... thsis is handled by hardware
 
  // timer 0 overflow
  if(T0IF)
  {   
    //BP_TOTAL_TIME_PREEMPTIV_START
	Kernel_Pre_Emptiv_Timer_ISR();
    //clear timer registeor
    TMR0=TIMER8_1_MS; // reload the timer
    T0IF=0;// reset timer interrupt flg
	//BP_TOTAL_TIME_PREEMPTIV_END
    
  }
  if(INTF)
  {
		INTF=0;
  }
  if(RABIF) // interupt on change port b and port a bits
  {
	RABIF=0;
  }

  if(PEIE) // if peripheral ISR are enabled
  {
	// timer 1 overflow used for delays and PWM measurements and communication protocols
	if(TMR1IF)
	{
		//BP_TOTAL_TIME_CO_OP_SHEDULER_START
		Kernel_Co_Op_Timer_ISR();
        TMR1IF=0;// reset timer interrupt flg
		TMR1=TMRER16_10_MS;//10ms iterrupt time
		//BP_TOTAL_TIME_CO_OP_SHEDULER_START
	
	}
  
	// timer 2 overflows
	if(TMR2IF)
	{
  

    TMR2IF=0;
    TMR2=0x63BF;//20ms iterrupt time
	}
	#if((HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_6==ADC)||(HW_PIN_CFG_5==ADC)||(HW_PIN_CFG_3==ADC))
	// ADC interuupt
	if(ADIF) // do only if ADC interrupt is enabled
	{
		ADIF=0; // clear ADC interrupt flag
   
	}
	#endif

 
	if(RCIF)  // receive interrupt flag
	{  
		Uart_Isr();   
	}  
   // EEPROM WRITE SUCCESS
	if(EEIF)
	{
	EEIF=0; // clear the fleg
	}
 }
  //GIE=1; // enable interuupts after ISR execution // This was big bug , found and commented and wasted lot of time ... thsis is handled by hardware
}

/*DRIVER INI PROC*/
// system initialisation
void Systm_Ini_Proc_1(void)
{
	    //---------OSCIllator-------
		OSCCON |= 0b01110001;     //set int osc to 8MHz// instuction cycle freq =8mz/4 = 2 mhz
                GIE=0;
		while(!HTS);               // wait until OSC stable
		 // ----- GPIO direction selection----------------
		Dio_Ini();

                //--------ADC configuration ----------------------
		#if((HW_PIN_CFG_19==ADC)||(HW_PIN_CFG_18==ADC)||(HW_PIN_CFG_17==ADC)||(HW_PIN_CFG_16==ADC)||(HW_PIN_CFG_15==ADC)||(HW_PIN_CFG_14==ADC)||(HW_PIN_CFG_13==ADC)||(HW_PIN_CFG_12==ADC)||(HW_PIN_CFG_9==ADC)||(HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_8==ADC)||(HW_PIN_CFG_3==ADC))

                Adc_Ini();
                #endif
                // ----------------OS_INI and Timer 0 and 1 ini
		Os_Ini();
		//UART_INI
                #if((HW_PIN_CFG_10==UART_TX)||(HW_PIN_CFG_12==UART_RX))
		Uart_Ini();
                #endif
                
                // PWM and Timer 2 INI
                #if((HW_PIN_CFG_5==PWM)||(HW_PIN_CFG_6==PWM)||(HW_PIN_CFG_7==PWM)||(HW_PIN_CFG_14==PWM))
		Pwm_Ini();
		#endif

		Eep_Ini(); // enable EEP ISR
                EEP_to_RAM(NO_ERROR_CHECK); // read back EEP data to RAM do not consider any errors
                usage_time_ini(); // should be called after eep ini
		#if(HW_PIN_CFG_5==EXT_INTR)
		//----------------External Interrupt---------------------------------
		INTE=1; // enable external interrupt on GP2 pin
		INTEDG=1; // 1:interrupt on rising edge 0:Interrupt on falling edge
		#endif
		//------------------interrupt on chnage por GPIO PINS--------------
		// IOC3 is working but no timing consistant ie immidiate sometime misses the pulse
		// IOC4=1;
		// GPIE=1;

}

void Systm_Ini_Proc_2(void)
{
    delay_ms(SYS_START_DELAY); // wait for stability delay
    GIE=1; // enable globele interrupt (used for timer0)
}
void System_Status_Indicator(void)  // 1 second task
{
    if(Sys_u8_st_Globle_System_Status!=0) // ther is error in system
    {
        // blink LED at ~ 0.5 hz
        Api_Led_Control(LED0,BLINK_500); // blink System LED every 500 ms
        // TODO send UART frame of system status
    }
    else // No error in system
    {
        Api_Led_Control(LED0,BLINK_1000); // blink System LED every 1000 ms
    }
}