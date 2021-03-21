#include "transaction.h"

#include "connection.h"

using namespace std::literals;

SQLite::Transaction::Transaction(std::shared_ptr<SQLite::Connection> connection, SQLite::Transaction::Behavior behavior)
{
    std::string_view statement;
    switch (behavior) {
    case Behavior::DEFERRED:
        statement = "BEGIN DEFERRED"sv;
        break;
    case Behavior::IMMEDIATE:
        statement = "BEGIN IMMEDIATE"sv;
        break;
    case Behavior::EXCLUSIVE:
        statement = "BEGIN EXCLUSIVE"sv;
        break;
    default:
        return;
    }

    if (connection->execute(statement)) {
        m_connection = connection;
    }
}

SQLite::Transaction::~Transaction()
{
    rollback();
}

bool SQLite::Transaction::isOpen() const
{
    return m_connection != nullptr;
}

bool SQLite::Transaction::commit()
{
    auto connection = m_connection;

    if (connection != nullptr) {
        m_connection.reset();
        return connection->execute("COMMIT"sv);
    }

    return false;
}

void SQLite::Transaction::rollback()
{
    auto connection = m_connection;

    if (connection != nullptr) {
        m_connection.reset();
        connection->execute("ROLLBACK"sv);
    }
}
