#include "httplib.h"
#include <iostream>
#include <string>

std::string serverIP;
int serverPort;

std::string getHighscores(std::string input)
{
    httplib::Client client(serverIP, serverPort);
    std::string req = "";

    size_t pos1 = input.find(';');
    req += input.substr(pos1 + 1);

    if (auto res = client.Post("/highscores", req, "text/plain"))
    {
        return res->body;
    }
    return "ERROR";
}

std::string postHighscore(std::string input)
{
    httplib::Client client(serverIP, serverPort);
    std::string req = "";

    size_t pos1 = input.find(';');
    size_t pos2 = input.find(';', pos1 + 1);
    size_t pos3 = input.find(';', pos2 + 1);

    req += input.substr(pos1 + 1, pos2 - pos1 - 1) + ";" + input.substr(pos2 + 1, pos3 - pos2 - 1) + ";" + input.substr(pos3 + 1);

    if (auto res = client.Post("/highscore", req, "text/plain"))
    {
        return res->body;
    }

    return "ERROR";
}

int main(int argc, char const *argv[])
{
    serverIP = argv[1];
    serverPort = std::stoi(argv[2]);
    std::string input;
    std::string output;

    while (true)
    {
        std::getline(std::cin, input);

        if (input.substr(0, 4) == "QUIT")
            break;

        else if (input.substr(0, 4) == "GETS")
        {
            output = getHighscores(input);
            std::cout << output.c_str() << "DONE" << std::endl;
        }
        else if (input.substr(0, 4) == "POST")
        {
            output = postHighscore(input);
            std::cout << output.c_str() << "DONE" << std::endl;
        }
        else
        {
            std::cout << "ERROR" << std::endl;
        }
    }

    return 0;
}