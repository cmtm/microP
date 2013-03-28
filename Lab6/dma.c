#include "dma.h"


osThreadId source_ID;
/**
  * @brief  handler for DMA interrupt
  *          
  * @param  None
  *         
  * @retval None
  */
void DMA2_Stream0_IRQHandler(void) {
	
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)) {

		/* Set CS high */
		GPIO_SetBits(LIS302DL_SPI_CS_GPIO_PORT, LIS302DL_SPI_CS_PIN);
		GPIO_SetBits(GPIOA, GPIO_Pin_4);

		/* Clear all the flags */
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
		DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);

		
		osSignalSet(source_ID, 0x2);
		
		osSemaphoreRelease(dmaSema_ID);
	}
}

void SPI_DMA_xfer(uint8_t* write_buff, uint8_t* read_buff, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int numByte, osThreadId source){

	// sema will be released in interrupt
	osSemaphoreWait(dmaSema_ID, osWaitForever);
	
	source_ID = source;
	
	//set chip select low
	GPIO_ResetBits(GPIOx, GPIO_Pin);
	// wait for slave
	while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)!=Bit_RESET);
	DMA2_Stream0->M0AR = (uint32_t)(&read_buff[0]);
	DMA2_Stream3->M0AR = (uint32_t)(&write_buff[0]);
	DMA2_Stream0->NDTR = numByte;
	DMA2_Stream3->NDTR = numByte;

	//set chip select to high
	DMA2_Stream0->CR |= DMA_SxCR_EN;
	DMA2_Stream3->CR |= DMA_SxCR_EN;
	
	// wait for DMA to finish
	osSignalWait(0x2, osWaitForever);	
}

// convinience function

void SPI_DMA_simple(uint8_t* write_buff, uint8_t* read_buff, int numByte, Target t) {
	switch(t) {
		case ACC:
			SPI_DMA_xfer(write_buff, read_buff, GPIOE, GPIO_Pin_3, numByte, mainThread_ID);
		break;
		case WIRELESS:
			SPI_DMA_xfer(write_buff, read_buff, GPIOA, GPIO_Pin_4, numByte, wirelessThread_ID);
		break;
	}
}

void dma_init(void)	{
    
	DMA_InitTypeDef  dma_init_s;
	DMA_StructInit(&dma_init_s);

	// Enable clock for DMA and SPI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream0);//RX acc
	DMA_DeInit(DMA2_Stream3);//TX acc	

	// Configure DMA streams
	dma_init_s.DMA_Channel = DMA_Channel_3;
	dma_init_s.DMA_PeripheralBaseAddr = (uint32_t) &SPI1->DR;
	dma_init_s.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_init_s.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_init_s.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_init_s.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_init_s.DMA_Mode = DMA_Mode_Normal;
	dma_init_s.DMA_Priority = DMA_Priority_High;
	dma_init_s.DMA_FIFOMode = DMA_FIFOMode_Disable;
	dma_init_s.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	dma_init_s.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma_init_s.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma_init_s.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_Init(DMA2_Stream0, &dma_init_s);

	dma_init_s.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_Init(DMA2_Stream3, &dma_init_s);

	//DMA_Cmd(DMA2_Stream0, ENABLE);

	// Enable transmission / receiving: starts
	SPI_DMACmd(SPI1, SPI_DMAReq_Rx | SPI_DMAReq_Tx , ENABLE);
	
	
	NVIC_InitTypeDef nvic_init_s;
	// Configure DMA interrupts
	nvic_init_s.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	nvic_init_s.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_init_s.NVIC_IRQChannelSubPriority = 2;
	nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_s);

	// Only interrupt on RX complete for entire transaction
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}
