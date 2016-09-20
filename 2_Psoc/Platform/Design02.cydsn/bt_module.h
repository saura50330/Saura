/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

// bt responce time is around 500ms 
//works on 9600 baud
// once paird LED stays on
// once paired with master any data sent on uart is trasperant to master
// hc-06 can act as slave only
#include "type_def.h"

#define BT_IDLE 0
#define BT_INI 1

void bt_module_config(_UINT8 Command);
void bt_module_manager(void);
/* [] END OF FILE */
