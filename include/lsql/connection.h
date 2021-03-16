#ifndef LSQL_CONNECTION_H
#define LSQL_CONNECTION_H

#include "query.h"

#include <string>
#include <memory>
#include <cstdint>

typedef struct sqlite3 sqlite3;

namespace SQLite {
class Transaction;

class Connection : public std::enable_shared_from_this<SQLite::Connection>
{
    friend SQLite::Transaction;
    friend SQLite::Query;

    Connection() = default;
    Connection(SQLite::Connection&) = default;
    Connection& operator=(const SQLite::Connection&) = default;
public:
    enum OpenMode : int
    {
        READONLY  = 1,
        READWRITE = 2,
        CREATE    = 4,
    };

    Connection(sqlite3* db);
    static std::shared_ptr<SQLite::Connection> OpenConnection(const char* filename, int flags = OpenMode::READONLY);
    ~Connection();

    bool setBusyTimeout(const int busyTimeoutMs);

    SQLite::Transaction beginDeferredTransaction();
    SQLite::Transaction beginImmediateTransaction();
    SQLite::Transaction beginExclusiveTransaction();

    SQLite::Query makeQuery(const char* statement, int statementLen);
    inline SQLite::Query makeQuery(std::string_view statement) { return makeQuery(statement.data(), (int)statement.size()); }
    inline SQLite::Query makeQuery(const std::string& statement) { return makeQuery(statement.data(), (int)statement.size()); }

    bool execute(const char* statement, int statementLen);
    inline bool execute(std::string_view statement) { return execute(statement.data(), (int)statement.size()); }
    inline bool execute(const std::string& statement) { return execute(statement.data(), (int)statement.size()); }

    bool tableExists(const char* tableName, int tableNameLen);
    inline bool tableExists(std::string_view tableName) { return tableExists(tableName.data(), (int)tableName.size()); }
    inline bool tableExists(const std::string& tableName) { return tableExists(tableName.data(), (int)tableName.size()); }

    int changes() const;
    std::int64_t lastInsertedRowId() const;

    const char* lastError() const;
private:
    sqlite3* m_db;
    SQLite::Transaction* m_transaction;
};
}

#endif // LSQL_CONNECTION_H
