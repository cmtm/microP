#include "TIM3.h"

#define PRESCALAR 42000

/*Timer initializer*/
void TIM3_Init()
{	
  //create instance of variables for initialization
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
  //get clock frequency 
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);

  //TIM3 clock enable 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  //enable the TIM3 gloabal Interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  //time base configuration 
  TIM_TimeBaseStructure.TIM_Period = 100-1; 	
  TIM_TimeBaseStructure.TIM_Prescaler = (RCC_Clocks.PCLK2_Frequency /10000)-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  //TIM interrupts enable for timer TIM3 
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  //TIM3 counter enable 
  TIM_Cmd(TIM3, ENABLE);
}
