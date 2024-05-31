#include "mz_config.h"

MZConfig::MZConfig() {}

MZConfig::~MZConfig() {}

bool MZConfig::readConfig()
{
    try
    {
        std::ifstream file(DB_FILENAME);
        if (!file.is_open())
        {
            currentConfig.ip = "localhost";
            currentConfig.port = 8082;
        }

        file >> currentConfig.ip >> currentConfig.port;
        file.close();

        printf("MZCHESS.INI CONFIG %s:%i\n", currentConfig.ip.c_str(), currentConfig.port);
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}