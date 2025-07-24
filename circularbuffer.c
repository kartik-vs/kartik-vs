#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_BUFFER_SIZE 8
#define HALF_FILLED_BUFFER 0x01
#define FULL_FILLED_BUFFER 0x02
#define RX_INTERRUPT_FLAG 0x04
#define TX_INTERRUPT_FLAG 0x08
static uint8_t tx_buffer[MAX_BUFFER_SIZE]= {0};
static uint8_t rx_buffer[MAX_BUFFER_SIZE]= {0};
volatile uint8_t uart_interrupt_flag = 0;
volatile uint8_t DMA_interrupt_flag = 0;
int main() {
    // function to write the code for circular buffer speacially used in the uart to send and receive the data with no issues
    //setting up the DMA structure to receive the messages from uart
    //setting up the uart to simulate the data is received or send
    //it should be done continously for at least 5 buffer max of uart
    

}

//UART interrupt for byte by byte data transfer or received

void Uart_interrupt(void) {
    if(uart_interrupt_flag && RX_INTERRUPT_FLAG) {
        // peform the task of moving the data from the buffer to the software buffer
    }

    if(uart_interrupt_flag && TX_INTERRUPT_FLAG) {
        // send some info or a callback that the data is transmitted.
    }
}

//this is the interrupt written to DMA interrupt when the data is half or full filled
void DMA_interrupt(void) {
    if(DMA_interrupt_flag && HALF_FILLED_BUFFER) {
        // peform the task of moving the data from the buffer to the software buffer first half
    }

    if(DMA_interrupt_flag && FULL_FILLED_BUFFER) {
        // peform the task of moving the data from the buffer to the software buffer second half
    }


}