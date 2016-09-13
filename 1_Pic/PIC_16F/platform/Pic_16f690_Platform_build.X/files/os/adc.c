#include "common.h"
#if((HW_PIN_CFG_19==ADC)||(HW_PIN_CFG_18==ADC)||(HW_PIN_CFG_17==ADC)||(HW_PIN_CFG_16==ADC)||(HW_PIN_CFG_15==ADC)||(HW_PIN_CFG_14==ADC)||(HW_PIN_CFG_13==ADC)||(HW_PIN_CFG_12==ADC)||(HW_PIN_CFG_9==ADC)||(HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_8==ADC)||(HW_PIN_CFG_3==ADC))

#include "adc.h"
/*ADC drivers*/
//-----------ADC variables---------------------
_UINT16 adc_chn0_data,adc_chn1_data,adc_chn2_data,adc_chn3_data;

inline void Adc_Isr(void)
{
  // ADC interuupt
  if(ADIF) // do only if ADC interrupt is enabled
  {
   ADIF=0; // clear ADC interrupt flag
   
  }
}

inline void Adc_Ini(void)
{
//--------ADC configuration ----------------------
	   //----set proper system configuration file for this to take affect
	
		//------Analog pins and adc clock----------------------------
		ANSEL=0b00000000;
                ANSELH=0b00000000;
		#if((HW_PIN_CFG_3==ADC))
		ANS3=1;
		#endif
		#if((HW_PIN_CFG_7==ADC))
		ANS7=1;
		#endif
		#if((HW_PIN_CFG_8==ADC))
		ANS8=1;
		#endif
		#if((HW_PIN_CFG_9==ADC))
		ANS9=1;
		#endif
		#if((HW_PIN_CFG_12==ADC))
		ANS11=1;
		#endif
		#if((HW_PIN_CFG_13==ADC))
		ANS10=1;
		#endif
		#if((HW_PIN_CFG_14==ADC))
		ANS6=1;
		#endif
		#if((HW_PIN_CFG_15==ADC))
		ANS5=1;
		#endif
		#if((HW_PIN_CFG_16==ADC))
		ANS4=1;
		#endif
		#if((HW_PIN_CFG_17==ADC))
		ANS2=1;
		#endif
		#if((HW_PIN_CFG_18==ADC))
		ANS1=1;
		#endif
		#if((HW_PIN_CFG_19==ADC))
		ANS0=1;
		#endif

		ADCON1=(ADCON1&0x8F)|(0x30); // adc clock select as internal oscillator
                VCFG=0; // use VCC as referance voltage
		ADFM=1; // 10 bit ADC converted data format as right justified
		ADIF=0; // clear ADC interrupt flag
		GO=0;// stop ADC convertion
		ADON=1; // enable ADC
		ADIE=0; // enable ADC interrupt
		ADCON0=(ADCON0&0xC3); // select ADC chennal 0
       
}

/*ADC DRIVERS*/
void adc_updator(void)
{
static _UBYTE adc_no;
_UINT16 temp_data;
  if(GO==0) // if ADC convertion is completed for previous request
   {
     temp_data=((((_UINT16)ADRESH)<<8)|(_UINT16)ADRESL);
     switch(adc_no)
     {
         case 0 :
                #if(HW_PIN_CFG_7==ADC)
                // calculate the data
                adc_chn0_data=temp_data;
                // load data to respective registors
                #endif
                 #if(HW_PIN_CFG_6==ADC)
                CHS0=1; // select ADC chennal 0
                CHS1=0;
                #endif
                adc_no=1;
                break;
        case 1:
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