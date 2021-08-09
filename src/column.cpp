#include "column.h"

#include "sqlite3.h"

#include "query.h"

SQLite::Column::Column(SQLite::Query* query, int col)
    : m_query(query)
    , m_col(col)
{
}

bool SQLite::Column::isValid() const
{
    return m_query != nullptr
        && m_query->m_stmt != nullptr
        && m_col <= m_query->m_ncols;
}

SQLite::Type SQLite::Column::type() const
{
    if (isValid())
    {
        return (SQLite::Type)sqlite3_column_type(m_query->m_stmt, m_col);
    }
    return SQLite::Type::Invalid;
}

std::int32_t SQLite::Column::getInt32()
{
    return sqlite3_column_int(m_query->m_stmt, m_col);
}

std::int64_t SQLite::Column::getInt64()
{
    return sqlite3_column_int64(m_query->m_stmt, m_col);
}

double SQLite::Column::getDouble()
{
    return sqlite3_column_double(m_query->m_stmt, m_col);
}

const char* SQLite::Column::getDataText()
{
    return (const char*)sqlite3_column_text(m_query->m_stmt, m_col);
}

const void* SQLite::Column::getDataBlob()
{
    return sqlite3_column_blob(m_query->m_stmt, m_col);
}

int SQLite::Column::getDataSize()
{
    return sqlite3_column_bytes(m_query->m_stmt, m_col);
}
