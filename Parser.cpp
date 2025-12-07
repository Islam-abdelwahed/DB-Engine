// src/Parser.cpp
#include "Parser.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"
#include <sstream>
#include <algorithm>
#include <cctype>

// Helper functions from SQLParser.h and Helper.cpp adapted

std::string toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        result.push_back(trim(item));
    }
    return result;
}

std::string stripQuotes(std::string value) {
    value = trim(value);
    if (value.length() >= 2 && ((value.front() == '\'' && value.back() == '\'') || (value.front() == '"' && value.back() == '"'))) {
        return value.substr(1, value.length() - 2);
    }
    return value;
}

// Parse condition from WHERE clause (simple column op value)
Condition parseCondition(const std::string& wherePart) {
    Condition c;
    std::string opStr = "="; // Default
    size_t opPos = wherePart.find('=');
    if (opPos == std::string::npos) {
        opPos = wherePart.find('>');
        if (opPos != std::string::npos) opStr = ">";
    }
    if (opPos == std::string::npos) {
        opPos = wherePart.find('<');
        if (opPos != std::string::npos) opStr = "<";
    }
    // Add more ops

    if (opPos != std::string::npos) {
        c.column = trim(wherePart.substr(0, opPos));
        c.op = opStr;
        std::string valStr = trim(wherePart.substr(opPos + opStr.length()));
        c.value = Value(DataType::STRING, stripQuotes(valStr)); // Assume STRING
    }
    return c;
}

Query* Parser::parse(const std::string& sqlText) {
    std::string upperQuery = toUpper(trim(sqlText));

    if (upperQuery.find("SELECT") == 0) {
        SelectQuery* q = new SelectQuery();
        size_t fromPos = upperQuery.find("FROM");
        if (fromPos == std::string::npos) { delete q; return nullptr; }

        std::string columnsPart = trim(sqlText.substr(6, fromPos - 6));
        if (columnsPart == "*") {
            q->columns.push_back("*");
        } else {
            q->columns = split(columnsPart, ',');
        }

        size_t wherePos = upperQuery.find("WHERE");
        if (wherePos != std::string::npos) {
            std::string tablePart = trim(sqlText.substr(fromPos + 4, wherePos - fromPos - 4));
            q->tableName = tablePart;
            std::string wherePart = trim(sqlText.substr(wherePos + 5));
            q->where = parseCondition(wherePart);
        } else {
            std::string tablePart = trim(sqlText.substr(fromPos + 4));
            q->tableName = tablePart;
        }

        // TODO: Parse groupBy, orderBy if present
        return q;
    } else if (upperQuery.find("INSERT") == 0) {
        InsertQuery* q = new InsertQuery();
        size_t intoPos = upperQuery.find("INTO");
        size_t valuesPos = upperQuery.find("VALUES");
        if (intoPos == std::string::npos || valuesPos == std::string::npos) { delete q; return nullptr; }

        q->tableName = trim(sqlText.substr(intoPos + 4, valuesPos - intoPos - 4));

        size_t openParen = sqlText.find('(', valuesPos);
        size_t closeParen = sqlText.rfind(')');
        if (openParen == std::string::npos || closeParen == std::string::npos) { delete q; return nullptr; }

        std::string valuesPart = sqlText.substr(openParen + 1, closeParen - openParen - 1);
        auto valueStrs = split(valuesPart, ',');
        for (const auto& vs : valueStrs) {
            q->values.values.emplace_back(DataType::STRING, stripQuotes(vs));
        }

        return q;
    } else if (upperQuery.find("UPDATE") == 0) {
        UpdateQuery* q = new UpdateQuery();
        size_t setPos = upperQuery.find("SET");
        if (setPos == std::string::npos) { delete q; return nullptr; }

        q->tableName = trim(sqlText.substr(6, setPos - 6));

        size_t wherePos = upperQuery.find("WHERE");
        std::string setPart;
        if (wherePos != std::string::npos) {
            setPart = trim(sqlText.substr(setPos + 3, wherePos - setPos - 3));
            std::string wherePart = trim(sqlText.substr(wherePos + 5));
            q->where = parseCondition(wherePart);
        } else {
            setPart = trim(sqlText.substr(setPos + 3));
        }

        // Parse set: assume single column=value
        size_t eqPos = setPart.find('=');
        if (eqPos != std::string::npos) {
            std::string col = trim(setPart.substr(0, eqPos));
            std::string val = stripQuotes(trim(setPart.substr(eqPos + 1)));
            q->newValues[col] = Value(DataType::STRING, val);
        }

        return q;
    } else if (upperQuery.find("DELETE") == 0) {
        DeleteQuery* q = new DeleteQuery();
        size_t fromPos = upperQuery.find("FROM");
        if (fromPos == std::string::npos) { delete q; return nullptr; }

        size_t wherePos = upperQuery.find("WHERE");
        if (wherePos != std::string::npos) {
            q->tableName = trim(sqlText.substr(fromPos + 4, wherePos - fromPos - 4));
            std::string wherePart = trim(sqlText.substr(wherePos + 5));
            q->where = parseCondition(wherePart);
        } else {
            q->tableName = trim(sqlText.substr(fromPos + 4));
        }

        return q;
    }

    return nullptr;
}