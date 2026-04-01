#ifndef VALIDATION_H
#define VALIDATION_H

#include "linkedList.h"

bool parseDateInput(const std::string &input, int &day, int &month, int &year);
bool parseTimeInput(const std::string &input, int &hour, int &minutes);
bool parseDurationInput(const std::string &input, int &duration);

bool isLeapYear(int year);
bool isPassedDate(int day, int month, int year);
bool isPassedStartTime(int day, int month, int year, int startHour, int startMinutes);
bool isValidDate(int day, int month, int year);
bool isValidTime(int hour, int minutes);
bool isValidDuration(int duration);

#endif