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
        // Compare data values, ignoring type for flexibility
        // This allows comparing INTEGER column with STRING value from WHERE clause
        return data == other.data;
    }

    // Add more operators if needed, e.g., <, > for sorting/conditions
    bool operator<(const Value& other) const {
        // Try numeric comparison first
        try {
            double thisNum = std::stod(data);
            double otherNum = std::stod(other.data);
            return thisNum < otherNum;
        } catch (...) {
            // Fall back to string comparison
            return data < other.data;
        }
    }

    bool operator>(const Value& other) const {
        // Try numeric comparison first
        try {
            double thisNum = std::stod(data);
            double otherNum = std::stod(other.data);
            return thisNum > otherNum;
        } catch (...) {
            // Fall back to string comparison
            return data > other.data;
        }
    }
};
