Bare metal implementation  of UART DMA transmitter Driver for STM32 development board 
## Features
UART2 initialization with configurable  baud rates (4800, 9600 ,115200) and peripheral clock = 8MHz.
## Requirements
### Hardware
- Discovery STM32 development board (STM32F411x series)
- AZ-Delivery UART-TTL USB adapter
- Multimeter  (optional for debugging)
### Software
- STM32CubeIDE or compatible toolchain
- RealTerm
  
## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/Asma-Prog-Eng/UART_Tx_DMA_Driver
   
2. Import project into STM32CubeIDE:
File → Import... → Existing Projects into Workspace

3. Update the include path directories ,  to CMSIS folder ( under Project properties -> C/C++ General -> Includes : delete existing CMSIS path directory and  add the path to CMSIS folder <br />,
   that is included in the project, : Add -> File System <br />

4. Rebuild project dependenciesFile 

## Usage
Initialization : <br />
GPIO_Init() <br />
dma1_stream6_init (USART2, (uint32_t) message, (uint32_t) &USART2->DR, 31) <br />
uart_tx_init(USART2, 115200) <br />
Transmitting Data using DMA : DMA1_Stream6_IRQHandler() <br />
a message of size 31 Bytes is transmitted, to Realterm terminal,  each time reset button is pressed <br />
two callback funtions are used to perform two different actions depending on the status of the transfered data <br />
(Can be ported for bigger data size) <br />
DMA1_Stream6_callback_2(void) <br />
DMA1_Stream6_callback_1 <br />

## Project Structure

├── Core<br />
├── Inc<br />  → uart.h <br />
├── Src<br /> → uart.c<br /> → main.c

## Troubleshooting

No data received: <br />
- Verify baud rate settings <br />
- Check TX/RX pin connections <br />
- Ensure correct clock configuration <br />

Garbage characters: <br />
- Verify clock source accuracy <br />
- Check voltage levels <br />

## Contributing
Pull requests are welcome. For major changes, open an issue first.

## License
MIT License <br />
Copyright (c) [2025] [Asma Askri]
