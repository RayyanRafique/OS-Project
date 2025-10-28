#ifndef COMMON_H
#define COMMON_H
// globalb constraints for my project
//development port = 8080 [apparently common in http style server client comm]

#define SERVER_PORT 8080
// maximum 10 connections in pending queue [made by OS inside TCP stack]
#define MAX_PENDING_CONNECTIONS 10
// amount of client threads in my threadpool, 4 as told by TA
#define CLIENT_THREAD_COUNT 4
// client queue capacity
#define QUEUE_CAPACITY 16

#endif
