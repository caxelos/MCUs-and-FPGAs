/*
 * STM32F407xx_spi_driver.c
 *
 *  Created on: Jun 30, 2026
 *      Author: caxel
 */


/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Feb 9, 2019
 *      Author: admin
 */

#include "STM32F407xx_spi_driver.h"

static void  spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle); //use "static" keyword to show that these are actually private helper functions
static void  spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void  spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}
	else
	{
		//TODO: This is for the disable part
	}
}


/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             -	Section 28.5 SPI and I2S register
 * 					  - SPI status register (SPI_SR) - 28.5.3
 * 					  			- E.g: It flags an event (RX buffer, TX buffer, Full/Empty events, idle events, CRC errors
 * 					  			- These status peripherals are for almost all peripherals (ADC, DAC, Ethernet) - 1,2 registers per peripheral at least
 * 					  			- BR0, BR1, BR2 registers are for the Baud Rate control
 *
 */
void SPI_Init(SPI_Handle_t *pSPIHandle) // input is a user-configurable item
{

	//peripheral clock enable, so that the user doesn't need to do it from the application level

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//first lets configure the SPI_CR1 register

	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR ; // Shift

	//2. Configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//bidi mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);

	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI mode should be set
		tempreg |= ( 1 << SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= ( 1 << SPI_CR1_RXONLY);
	}

	// 3. Configure the spi serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4.  Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5. configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6 . configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;


	//TODO: Configure the MSB or LSB (by default, the transmission is Most Significant bit) SPI_CR1_LSBFIRST
	//tempreg |= (1 << SPI_CR1_LSBFIRST);

	pSPIHandle->pSPIx->CR1 = tempreg;

}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
 //todo
}




uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SendData

 * @Note              - This is blocking call. We call it blocking API, because the function call will wait until all the bytes are transmitted
 * 					  - The number of bytes to be transmitted are "Len"
 *f
 * 					  - TXE stands for Transmit Buffer Empty.
 * 					  - It's a status flag (typically SPI_SR.TXE) that tells you whether the SPI transmit data register is ready to accept another data word.
 * 					  The TXE flag indicates whether that register is empty and can accept new dat
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		/*
		 * 1. Wait until the Transmit Buffer Empty (TXE) flag becomes 1,
		 *    indicating that the SPI data register is ready to accept another byte/word.
		 *    TXE is a bit in the SPI Status Register (SPI_SR).
		 */
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG)  == FLAG_RESET ); //SPI_TXE_FLAG is MCU specifiC

		/*
		 * 2. check the DFF bit in CR1
		 * The DFF (Data Frame Format) bit tells the SPI peripheral how many bits make up one data frame.
		 */
		if( (pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
		{
			//16 bit DFF
			//1. load the data in to the DR
			pSPIx->DR =   *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else // this never runs, since we are currently at 16-bit mode
		{
			/*
			 * 8 bit DFF -> Only the lower byte of txBuffer is written to pSpix->DR register
			 * (It is the register you use to send and receive SPI data.)
			 * The hardware then moves this data into the shift register
			 * when it is ready and starts sending it out on the MOSI pin.
			 *
			 * SR: "0x3" = Transmit buffer is empty and received data is waiting to be read.
			 * 	   "0x2" = Transmit buffer is empty, and the received data has already been read.
			 */

			*((volatile uint8_t *)&pSPIx->DR) = *pTxBuffer;
			//*((volatile uint8_t *)&pSPIx->DR) = 0x55; //pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
		{
			//1. wait until RXNE is set
			while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG)  == (uint8_t)FLAG_RESET );

			//2. check the DFF bit in CR1
			if( (pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
			{
				//16 bit DFF
				//1. load the data from DR to Rxbuffer address
				 *((uint16_t*)pRxBuffer) = pSPIx->DR ;
				Len--;
				Len--;
				(uint16_t*)pRxBuffer++;
			}else
			{
				//8 bit DFF
				*(pRxBuffer) = pSPIx->DR ;
				Len--;
				pRxBuffer++;
			}
		}

}


/*********************************************************************
 * @fn      		  - SPI_PeripheralControl
 * @brief             -
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &=  ~(1 << SPI_CR1_SPE);
	}



}

/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 */
void  SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &=  ~(1 << SPI_CR1_SSI);
	}


}


/*********************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             -Slave Select Output Enable.
 *
 * @param[in]         - It is a configuration bit in the STM32 SPI peripheral (typically in the SPI_CR2 register) that
 * 						controls whether the SPI hardware automatically drives the NSS (Slave Select) pin when the STM32
 * 						 is operating as an SPI master.
 * @param[in]         -
 * @param[in]         -
 *
				 * SSOE	SPI Mode	NSS Pin Behavior
				0	Master	NSS pin is not driven by SPI hardware. You control it manually as a GPIO, or ignore it.
				1	Master	SPI hardware automatically drives the NSS pin (when configured for hardware NSS output).
				Either	Slave	This bit has no effect; the slave receives NSS from the external master.
				 *
 * @return            -
 *SSOE = 1
				When you enable SPI (SPE = 1), the hardware pulls NSS low.
				When you disable SPI (SPE = 0), the hardware releases NSS high.
				You don't have to toggle NSS yourself.
				SSOE = 0
				The SPI peripheral does not control the NSS pin.
				You typically configure NSS as a normal GPIO and manually assert/deassert it before and after each transfer.
				This is the most common approach when communicating with multiple SPI slaves.
				 * @Note              -

 */
void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |=  (1 << SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &=  ~(1 << SPI_CR2_SSOE);
	}


}




/*
 * void GPIO_IRQPriorityConfig()
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}


uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState; //Check 28.5.2, SPI control register 2 (SPI_CR2), 7th position

	if(state != SPI_BUSY_IN_TX)
	{
		//1 . Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;


		//2.  Mark the SPI state as busy in transmission so that
		//    no other code can take over same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;


		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= ( 1 << SPI_CR2_TXEIE );

	}


	return state;
}


/*
RXNE = Receive buffer Not Empty, RXNEIE = RXNE Interrupt Enable
RXNE tells you that received data is waiting; RXNEIE tells the SPI peripheral to interrupt the CPU when that happens.
RXNEIE → "Should an interrupt occur?"
         Control bit in CR2

RXNE   → "Has received data arrived?"
         Status flag in SR
*/
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1 . Save the Rx buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		//2.  Mark the SPI state as busy in reception so that // no other code can take over same SPI peripheral until reception is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3. Enable the RXNEIE control bit to get interrupt whenever RXNEIE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= ( 1 << SPI_CR2_RXNEIE );

	}


	return state;

}




/*
 * Decode the reason of the interrupt (receive, transmit or error related)
 */


/*
 * OVR --> (an overrun condition occurs when the master or he slave completes the reception of the next data frame while the read operOVation of the previous frame from the Rx
	 buffer has not completed (RXNE flag is set)
	In this case, the content of the Rx buffer is not updated with the new data received and new data will be discared.
	(check datasheet for more details)
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

	uint8_t temp1 , temp2;
	//first lets check for TXE
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_TXE); //global vars
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_TXEIE);

	if( temp1 && temp2)
	{
		//handle TXE
		spi_txe_interrupt_handle(pHandle);
	}

	// check for RXNE
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_RXNEIE);

	if( temp1 && temp2)
	{
		//handle RXNE
		spi_rxne_interrupt_handle(pHandle);
	}

	// check for ovr flag
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_ERRIE);

	if( temp1 && temp2)
	{
		//handle ovr error
		spi_ovr_err_interrupt_handle(pHandle);
	}

	// TODO: CRC error

	// TODO: MODF error
}


//some helper function implementations


/*
 * TXE (Transmit Buffer Empty) Interrupt Handling
 *
 * 1. Handle the TXE interrupt.
 *
 * 2. Check the SPI data-frame format:
 *
 *    If SPI is in 8-bit mode:
 *      - Write 1 byte of data into the SPI Data Register (DR).
 *      - Decrement the transmit length by 1.
 *
 *    If SPI is in 16-bit mode:
 *      - Write 2 bytes of data into the SPI Data Register (DR).
 *      - Decrement the transmit length by 2.
 *
 * 3. Check whether the transmit length has reached zero.
 *
 *    If Len == 0:
 *      - Transmission is complete.
 *      - Close/disable SPI TXE interrupt.
 *
 *    If Len != 0:
 *      - Transmission is not complete.
 *      - Wait for the next TXE interrupt and repeat the process.
 */
static void  spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	// check the DFF bit in CR1
	if( (pSPIHandle->pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
	{
		//16 bit DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR =   *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}else
	{
		//8 bit DFF
		pSPIHandle->pSPIx->DR =   *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(! pSPIHandle->TxLen)
	{
		// TxLen is zero , we need to close communication; so close the spi transmission
		// Also inform the application that TX is over
		//We don't want any more interrupts from TXE flag
		SPI_CloseTransmisson(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT); // The application has to implement this callback
	}

}


static void  spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//do rxing as per the dff
	if(pSPIHandle->pSPIx->CR1 & ( 1 << 11))
	{
		//16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer++;
		pSPIHandle->pRxBuffer++;

	}else
	{
		//8 bit
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(! pSPIHandle->RxLen)
	{
		//reception is complete
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}

}


static void  spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{

	uint8_t temp;
	//1. clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	//2. inform the application
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);

}


void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;

}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;

}



void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;

}


/*
 * IRQ Configuration and ISR handling
 * This configuration happens on the processor's side (we'll use the "Cortex-M4 Devices" Generic User Manual
 * Eg. Given IRQ number 236,
 * 	- So, we have to touch the 59th IPR...and to find the section, we have to mod4 the result

 	  -We will keep this function only to enable and disable the interrupt!

 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) //(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
    	if (IRQNumber <= 31)
    	{
    	    // program ISER0 register
    	    *NVIC_ISER0 = (1U << IRQNumber);
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


__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	return;
	//This is a weak implementation . the user application may override this function.
}
