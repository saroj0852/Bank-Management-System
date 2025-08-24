#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <memory> // for unique_ptr

using namespace std;

// Base class for all accounts
class Account {
protected:
    string accountNumber;
    string accountHolderName;
    string accountType;
    double balance;

public:
    Account(string accNum, string accHolderName, string accType, double initialBalance)
        : accountNumber(accNum), accountHolderName(accHolderName), accountType(accType), balance(initialBalance) {}

    string getAccountNumber() const {
        return accountNumber;
    }

    string getAccountHolderName() const {
        return accountHolderName;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            throw invalid_argument("Deposit amount must be positive.");
        }
        balance += amount;
        cout << "Deposit successful! New balance: $" << balance << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            throw invalid_argument("Withdrawal amount must be positive.");
        }
        if (amount > balance) {
            throw runtime_error("Insufficient balance.");
        }
        balance -= amount;
        cout << "Withdrawal successful! New balance: $" << balance << endl;
    }

    virtual void displayAccountInfo() const {
        cout << "---------------------------------\n";
        cout << "Account Number : " << accountNumber << endl;
        cout << "Holder Name    : " << accountHolderName << endl;
        cout << "Account Type   : " << accountType << endl;
        cout << "Balance        : $" << balance << endl;
        cout << "---------------------------------\n";
    }

    virtual ~Account() {}
};

// Derived class for SavingsAccount
class SavingsAccount : public Account {
public:
    SavingsAccount(string accNum, string accHolderName, double initialBalance)
        : Account(accNum, accHolderName, "Savings", initialBalance) {}
};

// Derived class for CurrentAccount
class CurrentAccount : public Account {
public:
    CurrentAccount(string accNum, string accHolderName, double initialBalance)
        : Account(accNum, accHolderName, "Current", initialBalance) {}
};

// Bank Management System class
class BankManagementSystem {
private:
    vector<unique_ptr<Account>> accounts;
    int accountCounter = 1001;

    string generateAccountNumber() {
        return "ACC" + to_string(accountCounter++);
    }

public:
    void openAccount() {
        string name, address, accType;
        double initialDeposit;

        cout << "Enter Account Holder Name: ";
        getline(cin >> ws, name);
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter Account Type (Savings/Current): ";
        getline(cin, accType);

        if (accType != "Savings" && accType != "Current") {
            cout << "Invalid account type. Please enter either 'Savings' or 'Current'." << endl;
            return;
        }

        cout << "Enter Initial Deposit: ";
        cin >> initialDeposit;

        string accountNumber = generateAccountNumber();

        if (accType == "Savings") {
            accounts.push_back(make_unique<SavingsAccount>(accountNumber, name, initialDeposit));
        } else {
            accounts.push_back(make_unique<CurrentAccount>(accountNumber, name, initialDeposit));
        }

        cout << "Account created successfully! Account Number: " << accountNumber << endl;
    }

    void depositMoney() {
        string accNumber;
        double amount;

        cout << "Enter Account Number: ";
        cin >> accNumber;

        Account* account = findAccount(accNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }

        cout << "Enter amount to deposit: ";
        cin >> amount;

        try {
            account->deposit(amount);
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void withdrawMoney() {
        string accNumber;
        double amount;

        cout << "Enter Account Number: ";
        cin >> accNumber;

        Account* account = findAccount(accNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }

        cout << "Enter amount to withdraw: ";
        cin >> amount;

        try {
            account->withdraw(amount);
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void displayAccount() {
        string accNumber;

        cout << "Enter Account Number: ";
        cin >> accNumber;

        Account* account = findAccount(accNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }

        account->displayAccountInfo();
    }

    void closeAccount() {
        string accNumber;

        cout << "Enter Account Number to Close: ";
        cin >> accNumber;

        auto it = find_if(accounts.begin(), accounts.end(), [&](const unique_ptr<Account>& acc) {
            return acc->getAccountNumber() == accNumber;
        });

        if (it != accounts.end()) {
            accounts.erase(it);
            cout << "Account closed successfully!" << endl;
        } else {
            cout << "Account not found!" << endl;
        }
    }

    void displayAllAccounts() {
        if (accounts.empty()) {
            cout << "No accounts found!" << endl;
            return;
        }

        for (const auto& account : accounts) {
            account->displayAccountInfo();
        }
    }

private:
    Account* findAccount(const string& accNumber) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accNumber) {
                return account.get(); // return raw pointer from unique_ptr
            }
        }
        return nullptr;
    }
};

// Main function
int main() {
    BankManagementSystem bankSystem;
    int choice;

    do {
        cout << "\n---- Bank Management System ----\n";
        cout << "1. Open Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Display Account\n";
        cout << "5. Close Account\n";
        cout << "6. Display All Accounts\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bankSystem.openAccount();
                break;
            case 2:
                bankSystem.depositMoney();
                break;
            case 3:
                bankSystem.withdrawMoney();
                break;
            case 4:
                bankSystem.displayAccount();
                break;
            case 5:
                bankSystem.closeAccount();
                break;
            case 6:
                bankSystem.displayAllAccounts();
                break;
            case 7:
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
