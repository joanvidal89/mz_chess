#ifndef MZ_CONFIG
#define MZ_CONFIG

#include <string>
#include <fstream>
#include <string>

/**
 * @param string ip
 * @param int port
*/
struct Config
{
    std::string ip;
    int port;
};

class MZConfig
{
public:
    MZConfig();
    ~MZConfig();

    bool readConfig();
    Config currentConfig;

    const std::string TITLE =
        "===============================================================================\n"
        "   @@@@@@@@@@   @@@@@@@@      @@@@@@@  @@@  @@@  @@@@@@@@   @@@@@@    @@@@@@   \n"
        "   @@@@@@@@@@@  @@@@@@@@     @@@@@@@@  @@@  @@@  @@@@@@@@  @@@@@@@   @@@@@@@   \n"
        "   @@! @@! @@!       @@!     !@@       @@!  @@@  @@!       !@@       !@@       \n"
        "   !@! !@! !@!      !@!      !@!       !@!  @!@  !@!       !@!       !@!       \n"
        "   @!! !!@ @!@     @!!       !@!       @!@!@!@!  @!!!:!    !!@@!!    !!@@!!    \n"
        "   !@!   ! !@!    !!!        !!!       !!!@!!!!  !!!!!:     !!@!!!    !!@!!!   \n"
        "   !!:     !!:   !!:         :!!       !!:  !!!  !!:            !:!       !:!  \n"
        "   :!:     :!:  :!:          :!:       :!:  !:!  :!:           !:!       !:!   \n"
        "   :::     ::    :: ::::      ::: :::  ::   :::   :: ::::  :::: ::   :::: ::   \n"
        "    :      :    : :: : :      :: :: :   :   : :  : :: ::   :: : :    :: : :    \n"
        "===============================================================================\n";

private:
    const std::string DB_FILENAME = "mzchess.ini";
};

#endif