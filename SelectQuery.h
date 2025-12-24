// include/SelectQuery.h
#pragma once
#include "Query.h"
#include <vector>
#include <string>
#include "Condition.h"
#include "SortRule.h"
using namespace std;

struct JoinClause {
    string tableName;
    string leftColumn;  // column from main table
    string rightColumn; // column from joined table
    string joinType;    // "INNER", "LEFT", "RIGHT"
};

struct AggregateFunction {
    string function;      // "SUM", "COUNT", "AVG", "MIN", "MAX"
    string column;        // column name to aggregate
    string alias;         // result column name (e.g., "AVG(age)")
};

class SelectQuery : public Query {
public:
    vector<string> columns;
    vector<AggregateFunction> aggregates; // Aggregate functions in SELECT
    string tableName;
    Condition where;
    vector<string> groupBy;
    vector<SortRule> orderBy;
    vector<JoinClause> joins;

    SelectQuery() { type = QueryType::SELECT; }
};
