#ifndef QUEUE_H
#define QUEUE_H

#include "linkedList.h"

typedef LinkedList Queue;

// Function for FIFO 
void enqueue(Queue &queue, const Reservation &data);

// Function to remove the front element of the queue
Node* dequeue(Queue &queue);

// Display the queue
void showQueue(const Queue &queue);

#endif