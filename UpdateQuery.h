// include/UpdateQuery.h
#pragma once
#include "Query.h"
#include <string>
#include <map>
#include "Value.h"
#include "Condition.h"

class UpdateQuery : public Query {
public:
    std::string tableName;
    Condition where;
    std::map<std::string, Value> newValues;

    UpdateQuery() { type = QueryType::UPDATE; }
};