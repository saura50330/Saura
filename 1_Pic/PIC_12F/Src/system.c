#include "common.h"
#include "system.h"
#include "eep_pub.h"
/*ISR routines*/
void interrupt isr(void) // 10ms timer    os timer
{
  
  // GIE=0; // pause interupts during ISR execution this is bug this is handled by  processor
 
  // timer 0 overflow
  if(T0IF)
  {   
    Os_Timer_ISR();
    
  }
   
  // timer 1 overflow used for delays and PWM measurements and communication protocols
  if(TMR1IF)
  {
    TMR1ON=0; //disable timer
    UART_ISR();
    TMR1IF=0;// reset timer interrupt flg
  }
  
  // timer 2 overflows
  if(TMR2IF)
  {
    
	app_speciel();
    TMR2IF=0;
	
	// stope CCP module

    // write duty cycle value
  }

  #if(HW_PIN_CFG_5==EXT_INTR)
  if(INTF)
  {
    INTF=0;
  }
  #endif
  if(GPIF)  // interrupt on change
  {
	#if(HW_PIN_CFG_4==UART_RX)
	
	#endif
   GPIF=0;

  }
  // EEPROM WRITE SUCCESS
  EEP_ISR();
 
 // GIE=1; // enable interuupts after ISR execution this is bug
}

/*DRIVER INI PROC*/
// system initialisation
inline void systm_ini_proc_1(void)
{
	   //---------OSCIllator-------
	   OSCCON |= 0b01110001;     //set int osc to 8MHz// instuction cycle freq =8mz/4 = 2 mhz
	   while(!HTS);               // wait until OSC stable
	   //------------------interrupt on chnage por GPIO PINS--------------
	   // IOC3 is working but no timing consistant ie immidiate sometime misses the pulse
	   // IOC4=1;
	   // GPIE=1;
	   //-----------COMPARATOR-----
	   CMCON0 = 7; // Disable comparators
	   
	   // ----- GPIO direction selection----------------
	   DIO_INI();	
	   //--------ADC configuration ----------------------
	   ADC_INI();   
	   Os_Ini();
       EEP_INI(); // initialise eep variables

        EEP_to_RAM(0); // copy varibale to ram , 0: cosider no checksum 1:cosider checksum

        //---------------timer1 with interrupt for generating communication delays or to use for pulse measurment-----
        TMR1IF=0;
        TMR1CS=0; // use internal clock fos/4
        T1OSCEN = 0; // do not enable oscillator
        UART_INI();
	    TMR1IE=0; // it will be anable based on aaplication
	#if((HW_PIN_CFG_5==PWM))
        PWM_INI();
	#else
	// timer 2 initialisation if TPWM is not needed
	T2CON=set_bit_position(T2CON,0); // st TMR2ON bits of timer prescalar(4)
	PR2=156;// value of TMR2 when reches 0xFF interrupt occurs
	TMR2ON=1; //enable timer
        TMR2IE=1; // enable timer 2 interrupt
	#endif
	 //----------------EEP---------------------------------
        
    
	#if(HW_PIN_CFG_5==EXT_INTR)
	//----------------External Interrupt---------------------------------
        INTE=1; // enable external interrupt on GP2 pin
	INTEDG=1; // 1:interrupt on rising edge 0:Interrupt on falling edge
        #endif
        app_ini(); /*initialise all application variables*/

}
inline void systm_ini_proc_2(void)
{
        GIE=1;
}