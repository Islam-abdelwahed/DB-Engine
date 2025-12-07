// src/Database.cpp
#include "Database.h"
#include <sys/stat.h>
#include <direct.h>
#include <filesystem> // C++17 for directory iteration
#include <stdexcept>

void Database::createTable(const std::string& name, const std::vector<Column>& cols) {
    if (tables.find(name) != tables.end()) {
        throw std::runtime_error("Table already exists: " + name);
    }
    tables.emplace(name, Table(name, cols));
}

Table* Database::getTable(const std::string& name) {
    auto it = tables.find(name);
    if (it == tables.end()) return nullptr;
    return &it->second;
}

void Database::dropTable(const std::string& name) {
    tables.erase(name);
    std::string filePath = storagePath + "/" + name + ".csv";
    std::remove(filePath.c_str());
}

void Database::loadAllTables() {
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(storagePath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            std::string tableName = entry.path().stem().string();
            Table table(tableName, {}); // Temp, will load columns
            table.loadFromCSV(entry.path().string());
            tables[tableName] = std::move(table);
        }
    }
}

std::vector<std::string> Database::getTableNames() const {
    std::vector<std::string> names;
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
        std::string filePath = storagePath + "/" + pair.first + ".csv";
        pair.second.saveToCSV(filePath);
    }
}
