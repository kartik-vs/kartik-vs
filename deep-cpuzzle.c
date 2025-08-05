#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#define BUF_SIZE 64
#define CHUNK_SIZE 8
#define FILE_NAME "output.bin"

typedef struct Node {
    uint8_t data[CHUNK_SIZE];
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    size_t size;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} CircularBuffer;

typedef struct {
    CircularBuffer* buffer;
    FILE* file;
    int running;
    void (*process)(uint8_t*, size_t);
} ThreadContext;

// ---------- Circular Buffer Logic ----------
void init_buffer(CircularBuffer* cb) {
    cb->head = cb->tail = NULL;
    cb->size = 0;
    pthread_mutex_init(&cb->lock, NULL);
    pthread_cond_init(&cb->not_empty, NULL);
    pthread_cond_init(&cb->not_full, NULL);
}

void free_buffer(CircularBuffer* cb) {
    Node* cur = cb->head;
    while (cur) {
        Node* next = cur->next;
        free(cur);
        cur = next;
    }
    pthread_mutex_destroy(&cb->lock);
    pthread_cond_destroy(&cb->not_empty);
    pthread_cond_destroy(&cb->not_full);
}

void enqueue(CircularBuffer* cb, uint8_t* data) {
    pthread_mutex_lock(&cb->lock);

    while (cb->size >= BUF_SIZE)
        pthread_cond_wait(&cb->not_full, &cb->lock);

    Node* node = malloc(sizeof(Node));
    memcpy(node->data, data, CHUNK_SIZE);
    node->next = NULL;

    if (cb->tail)
        cb->tail->next = node;
    else
        cb->head = node;

    cb->tail = node;
    cb->size++;

    pthread_cond_signal(&cb->not_empty);
    pthread_mutex_unlock(&cb->lock);
}

int dequeue(CircularBuffer* cb, uint8_t* out) {
    pthread_mutex_lock(&cb->lock);

    while (cb->size == 0)
        pthread_cond_wait(&cb->not_empty, &cb->lock);

    Node* node = cb->head;
    if (!node) {
        pthread_mutex_unlock(&cb->lock);
        return -1;
    }

    memcpy(out, node->data, CHUNK_SIZE);
    cb->head = node->next;
    if (!cb->head)
        cb->tail = NULL;

    free(node);
    cb->size--;

    pthread_cond_signal(&cb->not_full);
    pthread_mutex_unlock(&cb->lock);
    return 0;
}

// ---------- Processor Logic ----------
void xor_encrypt(uint8_t* data, size_t len) {
    uint8_t key = 0xAA;
    for (size_t i = 0; i < len; i++)
        data[i] ^= key;
}

// ---------- Thread Routines ----------
void* producer(void* arg) {
    ThreadContext* ctx = (ThreadContext*)arg;
    for (int i = 0; ctx->running && i < 200; i++) {
        uint8_t buffer[CHUNK_SIZE];
        for (int j = 0; j < CHUNK_SIZE; j++)
            buffer[j] = (uint8_t)(rand() % 256);

        ctx->process(buffer, CHUNK_SIZE);
        enqueue(ctx->buffer, buffer);
        usleep(10000);
    }
    ctx->running = 0;
    return NULL;
}

void* consumer(void* arg) {
    ThreadContext* ctx = (ThreadContext*)arg;
    while (ctx->running || ctx->buffer->size > 0) {
        uint8_t buffer[CHUNK_SIZE];
        if (dequeue(ctx->buffer, buffer) == 0) {
            fwrite(buffer, 1, CHUNK_SIZE, ctx->file);
            fflush(ctx->file);
        }
    }
    return NULL;
}

// ---------- Main ----------
int main() {
    CircularBuffer cb;
    init_buffer(&cb);

    FILE* fout = fopen(FILE_NAME, "wb");
    if (!fout) {
        perror("fopen");
        return 1;
    }

    ThreadContext ctx = {
        .buffer = &cb,
        .file = fout,
        .running = 1,
        .process = xor_encrypt
    };

    pthread_t prod_tid, cons_tid;
    pthread_create(&prod_tid, NULL, producer, &ctx);
    pthread_create(&cons_tid, NULL, consumer, &ctx);

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    fclose(fout);
    free_buffer(&cb);

    printf("Data written to %s\n", FILE_NAME);
    return 0;
}
