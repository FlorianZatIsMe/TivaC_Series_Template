/*

#include "Clock.h"

uint8_t CLOCK_SystemClock_MHz;
uint32_t CLOCK_SYSTICK_STRELOAD;
float CLOCK_SystemClock_us;
int CLOCK_TicksNumberForOne_ms;


//------------PLL------------
// Set the uC clock using the PLL
// 
// Input: none
// Output: none
void CLOCK_PLL_Init(uint8_t frequency){
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
	
	CLOCK_SystemClock_MHz = frequency;
	CLOCK_SystemClock_us = (float)(1 / CLOCK_SystemClock_MHz); // 0.0125 us for CLOCK_SystemClock_MHz = 80
	CLOCK_TicksNumberForOne_ms = 1000 * CLOCK_SystemClock_MHz;
}


int CLOCK_GetSystemClock() { return CLOCK_SystemClock_MHz; }

//------------SYSTICK------------
// Initialize the Timer SYSTICK with interrupt
// 
// Input: TicksNumber on 24 bits
// Output: none
void CLOCK_SysTickInitFromTicks(uint32_t TicksNumber, uint8_t withInterrupt)
{
//	printf("\nSTART OF SysTick_InterruptInit()\n");
//	printf("--------------------------------\n");
	
	CLOCK_SYSTICK_STRELOAD = TicksNumber;
	
	SYSTICK_STCTRL_R 		= (unsigned long)0x0;								// Register STCTRL: Disables SysTick
	SYSTICK_STRELOAD_R	= CLOCK_SYSTICK_STRELOAD;						// Register STRELOAD: Update the timer value (max here)
	SYSTICK_STCURRENT_R	= CLOCK_SYSTICK_STRELOAD;						// Register STCURRENT: Set the current timer
	NVIC_PRI3_R = (NVIC_PRI3_R&0x0FFFFFFF) | (0x7<<29);			// Set interrupt priority to PRI_SYSTICK
	SYSTICK_STCTRL_R 		= (unsigned long)										// Register STCTRL
											( (0x0 << 0)												// ENABLE = 0x1: Enable clock
											+ (withInterrupt ? 0x1 << 1 : 0)	  // INTEN = 0x1 if withInterrupt: with interrupt if configured
											+ (0x1 << 2));											// CLK_SRC = 0x1: using the system clock
	
//	printf("END OF SysTick_InterruptInit()\n");
//	printf("------------------------------\n");
}

void CLOCK_SysTickInitFromFrequency(uint16_t frequency_kHz, uint8_t withInterrupt)
{ 
	CLOCK_SysTickInitFromTicks(
		(uint32_t)(1000 * CLOCK_SystemClock_MHz / frequency_kHz), 
		withInterrupt); 
}

void CLOCK_SysTickEnable(uint8_t enable)
{	
	SYSTICK_STCTRL_R 		&=~(unsigned long)						// Register STCTRL
											  ( (0x1 << 0));							// ENABLE = 0x0: Disable clock	
	if(enable) {
		SYSTICK_STCTRL_R 		|= (unsigned long)						// Register STCTRL
													 (0x1 << 0);  							// ENABLE = 1x0: Enable clock
	}
}

uint32_t CLOCK_GetCLOCK_SYSTICK_STRELOAD() { return CLOCK_SYSTICK_STRELOAD; }

uint32_t CLOCK_GetSYSTICK_CurrentTimer()
{
	return SYSTICK_STCURRENT_R;
}



void CLOCK_SysTick_InterruptInit(void)
{
//	printf("\nSTART OF SysTick_InterruptInit()\n");
//	printf("--------------------------------\n");
	
	SYSTICK_STCTRL_R 		= 0x00000000;															// Disables SysTick
	SYSTICK_STRELOAD_R	= 0x00FFFFFF;																// Update the timer value (max here)
	SYSTICK_STCURRENT_R	= 0x00FFFFFF;																// Set the current timer
	NVIC_PRI3_R = (NVIC_PRI3_R&0x0FFFFFFF) | (0x03<<29);		// Set interrupt priority to PRI_SYSTICK
	SYSTICK_STCTRL_R 		= 0x00000007;															// Enable clock with interrupt
	
//	printf("END OF SysTick_InterruptInit()\n");
//	printf("------------------------------\n");
}


//------------DELAY------------
// Delay function expressed ticks
// 
// Input: delay in tick
// Output: none
void CLOCK_delay_ticks(unsigned long nticks)
{
	unsigned long start;	
	unsigned long end;
	
	SYSTICK_STCTRL_R 		|= (unsigned long)						// Register STCTRL
												 (0x1 << 0);  							// ENABLE = 1x0: Enable clock

	start = SYSTICK_STCURRENT_R;
	SYSTICK_STCTRL_R &= (unsigned long)~(0x1<<16);
	
	if(start >= nticks)
	{
		end = start - nticks;
		while( (SYSTICK_STCURRENT_R > end) && !(SYSTICK_STCTRL_R&(0x1<<16)) );
	}
	else
	{
		end = CLOCK_SYSTICK_STRELOAD - nticks + start;
		while(!(SYSTICK_STCTRL_R & (0x1 << 16) ));
		SYSTICK_STCTRL_R &= (unsigned long)~(0x1<<16);
		while( (SYSTICK_STCURRENT_R > end) && !(SYSTICK_STCTRL_R&(0x1<<16)) );
	}
}

//------------DELAY------------
// Delay function expressed in ms using SysTick
// 
// Input: delay in ms
// Output: 0 if correct input 1 otherwise
void CLOCK_delay_ms(unsigned long nms)
{
	unsigned long i, j;
	for(i=0;i<nms;i++)
	{
		for(j=0;j<CLOCK_SystemClock_MHz;j++)
		{
			CLOCK_delay_ticks(1000); // 1ms / clock period (ns)
		}
	}
	
	//
	
	
	//for(i=0;i<nms;i++) delay_ticks(CLOCK_TicksNumberForOne_ms); // 1ms / clock period (ns)
}


*/

