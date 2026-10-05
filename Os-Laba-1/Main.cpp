#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

bool LaunchAndWait(const  string& prog, const vector<string>& arguments) {
    pid_t pid = fork();

    if (pid < 0) {
        return false;
    } 
    else if (pid == 0) {
        vector<char*> argv;
        argv.push_back(const_cast<char*>(prog.c_str()));
        for (const auto& arg : arguments) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        cerr << "Error: Failed to execute " << prog << "\n";
        exit(1);
    } 
    else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
    }
}

void PrintBinaryFile(const string& fileName) {
    ifstream inFile(fileName, ios::binary);
    if (!inFile.is_open()) {
        cout << "Error: Could not open binary file for reading.\n";
        return;
    }

    cout << "\n--- Содержимое бинарного файла " << fileName << " ---\n";
    employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        cout << "ID: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << "\n";
    }
    cout << "---------------------------------------\n\n";
    inFile.close();
}

void PrintTextFile(const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        cout << "Error: Could not open report file for reading.\n";
        return;
    }

    cout << "\n--- Содержимое файла отчета ---\n";
    string line;
    while (getline(inFile, line)) {
        cout << line << "\n";
    }
    cout << "-------------------------------\n";
    inFile.close();
}

int main() {
    string binFileName;
    int recordCount;

    cout << "Введите имя бинарного файла: ";
    cin >> binFileName;
    cout << "Введите количество записей: ";
    cin >> recordCount;

    cout << "Запуск процесса creator...\n";
    vector<string> creatorArgs = { binFileName, to_string(recordCount) };
    if (!LaunchAndWait("./creator", creatorArgs)) {
        cerr << "Ошибка при работе утилиты creator\n";
        return 1;
    }
    cout << "Процесс creator успешно завершил работу.\n";

    PrintBinaryFile(binFileName);

    string reportFileName;
    double hourlyRate;
    cout << "Введите имя файла отчета: ";
    cin >> reportFileName;
    cout << "Введите оплату за час работы: ";
    cin >> hourlyRate;

    cout << "Запуск процесса reporter...\n";
    vector<string> reporterArgs = { binFileName, reportFileName, to_string(hourlyRate) };
    if (!LaunchAndWait("./reporter", reporterArgs)) {
        cerr << "Ошибка при работе утилиты reporter\n";
        return 1;
    }
    cout << "Процесс reporter успешно завершил работу.\n";

    PrintTextFile(reportFileName);

    return 0;
}