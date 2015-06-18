#include "mcp9700.h"


#include "hw_types.h"
#include "hw_memmap.h"

#include "pin.h"
#include "rom.h"
#include "adc12.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "gpio.h"

#define  ADC_SAMPES_VALUE     16


// GPIO
#define        PWR_GP                     6       // (PIN_61)

// PORT _ A0,A1,A2,A3 = (GPIO / 8)
#define       PWR_PORT              GPIOA0_BASE   //  6/ 8 = 0

//  PIN _ 1,2,3,4,5,6,7,,,, = 1 << (GPIO % 8)
#define       PWR_PIN        (1<< (PWR_GP%8))


inline void Mcp9700On()
{
	GPIOPinWrite(PWR_PORT, PWR_PIN,PWR_PIN);  // power on
	MAP_UtilsDelay(80000000/10); 	  	      // wait 100 ms
}

inline void Mcp9700Off()
{
	 GPIOPinWrite(PWR_PORT, PWR_PIN,0x00);
}

float Mcp9700Temperature()
{
	uInt32 adcBinary;
	float  adcVoltage;
	float  temperature;

	Mcp9700On();
	adcBinary  =  Adc12Read(PIN_58, ADC_SAMPES_VALUE);
	Mcp9700Off();

	adcVoltage  =  (adcBinary / (float)4095) * (float)1.45;  // 12 bit range 0V-1.45V
	temperature =  (adcVoltage - (float)0.5) * (float)100;   // 0 C = 0.5V and  1 C = 10 mV

	return temperature;
}

