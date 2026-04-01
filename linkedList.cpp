#include "linkedList.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <tuple>
#include <utility>
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

Node *deleteMiddle(LinkedList &list, const std::string &niu)
{
    if (isEmpty(list))
        return nullptr;

    if (list.head->data.niu == niu)
        return deleteFront(list);

    Node *prev = list.head;
    Node *curr = list.head->next;

    while (curr != nullptr && curr->data.niu != niu)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr)
        return nullptr;

    prev->next = curr->next;

    if (curr == list.tail)
        list.tail = prev;

    curr->next = nullptr;
    list.size--;

    return curr;
}

Node *deleteEnd(LinkedList &list)
{
    if (isEmpty(list))
        return nullptr;

    if (list.head == list.tail)
        return deleteFront(list);

    Node *prev = list.head;
    while (prev->next != list.tail)
        prev = prev->next;

    Node *removed = list.tail;
    list.tail = prev;
    list.tail->next = nullptr;
    removed->next = nullptr;
    list.size--;

    return removed;
}

void printNode(const Node *node, int nomor)
{
    if (node == nullptr)
        return;

    const Reservation &r = node->data;

    string purposeStr;
    if (r.purpose == 1)
        purposeStr = "Praktikum";
    else if (r.purpose == 2)
        purposeStr = "Pelatihan";
    else
        purposeStr = "Other";

    if (nomor > 0)
        cout << nomor << ". ";
    else
        cout << "   ";

    cout << r.group_name << " (" << r.niu << ")\n";
    cout << "   Purpose      : " << purposeStr << "\n";
    cout << "   Schedule     : "
         << setfill('0') << setw(2) << r.date_day << "-"
         << setfill('0') << setw(2) << r.date_month << "-"
         << r.date_year
         << "  |  "
         << setfill('0') << setw(2) << r.time_start_hour << ":"
         << setfill('0') << setw(2) << r.time_start_minutes << " - "
         << setfill('0') << setw(2) << r.time_stop_hour << ":"
         << setfill('0') << setw(2) << r.time_stop_minutes
         << "  (" << r.duration << " mnt)\n";
    cout << "   Status       : " << r.status << "\n";
}

void displayList(const LinkedList &list, const string &label)
{
    cout << string(15, '=') << " " << label << " " << string(15, '=') << "\n";
    cout << string(50, '-') << "\n";

    cout << "Total      : " << list.size << " data\n";

    if (isEmpty(list))
    {
        cout << "  [!] Data is not available.\n";
        cout << string(50, '-') << "\n";
        return;
    }

    Node *temp = list.head;
    int urutan = 1;
    while (temp != nullptr)
    {
        printNode(temp, urutan++);
        cout << string(50, '-') << endl;
        temp = temp->next;
    }
}

bool isLater(const Reservation &a, const Reservation &b)
{
    return std::tie(a.date_year, a.date_month, a.date_day,
                    a.time_start_hour, a.time_start_minutes) > std::tie(b.date_year, b.date_month, b.date_day,
                                                                        b.time_start_hour, b.time_start_minutes);
}

bool isEarlier(const Reservation &a, const Reservation &b)
{
    return std::tie(a.date_year, a.date_month, a.date_day,
                    a.time_start_hour, a.time_start_minutes) < std::tie(b.date_year, b.date_month, b.date_day,
                                                                        b.time_start_hour, b.time_start_minutes);
}

namespace
{
    Node *splitList(Node *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return nullptr;
        }

        Node *slow = head;
        Node *fast = head->next;

        while (fast != nullptr && fast->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        Node *secondHalf = slow->next;
        slow->next = nullptr;
        return secondHalf;
    }

    Node *mergeAscending(Node *left, Node *right)
    {
        Node dummy;
        dummy.next = nullptr;
        Node *tail = &dummy;

        while (left != nullptr && right != nullptr)
        {
            if (!isLater(left->data, right->data))
            {
                tail->next = left;
                left = left->next;
            }
            else
            {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }

        tail->next = (left != nullptr) ? left : right;
        return dummy.next;
    }

    Node *mergeDescending(Node *left, Node *right)
    {
        Node dummy;
        dummy.next = nullptr;
        Node *tail = &dummy;

        while (left != nullptr && right != nullptr)
        {
            if (!isEarlier(left->data, right->data))
            {
                tail->next = left;
                left = left->next;
            }
            else
            {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }

        tail->next = (left != nullptr) ? left : right;
        return dummy.next;
    }

    Node *mergeSortAscending(Node *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        Node *secondHalf = splitList(head);

        Node *left = mergeSortAscending(head);
        Node *right = mergeSortAscending(secondHalf);

        return mergeAscending(left, right);
    }

    Node *mergeSortDescending(Node *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        Node *secondHalf = splitList(head);

        Node *left = mergeSortDescending(head);
        Node *right = mergeSortDescending(secondHalf);

        return mergeDescending(left, right);
    }

    void refreshTail(LinkedList &list)
    {
        list.tail = list.head;
        while (list.tail != nullptr && list.tail->next != nullptr)
        {
            list.tail = list.tail->next;
        }
    }
}

void sortByScheduleASC(LinkedList &list)
{
    if (isEmpty(list) || list.head == nullptr || list.head->next == nullptr)
        return;

    list.head = mergeSortAscending(list.head);
    refreshTail(list);
}

void sortByScheduleDESC(LinkedList &list)
{
    if (isEmpty(list) || list.head == nullptr || list.head->next == nullptr)
        return;

    list.head = mergeSortDescending(list.head);
    refreshTail(list);
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
