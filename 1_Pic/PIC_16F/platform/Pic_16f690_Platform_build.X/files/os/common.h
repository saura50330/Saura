#include <xc.h>
#include "pic16f690.h"
#include "type_def.h"
#include "system_config.h"
#include "sevlb_pub.h"
#include "system_pub.h"
#include "adc_pub.h"
#include "eep_pub.h"
#include "uart_pub.h"
#include "os_pub.h"
#include "dio_pub.h"

#if((HW_PIN_CFG_14==PWM)||(HW_PIN_CFG_7==PWM)||(HW_PIN_CFG_6==PWM)||(HW_PIN_CFG_5==PWM))
#include "pwm_pub.h"
#endif
