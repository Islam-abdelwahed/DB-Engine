// include/Column.h
#pragma once
#include <string>
#include "Value.h"

class Column {
public:
    std::string name;
    DataType type;

    Column() : type(DataType::UNKNOWN) {}
    Column(const std::string& n, DataType t) : name(n), type(t) {}
};
