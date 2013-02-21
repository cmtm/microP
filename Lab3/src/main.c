#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"

#include "TIM3.h"
#include "acc.h"
#include "LED.h"
#include "moving_average.h"

// incremented by interupt at 100Hz
static volatile uint_fast16_t ticks;

uint8_t accData[3];	//Stores the output data from the accelerometer
double angles[3];		//Stores the angles calculatred from the accelerations

void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    ticks++;
  }
  // else: PANIC
}

int main() {
	ticks = 0;
	
	acc_init();
	LED_init();
	
 
	// set interupt to 100Hz
	TIM3_Init(100);
	
	while(1) {
		// wait for interupt
		while(!ticks);
		
		ticks--;
		

		acc_read(accData);	//Reads the accelerometer data
		printf("%d; %d; %d;\n", accData[0], accData[1], accData[2]);
		// accData[0] = accData[0]- offset[0];	//Adjusts x according to the offset
		// accData[1] = accData[1]- offset[1];	//Adjusts y according to the offset
		// accData[2] = accData[2]- offset[2];	//Adjusts z according to the offset
		
		// calculate_angle(accData, angles);
		
	}	
	
}



