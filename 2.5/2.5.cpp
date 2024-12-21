#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <locale>
#include <io.h>
#include <fcntl.h>
#include <ctime> // Для получения текущего времени
#include <windows.h> // Для поддержки кодировок на Windows

unsigned char calculateChecksum(const std::wstring& text) {
    unsigned char checksum = 0;
    for (wchar_t ch : text) {
        // Исправленный расчет контрольной суммы, учитывающий старший и младший байт.
        checksum += static_cast<unsigned char>(ch) + (static_cast<unsigned char>(ch >> 8) & 0xFF);
    }
    return checksum;
}

std::map<wchar_t, wchar_t> createSubstitutionTable(const std::wstring& alphabet, const std::wstring& cipher) {
    std::map<wchar_t, wchar_t> table;
    for (size_t i = 0; i < alphabet.size(); ++i) {
        table[alphabet[i]] = cipher[i];
    }
    return table;
}

std::wstring selectCipher(const std::wstring& alphabet, const std::wstring& cipher1, const std::wstring& cipher2, const std::wstring& cipher3) {
    std::time_t t = std::time(nullptr);
    std::tm now = {};
    errno_t err = localtime_s(&now, &t);
    if (err != 0) {
        throw std::runtime_error("Ошибка при получении текущего времени.");
    }

    if (now.tm_wday == 0 || now.tm_wday == 6) { // Выходные дни
        return cipher2;
    }
    else if (now.tm_mon % 2 == 0) { // Чётный месяц
        return cipher3;
    }
    else { // Нечётный месяц
        return cipher1;
    }
}

void encryptWithSimpleSubstitution(const std::wstring& inputFile, const std::wstring& outputFile) {
    try {
        std::wifstream infile(inputFile);
        infile.imbue(std::locale("ru_RU.UTF-8"));
        if (!infile.is_open()) {
            throw std::ios_base::failure("Не удалось открыть входной файл.");
        }

        std::wstring plaintext;
        std::wstring line;
        while (std::getline(infile, line)) {
            plaintext += line + L"\n";
        }
        infile.close();

        std::wstring alphabet =
            L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя"
            L"0123456789";

        std::wstring cipher1 =
            L"NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm"
            L"РСТУФХЦЧШЩЪЫЬЭЮЯАБВГДЕЖЗИЙКЛМНОПрстуфхцчшщъыьэюяабвгдежзийклмноп"
            L"5678901234";

        std::wstring cipher2 =
            L"ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
            L"ЯЮЭЬЫЪЩШЧЦХФУТСРПОНМЛКИЙЗЖЕДГВБАяюэьыъщшчцхфутсрпонмлкийзжедгвба"
            L"9876543210";

        std::wstring cipher3 =
            L"ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
            L"ЯЮЭЬЫЪЩШЧЦХФУТСРПОНМЛКЙИЗЖЕДГВБАяюэьыъщшчцхфутсрпонмлкйизжедгвба"
            L"9876543210";

        if (alphabet.size() != cipher1.size() || alphabet.size() != cipher2.size() || alphabet.size() != cipher3.size()) {
            std::wcout << L"Длина alphabet: " << alphabet.size() << std::endl;
            std::wcout << L"Длина cipher1: " << cipher1.size() << std::endl;
            std::wcout << L"Длина cipher2: " << cipher2.size() << std::endl;
            std::wcout << L"Длина cipher3: " << cipher3.size() << std::endl;
            throw std::logic_error("Длины алфавитов и шифров не совпадают.");
        }

        std::wstring selectedCipher = selectCipher(alphabet, cipher1, cipher2, cipher3);
        auto table = createSubstitutionTable(alphabet, selectedCipher);

        std::wstring encrypted;
        for (wchar_t ch : plaintext) {
            encrypted += (table.count(ch) ? table[ch] : ch);
        }

        unsigned char checksum = calculateChecksum(plaintext);
        std::wstring marker = L"MARKANT";
        size_t textLength = plaintext.length();

        std::wofstream outfile(outputFile);
        outfile.imbue(std::locale("ru_RU.UTF-8"));
        if (!outfile.is_open()) {
            throw std::ios_base::failure("Не удалось создать выходной файл.");
        }

        outfile << marker << L"\n";
        outfile << L"Длина текста: " << textLength << L"\n";
        outfile << L"Зашифрованный текст: " << encrypted << L"\n";
        outfile << L"Контрольная сумма: " << static_cast<int>(checksum) << std::endl;
        outfile.close();

        std::wcout << L"Файл успешно зашифрован. Результаты сохранены в " << outputFile << std::endl;
    }
    catch (const std::exception& e) {
        std::wcerr << L"Произошла ошибка: " << e.what() << std::endl;
    }
}

int wmain() {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);
#endif

    std::wstring inputFile = L"C:\\Users\\Платон\\Desktop\\Шифратор2.5\\SourceText\\input.txt";
    std::wstring outputFile = L"C:\\Users\\Платон\\Desktop\\Шифратор2.5\\SourceText\\output.txt";

    encryptWithSimpleSubstitution(inputFile, outputFile);
    return 0;
}
