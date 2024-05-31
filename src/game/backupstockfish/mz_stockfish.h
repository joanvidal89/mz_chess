#ifndef MZ_STOCKFISH
#define MZ_STOCKFISH

#include <vector>
#include <future>
#include <iostream>
#include <processthreadsapi.h>
#include <stdio.h>
#include <string>

class MZStockfish
{
public:
    MZStockfish();
    ~MZStockfish();

    void launchProcess();

    std::string test();

    void startRequestInit();
    bool isReadyInit();
    std::string getAnswerInit();

    void startRequestMove();
    bool isReadyMove();
    std::string getAnswerMove();

    std::string level;
    std::string depth;
    std::vector<std::string> moves;

private:
    std::future<std::string> responseInit;
    std::future<std::string> responseMove;

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

    BYTE buffer[2048];

    DWORD read, write, exit_code, available;

    std::future<std::string> nextMove;
    LPSTR fileName = const_cast<LPSTR>("stockfish.exe");
};

#endif