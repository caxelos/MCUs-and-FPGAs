
/* VIDEO 102, 103,
 * Toggle the on board LED with some delay

	Case 1: Use push pull configuration for the output pin
	Case 2: Use open drain configuration for the output pin

	Important: The STM32F4 Discovery board uses LEDs on PD12–PD15.
	Your NUCLEO-F411RE does not have those LEDs. Nucleo uses PA5 and Discovery-board tutorial PD12 for LED2.


	- If we use Open Drain pin configuration, don't use internal Pull resistor, but an 320/470Ohm external one!and then send it to Vcc (+5V)
 */
#include "stm32f407xx.h"
#include "STM32F407xx_gpio_driver.h"

void delay(void)
{
    for(uint32_t i = 0; i < 500000; i++);
}


int main(void)
{
    GPIO_Handle_t GpioLed;

    // LED
    GpioLed.pGPIOx = GPIOA; // GPIOD for the discovery board
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5; // GPIO_PIN_NO_12 for discovery board, for my board, it's PA5
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // Configurations: 1) OD: OpenDrain, 2) PP: PushPull
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;// GPIO_NO_PUPD;	// Configurations: "PU"PD", Pull-Up & Pull-Down

    GPIO_PeriClockControl( GPIOA, ENABLE); //GPIOD for discovery board
    GPIO_Init(&GpioLed);

    while(1)
    {
    	//GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);//GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
    	delay();
    }


    return 0;
}
