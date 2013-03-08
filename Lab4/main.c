#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "LED.h"
#include "button.h"
#include "accMode.h"
#include "tempMode.h"

/*!
 @brief Program entry point
 */
int main (void) {
	LED_init();
	buttonInit();
	tM_init();
	aM_init();

	
	osTimerDef (TEMP_TIMER, tM_run);
	osTimerId tempTimerID = osTimerCreate( osTimer (TEMP_TIMER), osTimerPeriodic, NULL);
	
	osTimerStart(tempTimerID, 50);
	
	// The below doesn't really need to be in a loop
	while(1);
}

void thread (void const *argument) {
	while(1){
		osDelay(1000);
		GPIOD->BSRRL = GPIO_Pin_12;
		osDelay(1000);
		GPIOD->BSRRH = GPIO_Pin_12;
	}
}
