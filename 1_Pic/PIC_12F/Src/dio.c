#include "common.h"
inline void DIO_INI(void)
{
/*set proper system configuration file for this to take affect*/
   
	   #if((HW_PIN_CFG_2==I)||(HW_PIN_CFG_2==PM))
	   TRISIO=set_bit_position(TRISIO,HW_PIN2);             
	   #else
	   TRISIO=clear_bit_position(TRISIO,HW_PIN2);
	   #endif
	   
	   #if((HW_PIN_CFG_3==I)||(HW_PIN_CFG_3==ADC)||(HW_PIN_CFG_3==PM))
	   TRISIO=set_bit_position(TRISIO,HW_PIN3);             
	   #else
	   TRISIO=clear_bit_position(TRISIO,HW_PIN3);
	   #endif
	   
	   TRISIO=set_bit_position(TRISIO,HW_PIN4);   // always input          
	  
	   #if((HW_PIN_CFG_5==I)||(HW_PIN_CFG_5==ADC)||(HW_PIN_CFG_5==PM)||(HW_PIN_CFG_5==EXT_INTR))
	   TRISIO=set_bit_position(TRISIO,HW_PIN5);             
	   #else
	   TRISIO=clear_bit_position(TRISIO,HW_PIN5);
	   #endif
	   
	   #if((HW_PIN_CFG_6==I)||(HW_PIN_CFG_6==ADC)||(HW_PIN_CFG_6==PM))
	   TRISIO=set_bit_position(TRISIO,HW_PIN6);             
	   #else
	   TRISIO=clear_bit_position(TRISIO,HW_PIN6);
	   #endif
	   
	   #if((HW_PIN_CFG_7==I)||(HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_7==PM))
	   TRISIO=set_bit_position(TRISIO,HW_PIN7);             
	   #else
	   TRISIO=clear_bit_position(TRISIO,HW_PIN7);
	   #endif
}