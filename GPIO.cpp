#include "GPIO.h"

GPIO::GPIO()
{
	PORTA = { PORTA_BASE_ADDRESS };
	PORTB = { PORTB_BASE_ADDRESS };
	PORTC = { PORTC_BASE_ADDRESS };
	PORTD = { PORTD_BASE_ADDRESS };
	PORTE = { PORTE_BASE_ADDRESS };
	PORTF = { PORTF_BASE_ADDRESS };
}

bool GPIO::Configure(GPIODEFFULLPORT* def)
{
	uint32_t baseAddress = def->GPIO->ODR;
	
	GPIOCALC calc;	
	if(!CalculateRegistersValues(def, &calc)) return false;
	
	//PORT - STANDARD REGISTER CONFIGURATION
	//--------------------------------------
	volatile unsigned long delay;	
  SYSCTL_RCGC2_R     |=  0x1 << calc.bitNumberRCGC2;				// Register RCGC2: Enable PORT clock
  delay = SYSCTL_RCGC2_R;             											// delay, IDK, it's supposed to do something	
	REGISTER(baseAddress + GPIO_LOCK_Offset) = 0x4C4F434B;		// Register GPIOLOCK:  Unlock the register GPIOCR	
  //REGISTER(baseAddress + GPIO_CR_Offset) &= (uint32_t)~calc.GPIOCR_SetTo0;			// (MAYBE) Register GPIOCR: Avoid change on the registers GPIOAFSEL, GPIOPUR, GPIOPDR, and GPIODEN on the used pins
	REGISTER(baseAddress + GPIO_CR_Offset) |= calc.GPIOCR_SetTo1;										// Register GPIOCR: Allow change on the registers GPIOAFSEL, GPIOPUR, GPIOPDR, and GPIODEN on the used pins
		
	//PORT - SPECIFIC REGISTER CONFIGURATION
	//--------------------------------------
  REGISTER(baseAddress + GPIO_DEN_Offset) &= (uint32_t)~calc.GPIODEN_SetTo_0;			// Register GPIODEN - DIGITAL: Disable digital for the applicable pins
  REGISTER(baseAddress + GPIO_DEN_Offset) |= calc.GPIODEN_SetTo_1;								// Register GPIODEN - DIGITAL: Enable digital for the applicable pins
  REGISTER(baseAddress + GPIO_DIR_Offset) &= (uint32_t)~calc.GPIODIR_SetTo_0;			// Register GPIODIR - INPUT / OUTPUT: Set the applicable pins as inputs
  REGISTER(baseAddress + GPIO_DIR_Offset) |= calc.GPIODIR_SetTo_1;								// Register GPIODIR - INPUT / OUTPUT: Set the applicable pins as outputs
  REGISTER(baseAddress + GPIO_AMSEL_Offset) &= (uint32_t)~calc.GPIOAMSEL_SetTo_0;	// Register GPIOAMSEL - ANALOG: Disable analog function for the applicable pins
  REGISTER(baseAddress + GPIO_AMSEL_Offset) |= calc.GPIOAMSEL_SetTo_1;						// Register GPIOAMSEL - ANALOG: Enable analog function for the applicable pins
  REGISTER(baseAddress + GPIO_AFSEL_Offset) &= (uint32_t)~calc.GPIOAFSEL_SetTo_0;	// Register GPIOAFSEL - ALT: No alternate function for the applicable pins
  REGISTER(baseAddress + GPIO_AFSEL_Offset) |= calc.GPIOAFSEL_SetTo_1;						// Register GPIOAFSEL - ALT: Alternate function for the applicable pins
  REGISTER(baseAddress + GPIO_PCTL_Offset) &= (uint32_t)~calc.GPIOPCTL_SetTo_0;		// Register GPIOPCTL - ALT: Variable used to clear the slternate function register for the applicable pin
  REGISTER(baseAddress + GPIO_PCTL_Offset)  |= calc.GPIOPCTL_SetTo_1;							// Register GPIOPCTL - ALT: Sets the slternate function for the applicable pins
  REGISTER(baseAddress + GPIO_PUR_Offset) &= (uint32_t)~calc.GPIOPUR_SetTo_0;			// Register GPIOPUR - PULLUP: Disable pullup resistors on applicable pins
  REGISTER(baseAddress + GPIO_PUR_Offset) |= calc.GPIOPUR_SetTo_1;								// Register GPIOPUR - PULLUP: Enable pullup resistors on applicable pins
  REGISTER(baseAddress + GPIO_PDR_Offset) &= (uint32_t)~calc.GPIOPDR_SetTo_0;			// Register GPIOPDR - PULLDOWN: Disable pulldown resistors on applicable pins
  REGISTER(baseAddress + GPIO_PDR_Offset) |= calc.GPIOPDR_SetTo_1;								// Register GPIOPDR - PULLDOWN: Enable pulldown resistors on applicable pins	
	REGISTER(baseAddress + GPIO_DATA_Offset) &= (uint32_t)~calc.GPIODATA_SetTo_0;		// Register GPIODATA - INITIAL STATE set to 0 on applicable pins
  REGISTER(baseAddress + GPIO_DATA_Offset) |= calc.GPIODATA_SetTo_1;							// Register GPIODATA - INITIAL STATE set to 1 on applicable pins
	
	printf("Configure() - delay = SYSCTL_RCGC2_R = %ld\n\n",  delay);

	return true;
}

bool GPIO::Configure(GPIODEF* def)
{
	uint32_t baseAddress = def->GPIO_Port ->ODR;
	
	GPIOCALC calc;	
	if(!CalculateRegistersValues(def, &calc)) return false;
	
	//PORT - STANDARD REGISTER CONFIGURATION
	//--------------------------------------
	volatile unsigned long delay;	
  SYSCTL_RCGC2_R     |=  0x1 << calc.bitNumberRCGC2;				// Register RCGC2: Enable PORT clock
  delay = SYSCTL_RCGC2_R;             											// delay, IDK, it's supposed to do something	
	REGISTER(baseAddress + GPIO_LOCK_Offset) = 0x4C4F434B;		// Register GPIOLOCK:  Unlock the register GPIOCR	
  //REGISTER(baseAddress + GPIO_CR_Offset) &= (uint32_t)~calc.GPIOCR_SetTo0;			// (MAYBE) Register GPIOCR: Avoid change on the registers GPIOAFSEL, GPIOPUR, GPIOPDR, and GPIODEN on the used pins
	REGISTER(baseAddress + GPIO_CR_Offset) |= calc.GPIOCR_SetTo1;										// Register GPIOCR: Allow change on the registers GPIOAFSEL, GPIOPUR, GPIOPDR, and GPIODEN on the used pins
		
	//PORT - SPECIFIC REGISTER CONFIGURATION
	//--------------------------------------
  REGISTER(baseAddress + GPIO_DEN_Offset) &= (uint32_t)~calc.GPIODEN_SetTo_0;			// Register GPIODEN - DIGITAL: Disable digital for the applicable pins
  REGISTER(baseAddress + GPIO_DEN_Offset) |= calc.GPIODEN_SetTo_1;								// Register GPIODEN - DIGITAL: Enable digital for the applicable pins
  REGISTER(baseAddress + GPIO_DIR_Offset) &= (uint32_t)~calc.GPIODIR_SetTo_0;			// Register GPIODIR - INPUT / OUTPUT: Set the applicable pins as inputs
  REGISTER(baseAddress + GPIO_DIR_Offset) |= calc.GPIODIR_SetTo_1;								// Register GPIODIR - INPUT / OUTPUT: Set the applicable pins as outputs
  REGISTER(baseAddress + GPIO_AMSEL_Offset) &= (uint32_t)~calc.GPIOAMSEL_SetTo_0;	// Register GPIOAMSEL - ANALOG: Disable analog function for the applicable pins
  REGISTER(baseAddress + GPIO_AMSEL_Offset) |= calc.GPIOAMSEL_SetTo_1;						// Register GPIOAMSEL - ANALOG: Enable analog function for the applicable pins
  REGISTER(baseAddress + GPIO_AFSEL_Offset) &= (uint32_t)~calc.GPIOAFSEL_SetTo_0;	// Register GPIOAFSEL - ALT: No alternate function for the applicable pins
  REGISTER(baseAddress + GPIO_AFSEL_Offset) |= calc.GPIOAFSEL_SetTo_1;						// Register GPIOAFSEL - ALT: Alternate function for the applicable pins	
  REGISTER(baseAddress + GPIO_PCTL_Offset) &= (uint32_t)~calc.GPIOPCTL_SetTo_0;		// Register GPIOPCTL - ALT: Variable used to clear the slternate function register for the applicable pin
  REGISTER(baseAddress + GPIO_PCTL_Offset)  |= calc.GPIOPCTL_SetTo_1;							// Register GPIOPCTL - ALT: Sets the slternate function for the applicable pins	
  REGISTER(baseAddress + GPIO_PUR_Offset) &= (uint32_t)~calc.GPIOPUR_SetTo_0;			// Register GPIOPUR - PULLUP: Disable pullup resistors on applicable pins
  REGISTER(baseAddress + GPIO_PUR_Offset) |= calc.GPIOPUR_SetTo_1;								// Register GPIOPUR - PULLUP: Enable pullup resistors on applicable pins
  REGISTER(baseAddress + GPIO_PDR_Offset) &= (uint32_t)~calc.GPIOPDR_SetTo_0;			// Register GPIOPDR - PULLDOWN: Disable pulldown resistors on applicable pins
  REGISTER(baseAddress + GPIO_PDR_Offset) |= calc.GPIOPDR_SetTo_1;								// Register GPIOPDR - PULLDOWN: Enable pulldown resistors on applicable pins	
	REGISTER(baseAddress + GPIO_DATA_Offset) &= (uint32_t)~calc.GPIODATA_SetTo_0;		// Register GPIODATA - INITIAL STATE set to 0 on applicable pins
  REGISTER(baseAddress + GPIO_DATA_Offset) |= calc.GPIODATA_SetTo_1;							// Register GPIODATA - INITIAL STATE set to 1 on applicable pins
	
	//printf("Configure() - delay = SYSCTL_RCGC2_R = %ld\n\n",  delay);

	return true;
}

bool GPIO::CalculateRegistersValues(const GPIODEFFULLPORT* def, GPIOCALC* calc)
{
	if(def->GPIO == &PORTA) calc->bitNumberRCGC2 = 0;
	else if(def->GPIO == &PORTB) calc->bitNumberRCGC2 = 1;
	else if(def->GPIO == &PORTC) calc->bitNumberRCGC2 = 2;
	else if(def->GPIO == &PORTD) calc->bitNumberRCGC2 = 3;
	else if(def->GPIO == &PORTE) calc->bitNumberRCGC2 = 4;
	else if(def->GPIO == &PORTF) calc->bitNumberRCGC2 = 5;
	else
	{
		printf("bitNumberRCGC2 not set: %x\n", def->GPIO->ODR);
		return false;
	}	
	
	for (auto const& pin : *def->Pins) {		
		if(!pin.Used) continue;
		calc->GPIOCR_SetTo1 += 0x1 << pin.PinNumber;
		
		// Function: DIGITAL
		if(pin.Function == GPIO_PinDef::IOFUNCTION::INPUT || pin.Function == GPIO_PinDef::IOFUNCTION::OUTPUT)
		{
			calc->GPIODEN_SetTo_1 += 0x1 << pin.PinNumber;		// If the function is input or output, then the pin is digital (I think)
			
			if(pin.Function == GPIO_PinDef::IOFUNCTION::INPUT) 					calc->GPIODIR_SetTo_0 += 0x1 << pin.PinNumber;
			else if(pin.Function == GPIO_PinDef::IOFUNCTION::OUTPUT)		calc->GPIODIR_SetTo_1 += 0x1 << pin.PinNumber;
			else
			{
				printf("pin.Function incorrect\n");
				return false;
			}
		}
		else calc->GPIODEN_SetTo_0 += 0x1 << pin.PinNumber;
		
		// Function: ANALOG
		if(pin.Function == GPIO_PinDef::IOFUNCTION::ANALOG)		calc->GPIOAMSEL_SetTo_1 += 0x1 << pin.PinNumber;
		else 																									calc->GPIOAMSEL_SetTo_0 += 0x1 << pin.PinNumber;
		
		// Function: ALT
		if(pin.Function == GPIO_PinDef::IOFUNCTION::ALT)	{		calc->GPIOAFSEL_SetTo_1 += 0x1 << pin.PinNumber;										// Enable alternate function
																													calc->GPIOPCTL_SetTo_0 += GPIOPCTL_Clear << (4 * pin.PinNumber);		// Variable to clear the alternate function
																													calc->GPIOPCTL_SetTo_1 += pin.AltFunction << (4 * pin.PinNumber); }	// Specify which alternate function
		else																									calc->GPIOAFSEL_SetTo_0 += 0x1 << pin.PinNumber;
		
		// Bias: PULLUP
		if(pin.Bias == GPIO_PinDef::IOPULL::PULLUP)						calc->GPIOPUR_SetTo_1 += 0x1 << pin.PinNumber;
		else																									calc->GPIOPUR_SetTo_0 += 0x1 << pin.PinNumber;
		
		// Bias: PULLDOWN
		if(pin.Bias == GPIO_PinDef::IOPULL::PULLDOWN)					calc->GPIOPDR_SetTo_1 += 0x1 << pin.PinNumber;
		else																									calc->GPIOPDR_SetTo_0 += 0x1 << pin.PinNumber;
																													
		// InitialState
		if(pin.InitialState == GPIO_PinDef::IOSTATE::LOGIC_HIGH)			calc->GPIODATA_SetTo_1 += 0x1 << pin.PinNumber;
		else if(pin.InitialState == GPIO_PinDef::IOSTATE::LOGIC_LOW)	calc->GPIODATA_SetTo_0 += 0x1 << pin.PinNumber;
	}
	PrintCalcultatedRegistersValues(calc);
	return true;
}

bool GPIO::CalculateRegistersValues(const GPIODEF* def, GPIOCALC* calc)
{
	if(def->GPIO_Port == &PORTA) calc->bitNumberRCGC2 = 0;
	else if(def->GPIO_Port == &PORTB) calc->bitNumberRCGC2 = 1;
	else if(def->GPIO_Port == &PORTC) calc->bitNumberRCGC2 = 2;
	else if(def->GPIO_Port == &PORTD) calc->bitNumberRCGC2 = 3;
	else if(def->GPIO_Port == &PORTE) calc->bitNumberRCGC2 = 4;
	else if(def->GPIO_Port == &PORTF) calc->bitNumberRCGC2 = 5;
	else
	{
		printf("bitNumberRCGC2 not set %x\n", def->GPIO_Port ->ODR);
		return false;
	}
	
	calc->GPIOCR_SetTo1 += 0x1 << def->PinNumber;
	
	// Function: DIGITAL
	if(def->Function == GPIODEF::IOFUNCTION::INPUT || def->Function == GPIODEF::IOFUNCTION::OUTPUT)
	{
		calc->GPIODEN_SetTo_1 += 0x1 << def->PinNumber;		// If the function is input or output, then the pin is digital (I think)
		
		if(def->Function == GPIODEF::IOFUNCTION::INPUT) 					calc->GPIODIR_SetTo_0 += 0x1 << def->PinNumber;
		else if(def->Function == GPIODEF::IOFUNCTION::OUTPUT)		calc->GPIODIR_SetTo_1 += 0x1 << def->PinNumber;
		else
		{
			printf("def.Function incorrect\n");
			return false;
		}
	}
	else calc->GPIODEN_SetTo_0 += 0x1 << def->PinNumber;
	
	// Function: ANALOG
	if(def->Function == GPIODEF::IOFUNCTION::ANALOG)		calc->GPIOAMSEL_SetTo_1 += 0x1 << def->PinNumber;
	else 																							calc->GPIOAMSEL_SetTo_0 += 0x1 << def->PinNumber;
	
	// Function: ALT
	if(def->Function == GPIODEF::IOFUNCTION::ALT)	{		calc->GPIOAFSEL_SetTo_1 += 0x1 << def->PinNumber;										// Enable alternate function
																										calc->GPIOPCTL_SetTo_0 += GPIOPCTL_Clear << (4 * def->PinNumber);		// Variable to clear the alternate function
																										calc->GPIOPCTL_SetTo_1 += def->AltFunction << (4 * def->PinNumber); }	// Specify which alternate function
	else																							calc->GPIOAFSEL_SetTo_0 += 0x1 << def->PinNumber;
	
	// Bias: PULLUP
	if(def->Bias == GPIODEF::IOPULL::PULLUP)						calc->GPIOPUR_SetTo_1 += 0x1 << def->PinNumber;
	else																							calc->GPIOPUR_SetTo_0 += 0x1 << def->PinNumber;
	
	// Bias: PULLDOWN
	if(def->Bias == GPIODEF::IOPULL::PULLDOWN)					calc->GPIOPDR_SetTo_1 += 0x1 << def->PinNumber;
	else																							calc->GPIOPDR_SetTo_0 += 0x1 << def->PinNumber;
																												
	// InitialState
	if(def->InitialState == GPIODEF::IOSTATE::LOGIC_HIGH)			calc->GPIODATA_SetTo_1 += 0x1 << def->PinNumber;
	else if(def->InitialState == GPIODEF::IOSTATE::LOGIC_LOW)	calc->GPIODATA_SetTo_0 += 0x1 << def->PinNumber;
	
	PrintCalcultatedRegistersValues(calc);
	return true;
}

void GPIO::PrintCalcultatedRegistersValues(GPIOCALC* calc)
{
	//*
	printf("bitNumberRCGC2: %x\n", calc->bitNumberRCGC2);
	printf("GPIOCR_CR: %x\n", calc->GPIOCR_SetTo1);
	printf("GPIOPCTL_SetTo_0: %x\n", calc->GPIOPCTL_SetTo_0); 
	printf("GPIOPCTL_SetTo_1: %x\n", calc->GPIOPCTL_SetTo_1); 
	printf("GPIOCR_AFSEL_SetTo_0: %x\n", calc->GPIOAFSEL_SetTo_0);
	printf("GPIOCR_AFSEL_SetTo_1: %x\n", calc->GPIOAFSEL_SetTo_1); 
	printf("GPIODEN_SetTo_0: %x\n", calc->GPIODEN_SetTo_0);
	printf("GPIODEN_SetTo_1: %x\n", calc->GPIODEN_SetTo_1);
	printf("GPIODIR_SetTo_0: %x\n", calc->GPIODIR_SetTo_0);
	printf("GPIODIR_SetTo_1: %x\n", calc->GPIODIR_SetTo_1);
	printf("GPIOAMSEL_SetTo_0: %x\n", calc->GPIOAMSEL_SetTo_0);
	printf("GPIOAMSEL_SetTo_1: %x\n", calc->GPIOAMSEL_SetTo_1);
	printf("GPIOPUR_SetTo_0: %x\n", calc->GPIOPUR_SetTo_0);
	printf("GPIOPUR_SetTo_1: %x\n", calc->GPIOPUR_SetTo_1);
	printf("GPIOPDR_SetTo_0: %x\n", calc->GPIOPDR_SetTo_0);
	printf("GPIOPDR_SetTo_1: %x\n", calc->GPIOPDR_SetTo_1);
	printf("GPIODATA_SetTo_0: %x\n", calc->GPIODATA_SetTo_0);
	printf("GPIODATA_SetTo_1: %x\n", calc->GPIODATA_SetTo_1);
	//*/	
}