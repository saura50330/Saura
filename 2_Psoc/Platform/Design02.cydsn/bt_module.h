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

// bt responce time is around 500ms 
//works on 9600 baud
// once paird LED stays on
// once paired with master any data sent on uart is trasperant to master
// hc-06 can act as slave only
#define BT_IDLE 0
#define BT_INI 1

const char *BtIniCmd[]={
                          "AT",   // check connecton responce is ok
                          "AT+NAMESaura",  // sts bt name as Saura
                          "AT+PIN1234",  // 1234 is pin 
                          ""
                         };
/* [] END OF FILE */
