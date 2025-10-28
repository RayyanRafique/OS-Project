#include "workerpool.h"
#include "work_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

static void* worker_thread_func(void *arg) {
    WorkerThreadPool *pool = (WorkerThreadPool*)arg;

    while (pool->running) {

        //block till we have work available
        int client_socket = dequeue_work(pool->work_queue);

        if (client_socket < 0) {
            continue;
        }

        printf("[Worker] Received task from client FD=%d\n", client_socket);

        char buffer[256] = {0};

        int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            printf("[Worker] Client %d disconnected.\n", client_socket);
            close(client_socket);
            continue;
        }

        if (strncmp(buffer, "upload", 6) == 0) {
            printf("[Worker] Client %d asked for UPLOAD!\n", client_socket);

        } else if (strncmp(buffer, "list", 4) == 0) {
            printf("[Worker] Client %d asked for LIST!\n", client_socket);

        } else if (strncmp(buffer, "delete", 6) == 0) {
            printf("[Worker] Client %d asked for DELETE!\n", client_socket);

        } else {
            printf("[Worker] Client %d sent UNKNOWN command: %s\n",
                   client_socket, buffer);
        }

        //sending response back to the client
        send(client_socket, "OK\n", 3, 0);

        // cleanup
        close(client_socket);
    }

    return NULL;
}

void worker_pool_init(WorkerThreadPool *pool, int num_threads, work_queue_t *queue) {
    pool->num_threads = num_threads;
    pool->work_queue = queue;
    pool->running = 1;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, worker_thread_func, pool);
        printf("[WorkerPool] Worker thread %d started.\n", i);
    }
}

void worker_pool_destroy(WorkerThreadPool *pool) {
    pool->running = 0;

    //wake up any blokinked threads waiting on the queue
    for (int i = 0; i < pool->num_threads; i++) {
        enqueue_work(pool->work_queue, -1);
    }

    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
        printf("[WorkerPool] Worker thread %d stopped.\n", i);
    }

    free(pool->threads);
}
