#pragma once

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

// CONSTANTS: keypad port/pin and column and rows port/pin
#define KEYPAD_PORT_2		GPIOC
#define KEYPAD_PIN_2		GPIO_Pin_14
#define KEYPAD_PORT_3		GPIOC
#define KEYPAD_PIN_3		GPIO_Pin_13
#define KEYPAD_PORT_4		GPIOD
#define KEYPAD_PIN_4		GPIO_Pin_0
#define KEYPAD_PORT_5		GPIOD
#define KEYPAD_PIN_5		GPIO_Pin_1
#define KEYPAD_PORT_6		GPIOD
#define KEYPAD_PIN_6		GPIO_Pin_2
#define KEYPAD_PORT_7		GPIOD
#define KEYPAD_PIN_7		GPIO_Pin_3
#define KEYPAD_PORT_8		GPIOC
#define KEYPAD_PIN_8		GPIO_Pin_15

#define	COL_1_PORT			KEYPAD_PORT_6
#define COL_1_PIN 			KEYPAD_PIN_6
#define	COL_2_PORT			KEYPAD_PORT_7
#define COL_2_PIN 			KEYPAD_PIN_7
#define	COL_3_PORT			KEYPAD_PORT_8
#define COL_3_PIN 			KEYPAD_PIN_8

#define	ROW_1_PORT			KEYPAD_PORT_2
#define ROW_1_PIN 			KEYPAD_PIN_2
#define	ROW_2_PORT			KEYPAD_PORT_3
#define ROW_2_PIN 			KEYPAD_PIN_3
#define	ROW_3_PORT			KEYPAD_PORT_4
#define ROW_3_PIN 			KEYPAD_PIN_4
#define	ROW_4_PORT			KEYPAD_PORT_5
#define ROW_4_PIN 			KEYPAD_PIN_5

// Input states
#define INPUT_COLUMN		0
#define INPUT_ROW				1

typedef enum {
	NOKEY, 
	K1, K2, K3, K4, K5, K6, K7, K8, K9,
	STAR, K0,
	POUND
} Key;


Key keyGetPushed(void);
void keyInit(void);
