#pragma once
#include "../sqlite/sqlite3.h"
#include <iostream>
#include <string>
#include <vector>

namespace NDataBase {
    sqlite3* db = nullptr;
    std::vector<std::map<std::string, std::string>> values = {};

    static int callback(void *notUsed, int argc, char **argv, char **columnName) {
        std::map<std::string, std::string> curvals;
        for (int i = 0; i < argc; i++) {
            curvals[std::string(columnName[i])] = std::string(argv[i] ? argv[i] : "NULL");
        }
        values.push_back(curvals);
        return 0;
    }

    void Close() {
        sqlite3_close(db);
    }

    void Open(const std::string path) {
        if (sqlite3_open(path.c_str(), &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            Close();
        }
    }

    std::vector<std::map<std::string, std::string>> Query(const std::string& query) {
        values.clear();
        char *zErrMsg = 0;
        if (sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg) != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        return values;
    }

    int64_t GetLastInsertID() {
        return sqlite3_last_insert_rowid(db);
    }
};
