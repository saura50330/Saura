#include "common.h"
#include "adc.h"
#include "app_process_pub.h"
# include "pic12f683.h"
/*ADC drivers*/
extern _UBYTE config_mode,hyst_var;
#if((HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_6==ADC)||(HW_PIN_CFG_5==ADC)||(HW_PIN_CFG_3==ADC))
//-----------ADC variables---------------------
_UINT16 adc_chn0_data,adc_chn1_data,adc_chn2_data,adc_chn3_data;

inline void ADC_ISR(void)
{
  // ADC interuupt
  if(ADIF) // do only if ADC interrupt is enabled
  {
   ADIF=0; // clear ADC interrupt flag
   
  }
}

inline void ADC_INI(void)
{
//----set proper system configuration file for this to take affect
	
		//------Analog pins and adc clock----------------------------
		ANSEL=0b00000000;
      	        #if((HW_PIN_CFG_7==ADC))
		ANSEL=set_bit_position(ANSEL,HW_PIN7);   
		#endif
		#if((HW_PIN_CFG_6==ADC))	
		ANSEL=set_bit_position(ANSEL,HW_PIN6);   
		#endif
		#if((HW_PIN_CFG_5==ADC))	
		ANSEL=set_bit_position(ANSEL,HW_PIN5);   
		#endif
		#if((HW_PIN_CFG_3==ADC))	
		ANSEL=set_bit_position(ANSEL,(HW_PIN3-1));   
		#endif
		
		ANSEL|=0b01110000; // adc clock select
                VCFG=0; // use VCC as referance voltage
		ADFM=1; // 10 bit ADC converted data format as right justified
		ADIF=0; // clear ADC interrupt flag
		GO=0;// stop ADC convertion
		ADON=1; // enable ADC
		ADIE=0; // enable ADC interrupt
		CHS0=0; // select ADC chennal 0
		CHS1=0;
}

/*ADC DRIVERS*/
void adc_updator(void)
{
static _UBYTE adc_no;
_UINT16 temp_data,temp_data1;
  if(GO==0) // if ADC convertion is completed for previous request
   {
     temp_data=((((_UINT16)ADRESH)<<8)|(_UINT16)ADRESL);
     switch(adc_no)
     {
         case 0 :
                #if(HW_PIN_CFG_7==ADC)
                // calculate the data
                // check for voltage dip due to overload or weak battery
                temp_data1=(adc_chn0_data>=temp_data)?(adc_chn0_data-temp_data):0; //check for delta v of battery and its sign
                if(temp_data1>MAX_VOLTAGE_DIP)
                {   
                     hyst_var=1;
                    // set battery for charging mode
                }
                adc_chn0_data=temp_data;
                // load data to respective registors
                #endif
                 #if(HW_PIN_CFG_6==ADC)
                CHS0=1; // select ADC chennal 0
                CHS1=0;
                #endif
                app_inv1();
                adc_no=1;
                break;
        case 1:
                app_inv2
                #if(HW_PIN_CFG_6==ADC)
                // calculate the data
                adc_chn1_data=temp_data;
                // load data to respective registors
                #endif
                 #if(HW_PIN_CFG_5==ADC)
                CHS0=0; // select ADC chennal 0
                CHS1=1;
                #endif
                adc_no=2;break;
         case 2:
                #if(HW_PIN_CFG_5==ADC)
                // calculate the data
                adc_chn2_data=temp_data;
                // load data to respective registors
                #endif
                 #if(HW_PIN_CFG_3==ADC)
                CHS0=1; // select ADC chennal 0
                CHS1=1;
                #endif
                adc_no=3;break;
         case 3:
                #if(HW_PIN_CFG_3==ADC)
                // calculate the data
                adc_chn3_data=temp_data;
                // load data to respective registors
                #endif
                 #if(HW_PIN_CFG_7==ADC)
                CHS0=0; // select ADC chennal 0
                CHS1=0;
                #endif
                adc_no=0;break;
        default:
                #if(HW_PIN_CFG_7==ADC)
                CHS0=0; // select ADC chennal 0
                CHS1=0;
                #endif
                adc_no=0;break;
     }
         GO=1;//start converion
   }
}
#endif