#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, void* data) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = data;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

void* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    Node* temp = q->front;
    void* data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return data;
}

/*
For testing purposes until I can find a better way to test this.
*/
int main() {
    Queue* queue = createQueue();
    int* new_val = (int*)malloc(sizeof(int));  // Dynamically allocate memory
    *new_val = 1;
    enqueue(queue, new_val);  // Store the pointer to the allocated memory
    int* head_data = (int*)dequeue(queue);  // Retrieve the pointer
    printf("HEAD: %d\n", *head_data);  // Dereference to get the value
    free(head_data);  // Free the allocated memory
    free(queue);  // Free the queue structure
    return 0;
}
