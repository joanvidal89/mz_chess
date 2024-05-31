#include "mz_uciengine.h"

#include <windows.h>

MZUciEngine::MZUciEngine()
{
    level = "0";
    depth = "5";
}

MZUciEngine::~MZUciEngine()
{
    WriteFile(pipeinWrite, "quit\n", 5, &write, NULL);

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

    free(fileName);
    free(pipeinWrite);
    free(pipeinRead);
    free(pipeoutWrite);
    free(pipeoutRead);
}

void MZUciEngine::launchProcess()
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

    if (!CreateProcessA(
            NULL,               // [in, optional]      LPCSTR                lpApplicationName,
            fileName,           // [in, out, optional] LPSTR                 lpCommandLine,
            NULL,               // [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
            NULL,               // [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
            TRUE,               // [in]                BOOL                  bInheritHandles,
            0,                  // [in]                DWORD                 dwCreationFlags,
            NULL,               // [in, optional]      LPVOID                lpEnvironment,
            NULL,               // [in, optional]      LPCSTR                lpCurrentDirectory,
            &startupInfo,       // [in]                LPSTARTUPINFOA        lpStartupInfo,
            &processInformation // [out]               LPPROCESS_INFORMATION lpProcessInformation
            ))
    {
        std::cerr << "Failed to create UCI ENGINE process." << std::endl;
    };

    // CLOSE UNNECESARY HANDLES
    CloseHandle(pipeinRead);
    CloseHandle(pipeoutWrite);
}

void MZUciEngine::startInitialize()
{
    initializeResponse = std::async(std::launch::async, &MZUciEngine::initialize, this);
}

std::string MZUciEngine::initialize()
{

    DWORD write, read, available, exitCode;
    char buffer[1024];
    std::string outputBuffer;
    std::string out;

    out = UCI + BR;

    if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
        return "ERROR";

    out = SKILL_LEVEL + level + BR;
    if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
        return "ERROR";

    out = READY + BR;

    if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
        return "ERROR";

    while (true)
    {
        if (!ReadFile(pipeoutRead, buffer, sizeof(buffer), &read, NULL))
            return "ERROR";

        outputBuffer.append(buffer, read);

        size_t readyPos = outputBuffer.find(READYOK);
        if (readyPos != std::string::npos)
        {
            size_t endLine = outputBuffer.find("\n", readyPos);
            if (endLine != std::string::npos)
            {
                break;
            }
        }

        memset(buffer, 0, sizeof(buffer));
        read = 0;
    }

    return "OK";
}

bool MZUciEngine::isReadyInitialize()
{
    return initializeResponse.valid() && initializeResponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::string MZUciEngine::getAnswerInit()
{
    return initializeResponse.get();
}

void MZUciEngine::startFindBestMove()
{
    bestMoveResponse = std::async(std::launch::async, &MZUciEngine::findBestMove, this);
}

std::string MZUciEngine::findBestMove()
{
    DWORD write, read, available, exitCode;
    char buffer[2048];
    std::string outputBuffer;
    std::string out;
    std::string bestMove;
    // std::string score = "0";

    out = STARTPOS;
    for (std::string move : moves)
    {
        out += move + " ";
    }
    out += BR;

    if (out.size() > 4096)
    {
        printf("ERROR OUT SIZE IN PROCESS");
        return "ERROR";
    }

    if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
        return "ERROR";

    out = GODEPTH + depth + BR;

    if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
        return "ERROR";

    while (true)
    {
        if (!ReadFile(pipeoutRead, buffer, sizeof(buffer), &read, NULL))
            return "ERROR";

        outputBuffer.append(buffer, read);

        size_t bestMovePos = outputBuffer.find(BESTMOVE);
        if (bestMovePos != std::string::npos)
        {
            size_t endLine = outputBuffer.find("\n", bestMovePos);
            if (endLine != std::string::npos)
            {
                size_t start = outputBuffer.find(" ", bestMovePos) + 1;
                bestMove = outputBuffer.substr(start);
                break;
            }
        }

        memset(buffer, 0, sizeof(buffer));
        read = 0;
    }

    return bestMove;
}

bool MZUciEngine::isReadyBestMove()
{
    return bestMoveResponse.valid() && bestMoveResponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::string MZUciEngine::getBestMoveResponse()
{
    return bestMoveResponse.get();
}