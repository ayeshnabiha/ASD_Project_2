#ifndef VALIDATION_H
#define VALIDATION_H

#include "linkedList.h"

bool isLeapYear(int year);
bool isPassedDate(int day, int month, int year);
bool isValidDate(int day, int month, int year);
bool isValidTime(int hour, int minutes);
bool isValidDuration(int duration);

#endif