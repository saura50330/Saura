#include "sevlb_priv.h"
#include "sevlb_pub.h"


/*below is the function library for code*/

// bit functions

_UBYTE clear_bit_position(_UBYTE number1,_UBYTE bit_position)
{
	return(number1 & (~(0x01<<bit_position)));
}
_UBYTE set_bit_position(_UBYTE number1,_UBYTE bit_position)
{
	return( number1 | (0x01<<bit_position) );
}
_UBYTE update_bit_position(_UBYTE number1,_UBYTE bit_position,_UBYTE bit_value)
{
    return(bit_value?( number1 | (0x01<<bit_position) ):(number1 & (~(0x01<<bit_position))));
}
_UBYTE get_bit_value(_UBYTE number1,_UBYTE bit_position)
{
	 return((number1>>bit_position) & 0x01);
}
//--------compare function
// compares if temp is in between two values or not
_UBYTE srvb_between(_SINT32 temp, _SINT32 num1, _SINT32 num2)
{
   return(((temp>num1)&&(temp<num2))||((temp>num2)&&(temp<num1)));
}
/*STRING FUNCTIONS*/
void num_to_string(_UBYTE number)
{
 static _UBYTE num_string[4]={'0','0','0',0};
 _UBYTE count ,wrt_spac;
 wrt_spac=1;
 count = 2;
 
        while (number>0)
        {
            num_string[count]=(number%10)+48;
            number/=10;
            if(count>0)
	        {
				count--;
			}
            else
			{
				wrt_spac=0;
				break;
            }
	    }
        while(wrt_spac&&(count!=0))
        {
		           num_string[count]='0';
                   count--;
        }
		num_string[0]='0';
   }
   
 /*MATH LIBRARY*/
 _SINT8 srvb_add_s8_s16_s16(_SINT16 add1,_SINT16 add2)
 {
	_SINT16 temp=0; 
	temp= add1 + add2;
	if(srvb_between(temp,MAX_SINT8,MAX_SINT16))   // max limiter
	{
	 return(MAX_SINT8);
	}
	else if(srvb_between(temp,MIN_SINT8,MIN_SINT16))  // min limiter
	{
	 return(MIN_SINT8);
	}
	else                                           // within limit
	{
	  return ((_SINT8)temp);
	}
 
 }
 
 _SINT16 srvb_add_s16_s16_s16(_SINT16 add1,_SINT16 add2)
 {
	_SINT16 temp=0; 
	temp= add1 + add2;
	return ((_SINT16)temp);
 
 }
 
 _SINT8 srvb_sub_s8_s16_s16(_SINT16 sub1,_SINT16 sub2)
 {
	_SINT16 temp=0; 
	temp= sub1 - sub2;
	if(srvb_between(temp,MAX_SINT8,MAX_SINT16))   // max limiter
	{
	 return(MAX_SINT8);
	}
	else if(srvb_between(temp,MIN_SINT8,MIN_SINT16))  // min limiter
	{
	 return(MIN_SINT8);
	}
	else                                           // within limit
	{
	  return ((_SINT8)temp);
	}
 
 }
 
 
 
_UINT16 srvb_mul_div_u16_u16_u16_u16(_UINT32 mul1 ,_UINT32 mul2 ,_UINT16 div1)
{
    _SINT32 temp=0;
	if(div1)
    {
		temp=(mul1*mul2)/div1;
		return(((temp>MAX_UINT16)?MAX_UINT16:(_UINT16)temp));
    }
    else
    {
        return(0);
    }
}
_UINT8 srvb_mul_div_u8_u16_u16_u16(_UINT32 mul1 ,_UINT32 mul2 ,_UINT16 div1)
{
    _SINT32 temp=0;
	if(div1)
    {
		temp=(mul1*mul2)/div1;
		return(((temp>MAX_UINT8)?MAX_UINT8:(_UINT8)temp));
    }
    else
    {
        return(0);
    }
}

_SINT16 srvb_mul_div_s16_s16_s16_s16(_SINT32 mul1 ,_SINT32 mul2 ,_SINT16 div1)
{
    _SINT32 temp=0;
	if(div1)
    {
		temp=(mul1*mul2)/div1;
		if(srvb_between(temp,MAX_SINT16,MAX_SINT32))   // max limiter
		{
		 return(MAX_SINT16);
		}
		else if(srvb_between(temp,MIN_SINT16,MIN_SINT32))  // min limiter
		{
		 return(MIN_SINT16);
		}
		else                                           // within limit
		{
		  return ((_SINT16)temp);
		}
    }
    else
    {
        return(0);
    }
}
_SINT8 srvb_mul_div_s8_s16_s16_s16(_UINT32 mul1 ,_UINT32 mul2 ,_UINT16 div1)
{
    _SINT32 temp=0;
	if(div1)
    {
		temp=(mul1*mul2)/div1;
		if(srvb_between(temp,MAX_SINT8,MAX_SINT32))   // max limiter
		{
		 return(MAX_SINT8);
		}
		else if(srvb_between(temp,MIN_SINT8,MIN_SINT32))  // min limiter
		{
		 return(MIN_SINT8);
		}
		else                                           // within limit
		{
		  return ((_SINT8)temp);
		}
    }
    else
    {
        return(0);
    }
}

// this function is hysteresis function
_UBYTE hyst_delta_minus_s10s10s10u8(_SINT16 var_compare,_SINT16 var_max,_SINT16 var_min, _UBYTE * state_var)
{

	// if state_var value is zero then it will become one only after var_compare > var_max
	// if state_var value is one  then it will become zero only after var_compare < var_min
	// state_var holds lat valid value in betwwen var_compare is in between var_max & var_min
	if(*state_var) // if state variable is true
	{
	  if(var_compare > var_max)
	  {
		*state_var=0;
	  }
	  else
	  {
	    // hold last value
	  }
	}
	else
	{
	   if(var_compare < var_min)
	  {
		*state_var=1;
	  }
	  else
	  {
	    // hold last value
	  }

	}


		return (* state_var);
}
void delay_ms(_UINT16 delay_count)  // waits for delay_count ms
{                                   // function is writen for 8MHz clock , if clock changes 
_UBYTE i;
	while(delay_count)
	 {
		 for(i=0;i<ONE_MS_INSTRUCTION_DELAY;i++);   // this instuction shuld give one ms delay , at 8MHZ ONE_MS_INSTRUCTION_DELAY value is approximatlt 2000 to get 1ms dely

                 delay_count=delay_count-1;
	 }

}
