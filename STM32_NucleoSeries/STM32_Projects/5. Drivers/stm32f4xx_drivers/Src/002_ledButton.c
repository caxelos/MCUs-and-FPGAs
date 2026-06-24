/*
 * 002_ledButton.c
 *
 *  Created on: Jun 11, 2026
 *      Author: caxel
 */

#include "stm32f407xx.h"
#include "STM32F407xx_gpio_driver.h"

#define HIGH 1
#define LOW 0
#define BTN_PRESSED LOW // In case of Nucleo board, PRESSED is "LOW". For discovery board, it's HIGH

void delay(void)
{
    for(uint32_t i = 0; i < 500000; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioBtn;


    // LED
    GpioLed.pGPIOx = GPIOA; // GPIOD for the discovery board
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5; // GPIO_PIN_NO_12 for discovery board, for my board, it's PA5
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // Configurations: 1) OD: OpenDrain, 2) PP: PushPull
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;// GPIO_NO_PUPD;	// Configurations: "PU"PD", Pull-Up & Pull-Down

    GPIO_PeriClockControl( GPIOA, ENABLE); //GPIOD for discovery board
    GPIO_Init(&GpioLed);

    // BUTTON
    GpioBtn.pGPIOx = GPIOC; // GPIOD for the discovery board
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13; // GPIO_PIN_NO_12 for discovery board, for my board, it's PA5
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    //GpioBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // This doesn't matter for Inputs, only for outputs
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;	// Because, if we look the schematic of button, we already have an 220K external resistor.

    GPIO_PeriClockControl( GPIOC, ENABLE); //GPIOD for discovery board
    GPIO_Init(&GpioBtn);

    while(1)
    {
    	if ( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == BTN_PRESSED )
    	{
    		delay();
    		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);//GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
    	}
    }

    return 0;
}
