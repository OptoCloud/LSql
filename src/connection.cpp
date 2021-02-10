#include "connection.h"

#include "internal/sqlite3.h"

#include "transaction.h"
#include "column.h"
#include "query.h"

#include <string_view>

SQLite::Connection::Connection(sqlite3* db)
    : m_db(db)
    , m_transaction(nullptr)
{

}

std::shared_ptr<SQLite::Connection> SQLite::Connection::OpenConnection(const char* filename, int flags)
{
    sqlite3* db;
    if (sqlite3_open_v2(filename, &db, flags, nullptr) != SQLITE_OK) {
        sqlite3_close_v2(db);
        return nullptr;
    }
    return std::make_shared<SQLite::Connection>(db);
}

SQLite::Connection::~Connection()
{
    if (m_transaction != nullptr) {
        m_transaction->rollback();
    }

    sqlite3_close_v2(m_db);
}

bool SQLite::Connection::setBusyTimeout(int busyTimeoutMs)
{
    return sqlite3_busy_timeout(m_db, busyTimeoutMs);
}

SQLite::Transaction SQLite::Connection::beginDeferredTransaction()
{
    return SQLite::Transaction(shared_from_this(), SQLite::Transaction::Behavior::DEFERRED);
}

SQLite::Transaction SQLite::Connection::beginImmediateTransaction()
{
    return SQLite::Transaction(shared_from_this(), SQLite::Transaction::Behavior::IMMEDIATE);
}

SQLite::Transaction SQLite::Connection::beginExclusiveTransaction()
{
    return SQLite::Transaction(shared_from_this(), SQLite::Transaction::Behavior::EXCLUSIVE);
}

SQLite::Query SQLite::Connection::makeQuery(const char* statement, int statementLen)
{
    return SQLite::Query(shared_from_this(), statement, statementLen);
}

bool SQLite::Connection::execute(const char* statement, int statementLen)
{
    return makeQuery(statement, statementLen).step();
}

bool SQLite::Connection::tableExists(const char* tableName, int tableNameLen)
{
    using namespace std::literals;
    auto query = makeQuery("SELECT count(*) FROM LSQL_master WHERE type='table' AND name=?"sv);
    query.bindText(1, tableName, tableNameLen);
    (void)query.step(); // Cannot return false, as the above query always return a result
    return (query.column(0).getInt64() > 0);
}

std::int64_t SQLite::Connection::lastInsertedRowId() const
{
    return sqlite3_last_insert_rowid(m_db);
}

const char *SQLite::Connection::lastError() const
{
    return sqlite3_errmsg(m_db);
}
