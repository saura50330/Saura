#include "type_def.h"
#include "pwm.h"
/*PWM CCP DRIVERS*/
/*PWM DRIVERS*/
#if((HW_PIN_CFG_5==PWM))
//--------PWM is available only on one pin for pic12f683 wich is HW_PIN5 ------------
// time period is used as 50 hz (0.02 sec)as default because it can be used for controlling servo motors


// call below function to set any pwm on HW_PIN5//
// dutycycle =0 represents 0% duty cycle , dutycycle value =255 represents 100% duty cycle

void get_pwm(unsigned char dutycycle)
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


inline void PWM_INI(void)
{
 
    //-----timer2 configuration for PWM-----------------
    TMR2IF=0;
    //T2CON=set_bit_position(T2CON,1); // st TMR2ON bits of timer prescalar(16)
    //T2CON pre scalar is kept 0 to make 75 kh PWM
   
    TMR2ON=1; //enable timer
    //TMR2IE=1; // enable timer 2 interrupt

    //------PWM module via timer2-------------------
    //PR2 = 0x9;// 200khz pwm for charge controller 8 bit resolution
    //CCPR1L=0x00;//select duty cycle 0%
    PR2 = 47;// PR2= 51 for 38khz / PR2= 49 for 40khz pwm for ir led
    //CCPR1L=50;//select duty cycle 40%
   // CCP1CON=CCP1CON & 0b11001111;//select duty cycle 0%
   // CCP1CON=CCP1CON|0x0C; //select CCP pin as pwm pin
    CCPR1L = 0b00011001; // set duty MSB
    CCP1CON = 0b00111100; // duty lowest bits + PWM mode
    CCP1CON=CCP1CON&0xF0; // turn off PWM
    /*use timer2 to generate 0.1ms interrupt(solar invertor specific remove this whwn not neede)*/
     /*0.1ms isr*/
	 

}
#endif
