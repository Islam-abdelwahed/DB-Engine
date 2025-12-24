// include/DropTableQuery.h
#pragma once
#include "Query.h"
#include <string>
using namespace std;

using namespace std;

class DropTableQuery : public Query {
public:
    vector<string> tableNames;
    bool ifExists;

    DropTableQuery() : ifExists(false) { type = QueryType::DROP_TABLE; }
};
