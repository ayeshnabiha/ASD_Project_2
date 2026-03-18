#include "linkedList.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// Function Implementation
void initList(LinkedList &list)
{
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

Node *createNode(const Reservation &data)
{
    Node *newNode = new Node();
    newNode->data = data;
    if (newNode->data.status.empty())
    {
        newNode->data.status = "Pending";
    }
    newNode->next = nullptr;

    return newNode;
}

bool isEmpty(const LinkedList &list)
{
    return list.head == nullptr && list.size == 0;
}

void insertEnd(LinkedList &list, Node *newNode)
{
    if (isEmpty(list))
    {
        list.head = newNode;
        list.tail = newNode;
    }
    else
    {
        list.tail->next = newNode;
        list.tail = newNode;
    }
    list.size++;
}

void insertFront(LinkedList &list, Node *newNode)
{
    if (isEmpty(list))
    {
        list.head = newNode;
        list.tail = newNode;
    }
    else
    {
        newNode->next = list.head;
        list.head = newNode;
    }
    list.size++;
}

Node *deleteFront(LinkedList &list)
{
    if (isEmpty(list))
        return nullptr;

    Node *removed = list.head;
    list.head = list.head->next;

    if (list.head == nullptr)
    {
        list.tail = nullptr;
    }

    removed->next = nullptr;
    list.size--;

    return removed;
}

void printNode(const Node *node, int nomor)
{
    if (node == nullptr)
        return;

    const Reservation &r = node->data;

    if (nomor > 0)
        cout << nomor << ". ";
    else
        cout << "   ";

    cout << r.group_name << " (" << r.niu << ")\n";
    cout << "   Keperluan : " << r.purpose << "\n";
    cout << "   Jadwal    : "
         << setfill('0') << setw(2) << r.date_day << "/"
         << setfill('0') << setw(2) << r.date_month << "/"
         << r.date_year
         << "  |  "
         << setfill('0') << setw(2) << r.time_start_hour << ":"
         << setfill('0') << setw(2) << r.time_start_minutes << " - "
         << setfill('0') << setw(2) << r.time_stop_hour << ":"
         << setfill('0') << setw(2) << r.time_stop_minutes
         << "  (" << r.duration << " mnt)\n";
    cout << "   Status    : " << r.status << "\n";
}

void displayList(const LinkedList &list, const string &label)
{
    cout << "\n=== " << label << " ===\n";
    cout << "Total: " << list.size << " data\n";
    cout << string(48, '-') << "\n";

    if (isEmpty(list))
    {
        cout << "  [!] Tidak ada data.\n";
        cout << string(48, '-') << "\n";
        return;
    }

    Node *temp = list.head;
    int urutan = 1;
    while (temp != nullptr)
    {
        printNode(temp, urutan++);
        cout << string(48, '-') << "\n";
        temp = temp->next;
    }
}

void freeList(LinkedList &list)
{
    Node *temp;
    while (list.head != nullptr)
    {
        temp = list.head;
        list.head = list.head->next;
        delete temp;
    }
    list.tail = nullptr;
    list.size = 0;
}