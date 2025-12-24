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
    
    // Validate if value can be converted to the specified type
    bool isValidForType(DataType targetType) const {
        if (isNull) return true; // NULL is valid for any type
        
        switch (targetType) {
            case DataType::INTEGER:
                try {
                    size_t pos;
                    stoi(data, &pos);
                    return pos == data.length(); // Ensure entire string was converted
                } catch (...) {
                    return false;
                }
            
            case DataType::FLOAT:
                try {
                    size_t pos;
                    stod(data, &pos);
                    return pos == data.length();
                } catch (...) {
                    return false;
                }
            
            case DataType::BOOLEAN:
                return data == "0" || data == "1" || 
                       data == "true" || data == "false" ||
                       data == "TRUE" || data == "FALSE";
            
            case DataType::STRING:
            case DataType::VARCHAR:
            case DataType::DATE:
            case DataType::UNKNOWN:
            default:
                return true; // Strings accept anything
        }
    }
};
