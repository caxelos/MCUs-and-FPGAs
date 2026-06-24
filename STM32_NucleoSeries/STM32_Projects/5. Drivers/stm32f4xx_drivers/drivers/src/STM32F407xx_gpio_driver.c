/*
 * STM32F407xx_gpio_driver.c
 *
 *  Created on: Jun 9, 2026
 *      Author: caxel
 */

#include "STM32F407xx_gpio_driver.h"
/*
 * - Peripheral Clock setup
 * -  Enable/Disable a peripheral clock for a given GPIO base address
 */

/******************************************************************************
 * @fn                 - GPIO_PeriClockControl
 *
 * @brief              - This function enables or disables peripheral clock
 *                       for the given GPIO port
 *
 * @param[in]          - base address of the gpio peripheral
 * @param[in]          - ENABLE or DISABLE macros
 * @param[in]          -
 *
 * @return             - none
 *
 * @Note               - none
 *
 *****************************************************************************/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)  { // 93

    if (EnorDi == ENABLE) {
        if (pGPIOx == GPIOA)
        {
            GPIOA_PCLK_EN();
        }
        else if (pGPIOx == GPIOB)
        {
            GPIOB_PCLK_EN();
        }
        else if (pGPIOx == GPIOC)
        {
            GPIOC_PCLK_EN();
        }
        else if (pGPIOx == GPIOD)
        {
            GPIOD_PCLK_EN();
        }
        else if (pGPIOx == GPIOE)
        {
            GPIOE_PCLK_EN();
        }
        else if (pGPIOx == GPIOF)
        {
            GPIOF_PCLK_EN();
        }
        else if (pGPIOx == GPIOG)
        {
            GPIOG_PCLK_EN();
        }
        else if (pGPIOx == GPIOH)
        {
            GPIOH_PCLK_EN();
        }
    }
    else {
    	// TODO:
    }

}
/*
 * Init and De-Init
 * - we need some pointer to the handled structure
  void GPIO_DeInit(void);
   - The user application should create a variable of this type initialize that and send the pointer of that variable to the GPIO init function
   - in order to initialize GPIO port and pin
   - DeInit() function resets the values of the registers to reset state (it just needs the base address of the peripheral
 */


void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t temp = 0; // temp. register

    //1. configure the mode of gpio pin

    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        // the non interrupt mode
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode
                << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

        pGPIOHandle->pGPIOx->MODER |= temp;
    }
    else
    {
    	// This part will be coded later (interrupt mode)

    	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
    	{
    	    // 1. Configure the FTSR
    	    EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

    	    // Clear the corresponding RTSR bit
    	    EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    	}
    	else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
    	{
    	    // 1. Configure the RTSR
    	    EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

    	    // Clear the corresponding FTSR bit
    	    EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    	}
    	else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
    	{
    	    // Configure both FTSR and RTSR
    	    EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    	    EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    	}

    	//2. configure the GPIO port selection in SYSCFG_EXTICR
    	uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
    	uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
    	uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
    	SYSCFG_PCLK_EN();
    	SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);
    }

    temp = 0;

    //2. configure the speed
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed
            << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

    pGPIOHandle->pGPIOx->OSPEEDR |= temp;

    temp = 0;

    //3. configure the pupd settings
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl
            << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

    pGPIOHandle->pGPIOx->PUPDR |= temp;

    temp = 0;

    //4. configure the optype
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType
            << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

    pGPIOHandle->pGPIOx->OTYPER |= temp;

    temp = 0;

    //5. configure the alt functionality
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        // configure the alt function registers.
        uint8_t temp1, temp2;

        temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
        temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;

        pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2)); // clearing

        pGPIOHandle->pGPIOx->AFR[temp1] |=
            (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
    }

}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {

	if (pGPIOx == GPIOA)
	{
	    GPIOA_REG_RESET();
	}
	else if (pGPIOx == GPIOB)
	{
	    GPIOB_PCLK_EN();
	}
	else if (pGPIOx == GPIOC)
	{
	    GPIOC_REG_RESET();
	}
	else if (pGPIOx == GPIOD)
	{
	    GPIOD_REG_RESET();
	}
	else if (pGPIOx == GPIOE)
	{
	    GPIOE_REG_RESET();
	}
	else if (pGPIOx == GPIOF)
	{
	    GPIOF_REG_RESET();
	}
	else if (pGPIOx == GPIOG)
	{
	    GPIOG_REG_RESET();
	}
	else if (pGPIOx == GPIOH)
	{
	    GPIOH_REG_RESET();
	}
	else if (pGPIOx == GPIOI)
	{
	    GPIOI_REG_RESET();
	}

}


/**********************************************************************
 * @fn                  - GPIO_ReadFromInputPin
 *
 * @brief               - Read a value from the given GPIO input pin
 *
 * @param[in]           - Base address of the GPIO peripheral
 * @param[in]           - GPIO pin number
 * @param[in]           - Not used
 *
 * @return              - 0 or 1 (pin state)
 *
 * @Note                - none
 *
 **********************************************************************/
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    uint8_t value;

    value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

    return value;
}



uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) { // a port is 16 bits long
	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}


void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value) {
	if (Value == GPIO_PIN_SET)
	{
	    // write 1 to the output data register at the bit field corresponding to the pin
	    pGPIOx->ODR |= (1 << PinNumber);
	}
	else
	{
	    // write 0
	    pGPIOx->ODR &= ~(1 << PinNumber);
	}
}



/**
 * @fn      - GPIO_WriteToOutputPort
 *
 * @brief   - Writes a value to the GPIO output port.
 *
 * @param[in] pGPIOx : Base address of the GPIO peripheral.
 * @param[in] Value  : 16-bit value to be written to the output data register.
 *
 * @return  - None
 *
 * @Note    - This function writes directly to the ODR register, affecting
 *            all GPIO pins of the selected port.
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
    pGPIOx->ODR = Value;
}



void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)  {

	pGPIOx->ODR ^= (1 << PinNumber);// toggle the pin, whenever the function is called.

}

/*
 * IRQ Configuration and ISR handling
 * This configuration happens on the processor's side (we'll use the "Cortex-M4 Devices" Generic User Manual
 * Eg. Given IRQ number 236,
 * 	- Find out which IPR registeor to touch (IPR0...IPR59). Divide 236 by 4, because it's resistor suports 4 sections (
 * 	- So, we have to touch the 59th IPR...and to find the section, we have to mod4 the result
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
    	if (IRQNumber <= 31)
    	{
    	    // program ISER0 register
    	    *NVIC_ISER0 |= (1 << IRQNumber);
    	}
    	else if (IRQNumber > 31 && IRQNumber < 64)   // 32 to 63
    	{
    	    // program ISER1 register
    	    *NVIC_ISER1 |= (1 << (IRQNumber % 32)); // explained in video 112 the mod32 part
    	}
    	else if (IRQNumber >= 64 && IRQNumber < 96)
    	{
    	    // program ISER2 register // 64 to 95
    	    *NVIC_ISER2 |= (1 << (IRQNumber % 64));
    	}
    }
    else
    {
    	if (IRQNumber <= 31)
    	{
    	    // program ICER0 register
    	    *NVIC_ICER0 |= (1 << IRQNumber);
    	}
    	else if (IRQNumber > 31 && IRQNumber < 64)
    	{
    	    // program ICER1 register
    	    *NVIC_ICER1 |= (1 << (IRQNumber % 32));
    	}
    	else if (IRQNumber >= 64 && IRQNumber < 96)
    	{
    	    // program ICER2 register
    	    *NVIC_ICER2 |= (1 << (IRQNumber % 64));
    	}
    }
}


/*
 * void GPIO_IRQPriorityConfig()
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    // 1. Find out the IPR register
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount =
        (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}


void GPIO_IRQHandling(uint8_t PinNumber)  {

	// clear the EXTI PR register corresponding to the pin number
	if (EXTI->PR & (1 << PinNumber)) //CR here insteadvof PR
	{
	    // clear
	    EXTI->PR |= (1 << PinNumber);
	}
}
