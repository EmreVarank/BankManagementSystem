#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <limits> 
using namespace std;

class Account {
public:
    string accHolder;
    int accID;
    int password;
    double balance;

    Account(string name, int id, int pass, double initialBalance)
        : accHolder(name), accID(id), password(pass), balance(initialBalance) {
    }

    vector <string> transactionHistory;

    void deposit(double amount) {           //deposit
        while (amount <= 0) {
            cout << "Invalid deposit amount. Please enter a positive amount: ";
            cin >> amount;
        }
        balance += amount;
        transactionHistory.push_back("Deposited: " + to_string(amount));
    }

    void withdraw(double amount) {          //withdraw
        while (amount <= 0 || amount > balance) {
            if (amount <= 0) {
                cout << "Invalid withdrawal amount. Please enter a positive amount: ";
            }
            else {
                cout << "Insufficient balance. Please enter a valid amount: ";
            }
            cin >> amount;
        }
        balance -= amount;
        transactionHistory.push_back("Withdrew: " + to_string(amount));
    }

    void displayAccount() {               //display acc infos
        cout << "Account Holder: " << accHolder << endl;
        cout << "Account ID: " << accID << endl;
        cout << "Balance: " << balance << endl;
    }

    void displayTransactionHistory() {    // display transaction history
        cout << "Transaction History:" << endl;
        for (const string& transaction : transactionHistory) {
            cout << transaction << endl;
        }
    }

    void moneyTransfer(Account& recipient) {        //transfer
        int recipientID;
        string recipientName;
        double transferAmount;
        int attempts = 0;

        while (true) {  // fix = 230
            cout << "Enter the recipient's Account ID: ";
            cin >> recipientID;

            if (recipientID != recipient.accID) {
                attempts++;
                cout << "Recipient's Account ID does not match. Please try again." << endl;

                if (attempts == 3) {
                    cout << "Too many incorrect attempts. Transfer cancelled." << endl;
                    return;
                }
            }
            else {
                break;
            }
        }

        attempts = 0;
        while (true) {  // fix = 230
            cout << "Enter the recipient's Name (First and Last): ";
            cin.ignore();
            getline(cin, recipientName);

            if (recipientName != recipient.accHolder) {
                attempts++;
                cout << "Recipient's Name does not match. Please try again." << endl;

                if (attempts == 3) {
                    cout << "Too many incorrect attempts. Transfer cancelled." << endl;
                    return;
                }
            }
            else {
                break;
            }
        }

        cout << "Enter amount to transfer: ";
        cin >> transferAmount;

        // Validate transfer amount
        if (transferAmount > 0 && transferAmount <= balance) {
            withdraw(transferAmount);
            recipient.deposit(transferAmount);

            // record transaction history 
            transactionHistory.push_back("Transferred: " + to_string(transferAmount) + " to account " + to_string(recipient.accID));
            recipient.transactionHistory.push_back("Received: " + to_string(transferAmount) + " from account " + to_string(accID));

            cout << "Transfer of " << transferAmount << " completed successfully." << endl;
            cout << "New Balance: " << balance << endl;
        }
        else {
            cout << "Insufficient balance or invalid transfer amount." << endl;
        }
    }
};

bool validInput(int& choice) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please enter a valid number." << endl;
        return false;
    }
    return true;
}

void showMenu(const string& userName) {
    cout << "\n************************** Welcome To The IMF **************************" << endl;
    cout << "\n1-) Deposit               2-) Withdraw                3-) Display Account \n4-) Money Transfer        5-) Display Transaction     6-) Exit" << endl;
}

int getPassword() {    //ASCII for hiding password
    int password = 0;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) {
            break;
        }
        if (ch == 8) {
            password /= 10;
            cout << "\b \b";
        }
        else {
            password = password * 10 + (ch - '0');
            cout << "*";
        }
    }
    return password;
}

int main() {
    int id, password, choice, dep, wit, say = 0;

    // accounts 
    Account account1("Agent Smith", 123, 123, 10000);
    Account account2("Agent Brown", 456, 456, 7500);
    Account account3("Agent Jones", 789, 789, 5000);
    Account* currentAccount = nullptr; // current acc

    do {
        cout << "Please enter your Account ID: " << endl;
        cin >> id;

        // id 
        if (id == account1.accID || id == account2.accID || id == account3.accID) {
            cout << "Please enter your password: " << endl;
            password = getPassword();

            // password
            if ((id == account1.accID && password == account1.password) ||
                (id == account2.accID && password == account2.password) ||
                (id == account3.accID && password == account3.password)) {
                break;
            }
            else {
                say++;
                cout << "\nWrong password. " << 3 - say << " tries remaining." << endl;
                if (say == 3) {
                    cout << "Too many incorrect attempts. Exiting..." << endl;
                    return 0;
                }
            }
        }
        else {
            cout << "Account ID not found. Please try again." << endl;
        }
    } while (true);

    // assigning the current account based on the ID

    if (id == account1.accID) {
        currentAccount = &account1;
    }
    else if (id == account2.accID) {
        currentAccount = &account2;
    }
    else if (id == account3.accID) {
        currentAccount = &account3;
    }

    cout << "\nHello, " << currentAccount->accHolder << endl;

    do {
        showMenu(currentAccount->accHolder);
        cin >> choice;

        if (validInput(choice)) {
            switch (choice) {
            case 1:
                cout << "How much money do you want to deposit: ";
                cin >> dep;
                currentAccount->deposit(dep);
                break;
            case 2:
                cout << "How much money do you want to withdraw: ";
                cin >> wit;
                currentAccount->withdraw(wit);
                break;
            case 3:
                currentAccount->displayAccount();
                break;
            case 4:
                int recipientChoice;
                cout << "Select the recipient for money transfer:" << endl; // fix = don't ask for current acc, names already given.
                cout << "1. Agent Smith" << endl;
                cout << "2. Agent Brown" << endl;
                cout << "3. Agent Jones" << endl;
                cout << "Enter your choice: ";
                cin >> recipientChoice;

                if (recipientChoice == 1) {
                    currentAccount->moneyTransfer(account1);
                }
                else if (recipientChoice == 2) {
                    currentAccount->moneyTransfer(account2);
                }
                else if (recipientChoice == 3) {
                    currentAccount->moneyTransfer(account3);
                }
                else {
                    cout << "Invalid recipient choice." << endl;
                }
                break;
            case 5:
                currentAccount->displayTransactionHistory();
                break;
            case 6:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    } while (choice != 6);

    return 0;
}