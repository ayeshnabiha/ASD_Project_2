#include "queue.h"
#include "linkedList.h"
#include <iostream>
using namespace std;

void initQueue(Queue &queue) {
    initList(queue); 
}

// Function for FIFO 
void enqueue(Queue &queue, const Reservation &data)
{
    Node* newNode = createNode(data);
    insertEnd(queue, newNode);
}

void enqueueLoadedDataAscending(Queue &queue, Node *loadedHead)
{
    LinkedList loadedList;
    initList(loadedList);

    Node *current = loadedHead;
    while (current != nullptr)
    {
        Node *nextNode = current->next;
        current->next = nullptr;
        insertEnd(loadedList, current);
        current = nextNode;
    }

    sortByScheduleASC(loadedList);

    current = loadedList.head;
    while (current != nullptr)
    {
        enqueue(queue, current->data);
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

// Function to remove the front element of the queue
Node* dequeue(Queue &queue)
{
    if (isEmpty(queue))
    {
        cout << "Queue kosong!" << endl;
        return nullptr;
    }

    return deleteFront(queue);
}

// Display the queue
void showQueue(const Queue &queue)
{
    displayList(queue, " Reservation List ");
}

// Function to peek at the front element of the queue without removing it
Node* peekQueue(const Queue &queue)
{
    if (isEmpty(queue))
    {
        cout << "Queue empty!" << endl;
        return nullptr;
    }

    return queue.head;
}

// Function to get the size of the queue
int sizeQueue(const Queue &queue)
{
    return queue.size;
}

// Function to clear the queue
void clearQueue(Queue &queue)
{
    freeList(queue);
}