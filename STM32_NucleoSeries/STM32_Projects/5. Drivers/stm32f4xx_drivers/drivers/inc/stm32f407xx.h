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

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __vo volatile


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
#define PERIPH_BASEADDR		0x40000000U	// this is the address of the first peripheral
#define APB1PERIPH_BASEADDR	PERIPH_BASE
#define APB2PERIPH_BASEADDR	0x40010000U
#define AHB1PERIPH_BASEADDR	0x40020000U
#define AHB2PERIPH_BASEADDR	0x50000000U


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
#define I2C1_BASEADDR	(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR	(APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR	(APB1PERIPH_BASE + 0x5C00)


// this microcontroller has actually 4 SPI peripherals
#define SPI2_BASE		(APB1PERIPH_BASE + 0x38000)
#define SPI3_BASE		(APB1PERIPH_BASE + 0x3C000)

#define USART2_BASE		(APB1PERIPH_BASE + 0x4400)
#define USART3_BASE		(APB1PERIPH_BASE + 0x4800)
#define UART4_BASE		(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE		(APB1PERIPH_BASE + 0x5000)

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
 * peripheral register definition structure for GPIO
 */
typedef struct
{
	__vo uint32_t MODER;      /*!< GPIO port mode register,        Address offset: 0x00 */
	__vo uint32_t OTYPER;     /*!< GPIO port output type register, Address offset: 0x04 */
	__vo uint32_t OSPEEDR;    /*!< GPIO port output speed register, Address offset: 0x08 */
	__vo uint32_t PUPDR;      /*!< GPIO port pull-up/pull-down register, Address offset: 0x0C */
	__vo uint32_t IDR;        /*!< GPIO port input data register, Address offset: 0x10 */
	__vo uint32_t ODR;        /*!< GPIO port output data register, Address offset: 0x14 */
	__vo uint32_t BSRRL;      /*!< GPIO port bit set/reset low register, Address offset: 0x18 */
	__vo uint32_t BSRRH;      /*!< GPIO port bit set/reset high register, Address offset: 0x1A */
	__vo uint32_t LCKR;       /*!< GPIO port configuration lock register, Address offset: 0x1C */
	__vo uint32_t AFR[2];     /*!< GPIO alternate function registers, Address offset: 0x20-0x24. Instead of creating 2 separate variable, I just created an array her (AFRL & AFRH for alternate functions)*/
} GPIO_RegDef_t;
//GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t *)0x40020000;
/*
 * The diference between USART and UART is that UART DOESNT support synchronous communication! No output clock production or serial clock
 */


/*
 * peripheral register definition structure for RCC
 */
typedef struct
{
    __vo uint32_t CR;          /* Address offset: 0x00 */
    __vo uint32_t PLLCFGR;     /* Address offset: 0x04 */
    __vo uint32_t CFGR;        /* Address offset: 0x08 */
    __vo uint32_t CIR;         /* Address offset: 0x0C */

    __vo uint32_t AHB1RSTR;    /* Address offset: 0x10 */
    __vo uint32_t AHB2RSTR;    /* Address offset: 0x14 */
    __vo uint32_t AHB3RSTR;    /* Address offset: 0x18 */

    uint32_t RESERVED0;        /* Reserved, 0x1C */

    __vo uint32_t APB1RSTR;    /* Address offset: 0x20 */
    __vo uint32_t APB2RSTR;    /* Address offset: 0x24 */

    uint32_t RESERVED1[2];     /* Reserved, 0x28-0x2C */

    __vo uint32_t AHB1ENR;     /* Address offset: 0x30 */
    __vo uint32_t AHB2ENR;     /* Address offset: 0x34 */
    __vo uint32_t AHB3ENR;     /* Address offset: 0x38 */

    uint32_t RESERVED2;        /* Reserved, 0x3C */

    __vo uint32_t APB1ENR;     /* Address offset: 0x40 */
    __vo uint32_t APB2ENR;     /* Address offset: 0x44 */

    uint32_t RESERVED3[2];     /* Reserved, 0x48-0x4C */

    __vo uint32_t AHB1LPENR;   /* Address offset: 0x50 */
    __vo uint32_t AHB2LPENR;   /* Address offset: 0x54 */
    __vo uint32_t AHB3LPENR;   /* Address offset: 0x58 */

    uint32_t RESERVED4;          /*!< Reserved, 0x5C */

    __vo uint32_t APB1LPENR;     /*!< Address offset: 0x60 */
    __vo uint32_t APB2LPENR;     /*!< Address offset: 0x64 */

    uint32_t RESERVED5[2];       /*!< Reserved, 0x68-0x6C */

    __vo uint32_t BDCR;          /*!< Address offset: 0x70 */
    __vo uint32_t CSR;           /*!< Address offset: 0x74 */

    uint32_t RESERVED6[2];       /*!< Reserved, 0x78-0x7C */

    __vo uint32_t SSCGR;         /*!< Address offset: 0x80 */
    __vo uint32_t PLLI2SCFGR;    /*!< Address offset: 0x84 */
    __vo uint32_t PLLSAICFGR;    /*!< Address offset: 0x88 */
    __vo uint32_t DCKCFGR;       /*!< Address offset: 0x8C */
    __vo uint32_t CKGATENR;      /*!< Address offset: 0x90 */
    __vo uint32_t DCKCFGR2;      /*!< Address offset: 0x94 */
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
 * peripheral register definition structure for SPI (28.5.10)
 */
typedef struct
{
	__vo uint32_t CR1;        /*!< TODO,     										Address offset: 0x00 */
	__vo uint32_t CR2;        /*!< TODO,     										Address offset: 0x04 */
	__vo uint32_t SR;         /*!< TODO,     										Address offset: 0x08 */
	__vo uint32_t DR;         /*!< TODO,     										Address offset: 0x0C */
	__vo uint32_t CRCPR;      /*!< TODO,     										Address offset: 0x10 */
	__vo uint32_t RXCRCR;     /*!< TODO,     										Address offset: 0x14 */
	__vo uint32_t TXCRCR;     /*!< TODO,     										Address offset: 0x18 */
	__vo uint32_t I2SCFGR;    /*!< TODO,     										Address offset: 0x1C */
	__vo uint32_t I2SPR;      /*!< TODO,     										Address offset: 0x20 */
} SPI_RegDef_t;

/*
 * Peripheral register definition structure for SYSCFG (9.3.3)
 */
typedef struct
{
    __vo uint32_t MEMRMP;          /* Address offset: 0x00 */
    __vo uint32_t PMC;             /* Address offset: 0x04 */
    __vo uint32_t EXTICR[4];       /* Address offset: 0x08 - 0x14 */ //Its only purpose is to tell the EXTI controller which GPIO port is connected to each EXTI line. It does not enable interrupts or configure trigger edges
    uint32_t RESERVED1[2];         /* Reserved, 0x18 - 0x1C */
    __vo uint32_t CMPCR;           /* Address offset: 0x20 */
    uint32_t RESERVED2[2];         /* Reserved, 0x24 - 0x28 */
    __vo uint32_t CFGR;            /* Address offset: 0x2C */

} SYSCFG_RegDef_t;


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

/*
 * Clock Enable Macros for SPIx peripherals
 */

/*
 * Clock Enable Macros for USARTx peripherals
 */


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

#endif /* INC_STM32F407XX_H_ */
