// include/SelectQuery.h
#pragma once
#include "Query.h"
#include <vector>
#include <string>
#include "Condition.h"
#include "SortRule.h"

struct JoinClause {
    std::string tableName;
    std::string leftColumn;  // column from main table
    std::string rightColumn; // column from joined table
    std::string joinType;    // "INNER", "LEFT", "RIGHT"
};

struct AggregateFunction {
    std::string function;      // "SUM", "COUNT", "AVG", "MIN", "MAX"
    std::string column;        // column name to aggregate
    std::string alias;         // result column name (e.g., "AVG(age)")
};

class SelectQuery : public Query {
public:
    std::vector<std::string> columns;
    std::vector<AggregateFunction> aggregates; // Aggregate functions in SELECT
    std::string tableName;
    Condition where;
    std::vector<std::string> groupBy;
    std::vector<SortRule> orderBy;
    std::vector<JoinClause> joins;

    SelectQuery() { type = QueryType::SELECT; }
};