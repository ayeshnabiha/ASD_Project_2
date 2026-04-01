#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <thread>

#include "linkedList.h"
#include "queue.h"
#include "stack.h"
#include "validation.h"
#include "database.h"

using namespace std;

void userLogin(Reservation &user);
void displayMenu(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db);
void displayMenuOrLogout(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db);
void addNewReservation(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db);
void inputSchedule(Reservation &user);
void showReservationQueue(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db);
void showReservationHistory(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db);
void showReservationDetails(Reservation &user, Stack &stack, sqlite3 *db);
void continueOrLogout(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db);
char getYesNoInput(const string &prompt);
void logout();

int main()
{
    Reservation user;
    Queue queue;
    Stack stack;
    sqlite3 *db;

    initList(queue);
    initStack(stack);
    initDatabase(&db, "reservations.db");

    Node *savedData = loadReservations(db);
    while (savedData != nullptr)
    {
        enqueue(queue, savedData->data);
        savedData = savedData->next;
    }

    userLogin(user);
    displayMenu(user, queue, stack, db);

    return 0;
}

void userLogin(Reservation &user)
{
    cout << "Welcome to Computer Laboratory Reservation System!" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << string(50, '-') << "\n";
    cout << "Please enter your 6-digit NIU: ";

    cin >> user.niu;
    while (user.niu.length() != 6)
    {
        cout << "Invalid NIU! Please enter a valid 6-digit NIU: ";
        cin >> user.niu;
    }
}

void displayMenu(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    int menu;

    this_thread::sleep_for(chrono::seconds(1));
    cout << string(50, '-') << "\n";
    cout << "Menu" << endl;
    cout << "1. Add new reservation" << endl;
    cout << "2. Show reservation queue" << endl;
    cout << "3. Show reservation history" << endl;

    cout << "\nPlease select an option : ";
    cin >> menu;

    cout << string(50, '-') << "\n";

    if (menu == 1)
    {
        addNewReservation(user, queue, stack, db);
    }
    else if (menu == 2)
    {
        showReservationQueue(user, queue, stack, db);
    }
    else if (menu == 3)
    {
        showReservationHistory(user, queue,stack, db);
    }
}

void displayMenuOrLogout(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    int menu;

    cout << "Menu" << endl;
    cout << "1. Add new reservation" << endl;
    cout << "2. Show reservation queue" << endl;
    cout << "3. Show reservation history" << endl;
    cout << "4. Logout" << endl;

    cout << "\nPlease select an option : ";
    cin >> menu;
    cout << string(50, '-') << "\n";

    if (menu == 1)
    {
        addNewReservation(user, queue, stack, db);
    }
    else if (menu == 2)
    {
        showReservationQueue(user, queue, stack, db);
    }
    else if (menu == 3)
    {
        showReservationHistory(user, queue, stack, db);
    }
    else if (menu == 4)
    {
        logout();
    }
}

void addNewReservation(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    char confirm;
    this_thread::sleep_for(chrono::seconds(1));

    cout << "Enter your group name: ";
    cin.ignore();
    getline(cin, user.group_name);

    cout << "\nPurpose" << endl;
    cout << "1. Praktikum" << endl;
    cout << "2. Pelatihan" << endl;
    cout << "3. Other" << endl;
    cout << "Enter the purpose of reservation: ";
    cin >> user.purpose;

    while (user.purpose != 1 && user.purpose != 2 && user.purpose != 3)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid! Please enter one of the options (1, 2, or 3): ";
        cin >> user.purpose;
    }

    inputSchedule(user);

    cout << string(50, '-') << endl;
    cout << "Please check your reservation details" << endl;
    cout << "Name       : " << user.group_name << endl;
    cout << "Purpose    : " << (user.purpose == 1 ? "Praktikum" : (user.purpose == 2 ? "Pelatihan" : "Other")) << endl;
    cout << "Date       : " << user.date_day << "-" << user.date_month << "-" << user.date_year << endl;
    cout << "Schedule   : " << setfill('0') << setw(2) << user.time_start_hour << ":"  << setw(2) << user.time_start_minutes << " - " << setw(2) << user.time_stop_hour << ":"  << setw(2) << user.time_stop_minutes << endl;

    confirm = getYesNoInput("\nConfirm reservation? (Y/N): ");

    if (confirm == 'N' || confirm == 'n')
    {
        cout << string(50, '-') << endl;
        addNewReservation(user, queue, stack, db);
    }

    cout << "\nChecking available time slots";

    while (hasTimeConflict(db, user))
    {
        cout << "\nTime slot is already taken. Please choose another time." << endl;

        inputSchedule(user);

        cout << string(50, '-') << endl;
        cout << "Please check your reservation details" << endl;
        cout << "Name       : " << user.group_name << endl;
        cout << "Purpose    : " << (user.purpose == 1 ? "Praktikum" : (user.purpose == 2 ? "Pelatihan" : "Other")) << endl;
        cout << "Date       : " << user.date_day << "-" << user.date_month << "-" << user.date_year << endl;
        cout << "Schedule   : " << user.time_start_hour << ":" << user.time_start_minutes << " - " << user.time_stop_hour << ":" << user.time_stop_minutes << endl;

        confirm = getYesNoInput("\nConfirm reservation? (Y/N): ");

        if (confirm == 'N' || confirm == 'n')
        {
            addNewReservation(user, queue, stack, db);
        }

        cout << "\nChecking available time slots";
    }

    saveReservation(db, createNode(user));
    updateStatus(db, user.niu, "Accepted");
    enqueue(queue, user);
    push(stack, user);

    this_thread::sleep_for(chrono::seconds(1));
    cout << "\nAdding new reservation";
    for (int i = 0; i < 3; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << ".";
    }
    cout << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << string(50, '-') << endl;
    this_thread::sleep_for(chrono::seconds(1));

    showReservationDetails(user, stack, db);
    continueOrLogout(user, queue, stack, db);
}

void inputSchedule(Reservation &user)
{
    char dash1, dash2;

    cout << "\nEnter the date of reservation (DD-MM-YYYY): ";
    cin >> user.date_day >> dash1 >> user.date_month >> dash2 >> user.date_year;

    while (!isValidDate(user.date_day, user.date_month, user.date_year))
    {
        cout << "Invalid date! Please enter the valid date (DD-MM-YYYY): ";
        cin >> user.date_day >> dash1 >> user.date_month >> dash2 >> user.date_year;
    }

    cout << "\nEnter the start time of reservation (HH:MM): ";
    cin >> user.time_start_hour >> dash1 >> user.time_start_minutes;

    while (!isValidTime(user.time_start_hour, user.time_start_minutes))
    {
        cout << "Invalid time! Please enter the valid start time (HH:MM): ";
        cin >> user.time_start_hour >> dash1 >> user.time_start_minutes;
    }

    cout << "\nEnter the duration of reservation (in minutes): ";
    cin >> user.duration;

    while (!isValidDuration(user.duration))
    {
        cout << "Invalid duration! Please enter the valid duration (in minutes): ";
        cin >> user.duration;
    }

    int totalMinutes = user.time_start_hour * 60 + user.time_start_minutes + user.duration;
    user.time_stop_hour = (totalMinutes / 60) % 24; 
    user.time_stop_minutes = totalMinutes % 60;
}

void showReservationQueue(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    showQueue(queue);
    continueOrLogout(user, queue, stack, db);
}

void showReservationHistory(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    Stack history;
    initStack(history);

    Node *head = getReservationsByNIU(db, user.niu);

    Node *temp = head;
    while (temp != nullptr)
    {
        push(history, temp->data);
        temp = temp->next;
    }

    displayStack(history);

    freeList(history);

    continueOrLogout(user, queue, stack, db);
}

void showReservationDetails(Reservation &user, Stack &stack, sqlite3 *db)
{
    peek(stack);
}

void continueOrLogout(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    char choice;

    choice = getYesNoInput("Do you want to continue? (Y/N): ");
    cout << string(50, '-') << endl;

    if (choice == 'N' || choice == 'n')
    {
        logout();
    }
    else
    {
        displayMenuOrLogout(user, queue, stack, db);
    }
}

char getYesNoInput(const string &prompt)
{
    char input;

    while (true)
    {
        cout << prompt;
        cin >> input;

        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n')
        {
            return input;
        }

        cout << "Invalid input! Please enter Y or N.\n";
    }
}

void logout()
{
    cout << "Logging out";
    for (int i = 0; i < 3; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << ".";
    }
    this_thread::sleep_for(chrono::seconds(1));
    cout << "\nLogged out successfully!" << endl;
    cout << string(50, '-') << endl;
    return;
}
