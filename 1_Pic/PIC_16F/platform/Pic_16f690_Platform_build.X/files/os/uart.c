/*UART drivers*/
#include "type_def.h"
#include "pic16f690.h"
#include "uart.h"
#include "system_config.h"
#if((HW_PIN_CFG_10==UART_TX)||(HW_PIN_CFG_12==UART_RX))

UART_MGR Uart_Net_Mgr;


//--------UART INI
inline void Uart_Ini(void)
{	
	    /*set baud rate*/
		BRGH = 0; 
		BRG16 = 0;
        SYNC = 0;
		SPEN = 1;
		#if(BAUD_RATE==1200)
			SPBRG=103;  // 1200 baud at 8mhz
        #elif(BAUD_RATE==2400)
            SPBRG=51;            // 2400
		#else
             SPBRG=12;			// 9600 baud
		#endif
		
		#if(HW_PIN_CFG_12==UART_RX)
			CREN = 1;
			RCIE=1; // enable receive interrupt
	    #endif
		#if(HW_PIN_CFG_10==UART_TX)
			TXEN = 1;
			/*Setting the TXEN bit of the TXSTA register enables the transmitter circuitry of the EUSART. Clearing the SYNC
			bit of the TXSTA register configures the EUSART for asynchronous operation. Setting the SPEN bit of the
			RCSTA register enables the EUSART and automatically configures the TX/CK I/O pin as an output.*/
		#endif


	#endif
}


//--------ISR ROUTINE--------------------------------
inline void Uart_Isr(void)
{
if(RCIF)  // receive interrupt flag
{
   if(Uart_Net_Mgr.Uart_Rx.com_status!=UART_FRAME_RECEIVED) // frame is read by application or in receving stage
   {
	   Uart_Net_Mgr.Uart_Rx.rx_period=0; // reset the receive timer, checked by 1ms task
	   Uart_Net_Mgr.Uart_Rx.com_status=UART_RECEIVING;
	   *Uart_Net_Mgr.Uart_Rx.rx_data_ptr=RCREG; // stor the received data
	   Uart_Net_Mgr.Uart_Rx.rx_data_ptr=Uart_Net_Mgr.Uart_Rx.rx_data_ptr+1;
   }
   if(OERR)  // buffer full error clearing
   {
		CREN=0;
   }
}

if(TXIF)
{
    if(Uart_Net_Mgr.Uart_Tx.com_satus==TRANSMIT_DATA)
	{
	  if(Uart_Net_Mgr.Uart_Tx.data_index<Uart_Net_Mgr.Uart_Tx.data_length)
	  {
		TXREG=Uart_Net_Mgr.Uart_Tx.TX_DATA[Uart_Net_Mgr.Uart_Tx.data_index];  // trasmmit the whole frame
		Uart_Net_Mgr.Uart_Tx.data_index=Uart_Net_Mgr.Uart_Tx.data_index+1;
	  }
	  else
	  {
		Uart_Net_Mgr.Uart_Tx.data_index=1;  // after trasmitting whole frame reset the parameters
                Uart_Net_Mgr.Uart_Tx.com_satus=IDLE;
	  }
	}
}
}
void Uart_Manager(void) // called every 10ms, to check time out of frame, set value of max tome as 50ms ie 5
{
	if( Uart_Net_Mgr.Uart_Rx.rx_period<=Uart_Net_Mgr.Uart_Rx.rx_period_max) // data is being received or not yet timed out
	{
	   Uart_Net_Mgr.Uart_Rx.rx_period=Uart_Net_Mgr.Uart_Rx.rx_period+1; // 1ms time out
	}
	else // its time out frame is received and time out or it can be time out 
	{
		if(Uart_Net_Mgr.Uart_Rx.com_status==UART_RECEIVING)
		{
			Uart_Net_Mgr.Uart_Rx.com_status=UART_FRAME_RECEIVED;
		}
	}	

}

void  Api_Uart_Read_Rx(void)
{

	if(Uart_Net_Mgr.Uart_Rx.com_status==UART_FRAME_RECEIVED)
	{
		/**/
		/*copy the content or call frame manager from here*/
		/*UART_RX_DATA[0]=UART_RX_MGR.RX_DATA[0];
		UART_RX_DATA[1]=UART_RX_MGR.RX_DATA[1];
		UART_RX_DATA[2]=UART_RX_MGR.RX_DATA[2];
		UART_RX_DATA[3]=UART_RX_MGR.RX_DATA[3];
		UART_RX_DATA[4]=UART_RX_MGR.RX_DATA[4];
		UART_RX_DATA[5]=UART_RX_MGR.RX_DATA[5];
		UART_RX_DATA[6]=UART_RX_MGR.RX_DATA[6];
		UART_RX_DATA[7]=UART_RX_MGR.RX_DATA[7];*/

                // this function get call on recepton of frame
                // Uart_Frm_Mgr();
		Uart_Net_Mgr.Uart_Rx.rx_data_ptr= &Uart_Net_Mgr.Uart_Rx.RX_DATA[0];
		Uart_Net_Mgr.Uart_Rx.com_status=UART_FRAME_PROCESSED;
		
	}
	//return(Uart_Net_Mgr.Uart_Rx.com_status);
}

_UBYTE Api_Uart_Tx(_UBYTE data_length, _UBYTE * data_values)
{
   if(Uart_Net_Mgr.Uart_Tx.com_satus==IDLE)
   {
	   // copy data
	   for(char i=0; i<data_length;i++ )
	   {
			Uart_Net_Mgr.Uart_Tx.TX_DATA[i]=* data_values;
			data_values=data_values+1;
	   }   
	   // update status
	   Uart_Net_Mgr.Uart_Tx.data_length=data_length;
	   Uart_Net_Mgr.Uart_Tx.data_index=1; // as first value is trasmitted anyway in this routine
	   Uart_Net_Mgr.Uart_Tx.com_satus=TRANSMIT_DATA;
	   // trigger ISR
	   TXREG=Uart_Net_Mgr.Uart_Tx.TX_DATA[0];
   }
   return(Uart_Net_Mgr.Uart_Tx.com_satus); // will be always trasmit data
}