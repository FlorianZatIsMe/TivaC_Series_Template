#include <stdint.h>
#include "NVIC.h"
#include "SYSCTL.h"
//#include "GPIO.h"
//#include "UART.h"

#define CLOCK_PLL_80MHz 80

//#define STRELOAD	 0xFFFFFF		//	0x2FFFFF			//			0x00CB7355																// Reload used in SysTick_InterruptInit

//SYSTICK DEFINES - REGISTERS, Base address: 0xE000.E000
//-----------------------------------------------------
#define SYSTICK_STCTRL_R				(*((volatile unsigned long *)0xE000E010)) //Offset 0x010
#define SYSTICK_STRELOAD_R			(*((volatile unsigned long *)0xE000E014)) //Offset 0x014
#define SYSTICK_STCURRENT_R			(*((volatile unsigned long *)0xE000E018)) //Offset 0x018

void CLOCK_PLL_Init(uint8_t frequency);
int CLOCK_GetSystemClock();

//------------SYSTICK------------
// Initialize the Timer SYSTICK with interrupt
// 
// Input: none
// Output: none

void CLOCK_SysTick_InterruptInit(void);

void CLOCK_SysTickInitFromFrequency(uint16_t frequency_kHz, uint8_t withInterrupt);
void CLOCK_SysTickInitFromTicks(uint32_t TicksNumber, uint8_t withInterrupt);
void CLOCK_SysTickEnable(uint8_t enable);

uint32_t CLOCK_GetCLOCK_SYSTICK_STRELOAD();
uint32_t CLOCK_GetSYSTICK_CurrentTimer();



//------------DELAY------------
// Delay function expressed ticks
// 
// Input: delay in tick
// Output: none
void CLOCK_delay_ticks(unsigned long nticks);

//------------DELAY------------
// Delay function expressed in ms using SysTick
// 
// Input: delay in ms
// Output: none
void CLOCK_delay_ms(unsigned long nms);