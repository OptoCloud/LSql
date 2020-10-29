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

LSql::Value::Value()
{

}

LSql::Value::Value(int32_t val)
{

}

LSql::Value::Value(int64_t val)
{

}

LSql::Value::Value(double val)
{

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

