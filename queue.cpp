#include "queue.h"
#include "linkedList.h"
#include <iostream>
using namespace std;

// Enqueue: tambah data ke belakang (FIFO)
void enqueue(Queue &queue, const Reservation &data)
{
    Node* newNode = createNode(data);
    insertEnd(queue, newNode);
}

// Dequeue: hapus data dari depan (FIFO)
Node* dequeue(Queue &queue)
{
    if (isEmpty(queue))
    {
        cout << "Queue kosong!" << endl;
        return nullptr;
    }

    return deleteFront(queue);
}

// Menampilkan isi queue
void showQueue(const Queue &queue)
{
    displayList(queue, "Queue (FIFO)");
}