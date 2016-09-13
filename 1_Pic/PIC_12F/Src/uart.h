#include "type_def.h"
#include "system_config.h"

//----------UART-----------


#define BAUD_RATE_TMR_VAL 0xF984
#define BAUD_RATE_TMR_VAL_RX 0xF970
#define BAUD_RATE_TMR_VAL_PT1MS 0xFDA7  // 0.3ms

#if((HW_PIN_CFG_2==UART_TX)||(HW_PIN_CFG_4==UART_RX))
/*UART drivers*/
extern _UINT16 uart_tx_data;
extern _UINT8 tx_data_cnt;
extern _UINT16 uart_rx_data;
extern _UINT8 uart_rx_status;

#endif