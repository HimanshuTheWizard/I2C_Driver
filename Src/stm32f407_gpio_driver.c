#include "stm32f407_gpio_driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
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
		else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
		else
		{
			/*To-Do*/
		}
	}
	else
	{
		//TODO
	}

}

void GPIO_Init(GPIO_Handler_t *pGpioHandler)
{
	uint32_t temp;
	uint32_t temp1;
	uint32_t temp2;
	/* 1. Enable Peripheral CLock*/
	GPIO_PeriClockControl(pGpioHandler->pGPIOx, ENABLE);

	/* 2. Configure the mode*/
	temp = pGpioHandler->GPIOConfig.GPIO_PinMode << (2*pGpioHandler->GPIOConfig.GPIO_PinNumber);
	pGpioHandler->pGPIOx->MODER &= ~(0x3 << (2*pGpioHandler->GPIOConfig.GPIO_PinNumber));
	pGpioHandler->pGPIOx->MODER |= temp;

	/* 3. Configure the speed*/
	temp = pGpioHandler->GPIOConfig.GPIO_PinSpeed << (2*pGpioHandler->GPIOConfig.GPIO_PinNumber);
	pGpioHandler->pGPIOx->OSPEEDR &= ~(0x3 << (2*pGpioHandler->GPIOConfig.GPIO_PinNumber));
	pGpioHandler->pGPIOx->OSPEEDR |= temp;

	/* 4. Configure the PUPD Setting*/
	temp = pGpioHandler->GPIOConfig.GPIO_PinPuPdControl << (2*pGpioHandler->GPIOConfig.GPIO_PinNumber);
	pGpioHandler->pGPIOx->PUPDR &= ~(0x3 << (2*pGpioHandler->GPIOConfig.GPIO_PinNumber));
	pGpioHandler->pGPIOx->PUPDR |= temp;

	/* 5. Configure the optype*/
	temp = pGpioHandler->GPIOConfig.GPIO_PinOPType << (pGpioHandler->GPIOConfig.GPIO_PinNumber);
	pGpioHandler->pGPIOx->OTYPER &= ~(0x1 << (pGpioHandler->GPIOConfig.GPIO_PinNumber));
	pGpioHandler->pGPIOx->OTYPER |= temp;

	/* 6. Configure the alt functionality*/
	if(pGpioHandler->GPIOConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		temp1 = pGpioHandler->GPIOConfig.GPIO_PinNumber/8;
		temp2 = pGpioHandler->GPIOConfig.GPIO_PinNumber%8;
		pGpioHandler->pGPIOx->AFR[temp1] &= ~(0xf << (4*temp2));
		pGpioHandler->pGPIOx->AFR[temp1] |= (pGpioHandler->GPIOConfig.GPIO_PinAltFunMode << (4*temp2));
	}
}













