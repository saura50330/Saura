#define TIMER8_1_MS 135 //135   // tmer 0 vaue to generate 1 ms interuupt at 8Mhz internal clock
#define TMRER16_10_MS 0xB1DF  // tmer 1 vaue to generate 10 ms interuupt at 8Mhz internal clock
#define TMRER8_0_1_MS 242  // tmer 2 vaue to generate 0.1 ms interuupt at 8Mhz internal clock

typedef void (*tFuncPtr)(void); // tFuncPtr is pointer to function which can have void as return type and no parameters passed
//typedef tFuncPtr* TaskFuncPtr; //TaskFuncPtr is a pointer which stores address of function pointer 

inline void Os_Ini(void);
void Sw_Timer_Updator(void);
void Kernel_vRunContainer(const tFuncPtr *afpContainerArray);
inline void Kernel_Co_Op_Timer_ISR(void);
inline void Kernel_Pre_Emptiv_Timer_ISR(void);
