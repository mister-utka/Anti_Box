#include <windows.h>

#include "check_box.c"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    HWND hwnd_2 = GetConsoleWindow();
    ShowWindow(hwnd_2, SW_HIDE);        // Скрываем консоль после запуска

    // Register the window class.
    const char CLASS_NAME[]  = "Sample Window Class";
    
    WNDCLASS wc = {0};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "Learn to Program Windows",     // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

	// делаем окно невидимым закоментировав
    //ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);

    // Run the message loop.

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        {
            Check_Box();
            // int a;
            // for(;;) {
            //     srand(time(NULL));                          // Инициализация генератора случайных чисел текущим временем
            //     int random_number = (rand() % 100) + 1;     // Генерация числа от 1 до 100
            //     a += random_number;
            // }
            SelfDestruction();
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
