#pragma once

#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4_discovery_lis302dl.h"
#include "moving_average.h"
#include "globals.h"
#include "cmsis_os.h"

typedef enum {
	ACC, 
	WIRELESS
} Target;

void dma_init(void);

void SPI_DMA_xfer(uint8_t* write_buff, uint8_t* read_buff, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int numByte);

void SPI_DMA_simple(uint8_t* write_buff, uint8_t* read_buff, Target t, int numByte);
