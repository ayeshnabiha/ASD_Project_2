#include <iostream>
#include "queue.h"
using namespace std;

// Helper untuk bikin data reservation cepat
Reservation createSample(string name, string niu, int day)
{
    Reservation r;
    r.group_name = name;
    r.niu = niu;
    r.date_day = day;
    r.date_month = 6;
    r.date_year = 2025;
    r.purpose = "Praktikum";
    r.duration = 60;
    r.time_start_hour = 8;
    r.time_start_minutes = 0;
    r.time_stop_hour = 9;
    r.time_stop_minutes = 0;
    r.status = "Pending";
    return r;
}

int main()
{
    Queue q;
    initList(q);

    cout << "=== TEST QUEUE (FIFO) ===\n";

    // Enqueue beberapa data
    enqueue(q, createSample("Kelompok A", "123456", 10));
    enqueue(q, createSample("Kelompok B", "234567", 11));
    enqueue(q, createSample("Kelompok C", "345678", 12));

    cout << "\nSetelah enqueue 3 data:\n";
    showQueue(q);

    // Dequeue 1 data
    cout << "\nDequeue 1 data...\n";
    Node* removed = dequeue(q);

    if (removed != nullptr)
    {
        cout << "Data yang keluar (harus Kelompok A):\n";
        printNode(removed, 0);
        delete removed; // penting biar nggak leak
    }

    cout << "\nQueue setelah dequeue:\n";
    showQueue(q);

    // Dequeue semua
    cout << "\nDequeue semua data...\n";
    while (!isEmpty(q))
    {
        Node* temp = dequeue(q);
        delete temp;
    }

    cout << "\nQueue setelah dikosongkan:\n";
    showQueue(q);

    return 0;
}