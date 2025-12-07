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

    // Handle JOINs
    std::vector<Column> allColumns = table->getColumns();
    std::vector<Row> joinedRows = selected;
    
    for (const auto& join : q->joins) {
        Table* joinTable = db.getTable(join.tableName);
        if (!joinTable) {
            error("Join table not found: " + join.tableName);
            return;
        }
        
        const auto& joinTableColumns = joinTable->getColumns();
        const auto& joinTableRows = joinTable->getRows();
        
        // Find column indices
        size_t leftColIdx = 0;
        bool foundLeft = false;
        for (size_t i = 0; i < allColumns.size(); ++i) {
            if (allColumns[i].name == join.leftColumn) {
                leftColIdx = i;
                foundLeft = true;
                break;
            }
        }
        
        size_t rightColIdx = 0;
        bool foundRight = false;
        for (size_t i = 0; i < joinTableColumns.size(); ++i) {
            if (joinTableColumns[i].name == join.rightColumn) {
                rightColIdx = i;
                foundRight = true;
                break;
            }
        }
        
        if (!foundLeft || !foundRight) {
            error("Join column not found");
            return;
        }
        
        // Perform join
        std::vector<Row> newJoinedRows;
        for (const auto& leftRow : joinedRows) {
            bool matched = false;
            for (const auto& rightRow : joinTableRows) {
                if (leftRow.values[leftColIdx] == rightRow.values[rightColIdx]) {
                    // Merge rows
                    Row mergedRow;
                    mergedRow.values = leftRow.values;
                    for (const auto& val : rightRow.values) {
                        mergedRow.values.push_back(val);
                    }
                    newJoinedRows.push_back(mergedRow);
                    matched = true;
                }
            }
            
            // For LEFT JOIN, include unmatched rows
            if (!matched && join.joinType == "LEFT") {
                Row mergedRow;
                mergedRow.values = leftRow.values;
                // Add NULL values for joined table columns
                for (size_t i = 0; i < joinTableColumns.size(); ++i) {
                    mergedRow.values.emplace_back(DataType::STRING, "");
                }
                newJoinedRows.push_back(mergedRow);
            }
        }
        
        joinedRows = newJoinedRows;
        // Add joined table columns to column list
        for (const auto& col : joinTableColumns) {
            allColumns.push_back(col);
        }
    }

    // Handle GROUP BY
    std::vector<Row> groupedRows = joinedRows;
    if (!q->groupBy.empty()) {
        // Find group by column indices
        std::vector<size_t> groupByIndices;
        for (const auto& colName : q->groupBy) {
            for (size_t i = 0; i < allColumns.size(); ++i) {
                if (allColumns[i].name == colName) {
                    groupByIndices.push_back(i);
                    break;
                }
            }
        }
        
        // Group rows by the specified columns
        std::map<std::string, Row> groups;
        for (const auto& row : joinedRows) {
            std::string groupKey;
            for (size_t idx : groupByIndices) {
                if (idx < row.values.size()) {
                    groupKey += row.values[idx].data + "|";
                }
            }
            
            if (groups.find(groupKey) == groups.end()) {
                groups[groupKey] = row;
            }
        }
        
        groupedRows.clear();
        for (const auto& pair : groups) {
            groupedRows.push_back(pair.second);
        }
    }

    // Handle ORDER BY
    if (!q->orderBy.empty()) {
        std::vector<size_t> orderByIndices;
        for (const auto& rule : q->orderBy) {
            for (size_t i = 0; i < allColumns.size(); ++i) {
                if (allColumns[i].name == rule.column) {
                    orderByIndices.push_back(i);
                    break;
                }
            }
        }
        
        std::sort(groupedRows.begin(), groupedRows.end(), [&](const Row& a, const Row& b) {
            for (size_t i = 0; i < q->orderBy.size() && i < orderByIndices.size(); ++i) {
                size_t idx = orderByIndices[i];
                if (idx >= a.values.size() || idx >= b.values.size()) continue;
                
                if (a.values[idx] == b.values[idx]) continue;
                
                if (q->orderBy[i].ascending) {
                    return a.values[idx] < b.values[idx];
                } else {
                    return a.values[idx] > b.values[idx];
                }
            }
            return false;
        });
    }

    // Handle column projection
    std::vector<Column> resultColumns;
    std::vector<Row> projectedRows;
    
    // Check if selecting all columns (*)
    bool selectAll = (q->columns.size() == 1 && q->columns[0] == "*");
    
    if (selectAll) {
        // Return all columns
        resultColumns = allColumns;
        projectedRows = groupedRows;
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
        for (const auto& row : groupedRows) {
            Row projectedRow;
            for (size_t idx : selectedIndices) {
                if (idx < row.values.size()) {
                    projectedRow.values.push_back(row.values[idx]);
                }
            }
            projectedRows.push_back(projectedRow);
        }
    }

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
    tree();
}

void QueryExecutor::executeDropTable(DropTableQuery* q, Database& db) {
    // Check if table exists
    if (!db.getTable(q->tableName)) {
        error("Table not found: " + q->tableName);
        return;
    }

    db.dropTable(q->tableName);
    output("Table '" + q->tableName + "' dropped successfully");
    tree();
}
