#ifndef LSQL_TRANSACTION_H
#define LSQL_TRANSACTION_H

namespace LSql {
class Connection;

class Transaction
{
    friend Connection;
    Transaction() = default;
    Transaction(const Transaction& other) = default;
    Transaction& operator=(const Transaction& other) = default;
public:
    Transaction(Connection& connection);
    ~Transaction();

    bool isOpen() const;

    bool commit();
    bool rollback();
private:
    Connection* m_connection;
};
}
#endif // LSQL_TRANSACTION_H
