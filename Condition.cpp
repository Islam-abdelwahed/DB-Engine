// src/Condition.cpp
#include "Condition.h"
#include <algorithm>
using namespace std;


bool Condition::evaluate(const Row& r, const vector<Column>& columns) const {
    if (column.empty()) {
        return true;
    }
    
    auto it = find_if(columns.begin(), columns.end(), [&](const Column& c) { return c.name == column; });
    if (it == columns.end()) return false;
    size_t idx = distance(columns.begin(), it);

    if (idx >= r.values.size()) return false;
    const Value& rowVal = r.values[idx];

    if (op == "=") {
        return rowVal == value;
    } else if (op == ">") {
        return rowVal > value;
    } else if (op == "<") {
        return rowVal < value;
    }else if (op == "!=") {
        return rowVal != value;
    }
    return false;
}
