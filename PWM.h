#ifndef PWM_HEADER
#define PWM_HEADER

#include <stdint.h>
#include <stdio.h>
#include <list>

#include "GPIO.h"

#define MODULE_0 0
#define MODULE_1 1

#define PWM_0 0
#define PWM_1 1
#define PWM_2 2
#define PWM_3 3
#define PWM_4 4
#define PWM_5 5
#define PWM_6 6
#define PWM_7 7

#define RCC_PWMDIV_Clear 		(0x7 << 17)
#define RCC_USEPWMDIV_Clear	(0x1 << 20)

using PWMDEF = \
struct PWMDefStuct {
	enum PWMMODULE:uint32_t { 
		MODULE0 = MODULE_0,
		MODULE1 = MODULE_1 } Module;
	enum PWMGENERATOR:uint32_t { 
		PWM0 = PWM_0, 
		PWM1 = PWM_1, 
		PWM2 = PWM_2, 
		PWM3 = PWM_3, 
		PWM4 = PWM_4, 
		PWM5 = PWM_5, 
		PWM6 = PWM_6, 
		PWM7 = PWM_7,  } Generator;
	enum PWM_CLOCK_SOURCE:uint32_t { SYSTEM_CLOCK = 0x1 << 20, CLOCK_DIVISOR = 0x0 << 20 } RCC_USEPWMDIV;
	enum PWM_CLOCK_DIVISOR:uint32_t {
		DIV2 = 0x0 << 17,
		DIV4 = 0x1 << 17,
		DIV8 = 0x2 << 17,
		DIV16 = 0x3 << 17,
		DIV32 = 0x4 << 17,
		DIV34 = 0x5 << 17,
		DONT_CARE = 0xF,
	} RCC_PWMDIV;
};

using PINDEF = \
struct PINDefStuct {
	GPIO_TypeDef Port;
	uint32_t PinNumber;
	uint32_t AltFunction;
};


class PWM
{
	public:
		PWM(GPIO* gpio);
		bool ConfigureGenerator(const PWMDEF* def);
	
	private:
		void InitializeGPIO();
	
		GPIO* _gpio;
		//GPIODEFFULLPORT _gpioDef;
		PINDEF _pwmPins[MODULE_1+1][PWM_7+1];
};

#endif
