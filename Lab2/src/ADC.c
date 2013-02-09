#include "ADC.h"

/* ADC. Can measure signals from 16 external sources. 
Conversion can be performed in single, continuous, scan or discontinuous mode
Result stored in 16 bit data register
Powered on using ADON in ADC_CR2
Conversion happens when SWSTART is set
Can stop conversion by unsetting ADON
ADC clock: ADCCLK = analog clock generated from APB2 allows us to work at 
*/
float samples;

// fill this in
float getTemp_celcius() {
	return 0.0;
}

void ADC_init(void)	{

	/*Defining the ADC initial parameter setting structures*/
	ADC_InitTypeDef	ADC_InitStructure;
	ADC_CommonInitTypeDef adc_common_init_s;
		
	/*Enabling clock on bus APB2 to peripheral ADC1*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/*Setting the parameters for the ADC and initializing*/
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;	//Half the APB2 speed
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	//No DMA
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//Setting the sampling delay to 
	ADC_CommonInit(&adc_common_init_s);	//Initialize with the above parameters
	
	/*Setting further parameters for the ADC and initializing*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//Returning a 12 bit (max) result
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//A single channel is used
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//Not in continuous mode
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//Alligned right
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1,&ADC_InitStructure);	//Initialize ADC1
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	/*Enabling the ADC*/
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_3Cycles);

}

float getTemperature(void)
{
	
	ADC_SoftwareStartConv(ADC1);
	
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)== RESET);
	
	samples = ADC_GetConversionValue(ADC1); //Get the voltage temperature sample reading from the ADC (in millivolts)
	return (samples); //Convert this sample to a temperature in degrees celsius	
	
}
	


