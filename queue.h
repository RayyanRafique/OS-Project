#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct {
    int *buffer;
    int capacity;
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} Queue;

void queue_init(Queue *q, int capacity);
void queue_destroy(Queue *q);
void queue_push(Queue *q, int value);
int queue_pop(Queue *q);

#endif
