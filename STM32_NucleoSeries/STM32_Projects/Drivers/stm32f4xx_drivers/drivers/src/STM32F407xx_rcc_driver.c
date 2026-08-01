/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: Mar 29, 2019
 *      Author: admin
 */


#include "STM32F407xx_rcc_driver.h"


uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512}; // it contains the devision factors mentioned in 6.3.3
uint8_t APB1_PreScaler[4] = { 2, 4 , 8, 16};



/* uint32_t RCC_GetPCLK1Value()
 *
 * Function that returns the current frequency of the APB1 peripheral clock (PCLK1), usually in Hz
 *
 * It is important for I2C because the I2C peripheral needs to know the clock feeding its hardware so it can generate the requested I²C SCL frequency.
 *
 * In order to calculate the value of the PCLK1, we need to refer to the clock tree in RM (Figure 14)
 *
 * PCLK1 depends oon the source clock (HSE, HSI, PLL, PLLR multiplexer)
 *
 * RCC = Reset and Clock Control.
 */
uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1,SystemClk;

	uint8_t clksrc,temp,ahbp,apb1p;

	clksrc = ((RCC->CFGR >> 2) & 0x3); // shift by 2, because the bring the bit number 2 and 3 to the bit position 0 and 1. ,asl

	if(clksrc == 0 ) // Source clock HSI oscillator
	{
		SystemClk = 16000000; // 16MHz
	}else if(clksrc == 1) // HSE oscillator as system clock
	{
		SystemClk = 8000000; // 8MHz
	}else if (clksrc == 2) // PLL used as system clock
	{
		SystemClk = RCC_GetPLLOutputClock();
	}



	//for ahb.
	//AHB prescaler value is decided by bitfields 4-7 of HPRE register (6.3.3 in RM)
	temp = ((RCC->CFGR >> 4 ) & 0xF);

	if(temp < 8)
	{
		ahbp = 1; // system clock divided by 2
	}else
	{
		ahbp = AHB_PreScaler[temp-8]; // system clock devided by 4 or 8 or 16 ... 512 (check 6.3.3 in RM)
	}



	//apb1
	//APB1 prescaler value is decided by bitfields 12-10 of PPRE1 register (6.3.3 in RM, "APB Low speed prescaler")
	temp = ((RCC->CFGR >> 10 ) & 0x7);

	if(temp < 4)
	{
		apb1p = 1; // divided by 2
	}else
	{
		apb1p = APB1_PreScaler[temp-4]; // divided by 4, 8 or 16 (see 6.3.3)
	}

	pclk1 =  (SystemClk / ahbp) /apb1p; // TODO: Explain this calculation based on RM

	return pclk1;
}



/*********************************************************************
 * @fn      		  - RCC_GetPCLK2Value
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
uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t SystemClock=0,tmp,pclk2;
	uint8_t clk_src = ( RCC->CFGR >> 2) & 0X3;

	uint8_t ahbp,apb2p;

	if(clk_src == 0)
	{
		SystemClock = 16000000;
	}else
	{
		SystemClock = 8000000;
	}
	tmp = (RCC->CFGR >> 4 ) & 0xF;

	if(tmp < 0x08)
	{
		ahbp = 1;
	}else
	{
       ahbp = AHB_PreScaler[tmp-8];
	}

	tmp = (RCC->CFGR >> 13 ) & 0x7;
	if(tmp < 0x04)
	{
		apb2p = 1;
	}else
	{
		apb2p = APB1_PreScaler[tmp-4];
	}

	pclk2 = (SystemClock / ahbp )/ apb2p;

	return pclk2;
}


/*
 * We will not use it right now
 * TODO: Create the similar functionality of HSI, HSE, but for PLL
 */
uint32_t  RCC_GetPLLOutputClock()
{

	return 0;
}

