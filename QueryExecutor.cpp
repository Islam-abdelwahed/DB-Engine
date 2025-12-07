// src/QueryExecutor.cpp
#include "QueryExecutor.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"
#include "CreateTableQuery.h"
#include "DropTableQuery.h"


void QueryExecutor::execute(Query* q, Database& db) {
    if (!q) return;

    switch (q->type) {
    case QueryType::SELECT:
        executeSelect(static_cast<SelectQuery*>(q), db);
        break;
    case QueryType::INSERT:
        executeInsert(static_cast<InsertQuery*>(q), db);
        break;
    case QueryType::UPDATE:
        executeUpdate(static_cast<UpdateQuery*>(q), db);
        break;
    case QueryType::DELETE:
        executeDelete(static_cast<DeleteQuery*>(q), db);
        break;
    case QueryType::CREATE_TABLE:
        executeCreateTable(static_cast<CreateTableQuery*>(q), db);
        break;
    case QueryType::DROP_TABLE:
        executeDropTable(static_cast<DropTableQuery*>(q), db);
        break;
    default:
        error("Unknown query type");
    }
}

void QueryExecutor::executeSelect(SelectQuery* q, Database& db) {
    Table* table = db.getTable(q->tableName);
    if (!table) {
        error("Table not found: " + q->tableName);
        return;
    }

    std::vector<Row> selected = table->selectRows(q->where);

    // Handle column projection
    std::vector<Column> resultColumns;
    std::vector<Row> projectedRows;

    const auto& allColumns = table->getColumns();
    
    // Check if selecting all columns (*)
    bool selectAll = (q->columns.size() == 1 && q->columns[0] == "*");
    
    if (selectAll) {
        // Return all columns
        resultColumns = allColumns;
        projectedRows = selected;
    } else {
        // Project only requested columns
        // Build column index mapping
        std::map<std::string, size_t> columnIndexMap;
        for (size_t i = 0; i < allColumns.size(); ++i) {
            columnIndexMap[allColumns[i].name] = i;
        }
        
        // Get indices of requested columns
        std::vector<size_t> selectedIndices;
        for (const auto& colName : q->columns) {
            auto it = columnIndexMap.find(colName);
            if (it != columnIndexMap.end()) {
                selectedIndices.push_back(it->second);
                resultColumns.push_back(allColumns[it->second]);
            } else {
                error("Column not found: " + colName);
                return;
            }
        }
        
        // Project rows to only include selected columns
        for (const auto& row : selected) {
            Row projectedRow;
            for (size_t idx : selectedIndices) {
                if (idx < row.values.size()) {
                    projectedRow.values.push_back(row.values[idx]);
                }
            }
            projectedRows.push_back(projectedRow);
        }
    }

    // TODO: Handle groupBy, orderBy

    // Call the result callback if set
    resultTable(resultColumns, projectedRows);

    output("(" + std::to_string(projectedRows.size()) + " row(s) selected)");
}

void QueryExecutor::executeInsert(InsertQuery* q, Database& db) {
    Table* table = db.getTable(q->tableName);
    if (!table) {
        error("Table not found: " + q->tableName);
        return;
    }

    table->insertRow(q->values);
    output("1 row inserted");
}

void QueryExecutor::executeUpdate(UpdateQuery* q, Database& db) {
    Table* table = db.getTable(q->tableName);
    if (!table) {
        error("Table not found: " + q->tableName);
        return;
    }

    table->updateRows(q->where, q->newValues);
    output("Rows updated");
}

void QueryExecutor::executeDelete(DeleteQuery* q, Database& db) {
    Table* table = db.getTable(q->tableName);
    if (!table) {
        error("Table not found: " + q->tableName);
        return;
    }

    table->deleteRows(q->where);
    output("Rows deleted");
}

void QueryExecutor::executeCreateTable(CreateTableQuery* q, Database& db) {
    // Check if table already exists
    if (db.getTable(q->tableName)) {
        error("Table already exists: " + q->tableName);
        return;
    }

    db.createTable(q->tableName, q->columns);
    output("Table '" + q->tableName + "' created successfully");
}

void QueryExecutor::executeDropTable(DropTableQuery* q, Database& db) {
    // Check if table exists
    if (!db.getTable(q->tableName)) {
        error("Table not found: " + q->tableName);
        return;
    }

    db.dropTable(q->tableName);
    output("Table '" + q->tableName + "' dropped successfully");
}
