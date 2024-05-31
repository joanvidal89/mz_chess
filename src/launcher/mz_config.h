#ifndef MZ_CONFIG
#define MZ_CONFIG

#include <string>
#include "mz_struct.h"
#include <fstream>
#include <string>

class MZConfig {
public:
    MZConfig();
    ~MZConfig();

    bool readConfig();
    Config currentConfig;

private:
    const std::string filename = "mzchess.ini";
};

#endif