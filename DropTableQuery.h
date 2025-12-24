// include/DropTableQuery.h
#pragma once
#include "Query.h"
#include <string>
using namespace std;

using namespace std;

class DropTableQuery : public Query {
public:
    string tableName;

    DropTableQuery() { type = QueryType::DROP_TABLE; }
};
