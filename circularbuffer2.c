#include <stdio.h>
#include <stdint.h>

#define MAX_BUFFER_SIZE       8
#define HALF_FILLED_BUFFER    0x01
#define FULL_FILLED_BUFFER    0x02
#define RX_INTERRUPT_FLAG     0x04
#define TX_INTERRUPT_FLAG     0x08

static uint8_t tx_buffer[MAX_BUFFER_SIZE] = {0};  // Software TX buffer
static uint8_t rx_buffer[MAX_BUFFER_SIZE] = {0};  // Software RX buffer

// Simulate DMA and UART HW Buffers (in real embedded, these would be registers)
static uint8_t DMA_buffer[MAX_BUFFER_SIZE] = {0};
static uint8_t UART_data = 0;

volatile uint8_t uart_interrupt_flag = 0;
volatile uint8_t DMA_interrupt_flag = 0;

uint8_t tx_head = 0;
uint8_t tx_tail = 0;
uint8_t rx_head = 0;
uint8_t rx_tail = 0;

// Emulate writing to TX buffer
void send_byte(uint8_t data) {
    uint8_t next = (tx_head + 1) % MAX_BUFFER_SIZE;
    if (next != tx_tail) {  // Check buffer full
        tx_buffer[tx_head] = data;
        tx_head = next;
        uart_interrupt_flag |= TX_INTERRUPT_FLAG;  // trigger TX
    }
}

// Emulate reading from RX buffer
uint8_t read_byte(void) {
    if (rx_head == rx_tail) {
        return 0xFF;  // Buffer empty
    }
    uint8_t data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % MAX_BUFFER_SIZE;
    return data;
}

// UART interrupt for byte-by-byte RX/TX
void Uart_interrupt(void) {
    if (uart_interrupt_flag & RX_INTERRUPT_FLAG) {
        rx_buffer[rx_head] = UART_data;
        rx_head = (rx_head + 1) % MAX_BUFFER_SIZE;
        uart_interrupt_flag &= ~RX_INTERRUPT_FLAG;
    }

    if (uart_interrupt_flag & TX_INTERRUPT_FLAG) {
        if (tx_tail != tx_head) {
            UART_data = tx_buffer[tx_tail];
            tx_tail = (tx_tail + 1) % MAX_BUFFER_SIZE;
            printf("TX sent: %d\n", UART_data);  // simulate TX output
        } else {
            uart_interrupt_flag &= ~TX_INTERRUPT_FLAG;
        }
    }
}

// DMA interrupt moves block data to RX buffer
void DMA_interrupt(void) {
    if (DMA_interrupt_flag & HALF_FILLED_BUFFER) {
        for (int i = 0; i < MAX_BUFFER_SIZE / 2; i++) {
            rx_buffer[rx_head] = DMA_buffer[i];
            rx_head = (rx_head + 1) % MAX_BUFFER_SIZE;
        }
        DMA_interrupt_flag &= ~HALF_FILLED_BUFFER;
    }

    if (DMA_interrupt_flag & FULL_FILLED_BUFFER) {
        for (int i = MAX_BUFFER_SIZE / 2; i < MAX_BUFFER_SIZE; i++) {
            rx_buffer[rx_head] = DMA_buffer[i];
            rx_head = (rx_head + 1) % MAX_BUFFER_SIZE;
        }
        DMA_interrupt_flag &= ~FULL_FILLED_BUFFER;
    }
}

int main(void) {
    // Simulate DMA filling buffer
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        DMA_buffer[i] = i + 1;
    }

    DMA_interrupt_flag |= HALF_FILLED_BUFFER;
    DMA_interrupt();
    DMA_interrupt_flag |= FULL_FILLED_BUFFER;
    DMA_interrupt();

    // Simulate reading from rx_buffer
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        uint8_t val = read_byte();
        if (val != 0xFF) {
            printf("RX received: %d\n", val);
        }
    }

    // Simulate TX buffer
    send_byte(10);
    send_byte(20);
    send_byte(30);
    Uart_interrupt();  // TX 10
    Uart_interrupt();  // TX 20
    Uart_interrupt();  // TX 30

    return 0;
}
