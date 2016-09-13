#include "app_process_pub.h"
/*speciel tasks (ISR based)*/

#define NOT_OK 0x00
/*------------- controller pins accerd by functions*/

/*IC pin No and names */
#define MOSFET1_PIN HW_PIN2
#define MOSFET2_PIN HW_PIN3
#define MAINS_VOLTAGE_SENS HW_PIN4
#define CHARGE_CON_RLY_PIN HW_PIN5

/*invertor mode values*/

#define ON_PURE_SIN 1

//voltage cycles
#define POSITIVE_CYL 0
#define NEGATIVE_CYL 1

//global variables
const _UINT32 SINE_WAWE=0x4515A8A2;//0x218E7310;//0x2CEBD734;//0xD31428CB;   // (20 bit sequence for sine wave:0x1D9137pR2=250)  , by changing this value any type of waveform can be generated
#define MAX_CHR_RLY_DEC_CNT 250

/*10ms task processes*/
// void process1(void); 
// void process2(void); 

/*100ms task processes*/
// void process1(void); 
// void process2(void); 

/*1000ms task processes*/
// void process1(void); 
// void process2(void); 