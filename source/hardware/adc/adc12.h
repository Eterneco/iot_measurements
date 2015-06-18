#ifndef __ADC12_H
#define __ADC12_H

#include "typeDefs.h"

extern void Adc12Init();

extern void Adc12Enable();
extern void Adc12Disable();

extern uInt16 Adc12Read(uInt32 pinNumber, uInt16 counter);

#endif
