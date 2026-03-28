#include "validation.h"
#include <sstream>

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(int day, int month, int year) {
    if (year < 2000 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year)) daysInMonth[1] = 29;

    if (day < 1 || day > daysInMonth[month - 1]) return false;

    return true;
}

bool isValidTime(int hour, int minutes) {
    if (hour < 0 || hour > 23) return false;
    if (minutes < 0 || minutes > 59) return false;
    return true;
}

bool isValidDuration(int duration) {
    return duration > 0 && duration <= 24 * 60;
}