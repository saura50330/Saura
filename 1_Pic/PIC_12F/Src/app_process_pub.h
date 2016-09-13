#include "type_def.h"
// charge controller configuration 
// rely charging hysterisis concept
#define BATT_CHARGE_MAX 135     //  at this batt voltage rely disconect battery from solar panel
#define BATT_CHARGE_MIN 110     //  at this batt voltage rely connects battery to solar panel
#define SOLAR_VOLT_NIGHT_TYM 80 // this is solar panal voltage below which its considerd as night time

//invertor backup stretegy values
#define TYPE1 0  // : mode used for bakeup at night time only and when ther is mais failuer
#define TYPE2 1  // :invertor turn on when ther is no power supply : mode used for homes
#define TYPE3 2  // : inveror turns on automaticaly in night , mode used for in automatic street ligts and energy sevaing

#define MAX_VOLTAGE_DIP 30 // delta v is 3 volts

#define  app_inv2  if(config_mode==OFF) \
{ adc_chn1_data=temp_data;}

/*ini*/
void app_sine_wave_ini(void);

void app_sine_wave_control_ini(void);

/*special*/
void app_sine_wave(void);
/*10ms task processes*/
// void process1(void); 
// void process2(void); 

/*100ms task processes*/
void app_sine_wave_control(void);
// void process1(void); 
// void process2(void); 

/*1000ms task processes*/
//void test_routine(void);
void Charge_Controller(void);

inline void app_inv1(void);
void uart_frame_mgr(void);
// void process1(void); 
// void process2(void); 