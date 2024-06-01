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

    void setLevel(std::string lv);
    void setDepth(std::string dp);

    std::string initialize();

    void startSetLevel();
    bool isReadySetLevel();
    std::string getSelLevelResponse();

    void startFindBestMove();
    bool isReadyBestMove();
    std::string getBestMoveResponse();

    std::vector<std::string> moves;

private:
    std::string findBestMove();
    std::string setAILevel();
    std::future<std::string> setLevelResponse;
    std::future<std::string> bestMoveResponse;
    std::string level = "0";
    std::string depth = "1";

    const std::string BR = "\n";
    const std::string UCI = "uci";
    const std::string UCIOK = "uciok";
    const std::string READY = "isready";
    const std::string READYOK = "readyok";

    const std::string SKILL_LEVEL = "setoption name Skill value ";
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
    LPSTR fileName = const_cast<LPSTR>("dragon.exe");
};

#endif