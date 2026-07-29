/*
 * 006spi_tx_testing.c

Exercise:

Test the SPI_SendData API to send the string "Hello world" and use the below configurations:
SPI-2 Master mode
SCLK = max possible
DFF = 0 and DFF = 1
 */

#include <stdio.h>
#include <string.h>
#include "STM32F407xx_gpio_driver.h"
#include "STM32F407xx_spi_driver.h"


/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 *
 * SPI1:
 * 	CLK  ->	PB3
 *  MOSI ->	PB5
 */


/*
 * Initialize Pins in order to behave as SPI peripheral pins
 */
void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

    memset(&SPIPins, 0, sizeof(SPIPins));// before initializing, clear all the members to zero!!! otherwise, you'll have corrupted data

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN; // To alternate mode, because we are using the pins for the SPI functionality
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5; // SPI 1 & 2: 5; // Datasheet, Table 9. Alternate function mapping
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // For I2C, it's OpenDrain, but for SPI, we can use Push-Pull
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; // Not required
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; // It doesn't matter what you set

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13; //SP1: GPIO_PIN_NO_3;//SPI2: GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15; //SPI1: GPIO_PIN_NO_5;//SPI2: GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//NSS
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPins); // check bit SSOE in SPIx_CR1
}


/*
 *
 */
void SPI2_Inits(void)
{

	SPI_Handle_t SPI2handle;
	memset(&SPI2handle, 0, sizeof(SPI2handle));// before initializing, clear all the members to zero!!! otherwise, you'll have corrupted data

	SPI2handle.pSPIx = SPI2; //SPI3; //SPI1
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;		// let's use full duplex
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;	// our application is behaving as Master (we don't have any slaves, just test our send API)
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;//SPI_SCLK_SPEED_DIV2;// SPI_SCLK_SPEED_DIV128; //generates sclk of 8MHz (prescaler=2)
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL =  SPI_CPOL_LOW;//SPI_CPOL_HIGH; // SPI_CPOL_LOW; idle state of CLK
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW; // determines which clock edge samples the data (0 = 1st edge, 1 = 2nd edge)
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN; //software slave management enabled for NSS pin (no slave for our application so we don't need NSS. So, let's enable SSM)

	SPI_Init(&SPI2handle);
}


int main(void) {
	char user_data[] = "Hello world";

	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	//This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	//this makes NSS signal internally high and avoids MODF error
	SPI_SSIConfig(SPI2,ENABLE);

	//enable the SPI2 peripheral (although we finished inits, SPI is not yet enabled. We need to enable SPE bit also)
	SPI_PeripheralControl(SPI2,ENABLE);

	SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data) ); // it takes the peripheral base addres and data

	//lets confirm SPI is not busy
	while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );

	// disable the SPI2 peripheral
	SPI_PeripheralControl(SPI2, DISABLE);

	while (1);

	return 0;
}
