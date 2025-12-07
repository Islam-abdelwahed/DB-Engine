// include/InsertQuery.h
#pragma once
#include "Query.h"
#include <string>
#include "Row.h"

class InsertQuery : public Query {
public:
    std::string tableName;
    Row values;  // Note: UML has Row, but can be vector of rows for multi-insert

    InsertQuery() { type = QueryType::INSERT; }
};