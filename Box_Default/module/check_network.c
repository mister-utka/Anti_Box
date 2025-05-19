#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <Iphlpapi.h>
#include <ws2tcpip.h>
#include <string.h>

#include "proxy_funck.c"

/*
// Необходимо для использования функции GetAdaptersInfo
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
*/

DWORD GetNetworkInterfaces(PIP_ADAPTER_INFO* pAdapterInfo)       // Функция для получения информации об интерфейсах
{
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    dwRetVal = ProxyGetAdaptersInfo(*pAdapterInfo, &dwSize);     // Вызываем функцию для получения размера буфера
    if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
        *pAdapterInfo = (PIP_ADAPTER_INFO) malloc(dwSize);
        if (*pAdapterInfo == NULL) {
            // printf("Memory allocation error");
            return -1;
        }
    }

    dwRetVal = ProxyGetAdaptersInfo(*pAdapterInfo, &dwSize);     // Получаем информацию об адаптерах
    return dwRetVal;
}


const char* CheckMacPrefix(BYTE* macAddress)                // Функция для проверки совпадения MAC-адреса с одним из префиксов
{
    struct MacPrefix {                  // Массив невалидных префиксов MAC адресов
        BYTE prefix[3];                 // Первые три байта MAC адреса
        const char* description;        // Описание
    };

    struct MacPrefix validPrefixes[] = {
        {{0x00, 0x50, 0x56}, "VMware ESX 3, Server, Workstation, Player"},
        {{0x00, 0x0C, 0x29}, "VMware ESX 3, Server, Workstation, Player"},
        {{0x00, 0x05, 0x69}, "VMware ESX 3, Server, Workstation, Player"},
        {{0x00, 0x1c, 0x14}, "VMware ESX 3, Server, Workstation, Player"},


        {{0x00, 0x03, 0xff}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x0d, 0x3a}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x50, 0xf2}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x7c, 0x1e, 0x52}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x12, 0x5a}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x15, 0x5d}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x17, 0xfa}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x28, 0x18, 0x78}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x7c, 0xed, 0x8d}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x1d, 0xd8}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x22, 0x48}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x00, 0x25, 0xae}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0x60, 0x45, 0xbd}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},
        {{0xdc, 0xb4, 0xc4}, "Microsoft Hyper-V, Virtual Server, Virtual PC"},

        {{0x00, 0x1c, 0x42}, "Parallels Desktop, Workstation, Server, Virtuozzo"},
        {{0x00, 0x0f, 0x4b}, "Virtual Iron 4"},
        {{0x00, 0x16, 0x3e}, "Red Hat Xen | Oracle VM | XenSource | Novell Xen"},
        {{0x08, 0x00, 0x27}, "Sun xVM VirtualBox"},
    };

    for (int i=0; i < sizeof(validPrefixes)/sizeof(validPrefixes[0]); i++) {    // Проход по масиву префиксов и проверка MAC-адреса
        if (macAddress[0] == validPrefixes[i].prefix[0] &&
            macAddress[1] == validPrefixes[i].prefix[1] &&
            macAddress[2] == validPrefixes[i].prefix[2]) {
            return validPrefixes[i].description;
        }
    }
    return NULL;
}


int Check_MAC()
{
    PIP_ADAPTER_INFO pAdapterInfo = NULL;                   // Указатель на структуру, в которой будет хранится инфа о сетевых интерфейсах
    DWORD dwRetVal = GetNetworkInterfaces(&pAdapterInfo);   // Получение информации об интерфейсах

    if (dwRetVal == ERROR_SUCCESS) {
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;           // Указатель на текущий адптер (инициализируется перез записью)

        while (pAdapter) {                                  // Перебираем все адаптеры в списке
            // printf("Adapter Name: %s\n", pAdapter->AdapterName);
            // printf("Description: %s\n", pAdapter->Description);

            PIP_ADDR_STRING pIpAddr = &pAdapter->IpAddressList;
            /*
            while (pIpAddr) {                                           // Печать всех IP-адресов, связынных с адаптером
                printf("IP Address %s\n", pIpAddr->IpAddress.String);   // Выводим IP адрес текцщего узла
                pIpAddr = pIpAddr->Next;                                // Переходим к следующему IP адресу если он есть
            }
            */

            // printf("MAC address: ");
            for(int i = 0; i < pAdapter->AddressLength; i++) {  // Проходим по всем байтам MAC адреса и выводим в HEX формате
                // printf("%02X", pAdapter->Address[i]);
                if(i != pAdapter->AddressLength - 1) {
                    // printf(":");
                }
            }
            // printf("\n");

            const char* description = CheckMacPrefix(pAdapter->Address);    // Проверка на совпадение MAC адреса с одним из известных префиксов
            if (description) {                                              // Если совпадение найдено
                // printf("This MAC address matches a known prefix: %s\n", description);
                return 4;
            } else {
                // printf("No known prefix match found for this MAC address.\n");
            }

            // printf("\n");

            pAdapter = pAdapter->Next;

        }
    }
    return 0;
}