#include "type_def.h"


/*public functions used by other files file*/
/*only this file should be included in other module*/
inline void Os_Timer_ISR(void);
inline void app_speciel(void);
inline void Os_Check(_UINT8 Os_Current_Running_task);
inline void Os_Ini(void);
inline void app_ini(void);