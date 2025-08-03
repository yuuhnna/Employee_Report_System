#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Structure for storing employee details
struct EmployeeRecord {
    string name;
    int id;
    string position;
    int salary;
};

// Utility function to pause the console
void waitForEnter() {
    cout << "\n\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Utility to print section headers
void printBanner(const string& title) {
    cout << "========================================================================\n";
    cout << setw(47) << title << "\n";
    cout << "========================================================================\n";
}

// Function to safely get an integer
int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Function to get a full line of input (used for names and positions)
string getLineInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

// Write employee to file (append mode)
void saveEmployeeToFile(const EmployeeRecord& emp) {
    ofstream file("data.csv", ios::app);
    if (!file) {
        cerr << "Error: Unable to open file.\n";
        return;
    }
    file << emp.name << "," << emp.id << "," << emp.position << "," << emp.salary << "\n";
    file.close();
}

// Read all employees from file
vector<EmployeeRecord> loadAllEmployees() {
    vector<EmployeeRecord> employees;
    ifstream file("data.csv");
    if (!file) return employees;

    EmployeeRecord emp;
    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos)
            continue;

        emp.name = line.substr(0, pos1);
        emp.id = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        emp.position = line.substr(pos2 + 1, pos3 - pos2 - 1);
        emp.salary = stoi(line.substr(pos3 + 1));

        employees.push_back(emp);
    }

    return employees;
}

// Rewrite the whole file (used after edits or deletes)
void writeAllEmployees(const vector<EmployeeRecord>& employees) {
    ofstream file("data.csv");
    for (const auto& emp : employees) {
        file << emp.name << "," << emp.id << "," << emp.position << "," << emp.salary << "\n";
    }
    file.close();
}

// Add new employee
void addNewEmployee() {
    system("cls");
    printBanner("ADD NEW EMPLOYEE");

    EmployeeRecord emp;
    emp.name = getLineInput("Enter Full Name: ");
    emp.id = getValidatedInt("Enter Employee ID: ");
    emp.position = getLineInput("Enter Position: ");
    emp.salary = getValidatedInt("Enter Salary: ");

    char confirm;
    cout << "Confirm save? [Y/N]: ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'Y' || confirm == 'y') {
        saveEmployeeToFile(emp);
        cout << "\nEmployee added successfully.\n";
    } else {
        cout << "\nOperation cancelled.\n";
    }

    waitForEnter();
}

// Show employee by ID
void showEmployeeDetails() {
    system("cls");
    printBanner("VIEW EMPLOYEE DETAILS");

    int searchId = getValidatedInt("Enter Employee ID to search: ");
    vector<EmployeeRecord> employees = loadAllEmployees();
    bool found = false;

    for (const auto& emp : employees) {
        if (emp.id == searchId) {
            cout << "\nName    : " << emp.name
                 << "\nID      : " << emp.id
                 << "\nPosition: " << emp.position
                 << "\nSalary  : " << emp.salary << "\n";
            found = true;
            break;
        }
    }

    if (!found) cout << "\nEmployee not found.\n";
    waitForEnter();
}

// Edit employee by ID
void editEmployee() {
    system("cls");
    printBanner("EDIT EMPLOYEE DETAILS");

    int searchId = getValidatedInt("Enter Employee ID to edit: ");
    vector<EmployeeRecord> employees = loadAllEmployees();
    bool found = false;

    for (auto& emp : employees) {
        if (emp.id == searchId) {
            cout << "\nEditing record for: " << emp.name << "\n";
            emp.position = getLineInput("Enter new position: ");
            emp.salary = getValidatedInt("Enter new salary: ");
            found = true;
            break;
        }
    }

    if (found) {
        writeAllEmployees(employees);
        cout << "\nEmployee record updated.\n";
    } else {
        cout << "\nEmployee not found.\n";
    }

    waitForEnter();
}

// Delete employee
void deleteEmployee() {
    system("cls");
    printBanner("DELETE EMPLOYEE");

    int searchId = getValidatedInt("Enter Employee ID to delete: ");
    vector<EmployeeRecord> employees = loadAllEmployees();

    bool found = false;

    for (vector<EmployeeRecord>::iterator it = employees.begin(); it != employees.end(); ) {
        if (it->id == searchId) {
            it = employees.erase(it);
            found = true;
        } else {
            ++it;
        }
    }

    if (found) {
        writeAllEmployees(employees);
        cout << "\nEmployee deleted successfully.\n";
    } else {
        cout << "\nEmployee not found.\n";
    }

    waitForEnter();
}


// List all employees
void listAllEmployees() {
    system("cls");
    printBanner("LIST OF ALL EMPLOYEES");

    vector<EmployeeRecord> employees = loadAllEmployees();
    if (employees.empty()) {
        cout << "\nNo employee records found.\n";
    } else {
        cout << left << setw(20) << "NAME" 
             << setw(10) << "ID" 
             << setw(20) << "POSITION" 
             << setw(10) << "SALARY\n";
        cout << "---------------------------------------------------------------\n";
        for (const auto& emp : employees) {
            cout << left << setw(20) << emp.name 
                 << setw(10) << emp.id 
                 << setw(20) << emp.position 
                 << setw(10) << emp.salary << "\n";
        }
    }

    waitForEnter();
}

// Main menu
void showMenu() {
    while (true) {
        system("cls");
        printBanner("EMPLOYEE REPORT SYSTEM");

        cout << "[1] Add New Employee\n";
        cout << "[2] View Employee Details\n";
        cout << "[3] Edit Employee\n";
        cout << "[4] List All Employees\n";
        cout << "[5] Delete Employee\n";
        cout << "[0] Exit\n";
        cout << "Select an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: 
				addNewEmployee(); 
			break;
            case 2: 
				showEmployeeDetails(); 
			break;
            case 3: 
				editEmployee(); 
			break;
            case 4: 
				listAllEmployees(); 
			break;
            case 5: 
				deleteEmployee(); 
			break;
            case 0:
                system("cls");
                printBanner("Thank you for using the system!");
                return;
            default:
                cout << "\nInvalid choice. Try again.\n";
                waitForEnter();
        }
    }
}

int main() {
    showMenu();
    return 0;
}
