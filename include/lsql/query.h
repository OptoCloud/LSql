#ifndef LSQL_QUERY_H
#define LSQL_QUERY_H

#include "type.h"

#include <span>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

typedef struct sqlite3_stmt sqlite3_stmt;

namespace SQLite {
class Column;
class Connection;

class Query
{
    friend SQLite::Column;
    friend SQLite::Connection;

    Query() = default;
    Query(std::shared_ptr<SQLite::Connection> connection, const char* statement, int statementLen);
    Query(const SQLite::Query& other) = default;
    SQLite::Query& operator=(const SQLite::Query& other) = default;
public:
    ~Query();

    bool isValid() const;

    bool bindInt32(int index, std::int32_t value);
    bool bindInt64(int index, std::int64_t value);
    bool bindReal(int index, double value);

    bool bindBlob(int index, const void* data, std::size_t size);
    inline bool bindBlob(int index, std::span<const std::uint8_t> blob) { return bindBlob(index, blob.data(), blob.size()); }
    inline bool bindBlob(int index, const std::vector<std::uint8_t>& blob) { return bindBlob(index, blob.data(), blob.size()); }

    bool bindText(int index, const char* data, std::size_t size);
    inline bool bindText(int index, std::string_view text) { return bindText(index, text.data(), text.size()); }
    inline bool bindText(int index, const std::string& text) { return bindText(index, text.data(), text.size()); }

    bool step();

    int columnCount() const;
    SQLite::Type getType(int col);
    SQLite::Column column(int col);

    void finalize();
private:
    sqlite3_stmt* m_stmt;
    int m_ncols;
};
}

#endif // LSQL_QUERY_H
