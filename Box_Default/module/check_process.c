#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>
#include <stdio.h>

#include "proxy_funck.c"

/*
#pragma comment(lib, "psapi.lib")
*/

BOOL IsMonitoredProcess(const TCHAR* processName) {                 // Функция сравнивает имя процесса с отслеживаемыми

    /*
    const TCHAR* monitoredProcesses[] = {
        TEXT("VBoxTray.exe"),                   // Иконка VirtualBox в трее гостевой ОС
        TEXT("VBoxService.exe"),                // Служба VirtualBox Guest Additions
        TEXT("vboxservice.exe"),                // Альтернативное имя службы VirtualBox
        TEXT("vboxguest.sys"),                  // Драйвер VirtualBox Guest

        TEXT("vmtoolsd.exe"),                   // VMware Tools daemon
        TEXT("vmwaretray.exe"),                 // VMware Tools tray icon
        TEXT("vmwareuser.exe"),                 // Пользовательский компонент VMware Tools
        TEXT("vmsrvc.exe"),                     // VMware service
        TEXT("vmhgfs.sys"),                     // VMware shared folders driver
        TEXT("vmware-vmx.exe"),                 // Основной процесс виртуальной машины VMware
        TEXT("vmware-authd.exe"),               // Процесс авторизации VMware.
        
        TEXT("vmicvss.exe"),                    // Компонент, который управляет виртуальными машинами в Hyper-V
        // TEXT("vmms.exe"),                       // Процесс, управляющий виртуальными машинами в Hyper-V              Есть в реальной мащине
        TEXT("vmmem.exe"),                      // Процесс, связанный с памятью виртуальной машины в Hyper-V
        TEXT("vmwp.exe"),                       // Процесс виртуальной машины, который работает в рамках Hyper-V

        TEXT("qemu-system-x86_64.exe"),         // Основной процесс для запуска виртуальных машин в QEMU
        TEXT("qemu-vm-guest.exe"),              // Процесс, связанный с виртуализированными гостями

        TEXT("prl_vm_app.exe"),                 // Основной процесс для работы виртуальных машин в Parallels Desktop
        TEXT("prl_tools.exe"),                  // Процесс, связанный с Parallels Tools, используемый для интеграции гостевой операционной системы и хостовой системы
        TEXT("prl_cc.exe"),                     // Parallels Workstation           
        TEXT("SharedIntApp.exe"),               // Parallels Workstation
        
        TEXT("vmmouse.sys"),                    // Драйвер мыши виртуальной машины
        TEXT("xenservice.exe"),                 // Xen Virtualization environment

        TEXT("WindowsSandbox.exe"),             // Parallels Tools (для macOS/Parallels Desktop)

        TEXT("SandboxieRpcSs.exe"),             // Sandboxie RPC service
        TEXT("SandboxieDcomLaunch.exe"),        // Sandboxie COM Launch
        TEXT("SbieSvc.exe"),                    // Основная служба Sandboxie
        TEXT("SbieCtrl.exe"),                   // Контроллер Sandboxie (GUI)
        TEXT("SxIn.exe"),                       // Comodo Sandbox
        TEXT("procmon.exe"),                    // Process Monitor — анализ поведения

        TEXT("VmRemoteGuest.exe"),              // Обнаружил в процесах одной из песочниц VirusTotal


        TEXT("ntsd.exe"),                       // Windows Debugging Tools
        TEXT("windbg.exe"),                     // Windows Debugging Tools

        TEXT("idaq.exe"),                       // IDA Pro
        TEXT("idag.exe"),                       // IDA Pro

        TEXT("x64dbg.exe"),                     
        TEXT("x32dbg.exe"),                     
    };
    */

    const TCHAR process1_utka[]  = "VBoxTray.exe";
    const TCHAR process2_utka[]  = "VBoxService.exe";
    const TCHAR process3_utka[]  = "vboxservice.exe";
    const TCHAR process4_utka[]  = "vboxguest.sys";
    const TCHAR process5_utka[]  = "vmtoolsd.exe";
    const TCHAR process6_utka[]  = "vmwaretray.exe";
    const TCHAR process7_utka[]  = "vmwareuser.exe";
    const TCHAR process8_utka[]  = "vmsrvc.exe";
    const TCHAR process9_utka[]  = "vmhgfs.sys";
    const TCHAR process10_utka[] = "vmware-vmx.exe";
    const TCHAR process11_utka[] = "vmware-authd.exe";
    const TCHAR process12_utka[] = "vmicvss.exe";
    // const TCHAR process13_utka[] = "vmms.exe"; // В реальной машине — опущен
    const TCHAR process14_utka[] = "vmmem.exe";
    const TCHAR process15_utka[] = "vmwp.exe";
    const TCHAR process16_utka[] = "qemu-system-x86_64.exe";
    const TCHAR process17_utka[] = "qemu-vm-guest.exe";
    const TCHAR process18_utka[] = "prl_vm_app.exe";
    const TCHAR process19_utka[] = "prl_tools.exe";
    const TCHAR process20_utka[] = "prl_cc.exe";
    const TCHAR process21_utka[] = "SharedIntApp.exe";
    const TCHAR process22_utka[] = "vmmouse.sys";
    const TCHAR process23_utka[] = "xenservice.exe";
    const TCHAR process24_utka[] = "WindowsSandbox.exe";
    const TCHAR process25_utka[] = "SandboxieRpcSs.exe";
    const TCHAR process26_utka[] = "SandboxieDcomLaunch.exe";
    const TCHAR process27_utka[] = "SbieSvc.exe";
    const TCHAR process28_utka[] = "SbieCtrl.exe";
    const TCHAR process29_utka[] = "SxIn.exe";
    const TCHAR process30_utka[] = "procmon.exe";
    const TCHAR process31_utka[] = "VmRemoteGuest.exe";
    const TCHAR process32_utka[] = "ntsd.exe";
    const TCHAR process33_utka[] = "windbg.exe";
    const TCHAR process34_utka[] = "idaq.exe";
    const TCHAR process35_utka[] = "idag.exe";
    const TCHAR process36_utka[] = "x64dbg.exe";
    const TCHAR process37_utka[] = "x32dbg.exe";

    const TCHAR* monitoredProcesses[] = {
        process1_utka,
        process2_utka,
        process3_utka,
        process4_utka,
        process5_utka,
        process6_utka,
        process7_utka,
        process8_utka,
        process9_utka,
        process10_utka,
        process11_utka,
        process12_utka,
        // process13_utka,
        process14_utka,
        process15_utka,
        process16_utka,
        process17_utka,
        process18_utka,
        process19_utka,
        process20_utka,
        process21_utka,
        process22_utka,
        process23_utka,
        process24_utka,
        process25_utka,
        process26_utka,
        process27_utka,
        process28_utka,
        process29_utka,
        process30_utka,
        process31_utka,
        process32_utka,
        process33_utka,
        process34_utka,
        process35_utka,
        process36_utka,
        process37_utka
    };

    for (int i = 0; i < sizeof(monitoredProcesses) / sizeof(monitoredProcesses[0]); i++) {
        if (_tcsicmp(processName, monitoredProcesses[i]) == 0) {
            return TRUE;                        // Совпадение найдено
        }
    }
    return FALSE;                               // Не найдено
}


int Check_Process()
{
    DWORD processes[1024];          // Массив для храниения PID-ов процессов
    DWORD cbNeeded;                 // Кол-во реально использованых байт в массиве process
    DWORD processCount;             // Общее число процессов

    if (!ProxyEnumProcesses(processes, sizeof(processes), &cbNeeded)) {      // Получаем список PID всех активных процессов
        // printf("Error get proccess");
        return 0;
    }

    processCount = cbNeeded / sizeof(DWORD);        // Вычисляем число процессов - всего байт / размер одного PID
    // printf("Total number of processes: %lu\n\n", processCount);

    for (DWORD i = 0; i < processCount; i++) {      // Перебираем каждый PID
        DWORD pid = processes[i];                   // Получаем текущий PID

        if(pid==0)                                  // PID 0 - это системный процесс
            continue;
        
        HANDLE hProcess = ProxyOpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);     // Пытаемся открыть процесс с нужными правами
        // PROCESS_QUERY_INFORMATION - для получения информации
        // PROCESS_VM_READ - для чтения памяти процесса (нужно для GetModuleBaseName)

        if (hProcess) {
            HMODULE hMod;                   // Для хранения хендла на первый модуль (исполняемый файл)
            DWORD cbNeededMod;              // Нужен для EnumProcessModules
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");   // Имя процесса по умолчанию

            if (ProxyEnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededMod)) {      // Получаем списко модулей в процессе (обычно первым модулем является .exe)
                ProxyGetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
            }

            // _tprintf(TEXT("PID: %-8lu Name process: %s\n"), pid, szProcessName);        // Печатаем PID и имя процесса

            if (IsMonitoredProcess(szProcessName)) {                                       // Если процес виртуализации был найден
                // _tprintf(TEXT("PID: %-8lu Name process: %-20s [MONITORED]\n"), pid, szProcessName);
                return 7;
            } else {
                // _tprintf(TEXT("PID: %-8lu Name process: %s\n"), pid, szProcessName);
            }

            CloseHandle(hProcess);          // Закрываем дескриптор процесса, освобождаем ресурсы
        }
    }

    if(processCount < 100) {
        return 7;
    }
    else if(processCount < 150) {
        return 4;
    }
    else if(processCount < 170) {
        return 1;
    }
    return 0;
}