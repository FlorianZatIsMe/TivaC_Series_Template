#include <stdlib.h>
#include <stdio.h>  // Diamond braces for sys lib: Standard I/O
#include "Clock.h"
#include "UART.h"
#include "GPIO.h"
#include "PWM.h"

#include <list>

//PORT F DEFINES - LED CONTROL
//----------------------------
#define RED_LED_R								(*((volatile unsigned long *)0x40025008))	//From DATA register - 0x02: ON
#define BLUE_LED_R							(*((volatile unsigned long *)0x40025010))	//From DATA register - 0x04: ON
#define GREEN_LED_R							(*((volatile unsigned long *)0x40025020))	//From DATA register - 0x08: ON
#define LEDS_R									(*((volatile unsigned long *)0x40025038))	//From DATA register - 0x02: RED ON, 0x04: BLUE ON, 0x08: GREEN ON

#define RED_LED_ON 							{LEDS_R 			= 0x02;}										// set PF1, clear PF2 and PF3
#define BLUE_LED_ON 						{LEDS_R 			= 0x04;}										// set PF2, clear PF1 and PF3
#define GREEN_LED_ON						{LEDS_R 			= 0x08;}										// set PF3, clear PF1 and PF2
#define PINK_LED_ON 						{LEDS_R 			= 0x06;}										// set PF1 and PF2, clear PF3
#define YELLOW_LED_ON						{LEDS_R 			= 0x0A;}										// set PF1 and PF3, clear PF2
#define SKY_LED_ON 							{LEDS_R 			= 0x0C;}										// set PF2 and PF3, clear PF1
#define WHITE_LED_ON 						{LEDS_R 			= 0x0E;}										// set PF1, PF2 and PF3
#define RED_LED_TOGGLE			 		{RED_LED_R   ^= 0x02;}										// toggle PF1
#define BLUE_LED_TOGGLE 				{BLUE_LED_R  ^= 0x04;}										// toggle PF2
#define GREEN_LED_TOGGLE			 	{GREEN_LED_R ^= 0x08;}										// toggle PF3
#define LEDS_OFF 								{LEDS_R       = 0x00;}										// clear PF1, PF2 and PF3

//PORT F DEFINES - SWITCHES CONTROL
//---------------------------------
#define PF0_R										(*((volatile unsigned long *)0x40025004))	//From DATA register - 0x01: UNPRESSED (SW2 of the LaunchPad)
#define PF4_R										(*((volatile unsigned long *)0x40025040))	//From DATA register - 0x10: UNPRESSED (SW1 of the LaunchPad)

Clock* myClock;
uint8_t systemClock;
UART* uart;
GPIODEFFULLPORT gpioDef;
GPIO* gpio;
PWM* pwm;



void InitializeGPIO()
{
	GPIO gpioTemp;
	gpio = &gpioTemp;
	
	std::array<GPIODEF, 5> gpioDefinitions = {{
	{
		&gpio->PORTF,
		0,																// uint32_t PinNumber;
		GPIODEF::IOFUNCTION::INPUT,		// enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
		0,																// uint32_t AltFunc;
		GPIODEF::IOPULL::PULLUP,			// enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
		GPIODEF::IOSTATE::DONT_CARE		// enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;		
  },
	{
		&gpio->PORTF,
		1,																// uint32_t PinNumber;
		GPIODEF::IOFUNCTION::OUTPUT,		// enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
		0,																// uint32_t AltFunc;
		GPIODEF::IOPULL::NONE,				// enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
		GPIODEF::IOSTATE::LOGIC_HIGH		// enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;		
  }, 
	{
		&gpio->PORTF,
		2,																// uint32_t PinNumber;
		GPIODEF::IOFUNCTION::OUTPUT,	// enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
		0,																// uint32_t AltFunc;
		GPIODEF::IOPULL::NONE,				// enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
		GPIODEF::IOSTATE::LOGIC_HIGH		// enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;		
  }, 
	{
		&gpio->PORTF,
		3,																// uint32_t PinNumber;
		GPIODEF::IOFUNCTION::OUTPUT,	// enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
		0,																// uint32_t AltFunc;
		GPIODEF::IOPULL::NONE,				// enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
		GPIODEF::IOSTATE::LOGIC_LOW		// enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;		
  }, 
	{
		&gpio->PORTF,
		4,																// uint32_t PinNumber;
		GPIODEF::IOFUNCTION::INPUT,	// enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
		0,																// uint32_t AltFunc;
		GPIODEF::IOPULL::PULLUP,			// enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
		GPIODEF::IOSTATE::DONT_CARE		// enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;		
	}
	}};
	
	for (auto gpioDef : gpioDefinitions) {
		printf("\nGPIO_Port: %x\n", gpioDef.GPIO_Port->ODR);
		printf("PinNumber: %d\n", gpioDef.PinNumber);
				
		gpio->Configure(&gpioDef);
	}
}

void InitializePWM()
{
	PWM pwmTemp(gpio);
	pwm = &pwmTemp;
	
	PWMDEF pwmDefinition = {
		PWMDEF::PWMMODULE::MODULE0,
		PWMDEF::PWMGENERATOR::PWM0,
		PWMDEF::PWM_CLOCK_SOURCE::CLOCK_DIVISOR,
		PWMDEF::PWM_CLOCK_DIVISOR::DIV8
	};
	
	pwm->ConfigureGenerator(&pwmDefinition);	
}


void Initialize()
{
	systemClock = CLOCK_PLL_80MHz;
	Clock clockTemp(systemClock);
	myClock = &clockTemp;
	
	UART uartTemp(115200, systemClock);
	uart = &uartTemp;

	InitializeGPIO();
	InitializePWM();
}


int main(void)
{	
	Initialize();
	
	while(1)
	{
		if(PF0_R == 0x01)
		{
			LEDS_OFF
		}
		else
		{
			BLUE_LED_ON
		}
		
		if(PF4_R == 0x10)
		{
			LEDS_OFF
		}
		else
		{
			GREEN_LED_ON
		}
	}
	
	return 0;
}

