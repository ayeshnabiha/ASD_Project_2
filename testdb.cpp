#include <iostream>
#include "database.h"
using namespace std;

int main() {
    sqlite3* db;

    // Test init
    if (!initDatabase(&db)) {
        cerr << "Init failed" << endl;
        return 1;
    }

    // Test save
    Node* testNode = createNode("Kelompok A", "123456",
                                18, 3, 2026, "Meeting", 60,
                                9, 0, 10, 0, "pending");
    if (saveReservation(db, testNode)) {
        cout << "Save successful" << endl;
    }

    // Test load
    Node* loaded = loadReservations(db);
    Node* curr = loaded;
    while (curr != nullptr) {
        cout << "Loaded: " << curr->data.group_name 
                  << " | " << curr->data.niu
                  << " | " << curr->data.status << endl;
        curr = curr->next;
    }

    // Test update
    if (updateStatus(db, "123456", "approved")) {
        cout << "Update successful" << endl;
    }

    // Load again to verify update
    loaded = loadReservations(db);
    curr = loaded;
    while (curr != nullptr) {
        cout << "After update: " << curr->data.group_name
                  << " | " << curr->data.status << endl;
        curr = curr->next;
    }

    closeDatabase(db);
    return 0;
}