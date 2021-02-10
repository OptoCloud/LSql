#ifndef LSQL_TRANSACTION_H
#define LSQL_TRANSACTION_H

#include <memory>

namespace SQLite {
class Connection;

class Transaction
{
    friend SQLite::Connection;

    enum class Behavior {
        DEFERRED,
        IMMEDIATE,
        EXCLUSIVE
    };

    Transaction() = default;
    Transaction(std::shared_ptr<SQLite::Connection> connection, SQLite::Transaction::Behavior behavior = Behavior::DEFERRED);
    Transaction(const SQLite::Transaction& other) = default;
    SQLite::Transaction& operator=(const SQLite::Transaction& other) = default;
public:
    ~Transaction();

    bool isOpen() const;

    bool commit();
    void rollback();
private:
    std::shared_ptr<SQLite::Connection> m_connection;
};
}
#endif // LSQL_TRANSACTION_H
