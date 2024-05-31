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
            currentConfig.port = 8082;
        }

        file >> currentConfig.ip >> currentConfig.port;
        file.close();
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}