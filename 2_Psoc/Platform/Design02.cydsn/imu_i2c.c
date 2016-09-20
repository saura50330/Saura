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
#include "common.h"
#if(IMU_SENSOR==ENABLE)
    
#include "imu_i2c.h"
#include <math.h>

// final values of interest
float MotionVal[9];    // contains final gyro acc,mag values
float angles[3];       // contains final pich yow roll
_SINT32 PressureVal = 0, TemperatureVal = 0, AltitudeVal = 0; // stores final values of all
  
// complex function
volatile float q0, q1, q2, q3; 

// manger states
static _UINT8 st_IMU;  // keeps status of initialisation state machine
static _UINT8 st_Read; // store state machine value 
      
unsigned char BUF[10];

MPU9250_TypeDef_Off MPU9250_Magn_Offset={0,0,0};





 
uint8 stI2CMgr;
 static _UBYTE rx_Slv_add,temp_rx_slv_reg,*rx_rd_data_ptr,rx_Slv_dta_lnt; // taperory value for rx function of i2c 
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
        case START_RECEIVE_SEQ_WR_REG:
        if(0u != (I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT))
        {
            I2C_1_MasterClearStatus();
            I2C_1_MasterReadBuf(rx_Slv_add, rx_rd_data_ptr, rx_Slv_dta_lnt, I2C_1_MODE_REPEAT_START);
        
            stI2CMgr=START_RECEIVE_SEQ_RD_DATA;
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
        case START_RECEIVE_SEQ_RD_DATA:
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
uint8 I2C_WRITE(_UINT8 SaveAdress,_UINT8 *wrData, _UINT8 Data_length) // sends bytes to slave
{
    if(stI2CMgr==IDLE)
    {
      I2C_1_MasterClearStatus(); /* Clear any previous status */
      I2C_1_MasterWriteBuf(SaveAdress,wrData, Data_length, I2C_1_MODE_COMPLETE_XFER);
      stI2CMgr=WAIT_FOR_RX;    
	  return(1);
    }
	else
	{
	  return(0);
	}
}


uint8 I2C_READ(_UINT8 SaveAdress,_UINT8 SaveReg,_UINT8 *rdData, _UINT8 Data_length) // sends bytes to slave
{
  
    
    if(stI2CMgr==IDLE)
    {
      I2C_1_MasterClearStatus(); /* Clear any previous status */
      temp_rx_slv_reg=SaveReg;
      rx_Slv_add=SaveAdress;
      rx_rd_data_ptr=rdData;  
      rx_Slv_dta_lnt=Data_length;
      // select the register to be read
      I2C_1_MasterWriteBuf(SaveAdress,&temp_rx_slv_reg, 1, I2C_1_MODE_NO_STOP);   
      // read the alue afr acc  
      stI2CMgr=START_RECEIVE_SEQ_WR_REG;
    
      return(1);
	}
	else
	{
	  return(0);
	}
}

// gyro globles
 MPU9250_TypeDef MPU9250_Offset={0,0,0};  // stores ofset perameter for 3 axis of gyro
 MPU9250_AvgTypeDef MPU9250_Filter[3];  // buffer and index for gyro
 static _UINT8 Gyro_Read_Data[6]; // stores real time gyro values receved from i2c
 static _SINT32 OutBuffer[3] = {0,0,0}; // stores integrated gro values

// pressure sensor fglobles
 static _UINT8 Alt_Read_Data[2][3]; // stores temperature data [0][] and pressure data [1][]
     
_SINT16 AC1, AC2, AC3, B1, B2, MB, MC, MD, _oss;  
 _UINT16 AC4, AC5, AC6;
 _SINT32 B5, UT, UP;
 _SINT32 Pressure0 = MSLP; // pressure at sea level
 static _SINT16 gyro[3];

void MPU9250_Init(void) // should be called when os is running 50 ms task
{
     static _UINT8 calc_count; // keeps counter value while calculating average
    
    // gyro variables
    
	_SINT16 InBuffer[3] = {0,0,0};  // stores real time combined gyro values
    _SINT16 gyro[3];
    static _SINT32	TempGx = 0, TempGy = 0, TempGz = 0; // stores added gyrovalue for avraging purpose
	
    // pressure sensor variables
    static _UINT8 Pres_Read_data[22]; // stores temporory pressure sensor data
    static _SINT32 Sum = 0;
    static _UINT8 reg_count; // stores count vaue for rx register read
    float Temp = 0.0f;
    
    _UINT8 i;  // for for loop purpose
   
      
	switch(st_IMU)
	{
		case IMU_INIT:	
					   if(I2C_WRITE(GYRO_ADDRESS, &Gyro_Calib_Data[0][0], 10))  // send first 10 bytes for initialisation
						{
						  st_IMU=IMU_CALIB_READ; 
						}	
						break;
	    case IMU_CALIB_READ: if(I2C_READ(GYRO_ADDRESS,Gyro_Reg[reg_count],&Gyro_Read_Data[reg_count],1))  // read all gyro values in Gyro_Calib_data 
						   {
						    if(reg_count>=5)
                            {
                                st_IMU=IMU_CALIB_CALC;
                                reg_count=0;
                            }
                            else
                            {
                              reg_count=reg_count+1;
                            }
					       }
						   break;
		case IMU_CALIB_CALC: 
							// read 32 gyro readings 
						   // add them all and find average
		                                  
							if(stI2CMgr==IDLE)	 // if reading data is completed by I2C
                            {
								InBuffer[0]=(Gyro_Read_Data[1]<<8)|Gyro_Read_Data[0];
								InBuffer[1]=(Gyro_Read_Data[3]<<8)|Gyro_Read_Data[2];
								InBuffer[2]=(Gyro_Read_Data[5]<<8)|Gyro_Read_Data[4];
								
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
								  calc_count++;
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
        		
        		st_IMU=IMU_PRES;
        		_oss = MODE_ULTRA_HIGHRES; // initialsie pressure sensor value
    		break;
	
	    case IMU_PRES: 
      
            	// BMP180_ReadCalibrationData
                if(I2C_READ(BMP180_ADDR,Pres_Reg[reg_count],&Pres_Read_data[reg_count*2],2))  // CAL_AC1 to MD see below
            	{
            	     if(reg_count>=10)
                     {
                        st_IMU=CALIB_PRES;
                        reg_count=0;
                     }
                     else
                     {
                        reg_count=reg_count+1;
                     }
                    
            	}
        	break; 
            
    case CALIB_PRES:
	if(stI2CMgr==IDLE)	
    {
							
          	AC1 = ((_SINT16)Pres_Read_data[0] <<8 | ((_SINT16)Pres_Read_data[1]));

          	AC2 = ((_SINT16)Pres_Read_data[2] <<8 | ((_SINT16)Pres_Read_data[3]));
          	
          	AC3 = ((_SINT16)Pres_Read_data[4] <<8 | ((_SINT16)Pres_Read_data[5]));
          	
          	AC4 = ((_UINT16)Pres_Read_data[6] <<8 | ((_UINT16)Pres_Read_data[7]));
         
          	AC5 = ((_UINT16)Pres_Read_data[8] <<8 | ((_UINT16)Pres_Read_data[9]));
          
          	AC6 = ((_UINT16)Pres_Read_data[10] <<8 | ((_UINT16)Pres_Read_data[11])); 

          	B1 = ((_SINT16)Pres_Read_data[12] <<8 | ((_SINT16)Pres_Read_data[13])); 

          	B2 = ((_SINT16)Pres_Read_data[14] <<8 | ((_SINT16)Pres_Read_data[15])); 
          	
          	MB = ((_SINT16)Pres_Read_data[16] <<8 | ((_SINT16)Pres_Read_data[17]));
         
          	MC = ((_SINT16)Pres_Read_data[18] <<8 | ((_SINT16)Pres_Read_data[19]));

          	MD = ((_SINT16)Pres_Read_data[20] <<8 | ((_SINT16)Pres_Read_data[21]));
            
	        st_IMU=SEL_TEMP;
	}
    break;
	
   case SEL_TEMP: 
				if(I2C_WRITE(BMP180_ADDR, &Alt_Calib_Data[0][0],2))  // CONTROL,READ_TEMPERATURE 
				{
						st_IMU=READ_TEMP;
				}
	break;
	case READ_TEMP: 
				
				if(I2C_READ(BMP180_ADDR,CONTROL_OUTPUT, &Alt_Read_Data[0][0],2))
				{
				   // read pressure
				   st_IMU=CALC_TEMP;
				}
					
				  break;
	case CALC_TEMP:
			    if(stI2CMgr==IDLE)	               
				   {
				     UT = ((_SINT32)Alt_Read_Data[0][0] << 8) + (_SINT32)Alt_Read_Data[0][1]; 
				     st_IMU=SEL_PRES;
				   }
				break;
    case SEL_PRES:
			    //BMP180_WriteReg(CONTROL, READ_PRESSURE + (_oss << 6)); 
				if(I2C_WRITE(BMP180_ADDR, &Alt_Calib_Data[1][0],2))  // CONTROL,READ_Pressure 
					{
							st_IMU=READ_PRES;
					}
					break;
			
	case READ_PRES:  
				if(I2C_READ(BMP180_ADDR, CONTROL_OUTPUT, &Alt_Read_Data[1][0],3))  // BMP180_ReadReg(CONTROL_OUTPUT, 3, &RegBuff[0]); 
				{
				   // read pressure
				   st_IMU=CALC_PRES;
				}
				break;
	case CALC_PRES : 
		        if(stI2CMgr==IDLE)	               	   
				{
				   UP = (((_SINT32)Alt_Read_Data[1][0] << 16) + ((_SINT32)Alt_Read_Data[1][1] << 8) + ((_SINT32)Alt_Read_Data[1][2])) >> (8 -_oss); // uncompensated pressure value
				   
                   BMP180_CalculateTruePressure(&PressureVal);
            	   BMP180_CalculateTrueTemperature(&TemperatureVal);
				   
                   if(calc_count<5)
                   {
                        
                        if(calc_count >= 2)
		                {
			                Sum += PressureVal;
		                }
                        calc_count=calc_count+1;
                        st_IMU=SEL_TEMP;
                   }
                   else
                   {
                    st_IMU=ALL_DONE;
                    calc_count=0;
                   }
                
                }
                break;
   case ALL_DONE:
            PressureVal= Sum / 3;               
        	Temp = (float)LOCAL_ADS_ALTITUDE / 4433000;
        	Temp = (float)pow((1 - Temp), 5.255f);
        	Pressure0 = (PressureVal - PRESSURE_OFFSET) / Temp;
            q0 = 1.0f;  
          	q1 = 0.0f;
          	q2 = 0.0f;
          	q3 = 0.0f;
            st_IMU=INIT_COMP;
            // TODO: remove it from task once initialisation is complete
            break;
    case INIT_COMP:
    case IDLE: 
    default:break;
}
}
// read from slave

void Read_From_ALL(void)
{
    
    // accelrometer
    static _UINT8 Acc_Read_Data[6]; // stores accelaration data received
    static _SINT16 accel[3]={0,0,0}; 
    // magnetometer
    static _UINT8 Mag_Read_Data[6]; // stores magcompas data received
    static _SINT16 magn[3];
    // pressure sens
    _SINT32 PVal,AVal, TVal;
    
    _SINT16 InBuffer[3] = {0,0,0};  // stores temperory gyro data
    
    
    static BMP180_AvgTypeDef BMP180_Filter[3];
	static _UINT8 reg_count; // stores tx reg counts 
    
    _UBYTE i;
			switch(st_Read)
			{
				case READ_GYRO: 
					if(I2C_READ(GYRO_ADDRESS,Gyro_Reg[reg_count],&Gyro_Read_Data[reg_count],1))  // read all gyro values in Gyro_Calib_data 
				    {
						    if(reg_count>=5)
                            {
                                st_Read=CALC_GYRO;
                                reg_count=0;
                            }
                            else
                            {
                              reg_count=reg_count+1;
                            }
					}
					break;
				case CALC_GYRO: 
					if(stI2CMgr==IDLE)	             
					   {
							InBuffer[0]=(Gyro_Read_Data[1]<<8)|Gyro_Read_Data[0];
							InBuffer[1]=(Gyro_Read_Data[3]<<8)|Gyro_Read_Data[2];
							InBuffer[2]=(Gyro_Read_Data[5]<<8)|Gyro_Read_Data[4];
					
							for( i = 0; i < 3; i ++)	
							{
								MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
							}
							gyro[0] = *(OutBuffer + 0) - MPU9250_Offset.X;
							gyro[1] = *(OutBuffer + 1) - MPU9250_Offset.Y;
							gyro[2] = *(OutBuffer + 2) - MPU9250_Offset.Z;
							st_Read=READ_ACC;
						}
					 break;
				case READ_ACC: 
					if(I2C_READ(ACCEL_ADDRESS,Acc_Reg[reg_count], &Acc_Read_Data[reg_count],1))  // read all gyro values in Acc_Read_Data 
					{
                         if(reg_count>=5)
                         {
                            st_Read=CALC_ACC;
                            reg_count=0;
                         }
                         else
                         {
                            reg_count=reg_count+1;
                         }   
						
					}
					break;
				case CALC_ACC: 
					if(stI2CMgr==IDLE)	             
					   {
							InBuffer[0]=(Acc_Read_Data[1]<<8)|Acc_Read_Data[0];
							InBuffer[1]=(Acc_Read_Data[3]<<8)|Acc_Read_Data[2];
							InBuffer[2]=(Acc_Read_Data[5]<<8)|Acc_Read_Data[4];
					
							for( i = 0; i < 3; i ++)	
						   {
							  MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
						   }
						   accel[0] = *(OutBuffer + 0);
						   accel[1] = *(OutBuffer + 1);
						   accel[2] = *(OutBuffer + 2); 
						   
                           st_Read=BYPASS_GYRO;
						}
					  break;
				case BYPASS_GYRO:   //  
					 
					  if(I2C_WRITE(GYRO_ADDRESS, &Gyro_Calib_Data[6][0],2))   //  GYRO_ADDRESS,0x37,0x02
					  {
							st_Read=START_MAG;  // swith to next case after 10 ms
					  }
					 break;
				case START_MAG:
					  if(I2C_WRITE(MAG_ADDRESS, &Mag_Calib_Data[0][0],2))   // read x axis mag
					  {
							st_Read=READ_MAG; 
					  }
					 break; //MAG_ADDRESS,0x0A,0x01
				case READ_MAG: 
					if(I2C_READ(MAG_ADDRESS,Mag_Reg[reg_count], &Mag_Read_Data[reg_count], 1))  // read all gyro values in Mag_Read_Data 
					{
						if(reg_count>=5)
                         {
                            st_Read=CALC_MAG;
                            reg_count=0;
                         }
                         else
                         {
                            reg_count=reg_count+1;
                         } 	
                   
					}
					break;
				case CALC_MAG: 
					if(stI2CMgr==IDLE)	             
					   {
							InBuffer[1]=(Mag_Read_Data[1]<<8)|Mag_Read_Data[0];   // byte seq is differant for magneto
							InBuffer[0]=(Mag_Read_Data[3]<<8)|Mag_Read_Data[2];
							InBuffer[2]=-((Mag_Read_Data[5]<<8)|Mag_Read_Data[4]);
					
							 for( i = 0; i < 3; i ++)	
							{
								MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
							} 
							magn[0] = *(OutBuffer + 0)-MPU9250_Magn_Offset.X_Off_Err;
							magn[1] = *(OutBuffer + 1)-MPU9250_Magn_Offset.Y_Off_Err;
							magn[2] = *(OutBuffer + 2)-MPU9250_Magn_Offset.Z_Off_Err;
							st_Read=SEL_TEMP;
						}
					  break;
			   // altitube measurment sensor
			   case SEL_TEMP: 
					if(I2C_WRITE(BMP180_ADDR, &Alt_Calib_Data[0][0],2))  // CONTROL,READ_TEMPERATURE 
					{
							st_Read=READ_TEMP;
					}
					break;
				case READ_TEMP: 
					
					if(I2C_READ(BMP180_ADDR,CONTROL_OUTPUT, &Alt_Read_Data[0][0],2))
					{
					   // read pressure
					   st_Read=CALC_TEMP;
					}
						
					  break;
				case CALC_TEMP:
				       if(stI2CMgr==IDLE)	               
					   {
					     UT = ((_UINT32)Alt_Read_Data[0][0] << 8) + (_UINT32)Alt_Read_Data[0][1]; 
					     st_Read=SEL_PRES;
					   }
					   break;
				case SEL_PRES:
				                         //BMP180_WriteReg(CONTROL, READ_PRESSURE + (_oss << 6)); 
					if(I2C_WRITE(BMP180_ADDR, &Alt_Calib_Data[1][0],2))  // CONTROL,READ_TEMPERATURE 
						{
								st_Read=READ_PRES;
						}
						break;
				
				case READ_PRES:
				     
					if(I2C_READ(BMP180_ADDR, CONTROL_OUTPUT, &Alt_Read_Data[1][0],3))  // BMP180_ReadReg(CONTROL_OUTPUT, 3, &RegBuff[0]); 
					{
					   // read pressure
					   st_Read=CALC_PRES;
					}
					break;
				case CALC_PRES : 
			        if(stI2CMgr==IDLE)	               
					   
					{
					    UP = (((_UINT32)Alt_Read_Data[0] << 16) + ((_UINT32)Alt_Read_Data[1] << 8) + ((_UINT32)Alt_Read_Data[2])) >> (8 -_oss); // uncompensated pressure value
					    
                        BMP180_CalculateTruePressure(&PVal);
						BMP180_CalAvgValue(&BMP180_Filter[0].Index, BMP180_Filter[0].AvgBuffer, PVal - PRESSURE_OFFSET, &PressureVal);

						BMP180_CalculateAbsoluteAltitude(&AVal, PVal - PRESSURE_OFFSET);
						BMP180_CalAvgValue(&BMP180_Filter[1].Index, BMP180_Filter[1].AvgBuffer, AVal, &AltitudeVal);

						BMP180_CalculateTrueTemperature(&TVal);
						BMP180_CalAvgValue(&BMP180_Filter[2].Index, BMP180_Filter[2].AvgBuffer, TVal, &TemperatureVal);
						
						// do all angle measurments
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
						
						// calculate angles
						angles[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
						angles[2] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
						angles[0] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   
						
                        // repet the process
                        st_Read=READ_GYRO;

					}
	
  	
				break;
             case IDLE:
             default : break;      
			}						
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
void MPU9250_CalAvgValue(_UINT8 *pIndex, _SINT16 *pAvgBuffer, _SINT16 InVal, _SINT32 *pOutVal)
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
//_SINT16 accel[3], gyro[3];



/**
  * @brief  invSqrt
  * @param  
  * @retval 
  */

float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	
	//long i = *(long*)&y;                //get bits for floating value
	long i = (union { float f; long l; }) {y} .l;
    
    i = 0x5f3759df - (i >> 1);          //gives initial guss you
	
    //y = *(float*)&i;                    //convert bits back to float
	y = (union { long l; float f; }) {i} .f;
    y = y * (1.5f - (halfx * y * y));   //newtop step, repeating increases accuracy
	
	return y;
}


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
  * @brief  Calculate true temperature
  * @param  *pTrueTemperature: true temperature 
  * @retval None
  */
void BMP180_CalculateTrueTemperature(_SINT32 *pTrueTemperature)
{
	_SINT32 X1, X2;
	
  	X1 = ((UT - AC6) * AC5) >> 15;
  	X2 = (MC << 11) / (X1 + MD);
  	B5 = X1 + X2;
  	*pTrueTemperature = (B5 + 8) >> 4;
}
/**
  * @brief  Calculate true pressure
  * @param  *pTruePressure: true pressure
  * @retval None
  */
void BMP180_CalculateTruePressure(_SINT32 *pTruePressure)
{
	_SINT32 X1, X2, X3, B3, B6, P, Temp;
	_UINT32  B4, B7;
	
	B6 = B5 - 4000;             
	X1 = (B2* ((B6 * B6) >> 12)) >> 11;
	X2 = AC2 * B6 >> 11;
	X3 = X1 + X2;
	Temp = (((_SINT32)AC1 << 2) + X3) << _oss;
	B3 = (Temp + 2) >> 2;
	X1 = (AC3 * B6) >> 13;
	X2 = (B1 * (B6 * B6 >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = (AC4 * (_UINT32) (X3 + 32768)) >> 15;
	B7 = ((_UINT32)UP - B3) * (50000 >> _oss);
  	if(B7 < 0x80000000)
  	{
   		P = (B7 << 1) / B4;
  	}	
  	else
  	{
  		P = (B7 / B4) << 1;
  	}

	X1 = (P >> 8) * (P >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * P) >> 16;
	
  	*pTruePressure = P + ((X1 + X2 + 3791) >> 4);
}






/** 
  * @brief  Calculating absolute altitude
  * @param  *pAltitude: absolute altitude
  * @param  PressureVal: the pressure at the absolute altitude
  * @retval None
  */
void BMP180_CalculateAbsoluteAltitude(_SINT32 *pAltitude, _SINT32 PressureVal)
{
	*pAltitude = 4433000 * (1 - pow((PressureVal / (float)Pressure0), 0.1903)); 
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
void BMP180_CalAvgValue(_UINT8 *pIndex, _SINT32 *pAvgBuffer, _SINT32 InVal, _SINT32 *pOutVal)
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

// API
void IMU_Command(_UBYTE Command)
{
   switch(Command)
   {
     case IMU_CMD_INIT: 
     if(st_IMU==IDLE)
     {
        st_IMU=IMU_INIT;
     }
     break;  
     case IMU_CMD_START:
     if(st_Read==IDLE)
     {
         st_Read= READ_GYRO;
     }
     break;
     case IMU_CMD_STOP:
         st_Read= IDLE;
         st_IMU=IDLE;
     default:
     break;
    
   }
}
_UBYTE IMU_Read_St(void)
{
    return(st_IMU);
}
#endif
/* [] END OF FILE */
