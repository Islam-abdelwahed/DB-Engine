// include/Table.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include "Column.h"
#include "Row.h"
#include "Condition.h"
using namespace std;


class Table {
private:
    string name;
    vector<Column> columns;
    vector<Row> rows;
    map<string, size_t> columnIndexMap;

    void rebuildIndexMap();

public:
    Table();
    Table(const string& n, const vector<Column>& cols);
    string getName() const { return name; }

    void insertRow(const Row& r);
    void insertPartialRow(const vector<string>& columnNames, const Row& values);
    vector<Row> selectRows(const Condition& c) const;
    void updateRows(const Condition& c, const map<string, Value>& nv);
    void deleteRows(const Condition& c);

    void loadFromCSV(const string& filePath);
    void saveToCSV(const string& filePath) const;

    const vector<Column>& getColumns() const { return columns; }
    const vector<Row>& getRows() const { return rows; }
    size_t getColumnIndex(const string& columnName) const;
};
