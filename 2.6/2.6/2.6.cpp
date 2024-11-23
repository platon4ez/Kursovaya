#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <locale>
#include <io.h>
#include <fcntl.h>

// Функция для подсчета контрольной суммы (простая сумма Unicode-кодов символов)
/**
 * @brief Вычисляет контрольную сумму строки.
 *
 * Данная функция суммирует Unicode коды всех символов строки.
 * Контрольная сумма используется для обеспечения целостности данных,
 * что позволяет в дальнейшем проверить, был ли файл изменен.
 *
 * @param text Входная строка, для которой необходимо рассчитать контрольную сумму.
 * @return unsigned char Возвращает контрольную сумму, представленную в виде беззнакового символа.
 *
 * @note Контрольная сумма рассчитывается путем сложения всех Unicode-кодов символов строки.
 * Это позволяет получить уникальное значение для каждого текста. В случае изменений в строке
 * сумма будет отличаться.
 */
unsigned char calculateChecksum(const std::wstring& text) {
    unsigned char checksum = 0;
    for (wchar_t ch : text) {
        checksum += static_cast<unsigned char>(ch);
    }
    return checksum;
}

/**
 * @brief Генерирует таблицу замены для шифрования.
 *
 * Функция формирует таблицу замены на основе заданной строки `plain`,
 * которая представляет собой множество символов, подлежащих шифрованию.
 * В зависимости от минут текущего времени определяется,
 * какая таблица (четная или нечетная) будет использоваться для шифрования.
 *
 * Если параметр `timeBased` установлен в `true`, то таблица замены будет зависеть от текущей
 * минуты. В чётные минуты будет использоваться одна таблица, а в нечётные — другая.
 * Если параметр `timeBased` равен `false`, будет использоваться постоянная таблица.
 *
 * @param timeBased Флаг, указывающий на необходимость использования времени для выбора таблицы.
 *                  Если `true`, таблица замены зависит от текущего времени.
 * @return std::map<wchar_t, wchar_t> Возвращает таблицу соответствий символов для шифрования.
 *
 * @note Данная функция использует два набора символов для замены:
 * - Одни символы используются для чётных минут.
 * - Другие для нечётных минут.
 * Это позволяет варьировать шифрование в зависимости от времени.
 */
std::map<wchar_t, wchar_t> getSubstitutionTable(bool timeBased) {
    std::map<wchar_t, wchar_t> table;
    std::wstring plain =
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
        L"0123456789";
    std::wstring cipher_even =
        L"NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm"
        L"РСТУФХЦЧШЩЪЫЬЭЮЯАБВГДЕЁЖЗИЙКЛМНОПабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
        L"5678901234";
    std::wstring cipher_odd =
        L"ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"
        L"ЯЮЭЬЫЪЩШЧЦХФУТСРПОНМЛКИЙЗЖЁЕДГВБАяюэьыъщшчцхфутсронмлкийзжёедгвба"
        L"9876543210";

    bool use_even = true;
    if (timeBased) {
        std::time_t t = std::time(nullptr);
        std::tm now;
        localtime_s(&now, &t); // Безопасная альтернатива localtime
        use_even = (now.tm_min % 2 == 0); // Чётная минута — even
    }

    const std::wstring& cipher = use_even ? cipher_even : cipher_odd;
    for (size_t i = 0; i < plain.size(); ++i) {
        table[plain[i]] = cipher[i];
    }
    return table;
}

/**
 * @brief Шифрует текст из входного файла и сохраняет в выходной файл.
 *
 * Функция считывает текст из входного файла с помощью функции `std::wifstream`,
 * использует таблицу замены для шифрования текста, а затем записывает
 * результат в выходной файл.
 *
 * Если таблица замены зависит от времени, используется флаг `timeBased`,
 * который определяет, какую таблицу использовать для замены символов.
 *
 * @param inputFile Путь к входному файлу. Должен быть в формате Unicode (UTF-8).
 * @param outputFile Путь к выходному файлу, в который будет записан зашифрованный текст.
 * @param timeBased Флаг, указывающий на необходимость использования времени для выбора таблицы.
 *
 * @exception std::ios_base::failure Если не удается открыть входной или выходной файл.
 *
 * @note Перед использованием этой функции убедитесь, что файлы находятся в правильной кодировке.
 * Входной файл должен быть в формате Unicode (UTF-8).
 */
void encrypt(const std::wstring& inputFile, const std::wstring& outputFile, bool timeBased) {
    try {
        std::wifstream infile(inputFile);
        infile.imbue(std::locale("Russian_Russia.65001"));
        if (!infile.is_open()) {
            throw std::ios_base::failure("Не удалось открыть входной файл.");
        }

        std::wstring plaintext((std::istreambuf_iterator<wchar_t>(infile)), std::istreambuf_iterator<wchar_t>());
        infile.close();

        auto substitutionTable = getSubstitutionTable(timeBased);

        std::wstring ciphertext;
        for (wchar_t ch : plaintext) {
            if (substitutionTable.find(ch) != substitutionTable.end()) {
                ciphertext += substitutionTable[ch];
            }
            else {
                ciphertext += ch; // Если символ не найден в таблице, добавляем его без изменений
            }
        }

        unsigned char checksum = calculateChecksum(plaintext);

        // Создание выходного файла или перезапись, если он существует
        std::wofstream outfile(outputFile);
        outfile.imbue(std::locale("Russian_Russia.65001"));
        if (!outfile.is_open()) {
            throw std::ios_base::failure("Не удалось создать выходной файл.");
        }

        // Запись зашифрованного текста и контрольной суммы
        outfile << ciphertext << L"\nКонтрольная сумма: " << static_cast<int>(checksum) << std::endl;
        outfile.close();

        std::wcout << L"Файл успешно зашифрован. Результат сохранён в " << outputFile << std::endl;
    }
    catch (const std::exception& e) {
        std::wcerr << L"Произошла ошибка: " << e.what() << std::endl;
    }
}

/**
 * @brief Главная функция программы.
 *
 * Устанавливает кодировку консоли для правильной работы с Unicode,
 * проверяет существование входного файла и запускает процесс шифрования.
 * В случае ошибки, программа выводит информацию о возникшей ошибке.
 *
 * @param argc Количество аргументов командной строки.
 * @param argv Массив аргументов командной строки.
 * @return int Код завершения программы.
 *
 * @exception std::ios_base::failure Если не удается открыть или создать файлы.
 *
 * @note Путь к файлам должен быть указан в абсолютном формате. Убедитесь, что файлы находятся в формате UTF-8.
 */
int wmain(int argc, wchar_t* argv[]) {
    // Установка UTF-8 для консоли (для Windows)
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);
#endif

    // Указываем явный путь для входного и выходного файлов
    std::wstring inputFile = L"C:\\Users\\Платон\\Desktop\\Шифратор2.6\\SourceText\\input.txt";
    std::wstring outputFile = L"C:\\Users\\Платон\\Desktop\\Шифратор2.6\\SourceText\\output.txt";
    bool timeBased = true;  // Если вы хотите использовать время, оставьте true, если нет — замените на false

    // Проверка существования входного файла
    std::wifstream infile(inputFile);
    if (!infile) {
        std::wcerr << L"Входной файл не существует: " << inputFile << std::endl;
        return 1;
    }

    encrypt(inputFile, outputFile, timeBased);
    return 0;
}
