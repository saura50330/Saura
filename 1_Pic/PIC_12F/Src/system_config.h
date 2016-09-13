#include "type_def.h"
// configure system pins i/O adc pwm , RS232
/*---------PIN TYPE CONFIGURATION----------------------*/
	/*This IC have 8 pins out of which two are vcc and ground*/
	/*-remaining 6 pin configuration is given below-*/

/*							---------------
	(vcc)	 HW_PIN1-------|			   |--------- HW_PIN8 (ground)
	(I/O,)	 HW_PIN2-------|			   |--------- HW_PIN7 (I/O,ADC)
	(I/O,ADC)HW_PIN3-------|  PIC12F683	   |--------- HW_PIN6 (I/O,ADC)
	(I)	 	 HW_PIN4-------|			   |--------- HW_PIN5 (I/O,ADC,CCP)
							---------------                       */

#define HW_PIN_CFG_2 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,UART_TX= software UART tx,
#define HW_PIN_CFG_3 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_4 UART_RX    // this Pin is always digital input I,or PM=PWM measurement,UART_RX= software UART Rx
#define HW_PIN_CFG_5 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,EXT_INTR=External interrupt
#define HW_PIN_CFG_6 UART_TX //ADC  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_7 ADC  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement

#define UART_TX_PIN HW_PIN_CFG_6
#define COM_TX HW_PIN6

#define UART_RX_PIN HW_PIN_CFG_4
#define COM_RX HW_PIN4
/*---------PIN NAME CONFIGURATION----------------------*/
	/*This IC have 8 pins out of which two are vcc and ground*/
	/*-remaining 6 pin configuration is given below-*/

/*							---------------
	(vcc)	 HW_PIN1-------|			   |--------- HW_PIN8 (ground)
	(I/O,)	 HW_PIN2-------|			   |--------- HW_PIN7 (I/O,ADC)
	(I/O,ADC)HW_PIN3-------|  PIC12F683	   |--------- HW_PIN6 (I/O,ADC)
	(I)	 	 HW_PIN4-------|			   |--------- HW_PIN5 (I/O,ADC,CCP)
							---------------                       */

#define HW_PIN2 0x05
#define HW_PIN3 0x04
#define HW_PIN4 0x03
#define HW_PIN5 0x02
#define HW_PIN6 0x01
#define HW_PIN7 0x00


#define ADC_CHN3 HW_PIN3
#define ADC_CHN2 HW_PIN5
#define ADC_CHN1 HW_PIN6
#define ADC_CHN0 HW_PIN7

#define SYSTEM_VER 102
