// src/Condition.cpp
#include "Condition.h"
#include <algorithm>

using namespace std;

bool Condition::evaluate(const Row& r, const vector<Column>& columns) const {
    // If no column specified (no WHERE clause), return true for all rows
    if (column.empty()) {
        return true;
    }
    
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
    } else if (op == "!=") {
        return rowVal != value;
    }
    return false;
}
