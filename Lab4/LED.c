#include "LED.h"



void LED_set(LED_state s0, LED_state s1, LED_state s2, LED_state s3) {

       if (s0==ON)
               GPIOD->BSRRL |= 1 << 12;
       else
               GPIOD->BSRRH |= 1 << 12;
	   
	   
       if (s1==ON)
               GPIOD->BSRRL |= 1 << 13;
       else
               GPIOD->BSRRH |= 1 << 13;
	   
	   
       if (s2==ON)
               GPIOD->BSRRL |= 1 << 14;
       else
               GPIOD->BSRRH |= 1 << 14;
	   
	   
       if (s3==ON)
               GPIOD->BSRRL |= 1 << 15;
       else
               GPIOD->BSRRH |= 1 << 15;
}

void LED_setAll(LED_state s) {
	LED_set(s,s,s,s);
}

void LED_init(void)	{

	/*Structure defined in stm32f4xx_gpio library
	Specifies the operating mode for the selected pins*/
	GPIO_InitTypeDef GPIO_InitStruct;

	/*Enabling the clock for the I/O peripheral (AHB1 peripheral) for Port D (where the LEDs are)*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/*Setting the properties of the peripheral. Note that
	the 4 LEDs we use on the board are connected to pins PD12-PD15*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;	//Initiate 4 LED pins
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;	//The pins are outputs
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 	//Setting all GPIO clock speeds
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	// this sets the pin type to push / pull (as opposed to open drain)
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	// this sets the pullup / pulldown resistors to be inactive
	GPIO_Init(GPIOD, &GPIO_InitStruct); 			// Set the operating mode for port D
}
