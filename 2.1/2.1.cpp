/**
 * @file encryptor.cpp
 * @brief Программа для шифрования текста с использованием трёх различных шифров.
 *
 * Программа реализует три шифра: Ш2.1.1, Ш2.1.2 и Ш2.1.3. Шифрование осуществляется без использования маппинга.
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * @brief Алфавиты для шифрования и соответствующие шифры.
 */
const wstring alphabet =
L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
L"0123456789";

const wstring cipher1 =
L"NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm"
L"РСТУФХЦЧШЩЪЫЬЭЮЯАБВГДЕЁЖЗИЙКЛМНОПрстуфхцчшщъыьэюяабвгдеёжзийклмноп"
L"5678901234";

const wstring cipher2 =
L"ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
L"ЯЮЭЬЫЪЩШЧЦХФУТСРПОНМЛКЙИЗЖЕДГВБАяюэьыъщшчцхфутсрпонмлкийзжедгвба"
L"9876543210";

const wstring cipher3 = cipher2; // Шифр 3 совпадает с шифром 2.

/**
 * @brief Функция для шифрования текста с использованием заданного шифра.
 * @param text Текст для шифрования.
 * @param cipher Алфавит шифра для преобразования.
 * @return Зашифрованный текст.
 */
string encrypt(const string& text, const wstring& cipher) {
    string encrypted_text;
    for (char c : text) {
        size_t pos = alphabet.find(c);
        if (pos != wstring::npos) {
            encrypted_text += cipher[pos];
        }
        else {
            encrypted_text += c; // Если символ не найден, оставляем его без изменений
        }
    }
    return encrypted_text;
}

/**
 * @brief Точка входа в программу.
 *
 * Программа считывает текст из входного файла, шифрует его с использованием трёх различных шифров
 * и записывает результаты в выходной файл.
 *
 * @return Код завершения программы (0 при успешном выполнении).
 */
int main() {
    // Указываем явно пути к файлам
    string input_file = "C:\\Users\\Платон\\Desktop\\Шифратор2.1\\SourceText\\input.txt"; // Путь к входному файлу
    string output_file = "C:\\Users\\Платон\\Desktop\\Шифратор2.1\\SourceText\\output.txt"; // Путь к выходному файлу

    // Открываем файл с открытым текстом
    ifstream infile(input_file);
    if (!infile) {
        cerr << "Ошибка при открытии входного файла!" << endl;
        return 1;
    }

    // Чтение всего содержимого файла
    string input_text((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    infile.close();

    // Шифруем текст с использованием всех трёх шифров
    string encrypted_text_1 = encrypt(input_text, cipher1); // Шифр 2.1.1
    string encrypted_text_2 = encrypt(input_text, cipher2); // Шифр 2.1.2
    string encrypted_text_3 = encrypt(input_text, cipher3); // Шифр 2.1.3

    // Открываем файл для записи зашифрованного текста
    ofstream outfile(output_file);
    if (!outfile) {
        cerr << "Ошибка при открытии выходного файла!" << endl;
        return 1;
    }

    // Записываем оригинальный текст
    outfile << "Оригинальный текст:" << endl;
    outfile << input_text << endl << endl;

    // Записываем зашифрованный текст для шифра 2.1.1
    outfile << "Зашифрованный текст (Ш2.1.1):" << endl;
    outfile << encrypted_text_1 << endl << endl;

    // Записываем зашифрованный текст для шифра 2.1.2
    outfile << "Зашифрованный текст (Ш2.1.2):" << endl;
    outfile << encrypted_text_2 << endl << endl;

    // Записываем зашифрованный текст для шифра 2.1.3
    outfile << "Зашифрованный текст (Ш2.1.3):" << endl;
    outfile << encrypted_text_3 << endl;

    outfile.close();

    cout << "Шифрование завершено. Зашифрованный текст записан в файл: " << output_file << endl;

    return 0;
}
