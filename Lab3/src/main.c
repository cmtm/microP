#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"

#include "TIM3.h"
#include "acc.h"
#include "LED.h"
#include "moving_average.h"

#define PI 3.14159265

// incremented by interupt at 100Hz
static volatile uint_fast16_t ticks;
static volatile uint_fast16_t gotTap;


void TIM3_IRQHandler(void) {
   TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
   ticks++;
}

void EXTI0_IRQHandler(void) {
	gotTap = 1;
	EXTI_ClearFlag(LIS302DL_SPI_INT1_EXTI_LINE);
}	

/*Measuring the tilt angle from the acceleration*/
void printAngles(int32_t* accData) {

	/*Change the type of the data from uint8_t to double*/
	double accX = (double) (accData[0]);
	double accY = (double) (accData[1]);
	double accZ = (double) (accData[2]);
	
	double roll;
	double pitch;

	roll = atan2(accX, sqrt(accY*accY + accZ*accZ)) * 180 / PI;	//Roll angle
	pitch = atan2(accY, sqrt(accX*accX + accZ*accZ)) * 180 / PI;	//Pitch angle	
	
	printf("Roll: %f, Pitch: %f\n", roll, pitch);
}

int main() {
	//Stores the output data from the accelerometer
	int32_t accData[3];
	
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
		//printf("x: %d; y: %d; z: %d;\n", accData[0], accData[1], accData[2]);
		printAngles(accData);
	}	
	
}



