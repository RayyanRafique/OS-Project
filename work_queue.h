#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <pthread.h>

typedef struct work_item {
    int client_fd;
    struct work_item *next;
} work_item_t;

typedef struct work_queue {
    work_item_t *head;
    work_item_t *tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} work_queue_t;

void work_queue_init(work_queue_t *queue);
void enqueue_work(work_queue_t *queue, int client_fd);
int dequeue_work(work_queue_t *queue);

#endif
