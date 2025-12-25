// src/Condition.cpp
#include "Condition.h"
#include <algorithm>

using namespace std;

// Copy constructor for deep copy
Condition::Condition(const Condition& other) 
    : column(other.column),
      op(other.op),
      value(other.value),
      logicalOp(other.logicalOp),
      left(other.left ? new Condition(*other.left) : nullptr),
      right(other.right ? new Condition(*other.right) : nullptr) {}

// Assignment operator
Condition& Condition::operator=(const Condition& other) {
    if (this != &other) {
        column = other.column;
        op = other.op;
        value = other.value;
        logicalOp = other.logicalOp;
        left = other.left ? unique_ptr<Condition>(new Condition(*other.left)) : nullptr;
        right = other.right ? unique_ptr<Condition>(new Condition(*other.right)) : nullptr;
    }
    return *this;
}

// Resolve column names with aliases (e.g., "alias.column" -> "column")
void Condition::resolveColumnAlias(const string& tableAlias) {
    // Resolve this condition's column
    if (!column.empty()) {
        size_t dotPos = column.find('.');
        if (dotPos != string::npos) {
            string prefix = column.substr(0, dotPos);
            string suffix = column.substr(dotPos + 1);
            if (prefix == tableAlias) {
                column = suffix;
            }
        }
    }
    
    // Recursively resolve left and right conditions
    if (left) {
        left->resolveColumnAlias(tableAlias);
    }
    if (right) {
        right->resolveColumnAlias(tableAlias);
    }
}

bool Condition::evaluate(const Row& r, const vector<Column>& columns) const {
    // If no column specified (no WHERE clause), return true for all rows
    if (column.empty() && logicalOp == LogicalOperator::NONE) {
        return true;
    }
    
    // Handle compound conditions (AND/OR)
    if (logicalOp != LogicalOperator::NONE) {
        if (!left || !right) return false;
        
        bool leftResult = left->evaluate(r, columns);
        bool rightResult = right->evaluate(r, columns);
        
        if (logicalOp == LogicalOperator::AND) {
            return leftResult && rightResult;
        } else if (logicalOp == LogicalOperator::OR) {
            return leftResult || rightResult;
        }
        return false;
    }
    
    // Handle simple condition
    // Find column index
    auto it = find_if(columns.begin(), columns.end(), [&](const Column& c) { return c.name == column; });
    if (it == columns.end()) return false;
    size_t idx = distance(columns.begin(), it);

    if (idx >= r.values.size()) return false;
    const Value& rowVal = r.values[idx];

    // Handle NULL comparisons:
    // In SQL, NULL comparisons with =, !=, <, > always return false
    // Use IS NULL or IS NOT NULL for NULL checks (not implemented here yet)
    if (rowVal.isNull || value.isNull) {
        return false; // NULL comparisons are always false in standard operators
    }

    if (op == "=") {
        return rowVal == value;
    } else if (op == ">") {
        return rowVal > value;
    } else if (op == "<") {
        return rowVal < value;
    } else if (op == ">=") {
        return rowVal > value || rowVal == value;
    } else if (op == "<=") {
        return rowVal < value || rowVal == value;
    } else if (op == "!=") {
        return rowVal != value;
    } else if (op == "<>") {
        return rowVal != value;
    }
    return false;
}
