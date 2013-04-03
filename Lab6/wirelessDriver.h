/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EZ430_RF2500T_H
#define __EZ430_RF2500T_H

/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"
   
/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)


//#define USE_DEFAULT_TIMEOUT_CALLBACK

#define RF2500_FLAG_TIMEOUT         ((uint32_t)0x1000)

/**
  * @brief  RF2500 SPI Interface pins
	* use same pins as LIS302DL accelerometer, only CS (chip select)
	* is different
  */
#define RF2500_SPI                       SPI1
#define RF2500_SPI_CLK                   RCC_APB2Periph_SPI1

#define RF2500_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define RF2500_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define RF2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define RF2500_SPI_SCK_SOURCE            GPIO_PinSource5
#define RF2500_SPI_SCK_AF                GPIO_AF_SPI1

#define RF2500_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.6 */
#define RF2500_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define RF2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define RF2500_SPI_MISO_SOURCE           GPIO_PinSource6
#define RF2500_SPI_MISO_AF               GPIO_AF_SPI1

#define RF2500_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.7 */
#define RF2500_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define RF2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define RF2500_SPI_MOSI_SOURCE           GPIO_PinSource7
#define RF2500_SPI_MOSI_AF               GPIO_AF_SPI1

#define RF2500_SPI_CS_PIN                GPIO_Pin_4                  /* PB.4 */
#define RF2500_SPI_CS_GPIO_PORT          GPIOA                       /* GPIOA */
#define RF2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOA


/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
// Settings
#define SMARTRF_SETTING_FSCTRL0 0x00			// Offset of 0
#define SMARTRF_SETTING_FREQ2 0x5D				// Frequency high byte 0x5D = 29
#define SMARTRF_SETTING_FREQ1 0x93				// Freq. middle byte 0x93 = 147
#define SMARTRF_SETTING_FREQ0 0xB1				// Freq. lower byte	0xB1 = 177
#define SMARTRF_SETTING_CHANNR 0x08				// Channel 1 (GROUP NUMBER) * 8
#define SMARTRF_SETTING_FREND1 0xB6				// Front end RX config
#define SMARTRF_SETTING_FREND0 0x10				// Front end TX config
#define SMARTRF_SETTING_MCSM0 0x18				// 0x18: autocal when going from idle to rx/tx; wait for PO
#define SMARTRF_SETTING_MCSM1 0x30				// 0x30: default (idle after rx/tx)
#define SMARTRF_SETTING_FOCCFG 0x1D				// Frequency offset
#define SMARTRF_SETTING_BSCFG 0x1C				// Bit synchronization config
#define SMARTRF_SETTING_AGCCTRL2 0xC7			// AGC control
#define SMARTRF_SETTING_AGCCTRL0 0xB0			// AGC control
#define SMARTRF_SETTING_FSCAL3 0xEA				// Freq. synth calibration settings
#define SMARTRF_SETTING_FSCAL2 0x0A				// Freq. " "
#define SMARTRF_SETTING_FSCAL1 0x00		
#define SMARTRF_SETTING_FSTEST 0x59				// For testing, 0x59: default value
#define SMARTRF_SETTING_TEST2 0x88				// 0x88: Default
#define SMARTRF_SETTING_TEST1 0x31				// 0x31: Default
#define SMARTRF_SETTING_TEST0 0x0B				// Test settings?
#define SMARTRF_SETTING_FIFOTHR 0x07			// 0x07: 32 byte for RX, 33 for TX
#define SMARTRF_SETTING_IOCFG2 0x29				// default: CHIP_RDYn
#define SMARTRF_SETTING_IOCFG0D 0x06			// Assert when sync word received
#define SMARTRF_SETTING_PKTCTRL1 0x04 		// 0x04 : No address check, append 2 status bytes on RX
#define SMARTRF_SETTING_PKTCTRL0 0x44			// 0x44: whitening, checksum, fixed length; 0x05 : No whitening, disable CC2400, CRC enabled, variable packet length 0x04 : fixed packet length
#define SMARTRF_SETTING_ADDR 0x00					// 0x00 address


#define SMARTRF_SETTING_FSCTRL1    0x0C
#define SMARTRF_SETTING_MDMCFG4    0x0E 
#define SMARTRF_SETTING_MDMCFG3    0x3B 
#define SMARTRF_SETTING_MDMCFG2    0x73 
#define SMARTRF_SETTING_MDMCFG1    0x42 
#define SMARTRF_SETTING_MDMCFG0    0xF8 
#define SMARTRF_SETTING_DEVIATN    0x00 
#define SMARTRF_SETTING_AGCCTRL1   0x40 
#define SMARTRF_SETTING_FSCAL0     0x19 

// Configuration Registers
#define CCxxx0_IOCFG2       0x00        // GDO2 output pin configuration
#define CCxxx0_IOCFG1       0x01        // GDO1 output pin configuration
#define CCxxx0_IOCFG0       0x02        // GDO0 output pin configuration
#define CCxxx0_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CCxxx0_SYNC1        0x04        // Sync word, high byte
#define CCxxx0_SYNC0        0x05        // Sync word, low byte
#define CCxxx0_PKTLEN       0x06        // Packet length
#define CCxxx0_PKTCTRL1     0x07        // Packet automation control
#define CCxxx0_PKTCTRL0     0x08        // Packet automation control
#define CCxxx0_ADDR         0x09        // Device address
#define CCxxx0_CHANNR       0x0A        // Channel number
#define CCxxx0_FSCTRL1      0x0B        // Frequency synthesizer control
#define CCxxx0_FSCTRL0      0x0C        // Frequency synthesizer control
#define CCxxx0_FREQ2        0x0D        // Frequency control word, high byte
#define CCxxx0_FREQ1        0x0E        // Frequency control word, middle byte
#define CCxxx0_FREQ0        0x0F        // Frequency control word, low byte
#define CCxxx0_MDMCFG4      0x10        // Modem configuration
#define CCxxx0_MDMCFG3      0x11        // Modem configuration
#define CCxxx0_MDMCFG2      0x12        // Modem configuration
#define CCxxx0_MDMCFG1      0x13        // Modem configuration
#define CCxxx0_MDMCFG0      0x14        // Modem configuration
#define CCxxx0_DEVIATN      0x15        // Modem deviation setting
#define CCxxx0_MCSM2        0x16        // Main Radio Cntrl State Machine config
#define CCxxx0_MCSM1        0x17        // Main Radio Cntrl State Machine config
#define CCxxx0_MCSM0        0x18        // Main Radio Cntrl State Machine config
#define CCxxx0_FOCCFG       0x19        // Frequency Offset Compensation config
#define CCxxx0_BSCFG        0x1A        // Bit Synchronization configuration
#define CCxxx0_AGCCTRL2     0x1B        // AGC control
#define CCxxx0_AGCCTRL1     0x1C        // AGC control
#define CCxxx0_AGCCTRL0     0x1D        // AGC control
#define CCxxx0_WOREVT1      0x1E        // High byte Event 0 timeout
#define CCxxx0_WOREVT0      0x1F        // Low byte Event 0 timeout
#define CCxxx0_WORCTRL      0x20        // Wake On Radio control
#define CCxxx0_FREND1       0x21        // Front end RX configuration
#define CCxxx0_FREND0       0x22        // Front end TX configuration
#define CCxxx0_FSCAL3       0x23        // Frequency synthesizer calibration
#define CCxxx0_FSCAL2       0x24        // Frequency synthesizer calibration
#define CCxxx0_FSCAL1       0x25        // Frequency synthesizer calibration
#define CCxxx0_FSCAL0       0x26        // Frequency synthesizer calibration
#define CCxxx0_RCCTRL1      0x27        // RC oscillator configuration
#define CCxxx0_RCCTRL0      0x28        // RC oscillator configuration
#define CCxxx0_FSTEST       0x29        // Frequency synthesizer cal control
#define CCxxx0_PTEST        0x2A        // Production test
#define CCxxx0_AGCTEST      0x2B        // AGC test

// Strobe commands
#define CCxxx0_SRES         0x30        // Reset chip.
#define CCxxx0_SFSTXON      0x31        // Enable/calibrate freq synthesizer
#define CCxxx0_SXOFF        0x32        // Turn off crystal oscillator.
#define CCxxx0_SCAL         0x33        // Calibrate freq synthesizer & disable
#define CCxxx0_SRX          0x34        // Enable RX.
#define CCxxx0_STX          0x35        // Enable TX.
#define CCxxx0_SIDLE        0x36        // Exit RX / TX
#define CCxxx0_SAFC         0x37        // AFC adjustment of freq synthesizer
#define CCxxx0_SWOR         0x38        // Start automatic RX polling sequence
#define CCxxx0_SPWD         0x39        // Enter pwr down mode when CSn goes hi
#define CCxxx0_SFRX         0x3A        // Flush the RX FIFO buffer.
#define CCxxx0_SFTX         0x3B        // Flush the TX FIFO buffer.
#define CCxxx0_SWORRST      0x3C        // Reset real time clock.
#define CCxxx0_SNOP         0x3D        // No operation.

// Status registers
#define CCxxx0_PARTNUM      (0x30 | 0x40)        // Part number
#define CCxxx0_VERSION      (0x31 | 0x40)        // Current version number
#define CCxxx0_FREQEST      (0x32 | 0x40)       // Frequency offset estimate
#define CCxxx0_LQI          (0x33 | 0x40)        // Demodulator estimate for link quality
#define CCxxx0_RSSI         (0x34 | 0x40)        // Received signal strength indication
#define CCxxx0_MARCSTATE    (0x35 | 0x40)        // Control state machine state
#define CCxxx0_WORTIME1     (0x36 | 0x40)        // High byte of WOR timer
#define CCxxx0_WORTIME0     (0x37 | 0x40)        // Low byte of WOR timer
#define CCxxx0_PKTSTATUS    (0x38 | 0x40)        // Current GDOx status and packet status
#define CCxxx0_VCO_VC_DAC   (0x39 | 0x40)        // Current setting from PLL cal module
#define CCxxx0_TXBYTES      (0x3A | 0x40)        // Underflow and # of bytes in TXFIFO
#define CCxxx0_RXBYTES      (0x3B | 0x40)        // Overflow and # of bytes in RXFIFO
#define CCxxx0_NUM_RXBYTES  (0x7F | 0x40)        // Mask "# of bytes" field in _RXBYTES

// Other memory locations
#define CCxxx0_PATABLE      0x3E
#define CCxxx0_TXFIFO       0x3F
#define CCxxx0_RXFIFO       0x3F

// Masks for appended status bytes
#define CCxxx0_LQI_RX       0x01        // Position of LQI byte
#define CCxxx0_CRC_OK       0x80        // Mask "CRC_OK" bit within LQI byte

// Definitions to support burst/single access:
#define CCxxx0_WRITE_BURST  0x40
#define CCxxx0_READ_SINGLE  0x80
#define CCxxx0_READ_BURST   0xC0

// Definitions of status codes
#define CCxxx0_STATUS_IDLE								0x0
#define CCxxx0_STATUS_RX									0x1
#define CCxxx0_STATUS_TX									0x2
#define CCxxx0_STATUS_FSTXON							0x3
#define CCxxx0_STATUS_CALIBRATE						0x4
#define CCxxx0_STATUS_SETTLING						0x5
#define CCxxx0_STATUS_RXFIFO_OVERFLOW			0x6
#define CCxxx0_STATUS_TXFIFO_UNDERFLOW		0x7


#define RF2500_CS_LOW()       GPIO_ResetBits(RF2500_SPI_CS_GPIO_PORT, RF2500_SPI_CS_PIN)
#define RF2500_CS_HIGH()      GPIO_SetBits(RF2500_SPI_CS_GPIO_PORT, RF2500_SPI_CS_PIN)

#define RF2500_MISO_STATE()   GPIO_ReadInputDataBit(RF2500_SPI_MISO_GPIO_PORT, RF2500_SPI_MISO_PIN)
/* Communication Constants */

/**
* @brief Synchronization word for synching TX/RX
*/
#define RF2500_SYNCHRONIZATION_WORD		(uint8_t)0x0F

/**
* @brief Number of bytes per packet
*	4 bytes for dummy, x,y,z
*/
#define RF2500_PACKET_LENGTH              (uint8_t)0x04

int RF2500_Init(void);

static uint8_t RF2500_SendByte(uint8_t byte);

void RF2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);

void RF2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

uint8_t RF2500_Strobe(uint8_t StrobeRegister);

void RF2500_WriteRegister(uint8_t reg, uint8_t value);

int RF2500_WriteRegisters(void);

int RF2500_CheckRegisters(void);

int RF2500_CheckRegister(uint8_t reg, uint8_t value);

uint8_t RF2500_GetCurrentState(void);

uint8_t RF2500_GetFifoSize(uint8_t reg);

uint32_t RF2500_TIMEOUT_UserCallback(void);

int receiveAccelData(uint8_t* accData);

void transmitAccelData(uint8_t* accData);

#endif
