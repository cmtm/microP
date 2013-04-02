    #include "wirelessDriver.h" //the wireless thing
	#include "dma.h"
	#include "globals.h"
	#include "cmsis_os.h"
    #include <math.h>
    #include <stdint.h>
	
	osThreadId source_ID;
	
	
	uint8_t r_buff[12]={0x00};
	uint8_t w_buff[12]={0x00};
	
	float alpha, beta;
	float comradeAlpha, comradeBeta;
	uint8_t RF2500_state;
	uint8_t comrade_data[RF2500_PACKET_LENGTH+2];
	
	uint8_t rssi;
	uint8_t crc_ok;
	

    uint32_t RF2500Timeout = RF2500_FLAG_TIMEOUT;

    /**
    * @brief  Basic management of the timeout situation.
    * @param  None.
    * @retval None.
    */
    uint32_t RF2500_TIMEOUT_UserCallback(void)
    {
      /* Block communication and all processes */
      while (1)
      {   
      }
    }

    /**
    * @brief Delays execution by (num_ticks) clock ticks.
    *
    * @param[in] num_ticks Number of clock ticks to delay execution by.
    */
    void delay(long num_ticks)
    {
        while(num_ticks-- > 0);
    }

    /**
    * @brief Converts the byte representation of a float
    * from -90 to 90 degrees to a float.
    *
    * @param[in] val	Value to convert to a float.
    *
    *@returns Floating point representation of degrees in (val).
    */
    float byteToDegrees(uint8_t val)
    {
    // 	if (val >= 128)
    // 	{
    // 		// Positive number
    // 		return (val - 128)*1.0f;
    // 	}
    // 	else
    // 	{
    // 		if (val == 0)
    // 		{
    // 			return 0;
    // 		}
    // 		else
    // 		{
    // 				// Negative number
    // 				return val*-1.0f;
    // 		}
    // 	}
        float deg = (val - 90);
        if (deg > 90.0f)
            return 90.0f;
        else if (deg < -90.0f)
            return -90.0f;
        
        return ((val - 90))*1.0f;
    }

    /**
    * @brief Converts a floating point degree value from -90 to 90
    * degrees to a byte.
    *
    * @param[in] val Degree value to convert to byte.
    *
    * @returns Byte representation of degrees in (val).
    */
    uint8_t degreesToByte(float val)
    {
        uint8_t ret;
    // 	if (val > 0)
    // 	{
    // 		// Map positive values to the upper half of the byte
    // 		ret = (uint8_t)(128 + floor(val));
    // 		if (ret > 218)
    // 		{
    // 			ret = 218;
    // 		}	
    // 	}
    // 	else
    // 	{
    // 		// Map negative numbers to lower half of the byte
    // 		ret = (uint8_t)(floor(val)*-1);
    // 		if (ret > 90)
    // 		{
    // 			ret = 90;
    // 		}
    // 	}
        ret = (uint8_t)(floor(val) + 90);
        return ret;
    }

    /**
      * @brief  Initializes the low level interface used to drive the RF2500
      * @param  None
      * @retval None
      */
    int RF2500_LowLevel_Init(void)
    {
       GPIO_InitTypeDef GPIO_InitStructure;
//       SPI_InitTypeDef  SPI_InitStructure;

//       /* Enable the SPI periph */
//       RCC_APB2PeriphClockCmd(RF2500_SPI_CLK, ENABLE);

//       /* Enable SCK, MOSI and MISO GPIO clocks */
//       RCC_AHB1PeriphClockCmd(RF2500_SPI_SCK_GPIO_CLK | RF2500_SPI_MISO_GPIO_CLK | RF2500_SPI_MOSI_GPIO_CLK, ENABLE);

//       /* Enable CS  GPIO clock */
//       RCC_AHB1PeriphClockCmd(RF2500_SPI_CS_GPIO_CLK, ENABLE);

//       GPIO_PinAFConfig(RF2500_SPI_SCK_GPIO_PORT, RF2500_SPI_SCK_SOURCE, RF2500_SPI_SCK_AF);
//       GPIO_PinAFConfig(RF2500_SPI_MISO_GPIO_PORT, RF2500_SPI_MISO_SOURCE, RF2500_SPI_MISO_AF);
//       GPIO_PinAFConfig(RF2500_SPI_MOSI_GPIO_PORT, RF2500_SPI_MOSI_SOURCE, RF2500_SPI_MOSI_AF);

//       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//       GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
//       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//       /* SPI SCK pin configuration */
//       GPIO_InitStructure.GPIO_Pin = RF2500_SPI_SCK_PIN;
//       GPIO_Init(RF2500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

//       /* SPI  MOSI pin configuration */
//       GPIO_InitStructure.GPIO_Pin =  RF2500_SPI_MOSI_PIN;
//       GPIO_Init(RF2500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

//       /* SPI MISO pin configuration */
//       GPIO_InitStructure.GPIO_Pin = RF2500_SPI_MISO_PIN;
//       GPIO_Init(RF2500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

//       /* SPI configuration -------------------------------------------------------*/
//       SPI_I2S_DeInit(RF2500_SPI);
//       SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//       SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//       SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//       SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//       SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//       SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
//       SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//       SPI_InitStructure.SPI_CRCPolynomial = 7;
//       SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//       SPI_Init(RF2500_SPI, &SPI_InitStructure);

//       /* Enable SPI1  */
//       SPI_Cmd(RF2500_SPI, ENABLE);

      /* Configure GPIO PIN for Lis Chip select */
      GPIO_InitStructure.GPIO_Pin = RF2500_SPI_CS_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_Init(RF2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

       /* Deselect : Chip Select high */
       GPIO_SetBits(RF2500_SPI_CS_GPIO_PORT, RF2500_SPI_CS_PIN);
         
         // Reset the chip
         RF2500_CS_LOW();
         delay(100);
         RF2500_CS_HIGH();
         delay(100);
         RF2500_CS_LOW();
         delay(150);
         
         // Send reset command
         RF2500_Strobe(CCxxx0_SRES); 
         RF2500_CS_HIGH();

         // Set to IDLE state
         RF2500_Strobe(CCxxx0_SIDLE);
        
         return RF2500_WriteRegisters();
    }



    int RF2500_Init(void)
    {
            return RF2500_LowLevel_Init();
    }

    /**
    * @brief Applies configuration settings to RF2500 peripheral.
    */
    int RF2500_WriteRegisters(void)
    {   
        // Send each setting for the RF2500 peripheral.
        RF2500_WriteRegister(CCxxx0_IOCFG0,SMARTRF_SETTING_IOCFG0D);      
        RF2500_WriteRegister(CCxxx0_PKTLEN,RF2500_PACKET_LENGTH);
        RF2500_WriteRegister(CCxxx0_PKTCTRL0,SMARTRF_SETTING_PKTCTRL0);    
        RF2500_WriteRegister(CCxxx0_ADDR,SMARTRF_SETTING_ADDR);        
        RF2500_WriteRegister(CCxxx0_CHANNR,SMARTRF_SETTING_CHANNR);      
        RF2500_WriteRegister(CCxxx0_FSCTRL1,SMARTRF_SETTING_FSCTRL1);     
        RF2500_WriteRegister(CCxxx0_FSCTRL0,SMARTRF_SETTING_FSCTRL0);     
        RF2500_WriteRegister(CCxxx0_FREQ2,SMARTRF_SETTING_FREQ2);      
        RF2500_WriteRegister(CCxxx0_FREQ1,SMARTRF_SETTING_FREQ1);       
        RF2500_WriteRegister(CCxxx0_FREQ0,SMARTRF_SETTING_FREQ0);      
        RF2500_WriteRegister(CCxxx0_MDMCFG4,SMARTRF_SETTING_MDMCFG4);     
        RF2500_WriteRegister(CCxxx0_MDMCFG3,SMARTRF_SETTING_MDMCFG3);    
        RF2500_WriteRegister(CCxxx0_MDMCFG2,SMARTRF_SETTING_MDMCFG2);    
        RF2500_WriteRegister(CCxxx0_MDMCFG1,SMARTRF_SETTING_MDMCFG1);      
        RF2500_WriteRegister(CCxxx0_MDMCFG0,SMARTRF_SETTING_MDMCFG0);     
        RF2500_WriteRegister(CCxxx0_DEVIATN,SMARTRF_SETTING_DEVIATN);
        RF2500_WriteRegister(CCxxx0_MCSM0,SMARTRF_SETTING_MCSM0);        
        RF2500_WriteRegister(CCxxx0_FOCCFG,SMARTRF_SETTING_FOCCFG);      
        RF2500_WriteRegister(CCxxx0_BSCFG,SMARTRF_SETTING_BSCFG);       
        RF2500_WriteRegister(CCxxx0_AGCCTRL2,SMARTRF_SETTING_AGCCTRL2);     
        RF2500_WriteRegister(CCxxx0_AGCCTRL1,SMARTRF_SETTING_AGCCTRL1);     
        RF2500_WriteRegister(CCxxx0_AGCCTRL0,SMARTRF_SETTING_AGCCTRL0);     
        RF2500_WriteRegister(CCxxx0_FREND1,SMARTRF_SETTING_FREND1); 
        RF2500_WriteRegister(CCxxx0_FREND0,SMARTRF_SETTING_FREND0);    
        RF2500_WriteRegister(CCxxx0_FSCAL3,SMARTRF_SETTING_FSCAL3);       
        RF2500_WriteRegister(CCxxx0_FSCAL2,SMARTRF_SETTING_FSCAL2);        
        RF2500_WriteRegister(CCxxx0_FSCAL1,SMARTRF_SETTING_FSCAL1);
        RF2500_WriteRegister(CCxxx0_FSCAL0,SMARTRF_SETTING_FSCAL0);         
        RF2500_WriteRegister(CCxxx0_FSTEST,SMARTRF_SETTING_FSTEST);        
        RF2500_WriteRegister(CCxxx0_FIFOTHR,SMARTRF_SETTING_FIFOTHR); 
        RF2500_WriteRegister(CCxxx0_IOCFG2,SMARTRF_SETTING_IOCFG2);    
        RF2500_WriteRegister(CCxxx0_PKTCTRL1,SMARTRF_SETTING_PKTCTRL1);         
        RF2500_WriteRegister(CCxxx0_MCSM1,SMARTRF_SETTING_MCSM1);	
        
        // Set sync word
        RF2500_WriteRegister(CCxxx0_SYNC1,RF2500_SYNCHRONIZATION_WORD);
        RF2500_WriteRegister(CCxxx0_SYNC0,RF2500_SYNCHRONIZATION_WORD);
        
        return RF2500_CheckRegisters();
    }

    /**
    * @brief Writes (value) field to (reg) register of RF2500 peripheral.
    *
    * @param[in] 	reg 	Register to write to.
    * @param[in]	value	Value to write to register.
    */
    void RF2500_WriteRegister(uint8_t reg, uint8_t value)
    {
		w_buff[0] = reg;  
		w_buff[1] = value; 
		SPI_DMA_simple(w_buff, r_buff, WIRELESS, 2);
	}

	/**
	SPI_DMA_xfer(w_buff, r_buff, WIRELESS_SPI_CS_GPIO_PORT , WIRELESS_SPI_CS_PIN , 2);    * @brief  Set RF2500 Initialization.
	*
    * @returns 1 on error; 0 otherwise.
    */
    int initRF2500(void)
    { 
        // Configure the low level interface (GPIO initialization of SPI)
         return RF2500_LowLevel_Init();
    }

    /**
    * @brief  Writes one byte to the RF2500 peripheral.
    * @param[in]    pBuffer : pointer to the buffer  containing the data to be written to the RF2500 peripheral.
    * @param[in]    WriteAddr : RF2500 peripheral's internal address to write to.
    * @param[in]    NumByteToWrite: Number of bytes to write.
    * @retval None
    */
    void RF2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) {
		if(NumByteToWrite > 0x01) {
			// Set Burst mode bit
			WriteAddr |= CCxxx0_WRITE_BURST ;
		}
 
		w_buff[0] = WriteAddr;
		for (int i = 0; i<NumByteToWrite; i++)
			w_buff[i+1] = pBuffer[i];
		
		SPI_DMA_simple(w_buff, r_buff, WIRELESS, NumByteToWrite + 1);	  			
    }

    /**
    * @brief  Reads a block of data from the RF2500 peripheral.
    * @param[out]   pBuffer : pointer to the buffer that receives the data read from the RF2500 peripheral.
    * @param[in]    ReadAddr : RF2500 peripheral's internal address to read from.
    * @param[in]    NumByteToRead : number of bytes to read from the RF2500 peripheral.
    * @retval       None
    */
    void RF2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
    {  
		// Configure Burst mode / header bits
		if(NumByteToRead > 0x01)
			ReadAddr |= CCxxx0_READ_BURST;
		else
			ReadAddr |= CCxxx0_READ_SINGLE;

        
		w_buff[0] = ReadAddr;
		w_buff[1] = 0x00;
				
		SPI_DMA_simple(w_buff, pBuffer, WIRELESS, NumByteToRead + 1);
    }

    /**
    * @brief Sends a command to the RF2500 peripheral.
    *
    * @param[in] CommandRegister The command to send.
    */
    uint8_t RF2500_Strobe(uint8_t CommandRegister)
    {
		w_buff[0] = CommandRegister;
		SPI_DMA_simple(w_buff, r_buff, WIRELESS, 1);
		return r_buff[1];
    }

    /**
    * @brief Checks to make sure (reg) has value (value) on RF2500 peripheral.
    *
    * @param[in]	reg		Register to check value.
    * @param[in]	value	Value to check for.
    *
    * @returns	int 1 if value is set; 0 otherwise.
    */
    int RF2500_CheckRegister(uint8_t reg, uint8_t value)
    {
		w_buff[0] = reg | READWRITE_CMD;
				
		SPI_DMA_simple(w_buff, r_buff, WIRELESS, 1 + 1);

        
            if (value != r_buff[1]) {
                return 0;
            }
            
            return 1;
    }

    /**
    * @brief Checks if the settings of the RF2500 peripheral are valid.
    *
    * @returns 1 if error, 0 otherwise.
    */
    int RF2500_CheckRegisters(void)
    {		
		// Check each setting individually
		if(RF2500_CheckRegister(CCxxx0_IOCFG0,SMARTRF_SETTING_IOCFG0D) && 
            RF2500_CheckRegister(CCxxx0_PKTLEN,RF2500_PACKET_LENGTH) &&
            RF2500_CheckRegister(CCxxx0_PKTCTRL0,SMARTRF_SETTING_PKTCTRL0) &&   
            RF2500_CheckRegister(CCxxx0_ADDR,SMARTRF_SETTING_ADDR) &&       
            RF2500_CheckRegister(CCxxx0_CHANNR,SMARTRF_SETTING_CHANNR) &&     
            RF2500_CheckRegister(CCxxx0_FSCTRL1,SMARTRF_SETTING_FSCTRL1) &&    
            RF2500_CheckRegister(CCxxx0_FSCTRL0,SMARTRF_SETTING_FSCTRL0) &&    
            RF2500_CheckRegister(CCxxx0_FREQ2,SMARTRF_SETTING_FREQ2) &&     
            RF2500_CheckRegister(CCxxx0_FREQ1,SMARTRF_SETTING_FREQ1) &&      
            RF2500_CheckRegister(CCxxx0_FREQ0,SMARTRF_SETTING_FREQ0) &&     
            RF2500_CheckRegister(CCxxx0_MDMCFG4,SMARTRF_SETTING_MDMCFG4) &&    
            RF2500_CheckRegister(CCxxx0_MDMCFG3,SMARTRF_SETTING_MDMCFG3) &&   
            RF2500_CheckRegister(CCxxx0_MDMCFG2,SMARTRF_SETTING_MDMCFG2) &&   
            RF2500_CheckRegister(CCxxx0_MDMCFG1,SMARTRF_SETTING_MDMCFG1) &&     
            RF2500_CheckRegister(CCxxx0_MDMCFG0,SMARTRF_SETTING_MDMCFG0) &&    
            RF2500_CheckRegister(CCxxx0_DEVIATN,SMARTRF_SETTING_DEVIATN) &&
            RF2500_CheckRegister(CCxxx0_MCSM0,SMARTRF_SETTING_MCSM0) &&       
            RF2500_CheckRegister(CCxxx0_FOCCFG,SMARTRF_SETTING_FOCCFG) &&     
            RF2500_CheckRegister(CCxxx0_BSCFG,SMARTRF_SETTING_BSCFG) &&      
            RF2500_CheckRegister(CCxxx0_AGCCTRL2,SMARTRF_SETTING_AGCCTRL2) &&    
            RF2500_CheckRegister(CCxxx0_AGCCTRL1,SMARTRF_SETTING_AGCCTRL1) &&    
            RF2500_CheckRegister(CCxxx0_AGCCTRL0,SMARTRF_SETTING_AGCCTRL0) &&    
            RF2500_CheckRegister(CCxxx0_FREND1,SMARTRF_SETTING_FREND1) &&
            RF2500_CheckRegister(CCxxx0_FREND0,SMARTRF_SETTING_FREND0) &&   
            RF2500_CheckRegister(CCxxx0_FSCAL3,SMARTRF_SETTING_FSCAL3) &&      
            RF2500_CheckRegister(CCxxx0_FSCAL2,SMARTRF_SETTING_FSCAL2) &&       
            RF2500_CheckRegister(CCxxx0_FSCAL1,SMARTRF_SETTING_FSCAL1) &&
            RF2500_CheckRegister(CCxxx0_FSCAL0,SMARTRF_SETTING_FSCAL0) &&        
            RF2500_CheckRegister(CCxxx0_FSTEST,SMARTRF_SETTING_FSTEST) &&       
            RF2500_CheckRegister(CCxxx0_FIFOTHR,SMARTRF_SETTING_FIFOTHR) &&
            RF2500_CheckRegister(CCxxx0_IOCFG2,SMARTRF_SETTING_IOCFG2) &&   
            RF2500_CheckRegister(CCxxx0_PKTCTRL1,SMARTRF_SETTING_PKTCTRL1) &&        
            RF2500_CheckRegister(CCxxx0_MCSM1,SMARTRF_SETTING_MCSM1))
        {
            // All settings are configured as expected.
            return 0;
        }	

        // Problem was found with at least one setting, so return error
        return 1;
    }

    /**
    * @brief Returns the current FIFO size of the RF2500 peripheral.
    *
    *	@param[in] reg	Register to read.
    *
    * @returns FIFO size of the RF2500 peripheral.
    */
    uint8_t RF2500_GetFifoSize(uint8_t reg)
    {
        uint8_t status;
        
        RF2500_Read(&status, reg, 1);
        
        return (status & 0x7F);
    }

    /**
    * @brief Returns the current status of the RF2500 peripheral.
    *
    * @returns Status of the RF2500 peripheral.
    */
    uint8_t RF2500_GetCurrentState(void)
    {
        return ((RF2500_Strobe(CCxxx0_SNOP) & 0x70) >> 4);
    }

    /**
    * @brief Receives the roll and pitch data using the RF2500 peripheral.
    */
    int receiveAccelData(uint32_t* accData)
    {
		uint8_t packet_read = 0;
        uint8_t fifoSize;
        //uint8_t trashData[10];
        
        // Enter RECEIVE MODE 
        RF2500_Strobe(CCxxx0_SRX);
        
        // Wait until mode changes
        while(RF2500_GetCurrentState() != CCxxx0_STATUS_RX);	

        // Count number of bytes in RX FIFO
        fifoSize = RF2500_GetFifoSize(CCxxx0_RXBYTES);
        
        // Make sure we have enough data in RX FIFO to read an entire packet
        if (fifoSize >= (RF2500_PACKET_LENGTH+2))
        {
            // Read a packet from the RF2500 peripheral
            RF2500_Read(comrade_data, CCxxx0_RXFIFO, RF2500_PACKET_LENGTH+2);		

            // Read rssi signal strength estimate
            rssi = comrade_data[RF2500_PACKET_LENGTH];
            
            // Check CRC OK flag
            crc_ok = (comrade_data[RF2500_PACKET_LENGTH + 1] & CCxxx0_CRC_OK == CCxxx0_CRC_OK);
            
            // We read an entire packet, check the check sum
            if (crc_ok)
            {
				accData[0] = ((uint32_t*)comrade_data)[0];
                // Valid data, so set roll and pitch    
                //comradeAlpha = byteToDegrees(comrade_data[0]);
                //comradeBeta = byteToDegrees(comrade_data[1]);
                packet_read = 1;
            }
            
            // Goto idle mode
            RF2500_Strobe(CCxxx0_SIDLE); 
            
            // Wait until mode changes
            while(RF2500_GetCurrentState() != CCxxx0_STATUS_IDLE);			
            
            // Flush RX buffer
            RF2500_Strobe(CCxxx0_SFRX); 
            
            // Return to RX mode
            RF2500_Strobe(CCxxx0_SRX); 
            
            // Wait until mode changes
            while(RF2500_GetCurrentState() != CCxxx0_STATUS_RX);					
        }
		return packet_read;
    }

    /**
    * @brief Transmit the roll and pitch data using the RF2500 peripheral.
    */
    void transmitAccelData(uint32_t* accData)
    {	
        // Enter idle mode
        RF2500_Strobe(CCxxx0_SIDLE);
        
        // Wait until idle mode is entered
        //while(RF2500_GetCurrentState() != CCxxx0_STATUS_IDLE);
        
        // Flush the TX buffer
        RF2500_Strobe(CCxxx0_SFTX);	

        // Write roll & pitch to FIFO register
        RF2500_Write((uint8_t*)accData, CCxxx0_TXFIFO, RF2500_PACKET_LENGTH);       	

        // Enable TX mode
        RF2500_Strobe(CCxxx0_STX);

        // Wait until TX mode is set
        //while(RF2500_GetCurrentState() != CCxxx0_STATUS_TX);
            
        // Wait for idle mode (TX just started) 
        // (config ensures transition from TX to IDLE after packet is sent)
        // This ensures packet was sent before leaving this routine
        //while(RF2500_GetCurrentState() != CCxxx0_STATUS_IDLE);	
    }

