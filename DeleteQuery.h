// include/DeleteQuery.h
#pragma once
#include "Query.h"
#include <string>
#include "Condition.h"
using namespace std;

using namespace std;

class DeleteQuery : public Query {
public:
    string tableName;
    string tableAlias; // Alias for the table
    Condition where;

    DeleteQuery() { type = QueryType::DELETE; }
};
