#include "type.h"

#include "sqlite3.h"

static_assert ((int)SQLite::Type::Integer == SQLITE_INTEGER, "SQLite type-id mismatch! (Integer)");
static_assert ((int)SQLite::Type::Real    == SQLITE_FLOAT,   "SQLite type-id mismatch! (Real)");
static_assert ((int)SQLite::Type::Text    == SQLITE_TEXT,    "SQLite type-id mismatch! (Text)");
static_assert ((int)SQLite::Type::Blob    == SQLITE_BLOB,    "SQLite type-id mismatch! (Blob)");
static_assert ((int)SQLite::Type::Null    == SQLITE_NULL,    "SQLite type-id mismatch! (Null)");
