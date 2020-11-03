#include "value.h"

#include <exception>

#include "internal/sqlite3.h"

LSql::Value::Value(const sqlite3_value* value)
    : m_value(nullptr)
{
    if (value != nullptr)
    {
        m_value = sqlite3_value_dup(value);
    }
}

LSql::Value::Value(const LSql::Value& other)
    : m_value(nullptr)
{
    if (other.isValid())
    {
        m_value = sqlite3_value_dup(other.m_value);
    }
}

LSql::Value& LSql::Value::operator=(const LSql::Value& other)
{
    if (isValid())
    {
        sqlite3_value_free(m_value);
        m_value = nullptr;
    }

    if (other.isValid())
    {
        m_value = sqlite3_value_dup(other.m_value);
    }

    return *this;
}

bool LSql::Value::isValid() const
{
    return m_value != nullptr;
}

LSql::Type LSql::Value::type() const
{
    if (isValid()) return (LSql::Type)sqlite3_value_type(m_value);
    return LSql::Type::Invalid;
}

std::int32_t LSql::Value::getInt()
{
    return sqlite3_value_int(m_value);
}
std::int64_t LSql::Value::getInt64()
{
    return sqlite3_value_int64(m_value);
}
double       LSql::Value::getDouble()
{
    return sqlite3_value_double(m_value);
}
std::string  LSql::Value::getText()
{
    const char* ptr = (const char*)sqlite3_value_blob(m_value);

    return std::string(ptr, ptr + sqlite3_value_bytes(m_value));
}
std::vector<std::uint8_t> LSql::Value::getBlob()
{
    const uint8_t* ptr = (const std::uint8_t*)sqlite3_value_blob(m_value);

    return std::vector<std::uint8_t>(ptr, ptr + sqlite3_value_bytes(m_value));
}
