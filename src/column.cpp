#include "column.h"

#include "internal/sqlite3.h"

#include "query.h"
#include "value.h"

LSql::Column::Column(LSql::Query* query, int col)
    : m_query(query)
    , m_col(col)
{
}

bool LSql::Column::isValid() const
{
    return m_query != nullptr
        && m_query->m_stmt != nullptr
        && m_col <= m_query->m_ncols;
}

LSql::Type LSql::Column::type() const
{
    if (isValid())
    {
        return (LSql::Type)sqlite3_column_type(m_query->m_stmt, m_col);
    }
    return LSql::Type::Invalid;
}

std::int32_t LSql::Column::getInt()
{
    return sqlite3_column_int(m_query->m_stmt, m_col);
}

std::int64_t LSql::Column::getInt64()
{
    return sqlite3_column_int64(m_query->m_stmt, m_col);
}

double LSql::Column::getDouble()
{
    return sqlite3_column_double(m_query->m_stmt, m_col);
}

std::string LSql::Column::getText()
{
    const char* ptr = (const char*)sqlite3_column_blob(m_query->m_stmt, m_col);

    return std::string(ptr, ptr + sqlite3_column_bytes(m_query->m_stmt, m_col));
}

std::vector<std::uint8_t> LSql::Column::getBlob()
{
    const std::uint8_t* ptr = (const std::uint8_t*)sqlite3_column_blob(m_query->m_stmt, m_col);

    return std::vector<std::uint8_t>(ptr, ptr + sqlite3_column_bytes(m_query->m_stmt, m_col));
}

LSql::Value LSql::Column::getValue()
{
    return LSql::Value(sqlite3_column_value(m_query->m_stmt, m_col));
}
