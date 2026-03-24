#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "linkedList.h"
#include "queue.h"
#include "validation.h"


using namespace std;

void userLogin(Reservation &user);
void displayMenu(Reservation &user, Queue &queue);
void displayMenuOrLogout(Reservation &user, Queue &queue);
void addNewReservation(Reservation &user, Queue &queue);
void showReservationQueue(Reservation &user, Queue &queue);
void showReservationHistory(Reservation &user, Queue &queue);
void showReservationDetails();
void continueOrLogout(Reservation &user, Queue &queue);
void backToMenuOrLogout(Reservation &user, Queue &queue);

int main(){
    Reservation user;
    Queue queue;
    // userLogin(user);
    // displayMenu(user, queue);
    addNewReservation(user, queue);
    // showReservationQueue(user, queue);

    return 0;
}

void userLogin(Reservation &user){
    cout << "Welcome to Computer Laboratory Reservation System!" << endl;
    this_thread::sleep_for(chrono::seconds(1)); 
    
    cout << "Please enter your NIU: ";
    
    cin >> user.niu;
    while(user.niu.length() != 6){
        cout << "Invalid NIU! Please enter a valid 6-digit NIU: ";
        cin >> user.niu;
    }

    /*
    call database function to keep the NIU for 
    - checking reservation history
    */
}

void displayMenu(Reservation &user, Queue &queue){
    int menu;
    
    this_thread::sleep_for(chrono::seconds(1)); 
    
    cout << "\nMenu" << endl;

    cout << "1. Add new reservation" << endl;
    cout << "2. Show reservation queue" << endl;
    cout << "3. Show reservation history" << endl;
    
    cout << "\nPlease select an option : ";

    cin >> menu;

    if(menu == 1){
        addNewReservation(user, queue);
    } else if(menu == 2){
        showReservationQueue(user, queue);}
    // } else if(menu == 3){
    //     showReservationHistory();
    // }
}

void displayMenuOrLogout(Reservation &user, Queue &queue){
    char menu;

    cout << "\nMenu" << endl;

    cout << "1. Add new reservation" << endl;
    cout << "2. Show reservation queue" << endl;
    cout << "3. Show reservation history" << endl;
    cout << "4. Logout" << endl;
    
    cout << "\nPlease select an option : ";
    cin >> menu;

    if(menu == 1){
        addNewReservation(user, queue);
    } else if(menu == 2){
        showReservationQueue(user, queue);}
    // } else if(menu == 3){
    //     showReservationHistory();
    // } else if(menu == 4){
    //     cout << "\nLogging out"; 
    //     this_thread::sleep_for(chrono::seconds(1));
    //     cout << ".";
    //     this_thread::sleep_for(chrono::seconds(1));
    //     cout << ". ";
    //     this_thread::sleep_for(chrono::seconds(1));
    //     cout << ". ";
    //     this_thread::sleep_for(chrono::seconds(2));
    //     cout << "\nLogged out successfully!" << endl;
    //     exit(0);
    // }
}

void addNewReservation(Reservation &user, Queue &queue){
    char confirm, dash1, dash2;
    
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\nEnter your group name: ";
    cin >> user.group_name;

    cout << "Purpose" << endl;
    cout << "1. Praktikum" << endl;
    cout << "2. Pelatihan" << endl;
    cout << "3. Other" << endl;
    cout << "Enter the purpose of reservation: ";
    cin >> user.purpose;

    while(user.purpose != '1' && user.purpose != '2' && user.purpose != '3'){
        cout << "Invalid! Please enter one of the option above : ";
        cin >> user.purpose;
    }

    cout << "Enter the duration of reservation (in minutes): ";
    cin >> user.duration;

    cout << "Enter the start time of reservation (HH:MM): ";
    cin >> user.time_start_hour >> dash1 >> user.time_start_minutes;

    user.time_stop_hour = user.time_start_hour + (user.duration / 60);
    user.time_stop_minutes = user.time_start_minutes + (user.duration % 60);

    cout << "Enter the date of reservation (DD-MM-YYYY): ";
    cin >> user.date_day >> dash1 >> user.date_month >> dash2 >> user.date_year;

    while(!isValidDate(user.date_day, user.date_month, user.date_year) || 
          !isValidTime(user.time_start_hour, user.time_start_minutes) || 
          !isValidDuration(user.time_start_hour, user.time_start_minutes, user.time_stop_hour, user.time_stop_minutes, user.duration)){
        cout << "Invalid date or time! Please enter the details again." << endl;

        cout << "Enter the duration of reservation (in minutes): ";
        cin >> user.duration;

        cout << "Enter the start time of reservation (HH:MM): ";
        cin >> user.time_start_hour >> dash1 >> user.time_start_minutes;

        user.time_stop_hour = user.time_start_hour + (user.duration / 60);
        user.time_stop_minutes = user.time_start_minutes + (user.duration % 60);

        cout << "Enter the date of reservation (YYYY-MM-DD): ";
        cin >> user.date_year >> dash1 >> user.date_month >> dash2 >> user.date_day;
    }

    cout << "\nPlease check your reservation details" << endl;
    cout << "Name       : " << user.group_name << endl;
    cout << "Purpose    : " << (user.purpose == '1' ? "Praktikum" : (user.purpose == '2' ? "Pelatihan" : "Other")) << endl;
    cout << "Schedule   : " << user.time_start_hour << ":" << user.time_start_minutes << " - " << user.time_stop_hour << ":" << user.time_stop_minutes << endl;
    cout << "Date       : " << user.date_day << "-" << user.date_month << "-" << user.date_year << endl;

    cout << "\nConfirm reservation? (Y/N): ";
    cin >> confirm;

    while(confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n'){
        cout << "Invalid input! Please enter Y or N." << endl;
        cout << "\nConfirm reservation? (Y/N): ";
        cin >> confirm;
    }

    if(confirm == 'N'|| confirm == 'n'){
        addNewReservation(user, queue);
    }
    else if (confirm == 'Y' || confirm == 'y'){
        cout << "\nChecking available time slots";
        //call database function to check available time slots

        this_thread::sleep_for(chrono::seconds(1));
        cout << "\nAdding new reservation";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(2));
        cout << "\nReservation added successfully!" << endl;

        // showReservationDetails();
        continueOrLogout(user, queue);
    }
}

void showReservationQueue(Reservation &user, Queue &queue){
    showQueue(queue); 
    backToMenuOrLogout(user, queue);
}

void showReservationHistory(Reservation &user, Queue &queue){
    //call stack function

    backToMenuOrLogout(user, queue);
}

void showReservationDetails(){
    //call stack function to show reservation details
}

void continueOrLogout(Reservation &user, Queue &queue){
    char choice;

    cout << "\nDo you want to continue? (Y/N): ";
    cin >> choice;

    while(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
        cout << "Invalid input! Please enter Y or N." << endl;
        cout << "\nDo you want to continue? (Y/N): ";
        cin >> choice;
    }

    if(choice == 'N' || choice == 'n'){
        cout << "\nLogging out"; 
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(2));
        cout << "\nLogged out successfully!" << endl;
        exit(0);
    }
    else{displayMenuOrLogout(user, queue);}
}
    
void backToMenuOrLogout(Reservation &user, Queue &queue){
    char choice;

    cout << "\nDo you want to go back to menu? (Y/N): ";
    cin >> choice;

    while(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
        cout << "Invalid input! Please enter Y or N." << endl;
        cout << "\nDo you want to go back to menu? (Y/N): ";
        cin >> choice;
    }

    if(choice == 'N' || choice == 'n'){displayMenuOrLogout(user, queue);}
}
