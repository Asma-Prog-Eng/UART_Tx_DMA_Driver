
#include "stm32f4xx.h"

#include "UART.h"

#include <stdio.h>

#define GPIODEN            				(1U << 3)

#define GPIOAEN            				(1U << 0)

#define AF7_Tx             				((1U << 8) |(1U << 9) | (1U << 10))

#define GPIOA_MODER_1      				(1U << 4)

#define GPIOA_MODER_2      				(1U << 5)

#define GPIOD_MODER_1   				(1U << 24)

#define GPIOD_MODER_2   				(1U << 25)

#define GPIOD_MODER_3   				(1U << 30)

#define GPIOD_MODER_4   				(1U << 31)

#define SR_TXE             				(1U << 7)

#define CR1_TE             				(1U << 3)

#define CR1_UE             				(1U << 13)

#define SysClk             				16000000UL

#define PeriphClk          				SysClk

#define CR3_DMAT           				(1U << 7)

#define AHB1ENR_DMA1EN                  (1U << 21)

#define DMA_DIR_MEM_TO_PERIPH            (1U <<6)

#define CR_CHSEL4          				(1U << 27)

#define CR_PSIZE         	 			((1U << 11) | (1U << 12))

#define CR_MSIZE          				((1U << 13) | (1U << 14))

#define FCR_DMDIS         				(1U << 0)

#define DMA_CR_EN          				(1U << 0)

#define DMA_CR_MEM_INC       		    (1U << 10)

#define DMA_CR_TCIE                     (1U << 4)

void uart_tx_init(USART_TypeDef *USARTx, uint32_t Baudrate) {


  	// configure baud rate

  	    BaudeRate(USARTx, Baudrate);

  	// set tranfer direction

  	   USARTx->CR1 |= CR1_TE ;

     // enable usart periph

      USARTx->CR1 |= CR1_UE;


}


void dma1_stream6_init (USART_TypeDef *USARTx, uint32_t src, uint32_t dst, uint32_t len){

	    // disable global interrupts

	    __disable_irq();

		// enable clock access for DMA1

		RCC->AHB1ENR |= AHB1ENR_DMA1EN;

		// disable DMA1 stream 6

		DMA1_Stream6->CR &= ~DMA_CR_EN;

		// wait till DMA1 stream 6 is disabled

		while(DMA1_Stream6->CR & DMA_CR_EN){};

		// clear all interrupt flags of stream 6

		 DMA1->HISR  |= ( (1U << 16) |  (1U << 18) |  (1U << 19) |  (1U << 20) |  (1U << 21));

		// configure transfer direction:  memory to periph

		DMA1_Stream6->CR |= DMA_DIR_MEM_TO_PERIPH;

		// DMA channel selection

		DMA1_Stream6->CR |= CR_CHSEL4  ;

		// Configure source/destination address

		// Source address :  memory address

		DMA1_Stream6->M0AR = src;

		// destination address : address of periph ,data register address, (USARTx->DR)

		DMA1_Stream6->PAR = dst;

		// Set length

		DMA1_Stream6->NDTR = len;

		// Transfer size : 8-bit

		DMA1_Stream6->CR &= ~CR_PSIZE;

		DMA1_Stream6->CR &= ~CR_MSIZE;

		//  Transfer type : direct mode

		DMA1_Stream6->FCR &= ~FCR_DMDIS;

		// Enable memory increment

		DMA1_Stream6->CR |= DMA_CR_MEM_INC  ;

		// Enable transfer complete interrupt

		DMA1_Stream6->CR |=  DMA_CR_TCIE  ;

	    // Enable Stream6 DMA

	   DMA1_Stream6->CR |= DMA_CR_EN;

		// enable UART transmitter DMA mode

	    USARTx->CR3 |= CR3_DMAT;

		// enable global interrupts

	     __enable_irq();

	    // Enable DMA interrupt enable in  NVIC

	    NVIC_EnableIRQ(DMA1_Stream6_IRQn);



}

void uart_write(USART_TypeDef *USARTx, char ch){

	 // wait till character is transmitted from data register

	// data register is empty

	while (!( USARTx->SR & SR_TXE )) {};

	// write data to data register

	USARTx->DR |= ch;


}

void GPIO_Init(void){

	// enable clock acess for port A and D

	RCC->AHB1ENR |=  (GPIOAEN |  GPIODEN );

	// set pin PA2 mode to alternate (0x2)

	GPIOA->MODER &=~ GPIOA_MODER_1;

	GPIOA->MODER |= GPIOA_MODER_2;

	// set alternate function to AF7

	GPIOA->AFR[0] |= AF7_Tx ;

	// set PD12 pin mode to digital output : 01

	 GPIOD->MODER |= GPIOD_MODER_1;

	 GPIOD->MODER &= ~GPIOD_MODER_2;

	 // set PD15 pin mode to digital output : 01

	 GPIOD->MODER |= GPIOD_MODER_3;

	 GPIOD->MODER &= ~GPIOD_MODER_4;

}

/*
 * USARTDiv = fclck/(Baude Rate * 16)
 * Exp : for baud rate = 9600 bps
 * USARTDiv= 104.166
 * Mantissa = 104 [4:12]
 * Fraction = 0.166 * 16 = 3 [0:3]
 * Baude Rate = {4800, 9600, 115200} ;
 *
 */

void  BaudeRate(USART_TypeDef *USARTx, uint32_t baudrate) {



	if (baudrate == 4800) {

		USARTx->BRR |= (1U << 8) | (1U << 10) | (1U << 11);

		USARTx->BRR |=  (1U << 1);
	}

	else if (baudrate == 9600) {

		USARTx->BRR |= (1U << 7) | (1U << 9 ) | (1U << 10);

		USARTx->BRR |= (1U << 0) | (1U << 1);
	}

	else if (baudrate == 115200) {

		USARTx->BRR |= (1U << 7);

		USARTx->BRR |= (1U << 0) | (1U << 1) | (1U << 3);
	}

	else {

		printf("Select baud rate in {4800, 9600, 115200}\n");
	}
}



