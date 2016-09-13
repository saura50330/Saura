#include "type_def.h"
#include  "app_process_pub.h"
#include "system_config.h"
#define USAGE_TIME_RES 10 //  system usage timer resolution 10seconds/bit 182 hrs bax for 1 bit variable
// below are EEPROM initialization values when system boot 1st time description is below

#define PIC12F83_EEP_START_ADD 0x2100//

#define EEP_SYS_IDX 6  // total no of  eep system variables are used
#define EEP_SYS_START_IDX 4 // eep sys start index
#define EEP_SYS_LAST_IDX (EEP_SYS_IDX+EEP_SYS_START_IDX-1) //  range (4 to 9)

#define EEP_APP_IDX 6   // total no of app EEP variable 
#define EEP_APP_START_IDX 10 // start index
#define EEP_APP_LAST_IDX (EEP_APP_START_IDX+EEP_APP_IDX-1) // stop index range (10 to 254)


#define TPTAL_EEP_BYTES (EEP_APP_START_IDX + EEP_APP_IDX)  // total eep bytes used const(0 to 3) + sys(4 to 9) + App(10 to 254)

#define EEP_CHECKSUM_INDEX 3
#define CHECKSUM 0xFF
#define FIRST_BOOT 0xFF
// do not change this values (first 4 bytes in EEP (0 to 3))
const unsigned char eep0 @ PIC12F83_EEP_START_ADD = FIRST_BOOT; // @ EEPROM addr = 0
const unsigned char eep1 @ (PIC12F83_EEP_START_ADD+1) = TPTAL_EEP_BYTES; //@ EEPROM addr = 1
const unsigned char eep2 @ (PIC12F83_EEP_START_ADD+2) = SYSTEM_VER;
const unsigned char eep3 @ (PIC12F83_EEP_START_ADD+3) = CHECKSUM;


// below variables can be changed by system application program but not for user scrachpad ( 4 to 9)


#define SYSTEM_MAINTAINACE_IDX EEP_SYS_START_IDX
#define SYSTEM_SHUT_DOWN_IDX 7
#define SYSTEM_MAINTAINACE_OFF 0
#define SYSTEM_SHUT_DOWN_OLD 0
#define SYSTEM_TIMER_0 0
#define SYSTEM_TIMER_1 0
#define RESERVED_EEP 0xFF
const unsigned char eep4 @ (PIC12F83_EEP_START_ADD+SYSTEM_MAINTAINACE_IDX) = SYSTEM_MAINTAINACE_OFF; // system status maintainace, fault,  error etc // RAM INDEX is 0
const unsigned char eep5 @ (PIC12F83_EEP_START_ADD+5) = SYSTEM_TIMER_0; // RAM index is 1
const unsigned char eep6 @ (PIC12F83_EEP_START_ADD+6) = SYSTEM_TIMER_1;  // RAM INDEX is 2
const unsigned char eep7 @ (PIC12F83_EEP_START_ADD+SYSTEM_SHUT_DOWN_IDX) = SYSTEM_SHUT_DOWN_OLD; // system was shut-down normally or not? 0xAA is proper shut down
const unsigned char eep8 @ (PIC12F83_EEP_START_ADD+8) = RESERVED_EEP;
const unsigned char eep10 @ (PIC12F83_EEP_START_ADD+9) = RESERVED_EEP;

// below is user scratched (10 to 254)


#define APP_EEP_DEF 0x00 // default value

const unsigned char abc11 @ (PIC12F83_EEP_START_ADD+10) = 0xFF; // usage timer comapre calibaration LSB
const unsigned char abc12 @ (PIC12F83_EEP_START_ADD+11) = 0xFF; // usage timer comapre calibaration MSB
const unsigned char abc13 @ (PIC12F83_EEP_START_ADD+12) = BATT_CHARGE_MAX; // 
const unsigned char abc14 @ (PIC12F83_EEP_START_ADD+13) = BATT_CHARGE_MIN; // 
const unsigned char abc15 @ (PIC12F83_EEP_START_ADD+14) = SOLAR_VOLT_NIGHT_TYM; //
const unsigned char abc16 @ (PIC12F83_EEP_START_ADD+15) = TYPE1; // backup at nigt and when powerfails
const unsigned char abc17 @ (PIC12F83_EEP_START_ADD+16) = 1; // enable charging when first start
//EEPROM ERRORS
#define EEP_NO_ERR 0
#define SYSTEM_CHECKSUM_ERR 1
#define SYSTEM_SHUTDOWN_ERR 2
#define SYSTEM_MAINTAINACE_ERR 3


//EEPROM MAPPING
// 1st byte is for EEPROM FLAGING 0xFF first INI, 0xAA alredy initialised
// 2nd BYTE TELLS ABOUT TOTAL DATA VALUES, total eeprom values to be cosiderd for checksum
// 3th byte system version
// 4ed byte eep checksum value (from 1st byte , total (2nd byte of eeprom))

// 5th byte system status (maintainance, mode etc)
// 6th byte system usage time 0
// 7th byte system usage time 1 
// 8th byte syatem shutdown satus
// 9th byte system reserved 0xFF
// 10th byte system reserved 0xFF
// 11th byte Application calibrations 
