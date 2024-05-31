#ifndef MZ_SERVER
#define MZ_SERVER

#include <string>
#include "httplib.h"
#include "mz_config.h"
#include "mz_database.h"

using namespace httplib;

class MZServer
{

public:
    MZServer();
    ~MZServer();

    void run();

private:
    MZDatabase *db;
    MZConfig *config;

    std::string dump_headers(const Headers &headers);
    std::string log(const Request &req, const Response &res);

    char buf[BUFSIZ];
    
    int initialize();
};

#endif