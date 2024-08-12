#include "stm32f407_i2c_driver.h"
uint32_t System_Clock;

uint32_t Get_Pll_Clock(void)
{
	 return 0;
}

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}

}

/*I2C Peripheral Initialization*/

void I2C_Init(I2C_Handler_t *pHandler)
{
	uint32_t temp_reg = 0;

	/* Enable the clock*/
	I2C_PeriClockControl(pHandler->pI2Cx, ENABLE);

	/*1. update ack bit*/
	temp_reg |= (pHandler->I2C_Config.I2C_ACKControl<<10);
	pHandler->pI2Cx->CR1 = temp_reg;

	/*2. set frequency*/
	temp_reg = 0;
	temp_reg |= (Get_Peripheral_Clock1()/1000000U);
	pHandler->pI2Cx->CR2 = temp_reg & 0x3F;

	/*3. configure the address*/
	temp_reg = 0;
	temp_reg |= (pHandler->I2C_Config.I2C_DeviceAddress<<1);
	temp_reg |= (pHandler->I2C_Config.I2C_DeviceAddress<<14);
	pHandler->pI2Cx->OAR1 = temp_reg;

	/*4. mode configuration*/
	uint16_t ccr_val = 0;
	temp_reg = 0;
	if(pHandler->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		/*standard mode*/
		ccr_val = Get_Peripheral_Clock1()/(2*pHandler->I2C_Config.I2C_SCLSpeed);
		temp_reg |= (ccr_val & 0xFFF);
	}
	else
	{
		/*Fast mode*/
		temp_reg |= (1<<15);
		temp_reg |= (pHandler->I2C_Config.I2C_FMDutyCycle << 14);
		if(pHandler->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_CYCLE_2)
		{
			ccr_val |= (Get_Peripheral_Clock1()/(3*pHandler->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			ccr_val |= (Get_Peripheral_Clock1()/(25*pHandler->I2C_Config.I2C_SCLSpeed));
		}
		temp_reg |= (ccr_val & 0xFFF);
	}
	pHandler->pI2Cx->CCR = temp_reg;
}

void I2C_Generate_Start_Condition(I2C_Handler_t *pHandler)
{
	pHandler->pI2Cx->CR1 |= (1 << START_CONDITION);
}

uint8_t I2c_GetFlagStatus(I2C_Handler_t *pHandler, uint32_t flag_bit)
{
	if(pHandler->pI2Cx->SR1 & flag_bit)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}
void I2C_ExecuteAddressPhaseWrite(I2C_Handler_t *pHandler, uint8_t SlaveAdd)
{
	SlaveAdd = (SlaveAdd << 1);
	SlaveAdd &= (~0x1);
	pHandler->pI2Cx->DR = SlaveAdd;
}
void I2C_ClearAddFlag(I2C_Handler_t *pHandler)
{
	uint32_t dummy;
	dummy = pHandler->pI2Cx->SR1;
	dummy = pHandler->pI2Cx->SR2;
	(void)dummy;
}

void I2C_GenerateStop(I2C_Handler_t *pHandler)
{
	pHandler->pI2Cx->CR1 |= (1<<STOP_CONDITION);
}

/*I2C Master Data/Address Send*/


void I2C_ExecuteAddressPhaseRead(I2C_Handler_t *pHandler, uint8_t SlaveAdd)
{
	SlaveAdd = SlaveAdd<<1;
	SlaveAdd |= 0x1;
	pHandler->pI2Cx->DR = SlaveAdd;
}

void I2C_UpdateAckStatus(I2C_Handler_t *pHandler, uint8_t ack_flag)
{
	if(ack_flag == I2C_ACK_DISABLE)
	{
		pHandler->pI2Cx->CR1 &= ~(1<<I2C_ACK_DISABLE);
	}
	else if(ack_flag == I2C_ACK_ENABLE)
	{
		pHandler->pI2Cx->CR1 |= (1<<I2C_ACK_ENABLE);
	}
	else
	{
		/*Do Nothing*/
	}
}

void I2C_MasterReceiveData(I2C_Handler_t *pHandler, uint8_t SlaveAdd, uint8_t *pRcvBuffer, uint8_t size)
{
	uint8_t Sr = 0;
	/* 1. Generate the start Condition*/
	I2C_Generate_Start_Condition(pHandler);

	/* 2. Confirm if start condition generation is done by checking the SB flag
	 * in SR1 register, */
	while(!I2c_GetFlagStatus(pHandler, START_CONDITION_GENERATED));


	/* 3. Send the address of the slave with r/w=1(R)*/
	I2C_ExecuteAddressPhaseRead(pHandler, SlaveAdd);

	/* 4. Confirm that address phase is completed*/
	while(!I2c_GetFlagStatus(pHandler, ADD_ACK_RECEIVED));

	if(size == 1)
	{
		/* 5. Disable ACK*/
		I2C_UpdateAckStatus(pHandler,I2C_ACK_DISABLE);

		/* 6. Clear ADDR flag*/
		I2C_ClearAddFlag(pHandler);

		/* 7. Wait for RxNE to become 1*/
		while(!I2c_GetFlagStatus(pHandler, RXNE_FLAG_STATUS));

		/* 8. generate the stop condition*/
		if(Sr == I2C_DISABLE_SR)
			I2C_GenerateStop(pHandler);

		/* 9. Read data register*/
		*pRcvBuffer = pHandler->pI2Cx->DR;
	}
	else
	{
		while(size)
		{
			/* 5. Wait for RxNE to become 1*/
			while(!I2c_GetFlagStatus(pHandler, RXNE_FLAG_STATUS));

			if(size == 2)
			{
				I2C_UpdateAckStatus(pHandler,I2C_ACK_DISABLE);

				if(Sr == I2C_DISABLE_SR)
					I2C_GenerateStop(pHandler);
			}

			*pRcvBuffer = pHandler->pI2Cx->DR;

			pRcvBuffer++;
			size--;
		}
	}
}

void I2C_MasterSendData(I2C_Handler_t *pHandler, uint8_t SlaveAdd, uint8_t *pData, uint8_t size, uint8_t Sr)
{
	uint8_t i = 0;

	/* 1. Generate the start Condition*/
	I2C_Generate_Start_Condition(pHandler);

	/* 2. Confirm if start condition generation is done by checking the SB flag
	 * in SR1 register, */
	while(!I2c_GetFlagStatus(pHandler, START_CONDITION_GENERATED));

	/* 3. Send the address of the slave with r/w=0(w)*/
	I2C_ExecuteAddressPhaseWrite(pHandler, SlaveAdd);

	/* 4. Confirm that address phase is completed*/
	while(!I2c_GetFlagStatus(pHandler, ADD_ACK_RECEIVED));

	/* 5. Clear the ADDR flag*/
	I2C_ClearAddFlag(pHandler);

	/* 6. Send the data until length becomes 0*/
	while(size)
	{
		while(!I2c_GetFlagStatus(pHandler, TXE_FLAG_STATUS));
		pHandler->pI2Cx->DR = pData[i];
		i++;
		size--;
	}

	/* 7. Wait for TXE and BTF to become 0*/
	while(!I2c_GetFlagStatus(pHandler, TXE_FLAG_STATUS));
	while(!I2c_GetFlagStatus(pHandler, BTF_FLAG_STATUS));

	/* 8. generate the stop condition*/
	if(Sr == I2C_DISABLE_SR)
		I2C_GenerateStop(pHandler);
}








