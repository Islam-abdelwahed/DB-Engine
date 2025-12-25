// include/Condition.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Value.h"
#include "Row.h"
#include "Column.h"
using namespace std;

enum class LogicalOperator {
    NONE,   // Single condition
    AND,
    OR
};

class Condition {
public:
    string column;
    string op;
    Value value;
    
    // Support for compound conditions
    LogicalOperator logicalOp;
    unique_ptr<Condition> left;
    unique_ptr<Condition> right;

    Condition() : logicalOp(LogicalOperator::NONE) {}
    
    // Copy constructor for deep copy
    Condition(const Condition& other);
    
    // Assignment operator
    Condition& operator=(const Condition& other);
    
    // Resolve column names with aliases (e.g., "alias.column" -> "column")
    void resolveColumnAlias(const string& tableAlias);

    bool evaluate(const Row& r, const vector<Column>& columns) const;
};
