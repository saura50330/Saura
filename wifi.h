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
#define WIFI_IDLE 0
#define WFI_INI 1

const char *WifiIniCmd[]={
                          "AT\r\n",   // check connecton responce is ok
                          "AT+CWMODE=3\r\n",  // Access point request, rep =0
                          "AT+RST\r\n",  // reset the card 
                          "AT+CWJAP=""NETGEAR"",""strongviolet962\r\n", // connect to rauter
                          "AT+CIPMUX=1\r\n", // connect to internet
                          "AT+CIPSTART=4,""TCP"",""google.com"",80\r\n",  // run website
                          ""
                         };

/* [] END OF FILE */
