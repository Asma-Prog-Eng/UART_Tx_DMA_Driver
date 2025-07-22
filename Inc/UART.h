

#ifndef UART_H_
#define UART_H_


#define USARTEN            (1U << 17)

#define LED6               (1U << 15)

#define LED4               (1U << 12)

void uart_tx_init(USART_TypeDef *USARTx, uint32_t Baudrate) ;

void uart_write(USART_TypeDef *USARTx, char ch);

void dma1_stream6_init (USART_TypeDef *USARTx, uint32_t src, uint32_t dst, uint32_t len);

void  BaudeRate(USART_TypeDef *USARTx, uint32_t baudrate);


void GPIO_Init(void);

#endif /* UART_H_ */
