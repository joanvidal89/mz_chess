#ifndef MZ_COMM
#define MZ_COMM

#include <string>
#include <future>

class MZComm
{
public:
    MZComm();
    ~MZComm();

    void setServer(std::string ip, int port);

    void startHintRequest();
    void startLoginRequest();
    void startSignupRequest();
    bool isReadyHintRequest();
    bool isReadyLoginRequest();
    bool isReadySignupRequest();
    std::string getHintResponse();
    std::string getLoginResponse();
    std::string getSignupResponse();

    std::string currentUsername;
    std::string currentPassword;
    std::string currentHint;

private:
    std::future<std::string> loginResponse;
    std::future<std::string> signupResponse;
    std::future<std::string> hintResponse;
};

#endif