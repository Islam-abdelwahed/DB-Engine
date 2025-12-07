// src/Condition.cpp
#include "Condition.h"
#include <algorithm> // for find_if

bool Condition::evaluate(const Row& r, const std::vector<Column>& columns) const {
    // Find column index
    auto it = std::find_if(columns.begin(), columns.end(), [&](const Column& c) { return c.name == column; });
    if (it == columns.end()) return false;
    size_t idx = std::distance(columns.begin(), it);

    if (idx >= r.values.size()) return false;
    const Value& rowVal = r.values[idx];

    if (op == "=") {
        return rowVal == value;
    } else if (op == ">") {
        return rowVal > value;
    } else if (op == "<") {
        return rowVal < value;
    }
    // Add more operators as needed
    return false;
}