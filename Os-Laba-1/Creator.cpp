#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./creator <binary_file_name> <number_of_records>\n";
        return 1;
    }

    string fileName = argv[1];
    int recordCount = stoi(argv[2]);

    ofstream outFile(fileName, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create binary file.\n";
        return 1;
    }

    for (int i = 0; i < recordCount; ++i) {
        employee emp;
        cout << "Enter data for employee #" << i + 1 << ":\n";
        cout << "ID (num): ";
        cin >> emp.num;
        cout << "Name (max 9 chars): ";
        string tempName;
        cin >> tempName;
        strncpy(emp.name, tempName.c_str(), 9);
        emp.name[9] = '\0';
        cout << "Hours: ";
        cin >> emp.hours;

        outFile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    outFile.close();
    return 0;
}