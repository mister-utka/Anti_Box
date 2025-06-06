#include <windows.h>
#include <stdio.h>

#include "proxy_funck.c"

int Get_Uptime()
{
    ULONGLONG uptime_ms = ProxyGetTickCount64();                        // Получаем uptime системы в милисекундах
                                                                        // GetTickCount64 - функция win32api возвращает кол-во милисекунда, прошедших с момента запуска системы

    ULONGLONG total_second = uptime_ms / 1000;                          // Переводим милисекунды в секунды

    int days = (int)(total_second / 86400);                             // Считаем кол-во полных дней (в 1 дне 86400 секунд)
    int hours = (int)((total_second % 86400) / 3600);                   // Оставшиеся после дней часы (в 1 часе 3600 секунд)
    int minutes = (int)((total_second % 3600) / 60);                    // Оставшиеся после часов минуты (в 1 минуте 60 секунд)
    int second = (int)(total_second % 60);                              // Оставшиеся секунды

    // printf("Uptime: %d, %02d:%02d:%02d (days, hh:mm:ss)\n", days, hours, minutes, second);
    
    return uptime_ms;
}

int Check_Uptime()
{
    // 1 sec = 1 000 ms
    // 1 min = 60 000 ms
    // 1 hour = 3 600 000 ms
    
    int uptime_ms = Get_Uptime();

    // < 10 min
    if (uptime_ms < 600000) {
        return 7;
    }
    // < 30 min
    else if (uptime_ms < 1800000) {
        return 4;
    }
    // < 1 hour
    else if (uptime_ms < 3600000) {
        return 1;
    }
    // > 1 hour
    else return 0;
}