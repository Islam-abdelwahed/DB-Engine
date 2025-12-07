// include/SelectQuery.h
#pragma once
#include "Query.h"
#include <vector>
#include <string>
#include "Condition.h"
#include "SortRule.h"

class SelectQuery : public Query {
public:
    std::vector<std::string> columns;
    std::string tableName;
    Condition where;
    std::vector<std::string> groupBy;
    std::vector<SortRule> orderBy;

    SelectQuery() { type = QueryType::SELECT; }
};