#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h> 


static void* client_thread_func(void *arg) {
    ClientThreadPool *pool = (ClientThreadPool*)arg;

    while (pool->running) {
        int client_socket = queue_pop(pool->client_queue);

        char buffer[256];
        sprintf(buffer, "Hello from server!\n");
        send(client_socket, buffer, strlen(buffer), 0);

        int n = recv(client_socket, buffer, sizeof(buffer)-1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Client says: %s\n", buffer);
        }

        close(client_socket);
    }

    return NULL;
}

void client_pool_init(ClientThreadPool *pool, int num_threads, Queue *queue) {
    pool->num_threads = num_threads;
    pool->client_queue = queue;
    pool->running = 1;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++)
        pthread_create(&pool->threads[i], NULL, client_thread_func, pool);
}

void client_pool_destroy(ClientThreadPool *pool) {
    pool->running = 0;
    for (int i = 0; i < pool->num_threads; i++)
        pthread_cancel(pool->threads[i]);
    free(pool->threads);
}
