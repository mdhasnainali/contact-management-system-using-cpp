#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <string.h>

using namespace std;

class contact
{

private:
    struct Date
    {
        int d, m, y;
    };
    char firstName[25], lastName[25], address[80], email[25], bloodGroup[4], phoneNumber[12];
    Date lastBloodDonation;
    const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

public:
    void takeRecordI()
    {
        cout << "\n\nCREATE A NEW RECORD";
        cout << "\n=====================\n";
        cout << "Enter your first name: ";
        cin >> firstName;
        cout << "Enter your last name: ";
        cin >> lastName;
        cout << "Enter your phone: ";
        cin >> phoneNumber;
        cout << "Enter your address: ";
        cin >> address;
        cout << "Enter your email: ";
        cin >> email;
        cout << "Enter your blood group: ";
        cin >> bloodGroup;
        cout << "Enter your last blood donation date: \n";
        cout << "Date: ";
        cin >> lastBloodDonation.d;
        cout << "Month: ";
        cin >> lastBloodDonation.m;
        cout << "Year: ";
        cin >> lastBloodDonation.y;
    }

    bool createRecord()
    {
        ofstream f1;
        f1.open("CMS.dat", ios::binary | ios::app);
        char ch;

        do
        {
            takeRecordI();
            f1.write(reinterpret_cast<char *>(this), sizeof(*this));
            cout << "Contact has been Successfully Created!";
            cout << "Do you have next data?(y/n)";
            cin >> ch;
        } while (ch == 'y');
        f1.close();
        return true;
    }

    void viewAllContact()
    {
        ifstream f2;
        f2.open("CMS.dat", ios::binary);
        int count = 0;

        cout << "\n================================\n";
        cout << "LIST OF CONTACTS";

        while (!f2.eof())
        {
            count++;
            if (f2.read(reinterpret_cast<char *>(this), sizeof(*this)))
            {
                cout << "\n=============== " << count << " ===============\n";
                showRecord();
            }
        }
        f2.close();

        cout << "\n\n";
    }

    void showRecord()
    {
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Address: " << address << endl;
        cout << "Email: " << email << endl;
        cout << "Blood group: " << bloodGroup << endl;
        cout << "Last blood donation: " << lastBloodDonation.d << "-" << lastBloodDonation.m << "-" << lastBloodDonation.y << endl;
    }

    void searchAContact()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        Date currentDate = {ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year};
        ifstream f3;
        char searchAddress[80];
        int count = 0;
        bool isFound = false;

        cout << "SEARCH A CONTACT";
        cout << "\n================================\n";
        cout << "Enter address: ";
        cin >> searchAddress;
        f3.open("CMS.dat", ios::binary);

        while (!f3.eof())
        {
            if (f3.read(reinterpret_cast<char *>(this), sizeof(*this)))
            {
                if (strcmp(searchAddress, address) == 0 && isGreaterThanNinetyDays(lastBloodDonation, currentDate))
                {
                    count++;
                    isFound = true;
                    cout << "\n=============== " << count << " ===============\n";
                    showRecord();
                    cout << "\n================================\n\n";
                }
            }
        }
        if (isFound == false)
        {
            cout << "\n\n================================\n";
            cout << "\n\n No record found";
            cout << "\n================================\n\n";
        }
        f3.close();

        cout << "\n\n";
    }

    void deleteAContact()
    {
        char num[12];
        int flag = 0;
        ofstream f4;
        ifstream f5;

        f5.open("CMS.dat", ios::binary);
        f4.open("temp.dat", ios::binary);

        cout << "DELETE A CONTACT";
        cout << "\n================================\n";
        cout << "Enter phone no. to delete: ";
        cin >> num;

        while (!f5.eof())
        {
            if (f5.read(reinterpret_cast<char *>(this), sizeof(*this)))
            {
                if (strcmp(num, phoneNumber))
                {
                    f4.write(reinterpret_cast<char *>(this), sizeof(*this));
                }
                else
                    flag = 1;
            }
        }
        f5.close();
        f4.close();
        remove("CMS.dat");
        rename("temp.dat", "CMS.dat");

        flag == 1 ? cout << endl
                         << endl
                         << "\tContact Deleted!"
                  : cout << endl
                         << endl
                         << "\tContact Not Found!";
    }

    void editContact()
    {
        char num[12];
        fstream f6;
        bool isDone = false;

        cout << "EDIT CONTACT";
        cout << "\n===============================\n\n";
        cout << "Enter the phone number to be edit: ";
        cin >> num;

        f6.open("CMS.dat", ios::binary | ios::out | ios::in);

        while (!f6.eof())
        {
            if (f6.read(reinterpret_cast<char *>(this), sizeof(*this)))
            {
                isDone = true;
                if (strcmp(num, phoneNumber) == 0)
                {
                    cout << "Enter new record\n";
                    takeRecordI();
                    long long pos = -1 * sizeof(*this);
                    f6.seekp(pos, ios::cur);
                    f6.write(reinterpret_cast<char *>(this), sizeof(*this));
                    cout << endl
                         << endl
                         << "\t Contact Successfully Updated!";
                }
            }
        }
        if (isDone == false)
        {
            cout << "\n\n No record not found";
        }
        f6.close();
    }

    int countLeapYears(Date d)
    {
        int years = d.y;

        if (d.m <= 2)
            years--;

        return years / 4 - years / 100 + years / 400;
    }

    bool isGreaterThanNinetyDays(Date date1, Date date2)
    {

        long int n1 = date1.y * 365 + date1.d;

        for (int i = 0; i < date1.m - 1; i++)
            n1 += monthDays[i];
        n1 += countLeapYears(date1);

        long int n2 = date2.y * 365 + date2.d;
        for (int i = 0; i < date2.m - 1; i++)
            n2 += monthDays[i];
        n2 += countLeapYears(date2);

        cout << n2 - n1 << endl;
        return (n2 - n1) >= 90;
    }
};

class view
{

public:
    int displayMenu()
    {
        int choice;

        cout << "\nCONTACT MANAGEMENT SYSTEM";
        cout << "\n\nMAIN MENU";
        cout << "\n=====================\n";
        cout << "[1] Add a new Contact\n";
        cout << "[2] List all Contacts\n";
        cout << "[3] Search for contact\n";
        cout << "[4] Delete a Contact\n";
        cout << "[5] Edit a Contact\n";
        cout << "[0] Exit";
        cout << "\n=====================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        return choice;
    }
};

class controller
{
    contact contactRecord;
    view viewController;
    int command;
    bool result = true;

public:
    bool handleMainMenu()
    {
        command = viewController.displayMenu();
        switch (command)
        {
        case 1:
            contactRecord.createRecord();
            break;
        case 2:
            contactRecord.viewAllContact();
            break;
        case 3:
            contactRecord.searchAContact();
            break;
        case 4:
            contactRecord.deleteAContact();
            break;
        case 5:
            contactRecord.editContact();
            break;
        case 0:
            system("cls");
            cout << "\n\n\n\t\t\tThank you for using CMS." << endl
                 << endl;
            return false;
        }

        return result;
    }
};

int main()
{
    controller controller;
    bool isRunning = true;
    system("cls");
    system("Color 06");

    cout << "\n\n\n\n\n\n\n\n\t\t\t * WELCOME TO CONTACT MANAGEMENT SYSTEM *";

    while (isRunning)
    {
        isRunning = controller.handleMainMenu();
    }
}
