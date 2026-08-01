/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: Jul 31, 2026
 *      Author: caxel
 *
 *
 * Exercise: I2C Master-Slave Communication
 *
 * Master: STM32 Discovery
 * Slave:  Arduino board
 *
 * Description:
 * When the button on the STM32 board (master) is pressed,
 * the master sends data to the Arduino board (slave).
 *
 * The data received by the Arduino is displayed on the
 * Serial Monitor of the Arduino IDE.
 *
 * Requirements:
 * - I2C SCL frequency: 100 kHz (Standard Mode)
 * - Use external 3.3 kOhm pull-up resistors on SDA and SCL
 * - SCL on PB6, SDA on PB9
 *
 * Note:
 * If external pull-up resistors are not available, the STM32
 * I2C pins' internal pull-up resistors can be enabled instead.
 *
 * On the STM32 NUCLEO-F411RE:

	🔵 Blue button = B1 (USER button) → connected to PC13. This is the button you use in your programs. <--- THIS BUTTON WE USE
	⚫ Black button = B2 (RESET button) → connected to NRST. Pressing it resets the microcontroller; it is not a GPIO input.
 *
 *
 */


#include<stdio.h>
#include<string.h>

#include "STM32F407xx_i2c_driver.h"
#include "STM32F407xx_gpio_driver.h"

#define MY_ADDR 0x61;

#define SLAVE_ADDR  0x68

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

I2C_Handle_t I2C1Handle;

//some data
//The Arduino sketch is written using Arduino Wire library.
// The wire library has limitation on how many bytes can be transferred or received in single I2C transaction and the limit is 32 bytes.
// So, don’t send/receive more than 32 bytes in single I2C transaction. You may split into multiple I2C transactions.
uint8_t some_data[] = "We are testing I2C master Tx\n";
/*
 * PB6-> SCL
 * PB9 or PB7 -> SDA
 */

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins; // TODO: memset the struct to zero

	/*Note : Internal pull-up resistors are used */

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	/*
	 * Note : In the below line use GPIO_NO_PUPD option if you want to use external pullup resistors, then you have to use 3.3K pull up resistors
	 * for both SDA and SCL lines
	 */
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins. GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//scl
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);


	//sda
	//Note : since we found a glitch on PB9 , you can also try with PB7
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;

	GPIO_Init(&I2CPins);


}

void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);

}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOC;//GPIOA;	// PC13 for Nucleo, PA0 for Discovery
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;//GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; // // Enable internal pull-up resistor


    GPIO_PeriClockControl( GPIOC, ENABLE); //GPIOD for discovery board
    GPIO_Init(&GPIOBtn);

}


int main(void)
{

	GPIO_ButtonInit();

	//i2c pin inits
	I2C1_GPIOInits();

	//i2c peripheral configuration
	I2C1_Inits();

	//enable the i2c peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	while(1)
	{
		//wait till button is pressed (while( ! GPIO_ReadFromInputPin(GPIOC,GPIO_PIN_NO_13) );)
		while( ! GPIO_ReadFromInputPin(GPIOC,GPIO_PIN_NO_13) ); //while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) ); // instead of PA0, I'll use the PC13 pin

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		//send some data to the slave
		I2C_MasterSendData(&I2C1Handle,some_data,strlen((char*)some_data),SLAVE_ADDR,0);
	}

}
