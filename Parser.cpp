// src/Parser.cpp
#include "Parser.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"
#include "CreateTableQuery.h"
#include "DropTableQuery.h"
#include <sstream>
#include <algorithm>
#include <cctype>

// Helper functions from SQLParser.h and Helper.cpp adapted

std::string toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        result.push_back(trim(item));
    }
    return result;
}

std::string stripQuotes(std::string value) {
    value = trim(value);
    if (value.length() >= 2 && ((value.front() == '\'' && value.back() == '\'') || (value.front() == '"' && value.back() == '"'))) {
        return value.substr(1, value.length() - 2);
    }
    return value;
}

// Parse condition from WHERE clause (simple column op value)
Condition parseCondition(const std::string& wherePart) {
    Condition c;
    std::string opStr = "="; // Default
    size_t opPos = wherePart.find('=');
    if (opPos == std::string::npos) {
        opPos = wherePart.find('>');
        if (opPos != std::string::npos) opStr = ">";
    }
    if (opPos == std::string::npos) {
        opPos = wherePart.find('<');
        if (opPos != std::string::npos) opStr = "<";
    }
    // Add more ops

    if (opPos != std::string::npos) {
        c.column = trim(wherePart.substr(0, opPos));
        c.op = opStr;
        std::string valStr = trim(wherePart.substr(opPos + opStr.length()));
        c.value = Value(DataType::STRING, stripQuotes(valStr)); // Assume STRING
    }
    return c;
}

Query* Parser::parse(const std::string& sqlText) {
    std::string upperQuery = toUpper(trim(sqlText));

    if (upperQuery.find("SELECT") == 0) {
        SelectQuery* q = new SelectQuery();
        size_t fromPos = upperQuery.find("FROM");
        if (fromPos == std::string::npos) { delete q; return nullptr; }

        std::string columnsPart = trim(sqlText.substr(6, fromPos - 6));
        if (columnsPart == "*") {
            q->columns.push_back("*");
        } else {
            // Parse columns and aggregate functions
            auto colList = split(columnsPart, ',');
            for (const auto& col : colList) {
                std::string colTrimmed = trim(col);
                std::string colUpper = toUpper(colTrimmed);
                
                // Check if it's an aggregate function
                bool isAggregate = false;
                std::vector<std::string> aggFuncs = {"SUM(", "COUNT(", "AVG(", "MIN(", "MAX("};
                
                for (const auto& aggFunc : aggFuncs) {
                    if (colUpper.find(aggFunc) != std::string::npos) {
                        isAggregate = true;
                        
                        // Extract function name
                        size_t openParen = colUpper.find('(');
                        std::string funcName = colUpper.substr(0, openParen);
                        
                        // Extract column name inside parentheses
                        size_t closeParen = colTrimmed.find(')');
                        if (closeParen != std::string::npos) {
                            std::string innerCol = trim(colTrimmed.substr(openParen + 1, closeParen - openParen - 1));
                            
                            AggregateFunction agg;
                            agg.function = funcName;
                            agg.column = innerCol; // Can be "*" for COUNT(*)
                            agg.alias = colTrimmed; // Store original expression as alias
                            
                            q->aggregates.push_back(agg);
                        }
                        break;
                    }
                }
                
                // If not aggregate, add as regular column
                if (!isAggregate) {
                    q->columns.push_back(colTrimmed);
                }
            }
        }

        // Find various clause positions
        size_t wherePos = upperQuery.find("WHERE");
        size_t joinPos = upperQuery.find("JOIN");
        size_t groupByPos = upperQuery.find("GROUP BY");
        size_t orderByPos = upperQuery.find("ORDER BY");
        
        // Determine the end of the FROM clause
        size_t fromEnd = std::string::npos;
        std::vector<size_t> positions = {wherePos, joinPos, groupByPos, orderByPos};
        for (size_t pos : positions) {
            if (pos != std::string::npos && (fromEnd == std::string::npos || pos < fromEnd)) {
                fromEnd = pos;
            }
        }
        
        if (fromEnd == std::string::npos) {
            fromEnd = sqlText.length();
        }
        
        // Extract table name
        std::string tablePart = trim(sqlText.substr(fromPos + 4, fromEnd - fromPos - 4));
        q->tableName = tablePart;

        // Parse JOIN clauses
        size_t currentPos = fromEnd;
        while (joinPos != std::string::npos && joinPos >= currentPos) {
            JoinClause join;
            // Determine join type
            size_t innerPos = upperQuery.rfind("INNER", joinPos);
            size_t leftPos = upperQuery.rfind("LEFT", joinPos);
            size_t rightPos = upperQuery.rfind("RIGHT", joinPos);
            
            if (innerPos != std::string::npos && innerPos < joinPos && innerPos > currentPos) {
                join.joinType = "INNER";
            } else if (leftPos != std::string::npos && leftPos < joinPos && leftPos > currentPos) {
                join.joinType = "LEFT";
            } else if (rightPos != std::string::npos && rightPos < joinPos && rightPos > currentPos) {
                join.joinType = "RIGHT";
            } else {
                join.joinType = "INNER"; // Default
            }
            
            // Find ON clause
            size_t onPos = upperQuery.find("ON", joinPos);
            if (onPos != std::string::npos) {
                // Extract joined table name (between JOIN and ON, excluding INNER/LEFT/RIGHT)
                std::string joinTablePart = trim(sqlText.substr(joinPos + 4, onPos - joinPos - 4));
                
                // Remove INNER/LEFT/RIGHT from table name if present
                std::string joinTablePartUpper = toUpper(joinTablePart);
                if (joinTablePartUpper.find("INNER") == 0) {
                    joinTablePart = trim(joinTablePart.substr(5));
                } else if (joinTablePartUpper.find("LEFT") == 0) {
                    joinTablePart = trim(joinTablePart.substr(4));
                } else if (joinTablePartUpper.find("RIGHT") == 0) {
                    joinTablePart = trim(joinTablePart.substr(5));
                }
                
                join.tableName = joinTablePart;
                
                // Find next clause to determine end of ON
                size_t onEnd = std::string::npos;
                size_t nextJoin = upperQuery.find("JOIN", onPos);
                std::vector<size_t> nextPositions = {wherePos, nextJoin, groupByPos, orderByPos};
                for (size_t pos : nextPositions) {
                    if (pos != std::string::npos && pos > onPos && (onEnd == std::string::npos || pos < onEnd)) {
                        onEnd = pos;
                    }
                }
                if (onEnd == std::string::npos) onEnd = sqlText.length();
                
                // Parse ON condition (table1.col = table2.col)
                std::string onClause = trim(sqlText.substr(onPos + 2, onEnd - onPos - 2));
                size_t eqPos = onClause.find('=');
                if (eqPos != std::string::npos) {
                    std::string leftSide = trim(onClause.substr(0, eqPos));
                    std::string rightSide = trim(onClause.substr(eqPos + 1));
                    
                    // Extract column names (may include table prefix)
                    size_t dotPos = leftSide.find('.');
                    if (dotPos != std::string::npos) {
                        join.leftColumn = trim(leftSide.substr(dotPos + 1));
                    } else {
                        join.leftColumn = leftSide;
                    }
                    
                    dotPos = rightSide.find('.');
                    if (dotPos != std::string::npos) {
                        join.rightColumn = trim(rightSide.substr(dotPos + 1));
                    } else {
                        join.rightColumn = rightSide;
                    }
                }
                
                q->joins.push_back(join);
                currentPos = onEnd;
                joinPos = upperQuery.find("JOIN", currentPos);
            } else {
                break;
            }
        }

        // Parse WHERE clause
        if (wherePos != std::string::npos) {
            size_t whereEnd = std::string::npos;
            std::vector<size_t> nextPositions = {groupByPos, orderByPos};
            for (size_t pos : nextPositions) {
                if (pos != std::string::npos && (whereEnd == std::string::npos || pos < whereEnd)) {
                    whereEnd = pos;
                }
            }
            if (whereEnd == std::string::npos) whereEnd = sqlText.length();
            
            std::string wherePart = trim(sqlText.substr(wherePos + 5, whereEnd - wherePos - 5));
            q->where = parseCondition(wherePart);
        }

        // Parse GROUP BY
        if (groupByPos != std::string::npos) {
            size_t groupByEnd = orderByPos != std::string::npos ? orderByPos : sqlText.length();
            std::string groupByPart = trim(sqlText.substr(groupByPos + 8, groupByEnd - groupByPos - 8));
            q->groupBy = split(groupByPart, ',');
        }

        // Parse ORDER BY
        if (orderByPos != std::string::npos) {
            std::string orderByPart = trim(sqlText.substr(orderByPos + 8));
            auto orderItems = split(orderByPart, ',');
            for (const auto& item : orderItems) {
                SortRule rule;
                std::string itemUpper = toUpper(item);
                if (itemUpper.find("DESC") != std::string::npos) {
                    rule.ascending = false;
                    rule.column = trim(item.substr(0, item.find_last_of(' ')));
                } else {
                    rule.ascending = true;
                    // Remove ASC if present
                    if (itemUpper.find("ASC") != std::string::npos) {
                        rule.column = trim(item.substr(0, item.find_last_of(' ')));
                    } else {
                        rule.column = trim(item);
                    }
                }
                q->orderBy.push_back(rule);
            }
        }

        return q;
    } else if (upperQuery.find("INSERT") == 0) {
        InsertQuery* q = new InsertQuery();
        size_t intoPos = upperQuery.find("INTO");
        size_t valuesPos = upperQuery.find("VALUES");
        if (intoPos == std::string::npos || valuesPos == std::string::npos) { delete q; return nullptr; }

        // Extract table name and optionally specified columns
        std::string tablePart = trim(sqlText.substr(intoPos + 4, valuesPos - intoPos - 4));
        size_t parenPos = tablePart.find('(');
        
        if (parenPos != std::string::npos) {
            // INSERT INTO table(col1, col2) VALUES(...)
            q->tableName = trim(tablePart.substr(0, parenPos));
            size_t closeParenPos = tablePart.find(')');
            if (closeParenPos == std::string::npos) { delete q; return nullptr; }
            std::string colsPart = tablePart.substr(parenPos + 1, closeParenPos - parenPos - 1);
            auto colNames = split(colsPart, ',');
            for (const auto& col : colNames) {
                q->specifiedColumns.push_back(trim(col));
            }
        } else {
            // INSERT INTO table VALUES(...)
            q->tableName = tablePart;
        }

        size_t openParen = sqlText.find('(', valuesPos);
        size_t closeParen = sqlText.rfind(')');
        if (openParen == std::string::npos || closeParen == std::string::npos) { delete q; return nullptr; }

        std::string valuesPart = sqlText.substr(openParen + 1, closeParen - openParen - 1);
        auto valueStrs = split(valuesPart, ',');
        for (const auto& vs : valueStrs) {
            q->values.values.emplace_back(DataType::STRING, stripQuotes(vs));
        }

        return q;
    } else if (upperQuery.find("UPDATE") == 0) {
        UpdateQuery* q = new UpdateQuery();
        size_t setPos = upperQuery.find("SET");
        if (setPos == std::string::npos) { delete q; return nullptr; }

        q->tableName = trim(sqlText.substr(6, setPos - 6));

        size_t wherePos = upperQuery.find("WHERE");
        std::string setPart;
        if (wherePos != std::string::npos) {
            setPart = trim(sqlText.substr(setPos + 3, wherePos - setPos - 3));
            std::string wherePart = trim(sqlText.substr(wherePos + 5));
            q->where = parseCondition(wherePart);
        } else {
            setPart = trim(sqlText.substr(setPos + 3));
        }

        // Parse set: assume single column=value
        size_t eqPos = setPart.find('=');
        if (eqPos != std::string::npos) {
            std::string col = trim(setPart.substr(0, eqPos));
            std::string val = stripQuotes(trim(setPart.substr(eqPos + 1)));
            q->newValues[col] = Value(DataType::STRING, val);
        }

        return q;
    } else if (upperQuery.find("DELETE") == 0) {
        DeleteQuery* q = new DeleteQuery();
        size_t fromPos = upperQuery.find("FROM");
        if (fromPos == std::string::npos) { delete q; return nullptr; }

        size_t wherePos = upperQuery.find("WHERE");
        if (wherePos != std::string::npos) {
            q->tableName = trim(sqlText.substr(fromPos + 4, wherePos - fromPos - 4));
            std::string wherePart = trim(sqlText.substr(wherePos + 5));
            q->where = parseCondition(wherePart);
        } else {
            q->tableName = trim(sqlText.substr(fromPos + 4));
        }

        return q;
    } else if (upperQuery.find("CREATE") == 0 && upperQuery.find("TABLE") != std::string::npos) {
        CreateTableQuery* q = new CreateTableQuery();
        size_t tablePos = upperQuery.find("TABLE");
        size_t openParen = sqlText.find('(', tablePos);
        size_t closeParen = sqlText.rfind(')');
        
        if (openParen == std::string::npos || closeParen == std::string::npos) {
            delete q;
            return nullptr;
        }

        // Extract table name and validate proper spacing
        std::string tableNamePart = sqlText.substr(tablePos + 5, openParen - tablePos - 5);
        q->tableName = trim(tableNamePart);
        
        // Validate table name is not empty (ensures space after TABLE keyword)
        if (q->tableName.empty()) {
            delete q;
            return nullptr;
        }
        
        std::string colsDef = sqlText.substr(openParen + 1, closeParen - openParen - 1);

        // Parse columns: column_name TYPE [PRIMARY KEY] [REFERENCES table(column)], ...
        auto defs = split(colsDef, ',');
        for (auto& def : defs) {
            std::string defUpper = toUpper(def);
            auto parts = split(trim(def), ' ');
            if (parts.size() >= 2) {
                Column col;
                col.name = parts[0];
                
                // Parse data type
                DataType type = DataType::STRING;
                std::string typeStr = toUpper(parts[1]);
                if (typeStr.find("INT") != std::string::npos) type = DataType::INTEGER;
                else if (typeStr.find("VARCHAR") != std::string::npos) type = DataType::VARCHAR;
                else if (typeStr.find("FLOAT") != std::string::npos || typeStr.find("DOUBLE") != std::string::npos) type = DataType::FLOAT;
                else if (typeStr.find("BOOL") != std::string::npos) type = DataType::BOOLEAN;
                col.type = type;
                
                // Check for PRIMARY KEY
                if (defUpper.find("PRIMARY") != std::string::npos && defUpper.find("KEY") != std::string::npos) {
                    col.isPrimaryKey = true;
                }
                
                // Check for FOREIGN KEY (REFERENCES table(column))
                size_t refPos = defUpper.find("REFERENCES");
                if (refPos != std::string::npos) {
                    col.isForeignKey = true;
                    std::string refPart = trim(def.substr(refPos + 10));
                    size_t parenPos = refPart.find('(');
                    if (parenPos != std::string::npos) {
                        col.foreignTable = trim(refPart.substr(0, parenPos));
                        size_t closeParenPos = refPart.find(')');
                        if (closeParenPos != std::string::npos) {
                            col.foreignColumn = trim(refPart.substr(parenPos + 1, closeParenPos - parenPos - 1));
                        }
                    }
                }
                
                q->columns.push_back(col);
            }
        }

        if (q->columns.empty()) {
            delete q;
            return nullptr;
        }

        return q;
    } else if (upperQuery.find("DROP") == 0 && upperQuery.find("TABLE") != std::string::npos) {
        DropTableQuery* q = new DropTableQuery();
        size_t tablePos = upperQuery.find("TABLE");
        if (tablePos == std::string::npos) {
            delete q;
            return nullptr;
        }

        q->tableName = trim(sqlText.substr(tablePos + 5));
        
        if (q->tableName.empty()) {
            delete q;
            return nullptr;
        }

        return q;
    }

    return nullptr;
}
