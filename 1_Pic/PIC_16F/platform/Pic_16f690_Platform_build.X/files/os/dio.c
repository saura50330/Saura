#include "common.h"
#include "dio.h"
Dio_Con Dio;
inline void Dio_Ini(void)
{
/*set proper system configuration file for this to take affect*/
   // ----- GPIO direction selection----------------
	   /*set proper system configuration file for this to take affect*/
            // CMCON0 = 7; // Disable comparators to do check why no comparator
	   
	   #if((HW_PIN_CFG_2==I)||(HW_PIN_CFG_2==PM))
	   TRISA5=1;            
	   #else
	   TRISA5=0;
	   #endif
	   
	   #if((HW_PIN_CFG_3==I)||(HW_PIN_CFG_3==ADC)||(HW_PIN_CFG_3==PM))
	   TRISA4=1;            
	   #else
	   TRISA4=0;
	   #endif
	   
	   TRISA3=1;   // always input
	  
	   #if((HW_PIN_CFG_5==I)||(HW_PIN_CFG_5==ADC)||(HW_PIN_CFG_5==PM))
	   TRISC5=1;            
	   #else
	   TRISC5=0;
	   #endif
	   
	   #if((HW_PIN_CFG_6==I)||(HW_PIN_CFG_6==ADC)||(HW_PIN_CFG_6==PM))
	   TRISC4=1;            
	   #else
	   TRISC4=0;
	   #endif
	   
	   #if((HW_PIN_CFG_7==I)||(HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_7==PM))
	   TRISC3=1;            
	   #else
	   TRISC3=0;
	   #endif
	   
	    #if((HW_PIN_CFG_8==I)||(HW_PIN_CFG_8==ADC)||(HW_PIN_CFG_8==PM))
	   TRISC6=1;            
	   #else
	   TRISC6=0;
	   #endif
	   
	   #if((HW_PIN_CFG_9==I)||(HW_PIN_CFG_9==ADC)||(HW_PIN_CFG_9==PM))
	   TRISC7=1;            
	   #else
	   TRISC7=0;
	   #endif
	   
	   #if((HW_PIN_CFG_10==I)||(HW_PIN_CFG_10==ADC)||(HW_PIN_CFG_10==PM))
	   TRISB7=1;            
	   #else
	   TRISB7=0;
	   #endif
	   
	   #if((HW_PIN_CFG_11==I)||(HW_PIN_CFG_11==ADC)||(HW_PIN_CFG_11==PM))
	   TRISB6=1;            
	   #else
	   TRISB6=0;
	   #endif
	   
	   #if((HW_PIN_CFG_12==I)||(HW_PIN_CFG_12==ADC)||(HW_PIN_CFG_12==PM)||(HW_PIN_CFG_12==UART_RX))
	   TRISB5=1;            
	   #else
	   TRISB5=0;
	   #endif
	   
	   #if((HW_PIN_CFG_13==I)||(HW_PIN_CFG_13==ADC)||(HW_PIN_CFG_13==PM))
	   TRISB4=1;            
	   #else
	   TRISB4=0;
	   #endif
	   
	   #if((HW_PIN_CFG_14==I)||(HW_PIN_CFG_14==ADC)||(HW_PIN_CFG_14==PM))
	   TRISC2=1;            
	   #else
	   TRISC2=0;
	   #endif
	   
	   #if((HW_PIN_CFG_15==I)||(HW_PIN_CFG_15==ADC)||(HW_PIN_CFG_15==PM))
	   TRISC1=1;            
	   #else
	   TRISC1=0;
	   #endif
	   
	   #if((HW_PIN_CFG_16==I)||(HW_PIN_CFG_16==ADC)||(HW_PIN_CFG_16==PM))
	   TRISC0=1;            
	   #else
	   TRISC0=0;
	   #endif
	   
	   #if((HW_PIN_CFG_17==I)||(HW_PIN_CFG_17==ADC)||(HW_PIN_CFG_17==PM))
	   TRISA2=1;            
	   #else
	   TRISA2=0;
	   #endif
    
	   #if((HW_PIN_CFG_18==I)||(HW_PIN_CFG_18==ADC)||(HW_PIN_CFG_18==PM))
	   TRISA1=1;            
	   #else
	   TRISA1=0;
	   #endif
    
	   #if((HW_PIN_CFG_19==I)||(HW_PIN_CFG_19==ADC)||(HW_PIN_CFG_19==PM))
	   TRISA0=1;            
	   #else
	   TRISA0=0;
	   #endif
}
void Api_Led_Control(_UBYTE led_no,_UBYTE led_status) //led_no = 0 to 7 , led_status= 0:OFF 1 =100ms blink ...255 =ON
{
    if(led_no<MAX_LED)
    {
        //Dio.Led_u8_st_Led_Status_Comp[led_no]=led_status; // fo checking value
        Dio.Led_u8_st_Led_Status[led_no]=led_status;// to be decremented
    }
}

// led manager manages led status of system
void Led_Manager(void) // call this every 200ms
{
    _UBYTE u8_temp_led;
    for(_UBYTE i=0;i<MAX_LED;i++) // go throug all led status
    {
        if(Dio.Led_u8_st_Led_Status[i]==LED_OFF)
        {
            // turn off led
            Dio_Write(Led_Matrix[i][0],Led_Matrix[i][1],0);// port , pin , value

        }
        else if(Dio.Led_u8_st_Led_Status[i]==LED_ON)
        {
            // turn On LED
            Dio_Write(Led_Matrix[i][0],Led_Matrix[i][1],1);// port , pin , value

        }
        else // blink
        {
            if(Dio.Led_u8_st_Led_Status_Comp[i]<=BLINK)
            {
                // toggel LED
                u8_temp_led=Dio_Read(Led_Matrix[i][0],Led_Matrix[i][1]);
                Dio_Write(Led_Matrix[i][0],Led_Matrix[i][1],(!u8_temp_led));// port , pin , value
                Dio.Led_u8_st_Led_Status_Comp[i]=Dio.Led_u8_st_Led_Status[i]; // reload count
            }
            else
            {
                Dio.Led_u8_st_Led_Status_Comp[i]--; // decrement count
            }
        }

    }
}
//Switch_Manager manages switch debounig delays and update debouce switch status
void Switch_Manager(void)  // call this every 100ms
{
    _UBYTE u8_st_temp_switch_ststus;
    for(_UBYTE i=0;i<MAX_SWITCH;i++) // go throug all led status
    {
       // read switch value
        u8_st_temp_switch_ststus=Dio_Read(Switch_Matrix[i][0],Switch_Matrix[i][1]); // port , pin
       // compare switch value with old value
        if(u8_st_temp_switch_ststus==Dio.Switch_u8_st_Current_Status[i])
        {
            // if equal incerment debounce count
            if(Dio.Switch_u8_st_Deb_Count[i] == MAX_SWITCH_DEB_COUNT)  // if debunce count reaches the final value
            {
                Dio.Switch_u8_st_Stable_Status[i]=Dio.Switch_u8_st_Current_Status[i];  // update switch stable status
                Dio.Switch_u8_st_Deb_Count[i]=0;// reset debounce count
            }
            else
            {
                Dio.Switch_u8_st_Deb_Count[i]++;
            }

        }
        else
        {
            Dio.Switch_u8_st_Deb_Count[i]=0;  //if not equal reset debounce count
        }
        // update old value
        Dio.Switch_u8_st_Current_Status[i]=u8_st_temp_switch_ststus;
    }
}
_UBYTE Api_Get_Switch_Status(_UBYTE switch_no)
{
    // return stable switch status
    if(switch_no<MAX_SWITCH)
    {
        return(Dio.Switch_u8_st_Stable_Status[switch_no]);
    }
    else
    {
        return(0);
    }
}

_UBYTE Dio_Read(_UBYTE port_no,_UBYTE pin_no)  // to read dio pin or port  pin_no=0 to 7 , pin_no=255 = complete port
{
    _UBYTE temp_port_data;
    switch(port_no)
    {
        case PORT_A: if(pin_no<8)
                     {
                       temp_port_data=((PORTA>>pin_no)&0x01);
                     }
                     else
                     {
                      temp_port_data= PORTA;          // read complete port
                     }
                     break;
        case PORT_B: if(pin_no<8)
                     {
                       temp_port_data=((PORTB>>pin_no)&0x01);
                     }
                     else
                     {
                      temp_port_data= PORTB;          // read complete port
                     }
                     break;
        case PORT_C: if(pin_no<8)
                     {
                       temp_port_data=((PORTC>>pin_no)&0x01);
                     }
                     else
                     {
                      temp_port_data= PORTC;          // read complete port
                     }
                     break;
        default : temp_port_data=0x00;break;
    }
}
void Dio_Write(_UBYTE port_no,_UBYTE pin_no,_UBYTE bit_value )// to write dio pin or port  pin_no=0 to 7,pin_no=255 = complete port
{
// Port no 0 to 2
// pin no 0to 7 , 255 all pins
// value 0 or 1 for pins else 0 to 255 for ports
    switch(port_no)
    {
        case PORT_A: if(pin_no<8)
                     {
                       PORTA=(bit_value?( PORTA | (0x01<<pin_no) ):(PORTA & (~(0x01<<pin_no))));
                     }
                     else
                     {
                      PORTA=pin_no;          // read complete port
                     }
                     break;
        case PORT_B: if(pin_no<8)
                     {
                       PORTB=(bit_value?( PORTB | (0x01<<pin_no) ):(PORTB & (~(0x01<<pin_no))));
                     }
                     else
                     {
                      PORTB=pin_no;          // read complete port
                     }
                     break;
        case PORT_C: if(pin_no<8)
                     {
                       PORTC=(bit_value?( PORTC | (0x01<<pin_no) ):(PORTC & (~(0x01<<pin_no))));
                     }
                     else
                     {
                      PORTC=pin_no;          // read complete port
                     }
        default : break;
    }
}
