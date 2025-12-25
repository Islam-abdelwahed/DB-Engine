// include/CreateTableQuery.h
#pragma once
#include "Query.h"
#include "Column.h"
#include <string>
#include <vector>
using namespace std;

using namespace std;

class CreateTableQuery : public Query {
public:
    string tableName;
    vector<Column> columns;

    CreateTableQuery() { type = QueryType::CREATE_TABLE; }
};
