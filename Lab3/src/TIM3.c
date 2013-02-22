#include "TIM3.h"

#define PRESCALAR 1000

void TIM3_Init(uint32_t freq)
{
	uint32_t periodValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* Compute the prescaler value */
	//PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / freq) - 1;
	periodValue = (SystemCoreClock / (2 * PRESCALAR)) / freq - 1;
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = periodValue;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* Prescaler configuration */
	TIM_PrescalerConfig(TIM3, PRESCALAR - 1, TIM_PSCReloadMode_Immediate);
	
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
