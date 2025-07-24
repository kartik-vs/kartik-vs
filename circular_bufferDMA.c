#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_BUFFER_SIZE 8
#define HALF_FILLED_BUFFER 0x01
#define FULL_FILLED_BUFFER 0x02
#define RX_INTERRUPT_FLAG 0x04
#define TX_INTERRUPT_FLAG 0x08

static uint8_t tx_buffer[MAX_BUFFER_SIZE] = {0};
static uint8_t rx_buffer[MAX_BUFFER_SIZE] = {0};
static uint8_t software_buffer[2 * MAX_BUFFER_SIZE] = {0}; // simulate circular buffer
static uint8_t sw_write_index = 0;

volatile uint8_t uart_interrupt_flag = 0;
volatile uint8_t DMA_interrupt_flag = 0;

void DMA_interrupt(void);
void uart_receive_byte_dma(uint8_t data);
void Uart_interrupt(void);

// Simulate DMA filling rx_buffer with data from UART
void uart_receive_byte_dma(uint8_t data) {
    static uint8_t dma_write_index = 0;

    rx_buffer[dma_write_index++] = data;

    if (dma_write_index == MAX_BUFFER_SIZE / 2) {
        DMA_interrupt_flag |= HALF_FILLED_BUFFER;
        DMA_interrupt(); // Trigger interrupt manually in simulation
    } else if (dma_write_index == MAX_BUFFER_SIZE) {
        DMA_interrupt_flag |= FULL_FILLED_BUFFER;
        DMA_interrupt(); // Trigger interrupt manually in simulation
        dma_write_index = 0; // wrap around for next full fill
    }
}

// UART interrupt stub (not used in this version, kept for structure)
void Uart_interrupt(void) {
    if (uart_interrupt_flag & RX_INTERRUPT_FLAG) {
        // process byte-by-byte receive
    }

    if (uart_interrupt_flag & TX_INTERRUPT_FLAG) {
        // process transmit complete
    }
}

// DMA interrupt to move data from rx_buffer to software_buffer
void DMA_interrupt(void) {
    if (DMA_interrupt_flag & HALF_FILLED_BUFFER) {
        for (int i = 0; i < MAX_BUFFER_SIZE / 2; i++) {
            software_buffer[sw_write_index++] = rx_buffer[i];
        }
        DMA_interrupt_flag &= ~HALF_FILLED_BUFFER;
    }

    if (DMA_interrupt_flag & FULL_FILLED_BUFFER) {
        for (int i = MAX_BUFFER_SIZE / 2; i < MAX_BUFFER_SIZE; i++) {
            software_buffer[sw_write_index++] = rx_buffer[i];
        }
        DMA_interrupt_flag &= ~FULL_FILLED_BUFFER;
    }
}

int main() {
    // Simulate UART receiving 16 bytes of data
    for (uint8_t i = 1; i <= 16; i++) {
        uart_receive_byte_dma(i);
    }

    printf("Software Buffer Contents:\n");
    for (int i = 0; i < sw_write_index; i++) {
        printf("%d ", software_buffer[i]);
    }
    printf("\n");

    return 0;
}
