/*
 * project_timer2_5.c
 *
 *  Created on: Mar 29, 2025
 *      Author: hasan
 */

#include "stm32f407xx.h"
#include "project_timer2_5.h"

void TIM2_IRQInterrupt_Config(uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		*NVIC_ISER0 |= ( 1 << 28 );
	}else
	{
		*NVIC_ICER0 |= ( 1 << 28 );
	}
}

void TIM5_IRQInterrupt_Config(uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		*NVIC_ISER1 |= ( 1 << (50 % 32) );
	}else
	{
		*NVIC_ICER1 |= ( 1 << (50 % 32) );
	}
}

void TIM3_IRQInterrupt_Config(uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        *NVIC_ISER0 |= (1 << (29 % 32));  // TIM3_IRQn = 29
    }
    else
    {
        *NVIC_ICER0 |= (1 << (29 % 32));
    }
}

