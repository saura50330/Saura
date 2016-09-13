// use below defination for API
// LED STATUS
#define LED_OFF 0
#define LED_BLINK 1
#define LED_ON 255
#define BLINK_500 2   // 200ms*2 ~ 500
#define BLINK_1000 5  // 200ms* 5  = 1000

//LED NUMBER MAX_LED 3
#define LED0 0
#define LED1 1
#define LED2 2

//LED NUMBER MAX_SWITCH 3
#define SWITCH0 0
#define SWITCH1 1
#define SWITCH2 2


//START DIO DRIVER
    //DRIVER
    inline void Dio_Ini(void);
    void Led_Manager(void); // call this every 200ms
    //Switch_Manager manages switch debounig delays and update debouce switch status
    void Switch_Manager(void);  // call this every 100ms

    //API
    void Api_Led_Control(_UBYTE led_no,_UBYTE led_status); //led_no = 0 to 7 , led_status= 0:OFF 1 =100ms blink ...255 =ON
    _UBYTE Api_Get_Switch_Status(_UBYTE switch_no); // read debaunced state of switch
//END