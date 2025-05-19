#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "proxy_funck.c"

int Check_Sleep()
{
    int time_sleep = 60000;

    struct timeval startr_time, end_time;               // Объявляем две переменные типа timeval для хранения времени
    long time_difference;                               // Переменная для разницы во времени

    gettimeofday(&startr_time, NULL);                   // Получем текущее время до вызова sleep
    ProxySleep(time_sleep);
    gettimeofday(&end_time, NULL);                      // Получем текущее время сразу после sleep

    time_difference = (end_time.tv_sec - startr_time.tv_sec);      // Разница в сек, умноженная на 1000 (переводим в ms)

    /*
    printf("time_difference: %ld\n", time_difference);
    printf("time_check: %ld\n", (time_sleep / 1000) - 1);
    */
    if (time_difference > (time_sleep / 1000) - 1) {               // Поскольку мы спали 60 sec, корректным временем считается более 59 сек.
        return 0;                                                  // получаем из mlsec->sec и вычитаем 1
    }
    else return 7;
}
