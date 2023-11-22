#ifndef CLOCK_HEADER
#define CLOCK_HEADER

#include <stdint.h>
#include "SYSCTL.h"

#define CLOCK_PLL_80MHz 80

//#define STRELOAD	 0xFFFFFF		//	0x2FFFFF			//			0x00CB7355																// Reload used in SysTick_InterruptInit

//SYSTICK DEFINES - REGISTERS, Base address: 0xE000.E000
//-----------------------------------------------------
#define SYSTICK_STCTRL_R				(*((volatile unsigned long *)0xE000E010)) //Offset 0x010
#define SYSTICK_STRELOAD_R			(*((volatile unsigned long *)0xE000E014)) //Offset 0x014
#define SYSTICK_STCURRENT_R			(*((volatile unsigned long *)0xE000E018)) //Offset 0x018


class Clock
{
	public:
		Clock(uint8_t frequency);
	
	private:
		uint8_t SystemClock_MHz;
		uint32_t SYSTICK_STRELOAD;
		//float SystemClock_us;
		int TicksNumberForOne_ms;
				
};

#endif