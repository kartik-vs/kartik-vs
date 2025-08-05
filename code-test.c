#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node {
    uint8_t value;
    uint16_t delay;

    struct node* next;
};

int main() {
    uint8_t value;
    uint8_t* ptr;
    value = 0x33;
    
    printf("Unsiged Integer value:%u, Hex value:%X, Address Location of the value:%p\n", value, value, &value);

    ptr = &value;

    printf("%u, %X, %p\n", *ptr, *ptr, ptr);

    return 0;

}