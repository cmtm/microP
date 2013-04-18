#include "HD44780.h"
#include "stm32f4xx.h"


u8 LCD_line = 1;
u8 LCD_column = 1;

void LCD_ConfigurePort(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Open the clocks we want
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	// Configure the LCD pins for push-pull output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
}

void LCD_init(void)	{
	
	Clr_RS;
	Clr_RW;
	
	//clear the display
	LCD_writeData(0x01);
	
	//Reset the position of the display and cursor
	LCD_writeData(0x02);
	
	//Increment mode, no screen shift
	LCD_writeData(0x06);
	
	//set the function
	LCD_writeData(0x38);
	
	// Turn display on, with cursor on and blinking
	LCD_writeData(0x0E);
	
	LCD_Clear();
}


void LCD_writeData(u8 data)
{
	int count = 0xFF;
	while(count--) 	{
	LCD_Delay(0xFF);
	}

	GPIOE->ODR=( (GPIOE->ODR & 0x70) | (data << 7));
	
	Set_Clk;

	count = 0xFF;
	while(count--) 	{
	LCD_Delay(0xFF);
	}

	Clr_Clk;

	count = 0xFF;
	while(count--) 	{
	LCD_Delay(0xFF);
	}
	
	if ((GPIOE->ODR & 0x40)== 0x40)
		LCD_IncrementCursorVariables();
}

void LCD_Clear(void)
{
	Clr_RS;
	Clr_RW;
	LCD_writeData(0x01);

	LCD_line = 1;
	LCD_column = 1;
	LCD_Delay(0xFFFF);
}

void LCD_IncrementCursorVariables(void)
{
	if((LCD_column + 1) <= Num_Characters) {
		LCD_column++;
	}else if((LCD_line + 1) <= Num_Lines) {
		LCD_line++;
		LCD_column = 1;
	}else {
		LCD_line = 1;
		LCD_column = 1;
	}
	
			printf("line: %d, column: %d\n", LCD_line, LCD_column);
}


void LCD_Print(uint8_t volume, Synth currentSynth, uint8_t alpha, uint8_t beta, uint8_t note){
	
	LCD_SetCursor(1,1);
	char vol[8];
 	snprintf(vol, 8, "vol:%d",volume);
 	LCD_WriteString(vol);
	
	LCD_SetCursor(1,8);
 	
 	char alph[10];
 	snprintf(alph, 10, "alpha:%d ", alpha);
 	LCD_WriteString(alph);
 	
	LCD_SetCursor(1,17);
	
 	char bet[9];
 	snprintf(bet, 9, "beta:%d ", beta);
 	LCD_WriteString(bet);
 	
 	LCD_SetCursor(2,1);
 	
 	char notee[9];
 	snprintf(notee, 9, "note:%d ", note);
 	LCD_WriteString(notee);
 	
	LCD_SetCursor(2,9);
		
	switch(currentSynth) {
		case SAWTOOTH:
			LCD_WriteString("Mode:Sawtooth");
		break;
		case FM:
			LCD_WriteString("Mode:FM");
		break;
	}
	
}
	


void LCD_WriteString(const char* line) 
{
	int i;
	int length = (unsigned)strlen(line);
	Set_RS;
	
	for(i = 0;i < length ;i++){
		LCD_writeData((int)line[i]);
	}
	
	Clr_RS;
}

//CHECK AGAIN (NOT USED YET)
void LCD_SetCursor(u8 line,u8 column)
{
	Clr_RS;
	Clr_RW;
	u8 position;
	
	switch(line) {
		case 1:
			position = column - 0x01;
			break;
		case 2:
			position = column + 0x3F;
			break;
		case 3:
			position = column + 0x13;
			break;
		case 4:
			position = column + 0x53;
			break;
		default:
			break;
	}

	LCD_writeData(position | 0x80);

	LCD_line = line;
	LCD_column = column;
	
	
}

//count the time required
void LCD_Delay(int Count)
{
	while(Count--)
	{
	}
}



