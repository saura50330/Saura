#include"type_def.h"
#define ONE_MS_INSTRUCTION_DELAY 224u  // actuly value shuld be 2000



_UBYTE clear_bit_position(_UBYTE number1,_UBYTE bit_position);
_UBYTE set_bit_position(_UBYTE number1,_UBYTE bit_position);
_UBYTE update_bit_position(_UBYTE number1,_UBYTE bit_position,_UBYTE bit_value);
_UBYTE get_bit_value(_UBYTE number1,_UBYTE bit_position);
_UBYTE srvb_between(_SINT32 temp, _SINT32 num1, _SINT32 num2);
void num_to_string(_UBYTE number);
void delay_ms(_UINT16 delay_count);
_SINT8 srvb_add_s8_s16_s16(_SINT16 add1,_SINT16 add2);
 _SINT16 srvb_add_s16_s16_s16(_SINT16 add1,_SINT16 add2);
_SINT8 srvb_sub_s8_s16_s16(_SINT16 sub1,_SINT16 sub2);

_UINT16 srvb_mul_div_u16_u16_u16_u16(_UINT32 mul1 ,_UINT32 mul2 ,_UINT16 div1);

_UINT8 srvb_mul_div_u8_u16_u16_u16(_UINT32 mul1 ,_UINT32 mul2 ,_UINT16 div1);

_SINT16 srvb_mul_div_s16_s16_s16_s16(_SINT32 mul1 ,_SINT32 mul2 ,_SINT16 div1);

_SINT8 srvb_mul_div_s8_s16_s16_s16(_UINT32 mul1 ,_UINT32 mul2 ,_UINT16 div1);
_UBYTE hyst_delta_minus_s10s10s10u8(_SINT16 var_compare,_SINT16 var_max,_SINT16 var_min, _UBYTE * state_var);