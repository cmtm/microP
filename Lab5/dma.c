#include "dma.h"

void dma_init(void)	{
    
	// SPI_InitTypeDef  SPI_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	DMA_Cmd(DMA2_Stream0, DISABLE); //disable receiver
	DMA_Cmd(DMA2_Stream3, DISABLE);	//disable transmitter
    
    DMA_DeInit(DMA2_Stream0);	//place default values 
	DMA_DeInit(DMA2_Stream3); 
	
	DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);	//transfer complete
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3); 
  
	DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TEIF0);	//transfer error
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TEIF3); 
  
	DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_FEIF0); //FIFO error
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_FEIF3); 
  
	DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_DMEIF0); //direct mode error
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_DMEIF3);
      
    /* Start DMA2 clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
	// Configure DMA streams
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;  //not needed
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
    //receiving
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  
	//sending
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; 
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);
    
    
	//enable
	SPI_DMACmd(SPI1, SPI_DMAReq_Rx | SPI_DMAReq_Tx, ENABLE);   
  
	//Setting up interrupt
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
  
  
	//enable receiving
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}
