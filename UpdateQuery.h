// include/UpdateQuery.h
#pragma once
#include "Query.h"
#include <string>
#include <map>
#include "Value.h"
#include "Condition.h"
using namespace std;


using namespace std;

class UpdateQuery : public Query {
public:
    string tableName;
    string tableAlias; // Alias for the table
    Condition where;
    map<string, Value> newValues;

    UpdateQuery() { type = QueryType::UPDATE; }
};
