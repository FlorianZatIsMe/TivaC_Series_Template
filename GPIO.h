#ifndef GPIO_HEADER
#define GPIO_HEADER

#include <stdio.h>

#include <cstdint>
#include <array>
#include <span>
#include <concepts>

#include "SYSCTL.h"

#define REGISTER(ADDRESS)				(*((volatile unsigned long *)(ADDRESS)))	// Macro used to read or write the register with the address ADDRESS

// GPIO base addresses
#define PORTA_BASE_ADDRESS			0x40020000
#define PORTB_BASE_ADDRESS			0x40021000
#define PORTC_BASE_ADDRESS			0x40022000
#define PORTD_BASE_ADDRESS			0x40023000
#define PORTE_BASE_ADDRESS			0x40024000
#define PORTF_BASE_ADDRESS			0x40025000

// Offset of the GPIO registers
#define GPIO_DATA_Offset				0x000003FC 
#define GPIO_DIR_Offset					0x00000400 
#define GPIO_IS_Offset					0x00000404 
#define GPIO_IBE_Offset					0x00000408 
#define GPIO_IEV_Offset					0x0000040C 
#define GPIO_IM_Offset					0x00000410 
#define GPIO_RIS_Offset					0x00000414 
#define GPIO_MIS_Offset					0x00000418 
#define GPIO_ICR_Offset					0x0000041C 
#define GPIO_AFSEL_Offset				0x00000420 
#define GPIO_DR2R_Offset				0x00000500 
#define GPIO_DR4R_Offset				0x00000504 
#define GPIO_DR8R_Offset				0x00000508 
#define GPIO_ODR_Offset					0x0000050C 
#define GPIO_PUR_Offset					0x00000510 
#define GPIO_PDR_Offset					0x00000514 
#define GPIO_SLR_Offset					0x00000518 
#define GPIO_DEN_Offset					0x0000051C 
#define GPIO_LOCK_Offset				0x00000520 
#define GPIO_CR_Offset					0x00000524 
#define GPIO_AMSEL_Offset				0x00000528 
#define GPIO_PCTL_Offset				0x0000052C 
#define GPIO_ADCCTL_Offset			0x00000530 
#define GPIO_DMACTL_Offset			0x00000534 
#define GPIO_PeriphID4_Offset		0x00000FD0 
#define GPIO_PeriphID5_Offset		0x00000FD4 
#define GPIO_PeriphID6_Offset		0x00000FD8 
#define GPIO_PeriphID7_Offset		0x00000FDC 
#define GPIO_PeriphID0_Offset		0x00000FE0 
#define GPIO_PeriphID1_Offset		0x00000FE4 
#define GPIO_PeriphID2_Offset		0x00000FE8 
#define GPIO_PeriphID3_Offset		0x00000FEC

#define GPIOPCTL_Clear					0xF

// STRUCTURE DEFINITION
// --------------------
// Representative of memory mapped configuration registers for the IO pins
struct GPIO_TypeDef { uint32_t ODR; };

// Used to define the configuration of an IO pin
using GPIO_PinDef = \
struct IOPinDefStruct {
		uint32_t PinNumber;
		bool Used;
    enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
    uint32_t AltFunction;
    enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
    enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;
};

// A type that will completely describe the function of a port
using GPIODEFFULLPORT = \
struct IOPortDefStuct {
    GPIO_TypeDef* GPIO;
		std::array<GPIO_PinDef, 8>* Pins;
};

// A type that will completely describe the function of one IO pin
using GPIODEF = \
struct IODefStuct {
    GPIO_TypeDef* GPIO_Port;
		uint32_t PinNumber;
    enum IOFUNCTION:uint32_t { INPUT = 0, OUTPUT = 1, ALT = 2, ANALOG = 3 } Function;
    uint32_t AltFunction;
    enum IOPULL:uint32_t { NONE = 0, PULLUP = 1, PULLDOWN = 2 } Bias;
    enum IOSTATE:uint32_t { LOGIC_LOW, LOGIC_HIGH, DONT_CARE } InitialState;
};

// Contains the values to put in the configuration registers fot the IO pins
using GPIOCALC = \
struct IOCalcStuct {
	uint8_t bitNumberRCGC2;					// Number of the bit to set to 1 for the Register RCGC2: Enable PORT clock
	uint8_t GPIOCR_SetTo1 = 0;			// Value to set to 1 for the Register GPIOCR: Allow change on the registers GPIOAFSEL, GPIOPUR, GPIOPDR, and GPIODEN on the used pins
	uint8_t GPIODEN_SetTo_0 = 0;		// Value to set to 0 for the Register GPIODEN - DIGITAL: Disable digital for the applicable pins
	uint8_t GPIODEN_SetTo_1 = 0;		// Value to set to 1 for the Register GPIODEN - DIGITAL: Ensable digital for the applicable pins
	uint8_t GPIODIR_SetTo_0 = 0;		// Value to set to 0 for the Register GPIODIR - INPUT / OUTPUT: Set the applicable pins as inputs
	uint8_t GPIODIR_SetTo_1 = 0;		// Value to set to 1 for the Register GPIODIR - INPUT / OUTPUT: Set the applicable pins as outputs
	uint8_t GPIOAMSEL_SetTo_0 = 0;	// Value to set to 0 for the Register GPIOAMSEL - ANALOG: Enable analog function for the applicable pins
	uint8_t GPIOAMSEL_SetTo_1 = 0;	// Value to set to 1 for the Register GPIOAMSEL - ANALOG: Disable analog function for the applicable pins
	uint8_t GPIOAFSEL_SetTo_0 = 0;	// Value to set to 0 for the Register GPIOAFSEL - ALT: No alternate function for the applicable pins
	uint8_t GPIOAFSEL_SetTo_1 = 0;	// Value to set to 1 for the Register GPIOAFSEL - ALT: Alternate function for the applicable pins
	uint32_t GPIOPCTL_SetTo_0 = 0;	// Value to set to 0 for the Register GPIOPCTL - ALT: Variable used to clear the slternate function register for the applicable pin
	uint32_t GPIOPCTL_SetTo_1 = 0;	// Value to set to 1 for the Register GPIOPCTL - ALT: Sets the slternate function for the applicable pins
	uint8_t GPIOPUR_SetTo_0 = 0;		// Value to set to 0 for the Register GPIOPUR - PULLUP: Disable pullup resistors on applicable pins
	uint8_t GPIOPUR_SetTo_1 = 0;		// Value to set to 1 for the Register GPIOPUR - PULLUP: Ensable pullup resistors on applicable pins
	uint8_t GPIOPDR_SetTo_0 = 0;		// Value to set to 0 for the Register GPIOPDR - PULLDOWN: Disable pulldown resistors on applicable pins
	uint8_t GPIOPDR_SetTo_1 = 0;		// Value to set to 1 for the Register GPIOPDR - PULLDOWN: Enable pulldown resistors on applicable pins
	uint8_t GPIODATA_SetTo_0 = 0;		// Value to set to 0 for the Register GPIODATA - INITIAL STATE set to 0 on applicable pins
	uint8_t GPIODATA_SetTo_1 = 0;		// Value to set to 1 for the Register GPIODATA - INITIAL STATE set to 1 on applicable pins
};

class GPIO
{
	public:
		GPIO();
		bool Configure(GPIODEFFULLPORT* def);
		bool Configure(GPIODEF* def);

		GPIO_TypeDef PORTA;
		GPIO_TypeDef PORTB;
		GPIO_TypeDef PORTC;
		GPIO_TypeDef PORTD;
		GPIO_TypeDef PORTE;
		GPIO_TypeDef PORTF;
	
	private:
		bool CalculateRegistersValues(const GPIODEFFULLPORT* def, GPIOCALC* calc);
		bool CalculateRegistersValues(const GPIODEF* def, GPIOCALC* calc);
		void PrintCalcultatedRegistersValues(GPIOCALC* calc);
};


#endif