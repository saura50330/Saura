#include "common.h"
#if((HW_PIN_CFG_14==PWM)||(HW_PIN_CFG_7==PWM)||(HW_PIN_CFG_6==PWM)||(HW_PIN_CFG_5==PWM))

inline void Pwm_Ini(void)
{
  //-----timer2 configuration for PWM-----------------
    TMR2IF=0;
    T2CON=set_bit_position(T2CON,1); // st TMR2ON bits of timer prescalar(16)
    TMR2ON=1; //enable timer
    //TMR2IE=1; // enable timer 2 interrupt

    //------PWM module via timer2-------------------
    PR2 = 100;// pwm period 1.25 kh at 8 mz clock with 8 bit PWM resolution
    CCPR1L=0x00;//select duty cycle 0%
    CCP1CON=CCP1CON & 0b11001111;//select duty cycle 0%
    CCP1CON=CCP1CON|0x0C; //select CCP pin as pwm pin
    PSTRCON=0x10; // desides which pins  gets the pwm, and switching happens only after previous PWM peroid is completed
	
   // USE PWMCON1 register for fine tuning ded band delays

}

/*PWM CCP DRIVERS*/
/*PWM DRIVERS*/
//--------PWM is available only on four pin for pic16f690 which is one is not used in this project ------------
// time period is used as 50 hz (0.02 sec)as default because it can be used for controlling servo motors
// call below function to set any pwm on//
// dutycycle =0 represents 0% duty cycle , dutycycle value =255 represents 100% duty cycle
void Api_Get_Pwm(unsigned char dutycycle)
{
	static _UBYTE  dutycycle_old; 	// to store old duty cycle
	_UINT16 duty_cycle_change_req;  // to calculate duty cycle values to be loaded in CCP register
	if(dutycycle!=dutycycle_old) // condition for duty cycle convertion
	{

           // pwm duty cycle is of 10 bit
          //  duty_cycle_change_req=(((_UINT16)dutycycle)<<2);    // (2^10* dutycycle)/ 2^8 =4*dutycycle/1....<<2 is *4
            // split above value into two
            // value to be loaded in CCP1CON two lsb bits at position 5=d1 4=d0  (7 - 0)
             duty_cycle_change_req=dutycycle;
             CCP1CON=((CCP1CON&0b11001111)|((_UBYTE)(((duty_cycle_change_req)&(0x0003))<<6)));
            // remaing 8 bits
            CCPR1L=(_UBYTE)(duty_cycle_change_req>>2);
            dutycycle_old=dutycycle;
	}

}

/*CCP DRIVERS captur compare routines*/
#endif