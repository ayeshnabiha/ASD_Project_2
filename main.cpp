#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>

#include "linkedList.h"
#include "queue.h"
#include "stack.h"
#include "validation.h"
#include "database.h"

using namespace std;

void userLogin(Reservation &user);
void displayMenu(Reservation &user, Queue &list, sqlite3* db);
void displayMenuOrLogout(Reservation &user, Queue &list, sqlite3* db);
void addNewReservation(Reservation &user, Queue &list, sqlite3* db);
void showReservationQueue(Reservation &user, Queue &list, sqlite3* db);
void showReservationHistory(Reservation &user, Queue &list, sqlite3* db);
void showReservationDetails(Reservation &user, Queue &list, sqlite3* db);
void continueOrLogout(Reservation &user, Queue &list, sqlite3* db);
void logout();

int main(){
    Reservation user;
    Queue list;
    sqlite3* db;
    
    initList(list);
    initDatabase(&db, "reservations.db");

    Node* savedData = loadReservations(db);
    while(savedData != nullptr) {
        enqueue(list, savedData->data);
        savedData = savedData->next;
    }

    userLogin(user);
    displayMenu(user, list, db);

    // freeList(list);
    // closeDatabase(db);

    return 0;
}

void userLogin(Reservation &user){
    cout << "Welcome to Computer Laboratory Reservation System!" << endl;
    this_thread::sleep_for(chrono::seconds(1)); 
    cout << string(50, '-') << "\n";
    cout << "Please enter your 6-digit NIU: ";
    
    cin >> user.niu;
    while(user.niu.length() != 6){
        cout << "Invalid NIU! Please enter a valid 6-digit NIU: ";
        cin >> user.niu;
    }
}

void displayMenu(Reservation &user, Queue &list, sqlite3* db){
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

    if(menu == 1){
        addNewReservation(user, list, db);
    } else if(menu == 2){
        showReservationQueue(user, list, db);
    } else if(menu == 3){
        showReservationHistory(user, list, db);
    }
}

void displayMenuOrLogout(Reservation &user, Queue &list, sqlite3* db){
    int menu;

    cout << "Menu" << endl;
    cout << "1. Add new reservation" << endl;
    cout << "2. Show reservation queue" << endl;
    cout << "3. Show reservation history" << endl;
    cout << "4. Logout" << endl;
    
    cout << "\nPlease select an option : ";
    cin >> menu;
    cout << string(50, '-') << "\n";

    if(menu == 1){
        addNewReservation(user, list, db);
    } else if(menu == 2){
        showReservationQueue(user, list, db);
    } else if(menu == 3){
        showReservationHistory(user, list, db);
    } else if(menu == 4){
        logout();
    }
}

void addNewReservation(Reservation &user, Queue &list, sqlite3* db){
    char confirm, dash1, dash2;
    
    this_thread::sleep_for(chrono::seconds(1));

    cout << "Enter your group name: ";
    cin >> user.group_name;

    cout << "\nPurpose" << endl;
    cout << "1. Praktikum" << endl;
    cout << "2. Pelatihan" << endl;
    cout << "3. Other" << endl;
    cout << "Enter the purpose of reservation: ";
    cin >> user.purpose;

    while(user.purpose != 1 && user.purpose != 2 && user.purpose != 3){
        cout << "Invalid! Please enter one of the option above: ";
        cin >> user.purpose;
    }

    cout << "\nEnter the date of reservation (DD-MM-YYYY): ";
    cin >> user.date_day >> dash1 >> user.date_month >> dash2 >> user.date_year;

    while(!isValidDate(user.date_day, user.date_month, user.date_year)){
        cout << "Invalid date! Please enter the valid date (DD-MM-YYYY): ";
        cin >> user.date_day >> dash1 >> user.date_month >> dash2 >> user.date_year;
    }

    cout << "\nEnter the start time of reservation (HH:MM): ";
    cin >> user.time_start_hour >> dash1 >> user.time_start_minutes;

    while(!isValidTime(user.time_start_hour, user.time_start_minutes)){
        cout << "Invalid time! Please enter the valid start time (HH:MM): ";
        cin >> user.time_start_hour >> dash1 >> user.time_start_minutes;
    }

    cout << "\nEnter the duration of reservation (in minutes): ";
    cin >> user.duration;

    user.time_stop_hour = user.time_start_hour + (user.duration / 60);
    user.time_stop_minutes = user.time_start_minutes + (user.duration % 60);

    while(!isValidDuration(user.duration)){
        cout << "Invalid duration! Please enter the valid duration (in minutes): ";
        cin >> user.duration;

    if (user.time_stop_minutes >= 60) {
        user.time_stop_hour += user.time_stop_minutes / 60;
        user.time_stop_minutes %= 60;}
    }

    cout << string(50, '-') << endl;
    cout << "Please check your reservation details" << endl;
    cout << "Name       : " << user.group_name << endl;
    cout << "Purpose    : " << (user.purpose == 1 ? "Praktikum" : (user.purpose == 2 ? "Pelatihan" : "Other")) << endl;
    cout << "Schedule   : " << user.time_start_hour << ":" << user.time_start_minutes << " - " << user.time_stop_hour << ":" << user.time_stop_minutes << endl;
    cout << "Date       : " << user.date_day << "-" << user.date_month << "-" << user.date_year << endl;

    cout << "\nConfirm reservation? (Y/N): ";
    cin >> confirm;

    while(confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n'){
        cout << "Invalid input! Please enter Y or N." << endl;
        cout << "\nConfirm reservation? (Y/N): ";
        cin >> confirm;
    };

    if(confirm == 'N' || confirm == 'n'){
        addNewReservation(user, list, db);
    }    
    
    cout << "\nChecking available time slots";

    hasTimeConflict(db, user);
    saveReservation(db, createNode(user));
    updateStatus(db, user.niu, "Accepted");
    enqueue(list, user);
    push(list, user);

    this_thread::sleep_for(chrono::seconds(1));
    cout << "\nAdding new reservation";
    for(int i = 0; i < 3; i++){
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << ".";
    }
    cout << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << string(50, '-') << endl;
    this_thread::sleep_for(chrono::seconds(1));

    showReservationDetails(user, list, db);
    continueOrLogout(user, list, db);
}

void showReservationQueue(Reservation &user, Queue &list, sqlite3* db){
    showQueue(list); 
    continueOrLogout(user, list, db);
}

void showReservationHistory(Reservation &user, Queue &list, sqlite3* db){
    Stack history;
    initStack(history);

    Node* head = getReservationsByNIU(db, user.niu);

    Node* temp = head;
    while(temp != nullptr) {
        push(history, temp->data);
        temp = temp->next;
    }

    displayList(history, "RESERVATION HISTORY");

    freeList(history);

    continueOrLogout(user, list, db);
}

void showReservationDetails(Reservation &user, Queue &list, sqlite3* db){
    peek(list);
}

void continueOrLogout(Reservation &user, Queue &list, sqlite3* db){
    char choice;

    cout << "Do you want to continue? (Y/N): ";
    cin >> choice;

    while(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
        cout << "Invalid input! Please enter Y or N." << endl;
        cout << "\nDo you want to continue? (Y/N): ";
        cin >> choice;
        cout << string(50, '-') << "\n";
    }

    if(choice == 'N' || choice == 'n'){
        logout(); 
    }
    else{
        cout << endl;
        displayMenuOrLogout(user, list, db);
    }
}

void logout(){
    cout << "\nLogging out"; 
    for(int i = 0; i < 3; i++){
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << ".";
        cout.flush();
    }
    this_thread::sleep_for(chrono::seconds(1));
    cout << "\nLogged out successfully!" << endl;
    cout << string(50, '-') << endl;
    //freeList(queue);
    //closeDatabase(db);
    return; 
}
