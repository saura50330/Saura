#define UART_FRAME_PROCESSED 4
#define UART_FRAME_RECEIVED 3
#define TRANSMIT_DATA 1
#define UART_RECEIVING 2
#define IDLE 0
typedef struct
    {
    _UBYTE com_status;
    _UBYTE rx_period;
    _UBYTE *rx_data_ptr;
    _UBYTE RX_DATA[8];
    _UBYTE rx_period_max;
    }RX_UART;

typedef struct
    {
        _UBYTE com_satus;
        _UBYTE TX_DATA[8];
        _UBYTE data_length;
        _UBYTE data_index;
    }TX_UART;

typedef struct 
{
    RX_UART Uart_Rx;
    TX_UART Uart_Tx;
}UART_MGR;