#include <iostream>
#include "stack.h"

using namespace std;

// Inisialisasi Stack
void initStack(Stack &s) {
    s.top = nullptr;
}

// Cek apakah kosong
bool isStackEmpty(const Stack &s) {
    return s.top == nullptr;
}

// PUSH: Menambah riwayat reservasi yang baru diproses ke tumpukan atas
void push(Stack &s, const Reservation &data) {
  
    Node* newNode = createNode(data);
    
    if (isStackEmpty(s)) {
        s.top = newNode;
        newNode->next = nullptr;
    } else {
        newNode->next = s.top;
        s.top = newNode;
    }
}

// POP: Menghapus data teratas 
void pop(Stack &s) {
    if (isStackEmpty(s)) {
        cout << "[Stack] Riwayat kosong, tidak ada yang bisa dihapus.\n";
        return;
    }
    Node* temp = s.top;
    s.top = s.top->next;
    delete temp;
}

// PEEK: Melihat data yang paling terakhir masuk riwayat
void peek(const Stack &s) {
    if (isStackEmpty(s)) {
        cout << "[Stack] Belum ada riwayat reservasi.\n";
        return;
    }
    cout << "Riwayat Terakhir: " << s.top->data.group_name 
         << " (NIU: " << s.top->data.niu << ")\n";
}

// DISPLAY: Menampilkan seluruh riwayat 
void displayStack(const Stack &s) {
    if (isStackEmpty(s)) {
        cout << "\n[Riwayat] Masih kosong.\n";
        return;
    }

    Node* current = s.top;
    cout << "\n========================================" << endl;
    cout << "       RIWAYAT RESERVASI (STACk)       " << endl;
    cout << "========================================" << endl;
    
    int i = 1;
    while (current != nullptr) {
        cout << i++ << ". Kelompok: " << current->data.group_name << endl;
        cout << "   NIU      : " << current->data.niu << endl;
        cout << "   Status   : " << current->data.status << endl;
        cout << "   Tanggal  : " << current->data.date_day << "/" 
             << current->data.date_month << "/" << current->data.date_year << endl;
        cout << "----------------------------------------" << endl;
        current = current->next;
    }
}
