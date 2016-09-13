#include "type_def.h"
#include "system_config.h"

//-------------OS--------
#define TIMER_0_1MS 135  // interrupt on every 1 ms
//
#define TMR2_PT1MS 242  // timer 2value to get 0.1 ms interrupt


/*private functions used by only os.c file*/




void Os_10ms_Task(void);
void Os_100ms_Task(void);
void Os_1000ms_Task(void);
