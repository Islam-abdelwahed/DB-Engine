// include/InsertQuery.h
#pragma once
#include "Query.h"
#include <string>
#include "Row.h"
using namespace std;


class InsertQuery : public Query {
public:
    string tableName;
    vector<string> specifiedColumns;  // Empty if all columns
    Row values;  // Note: UML has Row, but can be vector of rows for multi-insert

    InsertQuery() { type = QueryType::INSERT; }
};
