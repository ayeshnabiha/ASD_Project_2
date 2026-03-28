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
    displayList(s, "RIWAYAT RESERVASI (STACK)"); 
}
