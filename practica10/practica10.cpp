#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h> 

volatile UINT count; 
bool start = false; 
void output_message() 
{
    for (;;)
    {
        count++; 
        Sleep(900);
        _cprintf("count = %d\n", count); 
    }
}

void thread() 
{
    for (;;)
    {
        Sleep(800); 
        _cprintf("COUNT * 2 = %d\n", count * 2); 
    }
}

WCHAR lpszHandle[20];
STARTUPINFO si;
PROCESS_INFORMATION pi;
HANDLE hThread, hThreadSecond;
DWORD IDThread, IDThreadSecond;

int main()
{
    setlocale(LC_ALL, "ru"); 
    printf("Нажмите '1', чтобы запустить первый поток.\n");
    printf("Нажмите '2', чтобы запустить второй поток.\n");
    printf("Нажмите '3', чтобы открыть Notepad.\n");
    printf("Нажмите '4', чтобы завершить Notepad.\n");
    printf("Нажмите клавишу '0' для выхода.\n\n\n");
    Sleep(500);
    while (true)
    {
        char c = _getch(); 
        switch (c)
        {
        case '3':
            if (start != false)
            {
                ZeroMemory(&si, sizeof(STARTUPINFO));
                si.cb = sizeof(STARTUPINFO);
                wsprintf(lpszHandle, L""); 
                WCHAR lpszNotepadPath[MAX_PATH] = L"C:\\Windows\\System32\\notepad.exe";
                if (!CreateProcess(
                    lpszNotepadPath, 
                    NULL,
                    NULL,
                    NULL,
                    TRUE,
                    CREATE_NEW_CONSOLE,
                    NULL,
                    NULL,
                    &si,
                    &pi
                ))
                {
                    printf("Не удалось запустить процесс Notepad.\n");
                    printf("Нажмите любую клавишу для выхода.\n");
                    _getch();
                    return GetLastError();
                }
                printf("Notepad успешно открыт!\n");
                start = false;
            }
            else
            {
                printf("Запустите первый поток с '1' и попробуйте снова.\n");
            }
            break;
        case '4':
            printf("Завершение Notepad...\n");
            TerminateProcess(pi.hProcess, 1);
            printf("Notepad завершен!\n");
            break;
        case '1':
            if (hThread == NULL)
            {
                start = true;
                printf("Запуск потока count++:\n");
                hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)output_message, NULL,
                    0, &IDThread);
            }
            else
            {
                printf("Поток count++ уже запущен!\n");
            }
            break;
        case '2':
            if (hThreadSecond == NULL)
            {
                printf("Запуск потока, умножающего count на 2:\n");
                hThreadSecond = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL,
                    0, &IDThreadSecond);
            }
            else
            {
                printf("Поток, умножающий count на 2, уже запущен!\n");
            }
            break;
        case '0':
            printf("Программа завершена.\n");
            TerminateProcess(pi.hProcess, 1);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            CloseHandle(hThread);
            CloseHandle(hThreadSecond);
            return 0;
        }
    }
}
