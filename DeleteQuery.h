// include/DeleteQuery.h
#pragma once
#include "Query.h"
#include <string>
#include "Condition.h"

class DeleteQuery : public Query {
public:
    std::string tableName;
    Condition where;

    DeleteQuery() { type = QueryType::DELETE; }
};