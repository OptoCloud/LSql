#include "query.h"

#include <cstring>

#include "internal/sqlite3.h"

#include "connection.h"
#include "column.h"

SQLite::Query::Query(std::shared_ptr<SQLite::Connection> connection, const char *statement, int statementLen)
{
    const char* tail;

    if (sqlite3_prepare_v2(connection->m_db, statement, statementLen, &m_stmt, &tail) != SQLITE_OK) {
        sqlite3_finalize(m_stmt);
        m_stmt = nullptr;
    }
}

SQLite::Query::~Query()
{
    sqlite3_finalize(m_stmt);
}

bool SQLite::Query::isValid() const
{
    return m_stmt != nullptr;
}

bool SQLite::Query::bindInt32(int index, std::int32_t value)
{
    if (isValid()) {
        return sqlite3_bind_int(m_stmt, index, value) == SQLITE_OK;
    }

    return false;
}

bool SQLite::Query::bindInt64(int index, std::int64_t value)
{
    if (isValid()) {
        return sqlite3_bind_int64(m_stmt, index, value) == SQLITE_OK;
    }

    return false;
}

bool SQLite::Query::bindReal(int index, double value)
{
    if (isValid()) {
        return sqlite3_bind_double(m_stmt, index, value) == SQLITE_OK;
    }

    return false;
}

bool SQLite::Query::bindBlob(int index, const void* data, std::size_t size)
{
    if (isValid()) {
        return sqlite3_bind_blob(m_stmt, index, data, size, nullptr) == SQLITE_OK;
    }

    return false;
}

bool SQLite::Query::bindText(int index, const char* data, std::size_t size)
{
    if (isValid()) {
        return sqlite3_bind_text(m_stmt, index, data, size, nullptr) == SQLITE_OK;
    }

    return false;
}

bool SQLite::Query::step()
{
    if (isValid()) {
        int ret = sqlite3_step(m_stmt);

        if (ret == SQLITE_ROW) {
            m_ncols = sqlite3_column_count(m_stmt);
            return true;
        }

        m_ncols = 0;

        if (ret == SQLITE_DONE) {
            return true;
        }
    }

    m_ncols = 0;
    return false;
}

int SQLite::Query::columnCount() const
{
    return m_ncols;
}

SQLite::Type SQLite::Query::getType(int col)
{
    if (isValid()) {
        return (SQLite::Type)sqlite3_column_type(m_stmt, col);
    }

    return SQLite::Type::Invalid;
}

SQLite::Column SQLite::Query::column(int col)
{
    return SQLite::Column(this, col);
}

void SQLite::Query::finalize()
{
    sqlite3_finalize(m_stmt);
    m_stmt = nullptr;
    m_ncols = 0;
}
