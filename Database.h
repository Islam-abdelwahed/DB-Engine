// include/Database.h
#pragma once
#include <string>
#include <map>
#include <vector>
#include "Table.h"

class Database {
private:
    std::map<std::string, Table> tables;  // Changed to own Table, not pointer
    std::string storagePath;

public:
    Database(const std::string& path = "data") : storagePath(path) {}

    void createTable(const std::string& name, const std::vector<Column>& cols);
    Table* getTable(const std::string& name);  // Returns pointer for compatibility
    void dropTable(const std::string& name);

    void loadAllTables();
    void saveAllTables();
    std::vector<std::string> getTableNames() const;


};
