#include <stdio.h>
#include "work_queue.h"

int main() {
    work_queue_t queue;
    work_queue_init(&queue);

    enqueue_work(&queue, 10);
    enqueue_work(&queue, 20);
    enqueue_work(&queue, 30);

    printf("%d\n", dequeue_work(&queue));
    printf("%d\n", dequeue_work(&queue));
    printf("%d\n", dequeue_work(&queue));
}
