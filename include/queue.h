#ifndef QUEUE_H
#define QUEUE_H

#include "linkedList.h"

typedef LinkedList Queue;

void initQueue(Queue &queue);

// Function for FIFO 
void enqueue(Queue &queue, const Reservation &data);

// Sort loaded data ascending, then enqueue one-by-one (FIFO enqueue)
void enqueueLoadedDataAscending(Queue &queue, Node *loadedHead);

// Function to remove the front element of the queue
Node* dequeue(Queue &queue);

// Display the queue
void showQueue(const Queue &queue);

// Function to peek at the front element of the queue without removing it
Node* peekQueue(const Queue &queue);

// Function to get the size of the queue
int sizeQueue(const Queue &queue);

// Function to clear the queue
void clearQueue(Queue &queue);

#endif