#ifndef LSQL_VALUE_H
#define LSQL_VALUE_H

#include <cstdint>
#include <string>
#include <vector>

#include "type.h"

typedef struct sqlite3_value sqlite3_value;

namespace LSql {
class Column;
class Query;

class Value
{
    friend LSql::Column;
    friend LSql::Query;

    Value(const sqlite3_value* value);
public:
    Value(); ///< Constructs a NULL value
    Value(std::int32_t val);
    Value(std::int64_t val);
    Value(double val);
    Value(const std::string& text);
    Value(const std::vector<std::uint8_t>& blob);
    Value(const Value& other);

    void null();
    Value& operator=(std::int32_t val);
    Value& operator=(std::int64_t val);
    Value& operator=(double val);
    Value& operator=(const std::string& text);
    Value& operator=(const std::vector<std::uint8_t>& blob);
    Value& operator=(const Value& other);

    bool isValid() const;

    LSql::Type type() const;

    int getInt();
    const char* getString();
private:
    sqlite3_value* m_value;
};
}

#endif // LSQL_VALUE_H
