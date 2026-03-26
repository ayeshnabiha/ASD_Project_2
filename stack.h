#ifndef STACK_H
#define STACK_H

#include "linkedList.h"

struct Stack {
    Node* top;
};

// Fungsi
void initStack(Stack &s);
bool isStackEmpty(const Stack &s);
void push(Stack &s, const Reservation &data); 
void pop(Stack &s);
void peek(const Stack &s);
void displayStack(const Stack &s);

#endif