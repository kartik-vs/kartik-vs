#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct node;
typedef uint16_t (*function)(struct node*);
typedef struct node {
    uint8_t value;
    struct node* next;
    function task;
}Node;

uint16_t square(Node* x) {
    uint8_t value = x->value;
    return value*value;

}

int main() {
    Node variable1, variable2;
    Node* ptr;

    ptr = &variable1;
    ptr->value = 2;
    ptr->task = square;
    ptr->next = &variable2;
    
    variable2.value = 3;
    variable2.task = square;
    variable2.next = NULL;

    while(ptr !=NULL){
        printf("%d\n", ptr->value);
        printf("%d\n",ptr->task(ptr));
        ptr = ptr->next;
    }

    
    uint8_t value;
    uint8_t* ptr1;
    ptr1 = &value;
    value = 0x33;
    printf("%X\n", *ptr1);
    *ptr1 = 0x22;
    printf("%X", value);
    return 0;
}