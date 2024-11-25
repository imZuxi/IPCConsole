/* 
    IPCConsole.cpp -> make for the skidies to have a console 
    https://github.com/imzuxi
    https://zuxi.dev
*/

#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <Windows.h>
std::thread ClientConsoleThread;
void clientReceverPipe() {
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;
    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\VConsole"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)
        {
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                /* add terminating zero */
                buffer[dwRead] = '\0';

                /* do something with data in buffer */
                printf("%s", buffer);
            }
        }

        DisconnectNamedPipe(hPipe);
        std::cout << "Disconnected." << std::endl;
    }

}
void main() {
    ClientConsoleThread = std::move(std::thread([=]() {clientReceverPipe(); return 1; }));
    std::cout << "[*] Created Console" << std::endl;
    std::cin.ignore();
}
