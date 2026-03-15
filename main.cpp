#include <iostream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

void userLogin();
void displayMenu();
void addNewReservation();
void showReservationQueue();
void showReservationHistory();
void showReservationDetails();
void continueOrLogout();
void displayMenuOrLogout();
void backToMenuOrLogout();

int main(){
    userLogin();
    displayMenu();
    
    return 0;
}

void userLogin(){
    string niu;

    cout << "Welcome to Computer Laboratory Reservation System!" << endl;
    this_thread::sleep_for(chrono::seconds(1)); 
    cout << "Please enter your NIU: ";
    cin >> niu;
    while(niu.length() != 6){
        cout << "Invalid NIU! Please enter a valid 6-digit NIU: ";
        cin >> niu;
    }

    /*
    call database function to keep the NIU for 
    - checking reservation history
    */
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

void displayMenuOrLogout(){
    char menu;

    cout << "\nMenu" << endl;

    cout << "1. Add new reservation" << endl;
    cout << "2. Show reservation queue" << endl;
    cout << "3. Show reservation history" << endl;
    cout << "4. Logout" << endl;
    
    cout << "\nPlease select an option : ";
    cin >> menu;

    if(menu == 1){
        addNewReservation();
    } else if(menu == 2){
        showReservationQueue();
    } else if(menu == 3){
        showReservationHistory();
    } else if(menu == 4){
        cout << "\nLogging out"; 
        this_thread::sleep_for(chrono::seconds(1));
        cout << ".";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(2));
        cout << "\nLogged out successfully!" << endl;
        exit(0);
    }
}

void addNewReservation(){
    string name, duration, date; 
    char purpose, confirm;
    
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\nEnter your name: ";
    cin >> name;

    cout << "Purpose" << endl;
    cout << "1. Praktikum" << endl;
    cout << "2. Pelatihan" << endl;
    cout << "3. Other" << endl;
    cout << "Enter the purpose of reservation: ";
    cin >> purpose;
    while(purpose != '1' && purpose != '2' && purpose != '3'){
        cout << "Invalid! Please enter one of the option above : ";
        cin >> purpose;
    }

    cout << "Enter the duration of reservation (in hours): ";
    cin >> duration;

    cout << "Enter the date of reservation (YYYY-MM-DD): ";
    cin >> date;
    while(date.length() != 10 || date[4] != '-' || date[7] != '-'){
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format : ";
        cin >> date;
    }

    cout << "\nPlease check the reservation details" << endl;
    cout << "Name: " << name << endl;
    cout << "Purpose: " << (purpose == '1' ? "Praktikum" : (purpose == '2' ? "Pelatihan" : "Other")) << endl;
    cout << "Duration: " << duration << " hours" << endl;
    cout << "Date: " << date << endl;

    cout << "\nConfirm reservation? (Y/N): ";
    cin >> confirm;

    while(confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n'){
        cout << "Invalid input! Please enter Y or N." << endl;
        cout << "\nConfirm reservation? (Y/N): ";
        cin >> confirm;
    }

    if(confirm == 'N'|| confirm == 'n'){
        addNewReservation();
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

        showReservationDetails();
        continueOrLogout();
    }
}

void showReservationQueue(){
    //call queue function

    backToMenuOrLogout();
}

void showReservationHistory(){
    //call stack function

    backToMenuOrLogout();
}

void showReservationDetails(){
    //call stack function to show reservation details
}

void continueOrLogout(){
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
        cout << ".";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(1));
        cout << ". ";
        this_thread::sleep_for(chrono::seconds(2));
        cout << "\nLogged out successfully!" << endl;
        exit(0);
    }
    else{displayMenuOrLogout();}
}
    
void backToMenuOrLogout(){
    char choice;

    cout << "\nDo you want to go back to menu? (Y/N): ";
    cin >> choice;

    while(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
        cout << "Invalid input! Please enter Y or N." << endl;
        cout << "\nDo you want to go back to menu? (Y/N): ";
        cin >> choice;
    }

    if(choice == 'N' || choice == 'n'){displayMenuOrLogout();}
}
