#include "type_def.h"
//#include "system_config.h"

inline void EEP_ISR(void);
inline void EEP_INI(void);

void EEP_WRITE(_UBYTE Index, _UBYTE Data);
void EEP_WRITE_2(_UBYTE Index, _UBYTE Data);

_UBYTE  EEP_READ(_UBYTE Index);

inline void EEP_to_RAM(_UBYTE eep_error_stetegy);

void usege_timer_start_stop(_UBYTE start_stop_tmr);
void usege_timer(void);