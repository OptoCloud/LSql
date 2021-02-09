#include "query.h"

#include <cstring>

#include "internal/sqlite3.h"

#include "connection.h"
#include "column.h"
#include "value.h"

LSql::Query::Query()
    : m_stmt(nullptr)
    , m_ncols(0)
{
}

LSql::Query::Query(const LSql::Query& other)
    : m_stmt(other.m_stmt)
    , m_ncols(other.m_ncols)
{
}

LSql::Query& LSql::Query::operator=(const LSql::Query& other)
{
    sqlite3_finalize(m_stmt);

    m_stmt = other.m_stmt;
    m_ncols = other.m_ncols;

    return *this;
}

LSql::Query::Query(const char *statement, int statementLen, LSql::Connection &connection)
{
    const char* tail;

    if (sqlite3_prepare_v2(connection.m_db, statement, statementLen, &m_stmt, &tail) != SQLITE_OK) {
        sqlite3_finalize(m_stmt);
        m_stmt = nullptr;
    }
}

LSql::Query::~Query()
{
    if (isValid()) {
        sqlite3_finalize(m_stmt);
    }
}

bool LSql::Query::isValid() const
{
    return m_stmt != nullptr;
}

bool LSql::Query::bind(int index, std::int32_t value)
{
    if (isValid()) {
        return sqlite3_bind_int(m_stmt, index, value) == SQLITE_OK;
    }

    return false;
}

bool LSql::Query::bind(int index, std::int64_t value)
{
    if (isValid()) {
        return sqlite3_bind_int64(m_stmt, index, value) == SQLITE_OK;
    }

    return false;
}

bool LSql::Query::bind(int index, double value)
{
    if (isValid()) {
        return sqlite3_bind_double(m_stmt, index, value) == SQLITE_OK;
    }

    return false;
}

bool LSql::Query::bind(int index, const LSql::Value &value)
{
    if (isValid()) {
        return sqlite3_bind_value(m_stmt, index, value.m_value) == SQLITE_OK;
    }

    return false;
}

bool LSql::Query::bindBlob(int index, const void* data, std::size_t size)
{
    if (isValid()) {
        return sqlite3_bind_blob(m_stmt, index, data, size, nullptr) == SQLITE_OK;
    }

    return false;
}

bool LSql::Query::bindString(int index, const char* data, std::size_t size)
{
    if (isValid()) {
        return sqlite3_bind_text(m_stmt, index, data, size, nullptr) == SQLITE_OK;
    }

    return false;
}

bool LSql::Query::step()
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

int LSql::Query::columnCount() const
{
    return m_ncols;
}

LSql::Type LSql::Query::getType(int col)
{
    if (isValid()) {
        return (LSql::Type)sqlite3_column_type(m_stmt, col);
    }

    return LSql::Type::Invalid;
}

LSql::Column LSql::Query::column(int col)
{
    return LSql::Column(this, col);
}

void LSql::Query::finalize()
{
    sqlite3_finalize(m_stmt);
    m_stmt = nullptr;
    m_ncols = 0;
}
