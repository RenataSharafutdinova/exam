#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <chrono>
#include <random>
#include <windows.h>

// Расшифровка шифра Цезаря
std::string decrypt(const std::string& encrypted, int shift = 3) {
    std::string decrypted;
    for (char c : encrypted) {
        if (std::isalpha(c)) {
            char base = std::islower(c) ? 'a' : 'A';
            decrypted += static_cast<char>((c - base - shift + 26) % 26 + base);
        }
        else {
            decrypted += c;
        }
    }
    return decrypted;
}

// Загрузка слов из файла
std::vector<std::string> loadWordsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> words;
    std::string line;
    while (std::getline(file, line)) {
        words.push_back(decrypt(line));
    }
    return words;
}

// Рисуем человечка по числу ошибок
void drawHangman(int errors) {
    std::string stages[] = {
        "  _______\n"
        " |/      |\n"
        " |\n"
        " |\n"
        " |\n"
        " |\n"
        "_|___\n",

        "  _______\n"
        " |/      |\n"
        " |      (_)\n"
        " |\n"
        " |\n"
        " |\n"
        "_|___\n",

        "  _______\n"
        " |/      |\n"
        " |      (_)\n"
        " |       |\n"
        " |       |\n"
        " |\n"
        "_|___\n",

        "  _______\n"
        " |/      |\n"
        " |      (_)\n"
        " |      \\|\n"
        " |       |\n"
        " |\n"
        "_|___\n",

        "  _______\n"
        " |/      |\n"
        " |      (_)\n"
        " |      \\|/\n"
        " |       |\n"
        " |\n"
        "_|___\n",

        "  _______\n"
        " |/      |\n"
        " |      (_)\n"
        " |      \\|/\n"
        " |       |\n"
        " |      /\n"
        "_|___\n",

        "  _______\n"
        " |/      |\n"
        " |      (_)\n"
        " |      \\|/\n"
        " |       |\n"
        " |      / \\\n"
        "_|___\n"
    };

    std::cout << stages[errors] << "\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::vector<std::string> words = loadWordsFromFile("words.txt");
    if (words.empty()) {
        std::cerr << "Ошибка: слова не загружены.\n";
        return 1;
    }

    // Выбираем случайное слово
    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<> dist(0, words.size() - 1);
    std::string secret = words[dist(rng)];

    std::string guessed(secret.length(), '_');
    std::vector<char> guessedLetters;
    int attempts = 0;
    int maxFails = 6;
    int fails = 0;

    auto start = std::chrono::steady_clock::now();

    std::cout << "===== Игра: Виселица =====\n";

    while (guessed != secret && fails < maxFails) {
        drawHangman(fails);
        std::cout << "Слово: " << guessed << "\n";
        std::cout << "Осталось попыток: " << maxFails - fails << "\n";
        std::cout << "Введённые буквы: ";
        for (char c : guessedLetters) std::cout << c << ' ';
        std::cout << "\nВведите букву: ";

        char ch;
        std::cin >> ch;
        ch = std::tolower(ch);

        if (!std::isalpha(ch)) {
            std::cout << "Введите букву!\n";
            continue;
        }

        if (std::find(guessedLetters.begin(), guessedLetters.end(), ch) != guessedLetters.end()) {
            std::cout << "Эта буква уже вводилась.\n";
            continue;
        }

        guessedLetters.push_back(ch);
        attempts++;

        bool correct = false;
        for (size_t i = 0; i < secret.length(); ++i) {
            if (secret[i] == ch) {
                guessed[i] = ch;
                correct = true;
            }
        }

        if (!correct) {
            fails++;
            std::cout << "Нет такой буквы!\n";
        }
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    drawHangman(fails);

    std::cout << "\n===== Результат =====\n";
    if (guessed == secret) {
        std::cout << "Вы победили! Слово: " << secret << "\n";
    }
    else {
        std::cout << "Вы проиграли. Слово было: " << secret << "\n";
    }

    std::cout << "\nСтатистика:\n";
    std::cout << "Время игры: " << elapsed.count() << " секунд\n";
    std::cout << "Количество попыток: " << attempts << "\n";
    std::cout << "Ошибок: " << fails << "\n";
    std::cout << "Буквы игрока: ";
    for (char c : guessedLetters) std::cout << c << " ";
    std::cout << "\n";

    return 0;
}
