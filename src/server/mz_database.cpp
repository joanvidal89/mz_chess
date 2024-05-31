#include "mz_database.h"

MZDatabase::MZDatabase() {}
MZDatabase::~MZDatabase() {}

int MZDatabase::initialize()
{
    findDBpath();

    if (createDatabase() != SQLITE_OK)
        return 1;
    else
        printf("MZDATABASE - OK\n");

    if (createTable(STMT_CREATE_TABLE_ACCOUNT) != SQLITE_OK)
        return 1;
    else
        printf("TABLE ACCOUNT - OK\n");

    if (createTable(STMT_CREATE_TABLE_HIGHSCORE) != SQLITE_OK)
        return 1;
    else
        printf("TABLE HIGHSCORE - OK\n");

    return 0;
}

int MZDatabase::openDatabase()
{
    if (sqlite3_open(dbPath, &db) != SQLITE_OK)
    {
        return 1;
    }
    return 0;
}

int MZDatabase::closeDatabase()
{
    sqlite3_close(db);
    return 0;
}

void MZDatabase::resetPointers()
{
    errorText = NULL;
    stmt = NULL;
}

int MZDatabase::createDatabase()
{
    FILE *file = fopen(dbPath, "r");

    if (file == NULL)
        file = fopen(dbPath, "w+b");
    else
        fclose(file);

    return 0;
}

int MZDatabase::createTable(const char *STMT)
{
    resetPointers();

    if (openDatabase() != SQLITE_OK)
        return 1;

    if (sqlite3_exec(db, STMT, NULL, 0, &errorText) != SQLITE_OK)
    {
        printf(errorText);
        sqlite3_free(errorText);
        return 1;
    }

    if (closeDatabase() != SQLITE_OK)
        return 1;

    return 0;
}

DBResponse MZDatabase::insertAccount(std::string request)
{
    resetPointers();
    openDatabase();

    if (sqlite3_prepare_v2(db, STMT_INSERT_ACCOUNT, -1, &stmt, NULL) != SQLITE_OK)
    {
        sqlite3_close(db);
        return DBR_ERR_STATEMENT;
    }

    size_t pos1 = request.find(';');
    size_t pos2 = request.find(';', pos1 + 1);

    std::string username = request.substr(0, pos1);
    std::string password = request.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string hint = request.substr(pos2 + 1);
    std::string date = getDateTime();

    // BANNED WORDS THAT AFFECT PARSING ANSWERS FOR THE CLIENT.
    if (username == "ERROR" || username == "DONE")
    {
        return DBR_ERR_ACCOUNT;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, hint.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_ACCOUNT;
    }
    sqlite3_reset(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return DBR_OK;
}

DBResponse MZDatabase::updateSavedGame(std::string request)
{
    return DBResponse();
}

DBResponse MZDatabase::insertHighscore(std::string request)
{
    DBResponse res = {"", 200};

    resetPointers();
    openDatabase();

    if (sqlite3_prepare_v2(db, STMT_INSERT_HIGHSCORE, -1, &stmt, NULL) != SQLITE_OK)
    {
        sqlite3_close(db);
        printf(sqlite3_errmsg(db));
        return DBR_ERR_STATEMENT;
    }

    size_t pos1 = request.find(';');
    size_t pos2 = request.find(';', pos1 + 1);

    int level = std::atoi(request.substr(0, pos1).c_str());
    int playerId = std::atoi(request.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
    int score = std::atoi(request.substr(pos2 + 1).c_str());
    std::string date = getDateTime();

    sqlite3_bind_int(stmt, 1, level);
    sqlite3_bind_int(stmt, 2, playerId);
    sqlite3_bind_int(stmt, 3, score);
    sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_STEP;
    }

    int insertedId = static_cast<int>(sqlite3_last_insert_rowid(db));

    sqlite3_reset(stmt);

    // SECOND PART OF THE INSERT, GETTING THE RANK
    if (sqlite3_prepare_v2(db, STMT_SELECT_RANK_BY_ID, -1, &stmt, NULL) != SQLITE_OK)
    {
        sqlite3_close(db);
        printf(sqlite3_errmsg(db));
        return DBR_ERR_STATEMENT;
    }
    sqlite3_bind_int(stmt, 1, level);
    sqlite3_bind_int(stmt, 2, insertedId);

    int stepResult = sqlite3_step(stmt);
    if (stepResult == SQLITE_ROW)
    {
        int rank = sqlite3_column_int(stmt, 0);
        res.text += std::to_string(rank);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    else if (stepResult == SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_ACCOUNT;
    }
    else
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_STEP;
    }

    return res;
}

DBResponse MZDatabase::selectAccount(std::string request)
{
    DBResponse res = {"OK:", 200};
    resetPointers();
    openDatabase();

    if (sqlite3_prepare_v2(db, STMT_SELECT_ACCOUNT, -1, &stmt, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return DBR_ERR_STATEMENT;
    }

    size_t pos1 = request.find(';');

    std::string username = request.substr(0, pos1);
    std::string password = request.substr(pos1 + 1);

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    int stepResult = sqlite3_step(stmt);
    if (stepResult == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        res.text += std::to_string(id);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return res;
    }
    else if (stepResult == SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_ACCOUNT;
    }
    else
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_STEP;
    }
}

DBResponse MZDatabase::selectHint(std::string request)
{
    DBResponse res = {"", 200};
    resetPointers();
    openDatabase();

    if (sqlite3_prepare_v2(db, STMT_SELECT_HINT, -1, &stmt, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return DBR_ERR_STATEMENT;
    }

    sqlite3_bind_text(stmt, 1, request.c_str(), -1, SQLITE_STATIC);

    int stepResult = sqlite3_step(stmt);
    if (stepResult == SQLITE_ROW)
    {
        const unsigned char *hint = sqlite3_column_text(stmt, 0);
        res.text = reinterpret_cast<const char *>(hint);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return res;
    }
    else if (stepResult == SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_ACCOUNT;
    }
    else
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return DBR_ERR_STEP;
    }
}

DBResponse MZDatabase::selectHighscores(std::string request)
{
    DBResponse res = {"", 200};
    int sqliteCode = -1;
    int level = std::stoi(request);

    resetPointers();
    openDatabase();

    if (sqlite3_prepare_v2(db, STMT_SELECT_HIGHSCORES_LIMIT_EIGHT, -1, &stmt, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return DBR_ERR_STATEMENT;
    }

    sqlite3_bind_int(stmt, 1, level);

    while ((sqliteCode = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char *player = sqlite3_column_text(stmt, 0);
        int score = sqlite3_column_int(stmt, 1);

        res.text += std::string("") + reinterpret_cast<const char *>(player) + ";" + std::to_string(score) + "\n";
    }

    if (sqliteCode != SQLITE_DONE)
    {
        return DBR_ERR_STEP;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return res;
}

DBResponse MZDatabase::selectPosition(std::string request)
{

    DBResponse res = {"", 200};
    int sqliteCode = -1;

    resetPointers();
    openDatabase();

    if (sqlite3_prepare_v2(db, STMT_SELECT_POSITION, -1, &stmt, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        printf(sqlite3_errmsg(db));
        return DBR_ERR_STATEMENT;
    }

    size_t pos1 = request.find(';');

    int level = std::atoi(request.substr(0, pos1).c_str());
    int score_id = std::atoi(request.substr(pos1 + 1).c_str());

    sqlite3_bind_int(stmt, 1, level);
    sqlite3_bind_int(stmt, 2, score_id);

    while ((sqliteCode = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int rank = sqlite3_column_int(stmt, 0);
        const unsigned char *username = sqlite3_column_text(stmt, 1);
        int score = sqlite3_column_int(stmt, 2);
        const unsigned char *date = sqlite3_column_text(stmt, 3);

        res.text += std::to_string(rank) + ";" + reinterpret_cast<const char *>(username) + ";" + std::to_string(score) + ";" + reinterpret_cast<const char *>(date) + "\n";
    }

    if (sqliteCode != SQLITE_DONE)
    {
        return DBR_ERR_STEP;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return res;
}

void MZDatabase::findDBpath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type size = std::string(buffer).find_last_of("\\/");
    std::string directoryString = std::string(buffer).substr(0, size + 1);
    directoryString += DB_FILENAME;

    char *directoryChar = new char[directoryString.length() + 1];
    strcpy(directoryChar, directoryString.c_str());

    dbPath = directoryChar;
}

std::string MZDatabase::getDateTime()
{
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << now->tm_mday << "/"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "/"
        << (now->tm_year + 1900);

    return oss.str();
}
