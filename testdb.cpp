#include <iostream>
#include "database.h"
using namespace std;

int main() {
    sqlite3* db;

    // init
    if (!initDatabase(&db)) {
        cerr << "Init failed" << endl;
        return 1;
    }

    Reservation r;
    r.group_name = "Kelompok A";
    r.niu = "123456";
    r.date_day = 18; r.date_month = 3; r.date_year = 2026;
    r.purpose = "Meeting";
    r.duration = 60;
    r.time_start_hour = 9; r.time_start_minutes = 0;
    r.time_stop_hour = 10; r.time_stop_minutes = 0;
    r.status = "Pending";

    // save
    Node* testNode = createNode(r);
    if (saveReservation(db, testNode)) {
     cout << "Save successful" << endl;
    } else {
        cout << "Save failed" << endl;
    }

    // load
    Node* loaded = loadReservations(db);
    Node* curr = loaded;
    while (curr != nullptr) {
        cout << "Loaded: " << curr->data.group_name 
                  << " | " << curr->data.niu
                  << " | " << curr->data.status << endl;
        curr = curr->next;
    }

    // update
    if (updateStatus(db, "123456", "Accepted")) {
        cout << "Update successful" << endl;
    }

    // load lagi utk cek update
    loaded = loadReservations(db);
    curr = loaded;
    while (curr != nullptr) {
        cout << "After update: " << curr->data.group_name
                  << " | " << curr->data.status << endl;
        curr = curr->next;
    }

    // close
    closeDatabase(db);
    return 0;
}