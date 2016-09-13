#include "common.h"
#include "eep_config.h"
#include "eep.h"

/*EEP DRIVERS*/
_UBYTE EEPRAM_APP_DATA[EEP_APP_IDX]; // update application variables
_UBYTE EEPRAM_SYSTEM_DATA[EEP_SYS_IDX]; // update system variables
_UINT16 usage_time;
_UBYTE maintainace_status;
_UBYTE start_stop_tmr_val=0;
//ISR------------
 // EEPROM WRITE SUCCESS
inline void EEP_ISR(void)
{
   if(EEIF)
	{
		EEIF=0; // clear the fleg
	}
}

inline void EEP_INI(void)
 {
  //----------------EEP---------------------------------
    EEIE=1; // Enables the EE write complete interrupt
	PEIE=1;// enable periferal interrupt (used for timer2)
 }

/*EEP Driver*/
/*EEP_BIT_STRUCTURE*/
//EEP data in index  0 is always the EEP status
//-----------------------EEP  data in index  0  (to detect first initialisation)------------------------ 
// when index 1 value is not equal to 0xAA means EEP is not eye initialised ie its first start of application
// all the EEP configured indexes takes value from DEFAULT MECRO

// when index 1 value is  equal to 0xAA  means EEP is  initialised and data is valid
// EEP configured indexes retains the value

//----------------------EEP data index  1 ( to detect first abnormal shut down)------------------------------
// when system boots up, application writes to  index  1 value 0x00 and at he end of shut down writes to index 1 value 0xAA
// if application do not detect 0xAA at startup it meanse there was abnormal stut down previously

/*WRITE TO EEP FROM RAM*/
void RAM_to_EEP(void) /*EEp ram is witten into EEP cells*/
{
	_UBYTE i,checksum;
	_UBYTE j; // j holds the app variable index
        checksum=0;
   // eep appalication data write back, calibration ram is always stat att eep index 10
   for(i=EEP_APP_START_IDX,j=0;i<=EEP_APP_LAST_IDX;i++,j++)
   {
	 EEP_WRITE_2(i,EEPRAM_APP_DATA[j]);
	 checksum+=EEPRAM_APP_DATA[j];
   }
    for(i=EEP_SYS_START_IDX,j=0;i<=EEP_SYS_LAST_IDX;i++,j++)  // update system variables
   {
	  EEP_WRITE_2(i,EEPRAM_SYSTEM_DATA[j]);
	  checksum+=EEPRAM_SYSTEM_DATA[j];
   } 
   
  EEP_WRITE_2(0,0xAA); // data written to eeprom sucess fully
  EEP_WRITE_2(1,TPTAL_EEP_BYTES); // data written to eeprom sucess fully
  EEP_WRITE_2(2,SYSTEM_VER); // data written to eeprom sucess fully
  checksum+=(0xAA+TPTAL_EEP_BYTES + SYSTEM_VER);
  EEP_WRITE_2(EEP_CHECKSUM_INDEX,checksum); // write checksum to checksum index
   // checksum algorithm 
  // add all eep variables do not include checksum variable and store it in checksum variable
  // 
}

/*WRITE TO RAM FROM EEP*/
inline void EEP_to_RAM(_UBYTE eep_error_stetegy) /*EEp ram is witten into EEP cells*/
{
	_UBYTE i,temp_data=0;
	_UBYTE j; // j holds the app variable index
	// temp_data=check_eep_checksum();  // commented due to less rom size
   if((temp_data==EEP_NO_ERR)||(eep_error_stetegy==0))
   {
	   // eep appalication data read back, calibration ram is always stat att eep index 10
	   for(_UBYTE i=EEP_APP_START_IDX,j=0;i<=EEP_APP_LAST_IDX;i++,j++)
	   {
		 EEPRAM_APP_DATA[j]=EEP_READ(i);
	   }
	   for(i=EEP_SYS_START_IDX,j=0;i<=EEP_SYS_LAST_IDX;i++,j++)  // update system variables
	   {
		  EEPRAM_SYSTEM_DATA[j]=EEP_READ(i);
	   } 
   }
   else
   {
			// eep error check the eep error table and take decition
   }
  
   
}


void EEP_WRITE(_UBYTE Index, _UBYTE Data)
{
	/*_UBYTE temp_data;
         EEADR=Index;
	 RD=1;
	 temp_data=EEDAT;*/

     if((EEP_READ(Index))!=Data) // condition for writing into eeprom
	 { 
		GIE=0;   // EEP wrie can effect os timings
		EEADR=Index;
		EEDAT=Data;
		WREN=1; // enable write
		EECON2=0x55;  //EEP write sequence
		EECON2=0xAA;
		WR=1;
		WREN=0; // disable write
		while(EEIF==0); // wait till write cycle complets
		EEIF=0;
		GIE=1;
	 }
}
void EEP_WRITE_2(_UBYTE Index, _UBYTE Data)
{
	/*_UBYTE temp_data;
         EEADR=Index;
	 RD=1;
	 temp_data=EEDAT;*/

     if((EEP_READ(Index))!=Data) // condition for writing into eeprom
	 {
		//GIE=0;   // EEP wrie can effect os timings
		EEADR=Index;
		EEDAT=Data;
		WREN=1; // enable write
		EECON2=0x55;  //EEP write sequence
		EECON2=0xAA;
		WR=1;
		WREN=0; // disable write
		while(EEIF==0); // wait till write cycle complets
		EEIF=0;
		//GIE=1;
	 }
}
_UBYTE  EEP_READ(_UBYTE Index)
{
	//_UBYTE temp_data;
        EEADR=Index;
	RD=1;
	//temp_data=EEDAT;
	return(EEDAT);
}

_UBYTE check_eep_checksum(void) // excepts TPTAL_EEP_BYTES,EEP_CHECKSUM_INDEX
{
  _UBYTE checksum_data;
  checksum_data=0;
  if(EEP_READ(EEP_CHECKSUM_INDEX)==FIRST_BOOT) // first boot
  {

      return(EEP_NO_ERR); // no error
  }
  else
  {
  for(_UBYTE i=0;i<=TPTAL_EEP_BYTES; i++)
  {  
	   if(i!=EEP_CHECKSUM_INDEX)
	   {
			checksum_data+=EEP_READ(i);
	   }
  }
  if(checksum_data==(EEP_READ(EEP_CHECKSUM_INDEX)))
  {
	if(EEP_READ(SYSTEM_SHUT_DOWN_IDX)==SYSTEM_SHUT_DOWN_OLD)
	{
		if(EEP_READ(SYSTEM_MAINTAINACE_IDX)!=SYSTEM_MAINTAINACE_OFF)
		{
		    return(SYSTEM_MAINTAINACE_ERR); // SYSTEM_MAINTAINACE_ERR:4 system should be given for maintenance
		}
		else
		{
			return(EEP_NO_ERR); // no error
		}
    }
	else
	{
		return(SYSTEM_SHUTDOWN_ERR); // SYSTEM_DIRECT_OFF:2 system was shutdown abnormaly previously
	}
	
  }
  else
  {
	return(SYSTEM_CHECKSUM_ERR); //SYSTEM_CHECKSUM_ERR:1  checksum error
  }
  // checksum check algorithm
  // add all eep variables do not include checksum variable and check if checksum variable is matching with checksum calculated
  //
  }
}


void usage_time_ini()
{
  
        _UINT16 maint_time;
	// make sure that EEP data is copyed to EEP RAM
	// check if maintainace time is elepsed
	usage_time=((((_UINT16)EEPRAM_SYSTEM_DATA[2])<<8)|EEPRAM_SYSTEM_DATA[1]);  // TODO : Change eep index as per system folder use # define EEP_INDEX
	/*check if maintainace time is arrived*/
	maint_time =((((_UINT16)EEPRAM_APP_DATA[1])<<8)|EEPRAM_APP_DATA[0]);

        if(usage_time<maint_time)
	    { // not elepsed
            maintainace_status=0;
        }
        else
        {
            maintainace_status=1;
        }

}

void usege_timer_start_stop(_UBYTE start_stop_tmr)
{
    
    if((start_stop_tmr_val==ON)&&(start_stop_tmr==OFF)) // if invertor is made off from on condition is cosiderd as eep update cycle or normal turn off
    {   
        RAM_to_EEP();

    }
    start_stop_tmr_val=start_stop_tmr;

}

void usege_timer(void)   // This function to be sheduled in 1sec task.
{
    // max 182 hrs with resolution of 10s /bit
    static _UBYTE usage_time_res_count=0;
    if(start_stop_tmr_val)
    {
			if(usage_time_res_count>=USAGE_TIME_RES)
			{
				usage_time_res_count=0;
				 if(maintainace_status==0)
				{
						if(usage_time<0xFFFF)
						{
							usage_time=usage_time+1;
							/*write back MSB data to EEP RAM*/
							EEPRAM_SYSTEM_DATA[2]=(usage_time>>8);
							/*write back LSB data*/
							EEPRAM_SYSTEM_DATA[1]=(usage_time&0x00FF);
						}
				}
				else
				{
					// system maintainence needed // TODO: UPDATE system status variable

				}
			}
			usage_time_res_count=usage_time_res_count+1;
    }
}
/*
EEPROM	
Solution 1a: Use something like this (and change '20' to the array size you want):

__eeprom unsigned char my_eeprom_values[20] =  
        {   0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20,
            0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30};**
But if you use the above then you do NOT know where compiler will place these in EEPROM. 				  
				  
				  
---------------------- Solution 1b:

const unsigned char abc1 @ 0x2100 = 0x11; // @ EEPROM addr = 0
    // ......
    const unsigned char abc2 @ 0x2110 = 0x22; // @ EEPROM addr = 17**
	
//-------bootloder
all memory related operation API are in memory.s
when bootloader starts it copys all application data at from location 0x940 and  resets the device to location 0x940 using Goto operation.*/