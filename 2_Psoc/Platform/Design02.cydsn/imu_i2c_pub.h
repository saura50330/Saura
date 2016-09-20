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
#include "type_def.h"

#define IMU_CMD_INIT 0
#define IMU_CMD_START 1
#define IMU_CMD_STOP 2

#define INIT_SUCC 7 // INIT_COMP
#define NOT_INIT 0 //IDLE
void IMU_Command(_UBYTE Command);
_UBYTE IMU_Read_St(void);

/* [] END OF FILE */
