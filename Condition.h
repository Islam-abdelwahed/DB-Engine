// include/Condition.h
#pragma once
#include <string>
#include "Value.h"
#include "Row.h"
#include "Column.h"
class Condition {
public:
    std::string column;
    std::string op;
    Value value;

    bool evaluate(const Row& r, const std::vector<Column>& columns) const;
};