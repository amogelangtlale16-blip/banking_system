#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Global constant for the number of users
const int TOTAL_USERS = 3;

// Function prototypes (using parameters and return values)
int loginUser(const int accNumbers[], const int pins[], int size);
void displayMenu();
double deposit(double currentBalance);
double withdraw(double currentBalance);
void sendCash(int senderIndex, const string names[], const int accNumbers[], double balances[], int size);
void checkBalance(string userName, int accNumber, double currentBalance);

int main() {
    // Parallel arrays storing details for 3 users (20 marks)
    string names[TOTAL_USERS] = {"Thabo Mokoena", "Lerato Ndlovu", "Sipho Khumalo"};
    int accNumbers[TOTAL_USERS] = {1001, 1002, 1003};
    int pins[TOTAL_USERS] = {1234, 4321, 5678};
    double balances[TOTAL_USERS] = {1500.00, 2500.50, 500.00};

    cout << "=====================================\n";
    cout << "       WELCOME TO GCC BANK           \n";
    cout << "=====================================\n";

    // Authenticate user before showing the main banking menu
    int currentUserIndex = loginUser(accNumbers, pins, TOTAL_USERS);

    // Exit if login fails after 3 attempts
    if (currentUserIndex == -1) {
        cout << "\nToo many failed attempts. Exiting system for security.\n";
        return 0;
    }

    cout << "\nLogin successful! Welcome, " << names[currentUserIndex] << ".\n";

    int choice = 0;

    // Loop keeps running until the user chooses 5 (Exit) (20 marks)
    do {
        displayMenu();
        cin >> choice;

        // Input validation: check if user entered letters instead of a number
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 5.\n";
            continue;
        }

        // Decision making using switch statement (15 marks)
        switch (choice) {
            case 1:
                balances[currentUserIndex] = deposit(balances[currentUserIndex]);
                break;
            case 2:
                balances[currentUserIndex] = withdraw(balances[currentUserIndex]);
                break;
            case 3:
                sendCash(currentUserIndex, names, accNumbers, balances, TOTAL_USERS);
                break;
            case 4:
                checkBalance(names[currentUserIndex], accNumbers[currentUserIndex], balances[currentUserIndex]);
                break;
            case 5:
                cout << "\nThank you for banking with GCC Bank. Goodbye!\n";
                break;
            default:
                cout << "Invalid menu choice! Please select an option from 1 to 5.\n";
        }

    } while (choice != 5);

    return 0;
}

// Function 1: Validates account number and PIN, returns user's array index
int loginUser(const int accNumbers[], const int pins[], int size) {
    int enteredAcc, enteredPin;
    int attempts = 0;

    while (attempts < 3) {
        cout << "\nEnter Account Number: ";
        cin >> enteredAcc;
        cout << "Enter 4-digit PIN: ";
        cin >> enteredPin;

        // Handle non-numeric input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Account number and PIN must be numbers.\n";
            attempts++;
            continue;
        }

        // Search arrays for matching credentials
        for (int i = 0; i < size; i++) {
            if (accNumbers[i] == enteredAcc && pins[i] == enteredPin) {
                return i; // Return index of logged-in user
            }
        }

        attempts++;
        cout << "Incorrect Account Number or PIN. Attempts left: " << (3 - attempts) << "\n";
    }
    return -1; // Login failed
}

// Function 2: Displays the required GCC BANK menu
void displayMenu() {
    cout << "\n===== GCC BANK =====\n";
    cout << "1. Deposit\n";
    cout << "2. Withdraw\n";
    cout << "3. Send Cash\n";
    cout << "4. Check Balance\n";
    cout << "5. Exit\n";
    cout << "Enter choice: ";
}

// Function 3: Adds money to balance and returns the updated balance
double deposit(double currentBalance) {
    double amount;
    cout << "Enter amount to deposit: R";
    cin >> amount;

    if (cin.fail() || amount <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Deposit amount must be greater than R0.00.\n";
        return currentBalance;
    }

    currentBalance += amount;
    cout << fixed << setprecision(2);
    cout << "Successfully deposited R" << amount << ". New balance: R" << currentBalance << "\n";
    return currentBalance;
}

// Function 4: Deducts money if funds are sufficient and returns updated balance
double withdraw(double currentBalance) {
    double amount;
    cout << "Enter amount to withdraw: R";
    cin >> amount;

    if (cin.fail() || amount <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Withdrawal amount must be greater than R0.00.\n";
        return currentBalance;
    }

    if (amount > currentBalance) {
        cout << "Error: Insufficient funds! You cannot withdraw more than your balance.\n";
        return currentBalance;
    }

    currentBalance -= amount;
    cout << fixed << setprecision(2);
    cout << "Successfully withdrew R" << amount << ". Remaining balance: R" << currentBalance << "\n";
    return currentBalance;
}

// Function 5: Transfers funds to another existing account
void sendCash(int senderIndex, const string names[], const int accNumbers[], double balances[], int size) {
    int recipientAcc;
    double amount;

    cout << "Enter recipient Account Number: ";
    cin >> recipientAcc;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Invalid account number format.\n";
        return;
    }

    // Prevent sending money to yourself
    if (recipientAcc == accNumbers[senderIndex]) {
        cout << "Error: You cannot transfer money to your own account.\n";
        return;
    }

    // Check if recipient account exists
    int recipientIndex = -1;
    for (int i = 0; i < size; i++) {
        if (accNumbers[i] == recipientAcc) {
            recipientIndex = i;
            break;
        }
    }

    if (recipientIndex == -1) {
        cout << "Error: Recipient account number does not exist.\n";
        return;
    }

    cout << "Enter amount to send to " << names[recipientIndex] << ": R";
    cin >> amount;

    if (cin.fail() || amount <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Transfer amount must be greater than R0.00.\n";
        return;
    }

    if (amount > balances[senderIndex]) {
        cout << "Error: Insufficient funds to complete this transfer.\n";
        return;
    }

    // Update both balances
    balances[senderIndex] -= amount;
    balances[recipientIndex] += amount;

    cout << fixed << setprecision(2);
    cout << "Successfully sent R" << amount << " to " << names[recipientIndex] << ".\n";
    cout << "Your new balance: R" << balances[senderIndex] << "\n";
}

// Function 6: Displays the user's current account balance
void checkBalance(string userName, int accNumber, double currentBalance) {
    cout << fixed << setprecision(2);
    cout << "\n--- Account Summary ---\n";
    cout << "Account Holder : " << userName << "\n";
    cout << "Account Number : " << accNumber << "\n";
    cout << "Current Balance: R" << currentBalance << "\n";
    cout << "-----------------------\n";
}
