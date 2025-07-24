#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_BUFFER 3

typedef struct circularbuffer* pcircle;

typedef struct circularbuffer {
    uint8_t head;
    uint8_t tail;
    uint8_t buffer[MAX_BUFFER];
    uint8_t count;
}circularbuffer;

//function to write the data in the buffer

bool CB_write_buffer(circularbuffer* ptr, uint8_t data) {
    if (ptr->count == MAX_BUFFER)
        return false;
    ptr->buffer[ptr->head] = data;
    ptr->head = (ptr->head + 1)% MAX_BUFFER;
    ptr->count += 1;
    return true;
}

bool CB_remove_buffer(circularbuffer* ptr, uint8_t* data) {
    if (ptr->count == 0)
        return false;
    *data = ptr->buffer[ptr->tail];
    ptr->tail = (ptr->tail + 1)% MAX_BUFFER;
    ptr->count -= 1;
    return true;
}
void CB_print(circularbuffer* cb) {
    printf("Buffer contents: ");
    uint8_t i = cb->tail;
    for (uint8_t c = 0; c < cb->count; c++) {
        printf("%d ", cb->buffer[i]);
        i = (i + 1) % MAX_BUFFER;
    }
    printf("\n");
}

int main() {
    circularbuffer CB;
    
    CB.count = 0;
    CB.head = 0;
    CB.tail = 0;
    for(int i =0; i<MAX_BUFFER; i++) {
        CB.buffer[i] = 0;
    }
    
    pcircle ptr = &CB;

    for(uint8_t i= 0; i<=MAX_BUFFER;i++)
    {
        //function to do that
        if(CB_write_buffer(ptr, i))
            printf("Data is written to the space, %d\n", i);
        else
            printf("Data cannot be written any more\n");

    }

    CB_print(ptr);
    uint8_t value;
    for(uint8_t i= 0; i<=0;i++)
    {
        //function to do that
        if(CB_remove_buffer(ptr, &value))
            printf("Removed from the space, %d\n", value);
        else
            printf("Data buffer is empty\n");

    }
    CB_print(ptr);
    
    return 0;
}