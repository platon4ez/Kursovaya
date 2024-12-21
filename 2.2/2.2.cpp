#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <filesystem>

using namespace std;

// Алфавит для шифрования
const string alphabet_upper = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
const string encrypted_upper = "НОПРСТУФХЦЧШЩЪЫЬЭЮЯАБВГДЕЖЗИЙКЛМ";
const string alphabet_lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
const string encrypted_lower = "нопртсуфхцчшщъыьэюяабвгдеёжзийклм";

// Алфавит для цифр
const string digits = "0123456789";
const string encrypted_digits = "5678901234";

// Функция шифрования для Ш2.1.1
string encrypt_211(const string& text) {
    string result = text;
    for (char& c : result) {
        if (isdigit(c)) {
            // Цифры сдвигаются на 9
            c = ((c - '0' + 9) % 10) + '0';
        }
        else {
            // Проверяем для символов в верхнем регистре
            size_t pos = alphabet_upper.find(toupper(c));
            if (pos != string::npos) {
                c = encrypted_upper[pos];
            }
            else {
                // Проверяем для символов в нижнем регистре
                pos = alphabet_lower.find(c);
                if (pos != string::npos) {
                    c = encrypted_lower[pos];
                }
                else {
                    cerr << "Предупреждение: символ '" << c << "' не является допустимым (буква или цифра)." << endl;
                }
            }
        }
    }
    return result;
}

// Функция шифрования для Ш2.1.2
string encrypt_212(const string& text) {
    string result = text;
    for (char& c : result) {
        if (isdigit(c)) {
            // Цифры сдвигаются на 1 (для Ш2.1.2)
            c = ((c - '0' + 1) % 10) + '0';
        }
        else {
            // Проверяем для символов в верхнем регистре
            size_t pos = alphabet_upper.find(toupper(c));
            if (pos != string::npos) {
                c = encrypted_upper[alphabet_upper.size() - 1 - pos];
            }
            else {
                // Проверяем для символов в нижнем регистре
                pos = alphabet_lower.find(c);
                if (pos != string::npos) {
                    c = encrypted_lower[alphabet_lower.size() - 1 - pos];
                }
                else {
                    cerr << "Предупреждение: символ '" << c << "' не является допустимым (буква или цифра)." << endl;
                }
            }
        }
    }
    return result;
}

// Функция шифрования для Ш2.1.3
string encrypt_213(const string& text) {
    string result = text;
    for (char& c : result) {
        if (isdigit(c)) {
            // Цифры сдвигаются на 9 (как в 2.1.1)
            c = ((c - '0' + 9) % 10) + '0';
        }
        else {
            // Проверяем для символов в верхнем регистре
            size_t pos = alphabet_upper.find(toupper(c));
            if (pos != string::npos) {
                c = encrypted_upper[alphabet_upper.size() - 1 - pos];
            }
            else {
                // Проверяем для символов в нижнем регистре
                pos = alphabet_lower.find(c);
                if (pos != string::npos) {
                    c = encrypted_lower[alphabet_lower.size() - 1 - pos];
                }
                else {
                    cerr << "Предупреждение: символ '" << c << "' не является допустимым (буква или цифра)." << endl;
                }
            }
        }
    }
    return result;
}

// Функция для выбора шифра в зависимости от времени
string encrypt_dynamic(const string& text) {
    // Получаем текущее время
    auto now = chrono::system_clock::now();
    auto hours = chrono::time_point_cast<chrono::hours>(now);
    auto hour = chrono::duration_cast<chrono::hours>(now - hours).count();

    // В зависимости от текущего часа выбираем шифр
    if (hour % 2 == 0) {
        return encrypt_212(text); // Шифр 2.1.2 для четных часов
    }
    else {
        return encrypt_213(text); // Шифр 2.1.3 для нечетных часов
    }
}

// Функция для выбора шифра по месяцу
string encrypt_by_month(const string& text) {
    auto now = chrono::system_clock::now();
    time_t current_time = chrono::system_clock::to_time_t(now);
    struct tm* time_info = localtime(&current_time);

    // В зависимости от месяца выбираем шифр
    if ((time_info->tm_mon + 1) % 2 == 0) {
        return encrypt_213(text); // Шифр 2.1.3 для четных месяцев
    }
    else {
        return encrypt_211(text); // Шифр 2.1.1 для нечетных месяцев
    }
}

// Функция для выбора шифра в зависимости от дня недели
string encrypt_by_day_of_week(const string& text) {
    auto now = chrono::system_clock::now();
    time_t current_time = chrono::system_clock::to_time_t(now);
    struct tm* time_info = localtime(&current_time);

    // В зависимости от дня недели выбираем шифр
    if (time_info->tm_wday >= 1 && time_info->tm_wday <= 5) {
        return encrypt_211(text); // Шифр 2.1.1 для будних дней
    }
    else {
        return encrypt_212(text); // Шифр 2.1.2 для выходных
    }
}

int main() {
    // Заданный путь к файлу
    string input_file = "C:\\Users\\Платон\\Desktop\\Шифратор 2.2\\SourceText\\input.txt";
    string output_file;

    // Проверка на существование файла
    ifstream infile(input_file);
    if (!infile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << input_file << endl;
        return 1;
    }

    // Считываем данные из файла
    string text((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    infile.close();

    // Пример выбора шифра на основе времени
    string encrypted_text = encrypt_dynamic(text);
    cout << "Зашифрованный текст (в зависимости от часа): " << encrypted_text << endl;

    // Записываем зашифрованный текст в новый файл в той же директории
    stringstream output_file_ss;
    size_t last_slash = input_file.find_last_of("\\/");
    if (last_slash != string::npos) {
        output_file_ss << input_file.substr(0, last_slash) << "\\output_encrypted.txt";
    }
    output_file = output_file_ss.str();

    ofstream outfile(output_file);
    if (outfile.is_open()) {
        outfile << encrypted_text;
        outfile.close();
        cout << "Зашифрованный текст сохранен в файл: " << output_file << endl;
    }
    else {
        cerr << "Ошибка при записи в файл." << endl;
    }

    return 0;
}
