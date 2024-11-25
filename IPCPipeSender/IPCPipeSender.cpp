/*
    IPCPipeSender.cpp -> make for the skidies to have a console
    https://github.com/imzuxi
    https://zuxi.dev
*/


#include <iostream>
#include <string>
#include <Windows.h>

HANDLE hPipe = INVALID_HANDLE_VALUE; 

bool OpenPipe(const std::string& pipeName) {
    if (hPipe != INVALID_HANDLE_VALUE) {
        return true;
    }

    // Open the named pipe with write access only
    hPipe = CreateFileA(
        pipeName.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open named pipe. Error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

bool SendMessage(const std::string& pipeName, const std::string& message) {
    if (!OpenPipe(pipeName)) {
        return false;
    }

    DWORD bytesWritten;
    if (!WriteFile(hPipe, message.c_str(), message.size(), &bytesWritten, nullptr)) {
        std::cerr << "Failed to write to pipe. Error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

void ClosePipe() {
    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE; 
    }
}

int main() {
    const std::string pipeName = "\\\\.\\pipe\\VConsole";
    SendMessage(pipeName, "Hello World!\n");


    // Finally, when done with communication, close the pipe
    ClosePipe();

    return 0;
}
