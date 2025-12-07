// include/Table.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include "Column.h"
#include "Row.h"
#include "Condition.h"

class Table {
private:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;
    std::map<std::string, size_t> columnIndexMap;

    void rebuildIndexMap();

public:
    Table();
    Table(const std::string& n, const std::vector<Column>& cols);
    std::string getName() const { return name; }

    void insertRow(const Row& r);
    std::vector<Row> selectRows(const Condition& c) const;
    void updateRows(const Condition& c, const std::map<std::string, Value>& nv);
    void deleteRows(const Condition& c);

    void loadFromCSV(const std::string& filePath);
    void saveToCSV(const std::string& filePath) const;

    const std::vector<Column>& getColumns() const { return columns; }
    const std::vector<Row>& getRows() const { return rows; }
};
