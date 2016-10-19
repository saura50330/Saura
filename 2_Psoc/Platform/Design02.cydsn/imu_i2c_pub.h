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
//karnel tasks
void MPU9250_Init(void); // 10ms
void Read_From_ALL(void); // 10ms
void I2C_Mgr(void); // 1ms task premptiv

// API
void IMU_Command(_UBYTE Command);
_UBYTE IMU_Read_St(void);

/* [] END OF FILE */
