#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>

using namespace std;

/**
 * @class SimpleSubstitutionCipher
 * @brief Реализация простого шифра с подстановкой.
 *
 * Этот класс реализует шифрование и дешифрование текста
 * с использованием подстановочной таблицы.
 * Он поддерживает алфавиты как для латинского, так и для русского языков.
 */
class SimpleSubstitutionCipher {
private:
    // Карта для шифрования символов
    unordered_map<char, char> substitutionMap; /**< сопоставление открытого текста и зашифрованного текста **/

    // Карта для дешифрования символов
    unordered_map<char, char> reverseSubstitutionMap; /**< сопоставление зашифрованного текста и открытого текста **/

    /**
     * @brief Вычисляет контрольную сумму заданного текста.
     *
     * Функция перебирает каждый символ строки и суммирует их ASCII значения.
     *
     * @param text Строка, для которой необходимо вычислить контрольную сумму.
     * @return unsigned int Контрольная сумма текста.
     */
    unsigned int calculateChecksum(const string& text) {
        unsigned int checksum = 0;
        for (char c : text) {
            checksum += static_cast<unsigned int>(c); // Суммируем ASCII значения символов
        }
        return checksum;
    }

public:
    /**
     * @brief Конструктор для инициализации шифрования.
     *
     * Он создаёт таблицы подстановки для открытого текста и шифрованного текста.
     * Включает латинские буквы, русские буквы и цифры.
     */
    SimpleSubstitutionCipher() {
        // Алфавит открытого текста для латинских и русских символов
        const string plaintext =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
            "0123456789";

        // Алфавит шифрованного текста, содержащий перестановленные символы
        const string ciphertext =
            "NOPQRSTUVWXYZABCDEFGHIJKLM"
            "РСТУФХЦЧШЩЪЫЬЭЮЯАБВГДЕЖЗИЙКЛМНОП"
            "5678901234";

        // Заполнение карт замены
        for (size_t i = 0; i < plaintext.size(); ++i) {
            substitutionMap[plaintext[i]] = ciphertext[i]; // Заполняем карту замены для шифрования
            reverseSubstitutionMap[ciphertext[i]] = plaintext[i]; // Заполняем карту для дешифрования
        }
    }

    /**
     * @brief Шифрует открытый текст.
     *
     * Эта функция заменяет каждый символ в открытом тексте на
     * соответствующий символ в шифрованном тексте, используя карту подстановки.
     *
     * @param plaintext Открытый текст для шифрования.
     * @return string Зашифрованный текст с добавленной длиной и контрольной суммой.
     */
    string encrypt(const string& plaintext) {
        string ciphertext;
        for (char c : plaintext) {
            if (substitutionMap.find(c) != substitutionMap.end()) {
                ciphertext += substitutionMap[c]; // Заменяем символ
            }
            else {
                ciphertext += c; // Сохраняем символ без изменений, если он не в карте
            }
        }
        unsigned int checksum = calculateChecksum(ciphertext);
        // Возвращаем зашифрованный текст и метаданные о длине и контрольной сумме
        return to_string(ciphertext.length()) + "|" + to_string(checksum) + "|" + ciphertext;
    }

    /**
     * @brief Дешифрует зашифрованный текст.
     *
     * Эта функция извлекает длину и контрольную сумму из зашифрованного текста,
     * затем восстанавливает открытый текст, используя обратную карту подстановки.
     *
     * @param encryptedText Зашифрованный текст для дешифрования.
     * @return string Восстановленный открытый текст.
     * @throws invalid_argument Если формат зашифрованного текста неверен.
     * @throws invalid_argument Если длина дешифрованного текста не совпадает с заданной.
     * @throws invalid_argument Если контрольная сумма не совпадает.
     */
    string decrypt(const string& encryptedText) {
        // Извлечение длины и контрольной суммы из зашифрованного текста
        size_t firstPipe = encryptedText.find('|');
        size_t secondPipe = encryptedText.find('|', firstPipe + 1);

        if (firstPipe == string::npos || secondPipe == string::npos) {
            throw invalid_argument("Invalid encrypted format"); // Проверка формата
        }

        int length = stoi(encryptedText.substr(0, firstPipe)); // Длина текста
        unsigned int checksum = stoi(encryptedText.substr(firstPipe + 1, secondPipe - firstPipe - 1)); // Контрольная сумма
        string ciphertext = encryptedText.substr(secondPipe + 1); // Зашифрованный текст

        if (ciphertext.length() != length) {
            throw invalid_argument("Decrypted text length mismatch"); // Проверка длины
        }

        unsigned int calculatedChecksum = calculateChecksum(ciphertext); // Вычисление контрольной суммы
        if (calculatedChecksum != checksum) {
            throw invalid_argument("Checksum mismatch"); // Проверка контрольной суммы
        }

        string plaintext;
        for (char c : ciphertext) {
            if (reverseSubstitutionMap.find(c) != reverseSubstitutionMap.end()) {
                plaintext += reverseSubstitutionMap[c]; // Восстанавливаем символ
            }
            else {
                plaintext += c; // Сохраняем символ без изменений
            }
        }
        return plaintext; // Возвращаем восстановленный открытый текст
    }
};

/**
 * @brief Пример использования класса SimpleSubstitutionCipher.
 *
 * В этом разделе создается экземпляр шифра, осуществляется
 * шифрование и дешифрование текста, а также выполняется проверка
 * на корректность.
 */
int main() {
    SimpleSubstitutionCipher cipher;

    string originalText = "HELLO123"; // Исходный текст
    cout << "Original text: " << originalText << endl;

    string encryptedText = cipher.encrypt(originalText); // Шифруем текст
    cout << "Encrypted text: " << encryptedText << endl;

    string decryptedText = cipher.decrypt(encryptedText); // Дешифруем текст
    cout << "Decrypted text: " << decryptedText << endl;

    assert(originalText == decryptedText); // Проверка на совпадение

    cout << "All tests passed!" << endl; // Сообщение о успешном завершении
    return 0; // Завершение программы
}