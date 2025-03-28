#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

class ContactBook {
public:
    string name;
    string number;

    ContactBook(string name = "", string number = "") { 
        this->name = name;
        this->number = number;
    }

    void create_contact() {
        ofstream write("ContactBook.txt", ios::app);
        if (write.is_open()) {
            write << "Name: " << name << endl;
            write << "Number: " << number << endl;
            write << "----------------------------------\n";
            write.close();
            cout << "Contact saved successfully.\n";
        } else {
            cout << "Error opening file.\n";
        }
    }

    void delete_contact(string delete_contact_name) {
        ifstream read("ContactBook.txt");
        vector<string> lines;
        string line;
        bool found = false;

        if (read.is_open()) {
            while (getline(read, line)) {
                if (line.find("Name: " + delete_contact_name) != string::npos) {
                    found = true;
                    getline(read, line); // Skip number line
                    getline(read, line); // Skip separator line
                } else {
                    lines.push_back(line);
                }
            }
            read.close();
        }

        if (!found) {
            cout << "Contact not found.\n";
            return;
        }

        ofstream write("ContactBook.txt");
        if (write.is_open()) {
            for (const auto& l : lines) {
                write << l << endl;
            }
            write.close();
            cout << "Contact deleted successfully.\n";
        } else {
            cout << "Error opening file for writing.\n";
        }
    }

    void edit_contact(string previous_name, string new_name, string new_number) {
        ifstream read("ContactBook.txt");
        vector<string> lines;
        string line;
        bool found = false;

        if (read.is_open()) {
            while (getline(read, line)) {
                if (line.find("Name: " + previous_name) != string::npos) {
                    found = true;
                    lines.push_back("Name: " + new_name);
                    getline(read, line); // Skip old number
                    lines.push_back("Number: " + new_number);
                    getline(read, line); // Skip separator
                    lines.push_back("----------------------------------");
                } else {
                    lines.push_back(line);
                }
            }
            read.close();
        }

        if (!found) {
            cout << "Contact not found.\n";
            return;
        }

        ofstream write("ContactBook.txt");
        if (write.is_open()) {
            for (const auto& l : lines) {
                write << l << endl;
            }
            write.close();
            cout << "Contact updated successfully.\n";
        } else {
            cout << "Error updating contact.\n";
        }
    }

    void search_contact(string search_contact_name) {
        ifstream read("ContactBook.txt");
        string line;
        bool found = false;

        if (read.is_open()) {
            while (getline(read, line)) {
                if (line.find("Name: " + search_contact_name) != string::npos) {
                    found = true;
                    cout << line << endl; // Print name
                    if (getline(read, line)) cout << line << endl; // Print number
                    getline(read, line); // Skip separator
                }
            }
            read.close();
        }

        if (!found) {
            cout << "No contact found.\n";
        }
    }
};

int main() {
    string command;
    string name;
    string number;
    
    cout << "Enter a command ('create_contact', 'edit', 'search', 'delete'): ";
    cin >> command;
    transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (command == "create_contact") {
        cout << "Enter the name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter the number: ";
        cin >> number;

        if (number.length() == 11 && number.find_first_not_of("0123456789") == string::npos && number[0] == '0') {
            ContactBook contact(name, number);
            contact.create_contact();
        } else {
            cout << "Invalid phone number format.\n";
        }

    } else if (command == "edit") {
        string new_name, new_number;
        cout << "Enter the existing contact name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter the new name: ";
        getline(cin, new_name);
        cout << "Enter the new number: ";
        cin >> new_number;

        ContactBook contact;
        contact.edit_contact(name, new_name, new_number);

    } else if (command == "search") {
        cout << "Enter the name to search: ";
        cin.ignore();
        getline(cin, name);
        
        ContactBook contact;
        contact.search_contact(name);

    } else if (command == "delete") {
        cout << "Enter the name to delete: ";
        cin.ignore();
        getline(cin, name);

        ContactBook contact;
        contact.delete_contact(name);
    } else {
        cout << "Invalid command.\n";
    }

    return 0;
}
