#include <windows.h>
#include <string>
#include <iostream>
#include "mz_execute.h"

int MZExecute::executeProcess(int arg1)
{
    std::string program = "game.exe";
    std::string commandLine = program + " " + std::to_string(arg1) + " 0";

    // Convert command line to a writable char array
    char *cmd = new char[commandLine.size() + 1];
    std::copy(commandLine.begin(), commandLine.end(), cmd);
    cmd[commandLine.size()] = '\0';

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // Initialize the STARTUPINFO structure
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process.
    if (!CreateProcessA(NULL,  // No module name (use command line)
                        cmd,   // Command line
                        NULL,  // Process handle not inheritable
                        NULL,  // Thread handle not inheritable
                        FALSE, // Set handle inheritance to FALSE
                        0,     // No creation flags
                        NULL,  // Use parent's environment block
                        NULL,  // Use parent's starting directory
                        &si,   // Pointer to STARTUPINFO structure
                        &pi)   // Pointer to PROCESS_INFORMATION structure
    )
    {
        std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
        delete[] cmd;
        return 1;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    delete[] cmd;

    return 0;
}