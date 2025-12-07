// include/Column.h
#pragma once
#include <string>
#include "Value.h"

class Column {
public:
    std::string name;
    DataType type;
    bool isPrimaryKey;
    bool isForeignKey;
    std::string foreignTable;
    std::string foreignColumn;

    Column() : type(DataType::UNKNOWN), isPrimaryKey(false), isForeignKey(false) {}
    Column(const std::string& n, DataType t) : name(n), type(t), isPrimaryKey(false), isForeignKey(false) {}
};
