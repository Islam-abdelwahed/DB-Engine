// src/Table.cpp
#include "Table.h"
#include <fstream>
#include <sstream>
#include <algorithm>

Table::Table() {
    rebuildIndexMap();
}

Table::Table(const std::string& n, const std::vector<Column>& cols) : name(n), columns(cols) {
    rebuildIndexMap();
}

void Table::rebuildIndexMap() {
    columnIndexMap.clear();
    for (size_t i = 0; i < columns.size(); ++i) {
        columnIndexMap[columns[i].name] = i;
    }
}

void Table::insertRow(const Row& r) {
    if (r.values.size() != columns.size()) return; // Error handling
    rows.push_back(r);
}

void Table::insertPartialRow(const std::vector<std::string>& columnNames, const Row& values) {
    Row fullRow;
    fullRow.values.resize(columns.size());
    
    // Initialize all values as empty strings with proper types
    for (size_t i = 0; i < columns.size(); ++i) {
        fullRow.values[i] = Value(columns[i].type, "");
    }
    
    // Fill in specified columns
    for (size_t i = 0; i < columnNames.size() && i < values.values.size(); ++i) {
        auto it = columnIndexMap.find(columnNames[i]);
        if (it != columnIndexMap.end()) {
            fullRow.values[it->second] = values.values[i];
        }
    }
    
    rows.push_back(fullRow);
}

size_t Table::getColumnIndex(const std::string& columnName) const {
    auto it = columnIndexMap.find(columnName);
    if (it != columnIndexMap.end()) {
        return it->second;
    }
    return static_cast<size_t>(-1); // Not found
}

std::vector<Row> Table::selectRows(const Condition& c) const {
    std::vector<Row> result;
    for (const auto& row : rows) {
        if (c.evaluate(row, columns)) {
            result.push_back(row);
        }
    }
    return result;
}

void Table::updateRows(const Condition& c, const std::map<std::string, Value>& nv) {
    for (auto& row : rows) {
        if (c.evaluate(row, columns)) {
            for (const auto& pair : nv) {
                auto it = columnIndexMap.find(pair.first);
                if (it != columnIndexMap.end()) {
                    row.values[it->second] = pair.second;
                }
            }
        }
    }
}

void Table::deleteRows(const Condition& c) {
    rows.erase(std::remove_if(rows.begin(), rows.end(), [&](const Row& row) {
        return c.evaluate(row, columns);
    }), rows.end());
}

void Table::loadFromCSV(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    std::string line;
    // Read column names
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string colName;
        columns.clear();
        while (std::getline(ss, colName, ',')) {
            columns.emplace_back(colName, DataType::STRING); // Assume STRING for now
        }
    }

    // Read types if present (current code has types in second line)
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string typeStr;
        size_t idx = 0;
        while (std::getline(ss, typeStr, ',') && idx < columns.size()) {
            if (typeStr == "INT") columns[idx].type = DataType::INTEGER;
            else if (typeStr == "VARCHAR") columns[idx].type = DataType::VARCHAR;
            else if (typeStr == "FLOAT") columns[idx].type = DataType::FLOAT;
            else if (typeStr == "BOOL") columns[idx].type = DataType::BOOLEAN;
            else columns[idx].type = DataType::STRING;
            ++idx;
        }
    }

    rebuildIndexMap();

    // Read rows
    rows.clear();
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string valStr;
        Row row;
        size_t idx = 0;
        while (std::getline(ss, valStr, ',')) {
            if (idx < columns.size()) {
                row.values.emplace_back(columns[idx].type, valStr);
            }
            ++idx;
        }
        if (!row.values.empty()) rows.push_back(row);
    }
}

void Table::saveToCSV(const std::string& filePath) const {
    std::ofstream file(filePath);
    if (!file.is_open()) return;

    // Write column names
    for (size_t i = 0; i < columns.size(); ++i) {
        file << columns[i].name;
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write types
    for (size_t i = 0; i < columns.size(); ++i) {
        std::string typeStr;
        switch (columns[i].type) {
            case DataType::INTEGER: typeStr = "INT"; break;
            case DataType::VARCHAR: typeStr = "VARCHAR"; break;
            case DataType::FLOAT: typeStr = "FLOAT"; break;
            case DataType::BOOLEAN: typeStr = "BOOL"; break;
            default: typeStr = "STRING";
        }
        file << typeStr;
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write rows
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.values.size(); ++i) {
            file << row.values[i].data;
            if (i < row.values.size() - 1) file << ",";
        }
        file << "\n";
    }
}
