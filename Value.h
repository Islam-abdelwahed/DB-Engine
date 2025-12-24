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
    bool isNull;

    Value() : type(DataType::UNKNOWN), isNull(false) {}
    Value(DataType t, const string& d) : type(t), data(d), isNull(d == "null" || d == "NULL") {}
    
    // Static factory method for NULL values
    static Value createNull(DataType t = DataType::UNKNOWN) {
        Value v;
        v.type = t;
        v.data = "null";
        v.isNull = true;
        return v;
    }

    bool operator==(const Value& other) const {
        // NULL comparisons: NULL == NULL is false in SQL
        if (isNull || other.isNull) return false;
        return data == other.data;
    }

    bool operator!=(const Value& other) const {
        // NULL comparisons: NULL != value is false in SQL
        if (isNull || other.isNull) return false;
        return data != other.data;
    }

    bool operator<(const Value& other) const {
        // NULL comparisons: NULL < value is false in SQL
        if (isNull || other.isNull) return false;
        try {
            double thisNum = stod(data);
            double otherNum = stod(other.data);
            return thisNum < otherNum;
        } catch (...) {
            return data < other.data;
        }
    }

    bool operator>(const Value& other) const {
        // NULL comparisons: NULL > value is false in SQL
        if (isNull || other.isNull) return false;
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
