#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  
#include <iomanip>    
#include <cstdlib>    
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Использование: ./reporter <бин_файл> <файл_отчёта> <оплата_за_час>\n";
        return 1;
    }

    const char* binFile = argv[1];
    const char* repFile = argv[2];
    double rate         = std::atof(argv[3]);  

    std::ifstream bin(binFile, std::ios::binary);
    if (!bin.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл '" << binFile << "'\n";
        return 1;
    }

    std::vector<employee> employees;
    employee emp;
    while (bin.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    bin.close();

    std::sort(employees.begin(), employees.end(),
              [](const employee& a, const employee& b) {
                  return a.num < b.num;
              });

    std::ofstream rep(repFile);
    if (!rep.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл отчёта '" << repFile << "'\n";
        return 1;
    }

    const int W1 = 10, W2 = 14, W3 = 10, W4 = 12;
    const std::string sep(W1 + W2 + W3 + W4, '-');

    rep << "Отчет по файлу \"" << binFile << "\"\n";
    rep << sep << "\n";

    rep << std::left
        << std::setw(W1 + 5)  << "Номер"
        << std::setw(W2 + 3)  << "Имя"
        << std::setw(W3 + 4)  << "Часы"
        << "Зарплата" << "\n";
    rep << sep << "\n";

    for (const auto& e : employees) {
        double salary = e.hours * rate;
        rep << std::left
            << std::setw(W1) << e.num
            << std::setw(W2) << e.name
            << std::setw(W3) << std::fixed << std::setprecision(1) << e.hours
            << std::fixed << std::setprecision(2) << salary << "\n";
    }

    rep.close();
    std::cout << "Файл отчёта '" << repFile << "' успешно создан.\n";
    return 0;
}
