// include/CreateTableQuery.h
#pragma once
#include "Query.h"
#include "Column.h"
#include <string>
#include <vector>

class CreateTableQuery : public Query {
public:
    std::string tableName;
    std::vector<Column> columns;

    CreateTableQuery() { type = QueryType::CREATE_TABLE; }
};
