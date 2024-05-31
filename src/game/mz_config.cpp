#include "mz_config.h"

MZConfig::MZConfig() {}

MZConfig::~MZConfig() {}

bool MZConfig::readConfig()
{
    try
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            currentConfig.ip = "localhost";
            currentConfig.port = "8080";
            currentConfig.video = 1;
            currentConfig.music = 1;
            currentConfig.audio = 1;
        }

        file >> currentConfig.ip >> currentConfig.port >> currentConfig.video >> currentConfig.music >> currentConfig.audio;
        file.close();
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}

bool MZConfig::writeConfig()
{
    try
    {
        std::ofstream file(filename, std::ios::out | std::ios::trunc);

        if (!file.is_open())
        {
            return false;
        }

        file << currentConfig.ip << std::endl;
        file << currentConfig.port << std::endl;
        file << currentConfig.video << std::endl;
        file << currentConfig.music << std::endl;
        file << currentConfig.audio << std::endl;
        file.close();
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}
