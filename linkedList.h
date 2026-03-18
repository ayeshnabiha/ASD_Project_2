#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>

// Struct for Reservation
struct Reservation
{
    std::string group_name;
    std::string niu;
    int date_day;
    int date_month;
    int date_year;
    std::string purpose;
    int duration; // in minutes
    int time_start_hour;
    int time_start_minutes;
    int time_stop_hour;
    int time_stop_minutes;
    std::string status; // "Pending", "Accepted", "Cancel"
};

// Struct for Linked List Mechanism
struct Node
{
    Reservation data;
    Node *next;
};

struct LinkedList
{
    Node *head;
    Node *tail;
    int size;
};

// Function Declaration
void initList(LinkedList &list);

Node *createNode(const Reservation &data);

bool isEmpty(const LinkedList &list);
void insertEnd(LinkedList &list, Node *newNode);
void insertFront(LinkedList &list, Node *newNode);
Node *deleteFront(LinkedList &list);
void printNode(const Node *node, int num = 0);                                         // display single node
void displayList(const LinkedList &list, const std::string &label = "Data Reservasi"); // display all list

void freeList(LinkedList &list);

#endif