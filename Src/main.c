/* Bare metal implementation of a USART DMA transmitter Driver
 * with configurable USART peripheral  ( USART1, USART2, USART6)
 * Features :
 * Tx Pin : PA2
 * Configurable Baud rate : {4800,9600,115200}
 * DMA interrupt is raised if reset button is pressed
 * If message is transmitted than two LEDS (LED4 and LED6) are toggled accordingly
 * if data is half or fully transmitted
 * After flashing the program into the board, the message is printed on RealTerm and
 * the LEDs are switched on
 * To test again, reset push button is to be pressed
 * External UART to USB adapter is used (AZ-Delivery)
 *
 * */

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




