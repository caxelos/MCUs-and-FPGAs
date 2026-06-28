/*
 * 003_buttonInterrupt.c
 *
 *  Created on: Jun 11, 2026
 *      Author: caxel
 */
    /*
     * GPIO outputs don't need pull-ups
     * The pin is never "floating", because:
     * 	HIGH → connected strongly to VCC internally.
     * 	LOW → connected strongly to GND internally.
     */
#include "stm32f407xx.h"
#include "STM32F407xx_gpio_driver.h"
#include <string.h>

#define HIGH 1
#define LOW 0
#define BTN_PRESSED LOW // In case of Nucleo board, PRESSED is "LOW". For discovery board, it's HIGH

void delay(void)
{	// 200ms of delay
    for(uint32_t i = 0; i < 600000; i++);
    	//for(uint32_t j = 0; j < 6000; j++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioBtn;

    memset(&GpioLed, 0, sizeof(GpioLed));// before initializing, clear all the members to zero!!! otherwise, you'll have corrupted data
    memset(&GpioBtn, 0, sizeof(GpioBtn)); //string.h

    /*
     * this is led gpio configuration (we use the internal pull up for the led)
     */
    GpioLed.pGPIOx = GPIOA; // GPIOA for the discovery board
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6; // GPIO_PIN_NO_12 for discovery board, for my board, it's PA5
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // Configurations: 1) OD: OpenDrain, 2) PP: PushPull
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;//GPIO_PIN_PU;// GPIO_NO_PUPD;	// Configurations: "PU"PD", Pull-Up & Pull-Down

    GPIO_PeriClockControl( GPIOA, ENABLE); //GPIOD for discovery board
    GPIO_Init(&GpioLed);
    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_6, GPIO_PIN_RESET); // reset LED's pin status

    /*
     * this is btn gpio configuration (we use external pull up for the button)
     */
    GpioBtn.pGPIOx = GPIOA; // GPIOD for the discovery board
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;// GPIO_PIN_NO_3; // GPIO_PIN_NO_12 for discovery board, for my board, it's PA5
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;//GPIO_MODE_IT_FT; //change the mode from MODE_IN to MODE_IT_FT (Interrupt Falling Edge)
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    //GpioBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // This doesn't matter for Inputs, only for outputs
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	// I changed this to Pull-UP Because, if we look the schematic of button, we already have an 220K external resistor.

    GPIO_PeriClockControl( GPIOA, ENABLE); //GPIOD for discovery board
    GPIO_Init(&GpioBtn);

    //IRQ configurations for button interrupt
    GPIO_IRQPriorityConfig( (uint8_t )IRQ_NO_EXTI0, NVIC_IRQ_PRIO15); // configure priority, set it to 15 (any value from 0 to 15). Better give macro
    GPIO_IRQInterruptConfig((uint8_t )IRQ_NO_EXTI0, ENABLE); // PD5 sends it's interrupt into EXTI5

    while (1);

    return 0;
}

/* EXTI Line[9:5] interrupts */
void EXTI0_IRQHandler(void)  // Mapping between Interrupt function name and pin number can be found at the MCU's datasheet document.
{
	//delay(); //200ms . wait till button de-bouncing gets over
	GPIO_IRQHandling(GPIO_PIN_NO_0); // clear the pending event from exti line (Button Interrupt)
	GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_6); // toggle the LED status (0 to 1 and vice versa)
}



/*
 * 	- Troubleshooting in case of error
 * 	1) Check whether the peripheral clock is enabled (works, because we enter interrupt)
 * 	2) MORE DEBUGGING, VIDEO 115
 */
