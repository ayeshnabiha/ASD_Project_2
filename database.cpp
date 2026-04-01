#include "database.h"
#include <iostream>
#include <tuple>
#include <ctime>

namespace {
bool isReservationPassed(const Reservation& reservation) {
    std::time_t currentTimestamp = std::time(nullptr);
    std::tm *currentTime = std::localtime(&currentTimestamp);

    auto reservationEnd = std::make_tuple(
        reservation.date_year,
        reservation.date_month,
        reservation.date_day,
        reservation.time_stop_hour,
        reservation.time_stop_minutes
    );

    auto now = std::make_tuple(
        currentTime->tm_year + 1900,
        currentTime->tm_mon + 1,
        currentTime->tm_mday,
        currentTime->tm_hour,
        currentTime->tm_min
    );

    return reservationEnd <= now;
}

bool markReservationCompleted(sqlite3* db, const Reservation& reservation) {
    const char* updateSQL = R"(
        UPDATE reservations
        SET status = 'Completed'
        WHERE niu = ?
          AND date_day = ?
          AND date_month = ?
          AND date_year = ?
          AND time_start_hour = ?
          AND time_start_minutes = ?
          AND status != 'Cancelled'
          AND status != 'Completed';
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare completed update statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, reservation.niu.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, reservation.date_day);
    sqlite3_bind_int(stmt, 3, reservation.date_month);
    sqlite3_bind_int(stmt, 4, reservation.date_year);
    sqlite3_bind_int(stmt, 5, reservation.time_start_hour);
    sqlite3_bind_int(stmt, 6, reservation.time_start_minutes);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}
}

bool initDatabase(sqlite3** db, const std::string& filename) {
    int rc = sqlite3_open(filename.c_str(), db);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(*db) << std::endl;
        return false;
    }

    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS reservations (
            niu TEXT NOT NULL,
            group_name TEXT NOT NULL,
            date_day INTEGER,
            date_month INTEGER,
            date_year INTEGER,
            purpose INTEGER,
            duration INTEGER,
            time_start_hour INTEGER,
            time_start_minutes INTEGER,
            time_stop_hour INTEGER,
            time_stop_minutes INTEGER,
            status TEXT DEFAULT 'Pending',
            PRIMARY KEY (niu, date_day, date_month, date_year, time_start_hour, time_start_minutes)
        );
    )";

    char* errMsg = nullptr;
    rc = sqlite3_exec(*db, createTableSQL, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    // std::cout << "Database initialized successfully." << std::endl;
    return true;
}

void closeDatabase(sqlite3* db) {
    sqlite3_close(db);
    // std::cout << "Database closed." << std::endl;
}

bool saveReservation(sqlite3* db, Node* node) {
    const char* insertSQL = R"(
        INSERT INTO reservations
        (niu, group_name, date_day, date_month, date_year, purpose, duration, time_start_hour, time_start_minutes, time_stop_hour, time_stop_minutes, status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, node->data.niu.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, node->data.group_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, node->data.date_day);
    sqlite3_bind_int(stmt, 4, node->data.date_month);
    sqlite3_bind_int(stmt, 5, node->data.date_year);
    sqlite3_bind_int(stmt, 6, node->data.purpose);    
    sqlite3_bind_int(stmt, 7, node->data.duration);
    sqlite3_bind_int(stmt, 8, node->data.time_start_hour);
    sqlite3_bind_int(stmt, 9, node->data.time_start_minutes);
    sqlite3_bind_int(stmt, 10, node->data.time_stop_hour);
    sqlite3_bind_int(stmt, 11, node->data.time_stop_minutes);
    sqlite3_bind_text(stmt, 12, node->data.status.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to save reservation: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

Node* loadReservations(sqlite3* db) {
    const char* selectSQL = "SELECT niu, group_name, date_day, date_month, date_year, purpose, duration, time_start_hour, time_start_minutes, time_stop_hour, time_stop_minutes, status FROM reservations WHERE status != 'Cancelled' AND status != 'Completed';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare load statement: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }

    Node* head = nullptr;
    Node* tail = nullptr;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Reservation r;
        r.niu                = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        r.group_name         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        r.date_day           = sqlite3_column_int(stmt, 2);
        r.date_month         = sqlite3_column_int(stmt, 3);
        r.date_year          = sqlite3_column_int(stmt, 4);
        r.purpose            = sqlite3_column_int(stmt, 5);    
        r.duration           = sqlite3_column_int(stmt, 6);
        r.time_start_hour    = sqlite3_column_int(stmt, 7);
        r.time_start_minutes = sqlite3_column_int(stmt, 8);
        r.time_stop_hour     = sqlite3_column_int(stmt, 9);
        r.time_stop_minutes  = sqlite3_column_int(stmt, 10);
        r.status             = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));

        if (isReservationPassed(r)) {
            if (!markReservationCompleted(db, r)) {
                std::cerr << "Warning: failed to mark reservation as Completed for NIU " << r.niu << std::endl;
            }
            continue;
        }

        Node* newNode = createNode(r);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    sqlite3_finalize(stmt);
    return head;
}

Node* getReservationsByNIU(sqlite3* db, const std::string& niu) {
    const char* querySQL = "SELECT niu, group_name, date_day, date_month, date_year, purpose, duration, time_start_hour, time_start_minutes, time_stop_hour, time_stop_minutes, status FROM reservations WHERE niu LIKE ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare NIU search: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, niu.c_str(), -1, SQLITE_STATIC);

    Node* head = nullptr;
    Node* tail = nullptr;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Reservation r;
        r.niu                = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        r.group_name         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        r.date_day           = sqlite3_column_int(stmt, 2);
        r.date_month         = sqlite3_column_int(stmt, 3);
        r.date_year          = sqlite3_column_int(stmt, 4);
        r.purpose            = sqlite3_column_int(stmt, 5);    
        r.duration           = sqlite3_column_int(stmt, 6);
        r.time_start_hour    = sqlite3_column_int(stmt, 7);
        r.time_start_minutes = sqlite3_column_int(stmt, 8);
        r.time_stop_hour     = sqlite3_column_int(stmt, 9);
        r.time_stop_minutes  = sqlite3_column_int(stmt, 10);
        r.status             = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));

        Node* newNode = createNode(r);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    sqlite3_finalize(stmt);
    return head;
}

bool hasTimeConflict(sqlite3* db, const Reservation& r) {
    const char* querySQL =  "SELECT time_start_hour, time_start_minutes, duration FROM reservations WHERE date_day = ? AND date_month = ? AND date_year = ? AND status != 'Cancelled';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare conflict check: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, r.date_day);
    sqlite3_bind_int(stmt, 2, r.date_month);
    sqlite3_bind_int(stmt, 3, r.date_year);

    int newStart = r.time_start_hour * 60 + r.time_start_minutes;
    int newStop = newStart + r.duration;

    bool conflict = false;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int existStartH = sqlite3_column_int(stmt, 0);
        int existStartM = sqlite3_column_int(stmt, 1);
        int existDur = sqlite3_column_int(stmt, 2);

        int existStart = existStartH * 60 + existStartM;
        int existStop  = existStart + existDur;
        if (newStart < existStop && newStop > existStart) {
            conflict = true;
            break;
        }
    }

    sqlite3_finalize(stmt);
    return conflict;
}

bool updateStatus(sqlite3* db, const std::string& niu, const std::string& status) {
    const char* updateSQL = "UPDATE reservations SET status = ? WHERE niu = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare update statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, niu.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update status: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

bool clearDatabase(sqlite3* db) {
    const char* clearSQL = "DELETE FROM reservations;";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, clearSQL, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to clear database: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    std::cout << "Database cleared successfully." << std::endl;
    return true;
} // cuma hapus data, bukan tabelnya