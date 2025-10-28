#ifndef WORKERPOOL_H
#define WORKERPOOL_H

#include <pthread.h>
#include "work_queue.h"

typedef struct {
    int client_socket;
    char buffer[256];
} WorkTask;

typedef struct {
    pthread_t *threads;
    int num_threads;
    work_queue_t *work_queue;
    int running;
} WorkerThreadPool;

void worker_pool_init(WorkerThreadPool *pool, int num_threads, work_queue_t *queue);
void worker_pool_destroy(WorkerThreadPool *pool);

#endif
