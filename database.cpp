#include "database.h"
#include <iostream>

bool initDatabase(sqlite3** db, const std::string& filename) {
    int rc = sqlite3_open(filename.c_str(), db);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(*db) << std::endl;
        return false;
    }

    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS reservations (
            niu TEXT PRIMARY KEY,
            group_name TEXT NOT NULL,
            date_day INTEGER,
            date_month INTEGER,
            date_year INTEGER,
            purpose TEXT,
            duration INTEGER,
            time_start_hour INTEGER,
            time_start_minutes INTEGER,
            time_stop_hour INTEGER,
            time_stop_minutes INTEGER,
            status TEXT DEFAULT 'pending'
        );
    )";

    char* errMsg = nullptr;
    rc = sqlite3_exec(*db, createTableSQL, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Database initialized successfully." << std::endl;
    return true;
}

void closeDatabase(sqlite3* db) {
    sqlite3_close(db);
    std::cout << "Database closed." << std::endl;
}


// insert reservation into database
bool saveReservation(sqlite3* db, Node* node) {
    const char* insertSQL = R"(
        INSERT OR REPLACE INTO reservations
        (niu, group_name, date_day, date_month, date_year,
         purpose, duration, time_start_hour, time_start_minutes,
         time_stop_hour, time_stop_minutes, status)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

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
    sqlite3_bind_text(stmt, 6, node->data.purpose.c_str(), -1, SQLITE_STATIC);
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


