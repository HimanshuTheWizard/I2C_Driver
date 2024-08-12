#include <stdint.h>
#include "stm32f407_i2c_driver.h"
#include "stm32f407_rcc_driver.h"
#include "stm32f407_gpio_driver.h"
/*
 * I2C Peripherals are on APB1 bus
 *
 * */

void delay(void);
int main(void)
{
	I2C_Handler_t I2C_handler;
	GPIO_Handler_t GPIO_handler;
	uint8_t pRcvBuffer[10];
	uint8_t i = 0;

	/*I2C and GPIO handlers are to be configured properly*/

	I2C_handler.pI2Cx    						= I2C1;
	I2C_handler.I2C_Config.I2C_SCLSpeed 		= I2C_SCL_SPEED_SM;
	I2C_handler.I2C_Config.I2C_DeviceAddress 	= MY_ADDRESS;//Applicable only for slave
	I2C_handler.I2C_Config.I2C_ACKControl 		= I2C_ACK_ENABLE;
	I2C_handler.I2C_Config.I2C_FMDutyCycle 		= I2C_FM_DUTY_CYCLE_2;

	GPIO_handler.pGPIOx							= GPIOA;
	GPIO_handler.GPIOConfig.GPIO_PinAltFunMode	= 0;
	GPIO_handler.GPIOConfig.GPIO_PinMode        = 0;
	GPIO_handler.GPIOConfig.GPIO_PinNumber      = 0;
	GPIO_handler.GPIOConfig.GPIO_PinOPType      = 0;
	GPIO_handler.GPIOConfig.GPIO_PinPuPdControl = 0;
	GPIO_handler.GPIOConfig.GPIO_PinSpeed       = 0;

	/*I2C GPIO Init*/
	GPIO_Init(&GPIO_handler);

	/*Init I2C peripheral*/
	I2C_Init(&I2C_handler);

	while(1)
	{
		I2C_MasterReceiveData(&I2C_handler, SLAVE_ADDRESS, pRcvBuffer+i, 1);
		i++;
		if(i == 9)
			i = 0;
		delay();
	}
}


void delay(void)
{
	uint32_t i = 0;
	while(i<1000000)
	{
		i--;
	}
}
