#include "Clock.h"

Clock::Clock(uint8_t frequency)
{
	uint8_t SYSDIV2;
	uint8_t SYSDIV2LSB;
	
	if(frequency == CLOCK_PLL_80MHz) {
		SYSDIV2 = 0x02;
		SYSDIV2LSB = 0x0;
	}
	
  SYSCTL_RCC2_R |= (unsigned long)  0x1 << 31;										// Register RCC2 - USERCC2 = 0x1: The RCC2 register fields override the RCC register fields
  SYSCTL_RCC2_R |= (unsigned long)  0x1 << 11;  									// Register RCC2 - BYPASS2 = 0x1: The system clock is derived from the OSC source and divided by the divisor specified by SYSDIV2.
  SYSCTL_RCC2_R &= (unsigned long)~(0x7 << 4);  									// Register RCC2 - OSCSRC2 = 0x0: MOSC (main oscillator) configure for main oscillator source
  SYSCTL_RCC2_R &= (unsigned long)~(0x1 << 13); 									// Register RCC2 - PWRDN2  = 0x1: The PLL operates normally
  SYSCTL_RCC2_R |= (unsigned long)  0x1 << 30; 										// Register RCC2 - DIV400  = 0x1: Append the SYSDIV2LSB bit to the SYSDIV2 field to create a 7 bit divisor using the 400 MHz PLL output, see Table 5-6 on page 224 (use 400 MHz PLL)
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R & (unsigned long)~(0x7F << 22)) 	// Register RCC2: clears SYSDIV2 and SYSDIV2LSB
								+ (unsigned long) (SYSDIV2<<22)										// Register RCC2 - SYSDIV2:
								+ (unsigned long) (SYSDIV2LSB<<21);     			 		// Register RCC2 - SYSDIV2LSB:
								
  while((SYSCTL_RIS_R & (0x1 << 6))==0);  												// Register RIS - PLLLRIS: wait for the PLL to lock by polling PLLLRIS								
  SYSCTL_RCC2_R &= (unsigned long)~(0x1 << 11);										// Register RCC2 - BYPASS2 = 0x0: The system clock is the PLL output clock divided by the divisor specified by SYSDIV2
	
	SystemClock_MHz = frequency;
	//SystemClock_us = (float)(1 / CLOCK_SystemClock_MHz); // 0.0125 us for CLOCK_SystemClock_MHz = 80
	TicksNumberForOne_ms = 1000 * SystemClock_MHz;
}
