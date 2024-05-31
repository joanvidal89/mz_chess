#ifndef MZ_UCIENGINE
#define MZ_UCIENGINE

#include <vector>
#include <future>
#include <iostream>
#include <processthreadsapi.h>
#include <stdio.h>
#include <string>

class MZUciEngine
{
public:
    MZUciEngine();
    ~MZUciEngine();

    void launchProcess();

    void startInitialize();
    bool isReadyInitialize();
    std::string getAnswerInit();

    void startFindBestMove();
    bool isReadyBestMove();
    std::string getBestMoveResponse();

    std::string level;
    std::string depth;
    std::vector<std::string> moves;

private:
    std::string initialize();
    std::future<std::string> initializeResponse;
    std::string findBestMove();
    std::future<std::string> bestMoveResponse;

    const std::string BR = "\n";
    const std::string UCI = "uci";
    const std::string UCIOK = "uciok";
    const std::string READY = "isready";
    const std::string READYOK = "readyok";

    const std::string SKILL_LEVEL = "setoption name Skill Level value ";
    const std::string STARTPOS = "position startpos moves ";
    const std::string GODEPTH = "go depth ";
    const std::string BESTMOVE = "bestmove";

    STARTUPINFOA startupInfo = {0};
    SECURITY_ATTRIBUTES securityAttributes = {0};
    PROCESS_INFORMATION processInformation = {0};

    HANDLE pipeinWrite = NULL;
    HANDLE pipeinRead = NULL;
    HANDLE pipeoutWrite = NULL;
    HANDLE pipeoutRead = NULL;
    
    std::future<std::string> nextMove;
    LPSTR fileName = const_cast<LPSTR>("stockfish.exe");
};

#endif