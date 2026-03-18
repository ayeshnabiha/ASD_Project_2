#ifndef QUEUE_H
#define QUEUE_H

#include "linkedList.h"

// Function for FIFO 
void enqueue(Node*& head, const Reservation& data);

// Function to remove the front element of the queue
void dequeue(Node*& head);

// Display the queue
void showQueue(Node* head);

#endif