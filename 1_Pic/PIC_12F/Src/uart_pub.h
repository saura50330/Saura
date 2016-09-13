

inline void UART_INI(void);
inline void UART_ISR(void);
#if(UART_TX_PIN==UART_TX)
_UBYTE uart_tx(_UBYTE data_byte);
#endif
#if(HW_PIN_CFG_4==UART_RX)
_UINT8 uart_rx(void);
#endif
