#ifndef LSQL_COLUMN_H
#define LSQL_COLUMN_H

#include <cstdint>
#include <string>
#include <vector>

#include "type.h"

typedef struct sqlite3_stmt sqlite3_stmt;

namespace LSql {
class Query;
class Value;

class Column
{
    friend LSql::Query;
    friend LSql::Value;
public:
    Column(LSql::Query* query, int col);
    Column(const Column& other) = default;
    Column& operator=(const Column& other) = default;
    bool isValid() const;

    LSql::Type type() const;

    std::int32_t getInt();
    std::int64_t getInt64();
    double       getDouble();
    std::string  getText();
    std::vector<std::uint8_t> getBlob();

    LSql::Value getValue(); ///< Allocates a copy of the value at the row, making it independant from the Query
private:
    LSql::Query* m_query;
    int m_col;
};
}

#endif // LSQL_COLUMN_H
