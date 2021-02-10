#ifndef LSQL_TYPE_H
#define LSQL_TYPE_H

namespace SQLite {
enum class Type : int
{
    Invalid  = 0,
    Integer  = 1,
    Real     = 2,
    Text     = 3,
    Blob     = 4,
    Null     = 5
};
}

#endif // LSQL_TYPE_H
