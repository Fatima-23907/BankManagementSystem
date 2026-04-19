#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <ctime>

using namespace std;


const string DATA_FILE = "bank_data.txt";
const string ADMIN_PASS = "Admin@123";
const double MIN_DEPOSIT = 500.0;
const int    MAX_ACCOUNTS = 200;


class Account {
private:
    int    accountNumber;
    string holderName;
    string accountType;
    double balance;
    string pin;

public:

    Account() : accountNumber(0), balance(0.0) {}


    Account(int no, const string& name, const string& type,
        double bal, const string& p)
        : accountNumber(no), holderName(name),
        accountType(type), balance(bal), pin(p) {
    }


    int    getAccountNumber() const { return accountNumber; }
    string getHolderName()    const { return holderName; }
    string getAccountType()   const { return accountType; }
    double getBalance()       const { return balance; }
    string getPin()           const { return pin; }


    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "\n  [ERROR] Invalid amount. Please enter a positive value.\n";
            return false;
        }
        balance += amount;
        cout << "\n  [SUCCESS] Rs. " << fixed << setprecision(2) << amount
            << " deposited successfully.\n";
        cout << "  [INFO]    New Balance: Rs. " << balance << "\n";
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "\n  [ERROR] Invalid amount.\n";
            return false;
        }
        if (amount > balance) {
            cout << "\n  [ERROR] Insufficient balance.\n";
            cout << "  [INFO]  Available Balance: Rs. "
                << fixed << setprecision(2) << balance << "\n";
            return false;
        }
        balance -= amount;
        cout << "\n  [SUCCESS] Rs. " << fixed << setprecision(2) << amount
            << " withdrawn successfully.\n";
        cout << "  [INFO]    Remaining Balance: Rs. " << balance << "\n";
        return true;
    }


    void display() const {
        cout << "\n";
        cout << "  +--------------------------------------+\n";
        cout << "  |         ACCOUNT DETAILS              |\n";
        cout << "  +--------------------------------------+\n";
        cout << "  | Account No  : " << setw(22) << left << accountNumber << " |\n";
        cout << "  | Holder Name : " << setw(22) << left << holderName << " |\n";
        cout << "  | Account Type: " << setw(22) << left << accountType << " |\n";
        cout << "  | Balance     : Rs. " << setw(18) << left
            << fixed << setprecision(2) << balance << " |\n";
        cout << "  +--------------------------------------+\n";
    }


    void writeToFile(ofstream& out) const {
        out << accountNumber << "\n"
            << holderName << "\n"
            << accountType << "\n"
            << balance << "\n"
            << pin << "\n";
    }


    bool readFromFile(ifstream& in) {
        if (!(in >> accountNumber)) return false;
        in.ignore();
        getline(in, holderName);
        getline(in, accountType);
        in >> balance;
        in.ignore();
        getline(in, pin);
        return true;
    }
};


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


void showBanner() {
    cout << "\n";
    cout << "  +=============================================+\n";
    cout << "  |       BANK MANAGEMENT SYSTEM  v1.0         |\n";
    cout << "  |       Built with C++ | OOP | File I/O      |\n";
    cout << "  +=============================================+\n\n";
}


int loadAccounts(Account accounts[], int maxSize) {
    ifstream in(DATA_FILE);
    int count = 0;
    while (count < maxSize && accounts[count].readFromFile(in)) {
        count++;
    }
    in.close();
    return count;
}


void saveAccounts(Account accounts[], int count) {
    ofstream out(DATA_FILE, ios::trunc);
    for (int i = 0; i < count; i++) {
        accounts[i].writeToFile(out);
    }
    out.close();
}


int generateAccountNumber() {
    Account temp[MAX_ACCOUNTS];
    int count = loadAccounts(temp, MAX_ACCOUNTS);
    int maxNum = 10000;
    for (int i = 0; i < count; i++) {
        if (temp[i].getAccountNumber() > maxNum)
            maxNum = temp[i].getAccountNumber();
    }
    return maxNum + 1;
}


int findAccount(Account accounts[], int count, int accNo) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].getAccountNumber() == accNo)
            return i;
    }
    return -1;
}


bool verifyPin(const Account& acc, const string& enteredPin) {
    return acc.getPin() == enteredPin;
}


void createAccount() {
    clearScreen();
    showBanner();
    cout << "  ---- CREATE NEW ACCOUNT ----\n\n";

    string name, pin, confirmPin, typeChoice;
    string accountType;
    double initialDeposit;

    cout << "  Enter Full Name       : ";
    cin.ignore();
    getline(cin, name);

    if (name.empty()) {
        cout << "\n  [ERROR] Name cannot be empty.\n";
        pause(); return;
    }


    cout << "  Account Type          : \n";
    cout << "    [1] Savings Account\n";
    cout << "    [2] Current Account\n";
    cout << "  Your Choice           : ";
    int typeNum; cin >> typeNum;
    accountType = (typeNum == 2) ? "Current" : "Savings";


    cout << "  Set 4-digit PIN       : ";
    cin >> pin;
    cout << "  Confirm PIN           : ";
    cin >> confirmPin;

    if (pin != confirmPin || pin.length() != 4) {
        cout << "\n  [ERROR] PINs do not match or must be exactly 4 digits.\n";
        pause(); return;
    }


    cout << "  Initial Deposit (min Rs. 500): Rs. ";
    cin >> initialDeposit;

    if (initialDeposit < MIN_DEPOSIT) {
        cout << "\n  [ERROR] Minimum initial deposit is Rs. 500.\n";
        pause(); return;
    }


    int accNo = generateAccountNumber();
    Account newAcc(accNo, name, accountType, initialDeposit, pin);

    ofstream out(DATA_FILE, ios::app);
    newAcc.writeToFile(out);
    out.close();


    cout << "\n  +----------------------------------------+\n";
    cout << "  |   ACCOUNT CREATED SUCCESSFULLY!        |\n";
    cout << "  |   Account Number : " << setw(20) << left << accNo << " |\n";
    cout << "  |   Account Type   : " << setw(20) << left << accountType << " |\n";
    cout << "  |   Please keep your account number safe.|\n";
    cout << "  +----------------------------------------+\n";
    pause();
}


void depositMoney() {
    clearScreen();
    showBanner();
    cout << "  ---- DEPOSIT MONEY ----\n\n";

    int accNo;
    string pin;
    double amount;

    cout << "  Enter Account Number : "; cin >> accNo;
    cout << "  Enter PIN            : "; cin >> pin;

    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts, MAX_ACCOUNTS);
    int idx = findAccount(accounts, count, accNo);

    if (idx == -1) {
        cout << "\n  [ERROR] Account not found.\n";
        pause(); return;
    }
    if (!verifyPin(accounts[idx], pin)) {
        cout << "\n  [ERROR] Incorrect PIN.\n";
        pause(); return;
    }

    cout << "  Enter Deposit Amount : Rs. "; cin >> amount;
    accounts[idx].deposit(amount);
    saveAccounts(accounts, count);
    pause();
}


void withdrawMoney() {
    clearScreen();
    showBanner();
    cout << "  ---- WITHDRAW MONEY ----\n\n";

    int accNo;
    string pin;
    double amount;

    cout << "  Enter Account Number : "; cin >> accNo;
    cout << "  Enter PIN            : "; cin >> pin;

    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts, MAX_ACCOUNTS);
    int idx = findAccount(accounts, count, accNo);

    if (idx == -1) {
        cout << "\n  [ERROR] Account not found.\n";
        pause(); return;
    }
    if (!verifyPin(accounts[idx], pin)) {
        cout << "\n  [ERROR] Incorrect PIN.\n";
        pause(); return;
    }

    cout << "  Enter Withdrawal Amount : Rs. "; cin >> amount;
    accounts[idx].withdraw(amount);
    saveAccounts(accounts, count);
    pause();
}


void checkBalance() {
    clearScreen();
    showBanner();
    cout << "  ---- CHECK BALANCE ----\n\n";

    int accNo;
    string pin;

    cout << "  Enter Account Number : "; cin >> accNo;
    cout << "  Enter PIN            : "; cin >> pin;

    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts, MAX_ACCOUNTS);
    int idx = findAccount(accounts, count, accNo);

    if (idx == -1) {
        cout << "\n  [ERROR] Account not found.\n";
        pause(); return;
    }
    if (!verifyPin(accounts[idx], pin)) {
        cout << "\n  [ERROR] Incorrect PIN.\n";
        pause(); return;
    }

    accounts[idx].display();
    pause();
}


void transferMoney() {
    clearScreen();
    showBanner();
    cout << "  ---- FUND TRANSFER ----\n\n";

    int fromAcc, toAcc;
    string pin;
    double amount;

    cout << "  Your Account Number       : "; cin >> fromAcc;
    cout << "  Your PIN                  : "; cin >> pin;
    cout << "  Recipient Account Number  : "; cin >> toAcc;
    cout << "  Amount to Transfer (Rs.)  : "; cin >> amount;

    if (fromAcc == toAcc) {
        cout << "\n  [ERROR] Cannot transfer to the same account.\n";
        pause(); return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts, MAX_ACCOUNTS);

    int fromIdx = findAccount(accounts, count, fromAcc);
    int toIdx = findAccount(accounts, count, toAcc);

    if (fromIdx == -1) { cout << "\n  [ERROR] Sender account not found.\n";   pause(); return; }
    if (toIdx == -1) { cout << "\n  [ERROR] Recipient account not found.\n"; pause(); return; }
    if (!verifyPin(accounts[fromIdx], pin)) { cout << "\n  [ERROR] Incorrect PIN.\n"; pause(); return; }

    // Perform transfer
    if (accounts[fromIdx].withdraw(amount)) {
        accounts[toIdx].deposit(amount);
        saveAccounts(accounts, count);
        cout << "\n  [SUCCESS] Rs. " << fixed << setprecision(2) << amount
            << " transferred to Account No. " << toAcc << " successfully.\n";
    }
    pause();
}

void updateAccount() {
    clearScreen();
    showBanner();
    cout << "  ---- UPDATE ACCOUNT ----\n\n";

    int accNo;
    string pin, newName;

    cout << "  Enter Account Number : "; cin >> accNo;
    cout << "  Enter PIN            : "; cin >> pin;

    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts, MAX_ACCOUNTS);
    int idx = findAccount(accounts, count, accNo);

    if (idx == -1) {
        cout << "\n  [ERROR] Account not found.\n";
        pause(); return;
    }
    if (!verifyPin(accounts[idx], pin)) {
        cout << "\n  [ERROR] Incorrect PIN.\n";
        pause(); return;
    }

    cout << "  Current Name         : " << accounts[idx].getHolderName() << "\n";
    cout << "  Enter New Name       : ";
    cin.ignore();
    getline(cin, newName);

    if (newName.empty()) {
        cout << "\n  [ERROR] Name cannot be empty.\n";
        pause(); return;
    }

    accounts[idx] = Account(
        accounts[idx].getAccountNumber(),
        newName,
        accounts[idx].getAccountType(),
        accounts[idx].getBalance(),
        accounts[idx].getPin()
    );

    saveAccounts(accounts, count);
    cout << "\n  [SUCCESS] Account name updated successfully.\n";
    pause();
}


void deleteAccount() {
    clearScreen();
    showBanner();
    cout << "  ---- DELETE ACCOUNT ----\n\n";

    int accNo;
    string pin;

    cout << "  Enter Account Number : "; cin >> accNo;
    cout << "  Enter PIN            : "; cin >> pin;

    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts, MAX_ACCOUNTS);
    int idx = findAccount(accounts, count, accNo);

    if (idx == -1 || !verifyPin(accounts[idx], pin)) {
        cout << "\n  [ERROR] Account not found or incorrect PIN.\n";
        pause(); return;
    }


    char confirm;
    cout << "\n  Are you sure you want to delete Account No. "
        << accNo << "? (y/n): ";
    cin >> confirm;

    if (confirm != 'y' && confirm != 'Y') {
        cout << "  [INFO] Deletion cancelled.\n";
        pause(); return;
    }

    for (int i = idx; i < count - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    count--;
    saveAccounts(accounts, count);

    cout << "\n  [SUCCESS] Account No. " << accNo << " deleted successfully.\n";
    pause();
}


void adminPanel() {
    clearScreen();
    showBanner();
    cout << "  ---- ADMIN PANEL ----\n\n";

    string pass;
    cout << "  Enter Admin Password : "; cin >> pass;

    if (pass != ADMIN_PASS) {
        cout << "\n  [ERROR] Incorrect admin password.\n";
        pause(); return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts, MAX_ACCOUNTS);

    if (count == 0) {
        cout << "\n  No accounts found in the system.\n";
        pause(); return;
    }

    double totalBalance = 0.0;

    cout << "\n  " << string(62, '-') << "\n";
    cout << "  " << left
        << setw(10) << "Acc No"
        << setw(25) << "Holder Name"
        << setw(10) << "Type"
        << setw(15) << "Balance (Rs.)" << "\n";
    cout << "  " << string(62, '-') << "\n";

    for (int i = 0; i < count; i++) {
        cout << "  " << left
            << setw(10) << accounts[i].getAccountNumber()
            << setw(25) << accounts[i].getHolderName()
            << setw(10) << accounts[i].getAccountType()
            << fixed << setprecision(2) << accounts[i].getBalance() << "\n";
        totalBalance += accounts[i].getBalance();
    }

    cout << "  " << string(62, '-') << "\n";
    cout << "  Total Accounts : " << count << "\n";
    cout << "  Total Deposits : Rs. " << fixed << setprecision(2) << totalBalance << "\n";
    cout << "  " << string(62, '-') << "\n";
    pause();
}

void showMenu() {
    cout << "  +--------------------------------------+\n";
    cout << "  |             MAIN MENU                |\n";
    cout << "  +--------------------------------------+\n";
    cout << "  |  1.  Create New Account              |\n";
    cout << "  |  2.  Deposit Money                   |\n";
    cout << "  |  3.  Withdraw Money                  |\n";
    cout << "  |  4.  Check Balance                   |\n";
    cout << "  |  5.  Transfer Money                  |\n";
    cout << "  |  6.  Update Account Name             |\n";
    cout << "  |  7.  Delete Account                  |\n";
    cout << "  |  8.  Admin Panel (All Accounts)      |\n";
    cout << "  |  0.  Exit                            |\n";
    cout << "  +--------------------------------------+\n";
    cout << "  Enter your choice: ";
}


int main() {
    int choice;

    do {
        clearScreen();
        showBanner();
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1: createAccount(); break;
        case 2: depositMoney();  break;
        case 3: withdrawMoney(); break;
        case 4: checkBalance();  break;
        case 5: transferMoney(); break;
        case 6: updateAccount(); break;
        case 7: deleteAccount(); break;
        case 8: adminPanel();    break;
        case 0:
            clearScreen();
            cout << "\n  Thank you for using Bank Management System. Goodbye!\n\n";
            break;
        default:
            cout << "\n  [ERROR] Invalid option. Please try again.\n";
            pause();
        }

    } while (choice != 0);

    return 0;
}
