#include "queue.h"
#include <stdlib.h>

void queue_init(Queue *q, int capacity) {
    q->buffer = malloc(sizeof(int) * capacity);
    q->capacity = capacity;
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

void queue_destroy(Queue *q) {
    free(q->buffer);
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}

void queue_push(Queue *q, int value) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == q->capacity)
        pthread_cond_wait(&q->not_full, &q->mutex);
    q->buffer[q->rear] = value;
    q->rear = (q->rear + 1) % q->capacity;
    q->count++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

int queue_pop(Queue *q) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0)
        pthread_cond_wait(&q->not_empty, &q->mutex);
    int value = q->buffer[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return value;
}
