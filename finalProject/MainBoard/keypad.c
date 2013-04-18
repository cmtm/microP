#include "keypad.h"

//input state: are we reading columns or rows?
uint8_t inputState;

//the column and row in which the pressed key lies
//which key is pressed?
uint8_t pressCol = 0;
uint8_t pressRow = 0;
uint8_t	pressKey = 0;

/**
  * @brief  set the keypad mode: reading GPIO pins of row
  *          
  * @param  None
  *         
  * @retval none
  */
void keypad_GPIO_Row(){
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	// Configure GPIO ports and pins
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	// Config columns as outputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = COL_1_PIN;	
	GPIO_Init(COL_1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = COL_2_PIN;	
	GPIO_Init(COL_2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = COL_3_PIN;	
	GPIO_Init(COL_3_PORT, &GPIO_InitStructure);
	
	// Config rows as inputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_1_PIN;
	GPIO_Init(ROW_1_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_2_PIN;
	GPIO_Init(ROW_2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_3_PIN;
	GPIO_Init(ROW_3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_4_PIN;
	GPIO_Init(ROW_4_PORT, &GPIO_InitStructure);	
	
	//Note: Pin 1 is ground --> no GPIO configuration
}

/**
  * @brief  set the keypad mode: reading GPIO pins of column
  *          
  * @param  None
  *         
  * @retval none
  */
void keypad_GPIO_Col(){
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	// Configure GPIO ports and pins
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	// Config columns as inputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = COL_1_PIN;	
	GPIO_Init(COL_1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = COL_2_PIN;	
	GPIO_Init(COL_2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = COL_3_PIN;	
	GPIO_Init(COL_3_PORT, &GPIO_InitStructure);
	
	// Config rows as outputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_1_PIN;
	GPIO_Init(ROW_1_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_2_PIN;
	GPIO_Init(ROW_2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_3_PIN;
	GPIO_Init(ROW_3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ROW_4_PIN;
	GPIO_Init(ROW_4_PORT, &GPIO_InitStructure);	
	
	//Note: Pin 1 is ground --> no GPIO configuration
}

void keyInit(void) {
	// Enable clocks for GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	//configure column as inputs
	keypad_GPIO_Col();
	
	//input state: columns are the inputs
	inputState = INPUT_COLUMN;
}



/**
  * @brief  check is a key is pressed
  *          
  * @param  None
  *         
  * @retval 1 - if a key is pressed, 0 - if no key is pressed
  */
uint8_t isKeyPressed(){
	
	pressCol = 0;
	pressRow = 0;
	pressKey = 0;
	//A key is pressed if a column key is pressed
	if (GPIO_ReadInputDataBit(COL_1_PORT, COL_1_PIN) == Bit_RESET || 
			GPIO_ReadInputDataBit(COL_2_PORT, COL_2_PIN) == Bit_RESET || 
			GPIO_ReadInputDataBit(COL_3_PORT, COL_3_PIN) == Bit_RESET)
		return 1;
	else 
		return 0;
}
/**
  * @brief  change inputs/outputs state of GPIO pins
  *          
  * @param  None
  *         
  * @retval None
  */
void swapKeyFunction(void){
	
	//switch the inputs state (reading GPIO inputs of rows or columns)
	if (inputState == INPUT_COLUMN){
		keypad_GPIO_Row();
		inputState = INPUT_ROW;
	}
	else {
		keypad_GPIO_Col();
		inputState = INPUT_COLUMN;
	}
}
/**
  * @brief  convert column and row value to the key number
  *          
  * @param  None
  *         
  * @retval None
  */
void convertToKeyNumber (){
	//calculate the key number based on the row and column in which the key is pressed
	uint8_t keyID =(pressRow-1)*3 + pressCol;
	
	//check to see if the keyID is valid
	if (keyID > 0 && keyID<13)
		pressKey = keyID;

}



/**
  * @brief  Procedure one needs to take to determine which key is pressed
  *          
  * @param  None
  *         
  * @retval key press
  */
Key keyGetPushed() {
	
	if (!isKeyPressed())
		return (Key)0;
	
	//check which key column is pressed
	pressCol = 0;
	if (GPIO_ReadInputDataBit(COL_1_PORT, COL_1_PIN)== Bit_RESET)
		pressCol = 1;
	
	if (GPIO_ReadInputDataBit(COL_2_PORT, COL_2_PIN)== Bit_RESET)
		pressCol = 2;
	
	if (GPIO_ReadInputDataBit(COL_3_PORT, COL_3_PIN)== Bit_RESET)
		pressCol = 3;
	
	//sway key function to check which key row is pressed
	swapKeyFunction();
	
	//check which key row is pressed
	pressRow = 0;
	if (GPIO_ReadInputDataBit(ROW_1_PORT, ROW_1_PIN)== Bit_RESET)
		pressRow = 1;
	
	if (GPIO_ReadInputDataBit(ROW_2_PORT, ROW_2_PIN)== Bit_RESET)
		pressRow = 2;
	
	if (GPIO_ReadInputDataBit(ROW_3_PORT, ROW_3_PIN)== Bit_RESET)
		pressRow = 3;
	
	if (GPIO_ReadInputDataBit(ROW_4_PORT, ROW_4_PIN)== Bit_RESET)
		pressRow = 4;
	
	//sway key function to check which column is pressed (get ready for the next polling of keypad)
	swapKeyFunction();
	
	//find/compute which key is pressed
	convertToKeyNumber ();

		
	
	return (Key)pressKey;
}
