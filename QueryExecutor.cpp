// src/QueryExecutor.cpp
#include "QueryExecutor.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"
#include "CreateTableQuery.h"
#include "DropTableQuery.h"
#include <algorithm>

using namespace std;

// Helper function to get type name as string
static string getTypeName(DataType type) {
    switch (type) {
        case DataType::INTEGER: return "INTEGER";
        case DataType::FLOAT: return "FLOAT";
        case DataType::BOOLEAN: return "BOOLEAN";
        case DataType::STRING: return "STRING";
        case DataType::VARCHAR: return "VARCHAR";
        case DataType::DATE: return "DATE";
        default: return "UNKNOWN";
    }
}

// Helper function to extract column name from qualified name (e.g., "alias.column" -> "column")
static string extractColumnName(const string& colName, const string& tableAlias) {
    size_t dotPos = colName.find('.');
    if (dotPos != string::npos) {
        // Has prefix (e.g., "alias.column")
        string prefix = colName.substr(0, dotPos);
        string suffix = colName.substr(dotPos + 1);
        
        // Check if prefix matches the table alias
        if (prefix == tableAlias) {
            return suffix; // Return just the column name
        }
        // If prefix doesn't match, return as is (will fail validation later)
        return colName;
    }
    // No prefix, return as is
    return colName;
}

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

    // Validate WHERE column exists (if specified)
    if (!q->where.column.empty()) {
        const auto& columns = table->getColumns();
        bool found = false;
        for (const auto& col : columns) {
            if (col.name == q->where.column) {
                found = true;
                break;
            }
        }
        if (!found) {
            error("Column not found in WHERE clause: " + q->where.column);
            return;
        }
    }

    vector<Row> selected = table->selectRows(q->where);

    // Handle JOINs
    vector<Column> allColumns = table->getColumns();
    vector<Row> joinedRows = selected;
    
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
        vector<Row> newJoinedRows;
        
        // Process based on join type
        if (join.joinType == "INNER") {
            // INNER JOIN: only include matching rows
            for (const auto& leftRow : joinedRows) {
                // Validate leftColIdx is in range
                if (leftColIdx >= leftRow.values.size()) continue;
                
                for (const auto& rightRow : joinTableRows) {
                    // Validate rightColIdx is in range
                    if (rightColIdx >= rightRow.values.size()) continue;
                    
                    // For JOIN condition: handle NULL values properly
                    // NULL values never match anything (including other NULLs) in SQL JOIN semantics
                    const Value& leftVal = leftRow.values[leftColIdx];
                    const Value& rightVal = rightRow.values[rightColIdx];
                    
                    // Only match if both are non-NULL and equal
                    if (!leftVal.isNull && !rightVal.isNull && leftVal.data == rightVal.data) {
                        // Merge rows
                        Row mergedRow;
                        mergedRow.values = leftRow.values;
                        for (const auto& val : rightRow.values) {
                            mergedRow.values.push_back(val);
                        }
                        newJoinedRows.push_back(mergedRow);
                    }
                }
            }
        } else if (join.joinType == "LEFT") {
            // LEFT JOIN: include all left rows, matching right rows where possible
            for (const auto& leftRow : joinedRows) {
                bool leftHasMatch = false;
                
                // Validate leftColIdx is in range
                if (leftColIdx < leftRow.values.size()) {
                    const Value& leftVal = leftRow.values[leftColIdx];
                    
                    for (const auto& rightRow : joinTableRows) {
                        // Validate rightColIdx is in range
                        if (rightColIdx >= rightRow.values.size()) continue;
                        
                        const Value& rightVal = rightRow.values[rightColIdx];
                        
                        // Only match if both are non-NULL and equal
                        if (!leftVal.isNull && !rightVal.isNull && leftVal.data == rightVal.data) {
                            // Merge rows
                            Row mergedRow;
                            mergedRow.values = leftRow.values;
                            for (const auto& val : rightRow.values) {
                                mergedRow.values.push_back(val);
                            }
                            newJoinedRows.push_back(mergedRow);
                            leftHasMatch = true;
                        }
                    }
                }
                
                // For LEFT JOIN, if no match found, include left row with NULL right values
                if (!leftHasMatch) {
                    Row mergedRow;
                    mergedRow.values = leftRow.values;
                    // Add NULL values for all joined table columns
                    for (const auto& col : joinTableColumns) {
                        mergedRow.values.push_back(Value::createNull(col.type));
                    }
                    newJoinedRows.push_back(mergedRow);
                }
            }
        } else if (join.joinType == "RIGHT") {
            // RIGHT JOIN: include all right rows, matching left rows where possible
            vector<bool> rightRowMatched(joinTableRows.size(), false);
            
            // Store the number of columns before adding the join table columns
            size_t leftColumnsCount = allColumns.size();
            
            // First pass: find and add all matching rows
            for (const auto& leftRow : joinedRows) {
                // Validate leftColIdx is in range
                if (leftColIdx >= leftRow.values.size()) continue;
                
                const Value& leftVal = leftRow.values[leftColIdx];
                
                for (size_t rightIdx = 0; rightIdx < joinTableRows.size(); ++rightIdx) {
                    const auto& rightRow = joinTableRows[rightIdx];
                    
                    // Validate rightColIdx is in range
                    if (rightColIdx >= rightRow.values.size()) continue;
                    
                    const Value& rightVal = rightRow.values[rightColIdx];
                    
                    // Only match if both are non-NULL and equal
                    if (!leftVal.isNull && !rightVal.isNull && leftVal.data == rightVal.data) {
                        // Merge rows
                        Row mergedRow;
                        mergedRow.values = leftRow.values;
                        for (const auto& val : rightRow.values) {
                            mergedRow.values.push_back(val);
                        }
                        newJoinedRows.push_back(mergedRow);
                        rightRowMatched[rightIdx] = true;
                    }
                }
            }
            
            // Second pass: add all unmatched right rows with NULL left values
            for (size_t rightIdx = 0; rightIdx < joinTableRows.size(); ++rightIdx) {
                if (!rightRowMatched[rightIdx]) {
                    Row mergedRow;
                    // Add NULL values for all left columns (before this join)
                    for (size_t i = 0; i < leftColumnsCount; ++i) {
                        mergedRow.values.push_back(Value::createNull(allColumns[i].type));
                    }
                    // Add actual values from the unmatched right row
                    for (const auto& val : joinTableRows[rightIdx].values) {
                        mergedRow.values.push_back(val);
                    }
                    newJoinedRows.push_back(mergedRow);
                }
            }
        }
        
        joinedRows = newJoinedRows;
        // Add joined table columns to column list
        for (const auto& col : joinTableColumns) {
            allColumns.push_back(col);
        }
    }

    // Handle GROUP BY
    vector<Row> groupedRows = joinedRows;
    vector<Column> groupedColumns = allColumns;
    
    if (!q->groupBy.empty() || !q->aggregates.empty()) {
        // Validate GROUP BY columns exist
        for (const auto& colName : q->groupBy) {
            bool found = false;
            for (const auto& col : allColumns) {
                if (col.name == colName) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                error("Column not found in GROUP BY clause: " + colName);
                return;
            }
        }
        
        // Validate aggregate columns exist (except COUNT(*))
        for (const auto& agg : q->aggregates) {
            if (agg.column != "*") {
                bool found = false;
                for (const auto& col : allColumns) {
                    if (col.name == agg.column) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    error("Column not found in aggregate function: " + agg.column);
                    return;
                }
            }
        }
        
        // Find group by column indices
        vector<size_t> groupByIndices;
        for (const auto& colName : q->groupBy) {
            for (size_t i = 0; i < allColumns.size(); ++i) {
                if (allColumns[i].name == colName) {
                    groupByIndices.push_back(i);
                    break;
                }
            }
        }
        
        // Group rows by the specified columns
        map<string, vector<Row>> groups;
        for (const auto& row : joinedRows) {
            string groupKey;
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
        groupedRows.clear();
        
        // Calculate aggregates for each group
        groupedRows.clear();
        groupedColumns.clear();
        
        // If we have aggregates but explicit columns selected, use SELECT order
        // Otherwise, GROUP BY columns first, then aggregates
        if (!q->columns.empty() && q->columns[0] != "*") {
            // User specified column order in SELECT - respect it
            // First add non-aggregate columns from SELECT
            for (const auto& colName : q->columns) {
                for (const auto& col : allColumns) {
                    if (col.name == colName) {
                        groupedColumns.push_back(col);
                        break;
                    }
                }
            }
            
            // Then add aggregate columns in SELECT order
            for (const auto& agg : q->aggregates) {
                Column aggCol;
                aggCol.name = agg.alias;
                aggCol.type = DataType::FLOAT;
                groupedColumns.push_back(aggCol);
            }
        } else {
            // No explicit SELECT columns, use default order: GROUP BY columns first
            for (const auto& colName : q->groupBy) {
                for (const auto& col : allColumns) {
                    if (col.name == colName) {
                        groupedColumns.push_back(col);
                        break;
                    }
                }
            }
            
            // Then add aggregate columns
            for (const auto& agg : q->aggregates) {
                Column aggCol;
                aggCol.name = agg.alias;
                aggCol.type = DataType::FLOAT;
                groupedColumns.push_back(aggCol);
            }
        }
        
        // Process each group
        for (const auto& pair : groups) {
            const auto& groupRows = pair.second;
            if (groupRows.empty()) continue;
            
            Row resultRow;
            
            // Build row in the same order as groupedColumns
            // If SELECT specified columns, follow that order
            if (!q->columns.empty() && q->columns[0] != "*") {
                // Process in SELECT order
                for (const auto& col : groupedColumns) {
                    bool isAggregate = false;
                    
                    // Check if this column is an aggregate
                    for (const auto& agg : q->aggregates) {
                        if (col.name == agg.alias) {
                            isAggregate = true;
                            // Calculate aggregate (will be added below)
                            break;
                        }
                    }
                    
                    if (!isAggregate) {
                        // It's a GROUP BY column, add its value
                        for (size_t idx : groupByIndices) {
                            if (allColumns[idx].name == col.name && idx < groupRows[0].values.size()) {
                                resultRow.values.push_back(groupRows[0].values[idx]);
                                break;
                            }
                        }
                    }
                }
            } else {
                // Default order: GROUP BY columns first
                for (size_t idx : groupByIndices) {
                    if (idx < groupRows[0].values.size()) {
                        resultRow.values.push_back(groupRows[0].values[idx]);
                    }
                }
            }
            
            // Calculate and add aggregate values in order
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
                                double val = stod(row.values[colIdx].data);
                                
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
                
                resultRow.values.emplace_back(DataType::FLOAT, to_string(result));
            }
            
            groupedRows.push_back(resultRow);
        }
        
        // Update allColumns to reflect grouped columns
        allColumns = groupedColumns;
    }

    // Handle ORDER BY
    if (!q->orderBy.empty()) {
        // Validate ORDER BY columns exist
        for (const auto& rule : q->orderBy) {
            bool found = false;
            for (const auto& col : allColumns) {
                if (col.name == rule.column) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                error("Column not found in ORDER BY clause: " + rule.column);
                return;
            }
        }
        
        vector<size_t> orderByIndices;
        for (const auto& rule : q->orderBy) {
            for (size_t i = 0; i < allColumns.size(); ++i) {
                if (allColumns[i].name == rule.column) {
                    orderByIndices.push_back(i);
                    break;
                }
            }
        }
        
        sort(groupedRows.begin(), groupedRows.end(), [&](const Row& a, const Row& b) {
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
    vector<Column> resultColumns;
    vector<Row> projectedRows;
    
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
        // Build column index mapping with table prefix support
        map<string, size_t> columnIndexMap;
        map<string, vector<size_t>> tableColumnMap; // table/alias -> list of column indices
        
        // Build maps for quick lookup
        size_t currentTableColCount = table->getColumns().size();
        
        // Map main table columns
        for (size_t i = 0; i < currentTableColCount; ++i) {
            columnIndexMap[allColumns[i].name] = i;
            tableColumnMap[q->tableName].push_back(i);
            if (!q->tableAlias.empty()) {
                tableColumnMap[q->tableAlias].push_back(i);
            }
        }
        
        // Map joined table columns
        size_t colOffset = currentTableColCount;
        for (const auto& join : q->joins) {
            Table* joinTable = db.getTable(join.tableName);
            if (joinTable) {
                size_t joinColCount = joinTable->getColumns().size();
                for (size_t i = 0; i < joinColCount; ++i) {
                    size_t globalIdx = colOffset + i;
                    columnIndexMap[allColumns[globalIdx].name] = globalIdx;
                    tableColumnMap[join.tableName].push_back(globalIdx);
                }
                colOffset += joinColCount;
            }
        }
        
        // Also build a map for all columns
        for (size_t i = 0; i < allColumns.size(); ++i) {
            columnIndexMap[allColumns[i].name] = i;
        }
        
        // Get indices of requested columns, expanding alias.* patterns
        vector<size_t> selectedIndices;
        for (const auto& colName : q->columns) {
            // Check if it's a wildcard pattern (alias.* or table.*)
            size_t dotPos = colName.find('.');
            if (dotPos != string::npos) {
                string prefix = colName.substr(0, dotPos);
                string suffix = colName.substr(dotPos + 1);
                
                if (suffix == "*") {
                    // Expand table.* or alias.*
                    auto it = tableColumnMap.find(prefix);
                    if (it != tableColumnMap.end()) {
                        // Add all columns from this table
                        for (size_t idx : it->second) {
                            selectedIndices.push_back(idx);
                            resultColumns.push_back(allColumns[idx]);
                        }
                    } else {
                        // Check if it's in tableAliases map
                        auto aliasIt = q->tableAliases.find(prefix);
                        if (aliasIt != q->tableAliases.end()) {
                            // Look up by actual table name
                            auto tableIt = tableColumnMap.find(aliasIt->second);
                            if (tableIt != tableColumnMap.end()) {
                                for (size_t idx : tableIt->second) {
                                    selectedIndices.push_back(idx);
                                    resultColumns.push_back(allColumns[idx]);
                                }
                            }
                        } else {
                            error("Table or alias not found: " + prefix);
                            return;
                        }
                    }
                } else {
                    // It's table.column or alias.column - just use the column name
                    auto it = columnIndexMap.find(suffix);
                    if (it != columnIndexMap.end()) {
                        selectedIndices.push_back(it->second);
                        resultColumns.push_back(allColumns[it->second]);
                    } else {
                        error("Column not found: " + colName);
                        return;
                    }
                }
            } else {
                // Regular column name
                auto it = columnIndexMap.find(colName);
                if (it != columnIndexMap.end()) {
                    selectedIndices.push_back(it->second);
                    resultColumns.push_back(allColumns[it->second]);
                } else {
                    error("Column not found: " + colName);
                    return;
                }
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
    if(!(resultColumns.size()==0 && projectedRows.size()==0))
    resultTable(resultColumns, projectedRows);

    output("(" + to_string(projectedRows.size()) + " row(s) selected)",false);
}

void QueryExecutor::executeInsert(InsertQuery* q, Database& db) {
    Table* table = db.getTable(q->tableName);
    if (!table) {
        error("Table not found: " + q->tableName);
        return;
    }

    // If specific columns are specified, use insertPartialRow
    if (!q->specifiedColumns.empty()) {
        // Validate columns exist and types match
        const auto& columns = table->getColumns();
        for (size_t i = 0; i < q->specifiedColumns.size() && i < q->values.values.size(); ++i) {
            size_t colIdx = table->getColumnIndex(q->specifiedColumns[i]);
            if (colIdx == static_cast<size_t>(-1)) {
                error("Column not found: " + q->specifiedColumns[i]);
                return;
            }
            
            // Validate type compatibility
            if (!q->values.values[i].isValidForType(columns[colIdx].type)) {
                error("Type mismatch for column '" + q->specifiedColumns[i] + 
                      "': cannot insert value '" + q->values.values[i].data + 
                      "' into " + getTypeName(columns[colIdx].type) + " column");
                return;
            }
        }
        
        if (!table->insertPartialRow(q->specifiedColumns, q->values)) {
            error("Failed to insert row: constraint violation");
            return;
        }
    } else {

        // Insert all columns
        // Validate all column types match
        const auto& columns = table->getColumns();
        if (q->values.values.size() != columns.size()) {
            error("Column count mismatch: expected " + to_string(columns.size()) + 
                  ", got " + to_string(q->values.values.size()));
            return;
        }
        
        for (size_t i = 0; i < q->values.values.size(); ++i) {
            if (!q->values.values[i].isValidForType(columns[i].type)) {
                error("Type mismatch for column '" + columns[i].name + 
                      "': cannot insert value '" + q->values.values[i].data + 
                      "' into " + getTypeName(columns[i].type) + " column");
                return;
            }
        }
        
        if (!table->insertRow(q->values)) {
            error("Failed to insert row: constraint violation");
            return;
        }
        if(!table->insertRow(q->values))
            error("Error with keys");
    }
    
    // Save to CSV immediately
    // string csvPath = "data/" + q->tableName + ".csv";
    // table->saveToCSV(csvPath);
    
    output("1 row inserted",true);
}

void QueryExecutor::executeUpdate(UpdateQuery* q, Database& db) {
    Table* table = db.getTable(q->tableName);
    if (!table) {
        error("Table not found: " + q->tableName);
        return;
    }

    // Validate SET columns exist and types match
    const auto& columns = table->getColumns();
    map<string, Value> resolvedNewValues; // Column name (without alias) -> Value
    
    for (const auto& pair : q->newValues) {
        // Extract actual column name (strip alias prefix if present)
        string actualColName = extractColumnName(pair.first, q->tableAlias);
        
        bool found = false;
        const Column* targetCol = nullptr;
        for (const auto& col : columns) {
            if (col.name == actualColName) {
                found = true;
                targetCol = &col;
                break;
            }
        }
        if (!found) {
            error("Column not found: " + pair.first);
            return;
        }
        
        // Validate type compatibility
        if (targetCol && !pair.second.isValidForType(targetCol->type)) {
            error("Type mismatch for column '" + pair.first + 
                  "': cannot update with value '" + pair.second.data + 
                  "' into " + getTypeName(targetCol->type) + " column");
            return;
        }
        
        // Store with actual column name
        resolvedNewValues[actualColName] = pair.second;
    }
    
    // Validate WHERE column exists (if specified)
    Condition resolvedWhere = q->where;
    if (!q->where.column.empty()) {
        // Extract actual column name from WHERE clause
        string actualColName = extractColumnName(q->where.column, q->tableAlias);
        resolvedWhere.column = actualColName;
        
        bool found = false;
        for (const auto& col : columns) {
            if (col.name == actualColName) {
                found = true;
                break;
            }
        }
        if (!found) {
            error("Column not found in WHERE clause: " + q->where.column);
            return;
        }
    }

    table->updateRows(resolvedWhere, resolvedNewValues);
    
    // Save to CSV immediately
    string csvPath = "data/" + q->tableName + ".csv";
    table->saveToCSV(csvPath);
    
    output("Rows updated",true);
}

void QueryExecutor::executeDelete(DeleteQuery* q, Database& db) {
    Table* table = db.getTable(q->tableName);
    if (!table) {
        error("Table not found: " + q->tableName);
        return;
    }

    // Validate WHERE column exists (if specified)
    Condition resolvedWhere = q->where;
    if (!q->where.column.empty()) {
        // Extract actual column name from WHERE clause
        string actualColName = extractColumnName(q->where.column, q->tableAlias);
        resolvedWhere.column = actualColName;
        
        const auto& columns = table->getColumns();
        bool found = false;
        for (const auto& col : columns) {
            if (col.name == actualColName) {
                found = true;
                break;
            }
        }
        if (!found) {
            error("Column not found in WHERE clause: " + q->where.column);
            return;
        }
    }

    table->deleteRows(resolvedWhere);
    
    // Save to CSV immediately
    string csvPath = "data/" + q->tableName + ".csv";
    table->saveToCSV(csvPath);
    
    output("Rows deleted",true);
}

void QueryExecutor::executeCreateTable(CreateTableQuery* q, Database& db) {
    // Check if table already exists
    if (db.getTable(q->tableName)) {
        error("Table already exists: " + q->tableName);
        return;
    }

    db.createTable(q->tableName, q->columns);
    output("Table '" + q->tableName + "' created successfully",true);
    tree();
}

void QueryExecutor::executeDropTable(DropTableQuery* q, Database& db) {
    int droppedCount = 0;
    
    for (const auto& tableName : q->tableNames) {
        // Check if table exists
        if (!db.getTable(tableName)) {
            if (!q->ifExists) {
                error("Table not found: " + tableName);
                return;
            }
            // If IF EXISTS is specified, silently skip non-existent tables
            continue;
        }

        db.dropTable(tableName);
        droppedCount++;
    }
    
    if (droppedCount > 0) {
        if (droppedCount == 1) {
            output("Table '" + q->tableNames[0] + "' dropped successfully",true);
        } else {
            output(to_string(droppedCount) + " tables dropped successfully",true);
        }
        tree();
    } else if (q->ifExists) {
        output("No tables to drop",true);
    }
}
