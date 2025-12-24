// src/Database.cpp
#include "Database.h"
#include <sys/stat.h>
#include <direct.h>
#include <filesystem> // C++17 for directory iteration
#include <stdexcept>

void Database::createTable(const string& name, const vector<Column>& cols) {
    if (tables.find(name) != tables.end()) {
        throw runtime_error("Table already exists: " + name);
    }
    tables.emplace(name, Table(name, cols));
}

Table* Database::getTable(const string& name) {
    auto it = tables.find(name);
    if (it == tables.end()) return nullptr;
    return &it->second;
}

void Database::dropTable(const string& name) {
    tables.erase(name);
    string filePath = storagePath + "/" + name + ".csv";
    remove(filePath.c_str());
}

void Database::loadAllTables() {
    namespace fs = filesystem;
    for (const auto& entry : fs::directory_iterator(storagePath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            string tableName = entry.path().stem().string();
            Table table(tableName, {}); // Temp, will load columns
            table.loadFromCSV(entry.path().string());
            tables[tableName] = std::move(table);
        }
    }
}

vector<string> Database::getTableNames() const {
    vector<string> names;
    names.reserve(tables.size());

    for (const auto& pair : tables) {
        names.push_back(pair.first);
    }

    return names;
}

void Database::saveAllTables() {
    // Create directory if not exists
    _mkdir(storagePath.c_str());
    for (const auto& pair : tables) {
        string filePath = storagePath + "/" + pair.first + ".csv";
        pair.second.saveToCSV(filePath);
    }
}
