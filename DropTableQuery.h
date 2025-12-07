// include/DropTableQuery.h
#pragma once
#include "Query.h"
#include <string>

class DropTableQuery : public Query {
public:
    std::string tableName;

    DropTableQuery() { type = QueryType::DROP_TABLE; }
};
