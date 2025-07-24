#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct node* pnode;

typedef void (*task_node)(pnode);

typedef struct node {
    uint8_t id;
    uint8_t value;
    pnode next;
    pnode prev;
    task_node task;
}Node;

void printvalue(pnode x) {
    printf("task peformed to print the value stored in struct : %d\n", x->value);
}

void printid(pnode x) {
    printf("%d, this is the task ID.\n", x->id);
}

int main() {
    
    Node task1, task2;
    pnode ptr = NULL;
    
    task1.id = 1;
    task1.value = 45;
    task1.task = printvalue;
    task1.next = &task2;
    task1.prev = NULL;
    
    task2.id = 2;
    task2.value = 21;
    task2.task = printvalue;
    task2.next = NULL;
    task2.prev = &task1;

    ptr = &task1;

    while(ptr != NULL) {
        printf("calling function pointer: %d\n", ptr->id);
        ptr->task(ptr);
        ptr = ptr->next;
    }
    
    printf("\n\n");

    ptr = &task2;

    while(ptr != NULL) {
        printf("calling function pointer in reverse order: %d\n", ptr->id);
        ptr->task(ptr);
        ptr = ptr->prev;
    }

    task1.task = printid;
    task2.task = printid;

    printf("\n\n");

    ptr = &task1;

    while(ptr != NULL) {
        printf("Updated the function pointer to make a differnt call in forward order\n");
        ptr->task(ptr);
        ptr = ptr->next;
    }

    return 0;
}