#include "transaction.h"

#include "connection.h"

LSql::Transaction::Transaction(LSql::Connection& connection)
    : m_connection(nullptr)
{
    if (connection.execute("BEGIN"))
    {
        m_connection = &connection;
        m_connection->m_transaction = this;
    }
}

LSql::Transaction::~Transaction()
{
    rollback();
}

bool LSql::Transaction::isOpen() const
{
    return m_connection != nullptr;
}

bool LSql::Transaction::commit()
{
    bool retval = false;

    if (isOpen())
    {
        LSql::Connection* connection = m_connection;
        m_connection = nullptr;
        retval = connection->execute("COMMIT");
        connection->m_transaction = nullptr;
    }

    return retval;
}

bool LSql::Transaction::rollback()
{
    bool retval = false;

    if (isOpen())
    {
        LSql::Connection* connection = m_connection;
        m_connection = nullptr;
        retval = connection->execute("ROLLBACK");
        connection->m_transaction = nullptr;
    }

    return retval;
}
