#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>

// Struct for Reservation
struct Reservation
{
    std::string group_name;
    std::string niu;
    int date_day;
    int date_month;
    int date_year;
    std::string purpose;
    int duration; // in minutes
    int time_start_hour;
    int time_start_minutes;
    int time_stop_hour;
    int time_stop_minutes;
    std::string status; // "pending", "approved", "rejected"
};

// Struct for Linked List Mechanism
struct Node
{
    Reservation data;
    Node *next;
};

// Function Declaration
Node *createNode(std::string group_name, std::string niu,
                 int date_day, int date_month, int date_year,
                 std::string purpose, int duration,
                 int time_start_hour, int time_start_minutes,
                 int time_stop_hour, int time_stop_minutes,
                 std::string status = "pending");

#endif