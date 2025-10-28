#include <stdlib.h>
#include "work_queue.h"

void work_queue_init(work_queue_t *queue) {
    queue->head = queue->tail = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

void enqueue_work(work_queue_t *queue, int client_fd) {
    work_item_t *item = malloc(sizeof(work_item_t));
    item->client_fd = client_fd;
    item->next = NULL;

    pthread_mutex_lock(&queue->mutex);

    if (queue->tail == NULL) {
        queue->head = queue->tail = item;
    } else {
        queue->tail->next = item;
        queue->tail = item;
    }

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

int dequeue_work(work_queue_t *queue) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == NULL) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    work_item_t *item = queue->head;
    queue->head = item->next;

    if (queue->head == NULL)
        queue->tail = NULL;

    pthread_mutex_unlock(&queue->mutex);

    int fd = item->client_fd;
    free(item);
    return fd;
}
