// include/Condition.h
#pragma once
#include <string>
#include "Value.h"
#include "Row.h"
#include "Column.h"
using namespace std;


class Condition {
public:
    string column;
    string op;
    Value value;

    bool evaluate(const Row& r, const vector<Column>& columns) const;
};
