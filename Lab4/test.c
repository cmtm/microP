

	
	//enter defailt values
	//SPI_StructInit(&SPI_InitStructure); //dont think this should be here
	DMA_StructInit(&DMA_InitStructure);



	//receiving
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	

	

	

	

