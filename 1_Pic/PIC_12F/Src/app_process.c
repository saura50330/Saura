#include "common.h"
/*private function inclusion*/
#include "app_process_priv.h"
#define EEP_IDXCHARGING_STATUS 16
/*public function inclusion*/
extern _UBYTE EEPRAM_APP_DATA[]; // update application variables
extern _UBYTE EEPRAM_SYSTEM_DATA[]; // update system variables

_UBYTE inv_control; //decides inverter ON/OFF and mode 
_UBYTE charge_controller_status; // this variable gives staus of charge controller 
_UBYTE config_mode;
_UBYTE hyst_var;  // this perameter holds the status of battry charging discarging staus, this value is assigned to 0x00 because on reset invertor shuld not be off state

_UINT16 Solar_voltage;

void app_sine_wave_ini(void)
{
    // tri state the invertor pins
    GPIO=(GPIO&(clear_bit_position(clear_bit_position(0xFF,MOSFET2_PIN),MOSFET1_PIN)));

    // disconnect battery frome solar panel
    GPIO=clear_bit_position(GPIO,CHARGE_CON_RLY_PIN);
   hyst_var=1; // alway chrge the battery when new restart
}

void app_sine_wave() // app is called from timer 2 isr
{
	static _UBYTE app_ms_ctr; 	/*point 1 ms counter*/
	static _UBYTE stCpyCyl; 	/*voltage cycles*/

	/*conntol values truth table*/
	switch(inv_control)
	{
		
		case ON_PURE_SIN :
                                  if(((SINE_WAWE>>app_ms_ctr)&0x01)) // off condition for 50hz supply
				  {
					// tri state both trasitors
					GPIO=(GPIO&(clear_bit_position(clear_bit_position(0xFF,MOSFET2_PIN),MOSFET1_PIN)));
				   }
				   else// on condition for trasistor
				   {
					 if((stCpyCyl==NEGATIVE_CYL))
					 {
						GPIO=((GPIO &( clear_bit_position(0xFF,MOSFET2_PIN)))|(set_bit_position(0x00,MOSFET1_PIN))); // -ve half cycle
					 }
					 else
					 {
					        GPIO=((GPIO & clear_bit_position(0xFF,MOSFET1_PIN))|(set_bit_position(0x00,MOSFET2_PIN))); // +ve half cycle
					 }
				   }
											
				   if(app_ms_ctr>=31)  // this value depnds on how meny bits define sine wave
			           {
					app_ms_ctr=0;
					stCpyCyl=(stCpyCyl==NEGATIVE_CYL)?POSITIVE_CYL:NEGATIVE_CYL;
				   }
				   else
				   {
					app_ms_ctr=app_ms_ctr+1;
				   }
				   break;
	    case OFF		 :  // tri state the pins
				   
                                    app_ms_ctr=0;
				   //break;
            default:   //tri state the pins
					GPIO=(GPIO&(clear_bit_position(clear_bit_position(0xFF,MOSFET2_PIN),MOSFET1_PIN)));
					TMR2IE=0;             // disable INV ISR
                                        break;
	}
}

void app_sine_wave_control(void) 
{
	static _UBYTE deb_tym=3; /*static flag variable*/
	
	_UBYTE Mains_voltage;
	
	if(charge_controller_status ==OK) // charge_controller_status == OK
	{
		/*get the status of battery voltage  and (supply voltage and auto manual switch : combined functions_ */
	
                Solar_voltage=adc_chn1_data;
                //GIE=0;
                Mains_voltage=get_bit_value(GPIO,MAINS_VOLTAGE_SENS);
                //GIE=1;
                
                if((EEPRAM_APP_DATA[5]==TYPE1))
                {
                    
                       if(((Solar_voltage<EEPRAM_APP_DATA[4])&&(Mains_voltage==0)))
                       {   
			   deb_tym++;                 
                       }
                       else
                       {
                           deb_tym--;
                       }
                }
                else if((EEPRAM_APP_DATA[5]==TYPE2))
                {
                       if((Mains_voltage==0))
                       {
                          // backup at prown
			  deb_tym++;
                       }
                       else
                       {
                           deb_tym--;
                       }
                }
                else 
                {
                        if((Solar_voltage<EEPRAM_APP_DATA[4]))  // mode used for in automatic street ligts and energy sevaing
			{
			  // backup at prown out
			  deb_tym++;
			}
                        else
                        {
                          deb_tym--;
                        }
                }
                /*
                switch(EEPRAM_APP_DATA[5])
		{
		   case TYPE1: // Home backup only at night time
			 // : mode used for bakeup at night time only and when ther is mais failuer
                      
                       if(((Solar_voltage<EEPRAM_APP_DATA[4])&&(Mains_voltage==0)))
                       {   
			   deb_tym++;
                          
                       }
                       else
                       {
                           deb_tym--;
                            GP2=!GP2;
                       }
                       break;
		   case TYPE2: // Home backup all time
		       // invertor turn on when ther is no power supply : mode used for homes
                       if((Mains_voltage==0))
                       {
                          // backup at prown 
			  deb_tym++;
                       }
                       else
                       {
                           deb_tym--;
                       }
			break;
                    case TYPE3: // street lights : turn on at whole night
			if((Solar_voltage<EEPRAM_APP_DATA[4]))  // mode used for in automatic street ligts and energy sevaing
			{
			  // backup at prown out
			  deb_tym++;
			}
                        else
                        {
                          deb_tym--;  
                        }
				
			break;
		   default :break;
		}*/
                if(deb_tym>=6)
                {
                    inv_control=ON_PURE_SIN;
                    deb_tym=3;
                   
                }
                else if (deb_tym==0)
                {
                    inv_control=OFF;
                    deb_tym=3;
                }
		// nga4cob: if inverter stats and due to over load inverter reset .. this will lead to abnormal condition
		// some batters read full when no load connected.... so use eep elemnt to check it or wait timer nex start
	}
        else
        {
            inv_control=OFF;
        }
             
        if(inv_control==ON_PURE_SIN)
        {
             usege_timer_start_stop(ON);  // start usage timer
             TMR2IE=1; // enable INV ISR
        }
        else
        {
            GIE=0;
            TMR2IE=0; // enable INV ISR
            GPIO=(GPIO&(clear_bit_position(clear_bit_position(0xFF,MOSFET2_PIN),MOSFET1_PIN)));
	    EEP_WRITE_2(EEP_IDXCHARGING_STATUS,hyst_var); // write back the eep status of battery charging
            usege_timer_start_stop(OFF); // stop usage timer
            GIE=1;
        }
}
 
void Charge_Controller(void) // 1000ms task  and its master and invertor is slave
{
   
   _UINT16 Battery_voltage; //

   static _UBYTE only_once,only_once1;
   static _UBYTE Chr_Rly_Dec_Count;
   
   // get the voltage signals
   Solar_voltage=adc_chn1_data;
   Battery_voltage=adc_chn0_data; // nga4cob : this may lead to wrong battey voltage measurment if charging relay is on
   // it wiil read both voltages same when charging relay is on and will never turn off till bat voltage reches desire value

   if((Battery_voltage>0x18)&&(config_mode==OFF))
        {
            if(only_once==0)  // reconfigure to invertor mode
            {
                 GIE=0;
                 TMR1IE=0;// reset timer interrupt flg
                 //enable INVERTOR timer ISR
                 TMR2IE=1;
                 TRISIO=set_bit_position(TRISIO,HW_PIN6);   // make it as ADC pin
                 ANSEL=set_bit_position(ANSEL,HW_PIN6);
                 GIE=1;
                 only_once=1;
                 only_once1=0;
            }

            else if(hyst_delta_minus_s10s10s10u8(Battery_voltage,EEPRAM_APP_DATA[2],EEPRAM_APP_DATA[3],&hyst_var))
	    {
		 // once syatem comes in this state invertor get disconnected and their may be battery voltage rise again
		 // so hysteresis is used
		 // once battery is fully charged it will be charged again only after fully discharged ie(when battery fallls below min bat voltage)
                  charge_controller_status = NOT_OK;  // invertor can not start and charg controller is charging battery
                  GIE=0;
                  // if solar is healthy
                  if((Solar_voltage>EEPRAM_APP_DATA[3]))
                  {
                     GPIO=set_bit_position(GPIO,CHARGE_CON_RLY_PIN);
                     // decoupple battery and solar for once 25 second and check
                       Chr_Rly_Dec_Count++;
                       if(Chr_Rly_Dec_Count>MAX_CHR_RLY_DEC_CNT)
                       {
                          Chr_Rly_Dec_Count=0;
                          GPIO=clear_bit_position(GPIO,CHARGE_CON_RLY_PIN);
                       }
                  }
                  else
                  {
                     GPIO=clear_bit_position(GPIO,CHARGE_CON_RLY_PIN);
                  }
                  // TODO: when batery is connected to solar panel solar voltage will fall down so latch is used
                  GIE=1;
            }
	     else   // battery is in normal state can can be used for inverter operation
	     {

                  GIE=0;
                  GPIO=clear_bit_position(GPIO,CHARGE_CON_RLY_PIN);
		  GIE=1;
                  charge_controller_status = OK;  // invertor can start and charg controller is idle
		  // nga4cob: due to battery abnormal condition relay chitter can happen that can be detected
		  // example due to relay operation : voltage change my lead to relay off ... which will be relay chitter
	     }
        }
   else // UART mode
   {
       if(only_once1==0) // configure UART MODE 
       {
            // GP4=1;
            charge_controller_status = NOT_OK;
            config_mode=ON;
            only_once=0;
            only_once1=1;
            GIE=0;
            //tri state the pins
            GPIO=(GPIO&(clear_bit_position(clear_bit_position(0xFF,MOSFET2_PIN),MOSFET1_PIN)));
            //enable uart timer
            TMR1IE=1;// reset timer interrupt flg
            //disable INVERTOR timer ISR
            TMR2IE=0;
            TRISIO=clear_bit_position(TRISIO,HW_PIN6);   // make it as output pin
            // ANSEL=set_bit_position(ANSEL,HW_PIN6);
            GIE=1;

       }
   }
}

void uart_frame_mgr(void) //100ms task
{
  _UBYTE  data_uart,cmd_uart,responce;
  static  _UBYTE dat_nibble,index;
  if(config_mode==ON)
  {
                  
		  data_uart=uart_rx();
		  cmd_uart=(data_uart>>4);
		  data_uart=(data_uart&0x0F);
		  
		  switch(cmd_uart)
		  {
			case 01 : // read eeprom any first 1 bytes
					responce=(EEP_READ(data_uart));
					break;
			case 02: // write eep rom  any firtst 1 bytes
                                        
                                        dat_nibble=dat_nibble+1;
                                        responce=dat_nibble; // waiting for next message
                                        switch(dat_nibble)
                                        {
                                           case 0x1:index=data_uart;break; // address
                                           // case 0x2:tempdata=((data_uart<<4)|tempdata);break;
                                           case 0x2:EEP_WRITE(index,data_uart);
                                                //  responce=0x42; // positiv responce
                                                  dat_nibble=0;
                                           default: break;
                                        }
                                        break;
			case 03: config_mode=OFF;
                                        dat_nibble=0;
                                        responce=0x7E;
					 return; // go back to normal mode
                        case 0x0F:return; // no data received */
			default : responce=0x7F;break; // wrong command
		  
		  }
		  
		  cmd_uart=uart_tx(responce); // positiv responce
               
  }
}
inline void app_inv1(void)
{
    if(config_mode==OFF)
    {
        CHS0=1; // select ADC chennal 0
        CHS1=0;
    }
}

