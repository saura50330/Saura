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
#include "I2C_1.h"

int16_t magn[3];
int16_t accel[3], gyro[3];
unsigned char BUF[10];
MPU9250_TypeDef MPU9250_Offset={0};
MPU9250_TypeDef_Off MPU9250_Magn_Offset={0};

uint8 stI2CMgr;

void I2C_Mgr(void) // 1ms task
{
    static uint8 i2c_timeout;
    switch(stI2CMgr)
    {
        case WAIT_FOR_TX: 
        if(0u != (I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT))
        {
         stI2CMgr=IDLE;
            i2c_timeout=0;
        }
        else  // exception handling
        {
           i2c_timeout++;
           if(i2c_timeout>MAX_WR_WAIT)
           {
            stI2CMgr=IDLE;
           }
        }
        break;
        case WAIT_FOR_RX:
        if(0u != (I2C_1_MasterStatus() & I2C_1_MSTAT_RD_CMPLT))
        {
          stI2CMgr=IDLE;
          i2c_timeout=0;
        }
        else  // exception handling
        {
           i2c_timeout++;
           if(i2c_timeout>MAX_RD_WAIT)
           {
            stI2CMgr=IDLE;
           }  
        }
        break;
        case IDLE:
        default: stI2CMgr=IDLE;break;
    }
    
}
uint8 I2C_WRITE(uint8 SaveAdress, (uint8 *) wrData, uint8 Data_length) // sends bytes to slave
{
    if(stI2CMgr==IDLE)
    {
      I2C_1_MasterClearStatus(); /* Clear any previous status */
      I2C_1_MasterWriteBuf(SaveAdress,wrData, Data_length, I2C_MODE_COMPLETE_XFER);
      stI2CMgr=WAIT_FOR_TX;
	  retuen(1);
    }
	else
	{
	retuen(0);
	}
}
}
uint8 I2C_READ(uint8 SaveAdress, (uint8 *) rdData, uint8 Data_length) // sends bytes to slave
{
    if(stI2CMgr==IDLE)
    {
      I2C_1_MasterClearStatus(); /* Clear any previous status */
      uint8 I2C_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 Data_length, 2C_MODE_COMPLETE_XFER);
      stI2CMgr=WAIT_FOR_RX;
      retuen(1);
	}
	else
	{
	retuen(0);
	}
}

void MPU9250_Init(void) // should be called when os is running 50 ms task
{
    static uint8 st_IMU;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	uint8_t i;
	int16_t InBuffer[3] = {0}; 
	static int32_t OutBuffer[3] = {0};
	static MPU9250_AvgTypeDef MPU9250_Filter[3];
	
	switch(st_IMU)
	{
		case IMU_INIT:	
					   if(I2C_WRITE(GYRO_ADDRESS, Gyro_Calib_init[0][0], 10))  // send all init data
								{
								  st_MCU=CALIB; 
								}	
								break;
	    case IMU_CALIB_READ: if(I2C_READ(GYRO_ADDRESS, Gyro_Calib_data[0],6))  // read all gyro values in Gyro_Calib_data 
						   {
						    st_IMU=IMU_CALIB_CALC;
					       }
						   break;
		case IMU_CALIB_CALC: 
							// read 32 gyro readings 
						   // add them all
		
							if(stI2CMgr==IDLE)	
                            {
								InBuffer[0]=(Gyro_Calib_data[1]<<8)|Gyro_Calib_data[0];
								InBuffer[1]=(Gyro_Calib_data[3]<<8)|Gyro_Calib_data[2];
								InBuffer[2]=(Gyro_Calib_data[5]<<8)|Gyro_Calib_data[4];
								
								for(i = 0; i < 3; i ++)	
								{
									MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
								}
								gyro[0] = *(OutBuffer + 0) - MPU9250_Offset.X;
								gyro[1] = *(OutBuffer + 1) - MPU9250_Offset.Y;
								gyro[2] = *(OutBuffer + 2) - MPU9250_Offset.Z;
						        
								TempGx += gyro[0];
								TempGy += gyro[1];
								TempGz += gyro[2];
						        
								// do this 32 times
							    if(calc_count<32)
								{
								  st_IMU=IMU_CALIB_READ;
								  calc_count++
								}
								else
								{
									calc_count=0;
									st_IMU=IMU_CALIB_AVG;
								}
								
							}
							break;
						 
		case IMU_CALIB_AVG:
						// take average of all readings 
						MPU9250_Offset.X = TempGx >> 5;
						MPU9250_Offset.Y = TempGy >> 5;
						MPU9250_Offset.Z = TempGz >> 5;
						st_IMU=IMU_INIT;
						// TODO: turn off this task     
						break;
	}
					       
}



/**
  * @brief Get compass datas
  * @param  None
  * @retval None
  */
void MPU9250_READ_MAG(void)
{ 
     uint8_t i;
	 int16_t InBuffer[3] = {0}; 
	 static int32_t OutBuffer[3] = {0};
	 static MPU9250_AvgTypeDef MPU9250_Filter[3];

    I2C_WriteOneByte(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode 
    Delay_ms(10);
    I2C_WriteOneByte(MAG_ADDRESS,0x0A,0x01);	
    Delay_ms(10);
    BUF[0]=I2C_ReadOneByte (MAG_ADDRESS,MAG_XOUT_L);
    BUF[1]=I2C_ReadOneByte (MAG_ADDRESS,MAG_XOUT_H);
    InBuffer[1] =(BUF[1]<<8)|BUF[0];

    BUF[2]=I2C_ReadOneByte(MAG_ADDRESS,MAG_YOUT_L);
    BUF[3]=I2C_ReadOneByte(MAG_ADDRESS,MAG_YOUT_H);
    InBuffer[0] =	(BUF[3]<<8)|BUF[2];
    
    BUF[4]=I2C_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_L);
    BUF[5]=I2C_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_H);
    InBuffer[2] =	(BUF[5]<<8)|BUF[4];	
    InBuffer[2] = -InBuffer[2];
	 
   for(i = 0; i < 3; i ++)	
   {
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
   } 
	    magn[0] = *(OutBuffer + 0)-MPU9250_Magn_Offset.X_Off_Err;
			magn[1] = *(OutBuffer + 1)-MPU9250_Magn_Offset.Y_Off_Err;
			magn[2] = *(OutBuffer + 2)-MPU9250_Magn_Offset.Z_Off_Err;
}
/**
  * @brief Get accelerometer datas
  * @param  None
  * @retval None
  */
void MPU9250_READ_ACCEL(void)
{ 
   uint8_t i;
	 int16_t InBuffer[3] = {0}; 
	 static int32_t OutBuffer[3] = {0};
	 static MPU9250_AvgTypeDef MPU9250_Filter[3];

   BUF[0]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_L); 
   BUF[1]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_H);
   InBuffer[0]=	(BUF[1]<<8)|BUF[0];

   BUF[2]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
   BUF[3]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
   InBuffer[1]=	(BUF[3]<<8)|BUF[2];
					   
   BUF[4]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   InBuffer[2]=	(BUF[5]<<8)|BUF[4];			       
   
   for(i = 0; i < 3; i ++)	
   {
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
   }
   accel[0] = *(OutBuffer + 0);
   accel[1] = *(OutBuffer + 1);
   accel[2] = *(OutBuffer + 2); 
}

/**
  * @brief  Digital filter
  * @param *pIndex:
  * @param *pAvgBuffer:
  * @param InVal:
  * @param pOutVal:
  *
  * @retval None
  *               
  */
void MPU9250_CalAvgValue(uint8_t *pIndex, int16_t *pAvgBuffer, int16_t InVal, int32_t *pOutVal)
{	
	uint8_t i;
	
	*(pAvgBuffer + ((*pIndex) ++)) = InVal;
  	*pIndex &= 0x07;
  	
  	*pOutVal = 0;
	for(i = 0; i < 8; i ++) 
  	{
    	*pOutVal += *(pAvgBuffer + i);
  	}
  	*pOutVal >>= 3;
}
// IMU APllication layer
//int16_t accel[3], gyro[3];
float angles[3];
volatile float q0, q1, q2, q3; 


/**
  * @brief  invSqrt
  * @param  
  * @retval 
  */

float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	
	long i = *(long*)&y;                //get bits for floating value
	i = 0x5f3759df - (i >> 1);          //gives initial guss you
	y = *(float*)&i;                    //convert bits back to float
	y = y * (1.5f - (halfx * y * y));   //newtop step, repeating increases accuracy
	
	return y;
}


/**
  * @brief  initializes IMU
  * @param  None
  * @retval None
  */
void IMU_Init(void)
{	 
	MPU9250_Init();
	BMP180_Init();
	
  	q0 = 1.0f;  
  	q1 = 0.0f;
  	q2 = 0.0f;
  	q3 = 0.0f;
}

#define Kp 4.50f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 1.0f    // integral gain governs rate of convergence of gyroscope biases

/**
  * @brief  Updata attitude and heading 
  * @param  ax: accelerometer X
  * @param  ay: accelerometer Y
  * @param  az: accelerometer Z
  * @param  gx: gyroscopes X
  * @param  gy: gyroscopes Y
  * @param  gz: gyroscopes Z
  * @param  mx: magnetometer X
  * @param  my: magnetometer Y
  * @param  mz: magnetometer Z
  * @retval None
  */
void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
{
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float exInt = 0.0, eyInt = 0.0, ezInt = 0.0;
	float ex, ey, ez, halfT = 0.024f;

	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;   
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;          

	norm = invSqrt(ax * ax + ay * ay + az * az);       
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	norm = invSqrt(mx * mx + my * my + mz * mz);          
	mx = mx * norm;
	my = my * norm;
	mz = mz * norm;

	// compute reference direction of flux
	hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
	hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
	hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);         
	bx = sqrt((hx * hx) + (hy * hy));
	bz = hz;     

	// estimated direction of gravity and flux (v and w)
	vx = 2 * (q1q3 - q0q2);
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
	wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
	wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);  

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
		exInt = exInt + ex * Ki * halfT;
		eyInt = eyInt + ey * Ki * halfT;	
		ezInt = ezInt + ez * Ki * halfT;

		gx = gx + Kp * ex + exInt;
		gy = gy + Kp * ey + eyInt;
		gz = gz + Kp * ez + ezInt;
	}

	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
	q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;  

	norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
}


/**
  * @brief  Get quaters
  * @param  None
  * @retval None
  */
void IMU_GetQuater(void)
{
	float MotionVal[9];
			
    MPU9250_READ_ACCEL();
    MPU9250_READ_GYRO();
    MPU9250_READ_MAG();

    MotionVal[0]=gyro[0]/32.8;
    MotionVal[1]=gyro[1]/32.8;
    MotionVal[2]=gyro[2]/32.8;
    MotionVal[3]=accel[0];
    MotionVal[4]=accel[1];
    MotionVal[5]=accel[2];
    MotionVal[6]=magn[0];
    MotionVal[7]=magn[1];
    MotionVal[8]=magn[2];
    
 	IMU_AHRSupdate((float)MotionVal[0] * 0.0175, (float)MotionVal[1] * 0.0175, (float)MotionVal[2] * 0.0175,
   	(float)MotionVal[3], (float)MotionVal[4], (float)MotionVal[5], (float)MotionVal[6], (float)MotionVal[7], MotionVal[8]);
}


/**
  * @brief  Get Yaw Pitch Roll
  * @param  None
  * @retval None
  */
void IMU_GetYawPitchRoll(float *Angles) 
{
  IMU_GetQuater(); 
	Angles[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
	Angles[2] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	Angles[0] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   
}
// PSOC support 
// i2c master mode write to slave
uint8 I2C_MasterWriteBuf(uint8 slaveAddress, uint8 * wrData, uint8 cnt, uint8 mode);
    // code for write to slave
    I2C_MasterClearStatus(); /* Clear any previous status */
    I2C_MasterWriteBuf(0x08, (uint8 *) wrData, 10, I2C_MODE_COMPLETE_XFER);
    for(;;)
    {
    if(0u != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT))
    {
    /* Transfer complete. Check Master status to make sure that transfer
    completed without errors. */
    break;
    }
    }
 // code for read from slave
    

// read from slave
uint8 I2C_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 cnt, uint8 mode);
/* [] END OF FILE */
