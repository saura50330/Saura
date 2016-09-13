#if((HW_PIN_CFG_7==ADC)||(HW_PIN_CFG_6==ADC)||(HW_PIN_CFG_5==ADC)||(HW_PIN_CFG_3==ADC))
extern _UINT16 adc_chn0_data,adc_chn1_data,adc_chn2_data,adc_chn3_data;
void adc_updator(void);
inline void Adc_Isr(void);
inline void Adc_Ini(void);
#endif