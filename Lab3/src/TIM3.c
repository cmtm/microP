#include "TIM3.h"

void TIM3_Init(uint32_t freq)
{
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / freq) - 1;
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* Prescaler configuration */
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}
