#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

bool compareEmployees(const employee& a, const employee& b) {
    return a.num < b.num;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: ./reporter <binary_file> <report_file> <hourly_rate>\n";
        return 1;
    }

    string binFileName = argv[1];
    string reportFileName = argv[2];
    double hourlyRate = stod(argv[3]);

    ifstream inFile(binFileName, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open binary file.\n";
        return 1;
    }

    vector<employee> employees;
    employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    inFile.close();

    sort(employees.begin(), employees.end(), compareEmployees);

    ofstream reportFile(reportFileName);
    if (!reportFile.is_open()) {
        cerr << "Error: Cannot create report file.\n";
        return 1;
    }

    reportFile << "Отчет по файлу \"" << binFileName << "\"\n";
    reportFile << "Номер сотрудника, имя сотрудника, часы, зарплата.\n";

    reportFile << fixed << setprecision(2);
    for (const auto& e : employees) {
        double salary = e.hours * hourlyRate;
        reportFile << e.num << " " << e.name << " " << e.hours << " " << salary << "\n";
    }

    reportFile.close();
    return 0;
}