// src/Table.cpp
#include "Table.h"
#include "Database.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

Table::Table() {
    rebuildIndexMap();
}

Table::Table(const string& n, const vector<Column>& cols) : name(n), columns(cols) {
    rebuildIndexMap();
}

void Table::rebuildIndexMap() {
    columnIndexMap.clear();
    for (size_t i = 0; i < columns.size(); ++i) {
        columnIndexMap[columns[i].name] = i;
    }
}

bool Table::validatePrimaryKey(const Row& r) const {
    // Check each column that is a primary key
    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].isPrimaryKey) {
            // Check if this value already exists in existing rows
            for (const auto& existingRow : rows) {
                if (i < r.values.size() && i < existingRow.values.size()) {
                    if (existingRow.values[i].data == r.values[i].data) {
                        return false; // Duplicate primary key found
                    }
                }
            }
        }
    }
    return true; // No duplicates found
}

bool Table::validateForeignKeys(const Row& r, Database* db) const {
    if (!db) return true; // Can't validate without database reference
    
    // Check each column that is a foreign key
    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].isForeignKey) {
            // Get the referenced table
            Table* refTable = db->getTable(columns[i].foreignTable);
            if (!refTable) {
                return false; // Referenced table doesn't exist
            }
            
            // Find the referenced column index
            size_t refColIdx = refTable->getColumnIndex(columns[i].foreignColumn);
            if (refColIdx == static_cast<size_t>(-1)) {
                return false; // Referenced column doesn't exist
            }
            
            // Check if the value exists in the referenced table
            if (i < r.values.size()) {
                const string& fkValue = r.values[i].data;
                bool found = false;
                
                const auto& refRows = refTable->getRows();
                for (const auto& refRow : refRows) {
                    if (refColIdx < refRow.values.size()) {
                        if (refRow.values[refColIdx].data == fkValue) {
                            found = true;
                            break;
                        }
                    }
                }
                
                if (!found && !fkValue.empty()) {
                    return false; // Foreign key value doesn't exist in referenced table
                }
            }
        }
    }
    return true; // All foreign keys are valid
}

bool Table::insertRow(const Row& r, Database* db) {
    if (r.values.size() != columns.size()) return false; // Error handling
    
    // Validate primary key uniqueness
    if (!validatePrimaryKey(r)) {
        return false; // Duplicate primary key
    }
    
    // Validate foreign key constraints
    if (!validateForeignKeys(r, db)) {
        return false; // Foreign key violation
    }
    
    rows.push_back(r);
    return true;
}

bool Table::insertPartialRow(const vector<string>& columnNames, const Row& values, Database* db) {
    Row fullRow;
    fullRow.values.resize(columns.size());
    
    // Initialize all values as NULL with proper types
    for (size_t i = 0; i < columns.size(); ++i) {
        fullRow.values[i] = Value::createNull(columns[i].type);
    }
    
    // Fill in specified columns
    for (size_t i = 0; i < columnNames.size() && i < values.values.size(); ++i) {
        auto it = columnIndexMap.find(columnNames[i]);
        if (it != columnIndexMap.end()) {
            fullRow.values[it->second] = values.values[i];
        }
    }
    
    // Validate primary key uniqueness
    if (!validatePrimaryKey(fullRow)) {
        return false; // Duplicate primary key
    }
    
    // Validate foreign key constraints
    if (!validateForeignKeys(fullRow, db)) {
        return false; // Foreign key violation
    }
    
    rows.push_back(fullRow);
    return true;
}

size_t Table::getColumnIndex(const string& columnName) const {
    auto it = columnIndexMap.find(columnName);
    if (it != columnIndexMap.end()) {
        return it->second;
    }
    return static_cast<size_t>(-1); // Not found
}

vector<Row> Table::selectRows(const Condition& c) const {
    vector<Row> result;
    for (const auto& row : rows) {
        if (c.evaluate(row, columns)) {
            result.push_back(row);
        }
    }
    return result;
}

bool Table::updateRows(const Condition& c, const map<string, Value>& nv, Database* db) {
    // First, collect rows that match the condition and prepare updated versions
    vector<size_t> matchingIndices;
    vector<Row> updatedRows;
    
    for (size_t idx = 0; idx < rows.size(); ++idx) {
        if (c.evaluate(rows[idx], columns)) {
            matchingIndices.push_back(idx);
            Row updatedRow = rows[idx];
            
            // Apply updates
            for (const auto& pair : nv) {
                auto it = columnIndexMap.find(pair.first);
                if (it != columnIndexMap.end()) {
                    updatedRow.values[it->second] = pair.second;
                }
            }
            
            updatedRows.push_back(updatedRow);
        }
    }
    
    // Validate all updated rows
    for (size_t i = 0; i < updatedRows.size(); ++i) {
        const Row& updatedRow = updatedRows[i];
        size_t originalIdx = matchingIndices[i];
        
        // Check primary key uniqueness (excluding the row being updated)
        for (size_t colIdx = 0; colIdx < columns.size(); ++colIdx) {
            if (columns[colIdx].isPrimaryKey) {
                for (size_t rowIdx = 0; rowIdx < rows.size(); ++rowIdx) {
                    if (rowIdx != originalIdx && colIdx < updatedRow.values.size() && colIdx < rows[rowIdx].values.size()) {
                        if (rows[rowIdx].values[colIdx].data == updatedRow.values[colIdx].data) {
                            return false; // Duplicate primary key
                        }
                    }
                }
            }
        }
        
        // Validate foreign keys
        if (!validateForeignKeys(updatedRow, db)) {
            return false; // Foreign key violation
        }
    }
    
    // If all validations pass, apply the updates
    for (size_t i = 0; i < matchingIndices.size(); ++i) {
        rows[matchingIndices[i]] = updatedRows[i];
    }
    
    return true;
}

void Table::deleteRows(const Condition& c) {
    rows.erase(remove_if(rows.begin(), rows.end(), [&](const Row& row) {
        return c.evaluate(row, columns);
    }), rows.end());
}

void Table::loadFromCSV(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    // Read column names
    if (getline(file, line)) {
        stringstream ss(line);
        string colName;
        columns.clear();
        while (getline(ss, colName, ',')) {
            columns.emplace_back(colName, DataType::STRING); // Assume STRING for now
        }
    }

    // Read types if present (current code has types in second line)
    if (getline(file, line)) {
        stringstream ss(line);
        string typeStr;
        size_t idx = 0;
        while (getline(ss, typeStr, ',') && idx < columns.size()) {
            if (typeStr == "INT") columns[idx].type = DataType::INTEGER;
            else if (typeStr == "VARCHAR") columns[idx].type = DataType::VARCHAR;
            else if (typeStr == "FLOAT") columns[idx].type = DataType::FLOAT;
            else if (typeStr == "BOOL") columns[idx].type = DataType::BOOLEAN;
            else columns[idx].type = DataType::STRING;
            ++idx;
        }
    }

    // Read primary key flags
    if (getline(file, line)) {
        stringstream ss(line);
        string pkFlag;
        size_t idx = 0;
        while (getline(ss, pkFlag, ',') && idx < columns.size()) {
            columns[idx].isPrimaryKey = (pkFlag == "1");
            ++idx;
        }
    }

    // Read foreign key flags
    if (getline(file, line)) {
        stringstream ss(line);
        string fkFlag;
        size_t idx = 0;
        while (getline(ss, fkFlag, ',') && idx < columns.size()) {
            columns[idx].isForeignKey = (fkFlag == "1");
            ++idx;
        }
    }

    // Read foreign table names
    if (getline(file, line)) {
        stringstream ss(line);
        string fTable;
        size_t idx = 0;
        while (getline(ss, fTable, ',') && idx < columns.size()) {
            columns[idx].foreignTable = fTable;
            ++idx;
        }
    }

    // Read foreign column names
    if (getline(file, line)) {
        stringstream ss(line);
        string fColumn;
        size_t idx = 0;
        while (getline(ss, fColumn, ',') && idx < columns.size()) {
            columns[idx].foreignColumn = fColumn;
            ++idx;
        }
    }

    rebuildIndexMap();

    // Read rows
    rows.clear();
    while (getline(file, line)) {
        stringstream ss(line);
        string valStr;
        Row row;
        size_t idx = 0;
        while (getline(ss, valStr, ',')) {
            if (idx < columns.size()) {
                // Check if value is "null" (case-insensitive)
                if (valStr == "null" || valStr == "NULL") {
                    row.values.push_back(Value::createNull(columns[idx].type));
                } else {
                    row.values.emplace_back(columns[idx].type, valStr);
                }
            }
            ++idx;
        }
        if (!row.values.empty()) rows.push_back(row);
    }
}

void Table::saveToCSV(const string& filePath) const {
    ofstream file(filePath);
    if (!file.is_open()) return;

    // Write column names
    for (size_t i = 0; i < columns.size(); ++i) {
        file << columns[i].name;
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write types
    for (size_t i = 0; i < columns.size(); ++i) {
        string typeStr;
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

    // Write primary key flags
    for (size_t i = 0; i < columns.size(); ++i) {
        file << (columns[i].isPrimaryKey ? "1" : "0");
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write foreign key flags
    for (size_t i = 0; i < columns.size(); ++i) {
        file << (columns[i].isForeignKey ? "1" : "0");
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write foreign table names
    for (size_t i = 0; i < columns.size(); ++i) {
        file << columns[i].foreignTable;
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write foreign column names
    for (size_t i = 0; i < columns.size(); ++i) {
        file << columns[i].foreignColumn;
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    // Write rows
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.values.size(); ++i) {
            // Write "null" for NULL values
            if (row.values[i].isNull) {
                file << "null";
            } else {
                file << row.values[i].data;
            }
            if (i < row.values.size() - 1) file << ",";
        }
        file << "\n";
    }
}
