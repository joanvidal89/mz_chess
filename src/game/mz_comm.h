#ifndef MZ_COMM
#define MZ_COMM

#include <future>
#include <iostream>
#include <processthreadsapi.h>
#include <stdio.h>
#include <string>

class MZComm
{
public:
    MZComm();
    ~MZComm();

    void launchProcess();

    void startRequestGET();
    void startRequestPOST();
    bool isReadyGET();
    bool isReadyPOST();
    std::string getAnswerGET();
    std::string getAnswerPOST();

    std::string address;
    std::string port;
    std::string accountId;
    std::string levelId;
    std::string score;

private:
    std::future<std::string> GETresponse;
    std::future<std::string> POSTresponse;

    SECURITY_ATTRIBUTES securityAttributes;
    STARTUPINFOA startupInfo = {sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION processInformation;
    HANDLE pipeinRead, pipeinWrite;
    HANDLE pipeoutRead, pipeoutWrite;

    char commandLine[256];
    std::string processName = "client.exe";
};

#endif