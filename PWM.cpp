#include "PWM.h"

PWM::PWM(GPIO* gpio)
{
	_gpio = gpio;
	
	_pwmPins[MODULE_0][PWM_0] = { _gpio->PORTB, 6, 4 };
	_pwmPins[MODULE_0][PWM_1] = { _gpio->PORTB, 7, 4 };
	_pwmPins[MODULE_0][PWM_2] = { _gpio->PORTB, 4, 4 };
	_pwmPins[MODULE_0][PWM_3] = { _gpio->PORTB, 5, 4 };
	_pwmPins[MODULE_0][PWM_4] = { _gpio->PORTE, 4, 4 };
	_pwmPins[MODULE_0][PWM_5] = { _gpio->PORTE, 5, 4 };
	_pwmPins[MODULE_0][PWM_6] = { _gpio->PORTC, 4, 4 };
	_pwmPins[MODULE_0][PWM_7] = { _gpio->PORTC, 5, 4 };
	
	_pwmPins[MODULE_1][PWM_0] = { _gpio->PORTD, 0, 5 };
	_pwmPins[MODULE_1][PWM_1] = { _gpio->PORTD, 1, 5 };
	_pwmPins[MODULE_1][PWM_2] = { _gpio->PORTA, 6, 5 };
	_pwmPins[MODULE_1][PWM_3] = { _gpio->PORTA, 7, 5 };
	_pwmPins[MODULE_1][PWM_4] = { _gpio->PORTF, 0, 5 };
	_pwmPins[MODULE_1][PWM_5] = { _gpio->PORTF, 1, 5 };
	_pwmPins[MODULE_1][PWM_6] = { _gpio->PORTF, 2, 5 };
	_pwmPins[MODULE_1][PWM_7] = { _gpio->PORTF, 3, 5 };
}

bool PWM::ConfigureGenerator(const PWMDEF* def)
{
	GPIO_TypeDef GPIO_Port = _pwmPins[def->Module][def->Generator].Port;
	uint32_t pinNumber = _pwmPins[def->Module][def->Generator].PinNumber;
	uint32_t altFunction = _pwmPins[def->Module][def->Generator].AltFunction;
	
	// GPIO Configuration
	// ------------------	
	GPIODEF gpioDefinitions = {
		&GPIO_Port,
		pinNumber,																// uint32_t PinNumber;
		GPIODEF::IOFUNCTION::ALT,									// enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
		altFunction,															// uint32_t AltFunc;
		GPIODEF::IOPULL::NONE,										// enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
		GPIODEF::IOSTATE::DONT_CARE								// enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;		
  };	
	_gpio->Configure(&gpioDefinitions);
	
	
	// PWM Configuration
	// -----------------
	SYSCTL_RCC_R					&=~(uint32_t)	(RCC_PWMDIV_Clear + RCC_USEPWMDIV_Clear);
	SYSCTL_RCC_R					|= def->RCC_USEPWMDIV + def->RCC_PWMDIV;
	
	/*
	
	
	PWM0_PWM0CTL_R				= (unsigned long)		// Register PWM0CTL
													(0x1 << 2);				// DEBUG = 0x1: The counter always runs when in Debug mode
	PWM0_PWM0LOAD_R				=  PWM0LOAD;						// Register PWM0LOAD: Set the load value (counter goes from load value to 0) = 12 pour les LED
	PWM0_PWM0CMPA_R				=  0;								// Register PWM0CMPA: PWM0 Compare A
	PWM0_PWM0CMPB_R				=  0;								// Register PWM0CMPB: PWM0 Compare B
	PWM0_PWM0GENA_R				= (unsigned long)		// Register PWM0GENA
												( (0x2 << 2)				// ACTLOAD = 0x3: Drive pwmA High when Counter = PWM0_PWM0LOAD_R (counter goes from LOAD to 0)
											  + (0x3 << 6));			// ACTCMPAD = 0x2: Drive pwmA Low when Counter = PWM0_PWM0CMPA_R
	PWM0_PWM0GENB_R				= (unsigned long)		// Register PWM0GENB
												( (0x2 << 2)				// ACTLOAD = 0x2: Drive pwmA High when Counter = PWM0_PWM0LOAD_R (counter goes from LOAD to 0)
											  + (0x3 << 10));			// ACTCMPBD = 0x3: Drive pwmA Low when Counter = PWM0_PWM0CMPB_R
	PWM0_PWM0CTL_R			 |=  0x1;							// Register PWM0CTL - ENABLE = 0x1 set: The PWM generation block is enabled and produces PWM signals
	
	// Interrupt	
	PWM0_PWMINTEN_R      |=  0x01;							// Register PWMINTEN - INTPWM0 = 0x1: Enable interrupt for PWM generator 0
	PWM0_PWM0INTEN_R      =  0x01 << 0;					// Register PWM0INTEN - INTCNTZERO = 0x1: Interrupt wwhen counter = 0
	PWM0_PWM0ISC_R			  =  0x01;							// Register PWM0ISC - INTCNTZERO = 0x1: Clears interrupt of PWM generator 0
	PWM0_PWMISC_R        |=  0x01;							// Register PWMISC - INTPWM0 Clears main interrupt
	
  NVIC_PRI2_R = (NVIC_PRI2_R&0xFFFFFFFF) | (PRI_PWM0GEN0 << 21); // Priority (PRI_PWM0GEN0) of interrupt 10 (PWM0 GEN0)
  NVIC_EN0_R |= 0x1 << 10;      								// Enable interrupt 10 in NVIC
	
	*/
	
	return true;
}


void PWM::InitializeGPIO()
{
	
}