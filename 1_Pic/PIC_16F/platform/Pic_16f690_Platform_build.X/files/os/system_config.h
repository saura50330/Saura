#include "type_def.h"

// configure system pins i/O adc pwm , RS232
/*---------PIN CONFIGURATION----------------------*/
	/*This IC have 20 pins out of which two are vcc and ground*/
	/*-remaining 18 pin configuration is given below-*/
    /*refer IC data sheet for PINS*/

#define HW_PIN_CFG_2 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,UART_TX= software UART tx,
#define HW_PIN_CFG_3 ADC    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_4 I    // this Pin is always digital input I,or PM=PWM measurement,UART_RX= software UART Rx
#define HW_PIN_CFG_5 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,EXT_INTR=External interrupt
#define HW_PIN_CFG_6 PWM  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_7 PWM  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_8 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,UART_TX= software UART tx,
#define HW_PIN_CFG_9 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_10 UART_TX    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_11 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,EXT_INTR=External interrupt
#define HW_PIN_CFG_12 UART_RX  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_13 O  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_14 PWM    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,UART_TX= software UART tx,
#define HW_PIN_CFG_15 ADC    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_16 ADC    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_17 O    // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement,EXT_INTR=External interrupt
#define HW_PIN_CFG_18 ADC  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement
#define HW_PIN_CFG_19 ADC  // I =input , O=OUTPUT ,ADC=ADC channel ,PWM = PWM channel,PM=PWM measurement


//------------software timers-----------------
#define SW_TMR NOT_REQUIRE    // REQUIRE  or NOT_REQUIRE
#define MAX_TIMERS 2

/*application version*/
#define SYSTEM_VER 102


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