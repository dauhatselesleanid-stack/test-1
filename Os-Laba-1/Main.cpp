#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>

#include <unistd.h>    
#include <sys/wait.h>  

#include "employee.h"

void waitForProcess(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);   
}

pid_t launchProcess(const char* path, std::vector<std::string>& argStrings) {
    std::vector<char*> argv;
    for (auto& s : argStrings)
        argv.push_back(const_cast<char*>(s.c_str()));
    argv.push_back(nullptr);   

    pid_t pid = fork();

    if (pid == 0) {
        execv(path, argv.data());
        std::cerr << "Ошибка запуска: " << path << "\n";
        _exit(1);  
    }

    if (pid < 0) {
        std::cerr << "Ошибка fork()\n";
    }

    return pid;
}

int main() {
    std::string binFile;
    int count;

    std::cout << "Введите имя бинарного файла: ";
    std::cin >> binFile;
    std::cout << "Введите количество записей:  ";
    std::cin >> count;

    std::string countStr = std::to_string(count);
    std::vector<std::string> creatorArgs = {"./creator", binFile, countStr};

    std::cout << "\n=== Запуск Creator ===\n";
    pid_t creatorPid = launchProcess("./creator", creatorArgs);
    if (creatorPid < 0) return 1;

    waitForProcess(creatorPid);
    std::cout << "=== Creator завершён ===\n\n";

    std::cout << "=== Содержимое файла '" << binFile << "' ===\n";
    std::ifstream bin(binFile, std::ios::binary);
    if (!bin.is_open()) {
        std::cerr << "Не удалось открыть бинарный файл\n";
        return 1;
    }

    std::cout << std::left
              << std::setw(8)  << "Номер"
              << std::setw(12) << "Имя"
              << "Часы\n"
              << std::string(28, '-') << "\n";

    employee emp;
    while (bin.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << std::left
                  << std::setw(8)  << emp.num
                  << std::setw(12) << emp.name
                  << emp.hours << "\n";
    }
    bin.close();

    std::string repFile;
    double rate;

    std::cout << "\nВведите имя файла отчёта: ";
    std::cin >> repFile;
    std::cout << "Введите оплату за час:    ";
    std::cin >> rate;

    std::string rateStr = std::to_string(rate);
    std::vector<std::string> reporterArgs = {"./reporter", binFile, repFile, rateStr};

    std::cout << "\n=== Запуск Reporter ===\n";
    pid_t reporterPid = launchProcess("./reporter", reporterArgs);
    if (reporterPid < 0) return 1;

    waitForProcess(reporterPid);
    std::cout << "=== Reporter завершён ===\n\n";

    std::cout << "=== Отчёт ===\n";
    std::ifstream rep(repFile);
    if (!rep.is_open()) {
        std::cerr << "Не удалось открыть файл отчёта\n";
        return 1;
    }

    std::string line;
    while (std::getline(rep, line)) {
        std::cout << line << "\n";
    }
    rep.close();

    std::cout << "\n=== Готово ===\n";
    return 0;
}
