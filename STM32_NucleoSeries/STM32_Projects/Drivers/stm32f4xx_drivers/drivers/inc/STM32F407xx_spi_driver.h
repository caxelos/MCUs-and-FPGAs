/*
 * STM32F407xx_spi_driver.h
 *
 *  Created on: Jun 30, 2026
 *      Author: caxel
 *      vIDEO 135
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_ // these guards actually prevent inclusion of multiple definitions during the preprocessor stage of the compilation
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"

/*
 *  Configuration structure for SPIx peripheral
 */
typedef struct
{
	uint8_t SPI_DeviceMode;	  // configure the mode either MASTER or SLAVE
	uint8_t SPI_BusConfig;   // decide whether the bus should be full-duplex, half-duplex or simplex
	uint8_t SPI_SclkSpeed;  // check reference manual (28.5.1, "SPI ontrol register 1 (SPI_CR1, bits 5 to 3 "Baud Rate") to see speeds (for SPI peripheral) - Periph. Clock: 16MHz
	uint8_t SPI_DFF;		// Data frame format (0 = 8-bit data frame format for transmission/reception or 1 = 16-bi data frame)
	uint8_t SPI_CPOL;		// sets the idle state of the clock (SCK).
	uint8_t SPI_CPHA;		// determines which clock edge samples the data.
	uint8_t SPI_SSM;		// SW slave management; 0 = disabled, 1 = enabled
}SPI_Config_t;



/*
 *Handle structure for SPIx peripheral
 */
typedef struct
{
	SPI_RegDef_t 	*pSPIx;   /*!< This holds the base address of SPIx(x:0,1,2) peripheral >*/
	SPI_Config_t 	SPIConfig;
	uint8_t 		*pTxBuffer; /* !< To store the app. Tx buffer address > */
	uint8_t 		*pRxBuffer;	/* !< To store the app. Rx buffer address > */
	uint32_t 		TxLen;		/* !< To store Tx len > */
	uint32_t 		RxLen;		/* !< To store Tx len > */
	uint8_t 		TxState;	/* !< To store Tx state > */
	uint8_t 		RxState;	/* !< To store Rx state > */
}SPI_Handle_t;



/*
 * SPI application states
 */
#define SPI_READY 					0
#define SPI_BUSY_IN_RX 				1
#define SPI_BUSY_IN_TX 				2


/*
 * Possible SPI Application events
 */
#define SPI_EVENT_TX_CMPLT   1
#define SPI_EVENT_RX_CMPLT   2
#define SPI_EVENT_OVR_ERR    3
#define SPI_EVENT_CRC_ERR    4
// TODO: MODF EVENT

/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER    1
#define SPI_DEVICE_MODE_SLAVE     0


/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD                1	// Full Duplex
#define SPI_BUS_CONFIG_HD                2  // Half Duplex
#define SPI_BUS_CONFIG_SIMPLEX_TXONLY    3
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY    4


/*
 * @SPI_SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2             	0
#define SPI_SCLK_SPEED_DIV4             	1
#define SPI_SCLK_SPEED_DIV8             	2
#define SPI_SCLK_SPEED_DIV16             	3
#define SPI_SCLK_SPEED_DIV32             	4
#define SPI_SCLK_SPEED_DIV64             	5
#define SPI_SCLK_SPEED_DIV128             	6
#define SPI_SCLK_SPEED_DIV256             	7


/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS 	0
#define SPI_DFF_16BITS  1

/*
 * @CPOL
 */
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0

/*
 * @CPHA
 */
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0

/*
 * @SPI_SSM
 */
#define SPI_SSM_EN     1		// By default, the SW slave management will be active
#define SPI_SSM_DI     0


/*
 * @CPOL
 */
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0


/*
 * SPI related status flags definitions
 */
#define SPI_TXE_FLAG    ( 1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG   ( 1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG   ( 1 << SPI_SR_BSY)

/*****************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx); // pointer to the base address of SPI peripheral



/*
 * In communication related to peripherals such as SPI, I2C, CAN or USB, we may have 3 different types of data transmitting or receiving methodologies
 * - Pooling based, non-blocking type, DMA-based API (we'll not implement this now)
 */


/*
 * Data Send and Receive to/from external world
 * 2nd  pointer (uint8 *) is a user provided pointer
 * 3rd para is the size of the transfer (always but it uint32)
 * IT = Interrupt Based
 * 	Instead of using the "SPI_RegDef_t *", let's use the "SPI_Handle_t *" structure
 */

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len); //

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi); // video 113
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
//uint8_t I2C_DeviceMode(I2C_RegDef_t *I2Cx);


/*
 * Application callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv);

#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
