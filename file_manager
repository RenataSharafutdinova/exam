#include <iostream>         // Для ввода/вывода
#include <filesystem>       // Для работы с файловой системой
#include <fstream>          // Для создания файлов
#include <string>
#include <windows.h>

namespace fs = std::filesystem; // Сокращаем длинное имя std::filesystem до fs

// Абстрактный базовый класс, представляющий объект файловой системы (файл или папку)
class FileSystemObject {
protected:
    fs::path path; // Путь к объекту
public:
    FileSystemObject(const fs::path& p) : path(p) {} // Конструктор

    virtual void remove() = 0;                         // Удаление
    virtual void rename(const std::string& newName) = 0; // Переименование
    virtual uintmax_t size() const = 0;               // Размер
    virtual ~FileSystemObject() = default;            // Виртуальный деструктор
};

// Класс для работы с файлом
class File : public FileSystemObject {
public:
    File(const fs::path& p) : FileSystemObject(p) {}

    void remove() override {
        fs::remove(path); // Удалить файл
    }

    void rename(const std::string& newName) override {
        fs::rename(path, path.parent_path() / newName); // Переименовать файл
    }

    uintmax_t size() const override {
        return fs::file_size(path); // Получить размер файла
    }
};

// Класс для работы с директорией 
class Directory : public FileSystemObject {
public:
    Directory(const fs::path& p) : FileSystemObject(p) {}

    void remove() override {
        fs::remove_all(path); // Удалить папку и всё внутри
    }

    void rename(const std::string& newName) override {
        fs::rename(path, path.parent_path() / newName); // Переименовать папку
    }

    uintmax_t size() const override {
        uintmax_t totalSize = 0;
        // Считаем размер всех файлов в папке рекурсивно
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry.path())) {
                totalSize += fs::file_size(entry.path());
            }
        }
        return totalSize;
    }

    void listContents() const {
        // Выводим содержимое папки
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << entry.path() << std::endl;
        }
    }
};

// Основной класс-менеджер, предоставляющий интерфейс управления
class FileManager {
public:
    void createFile(const std::string& path) {
        std::ofstream(path).close(); // Создать пустой файл
    }

    void createDirectory(const std::string& path) {
        fs::create_directory(path); // Создать папку
    }

    void copy(const std::string& source, const std::string& destination) {
        fs::copy(source, destination, fs::copy_options::recursive); // Копировать (включая вложенные)
    }

    void move(const std::string& source, const std::string& destination) {
        fs::rename(source, destination); // Переместить (или переименовать)
    }

    void searchByMask(const std::string& rootPath, const std::string& mask) {
        // Поиск файлов по маске (например, ".txt")
        for (const auto& entry : fs::recursive_directory_iterator(rootPath)) {
            if (entry.path().filename().string().find(mask) != std::string::npos) {
                std::cout << entry.path() << std::endl;
            }
        }
    }

    void listDirectory(const std::string& path) {
        // Список содержимого папки
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << entry.path() << std::endl;
        }
    }
};


void showMenu() {
    std::cout << "\n===== ФАЙЛОВЫЙ МЕНЕДЖЕР =====\n";
    std::cout << "1. Показать содержимое папки\n";
    std::cout << "2. Создать файл\n";
    std::cout << "3. Создать папку\n";
    std::cout << "4. Удалить файл/папку\n";
    std::cout << "5. Переименовать файл/папку\n";
    std::cout << "6. Скопировать файл/папку\n";
    std::cout << "7. Переместить файл/папку\n";
    std::cout << "8. Размер файла/папки\n";
    std::cout << "9. Поиск по маске\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите действие: ";
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FileManager manager;
    int choice;
    std::string path, newPath, mask; // Пути и маска

    do {
        showMenu(); 
        std::cin >> choice;
        std::cin.ignore(); 

        switch (choice) {
        case 1:
            std::cout << "Введите путь к папке: ";
            std::getline(std::cin, path);
            manager.listDirectory(path); // Показать содержимое папки
            break;

        case 2:
            std::cout << "Введите путь для создания файла: ";
            std::getline(std::cin, path);
            manager.createFile(path); // Создать файл
            break;

        case 3:
            std::cout << "Введите путь для создания папки: ";
            std::getline(std::cin, path);
            manager.createDirectory(path); // Создать папку
            break;

        case 4:
            std::cout << "Введите путь для удаления: ";
            std::getline(std::cin, path);
            // Проверяем, папка это или файл
            if (fs::is_directory(path))
                Directory(path).remove(); // Удалить папку
            else
                File(path).remove();     // Удалить файл
            break;

        case 5:
            std::cout << "Введите путь: ";
            std::getline(std::cin, path);
            std::cout << "Введите новое имя: ";
            std::getline(std::cin, newPath);
            if (fs::is_directory(path))
                Directory(path).rename(newPath); // Переименовать папку
            else
                File(path).rename(newPath);      // Переименовать файл
            break;

        case 6:
            std::cout << "Введите путь источника: ";
            std::getline(std::cin, path);
            std::cout << "Введите путь назначения: ";
            std::getline(std::cin, newPath);
            manager.copy(path, newPath); // Скопировать файл или папку
            break;

        case 7:
            std::cout << "Введите путь источника: ";
            std::getline(std::cin, path);
            std::cout << "Введите путь назначения: ";
            std::getline(std::cin, newPath);
            manager.move(path, newPath); // Переместить файл или папку
            break;

        case 8:
            std::cout << "Введите путь: ";
            std::getline(std::cin, path);
            // Вывести размер файла или папки
            if (fs::is_directory(path))
                std::cout << "Размер папки: " << Directory(path).size() << " байт\n";
            else
                std::cout << "Размер файла: " << File(path).size() << " байт\n";
            break;

        case 9:
            std::cout << "Введите путь: ";
            std::getline(std::cin, path);
            std::cout << "Введите маску (например, .txt или part): ";
            std::getline(std::cin, mask);
            manager.searchByMask(path, mask); // Поиск по маске
            break;

        case 0:
            std::cout << "Выход...\n"; 
            break;

        default:
            std::cout << "Неверный выбор. Повторите.\n";
        }

    } while (choice != 0); 

    return 0; 
}
