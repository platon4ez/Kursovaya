/**
 * @file SimpleSubstitutionCipher.cpp
 * @brief Реализация класса для шифрования и дешифрования текста с использованием шифра подстановки.
 *
 * В этом файле реализован класс SimpleSubstitutionCipher, который позволяет шифровать и дешифровать
 * текст с использованием различных вариантов шифра подстановки. Шифры выбираются при инициализации объекта
 * и могут быть изменены в зависимости от типа шифра.
 */

#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>
#include <fstream>  // Для работы с файлами
#include <locale>   // Для установки локали
#include <codecvt>  // Для преобразования между кодировками
using namespace std;

/**
 * @class SimpleSubstitutionCipher
 * @brief Класс для выполнения шифрования и дешифрования текста с использованием шифра подстановки.
 *
 * Этот класс реализует шифр подстановки, который преобразует каждый символ в тексте в другой символ,
 * согласно заранее заданной карте подстановки. Также доступны функции для вычисления контрольной суммы
 * и обработки файлов.
 */
class SimpleSubstitutionCipher {
private:
    unordered_map<wchar_t, wchar_t> substitutionMap; /**< Карта подстановки для шифрования */
    unordered_map<wchar_t, wchar_t> reverseSubstitutionMap; /**< Карта для обратной подстановки (для дешифрования) */

    /**
     * @brief Вычисление контрольной суммы текста.
     *
     * Контрольная сумма вычисляется как сумма значений всех символов в строке. Она используется для проверки
     * целостности данных после шифрования и дешифрования.
     *
     * @param text Текст для вычисления контрольной суммы.
     * @return Контрольная сумма текста, которая является суммой кодов всех символов в строке.
     */
    unsigned int calculateChecksum(const wstring& text) {
        unsigned int checksum = 0;
        for (wchar_t c : text) {
            checksum += static_cast<unsigned int>(c); // Преобразование символа в число и добавление к контрольной сумме
        }
        return checksum;
    }

public:
    /**
     * @brief Конструктор класса для инициализации шифра.
     *
     * В зависимости от переданного параметра выбирается один из типов шифра:
     * - cipher1: шифр с использованием простых сдвигов (A->N, B->O и т.д.)
     * - cipher2: шифр с использованием обратного порядка букв (A->Z, B->Y и т.д.)
     * - cipher3: шифр с использованием более сложной перестановки букв.
     *
     * @param cipherType Тип шифра: 1, 2 или 3. Если передан неверный тип, будет выведена ошибка.
     */
    SimpleSubstitutionCipher(int cipherType) {
        wstring alphabet =
            L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя"
            L"0123456789";

        wstring cipher1 =
            L"NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm"
            L"РСТУФХЦЧШЩЪЫЬЭЮЯАБВГДЕЖЗИЙКЛМНОПрстуфхцчшщъыьэюяабвгдежзийклмноп"
            L"5678901234";

        wstring cipher2 =
            L"ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
            L"ЯЮЭЬЫЪЩШЧЦХФУТСРПОНМЛКИЙЗЖЕДГВБАяюэьыъщшчцхфутсрпонмлкийзжедгвба"
            L"9876543210";

        wstring cipher3 =
            L"ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
            L"ЯЮЭЬЫЪЩШЧЦХФУТСРПОНМЛКЙИЗЖЕДГВБАяюэьыъщшчцхфутсрпонмлкйизжедгвба"
            L"9876543210";

        wstring cipher;

        // Определение шифра в зависимости от типа
        if (cipherType == 1) {
            cipher = cipher1;
        }
        else if (cipherType == 2) {
            cipher = cipher2;
        }
        else if (cipherType == 3) {
            cipher = cipher3;
        }
        else {
            wcerr << L"Invalid cipher type" << endl;
            return;
        }

        // Инициализация карт подстановки
        for (size_t i = 0; i < alphabet.size(); ++i) {
            substitutionMap[alphabet[i]] = cipher[i]; /**< Заполнение карты подстановки для шифрования */
            reverseSubstitutionMap[cipher[i]] = alphabet[i]; /**< Заполнение карты подстановки для дешифрования */
        }
    }

    /**
     * @brief Шифрование текста с использованием подстановочного шифра.
     *
     * Каждый символ в тексте заменяется на символ из карты подстановки. Если символ не найден в карте,
     * он остаётся неизменным.
     *
     * @param plaintext Исходный текст для шифрования.
     * @return Зашифрованный текст, его длина и контрольная сумма.
     */
    wstring encrypt(const wstring& plaintext) {
        wstring ciphertext;
        for (wchar_t c : plaintext) {
            if (substitutionMap.find(c) != substitutionMap.end()) {
                ciphertext += substitutionMap[c]; /**< Замена символа с использованием карты подстановки */
            }
            else {
                ciphertext += c; /**< Если символ не найден, добавляем его без изменений */
            }
        }
        unsigned int checksum = calculateChecksum(ciphertext); /**< Вычисление контрольной суммы зашифрованного текста */
        return to_wstring(ciphertext.length()) + L"|" + to_wstring(checksum) + L"|" + ciphertext;
    }

    /**
     * @brief Дешифрование текста с использованием обратной подстановки.
     *
     * Дешифрование происходит путём замены каждого символа на его соответствующий в обратной карте подстановки.
     * Если символ не найден, он остаётся неизменным.
     *
     * @param encryptedText Зашифрованный текст.
     * @return Дешифрованный текст.
     * @throws invalid_argument Если формат зашифрованного текста некорректен (отсутствуют разделители или неверная длина).
     */
    wstring decrypt(const wstring& encryptedText) {
        size_t firstPipe = encryptedText.find(L'|'); /**< Поиск первого разделителя (|) */
        size_t secondPipe = encryptedText.find(L'|', firstPipe + 1); /**< Поиск второго разделителя (|) */

        // Проверка на корректность формата зашифрованного текста
        if (firstPipe == wstring::npos || secondPipe == wstring::npos) {
            throw invalid_argument("Invalid encrypted format");
        }

        int length = stoi(encryptedText.substr(0, firstPipe)); /**< Извлечение длины исходного текста */
        unsigned int checksum = stoi(encryptedText.substr(firstPipe + 1, secondPipe - firstPipe - 1)); /**< Извлечение контрольной суммы */
        wstring ciphertext = encryptedText.substr(secondPipe + 1); /**< Извлечение зашифрованного текста */

        // Проверка на совпадение длины
        if (ciphertext.length() != length) {
            throw invalid_argument("Decrypted text length mismatch");
        }

        // Проверка на совпадение контрольной суммы
        unsigned int calculatedChecksum = calculateChecksum(ciphertext);
        if (calculatedChecksum != checksum) {
            throw invalid_argument("Checksum mismatch");
        }

        // Дешифрование текста
        wstring plaintext;
        for (wchar_t c : ciphertext) {
            if (reverseSubstitutionMap.find(c) != reverseSubstitutionMap.end()) {
                plaintext += reverseSubstitutionMap[c]; /**< Замена символа на основе обратной карты подстановки */
            }
            else {
                plaintext += c; /**< Если символ не найден, добавляем его без изменений */
            }
        }
        return plaintext;
    }
};

/**
 * @brief Основная функция программы.
 *
 * Программа выполняет следующие действия:
 * - Чтение исходного текста из файла.
 * - Шифрование текста с использованием шифра подстановки.
 * - Дешифрование зашифрованного текста и проверка корректности.
 * - Запись зашифрованного текста в выходной файл.
 *
 * @return Код возврата программы.
 */
int main() {
    // Устанавливаем локаль для работы с русским текстом
    wcin.imbue(locale("ru_RU.UTF-8"));
    wcout.imbue(locale("ru_RU.UTF-8"));

    wstring inputFile = L"C:\\Users\\Платон\\Desktop\\Шифратор2.7\\SourceText\\input.txt";
    wstring outputFile = L"C:\\Users\\Платон\\Desktop\\Шифратор2.7\\SourceText\\output.txt";

    // Чтение текста из файла
    wifstream infile(inputFile);
    if (!infile) {
        wcerr << L"Ошибка при открытии входного файла!" << endl;
        return 1;
    }
    infile.imbue(locale("ru_RU.UTF-8"));
    wstring inputText((istreambuf_iterator<wchar_t>(infile)), istreambuf_iterator<wchar_t>());
    infile.close();

    SimpleSubstitutionCipher cipher(1);  // 1 для cipher1, 2 для cipher2, 3 для cipher3
    wcout << L"Original text: " << inputText << endl;

    wstring encryptedText = cipher.encrypt(inputText); /**< Шифрование текста */
    wcout << L"Encrypted text: " << encryptedText << endl;

    wstring decryptedText = cipher.decrypt(encryptedText); /**< Дешифрование текста */
    wcout << L"Decrypted text: " << decryptedText << endl;

    // Проверка на совпадение исходного и дешифрованного текста
    assert(inputText == decryptedText);
    wcout << L"All tests passed!" << endl;

    // Запись зашифрованного текста в выходной файл
    wofstream outfile(outputFile);
    if (!outfile) {
        wcerr << L"Ошибка при открытии выходного файла!" << endl;
        return 1;
    }
    outfile.imbue(locale("ru_RU.UTF-8"));
    outfile << encryptedText;
    outfile.close();

    return 0;
}
