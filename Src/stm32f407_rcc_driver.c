#include "stm32f407_rcc_driver.h"

uint16_t ahb_divisor_arr[] = {2,4,8,16,32,64,128,256,512};
uint16_t apb_divisor_arr[] = {2,4,8,16};

uint32_t Get_Peripheral_Clock1(void)
{
	uint32_t pclk;
	int32_t APBP = 0;
	int32_t AHBP = 0;
	uint32_t System_Clock;
	uint32_t temp_clock_type;
	uint32_t temp_AHB_Prescaler;
	uint32_t temp_APB_Prescaler;

	/*Getting the status of which clock being used*/
	temp_clock_type = (RCC->CFGR >> 2);
	if(temp_clock_type == 0U)
	{
		System_Clock = 16000000U;
	}
	else if(temp_clock_type == 1U)
	{
		System_Clock = 8000000U;
	}
	else if(temp_clock_type == 2U)
	{
		System_Clock = Get_Pll_Clock();
	}
	else
	{
		/*Do nothing*/
	}

	/*Extracting the AHB pre-scaler*/
	temp_AHB_Prescaler = ((RCC->CFGR >> 4) & 0xF);
	if(temp_AHB_Prescaler < 8)
	{
		AHBP = 0;
	}
	else
	{
		AHBP = ahb_divisor_arr[temp_AHB_Prescaler - 8];
	}

	/*extracting the APB pre-scaler*/
	temp_APB_Prescaler = ((RCC->CFGR >> 10) & 0x7);
	if(temp_APB_Prescaler < 4)
	{
		APBP = 0;
	}
	else
	{
		APBP = apb_divisor_arr[temp_APB_Prescaler - 4];
	}

	pclk = (System_Clock/AHBP)/APBP;
	return pclk;
}
