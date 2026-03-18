#ifndef VALIDATION_H
#define VALIDATION_H

#include "linkedList.h"

// Date validation
bool isLeapYear(int year);
bool isValidDate(int day, int month, int year);
bool isValidTime(int hour, int minutes);
bool isValidDuration(int time_start_hour, int time_start_minutes,
                     int time_stop_hour, int time_stop_minutes,
                     int duration);

// Full reservation validation
bool isValidReservation(const Reservation& r, std::string& errorMsg);

#endif