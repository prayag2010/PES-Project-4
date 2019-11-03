/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    PES Project 4.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
//#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "loggerFunctions.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

volatile int i = 0;

void Init_SysTick(void)
{
	SysTick->LOAD = (42915344L);
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

bool loggerEnable = true;

int main(void) {
	/* Init board hardware. */
//	BOARD_InitBootPins();
//	BOARD_InitBootClocks();
//	BOARD_InitBootPeripherals();

	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	PORTD->PCR[1] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_MUX(1) | \
			PORT_PCR_IRQC(0x08);

	NVIC->ISER[0] |= (1 << PORTD_IRQn);

	printf("Hello World\n");
	Init_SysTick();
	printf("START\n");

#ifdef debug
	printf("DEBUG\n");
#endif

#ifdef test
	printf("test\n");
#endif

#ifdef normal
	printf("normal\n");
#endif

	log_message(__func__, "Hello");

	/* Force the counter to be placed into memory. */
	volatile static int i = 0 ;
	/* Enter an infinite loop, just incrementing a counter. */
	while(1) {
		i++ ;
		/* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
		__asm volatile ("nop");
	}
	return 0 ;

}

void PORTD_IRQHandler(void)
{
	PORTD->PCR[1] |= PORT_PCR_ISF_MASK;
	printf("INTERRUPT!\n");
}

void SysTick_Handler(void)  {                               /* SysTick interrupt Handler. */
	i++;                                      /* See startup file startup_LPC17xx.s for SysTick vector */
	if(i > 1){
		i = 0;
		printf("SYSTICK\n");
	}
}
