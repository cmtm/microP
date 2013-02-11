/// @file ADC.h

#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"


/// Initialize ADC functionality
///
/// This must be called before getTemp_celcius()
void ADC_init(void);

/// Get on chip temperature
///
/// @return temperature in degrees celcius
float getTemp_celcius(void);
