#include "mz_uciengine.h"

#include <windows.h>

MZStockfish::MZStockfish()
{
    level = "0";
    depth = "1";
}

MZStockfish::~MZStockfish()
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

void MZStockfish::launchProcess()
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
        std::cerr << "Failed to create STOCKFISH process." << std::endl;
    };
}

std::string MZStockfish::test()
{
    return "";
}

void MZStockfish::startRequestInit()
{
    responseInit = std::async(std::launch::async, [this]() -> std::string
                              {

        DWORD write, read, available, exitCode;
        char buffer[512];
        std::string outputBuffer;
        std::string out;

        out = UCI + BR;

        if (!WriteFile(pipeinWrite, out.c_str(), out.length(), &write, NULL))
            return "ERROR";

        while (true)
        {
            if (!ReadFile(pipeoutRead, buffer, sizeof(buffer), &read, NULL))
                return "ERROR";

            outputBuffer.append(buffer, read);

            if (outputBuffer.find(UCIOK) != std::string::npos)
                break;

            memset(buffer, 0, sizeof(buffer));
            read = 0;
        }

        outputBuffer = "";

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

            if (outputBuffer.find(READYOK) != std::string::npos)
                break;

            memset(buffer, 0, sizeof(buffer));
            read = 0;
        }

        return "OK"; });
}

bool MZStockfish::isReadyInit()
{
    return responseInit.valid() && responseInit.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::string MZStockfish::getAnswerInit()
{
    return responseInit.get();
}

void MZStockfish::startRequestMove()
{
    responseMove = std::async(std::launch::async, [this]() -> std::string
                              {

        DWORD write, read, available, exitCode;
        char buffer[512];
        std::string outputBuffer;
        std::string out;
        std::string bestMove;
        //std::string score = "0";

        out = STARTPOS;
        for (std::string move : moves)
        {
            out += move + " ";
        }
        out += BR;

        printf("REQUEST FOR AI: %s", out.c_str());

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

            size_t bestMovePos = outputBuffer.find("bestmove");
            if (bestMovePos != std::string::npos)
            {
                // Extract the best move from the outputBuffer
                size_t start = outputBuffer.find(" ", bestMovePos) + 1;
                bestMove = outputBuffer.substr(start);
                break;
            }

            memset(buffer, 0, sizeof(buffer));
            read = 0;
        }

        return bestMove;
    });
}

bool MZStockfish::isReadyMove()
{
    return responseMove.valid() && responseMove.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::string MZStockfish::getAnswerMove()
{
    return responseMove.get();
}