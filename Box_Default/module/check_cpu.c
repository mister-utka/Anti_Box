#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "proxy_funck.c"

int Get_CPU_Info(void* cpu_info)
{
    struct CPU_INFO {
        int cpu_count;
        int cpu_architecture;
    };

    SYSTEM_INFO sysInfo;
    ProxyGetSystemInfo(&sysInfo);

    /*
    printf("=== SYSTEM INFORMATION ===\n");
    printf("Processor Architecture: %u\n", sysInfo.wProcessorArchitecture);
    printf("Processor Level: %u\n", sysInfo.wProcessorLevel);
    printf("Processor Revision: %u\n", sysInfo.wProcessorRevision);
    printf("Number of Processors: %u\n", sysInfo.dwNumberOfProcessors);
    printf("Page Size: %u bytes\n", sysInfo.dwPageSize);
    printf("Minimum Application Address: 0x%p\n", sysInfo.lpMinimumApplicationAddress);
    printf("Maximum Application Address: 0x%p\n", sysInfo.lpMaximumApplicationAddress);
    printf("Active Processor Mask: 0x%p\n", sysInfo.dwActiveProcessorMask);
    printf("Allocation Granularity: %u bytes\n", sysInfo.dwAllocationGranularity);
    printf("Processor Type: %u\n", sysInfo.dwProcessorType);
    */

    struct CPU_INFO *info = (struct CPU_INFO*)cpu_info;         // Преобразуем указатель общего типа (void*) к струутуре CPU_INFO

    info->cpu_count = sysInfo.dwNumberOfProcessors;             // Кол-во логических процессоров в системе
    
    // архитектура:
    //    0 - x86
    //    5 - ARM
    //    6 - intel Itanium
    //    9 - x64 (AMD64)
    //    12 - ARM64
    info->cpu_architecture = sysInfo.wProcessorArchitecture;

    return 0;

}

int Check_CPU_Info()
{
    struct CPU_INFO {
        int cpu_count;
        int cpu_architecture;
    };

    struct CPU_INFO cpu_info;
    Get_CPU_Info(&cpu_info);

    // printf("CPU count: %d\n", cpu_info.cpu_count);
    // printf("CPU architecture : %d\n", cpu_info.cpu_architecture);

    if (cpu_info.cpu_architecture != 9) {
        // printf("The architecture is not supported!");
        exit(1);
    }

    if (cpu_info.cpu_count <= 1) {
        return 7;
    }
    else if (cpu_info.cpu_count <= 2) {
        return 4;
    }
    else if(cpu_info.cpu_count <= 4) {
        return 1;
    }
    else return 0;
}