#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include "queue.h"

typedef struct {
    pthread_t *threads;
    int num_threads;
    Queue *client_queue;
    int running;
} ClientThreadPool;

void client_pool_init(ClientThreadPool *pool, int num_threads, Queue *queue);
void client_pool_destroy(ClientThreadPool *pool);

#endif
