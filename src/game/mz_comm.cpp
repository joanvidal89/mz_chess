#include "mz_comm.h"

#include <windows.h>

std::string serverIP;
int serverPort;

MZComm::MZComm()
{
}

MZComm::~MZComm()
{
    DWORD write;
    WriteFile(pipeinWrite, "QUIT\n", 5, &write, NULL);

    if (pipeinWrite != NULL)
        CloseHandle(pipeinWrite);
    if (pipeoutWrite != NULL)
        CloseHandle(pipeoutWrite);
    if (pipeinRead != NULL)
        CloseHandle(pipeinRead);
    if (pipeinWrite != NULL)
        CloseHandle(pipeinWrite);
    if (processInformation.hProcess != NULL)
        CloseHandle(processInformation.hProcess);
    if (processInformation.hThread != NULL)
        CloseHandle(processInformation.hThread);

    free(pipeinWrite);
    free(pipeinRead);
    free(pipeoutWrite);
    free(pipeoutRead);
}

void MZComm::launchProcess()
{
    securityAttributes.nLength = sizeof(securityAttributes);
    securityAttributes.bInheritHandle = TRUE;
    securityAttributes.lpSecurityDescriptor = NULL;

    CreatePipe(&pipeoutRead, &pipeoutWrite, &securityAttributes, 0);
    CreatePipe(&pipeinRead, &pipeinWrite, &securityAttributes, 0);

    startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    startupInfo.wShowWindow = SW_SHOW;
    startupInfo.hStdInput = pipeinRead;
    startupInfo.hStdOutput = pipeoutWrite;
    startupInfo.hStdError = pipeoutWrite;

    sprintf_s(commandLine, "%s %s %s", processName.c_str(), address.c_str(), port.c_str());

    CreateProcessA(
        NULL,               // [in, optional]      LPCSTR                lpApplicationName,
        commandLine,        // [in, out, optional] LPSTR                 lpCommandLine,
        NULL,               // [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
        NULL,               // [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
        TRUE,               // [in]                BOOL                  bInheritHandles,
        0,                  // [in]                DWORD                 dwCreationFlags,
        NULL,               // [in, optional]      LPVOID                lpEnvironment,
        NULL,               // [in, optional]      LPCSTR                lpCurrentDirectory,
        &startupInfo,       // [in]                LPSTARTUPINFOA        lpStartupInfo,
        &processInformation // [out]               LPPROCESS_INFORMATION lpProcessInformation
    );
}

void MZComm::startRequestGET()
{
    GETresponse = std::async(std::launch::async, [this]() -> std::string {
        DWORD write, read, available, exitCode;
        char buffer[512];
        std::string outputBuffer;

        std::string out = "GETS;" + levelId + "\n";

        if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
            return "ERROR";

        while (true)
        {
            if (!ReadFile(pipeoutRead, buffer, sizeof(buffer), &read, NULL))
                return "ERROR";

            outputBuffer.append(buffer, read);

            if (outputBuffer.find("ERROR") != std::string::npos)
                break;

            if (outputBuffer.find("DONE") != std::string::npos)
                break;

            memset(buffer, 0, sizeof(buffer));
            read = 0;
        }
        return outputBuffer;
    });
}

void MZComm::startRequestPOST()
{
    POSTresponse = std::async(std::launch::async, [this]() -> std::string {
        DWORD write, read, available, exitCode;
        char buffer[512];
        std::string outputBuffer;

        std::string out = "POST;" + accountId + ";" + levelId + ";" + score + "\n";

        if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
            return "ERROR";

        while (true)
        {
            if (!ReadFile(pipeoutRead, buffer, sizeof(buffer), &read, NULL))
                return "ERROR";

            outputBuffer.append(buffer, read);

            if (outputBuffer.find("ERROR") != std::string::npos)
                break;

            if (outputBuffer.find("DONE") != std::string::npos)
                break;

            memset(buffer, 0, sizeof(buffer));
            read = 0;
        }
        return outputBuffer;
    });
}

bool MZComm::isReadyGET()
{
    return GETresponse.valid() && GETresponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

bool MZComm::isReadyPOST()
{
    return POSTresponse.valid() && POSTresponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::string MZComm::getAnswerGET()
{
    return GETresponse.get();
}

std::string MZComm::getAnswerPOST()
{
    return POSTresponse.get();
}
