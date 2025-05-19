#include <windows.h>
#include <stdio.h>

#include "proxy_funck.c"

int Check_Mouse_Motion()
{
    int count = 0;

    POINT mouse_coordinate1 = {};
    POINT mouse_coordinate2 = {};

    ProxyGetCursorPos(&mouse_coordinate1);           // Получаем начальную позицию курсора

    ProxySleep(60000);

    ProxyGetCursorPos(&mouse_coordinate2);           // Получаем вторую позицию курсора

    if ((mouse_coordinate1.x == mouse_coordinate2.x) &&
        (mouse_coordinate1.y == mouse_coordinate2.y)) {
            return 7;
    }
    else return 0;
}
