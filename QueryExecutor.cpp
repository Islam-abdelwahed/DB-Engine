// src/QueryExecutor.cpp
#include "QueryExecutor.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"
#include "CreateTableQuery.h"
#include "DropTableQuery.h"
#include <algorithm>


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
    std::vector<Column> groupedColumns = allColumns;
    
    if (!q->groupBy.empty() || !q->aggregates.empty()) {
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
        std::map<std::string, std::vector<Row>> groups;
        for (const auto& row : joinedRows) {
            std::string groupKey;
            for (size_t idx : groupByIndices) {
                if (idx < row.values.size()) {
                    groupKey += row.values[idx].data + "|";
                }
            }
            
            // If no GROUP BY columns but has aggregates, use single group
            if (groupKey.empty() && !q->aggregates.empty()) {
                groupKey = "ALL";
            }
            
            groups[groupKey].push_back(row);
        }
        
        // Calculate aggregates for each group
        groupedRows.clear();
        groupedColumns.clear();
        
        // Add GROUP BY columns first
        for (const auto& colName : q->groupBy) {
            for (const auto& col : allColumns) {
                if (col.name == colName) {
                    groupedColumns.push_back(col);
                    break;
                }
            }
        }
        
        // Add aggregate result columns
        for (const auto& agg : q->aggregates) {
            Column aggCol;
            aggCol.name = agg.alias;
            aggCol.type = DataType::FLOAT; // Aggregates return numeric values
            groupedColumns.push_back(aggCol);
        }
        
        // Process each group
        for (const auto& pair : groups) {
            const auto& groupRows = pair.second;
            if (groupRows.empty()) continue;
            
            Row resultRow;
            
            // Add GROUP BY column values (from first row of group)
            for (size_t idx : groupByIndices) {
                if (idx < groupRows[0].values.size()) {
                    resultRow.values.push_back(groupRows[0].values[idx]);
                }
            }
            
            // Calculate aggregate values
            for (const auto& agg : q->aggregates) {
                // Find column index for this aggregate (unless it's COUNT(*))
                size_t colIdx = 0;
                bool found = false;
                
                if (agg.column != "*") {
                    for (size_t i = 0; i < allColumns.size(); ++i) {
                        if (allColumns[i].name == agg.column) {
                            colIdx = i;
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        error("Aggregate column not found: " + agg.column);
                        return;
                    }
                }
                
                // Calculate aggregate
                double result = 0.0;
                int count = 0;
                double sum = 0.0;
                double minVal = 0.0;
                double maxVal = 0.0;
                bool first = true;
                
                // COUNT(*) just counts all rows
                if (agg.function == "COUNT" && agg.column == "*") {
                    result = static_cast<double>(groupRows.size());
                } else {
                    for (const auto& row : groupRows) {
                        if (colIdx < row.values.size()) {
                            try {
                                double val = std::stod(row.values[colIdx].data);
                                
                                if (agg.function == "SUM" || agg.function == "AVG") {
                                    sum += val;
                                }
                                if (agg.function == "MIN") {
                                    if (first || val < minVal) minVal = val;
                                }
                                if (agg.function == "MAX") {
                                    if (first || val > maxVal) maxVal = val;
                                }
                                
                                count++;
                                first = false;
                            } catch (...) {
                                // Skip non-numeric values
                            }
                        }
                    }
                    
                    // Set result based on function type
                    if (agg.function == "COUNT") {
                        result = static_cast<double>(count);
                    } else if (agg.function == "SUM") {
                        result = sum;
                    } else if (agg.function == "AVG") {
                        result = count > 0 ? sum / count : 0.0;
                    } else if (agg.function == "MIN") {
                        result = minVal;
                    } else if (agg.function == "MAX") {
                        result = maxVal;
                    }
                }
                
                resultRow.values.emplace_back(DataType::FLOAT, std::to_string(result));
            }
            
            groupedRows.push_back(resultRow);
        }
        
        // Update allColumns to reflect grouped columns
        allColumns = groupedColumns;
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
    bool selectAll = (q->columns.size() == 1 && q->columns[0] == "*" && q->aggregates.empty());
    
    if (selectAll) {
        // Return all columns
        resultColumns = allColumns;
        projectedRows = groupedRows;
    } else if (!q->groupBy.empty() || !q->aggregates.empty()) {
        // When using GROUP BY or aggregates, columns are already properly set up
        resultColumns = allColumns;
        projectedRows = groupedRows;
    } else {
        // Project only requested columns (no GROUP BY/aggregates)
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

    // If specific columns are specified, use insertPartialRow
    if (!q->specifiedColumns.empty()) {
        // Validate columns exist
        for (const auto& colName : q->specifiedColumns) {
            if (table->getColumnIndex(colName) == static_cast<size_t>(-1)) {
                error("Column not found: " + colName);
                return;
            }
        }
        table->insertPartialRow(q->specifiedColumns, q->values);
    } else {
        // Insert all columns
        table->insertRow(q->values);
    }
    
    // Save to CSV immediately
    std::string csvPath = "data/" + q->tableName + ".csv";
    table->saveToCSV(csvPath);
    
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
