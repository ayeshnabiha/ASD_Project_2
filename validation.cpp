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

bool isValidDuration(int time_start_hour, int time_start_minutes,
                     int time_stop_hour, int time_stop_minutes,
                     int duration) {
    // check stop is after start
    int startTotal = time_start_hour * 60 + time_start_minutes;
    int stopTotal  = time_stop_hour  * 60 + time_stop_minutes;

    if (stopTotal <= startTotal) return false;

    // check duration matches start and stop
    int actualDuration = stopTotal - startTotal;
    if (actualDuration != duration) return false;

    return true;
}

bool isValidReservation(const Reservation& r, std::string& errorMsg) {
    if (r.group_name.empty()) {
        errorMsg = "Nama kelompok tidak boleh kosong.";
        return false;
    }

    if (r.niu.empty()) {
        errorMsg = "NIU tidak boleh kosong.";
        return false;
    }

    if (!isValidDate(r.date_day, r.date_month, r.date_year)) {
        errorMsg = "Tanggal tidak valid.";
        return false;
    }

    if (!isValidTime(r.time_start_hour, r.time_start_minutes)) {
        errorMsg = "Jam mulai tidak valid.";
        return false;
    }

    if (!isValidTime(r.time_stop_hour, r.time_stop_minutes)) {
        errorMsg = "Jam selesai tidak valid.";
        return false;
    }

    if (!isValidDuration(r.time_start_hour, r.time_start_minutes,
                         r.time_stop_hour, r.time_stop_minutes,
                         r.duration)) {
        errorMsg = "Durasi tidak sesuai dengan jam mulai dan jam selesai.";
        return false;
    }

    if (r.purpose.empty()) {
        errorMsg = "Keperluan tidak boleh kosong.";
        return false;
    }

    return true;
}