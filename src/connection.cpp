#include "connection.h"

#include "internal/sqlite3.h"

#include "transaction.h"
#include "column.h"
#include "query.h"
#include "value.h"

LSql::Connection::Connection(const char* apFilename, /* ex: "file:database.db */
                                      int aFlags,             /* = OpenMode::READONLY  */
                                      int aBusyTimeoutMs      /* = 0                   */)
    : m_db(nullptr)
    , m_transaction(nullptr)
{
    if (apFilename != nullptr)
    {
        if (sqlite3_open_v2(apFilename, &m_db, aFlags, NULL) != SQLITE_OK) {
            sqlite3_close_v2(m_db);
            m_db = nullptr;
            return;
        }

        if (aBusyTimeoutMs > 0) {
            setBusyTimeout(aBusyTimeoutMs);
        }
    }
}

LSql::Connection::~Connection()
{
    if (m_transaction != nullptr) {
        m_transaction->rollback();
    }

    if (isOpen()) {
        sqlite3_close_v2(m_db);
    }
}

bool LSql::Connection::isOpen() const
{
    return m_db != nullptr;
}

bool LSql::Connection::setBusyTimeout(int aBusyTimeoutMs)
{
    if (isOpen()) {
        return sqlite3_busy_timeout(m_db, aBusyTimeoutMs);
    }

    return false;
}

LSql::Transaction LSql::Connection::transaction()
{
    return Transaction(*this);
}

LSql::Query LSql::Connection::query(const char* statement)
{
    return Query(statement, *this);
}

bool LSql::Connection::execute(const char* statement)
{
    return Query(statement, *this).step();
}

bool LSql::Connection::tableExists(const char* apTableName)
{
    Query query("SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?", *this);
    query.bindString(1, apTableName, strlen(apTableName));
    (void)query.step(); // Cannot return false, as the above query always return a result
    return (1 == query.column(0).getInt());
}

std::int64_t LSql::Connection::lastInsertedRowId() const
{
    if (isOpen()) {
        return sqlite3_last_insert_rowid(m_db);
    }

    return 0;
}

const char *LSql::Connection::lastError() const
{
    if (isOpen()) {
        return sqlite3_errmsg(m_db);
    }

    return "Database is not open!";
}
