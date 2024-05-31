#ifndef MZ_CONFIG
#define MZ_CONFIG

#include <fstream>
#include <string>

#include "mz_struct.h"

class MZConfig
{
public:
    MZConfig();
    ~MZConfig();

    bool readConfig();
    bool writeConfig();
    Config currentConfig;

private:
    const std::string filename = "mzchess.ini";
};

#endif