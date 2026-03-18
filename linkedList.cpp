#include "linkedList.h"

// Function Implementation
Node *createNode(std::string group_name, std::string niu,
                 int date_day, int date_month, int date_year,
                 std::string purpose, int duration,
                 int time_start_hour, int time_start_minutes,
                 int time_stop_hour, int time_stop_minutes,
                 std::string status)
{
    Node *newNode = new Node();

    newNode->data.group_name = group_name;
    newNode->data.niu = niu;
    newNode->data.date_day = date_day;
    newNode->data.date_month = date_month;
    newNode->data.date_year = date_year;
    newNode->data.purpose = purpose;
    newNode->data.duration = duration;
    newNode->data.time_start_hour = time_start_hour;
    newNode->data.time_start_minutes = time_start_minutes;
    newNode->data.time_stop_hour = time_stop_hour;
    newNode->data.time_stop_minutes = time_stop_minutes;
    newNode->data.status = status;

    newNode->next = nullptr;

    return newNode;
}