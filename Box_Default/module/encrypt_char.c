#include <stdio.h>
#include <string.h>

void Xor_Encrypt(char* data, size_t len, const char* key) {         // Данную функцию вставляем в проект
    size_t key_len = strlen(key);                   // Вычисляем длину ключа
    for (size_t i = 0; i < len; ++i) {              // Применяем xor к каждому байту байту данных
        data[i] ^= key[i % key_len];
    }
}

void Print_Arrt_Crypt(const char *label, const char *data, size_t len) {
    printf("char %s[%zu] = {", label, len + 1);     // Начало объявления массива
    for (size_t i = 0; i < len; ++i) {              // Печатаем каждый байт в виде 16-ти ричного литерала 0xXX
        printf("0x%02X, ", (unsigned char)data[i]);
    }
    printf("0x00 };\n");                             // Добавляем нультерминатор
}


int main() {
    char message[] = "DEV_A001";
    const char *key = "WZQRETOQIEI";

    size_t len = sizeof(message) - 1;
    Print_Arrt_Crypt("1", message, len);

    Xor_Encrypt(message, len, key);                   // Зашифровка
    Print_Arrt_Crypt("encrypted", message, len);      // Вывод как C-массив

    char encrypted[9] = {0x13, 0x1F, 0x07, 0x0D, 0x04, 0x64, 0x7F, 0x60, 0x00 };

    Xor_Encrypt(encrypted, strlen(encrypted), key);   // Рассшифровка
    
    Print_Arrt_Crypt("2", encrypted, len);

    printf("decrypt: %s\n", encrypted);

    return 0;
}
