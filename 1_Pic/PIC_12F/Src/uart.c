#include "common.h"
#include "uart.h"
/*UART drivers*/
#if((UART_TX_PIN==UART_TX)||(HW_PIN_CFG_4==UART_RX))

_UINT16 uart_tx_data;
_UINT8 tx_data_cnt,UART_STATE;
_UINT16 uart_rx_data;
_UINT8 uart_rx_status,uart_rx_data_temp;
#endif

// UART ISR
inline void UART_ISR(void)
{
#if((UART_TX_PIN==UART_TX)||(HW_PIN_CFG_4==UART_RX))
	
	switch(UART_STATE)
	{
		case UART_IDLE:  
						#if((HW_PIN_CFG_4==UART_RX))
							if(GP3==0)// if falling edge means start of data
							{
							uart_rx_status=2;     // started receiving data
							TMR1=BAUD_RATE_TMR_VAL +120;  // bit rate for 1200 baud uart;  // bit rate for 1200 baud uart recive  sampling (adding offset for correct sampling point 1234 us)
							UART_STATE=UART_RX;
                                                        uart_rx_data=0;
                                                        //GPIO=clear_bit_position(GPIO,LED_PIN);
							}
							else
							{
							TMR1=BAUD_RATE_TMR_VAL+160;   // ready to receive data	  // bit rate for 1200 baud uart recive  sampling (adding offset for correct sampling point 1234 us)
							}
						#endif
							tx_data_cnt=0;
                                                        
                                        break;
		#if((HW_PIN_CFG_4==UART_RX))
		case UART_RX:
		
                    if(tx_data_cnt<8)
			  {
                            TMR1=BAUD_RATE_TMR_VAL+125;  // bit rate for 1200 baud uart recive  sampling (adding offset for correct sampling point 1234 us)
                            if(GP3==1)
                            {
                             //   GPIO=set_bit_position(GPIO,LED_PIN);
                                uart_rx_data|=(0x0001<<tx_data_cnt); // lsb is recived first
                            }
                            else
                            {
                               // GPIO=clear_bit_position(GPIO,LED_PIN);
                                 uart_rx_data&=(~(0x0001<<tx_data_cnt));
                            }
			  } // write frame error case if needed ie stop bit not received (ie in else part if stop bit is zero)
			  else
			  {
                           /// GPIO=set_bit_position(GPIO,LED_PIN);
			   //uart_rx_data=((GP3==1)?uart_rx_data:0xFFFF); // replace data with error value
                            UART_STATE=UART_IDLE;
                            TMR1=BAUD_RATE_TMR_VAL+160;   // ready to receive data	  // bit rate for 1200 baud uart recive  sampling (adding offset for correct sampling point 1234 us)
                           
                            uart_rx_status=1;     // complted recevind data
			  }
			  tx_data_cnt=tx_data_cnt+1;
			  break;
		#endif
		#if(UART_TX_PIN==UART_TX)
		case UART_TX:if(tx_data_cnt<10) //8data bytes and one stop bytes
					{
                                                
                                                if((uart_tx_data>>tx_data_cnt)&(0x01))// PUT DATA on Tx pin
                                                {
                                                // CCP1CON=CCP1CON&0xF0; // turn off pwm
						 //GPIO=clear_bit_position(GPIO,IR_OUT_PIN);
                                                 GPIO=set_bit_position(GPIO,COM_TX);

                                                }
                                                else
                                                {

                                               //  CCP1CON=CCP1CON|0x0C;     // turn on pwm

                                                 GPIO=clear_bit_position(GPIO,COM_TX);
                                                }
                                                // 0.833 ms is one bit time for 1200 baud rate

                                              
                                                tx_data_cnt=tx_data_cnt+1;
                                             /*   if(tx_data_cnt>5)
                                                {
                                                TMR1=BAUD_RATE_TMR_VAL+27;  // bit rate for 1200 baud uart tx and copensation
                                                }
                                                else
                                                {
                                                    TMR1=BAUD_RATE_TMR_VAL;
                                                }*/
                                                TMR1=BAUD_RATE_TMR_VAL+120;
                                        }
				else
                                     {
                                        UART_STATE=UART_IDLE;
                                        tx_data_cnt=0;
                                        TMR1=BAUD_RATE_TMR_VAL_PT1MS;  // ready to receive data	  // bit rate for 1200 baud uart recive  sampling (adding offset for correct sampling point 1234 us)

                          }
		
				break;
		#endif
		default:
		UART_STATE=UART_IDLE;			
		break;// precautionbreak;
	}
	#if((HW_PIN_CFG_4==UART_RX)&&(UART_TX_PIN==UART_TX))
	TMR1ON=1; //enable timer  we need isr runing to poll rx message
       #endif
       #if((HW_PIN_CFG_4==UART_RX)&&(UART_TX_PIN!=UART_TX))
	TMR1ON=1; //enable timer we need isr running to poll rx message
       #endif
       #if((HW_PIN_CFG_4!=UART_RX)&&(UART_TX_PIN==UART_TX))
        
        TMR1ON=(UART_STATE==UART_IDLE)?0:1; // as tx is over and no rx message is enable disable the running timer
       #endif
	#else
		TMR1IF=0;// reset timer interrupt flg
		TMR1=0x63BF;//20ms iterrupt time
	#endif

}

inline void UART_INI(void)
{
#if((UART_TX_PIN==UART_TX)||(HW_PIN_CFG_4==UART_RX))
		
		UART_STATE=UART_IDLE;
		#if(HW_PIN_CFG_4==UART_RX)
			TMR1=TMR1=BAUD_RATE_TMR_VAL +160;  // ready to receive data
			TMR1ON=1; // enable timer 1
			TMR1IE=1; // enable timer 1 interrupt
	    #endif
		
		#if(UART_TX_PIN==UART_TX)
                         //CCP1CON=CCP1CON|0x0C;
                         GPIO=set_bit_position(GPIO,COM_TX);// BUS_IDLE
                       // GPIO=set_bit_position(GPIO,COM_TX);
			#if(HW_PIN_CFG_4!=UART_RX)
			TMR1ON=0; // disable timer 1
			TMR1IE=0; // disable timer 1 interrupt
			#endif
		#endif
	#else
		TMR1=0x63BF;  // 20ms interrupt
		TMR1ON=1; //enable timer
		TMR1IE=1; // enable timer 1 interrupt
	#endif
}

#if(UART_TX_PIN==UART_TX)
_UBYTE uart_tx(_UBYTE data_byte)
{
    char tx_st;
    tx_st=1;
    
    if(UART_STATE==UART_IDLE)
    {
        uart_tx_data=(0xFF00|data_byte); // 16 bit
       
        
	TMR1=BAUD_RATE_TMR_VAL +160;  // bit rate for 1200 baud uart
	UART_STATE=UART_TX;
        // GPIO=clear_bit_position(GPIO,COM_TX);
         //CCP1CON=CCP1CON|0x0C;     // turn on pwm

        GPIO=clear_bit_position(GPIO,COM_TX);
        TMR1ON=1; //enable timer
        TMR1IE=1; // enable timer 1 interrupt
    }
    else
    {
        tx_st=0;
    }
    
    return(tx_st);
  
}
#endif

#if(HW_PIN_CFG_4==UART_RX)
_UINT8 uart_rx(void)
{
    
 if (uart_rx_status==1)
  {
    GIE=0;
    uart_rx_data_temp=(uart_rx_data&0x0FF);
    GIE=1;
    uart_rx_status=0;
     return(uart_rx_data_temp);  // valid data received
  }
  return(0xFF); // no data received
}

#endif

