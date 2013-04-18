#include "stm32f4xx.h"
#include "stdio.h"
#include <string.h>
#include "dsp.h"

// Device Defines (set these for your device!)
#define Num_Lines 2
#define Num_Characters 24  // Per line

#define Set_RS GPIO_SetBits(GPIOE,GPIO_Pin_6)
#define Clr_RS GPIO_ResetBits(GPIOE,GPIO_Pin_6)

#define Set_RW GPIO_SetBits(GPIOE,GPIO_Pin_4)
#define Clr_RW GPIO_ResetBits(GPIOE,GPIO_Pin_4)

#define Set_Clk GPIO_SetBits(GPIOE,GPIO_Pin_5)
#define Clr_Clk GPIO_ResetBits(GPIOE,GPIO_Pin_5)

void lcdInit(void);
void lcdPrint(uint8_t volume, Synth currentSynth, uint8_t alpha, uint8_t beta, uint8_t note);
void lcdThread(const void* args);

void LCD_ConfigurePort(void);
void LCD_writeData(u8 data);
void LCD_Clear(void);
void LCD_IncrementCursorVariables(void);
void LCD_SetCursor(u8 line,u8 column);
void LCD_Delay(int Count);
void LCD_WriteString(const char* line);