#include <windows.h>
#include <stdio.h>

#include "proxy_funck.c"

/*
typedef struct {                        // Структура для хранения информации о диске
    char name[4];                       // Для имени диска (например С:)
    ULARGE_INTEGER total;               // Общий размер диска в байтах
    ULARGE_INTEGER free;                // Свободное место на диске в байтах
} DISK_INFO;
*/

/*
void formatSize(ULARGE_INTEGER size, char* buffer, size_t bufferSize) {      // Для формирования размера в читаемый формат (B, KB, MB, GB, TB)
    static const char* suffixes[] = { "B", "KB", "MB", "GB", "TB" };        // Массив суффиксов для разных едениц измерения

    double bytes = size.QuadPart;                                           // Преобразуем 64-битное число в double для вычислений
    int i = 0;                                                              // Индекс текущего суфикса (начинаем с байтов)

    while (bytes >= 1024 && i < 4) {                                        // Делим на 1024 пока не получим значения меньше 1024 или не достигнем TB
        bytes /= 1024;
        i++;
    }
    

    sprintf_s(buffer, bufferSize, "%.2f %s", bytes, suffixes[i]);           // Форматируем строку в виде "XX.XX суфикс"
}
*/

int Get_Disk_Info(ULARGE_INTEGER* totalDiskSpace) { // Получаем список всех логических дисков в системе

    typedef struct {                                // Структура для хранения информации о диске
        char name[4];                               // Для имени диска (например С:)
        ULARGE_INTEGER total;                       // Общий размер диска в байтах
        ULARGE_INTEGER free;                        // Свободное место на диске в байтах
    } DISK_INFO;

    char buffer[1024];                              // Для хранения имени диска
    DWORD bufferSize = 1024;                        // Размер буфера
    DWORD drives = ProxyGetLogicalDrives();              // Битовая маска доступных дисков
                                                    // GetLogicalDrives возвращает значение типа DWORD (32-битное целое число)
                                                    // где каждый бит соответствует определенной букве диска (бит 0 - диск A, бит 1 - диск B ...)

    for (DWORD i=0; i<26; i++) {                    // Перебираем все возможные буквы дисков (A-Z)
        if (drives & (1 <<i)) {                     // Проверяем, существует ли диск с текущей буквой
                                                    // 1 << i создает маску с единицей на нужной позиции
                                                    // для диска A(i=0): 1 << 0 = 00000001
                                                    // для диска B(i=1): 1 << 1 = 00000010
            
            sprintf_s(buffer, sizeof(buffer), "%c:\\", 'A' + i);    // Формируем имя диска (например "C:\")
            
            ULARGE_INTEGER freeBytes, totalBytes, totalFree;        // Получаем инорфмацию о свободном и общем месте на диске
            if(ProxyGetDiskFreeSpaceExA(buffer, &freeBytes, &totalBytes, &totalFree)) {
                DISK_INFO diskInfo;                                 // Создаем структуру для хранения информации о текущем диске
                /*
                diskInfo.name[0] = 'A' + i;
                diskInfo.name[1] = ':';
                diskInfo.name[2] = '\\';
                diskInfo.name[3] = '\0';
                */

                diskInfo.total = totalBytes;
                /*
                diskInfo.free = freeBytes;

                char totalSize[50], freeSize[50], usedSize[50];                   // Буферы для форматированного вывода размеров

                formatSize(diskInfo.total, totalSize, 50);
                formatSize(diskInfo.free, freeSize, 50);
                
                ULARGE_INTEGER usedSpace;                           // Вычисляем использованное место
                usedSpace.QuadPart = diskInfo.total.QuadPart - diskInfo.free.QuadPart;
                formatSize(usedSpace, usedSize, 50);

                printf("Disk %s:\n", diskInfo.name);
                printf("  Overall size: %s\n", totalSize);
                printf("  Free space: %s\n", freeSize);
                printf("  Used: %s\n\n", usedSize);
                */

                totalDiskSpace->QuadPart = totalDiskSpace->QuadPart + diskInfo.total.QuadPart;
            }            
        }

    }
    return 0;
}


int Check_Disk_Info()
{
    ULARGE_INTEGER totalDiskSpace = {0};
    Get_Disk_Info(&totalDiskSpace);

    double bytes_totalDiskSpace = totalDiskSpace.QuadPart;
    double gbytes_totalDiskSpace = bytes_totalDiskSpace / (1024 * 1024 * 1024);

    /*
    double kb = bytes_totalDiskSpace / 1024.0;
    double mb = kb / 1024.0;
    double gbytes_totalDiskSpace = mb / 1024.0;

    printf("bytes_totalDiskSpace: %.2f\n", bytes_totalDiskSpace);
    printf("gbytes_totalDiskSpace: %.2f\n", gbytes_totalDiskSpace);
    */

    if(gbytes_totalDiskSpace < 100.0) {         // Если общее дисковое пространство меньше 100 GB
        return 7;
    }

    return 0;
}


































// #include <windows.h>
// #include <stdio.h>

// // Структура для хранения информации о диске
// typedef struct {
//     char name[4];           // Имя диска (например, "C:")
//     ULARGE_INTEGER total;    // Общий размер
//     ULARGE_INTEGER free;     // Свободное место
// } DISK_INFO;

// // Функция для форматирования размера в читаемый формат
// void formatSize(ULARGE_INTEGER size, char* buffer, size_t bufferSize) {
//     static const char* suffixes[] = { "B", "KB", "MB", "GB", "TB" };
//     double bytes = size.QuadPart;
//     int i = 0;
    
//     while (bytes >= 1024 && i < 4) {
//         bytes /= 1024;
//         i++;
//     }
    
//     sprintf_s(buffer, bufferSize, "%.2f %s", bytes, suffixes[i]);
// }

// int main() {
//     // Получаем список всех логических дисков
//     char buffer[1024];
//     DWORD bufferSize = 1024;
//     DWORD drives = GetLogicalDrives();
    
//     // Перебираем все возможные буквы дисков
//     for (DWORD i = 0; i < 26; i++) {
//         if (drives & (1 << i)) {
//             // Формируем имя диска
//             sprintf_s(buffer, sizeof(buffer), "%c:\\", 'A' + i);
            
//             // Получаем информацию о диске
//             ULARGE_INTEGER freeBytes, totalBytes, totalFree;
//             if (GetDiskFreeSpaceExA(buffer, &freeBytes, &totalBytes, &totalFree)) {
//                 DISK_INFO diskInfo;
//                 diskInfo.name[0] = 'A' + i;
//                 diskInfo.name[1] = ':';
//                 diskInfo.name[2] = '\\';
//                 diskInfo.name[3] = '\0';
//                 diskInfo.total = totalBytes;
//                 diskInfo.free = freeBytes;
                
//                 // Выводим информацию
//                 char totalSize[50], freeSize[50];
//                 formatSize(diskInfo.total, totalSize, 50);
//                 formatSize(diskInfo.free, freeSize, 50);
                
//                 printf("Disk %s:\n", diskInfo.name);
//                 printf("  Overall size: %s\n", totalSize);
//                 printf("  Free space: %s\n", freeSize);
//                 printf("  Used: ");
                
//                 // Вычисляем использованное место
//                 ULARGE_INTEGER usedSpace;
//                 usedSpace.QuadPart = diskInfo.total.QuadPart - diskInfo.free.QuadPart;
//                 formatSize(usedSpace, totalSize, 50);
//                 printf("%s\n\n", totalSize);
//             }
//         }
//     }
    
//     return 0;
// }

























// #include <windows.h>
// #include <stdio.h>
// #include <winioctl.h>

// #define IOCTL_SCSI_PASS_THROUGH 0x0004D008  // Для SCSI команд

// typedef struct _SCSI_PASS_THROUGH {
//     ULONG Length;
//     UCHAR CdbLength;
//     UCHAR SenseInfoLength;
//     ULONG DataIn;
//     ULONG DataTransferLength;
//     ULONG TimeOutValue;
//     ULONG DataBuffer;
//     ULONG SenseInfoOffset;
//     UCHAR Cdb[16];
// } SCSI_PASS_THROUGH, *PSCSI_PASS_THROUGH;

// int main() {
//     HANDLE hDevice;
//     DWORD bytesReturned;
//     char buffer[512];  // Буфер для получения данных
//     SCSI_PASS_THROUGH spt;
    
//     // Открываем физический диск (например, \\.\PhysicalDrive0)
//     hDevice = CreateFile(
//         "\\\\.\\PhysicalDrive0",  // Путь к устройству
//         GENERIC_READ | GENERIC_WRITE,  // Чтение и запись
//         0,  // Нет совместного использования
//         NULL,  // Нет атрибутов безопасности
//         OPEN_EXISTING,  // Открываем существующее устройство
//         0,  // Без атрибутов
//         NULL   // Нет шаблона
//     );

//     if (hDevice == INVALID_HANDLE_VALUE) {
//         printf("Не удалось открыть устройство. Ошибка: %d\n", GetLastError());
//         return 1;
//     }

//     // Заполняем структуру SCSI_PASS_THROUGH
//     memset(&spt, 0, sizeof(spt));
//     spt.Length = sizeof(SCSI_PASS_THROUGH);
//     spt.CdbLength = 6;
//     spt.DataIn = 1;  // Указываем, что данные передаются в сторону хоста (READ)
//     spt.DataTransferLength = sizeof(buffer);
//     spt.DataBuffer = (ULONG)(ULONG_PTR)buffer;

//     // INQUIRY команда для получения модели устройства
//     spt.Cdb[0] = 0x12;  // INQUIRY команда
//     spt.Cdb[4] = sizeof(buffer);  // Размер данных для INQUIRY

//     // Выполняем команду
//     if (!DeviceIoControl(
//         hDevice,
//         IOCTL_SCSI_PASS_THROUGH,
//         &spt,
//         sizeof(SCSI_PASS_THROUGH),
//         buffer,
//         sizeof(buffer),
//         &bytesReturned,
//         NULL
//     )) {
//         printf("Ошибка выполнения DeviceIoControl. Ошибка: %d\n", GetLastError());
//         CloseHandle(hDevice);
//         return 1;
//     }

//     // Выводим модель устройства (это будет в данных INQUIRY)
//     printf("Модель устройства: %.40s\n", buffer + 16);  // Модель начинается с 16-го байта

//     // Закрываем дескриптор устройства
//     CloseHandle(hDevice);
    
//     return 0;
// }
