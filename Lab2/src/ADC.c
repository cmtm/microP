#include "ADC.h"


float samples;

// fill this in
float getTemp_celcius() {
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

	return ((ADC1->DR * 3000.0/4096.0) - 760)/2.5 + 25;
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
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//Alligned right
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1,&ADC_InitStructure);	//Initialize ADC1
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	/*Enabling the ADC*/
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
}
	


