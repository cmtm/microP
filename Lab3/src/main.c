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
static volatile uint_fast16_t gotTap;


void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    ticks++;
  }
  // else: PANIC
}

void EXTI0_IRQHandler(void) {
	gotTap = 1;
	EXTI_ClearFlag(LIS302DL_SPI_INT1_EXTI_LINE);
}	

int main() {
	//Stores the output data from the accelerometer
	int32_t accData[3];
	//Stores the angles calculated from the accelerations
	double angles[3];
	
	LED_state led_state = OFF;
	
	ticks = 0;
	
	acc_init();
	LED_init();
	
 
	// set interupt to 100Hz
	TIM3_Init(1);
	
	while(1) {
		// wait for interupt
		while(!ticks);
		
		ticks--;
		
		// NOT MEMORY SAFE !!!
		if(gotTap) {
			led_state = led_state ? OFF : ON;
			LED_setAll(led_state);
			gotTap = 0;
		}		

		acc_read(accData);	//Reads the accelerometer data
		printf("x: %d; y: %d; z: %d;\n", accData[0], accData[1], accData[2]);
		// accData[0] = accData[0]- offset[0];	//Adjusts x according to the offset
		// accData[1] = accData[1]- offset[1];	//Adjusts y according to the offset
		// accData[2] = accData[2]- offset[2];	//Adjusts z according to the offset
		
		// calculate_angle(accData, angles);
		
	}	
	
}



