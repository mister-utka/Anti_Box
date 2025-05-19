#include <windows.h>
#include <stdio.h>

#include "proxy_funck.c"

int Get_MEM_Info()
{
    MEMORYSTATUSEX memInfo;                         // Структура для храниения информации о памяти
    memInfo.dwLength = sizeof(memInfo);             // Указываем размер структуры перед вызовом

    ProxyGlobalMemoryStatusEx(&memInfo);            // Получам информацию о памяти через Win32 Api

    DWORDLONG totalPhys = memInfo.ullTotalPhys;     // Всего физической памяти (RAM) (в байтах)
    DWORDLONG availPhys = memInfo.ullAvailPhys;     // Доступная (свободная) физическая память
    DWORDLONG usedPhys = totalPhys - availPhys;     // Занятая память: всего - доступная

    // Переводим значения в понятные человеку
    double totalMB = totalPhys / (1024.0 * 1024);   // Переводим байты в MB (1 килбайт = 1024 байта, 1 мегабайт = 1024 кбайта)
    double totalGB = totalMB / 1024;

    double usedMB = usedPhys / (1024.0 * 1024);
    double usedGB = usedMB / 1024;

    double availMB = availPhys / (1024.0 * 1024);
    double availGB = availMB / 1024;

    /*
    // Выводим информацию о памяти
    printf("Total memory capacity: %.2f GB (%.0f MB)\n", totalGB, totalMB);
    printf("Used: %.2f GB (%.0f MB)\n", usedGB, usedMB);
    printf("Available: %.2f GB (%.0f MB)\n", availGB, availMB);
    printf("Free: %.2f GB (%.0f MB)\n", availGB, availMB);  // В этом API Free примерно равно Available
    */

    return (int)totalGB;
}

int Check_MEM_Info()
{
    int totalGB = Get_MEM_Info();
    
    if (totalGB <= 2) {
        return 7;
    }
    else if (totalGB <= 4) {
        return 4;
    }
    else if (totalGB <= 6) {
        return 1;
    }
    else return 0;
}