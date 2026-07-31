/*
 * STM32F407xx_i2c_driver.h
 *
 *  Created on: Jul 30, 2026
 *      Author: caxel
 */


#ifndef INC_STM32F407XX_I2C_DRIVER_H_	// these guards actually prevent inclusion of multiple definitions during the preprocessor stage of the compilation
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed; // can be normal or fast mode
	uint8_t  I2C_DeviceAddress; //It is actually 7-bit
	uint8_t  I2C_AckControl; // Disabled by default
	uint8_t  I2C_FMDutyCycle;

}I2C_Config_t;

/*
 *Handle structure for I2Cx peripheral
 */
typedef struct
{
    I2C_RegDef_t   *pI2Cx;       // Pointer to the base address of the I2C peripheral registers.
    I2C_Config_t    I2C_Config;  // Stores the I2C peripheral configuration settings.
    uint8_t        *pTxBuffer;    // Pointer to the application transmit (Tx) buffer.
    uint8_t        *pRxBuffer;    // Pointer to the application receive (Rx) buffer.
    uint32_t        TxLen;        // Stores the remaining number of bytes to transmit.
    uint32_t        RxLen;        // Stores the remaining number of bytes to receive.
    uint8_t         TxRxState;    // Stores the current I2C communication state (Tx, Rx, or Ready).
    uint8_t         DevAddr;      // Stores the slave/device I2C address.
    uint32_t        RxSize;       // Stores the original total receive size.
    uint8_t         Sr;           // Stores the repeated START setting/value.
} I2C_Handle_t;


/*
 * I2C application states
 */
#define I2C_READY 					0
#define I2C_BUSY_IN_RX 				1
#define I2C_BUSY_IN_TX 				2

/*
 * @I2C_SCLSpeed: If you need different speed than these, create a new macro
 */
#define I2C_SCL_SPEED_SM 	100000	// Speed Standard mode: (100KHz)
#define I2C_SCL_SPEED_FM4K 	400000  // Fast Mode
#define I2C_SCL_SPEED_FM2K  200000


/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE        1
#define I2C_ACK_DISABLE       0


/*
 * @I2C_FMDutyCycle: Ref. Manual 27.6.8 (I2C_CCR reg) - bit 14
 */
#define I2C_FM_DUTY_2        0    // Fast Mode duty cycle: Tlow/Thigh = 2 (duty cycle 2:1)
#define I2C_FM_DUTY_16_9     1    // Fast Mode duty cycle: Tlow/Thigh = 16/9 (duty cycle 16:9)


/*
 * I2C related status flags definitions
 */
#define I2C_FLAG_TXE      (1U << I2C_SR1_TXE)      // TXE: Transmit data register empty. DR is ready for new data.
#define I2C_FLAG_RXNE     (1U << I2C_SR1_RXNE)     // RXNE: Receive data register not empty. Received data is available in DR.
#define I2C_FLAG_SB       (1U << I2C_SR1_SB)       // SB: Start bit. START condition has been generated in Master mode.
#define I2C_FLAG_OVR      (1U << I2C_SR1_OVR)      // OVR: Overrun/Underrun error. Data was lost due to late access to DR.
#define I2C_FLAG_AF       (1U << I2C_SR1_AF)       // AF: Acknowledge failure. No ACK was received after transmitting a byte/address.
#define I2C_FLAG_ARLO     (1U << I2C_SR1_ARLO)     // ARLO: Arbitration lost. Master lost arbitration on the I2C bus.
#define I2C_FLAG_BERR     (1U << I2C_SR1_BERR)     // BERR: Bus error. Illegal START or STOP condition was detected.
#define I2C_FLAG_STOPF    (1U << I2C_SR1_STOPF)    // STOPF: STOP detection. STOP condition was detected in Slave mode.
#define I2C_FLAG_ADD10    (1U << I2C_SR1_ADD10)    // ADD10: 10-bit header sent. First part of a 10-bit address was transmitted.
#define I2C_FLAG_BTF      (1U << I2C_SR1_BTF)      // BTF: Byte transfer finished. Current byte transfer has completed.
#define I2C_FLAG_ADDR     (1U << I2C_SR1_ADDR)     // ADDR: Address sent/matched. Master received ACK or Slave address matched.
#define I2C_FLAG_TIMEOUT  (1U << I2C_SR1_TIMEOUT)  // TIMEOUT: Timeout/Tlow error. I2C bus timeout condition was detected.

#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET


/*
 * I2C application events macros
 */
#define I2C_EV_TX_CMPLT        0    // Event: I2C data transmission completed successfully.
#define I2C_EV_RX_CMPLT        1    // Event: I2C data reception completed successfully.
#define I2C_EV_STOP            2    // Event: STOP condition detected on the I2C bus.
#define I2C_ERROR_BERR         3    // Error: Bus error caused by an invalid START or STOP condition.
#define I2C_ERROR_ARLO         4    // Error: Arbitration lost while attempting to control the I2C bus.
#define I2C_ERROR_AF           5    // Error: Acknowledge failure. Slave did not send an ACK.
#define I2C_ERROR_OVR          6    // Error: Overrun/Underrun occurred because data was not handled in time.
#define I2C_ERROR_TIMEOUT      7    // Error: I2C communication timeout occurred.
#define I2C_EV_DATA_REQ        8    // Event: Slave transmitter is requested by the Master to send data.
#define I2C_EV_DATA_RCV        9    // Event: Slave receiver has received data from the Master.
/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);


/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);


void I2C_SlaveSendData(I2C_RegDef_t *pI2C,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C);

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);


/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);

/*
 * Application callback
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);

#endif

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_



#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
