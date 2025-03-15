#ifndef __QUEUE_H
#define __QUEUE_H

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue();

void enqueue(Queue* q, void* data);

void* dequeue(Queue* q);

#endif // __QUEUE_H
