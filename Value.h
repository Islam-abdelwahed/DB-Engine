// include/Value.h
#pragma once
#include <string>

enum class DataType {
    STRING,
    VARCHAR,
    INTEGER,
    FLOAT,
    BOOLEAN,
    UNKNOWN
};

class Value {
public:
    DataType type;
    std::string data;

    Value() : type(DataType::UNKNOWN) {}
    Value(DataType t, const std::string& d) : type(t), data(d) {}

    bool operator==(const Value& other) const {
        if (type != other.type) return false;
        return data == other.data;
    }

    // Add more operators if needed, e.g., <, > for sorting/conditions
    bool operator<(const Value& other) const {
        if (type != other.type) return false;
        // Simple string comparison for now
        return data < other.data;
    }

    bool operator>(const Value& other) const {
        if (type != other.type) return false;
        return data > other.data;
    }
};
