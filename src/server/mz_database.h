#ifndef DATABASE_H
#define DATABASE_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "sqlite3.h"

#define DB_FILENAME "mzdatabase.db"

/**
 * @param string text
 * @param int status
 */
struct DBResponse
{
    std::string text;
    int status;
};

class MZDatabase
{
public:
    MZDatabase();
    ~MZDatabase();

    int initialize();

    int openDatabase();
    int closeDatabase();
    void resetPointers();

    int createDatabase();
    int createTable(const char *STMT);

    DBResponse insertAccount(std::string request);
    DBResponse updateSavedGame(std::string request);
    DBResponse insertHighscore(std::string request);

    DBResponse selectAccount(std::string request);
    DBResponse selectHint(std::string request);
    DBResponse selectHighscores(std::string request);
    DBResponse selectPosition(std::string request);

private:
    sqlite3 *db;
    char *errorText = 0;
    sqlite3_stmt *stmt;

    char *dbPath;
    void findDBpath();
    std::string getDateTime();

    const DBResponse DBR_OK = {"OK", 200};
    const DBResponse DBR_ERR_DATABASE = {"Database Error.", 403};
    const DBResponse DBR_ERR_UNIQUE = {"Username already in use.", 400};
    const DBResponse DBR_ERR_STATEMENT = {"Internal Server error.", 403};
    const DBResponse DBR_ERR_STEP = {"Internal Step Error.", 403};
    const DBResponse DBR_ERR_ACCOUNT = {"Wrong Username or Password.", 401};

    const char *STMT_CREATE_TABLE_ACCOUNT =
        "CREATE TABLE IF NOT EXISTS ACCOUNT("
        "   ID          INTEGER    PRIMARY KEY AUTOINCREMENT,"
        "   USERNAME    TEXT       UNIQUE NOT NULL,"
        "   PASSWORD    TEXT       NOT NULL,"
        "   HINT        TEXT       NOT NULL,"
        "   SAVEDGAME   TEXT       ,"
        "   DATE        TEXT       NOT NULL"
        ");";

    const char *STMT_CREATE_TABLE_HIGHSCORE =
        "CREATE TABLE IF NOT EXISTS HIGHSCORE("
        "   ID          INTEGER    PRIMARY KEY AUTOINCREMENT,"
        "   LEVEL       INTEGER    NOT NULL,"
        "   PLAYER_ID   INTEGER    NOT NULL,"
        "   SCORE       INTEGER    NOT NULL,"
        "   DATE        TEXT       NOT NULL"
        ");";

    const char *STMT_INSERT_ACCOUNT =
        "INSERT INTO ACCOUNT (USERNAME, PASSWORD, HINT, SAVEDGAME, DATE)"
        "VALUES(?1, ?2, ?3, NULL, ?4);";

    const char *STMT_INSERT_HIGHSCORE =
        "INSERT INTO HIGHSCORE (LEVEL, PLAYER_ID, SCORE, DATE)"
        "VALUES(?1, ?2, ?3, ?4);";

    const char *STMT_SELECT_ACCOUNT =
        "SELECT ID FROM ACCOUNT WHERE USERNAME = ?1 AND PASSWORD = ?2 LIMIT 1";

    const char *STMT_SELECT_HINT =
        "SELECT HINT FROM ACCOUNT WHERE USERNAME = ?1 LIMIT 1";

    const char *STMT_SELECT_RANK_BY_ID =
        "WITH OrderedHighscores AS (SELECT ROW_NUMBER() OVER (ORDER BY SCORE DESC, DATE) AS RANK, ID FROM HIGHSCORE WHERE LEVEL = ?1) "
        "SELECT RANK FROM OrderedHighscores WHERE ID = ?2";

    const char *STMT_SELECT_HIGHSCORES_LIMIT_EIGHT =
        "SELECT A.USERNAME, H.SCORE "
        "FROM HIGHSCORE H "
        "INNER JOIN ACCOUNT A ON A.ID = H.PLAYER_ID "
        "WHERE LEVEL = ? "
        "ORDER BY SCORE DESC "
        "LIMIT 8;";

    const char *STMT_SELECT_POSITION =
        "WITH OrderedHighscores AS(SELECT ROW_NUMBER() OVER (ORDER BY HS.SCORE DESC, HS.DATE) AS RANK, "
        "HS.ID, A.USERNAME, HS.SCORE, HS.DATE FROM ACCOUNT A INNER JOIN HIGHSCORE HS ON A.ID = HS.PLAYER_ID WHERE LEVEL = ?1) "
        "SELECT RANK, USERNAME, SCORE, DATE FROM OrderedHighscores WHERE RANK = (SELECT RANK FROM OrderedHighscores WHERE ID = ?2) - 1 "
        "UNION ALL SELECT RANK, USERNAME, SCORE, DATE FROM OrderedHighscores WHERE ID = ?2 "
        "UNION ALL SELECT RANK, USERNAME, SCORE, DATE FROM OrderedHighscores WHERE RANK = (SELECT RANK FROM OrderedHighscores WHERE ID = ?2) + 1";
};

#endif

/*
SELECT
(SELECT COUNT(DISTINCT SCORE) + 1 FROM HIGHSCORE WHERE LEVEL = 1 AND SCORE = 7118) AS RANK,
A.USERNAME,
HS.SCORE,
HS.DATE
FROM ACCOUNT A
INNER JOIN HIGHSCORE HS ON A.ID = HS.PLAYER_ID
WHERE HS.ID = 2;
*/