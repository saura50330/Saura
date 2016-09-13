#define MAX_LED 3 // maximum LED in system
#define MAX_SWITCH 3 // maximum switches in system
#define MAX_SWITCH_DEB_COUNT 3 // how much switch to be debaunce befor confirming the status

#define PORT_A 0
#define PORT_B 1
#define PORT_C 2

#define PIN0 0
#define PIN1 1
//-----------LED and SWITCH port pin configurations---------------------------------------
// switches have two parameters one is port and other is pin
const _UBYTE Switch_Matrix[MAX_SWITCH][2]={    //port    pin       // MAX_SWITCH
                                                   { PORT_A , PIN0 },   // switch 0
                                                   { PORT_B , PIN1 },   // switch 1
                                                   { PORT_C , PIN0 }    // switch 2
                                               };

// Led have two parameters one is port and other is pin
const _UBYTE Led_Matrix[MAX_LED][2]={    //port    pin       // MAX_SWITCH
                                                   { PORT_A , PIN1 },   // LED0
                                                   { PORT_B , PIN0 },   // LED1
                                                   { PORT_C , PIN1 }    // LED2
                                               };
typedef struct
{
    _UBYTE Led_u8_st_Led_Status_Comp[MAX_LED];
    _UBYTE Led_u8_st_Led_Status[MAX_LED];

     _UBYTE Switch_u8_st_Current_Status[MAX_SWITCH];
     _UBYTE Switch_u8_st_Stable_Status[MAX_SWITCH];
     _UBYTE Switch_u8_st_Deb_Count[MAX_SWITCH];
}Dio_Con;

_UBYTE Dio_Read(_UBYTE port_no,_UBYTE pin_no);  // to read dio pin or port  pin_no=0 to 7 , pin_no=255 = complete port
void Dio_Write(_UBYTE port_no,_UBYTE pin_no,_UBYTE bit_value );// to write dio pin or port  pin_no=0 to 7,pin_no=255 = complete port

