// include/Table.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include "Column.h"
#include "Row.h"
#include "Condition.h"

// Forward declaration
class Database;

class Table {
private:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;
    std::map<std::string, size_t> columnIndexMap;

    void rebuildIndexMap();
    bool validatePrimaryKey(const Row& r) const;
    bool validateForeignKeys(const Row& r, Database* db) const;

public:
    Table();
    Table(const std::string& n, const std::vector<Column>& cols);
    std::string getName() const { return name; }

    bool insertRow(const Row& r, Database* db = nullptr);
    bool insertPartialRow(const std::vector<std::string>& columnNames, const Row& values, Database* db = nullptr);
    std::vector<Row> selectRows(const Condition& c) const;
    bool updateRows(const Condition& c, const std::map<std::string, Value>& nv, Database* db = nullptr);
    void deleteRows(const Condition& c);

    void loadFromCSV(const std::string& filePath);
    void saveToCSV(const std::string& filePath) const;

    const std::vector<Column>& getColumns() const { return columns; }
    const std::vector<Row>& getRows() const { return rows; }
    size_t getColumnIndex(const std::string& columnName) const;
};
