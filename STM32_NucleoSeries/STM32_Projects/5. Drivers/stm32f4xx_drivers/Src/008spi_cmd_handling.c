/*
 * 006spi_tx_testing.c
 *
 * Exercise: SPI Master (STM) and SPI Slave (Arduino)
 *           Command & Response Based Communication
 *
 * Description:
 * When the button on the master is pressed, the master sends a command
 * to the slave, and the slave responds according to the command
 * implementation.
 *
 * SPI Configuration:
 * 1. Use SPI in Full-Duplex mode.
 * 2. STM board operates in SPI Master mode.
 *    Arduino is configured in SPI Slave mode.
 * 3. Use DFF = 0 (8-bit data frame).
 * 4. Use Hardware Slave Management (SSM = 0).
 * 5. SCLK speed = 2 MHz, fCLK = 16 MHz.
 *
 * Command/Response Protocol:
 *
 *   MASTER                                      SLAVE
 *     |                                           |
 *     | -------- Send Command ------------------> |
 *     |                                           |
 *     | <------- ACK / NACK --------------------- |
 *     |                                           |
 *     |          ACK  = 0xF5                      |
 *     |          NACK = 0xA5                      |
 *     |                                           |
 *     | -- Command Argument(s), if ACK ---------> |
 *     |                                           |
 *     |                                  Take action
 *     |                                  according to
 *     |                                  the command
 *     |                                           |
 *     | <---- Response Data (for read command) -- |
 *     |                                           |
 *
 * Communication Sequence:
 *
 * 1. Master sends a command byte to the slave.
 *
 * 2. Slave validates the received command and responds with:
 *      ACK  (0xF5) -> Command is valid/supported.
 *      NACK (0xA5) -> Command is invalid/unsupported.
 *
 * 3. If ACK is received:
 *      - Master sends one or more command arguments, if required.
 *      - Slave takes the appropriate action according to the command.
 *
 * 4. If NACK is received:
 *      - Master displays/reports an error message.
 *      - No command arguments are sent.
 *
 * 5. For a data-read command:
 *      - After processing the command, the slave sends the requested
 *        data back to the master.
 *
 * Note:
 * SPI is full-duplex, so transmission and reception occur simultaneously.
 * The master must always generate the clock (SCLK), including when reading
 * ACK/NACK or response data from the slave.
 *
 * * Using semihosting
 * =================
 *
 * 1. Linker argument settings:
 *    -specs=rdimon.specs -lc -lrdimon
 *
 * 2. Debug configuration of your application:
 *    monitor arm semihosting enable
 *
 * 3. In main.c, use the following code:
 * 	"extern void initialise_monitor_handles(void);"
 */

#include <string.h>
#include <stdio.h>

#include "STM32F407xx_gpio_driver.h"
#include "STM32F407xx_spi_driver.h"

extern void initialise_monitor_handles();

//command codes
#define COMMAND_LED_CTRL      		0x50
#define COMMAND_SENSOR_READ      	0x51
#define COMMAND_LED_READ      		0x52
#define COMMAND_PRINT      			0x53
#define COMMAND_ID_READ      		0x54

#define LED_ON     1
#define LED_OFF    0

#define ACK 1
#define NACK 0

//arduino analog pins
#define ANALOG_PIN0 	0
#define ANALOG_PIN1 	1
#define ANALOG_PIN2 	2
#define ANALOG_PIN3 	3
#define ANALOG_PIN4 	4

//arduino led

#define LED_PIN  9

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i ++);
}

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

/* SPI Slave Demo

 *
 * SPI pin numbers:
 * SCK   13  // Serial Clock.
 * MISO  12  // Master In Slave Out.
 * MOSI  11  // Master Out Slave In.
 * SS    10  // Slave Select . Arduino SPI pins respond only if SS pulled low by the master
 *

 */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{

	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; //Hardware slave management enabled for NSS pin

	SPI_Init(&SPI2handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

}

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{

	if(ackbyte == (uint8_t)0xF5)
	{

		return ACK;
	}

	return NACK;
}

/*
 * SPI Function prototypes
 */
static void WaitForButtonPress(void);

static uint8_t SPI_SendCommand(uint8_t commandcode);
static uint8_t SPI_ReadByte(void);

static void SPI_CommandLedCtrl(void);
static void SPI_CommandSensorRead(void);
static void SPI_CommandLedRead(void);
static void SPI_CommandPrint(void);
static void SPI_CommandIdRead(void);



/* Execution Flow:
main()
    │
    ├── WaitForButtonPress()
    │
    ├── SPI_CommandLedCtrl()
    ├── SPI_CommandSensorRead()
    ├── SPI_CommandLedRead()
    ├── SPI_CommandPrint()
    └── SPI_CommandIdRead()
              │
              ├── SPI_SendCommand()
              └── SPI_ReadByte()

*/
int main(void)
{
	initialise_monitor_handles();

	printf("Application is running\n");	// when using printf() in semi hosting, the printf() arguments should be terminated by '\n'

	GPIO_ButtonInit();

	SPI2_GPIOInits(); //this function is used to initialize the GPIO pins to behave as SPI2 pins

	SPI2_Inits(); //This function is used to initialize the SPI2 peripheral parameters

	printf("SPI init done!\n");
	/*
	 * making SSOE 1 does NSS output enable.
	 * The NSS pin is automatically managed by the hardware.
	 * i.e when SPE=1 , NSS will be pulled to low
	 * and NSS pin will be high when SPE=0
	 */
	SPI_SSOEConfig(SPI2, ENABLE);


	while(1)
	{
		/*
		 * 1. begin of command: CMD_LED_CTRL  	<pin no(1)>     <value(1)>
		 */
		WaitForButtonPress();

		SPI_PeripheralControl(SPI2, ENABLE); //enable the SPI2 peripheral

		SPI_CommandLedCtrl();
		/*
		 * end of COMMAND_LED_CTRL
		 */


		/*
		 * 2. begin of command CMD_SENSOR_READ   <analog pin number(1)>
		 */
		WaitForButtonPress(); //wait till button is pressed
		SPI_CommandSensorRead();
		/*
		 * 2. end of command CMD_SENSOR_READ   <analog pin number(1)>
		 */


		/*
		 * 3. begin of command CMD_LED_READ 	 <pin no(1)>
		 */
		WaitForButtonPress(); //wait till button is pressed
		SPI_CommandLedRead();
		/*
		 * end of command CMD_LED_READ 	 <pin no(1)>
		 */


		/*
		 * 4. begin of command CMD_PRINT 		<len(1)>  <message(len)>
		 */
		WaitForButtonPress(); //wait till button is pressed
		SPI_CommandPrint();
		/*
		 * end of command CMD_PRINT 		<len(1)>  <message(len)>
		 */


		/*
		 * 5. begin of command CMD_ID_READ
		 */

		WaitForButtonPress(); //wait till button is pressed
		SPI_CommandIdRead();
		/*
		 * end of command CMD_ID_READ
		 */


		while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) ); //lets confirm SPI is not busy

		SPI_PeripheralControl(SPI2, DISABLE); //Disable the SPI2 peripheral

		printf("SPI Communication Closed\n");
	}

	return 0;
}


/*
 * =============================================================
 * WAIT FOR BUTTON PRESS
 * =============================================================
 *
 * This code was repeated before every command, so it is now
 * placed inside one function.
 */
static void WaitForButtonPress(void)
{

	while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) ); //wait till button is pressed

	delay(); //to avoid button de-bouncing related issues 200ms of delay
}


/*
 * =============================================================
 * SEND COMMAND AND RECEIVE ACK/NACK
 * =============================================================
 *
 * This is the common part used by all the commands.
 *
 * 	DUMMY READ TO CLEAR THE RXNE
 * 	- In SPI communication, when Master or Slave sends 1 byte, it also receives 1 byte in return
 *  - The transmission of the previous 1 byte resulted 1 garbage byte collection in Rx buffer of the master and RXNE
 *    flag is set.
 *  - So we need to do the dummy read 1 byte and also clear the RXNE flag
 *  - And when we read data register, RXNE is automatically cleared
 *
 *
 *	SPI_SendData()
 *	  - send command()
 *    - when slave receives this, it will check if this command is supported or not
 *    - if slave supports the command, it will reply with ACK (else, NACK)
 *
 *	SPI_ReceiveData()
 * 	  - Read data that arrive at the master
 * 	  - When SPI_ReceiveData() API call returns response from the slave would have arrived at the master. so let's read next
 *
 * 	SPI_VerifyResponse()
 *	  - Return the result of the ACK/NACK verification.
 *	  1 -> ACK
 *	  0 -> NACK
 */
static uint8_t SPI_SendCommand(uint8_t commandcode)
{
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;
	uint8_t ackbyte;

	SPI_SendData(SPI2, &commandcode, 1);
	SPI_ReceiveData(SPI2, &dummy_read, 1);

	SPI_SendData(SPI2, &dummy_write, 1); //Send some dummy bits (1 byte) fetch the response from the slave
	SPI_ReceiveData(SPI2, &ackbyte, 1); //read the ack byte received

	return SPI_VerifyResponse(ackbyte);
}


/*
 * =============================================================
 * READ ONE BYTE FROM THE SLAVE
 * =============================================================
 *
 * In SPI communication the master has to generate the clock
 * in order to receive data from the slave.
 *
 * So we send one dummy byte and receive one real byte.
 */
static uint8_t SPI_ReadByte(void)
{
	uint8_t dummy_write = 0xff;
	uint8_t data;

	SPI_SendData(SPI2, &dummy_write, 1); //Send some dummy bits (1 byte) fetch the response from the slave

	SPI_ReceiveData(SPI2, &data, 1); //read the data received from the slave

	return data;
}


/*
 * =============================================================
 * 1st command
 *
 * CMD_LED_CTRL  	<pin no(1)>     <value(1)>
 * =============================================================
 */
static void SPI_CommandLedCtrl(void)
{
	uint8_t args[2];


	/*
	 * send command()
	 * when slave receives this, it will check if this command is supported or not
	 * if slave supports the command, it will reply with ACK (else, NACK)
	 *
	 * SPI_SendCommand() now performs this common sequence.
	 */
	if( SPI_SendCommand(COMMAND_LED_CTRL) == ACK )
	{

		//if response is ACK, then only send the arguments
		args[0] = LED_PIN;	// connect LED to 9th pin of Arduino
		args[1] = LED_ON;

		//send arguments
		SPI_SendData(SPI2, args, 2); // 2 bytes
		SPI_ReceiveData(SPI2, args, 2);

		printf("COMMAND_LED_CTRL Executed\n");
	}

	//end of COMMAND_LED_CTRL
}


/*
 * =============================================================
 * 2nd command
 *
 * CMD_SENSOR_READ   <analog pin number(1)>
 * =============================================================
 */
static void SPI_CommandSensorRead(void)
{
	uint8_t dummy_read;
	uint8_t args[1];
	uint8_t analog_read;


	/*
	 * send command
	 *
	 * SPI_SendCommand() also does the dummy read,
	 * sends the dummy byte to fetch the response,
	 * reads the ACK byte and verifies the response.
	 */
	if( SPI_SendCommand(COMMAND_SENSOR_READ) == ACK )
	{
		args[0] = ANALOG_PIN0; //send arguments

		SPI_SendData(SPI2, args, 1); //sending one byte
		SPI_ReceiveData(SPI2, &dummy_read, 1); //do dummy read to clear off the RXNE

		delay(); //insert some delay so that slave can ready with the data
		analog_read = SPI_ReadByte(); //Send some dummy bits (1 byte) fetch the response from the slave

		printf("COMMAND_SENSOR_READ %d\n", analog_read);
	}
}


/*
 * =============================================================
 * 3. CMD_LED_READ 	 <pin no(1)>
 * =============================================================
 */
static void SPI_CommandLedRead(void)
{
	uint8_t dummy_read;
	uint8_t args[1];
	uint8_t led_status;


	/*
	 * Send the command and check the response from the slave.
	 *
	 * SPI_SendCommand() performs:
	 *
	 * send command
	 * do dummy read to clear off the RXNE
	 * send dummy byte to fetch response from slave
	 * read the ack byte received
	 */
	if( SPI_SendCommand(COMMAND_LED_READ) )
	{
		args[0] = LED_PIN;


		//send arguments
		SPI_SendData(SPI2, args, 1); //sending one byte
		SPI_ReceiveData(SPI2, &dummy_read, 1); //do dummy read to clear off the RXNE

		delay(); //insert some delay so that slave can ready with the data

		led_status = SPI_ReadByte(); //Send some dummy bits (1 byte) fetch the response from the slave

		printf("COMMAND_READ_LED %d\n", led_status);
	}
}


/*
 * =============================================================
 * 4. CMD_PRINT 		<len(1)>  <message(len)>
 * =============================================================
 */
static void SPI_CommandPrint(void)
{
	uint8_t dummy_read;
	uint8_t args[1];

	uint8_t message[] = "Hello ! How are you ??";


	/*
	 * Send the command and check if the slave replies with ACK.
	 */
	if( SPI_SendCommand(COMMAND_PRINT) )
	{

		args[0] = strlen((char*)message); // Store the message length in args[0].


		//send arguments
		SPI_SendData(SPI2, args, 1); //sending length
		SPI_ReceiveData(SPI2, &dummy_read, 1); //do dummy read to clear off the RXNE

		delay();


		/*
		 * send message
		 *
		 * The message is sent one byte at a time.
		 *
		 * In SPI communication, when the Master sends 1 byte,
		 * it also receives 1 byte in return.
		 *
		 * Since we don't need the received byte here, we read
		 * it into dummy_read to clear the RXNE flag.
		 */
		for(int i = 0; i < args[0]; i++)
		{
			SPI_SendData(SPI2, &message[i], 1);

			SPI_ReceiveData(SPI2, &dummy_read, 1);
		}


		printf("COMMAND_PRINT Executed \n");
	}
}


/*
 * =============================================================
 * 5. CMD_ID_READ
 * =============================================================
 */
static void SPI_CommandIdRead(void)
{
	uint8_t id[11];
	uint32_t i = 0;


	/*
	 * Send the command and check if the slave replies with ACK.
	 */
	if( SPI_SendCommand(COMMAND_ID_READ) )
	{

		for(i = 0; i < 10; i++) //read 10 bytes id from the slave
		{
			/*
			 * send dummy byte to fetch data from slave
			 *
			 * SPI_ReadByte() now does both:
			 *
			 * SPI_SendData(SPI2,&dummy_write,1);
			 * SPI_ReceiveData(SPI2,&id[i],1);
			 */
			id[i] = SPI_ReadByte();
		}

		id[10] = '\0'; // Add null character at the end because the ID will be printed as a string using %s

		printf("COMMAND_ID : %s \n", id);
	}
}
