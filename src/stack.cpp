#include <iostream>
#include "stack.h"

using namespace std;

void initStack(Stack &s) {
    initList(s); 
}

bool isStackEmpty(const Stack &s) {
    return isEmpty(s); 
}


void push(Stack &s, const Reservation &data) {
    Node* newNode = createNode(data);
    insertFront(s, newNode);
}

void pushLoadedDataDescending(Stack &s, Node *loadedHead) {
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
        push(s, current->data);
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
}


void pop(Stack &s) {
    if (isStackEmpty(s)) {
        cout << "Riwayat kosong!" << endl;
        return;
    }
    deleteFront(s); 
}

void peek(const Stack &s) {
    if (isStackEmpty(s)) {
        cout << "Riwayat kosong!" << endl;
    } else {
        cout << "Successfully added your reservation!" << endl;
        printNode(s.head); 
        cout << string(50, '-') << "\n";
    }
}

void displayStack(const Stack &s) {
    displayList(s, "RESERVATION HISTORY"); 
}
