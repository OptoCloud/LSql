#ifndef LSQL_CONNECTION_H
#define LSQL_CONNECTION_H

#include <cstdint>
#include <string>

typedef struct sqlite3 sqlite3;

namespace LSql {
class Transaction;
class Query;
class Value;

class Connection
{
    friend LSql::Transaction;
    friend LSql::Query;
public:
    enum OpenMode : int
    {
        READONLY  = 1,
        READWRITE = 2,
        CREATE    = 4,
    };

    Connection(const char* apFilename, int aFlags = OpenMode::READONLY, int aBusyTimeoutMs = 0);
    ~Connection();

    bool isOpen() const;

    bool setBusyTimeout(const int aBusyTimeoutMs);

    Transaction transaction();

    Query query(const char* statement);
    bool execute(const char* statement);
    bool tableExists(const char* apTableName);

    std::int64_t lastInsertedRowId() const;

    const char* lastError() const;
private:
    sqlite3* m_db;
    Transaction* m_transaction;
};
}

#endif // LSQL_CONNECTION_H
