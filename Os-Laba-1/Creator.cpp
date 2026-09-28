#include <iostream>
#include <fstream>
#include <iomanip>   
#include <cstdlib>   
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: ./creator <имя_файла> <кол-во_записей>\n";
        return 1;
    }

    const char* filename = argv[1];
    int count = std::atoi(argv[2]);

    if (count <= 0) {
        std::cerr << "Ошибка: количество записей должно быть > 0\n";
        return 1;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл '" << filename << "'\n";
        return 1;
    }

    std::cout << "Введите " << count << " запись(ей) сотрудников:\n";

    for (int i = 0; i < count; ++i) {
        employee emp{};   

        std::cout << "\nЗапись " << (i + 1) << ":\n";

        std::cout << "  Номер (ID): ";
        std::cin >> emp.num;

        std::cout << "  Имя (до 9 символов): ";
        std::cin >> std::setw(10) >> emp.name;  

        std::cout << "  Часы: ";
        std::cin >> emp.hours;

        file.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }

    file.close();
    std::cout << "\nФайл '" << filename << "' успешно создан ("
              << count << " запись(ей)).\n";
    return 0;
}
