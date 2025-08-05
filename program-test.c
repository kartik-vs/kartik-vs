#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct code{
    int16_t value;
    uint8_t id;
    struct code* next;
}Code;
typedef Code* pcode;

Code Coding_var1, Coding_var2;

int main() {
    Coding_var1.id = 1;
    Coding_var1.value = 11;
    Coding_var1.next = &Coding_var2;

    Coding_var2.id = 2;
    Coding_var2.value = 22;
    Coding_var2.next = NULL;

    pcode ptr = &Coding_var1;

    while(ptr != NULL) {
        printf("ID: %d\n", ptr->id);
        printf("Value: %d\n", ptr->value);
        ptr = ptr->next;
    }
}