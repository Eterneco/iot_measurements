#include "adc12.h"

#include "adc.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "utils.h"
#include "pin.h"
#include "gpio_if.h"
#include "rom.h"
#include "rom_map.h"

#include "uartA0.h"

// voltage=readADC(PIN_58);

void Adc12Init()
{

#ifdef CC3200_ES_1_2_1
    //
    // Enable ADC clocks.###IMPORTANT###Need to be removed for PG 1.32
    //
    HWREG(GPRCM_BASE + GPRCM_O_ADC_CLK_CONFIG) = 0x00000043;
    HWREG(ADC_BASE + ADC_O_ADC_CTRL)   = 0x00000004;
    HWREG(ADC_BASE + ADC_O_ADC_SPARE0) = 0x00000100;
    HWREG(ADC_BASE + ADC_O_ADC_SPARE1) = 0x0355AA00;
#endif

    //
    // Disable ADC timer which is used to timestamp the ADC data samples
    //
     ADCTimerDisable(ADC_BASE);

}

void Adc12Enable()
{
	MAP_ADCEnable(ADC_BASE);
}

void Adc12Disable()
{
	MAP_ADCDisable(ADC_BASE);
}


uInt16 Adc12Read(uInt32 pinNumber, uInt16 counter)
{
		uInt16  channel, index;
		uInt16  sample;
		uInt32  buffor;

		index  = 0;
		buffor = 0;

		switch(pinNumber)
		{
			case PIN_57:  channel = ADC_CH_0;
 					  	  break;
			case PIN_58:  channel = ADC_CH_1;
						  break;
			case PIN_59:  channel = ADC_CH_2;
			 	 	 	  break;
			case PIN_60:  channel = ADC_CH_3;
						  break;
			default: break;
		}

		MAP_ADCChannelEnable(ADC_BASE, channel);
		MAP_UtilsDelay(80000000/100);			// wait 100 ms

		while(index < counter)
		{
			if(MAP_ADCFIFOLvlGet(ADC_BASE, channel))
			{
				sample = MAP_ADCFIFORead(ADC_BASE, channel);
				sample = (sample & 0x3ffc)>>2;

				//DBG_PRINT(" %.2d ",sample);

				index  += 1;
				buffor += sample;
			 }
		}

		MAP_ADCChannelDisable(ADC_BASE, channel);

		return ( buffor / counter);
}
