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
// define MPU9250 register address
//****************************************
#define	SMPLRT_DIV		0x19	//Sample Rate Divider. Typical values:0x07(125Hz) 1KHz internal sample rate
#define	CONFIG				0x1A	//Low Pass Filter.Typical values:0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//Gyro Full Scale Select. Typical values:0x10(1000dps)
#define	ACCEL_CONFIG	0x1C	//Accel Full Scale Select. Typical values:0x01(2g)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define	PWR_MGMT_1		0x6B	//Power Management. Typical values:0x00(run mode)
#define	WHO_AM_I		  0x75	//identity of the device

#define	GYRO_ADDRESS   0xD0	  //Gyro and Accel device address
#define MAG_ADDRESS    0x18   //compass device address
#define ACCEL_ADDRESS  0xD0 

#define ADDRESS_AD0_LOW     0xD0 //address pin low (GND), default for InvenSense evaluation board
#define ADDRESS_AD0_HIGH    0xD1 //address pin high (VCC)
#define DEFAULT_ADDRESS     GYRO_ADDRESS
#define WHO_AM_I_VAL        0x73 //identity of MPU9250 is 0x71. identity of MPU9255 is 0x73.

_UINT8 Gyro_Calib_init[7][2]= 
{
   {PWR_MGMT_1,0x00},   // reg address , data
   {SMPLRT_DIV,0x07},
   {CONFIG,0x06},
   {GYRO_CONFIG,0x10},
   {ACCEL_CONFIG,0x01}
};

_UINT8 Gyro_Read_Data[6];

_UINT8 Acc_Read_Data[6];
_UINT8  Mag_Calib_Data[1][2]=
{
   {0x00,0x00},
};

_UINT8 Mag_Read_Data[6];

 #define CONTROL           0xF4  // W   Control register 
// Control register
#define READ_TEMPERATURE        0x2E 
#define READ_PRESSURE           0x34 
//Other
#define MSLP                    101325          // Mean Sea Level Pressure = 1013.25 hPA (1hPa = 100Pa = 1mbar)
#define LOCAL_ADS_ALTITUDE      2500            //mm     altitude of your position now
#define PRESSURE_OFFSET         0               //Pa    Offset

_UINT8  Alt_Calib_Data[2][2]=
{
   {CONTROL, READ_TEMPERATURE},   // reg address , data
   {CONTROL, READ_TEMPERATURE}
   
};

_UINT8 Alt_Read_Data[2][2];
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

extern _SINT16 magn[3];
extern _SINT16 accel[3], gyro[3];

void MPU9250_Init(void);
void MPU9250_READ_ACCEL(void);
void MPU9250_READ_GYRO(void);
void MPU9250_READ_MAG(void);
_UBYTE MPU9250_Check(void);
void MPU9250_CalAvgValue(_UINT8 *pIndex, _SINT16 *pAvgBuffer, _SINT16 InVal, _SINT32 *pOutVal);
void MPU9250_InitGyrOffset(void);

void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) ;
void BMP180_CalculateTruePressure(_SINT32 *pTruePressure);
void BMP180_CalAvgValue(_UINT8 *pIndex, _SINT32 *pAvgBuffer, _SINT32 InVal, _SINT32 *pOutVal);
void BMP180_CalculateAbsoluteAltitude(_SINT32 *pAltitude, _SINT32 PressureVal);
void BMP180_CalculateTrueTemperature(_SINT32 *pTrueTemperature);
// IMU appl layer
#define M_PI  (float)3.1415926535

extern _SINT16 accel[3], gyro[3];
extern float angles[3];

extern void IMU_Init(void); 
extern void IMU_GetYawPitchRoll(float *Angles) ;


//180 sensor
_SINT16 AC1, AC2, AC3, B1, B2, MB, MC, MD, _oss;  
_UINT16 AC4, AC5, AC6;
_SINT32 B5, UT, UP, Pressure0 = MSLP; 
_SINT32 PressureVal = 0, TemperatureVal = 0, AltitudeVal = 0;

_UINT8 Pres_Read_data[22];
/* ---- Registers ---- */
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
#define CONTROL           0xF4  // W   Control register 
#define CONTROL_OUTPUT    0xF6  // R   Output registers 0xF6=MSB, 0xF7=LSB, 0xF8=XLSB

// BMP180 Modes
#define MODE_ULTRA_LOW_POWER    0 //oversampling=0, internalsamples=1, maxconvtimepressure=4.5ms, avgcurrent=3uA, RMSnoise_hPA=0.06, RMSnoise_m=0.5
#define MODE_STANDARD           1 //oversampling=1, internalsamples=2, maxconvtimepressure=7.5ms, avgcurrent=5uA, RMSnoise_hPA=0.05, RMSnoise_m=0.4
#define MODE_HIGHRES            2 //oversampling=2, internalsamples=4, maxconvtimepressure=13.5ms, avgcurrent=7uA, RMSnoise_hPA=0.04, RMSnoise_m=0.3
#define MODE_ULTRA_HIGHRES      3 //oversampling=3, internalsamples=8, maxconvtimepressure=25.5ms, avgcurrent=12uA, RMSnoise_hPA=0.03, RMSnoise_m=0.25

// Control register
#define READ_TEMPERATURE        0x2E 
#define READ_PRESSURE           0x34 
//Other
#define MSLP                    101325          // Mean Sea Level Pressure = 1013.25 hPA (1hPa = 100Pa = 1mbar)
#define LOCAL_ADS_ALTITUDE      2500            //mm     altitude of your position now
#define PRESSURE_OFFSET         0               //Pa    Offset


typedef struct
{
	_UINT8 Index;
	_SINT32 AvgBuffer[8];
}BMP180_AvgTypeDef;


enum
{
	START_TEMPERATURE_MEASUREMENT = 0,
	READ_UT_AND_START_PRESSURE_MEASUREMENT,
	READ_UP_CAL_TRUE_PRESSURE_TEMPERATURE,
};

extern _SINT32 PressureVal, TemperatureVal, AltitudeVal;
extern void CalTemperatureAndPressureAndAltitude(void);
extern void BMP180_Init(void);
#define BMP180_ADDR                 0xEE     // default I2C address 
#define IDLE 0
#define WAIT_FOR_TX 1
#define WAIT_FOR_RX 2

#define IMU_INIT 0
#define IMU_CALIB_READ 1
#define IMU_CALIB_CALC 2
#define IMU_CALIB_AVG 3
#define IMU_PRES 4
#define CALIB_PRES 5

#define READ_GYRO 0
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

#define  ALL_DONE 14
#define MAX_RD_WAIT 5  // 5 ms
#define MAX_WR_WAIT 5 // 5ms

#define Kp 4.50f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 1.0f    // integral gain governs rate of convergence of gyroscope biases

/* [] END OF FILE */
