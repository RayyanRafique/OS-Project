#include "server.h"
#include "common.h"
#include "work_queue.h"
#include "threadpool.h"
#include "workerpool.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void start_server() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    Queue client_queue;
    queue_init(&client_queue, QUEUE_CAPACITY);

    ClientThreadPool client_pool;
    client_pool_init(&client_pool, CLIENT_THREAD_COUNT, &client_queue);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    work_queue_t work_queue;
    work_queue_init(&work_queue);

    WorkerThreadPool worker_pool;
    worker_pool_init(&worker_pool, 4, &work_queue); // use kro isko worker thread ky liye

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_PENDING_CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }
        printf("New client connected.\n");
        queue_push(&client_queue, client_socket);
    }

    client_pool_destroy(&client_pool);
    queue_destroy(&client_queue);
    close(server_fd);
}
