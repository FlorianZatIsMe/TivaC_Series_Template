
//GENERAL DEFINES - REGISTERS
//---------------------------
#define SYSCTL_RIS_R          	(*((volatile unsigned long *)0x400FE050)) //
#define SYSCTL_RCC_R          	(*((volatile unsigned long *)0x400FE060)) //
#define SYSCTL_RCC2_R          	(*((volatile unsigned long *)0x400FE070)) //
#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100)) //Register to enable interface/function/module clock
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104)) //Register to enable interface/function/module clock
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108)) //Register to enable interface/function/module clock
#define SYSCTL_RCGCGPIO_R				(*((volatile unsigned long *)0x400FE608)) //TO TEST //Not used Register to enable GPIO clock
#define SYSCTL_RCGCQEI_R        (*((volatile unsigned long *)0x400FE644)) //TO TEST //Not used Register to enable QEI clock

//Gates control REGISTERS
//-----------------------------------------------------
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))