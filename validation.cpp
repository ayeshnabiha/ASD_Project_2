#include "validation.h"
#include <sstream>
using namespace std;

bool parseDateInput(const string &input, int &day, int &month, int &year) {
    char dash1, dash2;
    stringstream ss(input);

    if (!(ss >> day >> dash1 >> month >> dash2 >> year)) return false;
    if (dash1 != '-' || dash2 != '-') return false;

    ss >> ws;
    return ss.eof();
}

bool parseTimeInput(const string &input, int &hour, int &minutes) {
    char colon;
    stringstream ss(input);

    if (!(ss >> hour >> colon >> minutes)) return false;
    if (colon != ':') return false;

    ss >> ws;
    return ss.eof();
}

bool parseDurationInput(const string &input, int &duration) {
    stringstream ss(input);

    if (!(ss >> duration)) return false;

    ss >> ws;
    return ss.eof();
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isPassedDate(int day, int month, int year) {
    time_t t = time(nullptr);
    tm *currentTime = localtime(&t);

    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;
    int currentDay = currentTime->tm_mday;

    if (year < currentYear) return true;
    if (year == currentYear && month < currentMonth) return true;
    if (year == currentYear && month == currentMonth && day < currentDay) return true;

    return false;
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