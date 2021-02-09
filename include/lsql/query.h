#ifndef LSQL_QUERY_H
#define LSQL_QUERY_H

#include "type.h"

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

typedef struct sqlite3_stmt sqlite3_stmt;

namespace LSql {
class Column;
class Connection;
class Value;

class Query
{
    friend Column;
    friend Connection;

    Query();
    Query(const Query& other);
    Query& operator=(const LSql::Query& other);
public:
    Query(const char* statement, int statementLen, LSql::Connection& connection);
    inline Query(const std::string& statement, LSql::Connection& connection) : Query(statement.data(), (int)statement.size(), connection){};
    inline Query(const char* statement, LSql::Connection& connection) : Query(statement, strlen(statement), connection) {}
    ~Query();

    bool isValid() const;

    bool bind(int index, std::int32_t value);
    bool bind(int index, std::int64_t value);
    bool bind(int index, double value);
    bool bind(int index, const std::string& value);
    bool bind(int index, const std::vector<std::uint8_t>& value);
    bool bind(int index, const Value& value);

    bool step();

    int columnCount() const;
    LSql::Type getType(int col);
    LSql::Column column(int col);

    void finalize();
private:
    sqlite3_stmt* m_stmt;
    int m_ncols;
};
}

#endif // LSQL_QUERY_H
