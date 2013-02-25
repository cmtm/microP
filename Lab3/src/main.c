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
<<<<<<< HEAD

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
	
=======


static volatile uint_fast16_t ticks; //incremented by interupt at 100Hz
static volatile uint_fast16_t gotTap;	//set when tap is detected

/*Increments ticks to 1 every 100 Hz*/
void TIM3_IRQHandler(void) {
   TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
   ticks++;
}

/*Sets tap to 1 when tap is detected*/
void EXTI0_IRQHandler(void) {
	EXTI_ClearFlag(LIS302DL_SPI_INT1_EXTI_LINE);
	gotTap = 50;

}	

/*Measuring the tilt angle from the acceleration and printing*/
void printAngles(int32_t* accData) {

	/*Change the type of the data from uint32_t to double*/
	double accX = (double) (accData[0]);
	double accY = (double) (accData[1]);
	double accZ = (double) (accData[2]);
	
	double roll;
	double pitch;


	roll = atan2(accX, sqrt(accY*accY + accZ*accZ)) * 180 / PI;	//Roll angle
	pitch = atan2(accY, sqrt(accX*accX + accZ*accZ)) * 180 / PI;	//Pitch angle	
	
	if (accZ<0) {
		if (accX>0)	roll = 180 - roll;
		else roll = -roll - 180;
		
		if (accY>0)	pitch = 180 - pitch;
		else pitch = -pitch - 180;
		
	}

	
>>>>>>> lab3 complete and some report
	printf("Roll: %f, Pitch: %f\n", roll, pitch);
}

int main() {
<<<<<<< HEAD
	//Stores the output data from the accelerometer
	int32_t accData[3];
	
=======
	
	//Stores the output acceleration from the accelerometer
	int32_t accData[3];
	
	static FilterState filterState1 = {0}, filterState2 = {0}, filterState3 = {0};
	
	//LEDs currently off
>>>>>>> lab3 complete and some report
	LED_state led_state = OFF;
	
	ticks = 0;
	
	acc_init();	//Setting up the acc parameters and interrupt
	LED_init();	//Setting up LED parameters
	
 
	// set interupt to 100Hz
	TIM3_Init(1);
	
	while(1) {
		// wait for interupt
		while(!ticks);
		
		ticks--;
		
		// NOT MEMORY SAFE !!!
<<<<<<< HEAD
		if(gotTap) {
			led_state = led_state ? OFF : ON;
			LED_setAll(led_state);
			gotTap = 0;
		}		

		acc_read(accData);	//Reads the accelerometer data
=======
		/*Turn on the LEDs if the accelerometer is tapped*/
		if(gotTap > 1)
			gotTap--;
		else if(gotTap == 1) {
			led_state = led_state ? OFF : ON;
			LED_setAll(led_state);
			gotTap = 0;
			acc_clearLatch();
		}

		acc_read(accData);	//Reads the accelerometer data
		
		accData[0] = moving_average_c(accData[0], &filterState1);
		accData[1] = moving_average_c(accData[1], &filterState2);
		accData[2] = moving_average_c(accData[2], &filterState3);
		
		
		//Print data
>>>>>>> lab3 complete and some report
		printf("x: %d; y: %d; z: %d;\n", accData[0], accData[1], accData[2]);
		printAngles(accData);
	}	
	
}



