#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <cstdlib>

using namespace std;

void input();
void writefile();
void search();
void output();

struct date {
    int month;
    int day;
    int year;
};

struct account {
    int number;
    char name[100];
    int acct_no;
    float mobile_no;
    char street[100];
    char city[100];
    char acct_type;
    float oldbalance;
    float newbalance;
    float payment;
    struct date lastpayment;
} customer;

int tl, sl, ts;

void mainMenu();

int main() {
    mainMenu();
    return 0;
}

void mainMenu() {
    int i, n;
    char ch;
    cout << "   CUSTOMER BILLING SYSTEM:\n\n";
    cout << "===============================\n";
    cout << "\n1:    to add account on list\n";
    cout << "2:    to search customer account\n";
    cout << "3:    exit\n";
    cout << "\n================================\n";
    do {
        cout << "\nSelect what you want to do: ";
        ch = getche();
    } while (ch <= '0' || ch > '3');
    switch (ch) {
        case '1':
            cout << "\nHow many customer accounts? ";
            cin >> n;
            for (i = 0; i < n; i++) {
                input();
                if (customer.payment > 0)
                    customer.acct_type = (customer.payment < 0.1 * customer.oldbalance) ? 'O' : 'D';
                else
                    customer.acct_type = (customer.oldbalance > 0) ? 'D' : 'C';
                customer.newbalance = customer.oldbalance - customer.payment;
                writefile();
            }
            mainMenu();
            break;
        case '2':
            cout << "Search by:\n";
            cout << "\n1 --- Search by customer number\n";
            cout << "2 --- Search by customer name\n";
            search();
            mainMenu();
            break;
        case '3':
            cout << "\nA PROJECT BY BIDUR & SUJAN\n";
            exit(0);
    }
}

void input() {
    fstream file("bidur.dat", ios::in | ios::binary);
    if (file.is_open()) {
        file.seekg(0, ios::end);
        tl = file.tellg();
        sl = sizeof(customer);
        ts = tl / sl;
        file.seekg((ts - 1) * sl, ios::beg);
        file.read(reinterpret_cast<char*>(&customer), sizeof(customer));
        cout << "\nCustomer no: " << ++customer.number << endl;
        file.close();
    } else {
        customer.number = 0;
    }
    cout << "         Account number: ";
    cin >> customer.acct_no;
    cout << "\n       Name: ";
    cin >> customer.name;
    cout << "\n       Mobile no: ";
    cin >> customer.mobile_no;
    cout << "         Street: ";
    cin >> customer.street;
    cout << "         City: ";
    cin >> customer.city;
    cout << "         Previous balance: ";
    cin >> customer.oldbalance;
    cout << "         Current payment: ";
    cin >> customer.payment;
    cout << "         Payment date (mm/dd/yyyy): ";
    scanf("%d/%d/%d", &customer.lastpayment.month, &customer.lastpayment.day, &customer.lastpayment.year);
}

void writefile() {
    fstream file("bidur.dat", ios::out | ios::app | ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&customer), sizeof(customer));
        file.close();
    }
}

void search() {
    char ch;
    char nam[100];
    int n, i;
    bool found;

    fstream file("bidur.dat", ios::in | ios::binary);
    if (!file.is_open()) {
        cout << "\nError: Unable to open file!";
        return;
    }

    do {
        cout << "\nEnter your choice: ";
        ch = getche();
    } while (ch != '1' && ch != '2');

    switch (ch) {
        case '1': { // Search by customer number
            file.seekg(0, ios::end);
            tl = file.tellg();
            sl = sizeof(customer);
            ts = tl / sl;

            do {
                cout << "\nChoose customer number: ";
                cin >> n;

                if (n <= 0 || n > ts) {
                    cout << "\nInvalid customer number! Try again.";
                } else {
                    file.seekg((n - 1) * sl, ios::beg);
                    file.read(reinterpret_cast<char*>(&customer), sizeof(customer));
                    output();
                }

                cout << "\n\nSearch another? (y/n): ";
                ch = getche();
            } while (ch == 'y');
            break;
        }

        case '2': { // Search by customer name
            do {
                cout << "\nEnter the name to search: ";
                cin >> nam;

                found = false;
                file.seekg(0, ios::beg); // Reset file pointer to the beginning

                while (file.read(reinterpret_cast<char*>(&customer), sizeof(customer))) {
                    if (strcmp(customer.name, nam) == 0) {
                        output();
                        found = true;
                    }
                }

                if (!found) {
                    cout << "\nCustomer with the name '" << nam << "' not found!";
                }

                cout << "\n\nSearch another? (y/n): ";
                ch = getche();
            } while (ch == 'y');
            break;
        }

        default:
            cout << "\nInvalid choice!";
            break;
    }

    file.close();
}

void output() {
    cout << "\n\n    Customer no    : " << customer.number << endl;
    cout << "    Name           : " << customer.name << endl;
    cout << "    Mobile no      : " << customer.mobile_no << endl;
    cout << "    Account number : " << customer.acct_no << endl;
    cout << "    Street         : " << customer.street << endl;
    cout << "    City           : " << customer.city << endl;
    cout << "    Old balance    : " << customer.oldbalance << endl;
    cout << "    Current payment: " << customer.payment << endl;
    cout << "    New balance    : " << customer.newbalance << endl;
    cout << "    Payment date   : " << customer.lastpayment.month << "/"
         << customer.lastpayment.day << "/" << customer.lastpayment.year << endl;
    cout << "    Account status : ";
    switch (customer.acct_type) {
        case 'C':
            cout << "CURRENT\n\n";
            break;
        case 'O':
            cout << "OVERDUE\n\n";
            break;
        case 'D':
            cout << "DELINQUENT\n\n";
            break;
        default:
            cout << "ERROR\n\n";
    }
}
