// include/Database.h
#pragma once
#include <string>
#include <map>
#include <vector>
#include "Table.h"
using namespace std;

class Database {
private:
    map<string, Table> tables;
    string storagePath;

public:
    Database(const string& path = "data") : storagePath(path) {}

    void createTable(const string& name, const vector<Column>& cols);
    Table* getTable(const string& name);  // Returns pointer for compatibility
    void dropTable(const string& name);

    void loadAllTables();
    void saveAllTables();
    vector<string> getTableNames() const;


};
