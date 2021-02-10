cmake_minimum_required(VERSION 3.5)

set(project lsql)
project(
    ${project}
    DESCRIPTION "A super light wrapper around SQLite"
)

message("Configuring ${project}...")

set(CMAKE_CXX_STANDARD 20)

set(${project}_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/type.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/query.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/column.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transaction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/connection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/sqlite3.c
)
set(${project}_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lsql/type.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lsql/query.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lsql/column.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lsql/transaction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lsql/connection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/sqlite3.h
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lsql
)

add_library(${project}
    ${${project}_SOURCES}
    ${${project}_HEADERS}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${project} ${CMAKE_DL_LIBS} Threads::Threads)
