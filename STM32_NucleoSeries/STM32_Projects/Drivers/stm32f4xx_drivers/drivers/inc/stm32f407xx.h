/*
 * stm32f407xx.h
 *
 *  Created on: Dec 15, 2025
 *      Author: caxel
 *
 *      - Check video no.74
 *      - Instead of creating a normal project, set:
 *      	- Targeted Project Type: Empty (since we write our own Driver)
 *      - We created the folder "drivers" and it's subfolders, it didn't exist at first
 *      - Store under "src" and "inc" the files for the driver
 *      - "Project Properties ---> C/C++ Build ---> Untick "Exclude resource from buil"
 *
 *		MCU specific header file
 */

#ifndef INC_STM32F407XX_H_ // these guards actually prevent inclusion of multiple definitions during the preprocessor stage of the compilation
#define INC_STM32F407XX_H_

#include <stdint.h>
#include <stddef.h>

#define __vo volatile

#ifndef __weak
#define __weak __attribute__((weak))
#endif


/****************************** START: Processor Specific Details ******************************
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 * Processor specific based on "Table 4-2 NVIC register summary" of ARM Cortex General User Manual
 * Video 112
 */

/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */

#define NVIC_ISER0   ((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1   ((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2   ((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3   ((__vo uint32_t*)0xE000E10C)

/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0   ((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1   ((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2   ((__vo uint32_t*)0xE000E188)
#define NVIC_ICER3   ((__vo uint32_t*)0xE000E18C)


/*
 * ARM Cortex Mx Processor Priority Register Address Calculation (check it in ARM Cortex User Manual)
 */
#define NVIC_PR_BASE_ADDR   ((__vo uint32_t*)0xE000E400)


/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED  4


/*
 * base addresses of Flash and SRAM memories
 */
#define FLASH_BASEADDR		0x08000000U // Table 5 in RF (the compiler treats all this numbers as signed int, so we have to cast)
#define SRAM1_BASEADDR		0X20000000U //this is our main RAM memory
#define SRAM2_BASEADDR		0X20001C00U // Because in the 2.1 System architecture, it says "Main internal SRAM1 is 112KB, which is 1C00 in hex
#define ROM_BASEADDR		0x1FFF0000U				 // 2.
#define SRAM 				SRAM1_BASEADDR

/*
 * AHBx and APBx Bus Peripheral base addresses
 */
#define PERIPH_BASEADDR 						0x40000000U  // this is the address of the first peripheral
#define APB1PERIPH_BASEADDR						PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR						0x40010000U
#define AHB1PERIPH_BASEADDR						0x40020000U
#define AHB2PERIPH_BASEADDR						0x50000000U


/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 */
#define GPIOA_BASEADDR	(AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR	(AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR	(AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR	(AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR	(AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR	(AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR	(AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR	(AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR	(AHB1PERIPH_BASEADDR + 0x2000)
#define RCC_BASEADDR	(AHB1PERIPH_BASEADDR + 0x3800)


/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */
#define I2C1_BASEADDR						(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR						(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR						(APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR						(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR						(APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASEADDR						(APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR						(APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR						(APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR						(APB1PERIPH_BASEADDR + 0x5000)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 * TODO : Complete for all other peripherals
 */
#define EXTI_BASEADDR						(APB2PERIPH_BASEADDR + 0x3C00)
#define SPI1_BASEADDR						(APB2PERIPH_BASEADDR + 0x3000)
#define SYSCFG_BASEADDR        				(APB2PERIPH_BASEADDR + 0x3800)
#define USART1_BASEADDR						(APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR						(APB2PERIPH_BASEADDR + 0x1400)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */
#define EXTI_BASEADDR		(APB2PERIPH_BASEADDR + 0x3C00)
#define SPI1_BASEADDR		(APB2PERIPH_BASEADDR + 0x3000)
#define SYSCFG_BASEADDR		(APB2PERIPH_BASEADDR + 0x3800)
#define UART1_BASE			(APB2PERIPH_BASEADDR + 0x1000)
#define UART6_BASE			(APB2PERIPH_BASEADDR + 0x1400)


/************************ peripheral register definition structures ************************/

/*
 * Note : Registers of a peripheral are specific to MCU
 * e.g : Number of registers of SPI peripheral of STM32F4x family of MCUs may be different (more or less)
 *       compared to number of registers of SPI peripheral of STM32Lx or STM32F0x family of MCUs
 * Please check your Device RM
 */

/*
 * Peripheral register definition structure for GPIO
 * STM32F411xx - RM0383, Section 8.4
 */
typedef struct
{
    __vo uint32_t MODER;      // 8.4.1  Offset: 0x00 - Selects pin mode: input, output, alternate function or analog
    __vo uint32_t OTYPER;     // 8.4.2  Offset: 0x04 - Selects output type: push-pull or open-drain
    __vo uint32_t OSPEEDR;    // 8.4.3  Offset: 0x08 - Controls output speed/slew rate for each GPIO pin
    __vo uint32_t PUPDR;      // 8.4.4  Offset: 0x0C - Configures no pull, pull-up or pull-down for each pin
    __vo uint32_t IDR;        // 8.4.5  Offset: 0x10 - Reads the current logic level present on GPIO input pins
    __vo uint32_t ODR;        // 8.4.6  Offset: 0x14 - Reads/writes the output value of GPIO output pins
    __vo uint32_t BSRR;       // 8.4.7  Offset: 0x18 - Atomically sets or resets individual GPIO output pins
    __vo uint32_t LCKR;       // 8.4.8  Offset: 0x1C - Locks GPIO configuration until the next MCU reset
    __vo uint32_t AFR[2];     // 8.4.9  Offset: 0x20-0x24 - Selects alternate function (AF0-AF15) for each GPIO pin.  Instead of creating 2 separate variable, I just created an array her (AFRL & AFRH for alternate functions)
                                 // AFR[0] = AFRL for pins 0-7, AFR[1] = AFRH for pins 8-15

} GPIO_RegDef_t;


/*
 * Peripheral register definition structure for RCC
 * STM32F411xx - RM0383, Section 6.3
 */
typedef struct
{
    __vo uint32_t CR;          // 6.3.1  Offset: 0x00 - Enables/monitors HSI, HSE, PLL and PLLI2S clocks
    __vo uint32_t PLLCFGR;     // 6.3.2  Offset: 0x04 - Configures main PLL source, dividers and multipliers
    __vo uint32_t CFGR;        // 6.3.3  Offset: 0x08 - Selects system clock and configures AHB/APB prescalers
    __vo uint32_t CIR;         // 6.3.4  Offset: 0x0C - Clock interrupt enable, status and clear flags

    __vo uint32_t AHB1RSTR;    // 6.3.5  Offset: 0x10 - Resets peripherals connected to the AHB1 bus
    __vo uint32_t AHB2RSTR;    // 6.3.6  Offset: 0x14 - Resets peripherals connected to the AHB2 bus
    __vo uint32_t AHB3RSTR;    // 6.3.7  Offset: 0x18 - Resets peripherals connected to the AHB3 bus

    uint32_t RESERVED0;        //        Offset: 0x1C - Reserved

    __vo uint32_t APB1RSTR;    // 6.3.8  Offset: 0x20 - Resets peripherals connected to the APB1 bus
    __vo uint32_t APB2RSTR;    // 6.3.9  Offset: 0x24 - Resets peripherals connected to the APB2 bus

    uint32_t RESERVED1[2];     //        Offset: 0x28-0x2C - Reserved

    __vo uint32_t AHB1ENR;     // 6.3.10 Offset: 0x30 - Enables clocks for AHB1 peripherals (GPIO, DMA, CRC, etc.)
    __vo uint32_t AHB2ENR;     // 6.3.11 Offset: 0x34 - Enables clocks for AHB2 peripherals (USB OTG FS)
    __vo uint32_t AHB3ENR;     // 6.3.12 Offset: 0x38 - Enables clocks for AHB3 peripherals

    uint32_t RESERVED2;        //        Offset: 0x3C - Reserved

    __vo uint32_t APB1ENR;     // 6.3.13 Offset: 0x40 - Enables APB1 peripheral clocks (I2C, USART2, SPI2, etc.)
    __vo uint32_t APB2ENR;     // 6.3.14 Offset: 0x44 - Enables APB2 peripheral clocks (SPI1, USART1/6, ADC, etc.)

    uint32_t RESERVED3[2];     //        Offset: 0x48-0x4C - Reserved

    __vo uint32_t AHB1LPENR;   // 6.3.15 Offset: 0x50 - Controls AHB1 peripheral clocks during Sleep mode
    __vo uint32_t AHB2LPENR;   // 6.3.16 Offset: 0x54 - Controls AHB2 peripheral clocks during Sleep mode
    __vo uint32_t AHB3LPENR;   // 6.3.17 Offset: 0x58 - Controls AHB3 peripheral clocks during Sleep mode

    uint32_t RESERVED4;        //        Offset: 0x5C - Reserved

    __vo uint32_t APB1LPENR;   // 6.3.18 Offset: 0x60 - Controls APB1 peripheral clocks during Sleep mode
    __vo uint32_t APB2LPENR;   // 6.3.19 Offset: 0x64 - Controls APB2 peripheral clocks during Sleep mode

    uint32_t RESERVED5[2];     //        Offset: 0x68-0x6C - Reserved

    __vo uint32_t BDCR;        // 6.3.20 Offset: 0x70 - Controls LSE, RTC clock source and backup domain reset
    __vo uint32_t CSR;         // 6.3.21 Offset: 0x74 - Controls LSI and contains reset-cause status flags

    uint32_t RESERVED6[2];     //        Offset: 0x78-0x7C - Reserved

    __vo uint32_t SSCGR;       // 6.3.22 Offset: 0x80 - Configures PLL spread-spectrum clock generation
    __vo uint32_t PLLI2SCFGR;  // 6.3.23 Offset: 0x84 - Configures PLLI2S clock multiplication/division
    __vo uint32_t PLLSAICFGR;  // 6.3.24 Offset: 0x88 - Configures PLLSAI clock parameters
    __vo uint32_t DCKCFGR;     // 6.3.25 Offset: 0x8C - Configures dedicated peripheral clock sources
    __vo uint32_t CKGATENR;    // 6.3.26 Offset: 0x90 - Controls clock gating for selected internal clocks
    __vo uint32_t DCKCFGR2;    // 6.3.27 Offset: 0x94 - Additional dedicated peripheral clock configuration

} RCC_RegDef_t;


/*
 * peripheral register definition structure for EXTI
 */
typedef struct
{
    __vo uint32_t IMR;    /*!< Interrupt mask register,      Address offset: 0x00 */
    __vo uint32_t EMR;    /*!< Event mask register,          Address offset: 0x04 */
    __vo uint32_t RTSR;   /*!< Rising trigger selection register, Address offset: 0x08 */
    __vo uint32_t FTSR;   /*!< Falling trigger selection register, Address offset: 0x0C */
    __vo uint32_t SWIER;  /*!< Software interrupt event register, Address offset: 0x10 */
    __vo uint32_t PR;     /*!< Pending register,             Address offset: 0x14 */
} EXTI_RegDef_t;


/*
 *
 * peripheral register definition structure for SPI (28.5.10)
 *
 * SPI3: 0x40003C00U
 *
 * Bitfields of SPI Status Register (SR) ---> contains flags indicating the current state of the SPI peripheral.
 * 		0	RXNE	Receive buffer not empty. Data is available to read from DR.
 *		1	TXE	Transmit buffer empty. You can write new data to DR.
 *		4	CRCERR	CRC error detected.
 * 		5	MODF	Mode fault error.
 * 		6	OVR	Overrun error. Received data was lost because it wasn't read in time.
 * 		7	BSY	SPI is busy transmitting or receiving.
 * 		8	FRE	Frame format error (TI mode).
 */

typedef struct
{
    __vo uint32_t CR1;      // 28.5.1 Offset: 0x00 - Controls SPI: enable, master/slave, baud rate, CPOL/CPHA, data format
    __vo uint32_t CR2;      // 28.5.2 Offset: 0x04 - Controls SPI interrupts, DMA, SS output and frame format
    __vo uint32_t SR;       // 28.5.3 Offset: 0x08 - Status flags: TxE, RxNE, BSY, overrun, mode fault and CRC error
    __vo uint32_t DR;       // 28.5.4 Offset: 0x0C - Holds data received or data to be transmitted
    __vo uint32_t CRCPR;    // 28.5.5 Offset: 0x10 - Sets the polynomial used for SPI CRC calculation
    __vo uint32_t RXCRCR;   // 28.5.6 Offset: 0x14 - Holds the calculated CRC value for received data
    __vo uint32_t TXCRCR;   // 28.5.7 Offset: 0x18 - Holds the calculated CRC value for transmitted data
    __vo uint32_t I2SCFGR;  // 28.5.8 Offset: 0x1C - Configures I2S mode, standard, data length, clock polarity and direction
    __vo uint32_t I2SPR;    // 28.5.9 Offset: 0x20 - Configures I2S clock prescaler and master clock output

} SPI_RegDef_t;


/*
 * Peripheral register definition structure for SYSCFG
 * STM32F411xx - RM0383, Section 9.3
 */
typedef struct
{
    __vo uint32_t MEMRMP;       // 9.3.1 Offset: 0x00 - Controls memory mapping at address 0x00000000
    __vo uint32_t PMC;          // 9.3.2 Offset: 0x04 - Controls peripheral mode, including Ethernet PHY interface
    __vo uint32_t EXTICR[4];    // 9.3.3 Offset: 0x08-0x14 - Selects GPIO port connected to each EXTI line
                                //                          Does not enable interrupts or configure trigger edges

    uint32_t RESERVED1[2];      //       Offset: 0x18-0x1C - Reserved memory locations

    __vo uint32_t CMPCR;        // 9.3.4 Offset: 0x20 - Controls I/O compensation cell for high-speed GPIO signals

    uint32_t RESERVED2[2];      //       Offset: 0x24-0x28 - Reserved memory locations

    __vo uint32_t CFGR;         // 9.3.5 Offset: 0x2C - SYSCFG configuration register; controls FMP on supported I2C pins

} SYSCFG_RegDef_t;



/*
 * Peripheral register definition structure for I2C
 * STM32F411xx - RM0383, Section 18.6
 */
typedef struct
{
  __vo uint32_t CR1;    // 18.6.1  Offset: 0x00 - Controls I2C: enable, START/STOP, ACK, reset
  __vo uint32_t CR2;    // 18.6.2  Offset: 0x04 - Sets peripheral clock, interrupts and DMA control
  __vo uint32_t OAR1;   // 18.6.3  Offset: 0x08 - Sets primary own address and 7/10-bit addressing
  __vo uint32_t OAR2;   // 18.6.4  Offset: 0x0C - Sets secondary own address for dual addressing
  __vo uint32_t DR;     // 18.6.5  Offset: 0x10 - Stores data byte to transmit or received data byte
  __vo uint32_t SR1;    // 18.6.6  Offset: 0x14 - Event/error flags: SB, ADDR, TxE, RxNE, BTF, errors
  __vo uint32_t SR2;    // 18.6.7  Offset: 0x18 - Bus status: master/slave, busy, transmitter/receiver
  __vo uint32_t CCR;    // 18.6.8  Offset: 0x1C - Sets SCL clock rate and standard/fast mode timing
  __vo uint32_t TRISE;  // 18.6.9  Offset: 0x20 - Sets maximum allowed SCL signal rise time
  __vo uint32_t FLTR;   // 18.6.10 Offset: 0x24 - Configures analog and digital noise filtering
} I2C_RegDef_t;


/*
 * peripheral register definition structure for USART
 */
typedef struct
{
	__vo uint32_t SR;         /*!< TODO,     										Address offset: 0x00 */
	__vo uint32_t DR;         /*!< TODO,     										Address offset: 0x04 */
	__vo uint32_t BRR;        /*!< TODO,     										Address offset: 0x08 */
	__vo uint32_t CR1;        /*!< TODO,     										Address offset: 0x0C */
	__vo uint32_t CR2;        /*!< TODO,     										Address offset: 0x10 */
	__vo uint32_t CR3;        /*!< TODO,     										Address offset: 0x14 */
	__vo uint32_t GTPR;       /*!< TODO,     										Address offset: 0x18 */
} USART_RegDef_t;


/*
 * peripheral definitions ( Peripheral base addresses typecasted to xxx_RegDef_t )
 */
#define GPIOA 			((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB 			((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 			((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD			((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE 			((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF 			((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG 			((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH 			((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI 			((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC 			((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI 			((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG 			((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)


#define SPI1  				((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2  				((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3  				((SPI_RegDef_t*)SPI3_BASEADDR)
//TODO: Include also SPI4


#define I2C1  				((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2  				((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3  				((I2C_RegDef_t*)I2C3_BASEADDR)

#define USART1  			((USART_RegDef_t*)USART1_BASEADDR)
#define USART2  			((USART_RegDef_t*)USART2_BASEADDR)
#define USART3  			((USART_RegDef_t*)USART3_BASEADDR)
#define UART4  				((USART_RegDef_t*)UART4_BASEADDR)
#define UART5  				((USART_RegDef_t*)UART5_BASEADDR)
#define USART6  			((USART_RegDef_t*)USART6_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */
/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()    (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()    (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()    (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()    (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()    (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()    (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()    (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()    (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()    (RCC->AHB1ENR |= (1 << 8))


/*
 * Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_EN()    ( RCC->APB1ENR |= (1 << 21) )
#define I2C2_PCLK_EN() (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN() (RCC->APB1ENR |= (1 << 23))


/*
 * Clock Enable Macros for SPIx peripheralsbu
 */
#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN() (RCC->APB2ENR |= (1 << 13))

/*
 * Clock Enable Macros for USARTx peripherals
 */
/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCCK_EN() (RCC->APB2ENR |= (1 << 4))
#define USART2_PCCK_EN() (RCC->APB1ENR |= (1 << 17))
#define USART3_PCCK_EN() (RCC->APB1ENR |= (1 << 18))
#define UART4_PCCK_EN()  (RCC->APB1ENR |= (1 << 19))
#define UART5_PCCK_EN()  (RCC->APB1ENR |= (1 << 20))
#define USART6_PCCK_EN() (RCC->APB1ENR |= (1 << 5))



/*
 * Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN()    (RCC->APB2ENR |= (1 << 14))


/*
 * Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 8))
/*
 * Clock Disable Macros for I2Cx peripherals
 */


/*
 * Clock Enable Macros for SPIx peripherals
 */
#define SPI1_PCLK_EN()     (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()     (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()     (RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()     (RCC->APB2ENR |= (1 << 13))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN()   (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()   (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()   (RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()    (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()    (RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()   (RCC->APB2ENR |= (1 << 5))


/*
 * Clock Disable Macros for SYSCFG peripheral
 */


/*
 * Macros to reset GPIOx peripherals (7.3.5)
 */
/*
 * Macros to reset GPIOx peripherals (use static inline here instead)
 */
#define GPIOA_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)
#define GPIOI_REG_RESET()  do{ (RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); }while(0)

#define GPIO_BASEADDR_TO_CODE(x)   ( (x == GPIOA) ? 0 : \
                                     (x == GPIOB) ? 1 : \
                                     (x == GPIOC) ? 2 : \
                                     (x == GPIOD) ? 3 : \
                                     (x == GPIOE) ? 4 : \
                                     (x == GPIOF) ? 5 : \
                                     (x == GPIOG) ? 6 : \
                                     (x == GPIOH) ? 7 : 0 )

/*
 * IRQ(Interrupt Request) Number of STM32F407x MCU
 * NOTE: update these macros with valid values according to your MCU
 * TODO: You may complete this list for other peripherals
 * Always verify these numbers according to our vector table in manual
 * TODO: Check the "CMSIS function" on ARM Cortex M4
 */
#define IRQ_NO_EXTI0        6
#define IRQ_NO_EXTI1        7
#define IRQ_NO_EXTI2        8
#define IRQ_NO_EXTI3        9
#define IRQ_NO_EXTI4        10
#define IRQ_NO_EXTI9_5      23
#define IRQ_NO_EXTI15_10    40

/*
 * SPI IRQ Numbers
 */
#define IRQ_NO_SPI1         35
#define IRQ_NO_SPI2         36
#define IRQ_NO_SPI3         51
#define IRQ_NO_SPI4			// TODO


/*
 * I2C IRQ Numbers
 */
#define IRQ_NO_I2C1_EV     31
#define IRQ_NO_I2C1_ER     32
#define IRQ_NO_I2C2_EV     //TODO
#define IRQ_NO_I2C2_ER     //TODO
#define IRQ_NO_I2C3_EV     //TODO
#define IRQ_NO_I2C3_ER     //TODO
#define IRQ_NO_USART1	    37
#define IRQ_NO_USART2	    38
#define IRQ_NO_USART3	    39
#define IRQ_NO_UART4	    52
#define IRQ_NO_UART5	    53
#define IRQ_NO_USART6	    71

/*
 * Define some IRQ priorities (instead of giving magic numbers)
 * macros for all possible priority levels
 */
#define NVIC_IRQ_PRIO0    0
#define NVIC_IRQ_PRIO1    1
#define NVIC_IRQ_PRIO2    2
#define NVIC_IRQ_PRIO3    3
#define NVIC_IRQ_PRIO4    4
#define NVIC_IRQ_PRIO5    5
#define NVIC_IRQ_PRIO6    6
#define NVIC_IRQ_PRIO7    7
#define NVIC_IRQ_PRIO8    8
#define NVIC_IRQ_PRIO9    9
#define NVIC_IRQ_PRIO10   10
#define NVIC_IRQ_PRIO11   11
#define NVIC_IRQ_PRIO12   12
#define NVIC_IRQ_PRIO13   13
#define NVIC_IRQ_PRIO14   14
#define NVIC_IRQ_PRIO15   15


//some generic macros

#define ENABLE 				1
#define DISABLE 			0
#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET		RESET
#define FLAG_RESET         RESET
#define FLAG_SET 			SET


/******************************************************************************************
 *Bit position definitions of SPI peripheral
 ******************************************************************************************/
/*
 * Bit position definitions SPI_CR1
 */
#define SPI_CR1_CPHA     				 0			// CLOCK PHASE (1-bit): Determines when data is sampled: 0 = First clock edge, 1 = Second clock Edgr. Use it together with CPOL to select SPI mode
#define SPI_CR1_CPOL      				 1			// CLOCK POLARITY (1-bit): Defines the idle state of SCK (either 0 or 1)
#define SPI_CR1_MSTR     				 2 			// MASTER SELECTION (1-bit): Selects wheter the peripheral is Slave  (=0) or Master (=1)
#define SPI_CR1_BR   					 3			// BAUD RATE (3-bit): This divides the APB2 clock (e.g. 16MHz). (e.g 000= PCLK/2, 001 = PCLK/4, 010 = PCLK/8, ... 111 = PCLK /256
#define SPI_CR1_SPE     				 6			// SPI ENABLE (1-bit): 0 = Disabled, 1 = Enabled
#define SPI_CR1_LSBFIRST   			 	 7			// TRANSMITION ORDER (1-bit):  0 = MSB first, 1 = LSB first
#define SPI_CR1_SSI     				 8			// INTERNAL SLAVE SELECT (1-bit): Used only when SW slave management (SSM) is enabled. It provides internal value for the NSS signal. Without setting SSI, the SPI may detect a mode fault (MODF) when operating as a master with SW-managed NSS
#define SPI_CR1_SSM      				 9			// SW SLAVE MANAGEMENT (1-bit):  Controls NSS management. 0 = Use actual NSS pin, 1 = Use SSI instead)
#define SPI_CR1_RXONLY      		 	10			// RECEIVE ONLY MODE (1-bit): 0 = Full Duplex, 1 = Receive only
#define SPI_CR1_DFF     			 	11			// DATA FRAME FORMAT (1-bit): Selects data size (0 = 8-bit, 1 = 16bit) The DFF (Data Frame Format) bit tells the SPI peripheral how many bits make up one data frame.
#define SPI_CR1_CRCNEXT   			 	12			// If this is enabled, this tells the SPI that the next transmitted frame should be the RC value instead of application data (most applications use it cleared)
#define SPI_CR1_CRCEN   			 	13			// Enable HW CRC calculation (Rarely used unless the communication protocol requires CRC checking.)
#define SPI_CR1_BIDIOE     			 	14			// Bidirectional Output Enable: Used only in 1-line bidirectional mode (BIDIMODE = 1). 0 = Receive, 1= Transmit
#define SPI_CR1_BIDIMODE      			15			// Chooses communication mode: 0 = Normal SPI (MOSI, MISO), 1 = 2 Separate data lines (full duplex) ---> One bidirectional data line, used in some specialized devices.

/*
 * Bit position definitions SPI_CR2
 */
#define SPI_CR2_TXDMAEN  1   /* TX buffer DMA enable */
#define SPI_CR2_SSOE     2   /* SS output enable */
#define SPI_CR2_FRF      4   /* Frame format: Motorola SPI or TI mode */
#define SPI_CR2_ERRIE    5   /* Error interrupt enable */
#define SPI_CR2_RXNEIE   6   /* RX buffer not empty interrupt enable */
#define SPI_CR2_TXEIE    7   /* TX buffer empty interrupt enable */


/*
 * Bit position definitions SPI_SR
 */
#define SPI_SR_RXNE     0   /* Receive buffer not empty */
#define SPI_SR_TXE      1   /* Transmit buffer empty */
#define SPI_SR_CHSIDE   2   /* Channel side (I2S mode) */
#define SPI_SR_UDR      3   /* Underrun flag (I2S mode) */
#define SPI_SR_CRCERR   4   /* CRC error flag */
#define SPI_SR_MODF     5   /* Mode fault flag */
#define SPI_SR_OVR      6   /* Overrun flag */
#define SPI_SR_BSY      7   /* SPI is busy */
#define SPI_SR_FRE      8   /* Frame format error */



/******************************************************************************************
 *Bit position definitions of I2C peripheral
 ******************************************************************************************/
/*
 * Bit position definitions I2C_CR1
 * STM32F411xx - RM0383, Section 18.6.1
 */
#define I2C_CR1_PE          0    // Peripheral Enable: 1 = I2C peripheral enabled
#define I2C_CR1_NOSTRETCH   7    // Clock stretching disable: 1 = clock stretching disabled
#define I2C_CR1_START       8    // START generation: generates START/repeated START condition
#define I2C_CR1_STOP        9    // STOP generation: generates STOP condition
#define I2C_CR1_ACK         10   // Acknowledge enable: 1 = ACK returned after receiving a byte
#define I2C_CR1_SWRST       15   // Software reset: resets the I2C peripheral


/*
 * Bit position definitions I2C_CR2
 * STM32F411xx - RM0383, Section 18.6.2
 */
#define I2C_CR2_FREQ        0    // Peripheral clock frequency field starts at bit 0
#define I2C_CR2_ITERREN     8    // Error interrupt enable
#define I2C_CR2_ITEVTEN     9    // Event interrupt enable
#define I2C_CR2_ITBUFEN     10   // Buffer interrupt enable: enables TxE/RxNE interrupts


/*
 * Bit position definitions I2C_OAR1
 * STM32F411xx - RM0383, Section 18.6.3
 */
#define I2C_OAR1_ADD0       0    // Address bit 0 used in 10-bit addressing mode
#define I2C_OAR1_ADD71      1    // 7-bit own-address field starts at bit 1
#define I2C_OAR1_ADD98      8    // Upper address bits used in 10-bit addressing mode
#define I2C_OAR1_ADDMODE    15   // Addressing mode: 0 = 7-bit, 1 = 10-bit


/*
 * Bit position definitions I2C_SR1
 * STM32F411xx - RM0383, Section 18.6.6
 */
#define I2C_SR1_SB          0    // Start Bit: START condition has been generated
#define I2C_SR1_ADDR        1    // Address sent/matched successfully
#define I2C_SR1_BTF         2    // Byte Transfer Finished
#define I2C_SR1_ADD10       3    // 10-bit header has been sent
#define I2C_SR1_STOPF       4    // STOP condition detected in slave mode
#define I2C_SR1_RXNE        6    // Receive data register not empty
#define I2C_SR1_TXE         7    // Transmit data register empty
#define I2C_SR1_BERR        8    // Bus error detected
#define I2C_SR1_ARLO        9    // Arbitration lost in master mode
#define I2C_SR1_AF          10   // Acknowledge failure: NACK received
#define I2C_SR1_OVR         11   // Overrun/underrun error
#define I2C_SR1_TIMEOUT     14   // Timeout or Tlow error detected


/*
 * Bit position definitions I2C_SR2
 * STM32F411xx - RM0383, Section 18.6.7
 */
#define I2C_SR2_MSL         0    // Master/Slave: 1 = master mode
#define I2C_SR2_BUSY        1    // Bus Busy: 1 = communication currently in progress
#define I2C_SR2_TRA         2    // Transmitter/Receiver: 1 = transmitter, 0 = receiver
#define I2C_SR2_GENCALL     4    // General call address received
#define I2C_SR2_DUALF       7    // Dual address flag: matched secondary own address


/*
 * Bit position definitions I2C_CCR
 * STM32F411xx - RM0383, Section 18.6.8
 */
#define I2C_CCR_CCR         0    // Clock control field starts here; determines SCL frequency
#define I2C_CCR_DUTY        14   // Fast-mode duty cycle: 0 = 2, 1 = 16/9
#define I2C_CCR_FS          15   // I2C mode: 0 = Standard mode, 1 = Fast mode



/******************************************************************************************
 *Bit position definitions of USART peripheral
 ******************************************************************************************/

/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK					0
#define USART_CR1_RWU 					1
#define USART_CR1_RE  					2
#define USART_CR1_TE 					3
#define USART_CR1_IDLEIE 				4
#define USART_CR1_RXNEIE  				5
#define USART_CR1_TCIE					6
#define USART_CR1_TXEIE					7
#define USART_CR1_PEIE 					8
#define USART_CR1_PS 					9
#define USART_CR1_PCE 					10
#define USART_CR1_WAKE  				11
#define USART_CR1_M 					12
#define USART_CR1_UE 					13
#define USART_CR1_OVER8  				15



/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD   				0
#define USART_CR2_LBDL   				5
#define USART_CR2_LBDIE  				6
#define USART_CR2_LBCL   				8
#define USART_CR2_CPHA   				9
#define USART_CR2_CPOL   				10
#define USART_CR2_STOP   				12
#define USART_CR2_LINEN   				14


/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE   				0
#define USART_CR3_IREN   				1
#define USART_CR3_IRLP  				2
#define USART_CR3_HDSEL   				3
#define USART_CR3_NACK   				4
#define USART_CR3_SCEN   				5
#define USART_CR3_DMAR  				6
#define USART_CR3_DMAT   				7
#define USART_CR3_RTSE   				8
#define USART_CR3_CTSE   				9
#define USART_CR3_CTSIE   				10
#define USART_CR3_ONEBIT   				11

/*
 * Bit position definitions USART_SR
 */

#define USART_SR_PE        				0
#define USART_SR_FE        				1
#define USART_SR_NE        				2
#define USART_SR_ORE       				3
#define USART_SR_IDLE       			4
#define USART_SR_RXNE        			5
#define USART_SR_TC        				6
#define USART_SR_TXE        			7
#define USART_SR_LBD        			8
#define USART_SR_CTS        			9


//#include "stm32f407xx_gpio_driver.h"
//#include "stm32f407xx_spi_driver.h"

#endif /* INC_STM32F407XX_H_ */
