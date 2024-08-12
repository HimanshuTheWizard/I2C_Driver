#ifndef STM32F407_I2C_DRIVER_H_
#define STM32F407_I2C_DRIVER_H_

#include "stm32f407xx.h"

/*I2C Configuration Structure*/
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_ACKControl;
	uint8_t I2C_FMDutyCycle;
}I2C_Config_t;

/*I2C Handle Structure*/
typedef struct
{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;
}I2C_Handler_t;

/*user defined macros*/
#define I2C_SCL_SPEED_SM 			100000
#define I2C_SCL_SPEED_FM_4K 		400000
#define I2C_SCL_SPEED_FM_2K 		200000

#define I2C_ACK_ENABLE 				1
#define I2C_ACK_DISABLE				0
#define MY_ADDRESS					0x61
#define SLAVE_ADDRESS				0

#define I2C_FM_DUTY_CYCLE_2   		0
#define I2C_FM_DUTY_CYCLE_16_9  	1

#define START_CONDITION				8
#define STOP_CONDITION				9
#define FLAG_SET					1
#define FLAG_RESET					0
#define I2C_DISABLE_SR				0
#define I2C_ENABLE_SR				1

#define ENABLE						1
#define DISABLE						0

/*Flags*/
#define START_CONDITION_GENERATED	0x00000001
#define ADD_ACK_RECEIVED			0x00000002
#define TXE_FLAG_STATUS				0x00000080
#define BTF_FLAG_STATUS				0x00000004
#define RXNE_FLAG_STATUS			0x00000040

/*function declaration*/
uint32_t Get_Pll_Clock(void);
uint8_t I2c_GetFlagStatus(I2C_Handler_t *pHandler, uint32_t flag_bit);
void I2C_ExecuteAddressPhaseWrite(I2C_Handler_t *pHandler, uint8_t SlaveAdd);
void I2C_ClearAddFlag(I2C_Handler_t *pHandler);
void I2C_GenerateStop(I2C_Handler_t *pHandler);
void I2C_MasterSendData(I2C_Handler_t *pHandler, uint8_t SlaveAdd, uint8_t *pData, uint8_t size, uint8_t Sr);
void I2C_Init(I2C_Handler_t *pHandler);
void I2C_Generate_Start_Condition(I2C_Handler_t *pHandler);
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
void I2C_MasterReceiveData(I2C_Handler_t *pHandler, uint8_t SlaveAdd, uint8_t *pRcvBuffer, uint8_t size);



#endif
