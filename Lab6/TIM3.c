#include "TIM3.h"

#define PRESCALAR 42000

/*Timer initializer*/
#define PRESCALAR 42000

/*Timer initializer*/
void TIM3_Init(uint32_t freq)
{	
	uint32_t periodValue;	//Holds the period value
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//Creating the timer initializing strucure
	
	NVIC_InitTypeDef NVIC_InitStructure;	//Creating the NVIC initializing structure
	RCC_ClocksTypeDef	RCC_Clock;	//APB bus clocks configuration structure
	RCC_GetClocksFreq(&RCC_Clock); //Returns the frequencies of different on chip clocks; SYSCLK, HCLK, PCLK1 and PCLK2
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*Results in a 1000 period value*/
	periodValue = (RCC_Clock.PCLK1_Frequency/(PRESCALAR)) / freq - 1;
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = periodValue;	//Setting the period 
	TIM_TimeBaseStructure.TIM_Prescaler = PRESCALAR; //Setting the prescalar
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	
	/*Initializing TIM3 with the settings above*/
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	
	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
