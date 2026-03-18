#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include "linkedList.h"
#include <string>

// setup
bool initDatabase(sqlite3** db, const std::string& filename = "reservations.db");
void closeDatabase(sqlite3* db);

// operations
bool saveReservation(sqlite3* db, Node* node);
bool updateStatus(sqlite3* db, const std::string& niu, const std::string& status);

//load on startup -> returns plain linked list
Node* loadReservations(sqlite3* db);

// clear db
bool clearDatabase(sqlite3* db);

// conflict check
bool hasTimeConflict(sqlite3* db, const Reservation& r);

// filters
Node* getReservationByNIU(sqlite3* db, const std::string& niu);

#endif
