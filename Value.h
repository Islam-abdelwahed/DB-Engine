// include/Value.h
#pragma once
#include <string>

using namespace std;

enum class DataType {
    STRING,
    VARCHAR,
    INTEGER,
    FLOAT,
    BOOLEAN,
    DATE,
    UNKNOWN
};

class Value {
public:
    DataType type;
    string data;

    Value() : type(DataType::UNKNOWN) {}
    Value(DataType t, const string& d) : type(t), data(d) {}

    bool operator==(const Value& other) const {
        return data == other.data;
    }

    bool operator!=(const Value& other) const {
        return data != other.data;
    }

    bool operator<(const Value& other) const {
        try {
            double thisNum = stod(data);
            double otherNum = stod(other.data);
            return thisNum < otherNum;
        } catch (...) {
            return data < other.data;
        }
    }

    bool operator>(const Value& other) const {
        // Try numeric comparison first
        try {
            double thisNum = stod(data);
            double otherNum = stod(other.data);
            return thisNum > otherNum;
        } catch (...) {
            // Fall back to string comparison
            return data > other.data;
        }
    }
};
