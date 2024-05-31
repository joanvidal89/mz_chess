#include "mz_comm.h"
#include "httplib.h"

std::string serverIP;
int serverPort;

MZComm::MZComm() {}

MZComm::~MZComm() {}

void MZComm::setServer(std::string ip, int port)
{
    serverIP = ip;
    serverPort = port;
}

std::string requestHint(std::string username)
{
    httplib::Client client(serverIP, serverPort);
    std::string req = "";

    req += username;

    if (auto res = client.Post("/hint", req, "text/plain"))
    {
        return res->body;
    }

    return "404 SERVER NOT FOUND";
}

std::string requestLogin(std::string username, std::string password)
{
    httplib::Client client(serverIP, serverPort);
    std::string req = username + ";" + password;

    if (auto res = client.Post("/login", req, "text/plain"))
    {
        return res->body;
    }

    return "404 SERVER NOT FOUND";
}

std::string requestSignup(std::string username, std::string password, std::string hint)
{
    httplib::Client client(serverIP, serverPort);
    std::string req = username + ";" + password + ";" + hint;

    if (auto res = client.Post("/signup", req, "text/plain"))
    {
        return res->body;
    }

    return "404 SERVER NOT FOUND";
}

void MZComm::startHintRequest()
{
    hintResponse = std::async(std::launch::async, requestHint, currentUsername);
}

void MZComm::startLoginRequest()
{
    loginResponse = std::async(std::launch::async, requestLogin, currentUsername, currentPassword);
}

void MZComm::startSignupRequest()
{
    signupResponse = std::async(std::launch::async, requestSignup, currentUsername, currentPassword, currentHint);
}

bool MZComm::isReadyLoginRequest()
{
    return loginResponse.valid() && loginResponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

bool MZComm::isReadySignupRequest()
{
    return signupResponse.valid() && signupResponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

bool MZComm::isReadyHintRequest()
{
    return hintResponse.valid() && hintResponse.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::string MZComm::getLoginResponse()
{
    return loginResponse.get();
}

std::string MZComm::getSignupResponse()
{
    return signupResponse.get();
}

std::string MZComm::getHintResponse()
{
    return hintResponse.get();
}
