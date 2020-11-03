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
    Value(const Value& other);
    Value& operator=(const Value& other);

    bool isValid() const;
    LSql::Type type() const;

    std::int32_t getInt();
    std::int64_t getInt64();
    double       getDouble();
    std::string  getText();
    std::vector<std::uint8_t> getBlob();
private:
    sqlite3_value* m_value;
};
}

#endif // LSQL_VALUE_H
