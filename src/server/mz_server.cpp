#include "mz_server.h"

MZServer::MZServer() {}

MZServer::~MZServer() {}

void MZServer::run()
{
    initialize();

    Server server;

    // PING
    server.Get("/hi", [](const Request & /*req*/, Response &res)
               { res.set_content("HI", "text/plain"); });

    // AUTH - CREATE NEW ACCOUNT
    server.Post("/signup", [&](const httplib::Request &req, httplib::Response &res)
                {
                    DBResponse dbres = db->insertAccount(req.body);
                    res.status = dbres.status;
                    res.set_content(dbres.text, "text/plain"); });

    // AUTH - EXISTS ACCOUNT BY USERNAME/PASSWORD
    server.Post("/login", [&](const httplib::Request &req, httplib::Response &res)
                {
                    DBResponse dbres = db->selectAccount(req.body);
                    res.status = dbres.status;
                    res.set_content(dbres.text, "text/plain"); });

    // AUTH - RETURN HINT BASED ON USERNAME
    server.Post("/hint", [&](const httplib::Request &req, httplib::Response &res)
                {
                    DBResponse dbres = db->selectHint(req.body);
                    res.status = dbres.status;
                    res.set_content(dbres.text, "text/plain"); });

    // HIGHSCORES - POST HIGHSCORE
    server.Post("/highscore", [&](const Request &req, Response &res)
                { 
                DBResponse dbres = db->insertHighscore(req.body);
                res.status = dbres.status;
                res.set_content(dbres.text, "text/plain"); });

    // HIGHSCORES - POSITION
    server.Post("/position", [&](const Request &req, Response &res)
                { 
                DBResponse dbres = db->selectPosition(req.body);
                res.status = dbres.status;
                res.set_content(dbres.text, "text/plain"); });

    // HIGHSCORES - GET TOP 10 SCORES BY LEVEL
    server.Post("/highscores", [&](const Request &req, Response &res)
               { 
                DBResponse dbres = db->selectHighscores(req.body);
                res.status = dbres.status;
                res.set_content(dbres.text, "text/plain"); });

    // SET HTTPLIB LOGGER OUTPUT
    server.set_logger([&](const Request &req, const Response &res)
                      { printf("%s", log(req, res).c_str()); });

    // SERVER LISTEN
    printf("SERVER - OK\n");
    printf("===============================================================================\n");
    server.listen(config->currentConfig.ip, config->currentConfig.port);
}

std::string MZServer::dump_headers(const Headers &headers)
{
    std::string s;
    char buf[BUFSIZ];

    for (auto it = headers.begin(); it != headers.end(); ++it)
    {
        const auto &x = *it;
        snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
        s += buf;
    }

    return s;
}

std::string MZServer::log(const Request &req, const Response &res)
{
    std::string s;
    char buf[BUFSIZ];

    s += "================================\n";

    snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
             req.version.c_str(), req.path.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it)
    {
        const auto &x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
                 (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
                 x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    s += buf;

    s += dump_headers(req.headers);

    s += "--------------------------------\n";

    snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
    s += buf;
    s += dump_headers(res.headers);
    s += "\n";

    if (!res.body.empty())
    {
        s += res.body;
    }

    s += "\n";

    return s;
}

int MZServer::initialize()
{
    config = new MZConfig();
    printf(config->TITLE.c_str());
    config->readConfig();

    db = new MZDatabase();
    db->initialize();

    return 0;
}
