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
// GYROSCOPE

#define	GYRO_ADDRESS   (0xD0>>1)	  //Gyro and Accel device address: 0x68

// define GYRO register address
//****************************************
#define	SMPLRT_DIV		0x19	//Sample Rate Divider. Typical values:0x07(125Hz) 1KHz internal sample rate
#define	PWR_MGMT_1		0x6B	//Power Management. Typical values:0x00(run mode)
#define	CONFIG			0x1A	//Low Pass Filter.Typical values:0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//Gyro Full Scale Select. Typical values:0x10(1000dps)
#define	ACCEL_CONFIG	0x1C	//Accel Full Scale Select. Typical values:0x01(2g)

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

//  GYRO_CONFIG
#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

// stores the gyroscope commands
_UINT8 Gyro_Calib_Data[7][2]= 
{
   {PWR_MGMT_1,0x00},   //select Internal 20MHz oscillator
   {SMPLRT_DIV,0x07},   //sample rate 
   {CONFIG,0x06},       //init seq 3
   {GYRO_CONFIG,GYRO_FULL_SCALE_1000_DPS},  //init seq 4   // GYRO_FULL_SCALE_1000_DPS
   {ACCEL_CONFIG,0x00},  //init seq 5  // was 0x01 ,00+-2g, 11 selated +-16 g
   {0x37,0x02}           // bypass gyroscope i2c bus
};



_UBYTE Gyro_Reg[6]=
{
    GYRO_XOUT_L,
    GYRO_XOUT_H,
    GYRO_YOUT_L,
    GYRO_YOUT_H,
    GYRO_ZOUT_L,
    GYRO_ZOUT_H    
};
typedef struct
{
	_SINT16 X;
	_SINT16 Y;
	_SINT16 Z;
}MPU9250_TypeDef;

typedef struct
{
	_SINT16 X_Off_Err;
	_SINT16 Y_Off_Err;
	_SINT16 Z_Off_Err;
}MPU9250_TypeDef_Off;

typedef struct
{
	_UINT8 Index;
	_SINT16 AvgBuffer[8];
}MPU9250_AvgTypeDef;


// ACEELROMETER
#define ACCEL_ADDRESS  (0xD0>>1) 

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

const _UINT8 Acc_Reg[6]=
{
  ACCEL_XOUT_L,
  ACCEL_XOUT_H,
  ACCEL_YOUT_L,
  ACCEL_YOUT_H,
  ACCEL_ZOUT_L,
  ACCEL_ZOUT_H
};

// MAGN COMPAS

#define MAG_ADDRESS    (0x18>>1)   //compass device address 0x0C

_UINT8  Mag_Calib_Data[4][2]=
{
   {0x0A,0x01}, // 14-bit output , Single measurement mode
   {0x00,0x00},
   {0x00,0x00},
   {0x00,0x00}
};

#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

const _UINT8 Mag_Reg[6]=
{
    MAG_XOUT_L,
    MAG_XOUT_H,
    MAG_YOUT_L,
    MAG_YOUT_H,
    MAG_ZOUT_L,
    MAG_ZOUT_H   
};
// resolution of mag sensor for 14 bit values

// resolution of mag sensor for 16 bit values
// 32760-0- (-32760) = 4912 - 0 - (-4912)  uTesla, resolution+- 0.15uT

// TEMPERATURE

//PRESSURE---------------------------
// address
#define BMP180_ADDR                 (0xEE>>1)     // default I2C address 

// registers
 #define CONTROL           0xF4  // W   Control register 
 #define CONTROL_OUTPUT    0xF6  // R   Output registers 0xF6=MSB, 0xF7=LSB, 0xF8=XLSB

 #define READ_TEMPERATURE        0x2E 
 #define READ_PRESSURE           0x34 

#define CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define CAL_AC3           0xAE  // R   Calibration data (16 bits)    
#define CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define CAL_B1            0xB6  // R   Calibration data (16 bits)
#define CAL_B2            0xB8  // R   Calibration data (16 bits)
#define CAL_MB            0xBA  // R   Calibration data (16 bits)
#define CAL_MC            0xBC  // R   Calibration data (16 bits)
#define CAL_MD            0xBE  // R   Calibration data (16 bits)

const _UBYTE Pres_Reg[11]={
    CAL_AC1,
    CAL_AC2,
    CAL_AC3,
    CAL_AC4,
    CAL_AC5,
    CAL_AC6,
    CAL_B1,
    CAL_B2,
    CAL_MB,
    CAL_MC,
    CAL_MD
};
// BMP180 Modes
  // #define MODE_ULTRA_LOW_POWER    0 //oversampling=0, internalsamples=1, maxconvtimepressure=4.5ms, avgcurrent=3uA, RMSnoise_hPA=0.06, RMSnoise_m=0.5
  #define MODE_STANDARD           1 //oversampling=1, internalsamples=2, maxconvtimepressure=7.5ms, avgcurrent=5uA, RMSnoise_hPA=0.05, RMSnoise_m=0.4
  // #define MODE_HIGHRES            2 //oversampling=2, internalsamples=4, maxconvtimepressure=13.5ms, avgcurrent=7uA, RMSnoise_hPA=0.04, RMSnoise_m=0.3
     #define MODE_ULTRA_HIGHRES      3 //oversampling=3, internalsamples=8, maxconvtimepressure=25.5ms, avgcurrent=12uA, RMSnoise_hPA=0.03, RMSnoise_m=0.25
// constants 
//Other
#define MSLP                    101325          // Mean Sea Level Pressure = 1013.25 hPA (1hPa = 100Pa = 1mbar)
#define LOCAL_ADS_ALTITUDE     950//  2500            //mm     altitude of your position now
#define PRESSURE_OFFSET         0               //Pa    Offset


_UINT8  Alt_Calib_Data[2][2]=
{
   {CONTROL, READ_TEMPERATURE},   // reg address , data
   {CONTROL, (READ_PRESSURE + (MODE_STANDARD << 6))}
   
};

typedef struct
{
	_UINT8 Index;
	_SINT32 AvgBuffer[8];
}BMP180_AvgTypeDef;


void MPU9250_CalAvgValue(_UINT8 *pIndex, _SINT16 *pAvgBuffer, _SINT16 InVal, _SINT32 *pOutVal);

void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) ;
void BMP180_CalculateTruePressure(_SINT32 *pTruePressure);
void BMP180_CalAvgValue(_UINT8 *pIndex, _SINT32 *pAvgBuffer, _SINT32 InVal, _SINT32 *pOutVal);
void BMP180_CalculateAbsoluteAltitude(_SINT32 *pAltitude, _SINT32 PressureVal);
void BMP180_CalculateTrueTemperature(_SINT32 *pTrueTemperature);

// IMU appl layer

#define IDLE 0
#define WAIT_FOR_TX 1
#define WAIT_FOR_RX 2
#define START_RECEIVE_SEQ_WR_REG 3
#define START_RECEIVE_SEQ_RD_DATA 4
#define Tx_ERR 5

#define IMU_INIT 6
#define IMU_CALIB_READ 1
#define IMU_CALIB_CALC 2
#define IMU_CALIB_AVG 3
#define IMU_PRES 4
#define CALIB_PRES 5
#define INIT_COMP 7

#define READ_GYRO 14
#define CALC_GYRO 1
#define READ_ACC 2
#define CALC_ACC 3
#define BYPASS_GYRO 4
#define START_MAG 5
#define READ_MAG 6
#define CALC_MAG 7
#define SEL_TEMP 8
#define READ_TEMP 9
#define CALC_TEMP 10
#define SEL_PRES 11
#define READ_PRES 12
#define CALC_PRES 13

#define ALL_DONE 14
#define MAX_RD_WAIT 5  // 5 ms
#define MAX_WR_WAIT 5 // 5ms

#define Kp 4.50f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 1.0f    // integral gain governs rate of convergence of gyroscope biases

_UBYTE I2C_WRITE(_UINT8 SaveAdress,_UINT8 *wrData, _UINT8 Data_length);
_UBYTE I2C_READ(_UINT8 SaveAdress,_UINT8 SaveReg,_UINT8 *rdData, _UINT8 Data_length); // sends bytes to slave
/* [] END OF FILE */
