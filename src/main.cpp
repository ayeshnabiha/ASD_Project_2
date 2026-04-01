#include <iostream>
#include <string>
#include <iomanip>
#include <thread>
#include <limits>
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
    enqueueLoadedDataAscending(queue, savedData);

    userLogin(user);
    displayMenu(user, queue, stack, db);

    freeList(queue);
    freeList(stack);
    closeDatabase(db);

    return 0;
}

void userLogin(Reservation &user)
{
    cout << "Welcome to Computer Laboratory Reservation System!" << endl;
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

    confirm = getYesNoInput("\nConfirm reservation? (Y/N): ");

    if (confirm == 'N' || confirm == 'n')
    {
        cout << string(50, '-') << endl;
        addNewReservation(user, queue, stack, db);
        return;
    }

    cout << "\nChecking available time slots" << endl;

    while (hasTimeConflict(db, user))
    {
        cout << "\nTime slot is already taken. Please choose another time." << endl;

        inputSchedule(user);

        confirm = getYesNoInput("\nConfirm reservation? (Y/N): ");

        if (confirm == 'N' || confirm == 'n')
        {
            addNewReservation(user, queue, stack, db);
            return;
        }

        cout << "\nChecking available time slots";
    }

    Node *reservationNode = createNode(user);
    bool isSaved = saveReservation(db, reservationNode);
    delete reservationNode;

    if (isSaved) {
        updateStatus(db, user.niu, "Accepted");
        
        user.status = "Accepted"; 
        
        enqueue(queue, user);
        sortByScheduleASC(queue);
        push(stack, user);

        cout << "\nAdding new reservation";
        for (int i = 0; i < 3; i++)
        {
            cout << ".";
        }
        cout << endl;
        cout << string(50, '-') << endl;

        showReservationDetails(user, stack, db);
    } else {
        cout << "\nFailed to add reservation." << endl;
        cout << "\nReturning to main menu";
    }

    continueOrLogout(user, queue, stack, db);
}

void inputSchedule(Reservation &user)
{
    string dateInput, timeInput, durationInput;

    while (true)
    {
        cout << "\nEnter the date of reservation (DD-MM-YYYY): ";

        getline(cin >> ws, dateInput);

        if (!parseDateInput(dateInput, user.date_day, user.date_month, user.date_year))
        {
            cout << "Invalid date format! Please use DD-MM-YYYY." << endl;
            continue;
        }

        if (!isValidDate(user.date_day, user.date_month, user.date_year))
        {
            cout << "Invalid date! Please enter the valid date (DD-MM-YYYY)." << endl;
            continue;
        }

        if (isPassedDate(user.date_day, user.date_month, user.date_year))
        {
            cout << "Date has already passed! Please enter a valid date (DD-MM-YYYY)." << endl;
            continue;
        }

        break;
    }

    while (true)
    {
        cout << "\nEnter the start time of reservation (HH:MM): ";

        getline(cin >> ws, timeInput);

        if (!parseTimeInput(timeInput, user.time_start_hour, user.time_start_minutes))
        {
            cout << "Invalid time format! Please use HH:MM." << endl;
            continue;
        }

        if (!isValidTime(user.time_start_hour, user.time_start_minutes))
        {
            cout << "Invalid time! Please enter the valid start time (HH:MM)." << endl;
            continue;
        }

        if (isPassedStartTime(user.date_day, user.date_month, user.date_year,
                                     user.time_start_hour, user.time_start_minutes))
        {
            cout << "Start time has already passed for today! Please enter a future time (HH:MM)." << endl;
            continue;
        }

        break;
    }

    while (true)
    {
        cout << "\nEnter the duration of reservation (in minutes): ";

        getline(cin >> ws, durationInput);

        if (!parseDurationInput(durationInput, user.duration))
        {
            cout << "Invalid duration! Please enter a number in minutes." << endl;
            continue;
        }

        if (!isValidDuration(user.duration))
        {
            cout << "Invalid duration! Please enter the valid duration (in minutes)." << endl;
            continue;
        }

        break;
    }

    int totalMinutes = user.time_start_hour * 60 + user.time_start_minutes + user.duration;
    user.time_stop_hour = (totalMinutes / 60) % 24; 
    user.time_stop_minutes = totalMinutes % 60;

    cout << string(50, '-') << endl;
    cout << "Please check your reservation details" << endl;
    cout << "Name       : " << user.group_name << endl;
    cout << "Purpose    : " << (user.purpose == 1 ? "Praktikum" : (user.purpose == 2 ? "Pelatihan" : "Other")) << endl;
    cout << "Date       : " << setfill('0') << setw(2) << user.date_day << "-" << setfill('0') << setw(2) << user.date_month << "-" << user.date_year << endl;
    cout << "Schedule   : " << setfill('0') << setw(2) << user.time_start_hour << ":" << setfill('0') << setw(2) << user.time_start_minutes << " - " << setfill('0') << setw(2) << user.time_stop_hour << ":" << setfill('0') << setw(2) << user.time_stop_minutes << endl;
}

void showReservationQueue(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    sortByScheduleASC(queue);
    showQueue(queue);
    continueOrLogout(user, queue, stack, db);
}

void showReservationHistory(Reservation &user, Queue &queue, Stack &stack, sqlite3 *db)
{
    Stack history;
    initStack(history);

    Node *head = getReservationsByNIU(db, user.niu);
    pushLoadedDataDescending(history, head);

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
        cout << ".";
    }
    cout << "\nLogged out successfully!" << endl;
    cout << string(50, '-') << endl;
    return;
}
