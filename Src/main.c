/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Asma Askri
 * @brief          : Main program body
 ******************************************************************************
 *Bare metal implementation of a UART DMA transmitter Driver
 * If message is transmitted from memory to the uart data register (DMA), 
 * than two LEDS (LED4 and LED6) are toggled (Interrupt) accordingly
 * if data is half or fully transmitted
 * 
 ******************************************************************************
 */
#include "stm32f4xx.h"

#include "UART.h"

static void DMA1_Stream6_callback_1(void);
static void DMA1_Stream6_callback_2(void);

int main(void) {


    char message[31] = "Hello from stm32 DMA transfer\n\r";

	GPIO_Init();

	// enable clock access to USART2
    RCC->APB1ENR |= USARTEN ;

    dma1_stream6_init (USART2, (uint32_t) message, (uint32_t) &USART2->DR, 31);

    uart_tx_init(USART2, 115200) ;


	while (1) {


	}


}


// static so that it can be only accessed here

static void DMA1_Stream6_callback_1(void) {

	// Toggle PD12

	GPIOD->ODR ^= LED4;


}


// static so that it can be only accessed here

static void DMA1_Stream6_callback_2(void) {

	// Toggle PD15

	GPIOD->ODR ^= LED6;

}


void DMA1_Stream6_IRQHandler (void) {


	//check if transmission  is half completed

		if (DMA1->HISR & (1U << 20)){

			// clear flag

			 DMA1->HIFCR  |= (1U << 20) ;

			// do something

			DMA1_Stream6_callback_1();

		}



	//check if transmission  is fully completed

	if (DMA1->HISR & (1U << 21)){

		// clear flag

		 DMA1->HIFCR  |= (1U << 21) ;

		// do something

		DMA1_Stream6_callback_2();

	}


}




