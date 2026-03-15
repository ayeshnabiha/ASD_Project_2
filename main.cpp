#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

void userLogin(){
    string niu;

    cout << "Welcome to Computer Laboratory Reservation System!" << endl;
    this_thread::sleep_for(chrono::seconds(1)); 
    cout << "Please enter your NIU: ";
    cin >> niu;

    /*
    call database function to keep the NIU for 
    - checking reservation history
    */
}

void addNewReservation(){
    string name, duration, date; 
    char purpose;

    
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\nEnter your name: ";
    cin >> name;

    cout << "Purpose" << endl;
    cout << "1. Praktikum" << endl;
    cout << "2. Pelatihan" << endl;
    cout << "3. Other" << endl;
    cout << "Enter the purpose of reservation: ";
    cin >> purpose;
    while(purpose != '1' || '2' || '3'){
        cout << "Invalid! Please enter one of the option above : ";
        cin >> purpose;
    }

    cout << "Enter the duration of reservation (in hours): ";
    cin >> duration;

    cout << "Enter the date of reservation (YYYYMMDD): ";
    cin >> date;
    while(date.length() != 8){
        cout << "Invalid date format. Please enter the date in YYYYMMDD format : ";
        cin >> date;
   }

    cout << "\nAdding new reservation";
    this_thread::sleep_for(chrono::seconds(1));
    cout << " .";
    this_thread::sleep_for(chrono::seconds(1));
    cout << " .";
    this_thread::sleep_for(chrono::seconds(1));
    cout << " .";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "\nReservation added successfully!" << endl;

}

void showReservationQueue(){
    //call queue function
}

void showReservationHistory(){
    //call stack function
}

void displayMenu(){
    int menu;
    
    this_thread::sleep_for(chrono::seconds(1)); 
    
    cout << "\nMenu" << endl;

    cout << "1. Add new reservation" << endl;
    cout << "2. Show reservation queue" << endl;
    cout << "3. Show reservation history" << endl;
    
    cout << "\nPlease select an option : ";

    cin >> menu;

    if(menu == 1){
        addNewReservation();
    } else if(menu == 2){
        showReservationQueue();
    } else if(menu == 3){
        showReservationHistory();
    }
}

int main(){
    userLogin();
    displayMenu();
    
    return 0;
}