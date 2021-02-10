#ifndef LSQL_COLUMN_H
#define LSQL_COLUMN_H

#include <cstdint>
#include <string>
#include <vector>

#include "type.h"

typedef struct sqlite3_stmt sqlite3_stmt;

namespace SQLite {
class Query;

class Column
{
    friend SQLite::Query;
public:
    Column(SQLite::Query* query, int col);

    bool isValid() const;

    SQLite::Type type() const;

    std::int32_t getInt32();
    std::int64_t getInt64();
    double       getDouble();
    const char*  getDataText();
    const void*  getDataBlob();
    int          getDataSize();
private:
    SQLite::Query* m_query;
    int m_col;
};
}

#endif // LSQL_COLUMN_H
