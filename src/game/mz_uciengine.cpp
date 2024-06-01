#include "mz_uciengine.h"

#include <windows.h>

MZUciEngine::MZUciEngine()
{
}

MZUciEngine::~MZUciEngine()
{
    DWORD write;
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
    // CloseHandle(pipeinRead);
    // CloseHandle(pipeoutWrite);
}

void MZUciEngine::setLevel(std::string lv)
{
    level = lv;
}

void MZUciEngine::setDepth(std::string dp)
{
    depth = dp;
}

std::string MZUciEngine::initialize()
{
    DWORD write, read;
    char buffer[1024];
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

        size_t readyPos = outputBuffer.find(UCIOK);
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

void MZUciEngine::startFindBestMove()
{
    bestMoveResponse = std::async(std::launch::async, &MZUciEngine::findBestMove, this);
}

std::string MZUciEngine::findBestMove()
{
    DWORD write, read;
    char buffer[4096];
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

void MZUciEngine::startSetLevel()
{
    setLevelResponse = std::async(std::launch::async, &MZUciEngine::setAILevel, this);
}

std::string MZUciEngine::setAILevel()
{
    DWORD write, read, bytesAvailable;
    char buffer[512];
    std::string outputBuffer;
    std::string out;

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

        size_t readyok = outputBuffer.find(READYOK);
        if (readyok != std::string::npos)
        {
            size_t endLine = outputBuffer.find("\n", readyok);
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

bool MZUciEngine::isReadySetLevel()
{
    return setLevelResponse.valid() && setLevelResponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::string MZUciEngine::getSetLevelResponse()
{
    return setLevelResponse.get();
}

void MZUciEngine::addMove(std::string m)
{
    moves.push_back(m);
}

bool MZUciEngine::checkDrawByRepeat()
{
    if (moves.size() < 6)
    {
        return false;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (moves[moves.size() - 1 - i] != moves[moves.size() - 4 - i])
        {
            return false;
        }
    }

    return true;
}

void MZUciEngine::clearMoves()
{
    moves.clear();
}