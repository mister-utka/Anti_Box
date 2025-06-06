#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <tchar.h>
#include <cfgmgr32.h>

// #pragma comment(lib, "setupapi.lib")

#include "proxy_funck.c"


int Get_First_Video_Device(DISPLAY_DEVICE *dd_out) {
    DISPLAY_DEVICE dd;                      // Структура для хранения информации о видеоконтроллере
    DEVMODE devMode;                        // Структура для хранения информации о настройках экрана
    int deviceIndex = 0;                    // Индекс устройства вывода, начиная с 0

    ZeroMemory(&dd, sizeof(dd));            // Инициализация структуры DISPLAY_DEVICE для нулевого усройста (заполняем нулями)
    dd.cb = sizeof(dd);                     // Указываем размер структуры

    while (ProxyEnumDisplayDevices(NULL, deviceIndex, &dd, 0)) {     // Перебираем все устройства вывода, пока есть устройства
        /*
        printf("Device Name: %s\n", dd.DeviceName);             // Выводим имя устройства
        printf("Device String: %s\n", dd.DeviceString);         // Описание устройства (нарпимер, название видеоакрты)
        printf("Devide State: %d\n", dd.StateFlags);            // Состояние устройства (вкл / выкл)
        */
                                                                //    StateFlags = 5
                                                                //    5 (десятичное) = 0b101 (двоичное)
                                                                //    DISPLAY_DEVICE_ACTIVE (бит 0) DISPLAY_DEVICE_MIRRORING_DRIVER (бит 3)
                                                                //    => стройство активно (выведено на экран) и Устройство используется в качестве зеркала
    

        ZeroMemory(&devMode, sizeof(devMode));  // Инициализация структуры DEVMODE
        devMode.dmSize = sizeof(devMode);       // Указываем размер структуры

        /*
        if (EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &devMode)) {           // Получаем настройки экрана для устройства
            printf("screen Resolution: %dx%d\n", devMode.dmPelsWidth, devMode.dmPelsHeight); // Выводим разерешение экрана (ширина и высота в пикселях)
            printf("Color Depth: %d bpp\n", devMode.dmBitsPerPel);                           // Выводим глубину цвета на пиксель (бит на пиксель)
            printf("Refresh Rate: %d Hz\n", devMode.dmDisplayFrequency);                     // Выводим герцовку экрана (в герцах) 
        }
        printf("\n");       // Разделитель между устройствами
        */

        *dd_out = dd;
        return 0;

        /*
        deviceIndex++;      // Переходим к следующему устройству
        */
    }
    return -1;
}


int Get_First_Video_Code(char* video_device_code)
{
    static const GUID GUID_DEVCLASS_DISPLAY = {             // Статическое определение GUID для класса видеоконтроллеров (Display adapters)
    0x4d36e968, 0xe325, 0x11ce,                             // это тоже, что и GUID_DEVCLASS_DISPLAY из <initguid.h>, но объявлено вручную
   {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};

    HDEVINFO deviceInfoSet;                                 // Хендл для множества устройства
    SP_DEVINFO_DATA devInfoData;                            // Структура для хранения информации об устройстве
    DWORD i;                                                // Индекс устройства

    deviceInfoSet = ProxySetupDiGetClassDevs(               // Получаем список всех текущих устройств видеоконтролерров (DISPLAY)
        &GUID_DEVCLASS_DISPLAY,                             // Указываем класс устройств - видеоконтролеры
        NULL,
        NULL,
        DIGCF_PRESENT                                       // Только присутствующие в системе устройства
    );

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        // printf("Error: couldn`t get a list of video devices");
        return -1;
    };

    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);           // Устанавливаем размер структуры перед использованием

    for (i=0; ProxySetupDiEnumDeviceInfo(deviceInfoSet, i, &devInfoData); i++) {     // Перебираем устройства по индексу
        char buffer[1024];                                  // Буфер для хранения строковых данных
        DWORD size = 0;

        if(ProxySetupDiGetDeviceInstanceIdA(deviceInfoSet, &devInfoData, buffer, sizeof(buffer), &size)) {    // Получаем строку ID устоойства (PCI\VEN_15AD&DEV_0405&SUBSYS_040515AD...)
            // printf("Video device %lu: %s\n", i, buffer);    // Вывод идентификации устройства
            strncpy(video_device_code, buffer, 1024 - 1);
            return 0;
        }
        else return -1;
    };

   return 0;
}


int Check_First_Video_Device()
{
    DISPLAY_DEVICE dd;                                          // Структура для хранения информации о видеоконтроллере
    ZeroMemory(&dd, sizeof(dd));                                // Структура для хранения информации о настройках экрана
    dd.cb = sizeof(dd);                                         // Указываем размер структуры
    
    if(Get_First_Video_Device(&dd) == -1) {                     // Получаем имя видеоустройства, если не смогли, программа завершает работу
        return 7;
    };

    /*
    char* virtual_keywords[] = {                                // Имена виртуальных видеоадаптеров
        "Microsoft Basic Display Adapter",
        "VMware SVGA 3D",
        "VirtualBox Graphics Adapter",
        "Hyper-V Video",
        "Parallels Display Adapter (WDDM)",
        "QXL",
        "Red Hat QXL"
        "Xen VGA",
        "Citrix Display Adapter",
        "GDI Generic",
        "VBOX DISP Adapter"
    };
    */

    char virtual_keyword1_utka[] = "Microsoft Basic Display Adapter";
    char virtual_keyword2_utka[] = "VMware SVGA 3D";
    char virtual_keyword3_utka[] = "VirtualBox Graphics Adapter";
    char virtual_keyword4_utka[] = "Hyper-V Video";
    char virtual_keyword5_utka[] = "Parallels Display Adapter (WDDM)";
    char virtual_keyword6_utka[] = "QXL";
    char virtual_keyword7_utka[] = "Red Hat QXL";
    char virtual_keyword8_utka[] = "Xen VGA";
    char virtual_keyword9_utka[] = "Citrix Display Adapter";
    char virtual_keyword10_utka[] = "GDI Generic";
    char virtual_keyword11_utka[] = "VBOX DISP Adapter";


    char* virtual_keywords[] = {
        virtual_keyword1_utka,
        virtual_keyword2_utka,
        virtual_keyword3_utka,
        virtual_keyword4_utka,
        virtual_keyword5_utka,
        virtual_keyword6_utka,
        virtual_keyword7_utka,
        virtual_keyword8_utka,
        virtual_keyword9_utka,
        virtual_keyword10_utka,
        virtual_keyword11_utka
    };

    int keyword_count = sizeof(virtual_keywords) / sizeof(virtual_keywords[0]);
    
    for(int i=0; i<keyword_count; ++i) {                        // Проверям полученное имя на совпадение
        if(strstr(dd.DeviceString, virtual_keywords[i]) != NULL) {
            // printf("Found: %s\n", virtual_keywords[i]);
            return 7;
        }
    }



    char video_device_code[1024];
    
    if(Get_First_Video_Code(video_device_code) == -1) {          // Получаем VEN и DEV коды видеоустройства
        return 7;
    };

    
    /*
    char* virtual_codes[] = {                                     // Виртуальные VEN и DEV коды
    //  Vendor ID    Device ID
        "VEN_15AD", "DEV_0405", "DEV_0740", "DEV_07A0",          // VMware
        "VEN_80EE", "DEV_BEEF", "DEV_CAFE",                      // VirtualBox
        "VEN_1414", "DEV_5353", "DEV_07B0", "DEV_08B0",          // Microsoft Hyper-V
        "VEN_1AF4", "DEV_1110",                                  // QEMU / KVM
        "VEN_1AB8", "DEV_4000", "DEV_4005",                      // Parallels Display Adapter
        "VEN_5853", "DEV_0001", "DEV_0002", "DEV_0003", "DEV_0004", "DEV_0005", "DEV_0006", "DEV_0007", "DEV_0008", "DEV_0009", "DEV_000A", "DEV_000B", "DEV_000C", "DEV_000D", "DEV_000E", "DEV_000F"  // Citrix Xen
        "VEN_1234", "DEV_1111",                                  //	Bochs/QEMU Generic VGA
        "VEN_1B36", "DEV_0100",                                  // Red Hat / QEMU
        "VEN_1AE0", "DEV_A001"                                   // Google
        
    };
    */

    char virtual_code1_utka[]  = "VEN_15AD";
    char virtual_code2_utka[]  = "DEV_0405";
    char virtual_code3_utka[]  = "DEV_0740";
    char virtual_code4_utka[]  = "DEV_07A0";
    char virtual_code5_utka[]  = "VEN_80EE";
    char virtual_code6_utka[]  = "DEV_BEEF";
    char virtual_code7_utka[]  = "DEV_CAFE";
    char virtual_code8_utka[]  = "VEN_1414";
    char virtual_code9_utka[]  = "DEV_5353";
    char virtual_code10_utka[] = "DEV_07B0";
    char virtual_code11_utka[] = "DEV_08B0";
    char virtual_code12_utka[] = "VEN_1AF4";
    char virtual_code13_utka[] = "DEV_1110";
    char virtual_code14_utka[] = "VEN_1AB8";
    char virtual_code15_utka[] = "DEV_4000";
    char virtual_code16_utka[] = "DEV_4005";
    char virtual_code17_utka[] = "VEN_5853";
    char virtual_code18_utka[] = "DEV_0001";
    char virtual_code19_utka[] = "DEV_0002";
    char virtual_code20_utka[] = "DEV_0003";
    char virtual_code21_utka[] = "DEV_0004";
    char virtual_code22_utka[] = "DEV_0005";
    char virtual_code23_utka[] = "DEV_0006";
    char virtual_code24_utka[] = "DEV_0007";
    char virtual_code25_utka[] = "DEV_0008";
    char virtual_code26_utka[] = "DEV_0009";
    char virtual_code27_utka[] = "DEV_000A";
    char virtual_code28_utka[] = "DEV_000B";
    char virtual_code29_utka[] = "DEV_000C";
    char virtual_code30_utka[] = "DEV_000D";
    char virtual_code31_utka[] = "DEV_000E";
    char virtual_code32_utka[] = "DEV_000F";
    char virtual_code33_utka[] = "VEN_1234";
    char virtual_code34_utka[] = "DEV_1111";
    char virtual_code35_utka[] = "VEN_1B36";
    char virtual_code36_utka[] = "DEV_0100";
    char virtual_code37_utka[] = "VEN_1AE0";
    char virtual_code38_utka[] = "DEV_A001";

    char* virtual_codes[] = {
        virtual_code1_utka,
        virtual_code2_utka,
        virtual_code3_utka,
        virtual_code4_utka,
        virtual_code5_utka,
        virtual_code6_utka,
        virtual_code7_utka,
        virtual_code8_utka,
        virtual_code9_utka,
        virtual_code10_utka,
        virtual_code11_utka,
        virtual_code12_utka,
        virtual_code13_utka,
        virtual_code14_utka,
        virtual_code15_utka,
        virtual_code16_utka,
        virtual_code17_utka,
        virtual_code18_utka,
        virtual_code19_utka,
        virtual_code20_utka,
        virtual_code21_utka,
        virtual_code22_utka,
        virtual_code23_utka,
        virtual_code24_utka,
        virtual_code25_utka,
        virtual_code26_utka,
        virtual_code27_utka,
        virtual_code28_utka,
        virtual_code29_utka,
        virtual_code30_utka,
        virtual_code31_utka,
        virtual_code32_utka,
        virtual_code33_utka,
        virtual_code34_utka,
        virtual_code35_utka,
        virtual_code36_utka,
        virtual_code37_utka,
        virtual_code38_utka
    };


    keyword_count = sizeof(virtual_codes) / sizeof(virtual_codes[0]);

    for(int i=0; i<keyword_count; ++i) {                         // Проверям полученные VEN и DEV коды на совпадение
        if(strstr(video_device_code, virtual_codes[i]) != NULL) {
            // printf("Found: %s\n", virtual_codes[i]);
            return 7;
        }
    }

    return 0;
}

