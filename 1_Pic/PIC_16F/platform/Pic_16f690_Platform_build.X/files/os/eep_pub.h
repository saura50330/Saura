 #define NO_ERROR_CHECK 0
    #define CHECK_FOR_ERROR 1
//START EEP DRIVER
    //DRIVER
    inline void Eep_Ini (void);
    inline void Eep_Isr(void);
    void Eep_Manager (void);
    void EEP_WRITE(_UBYTE Index, _UBYTE Data);
    _UBYTE  EEP_READ(_UBYTE Index);

    //API
    inline void EEP_to_RAM(_UBYTE eep_error_stetegy);  //  copy eep data  to RAM
   
//END
// START System USAGE TIMER
    //DRIVER
    inline void usage_time_ini(void); // call by ini
    void usege_timer(void); // called evey 1S interal divided by 20 , timer is 20sec resolution
    // API
    void Api_usege_timer_start_stop(_UBYTE start_stop_tmr); // called by user to start stop timer
//END