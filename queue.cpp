#include "queue.h"
#include <iostream>
using namespace std;

// Functions implementation, mainly using linked list functions xixixi
void enqueue(Node*& head, const Reservation& data){
    insertEnd(head, data);
}

void dequeue(Node*& head){
    if(isEmpty(head)){
        cout << "Queue kosong!" << endl;
        return;
    }
    deleteFront(head);
}

void showQueue(Node* head){
    if(isEmpty(head)){
        cout << "Queue kosong!" << endl;
        return;
    }
    displayList(head);
}