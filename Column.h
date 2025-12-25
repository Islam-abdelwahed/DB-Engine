// include/Column.h
#pragma once
#include <string>
#include "Value.h"
using namespace std;
class Column {
public:
    string name;
    DataType type;
    bool isPrimaryKey;
    bool isUnique;
    bool isForeignKey;
    string foreignTable;
    string foreignColumn;

    Column() : type(DataType::UNKNOWN), isPrimaryKey(false), isUnique(false), isForeignKey(false) {}
    Column(const string& n, DataType t) : name(n), type(t), isPrimaryKey(false), isUnique(false), isForeignKey(false) {}
};
