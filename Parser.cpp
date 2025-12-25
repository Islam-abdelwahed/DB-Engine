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

using namespace std;

// Helper functions from SQLParser.h and Helper.cpp adapted

string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

vector<string> split(const string& str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delimiter)) {
        result.push_back(trim(item));
    }
    return result;
}

string stripQuotes(string value) {
    value = trim(value);
    if (value.length() >= 2 && ((value.front() == '\'' && value.back() == '\'') || (value.front() == '"' && value.back() == '"'))) {
        return value.substr(1, value.length() - 2);
    }
    return value;
}

// Infer data type from value string
DataType inferDataType(const string& value) {
    string trimmedValue = trim(value);
    
    // Check if it's quoted (string)
    if (trimmedValue.length() >= 2 && 
        ((trimmedValue.front() == '\'' && trimmedValue.back() == '\'') || 
         (trimmedValue.front() == '"' && trimmedValue.back() == '"'))) {
        return DataType::STRING;
    }
    
    // Check if it's a number
    if (!trimmedValue.empty()) {
        bool hasDecimal = false;
        bool isNumber = true;
        size_t start = 0;
        
        // Handle negative numbers
        if (trimmedValue[0] == '-' || trimmedValue[0] == '+') {
            start = 1;
        }
        
        for (size_t i = start; i < trimmedValue.length(); ++i) {
            if (trimmedValue[i] == '.') {
                if (hasDecimal) {
                    isNumber = false;
                    break;
                }
                hasDecimal = true;
            } else if (!isdigit(trimmedValue[i])) {
                isNumber = false;
                break;
            }
        }
        
        if (isNumber && trimmedValue.length() > start) {
            return hasDecimal ? DataType::FLOAT : DataType::INTEGER;
        }
    }
    
    // Check for boolean
    string upper = toUpper(trimmedValue);
    if (upper == "TRUE" || upper == "FALSE" || upper == "1" || upper == "0") {
        return DataType::BOOLEAN;
    }
    
    // Default to string
    return DataType::STRING;
}

// Validate SQL identifier (table/column name)
bool isValidIdentifier(const string& name) {
    if (name.empty()) return false;
    
    // Must start with letter or underscore
    if (!isalpha(name[0]) && name[0] != '_') return false;
    
    // Rest must be alphanumeric or underscore
    for (size_t i = 1; i < name.length(); ++i) {
        if (!isalnum(name[i]) && name[i] != '_') return false;
    }
    
    return true;
}

// Check if keyword has proper spacing (next char must be whitespace)
bool hasProperSpacing(const string& upperQuery, const string& keyword, size_t keywordPos) {
    size_t endPos = keywordPos + keyword.length();
    if (endPos >= upperQuery.length()) return true; // End of query is OK
    
    char nextChar = upperQuery[endPos];
    return isspace(nextChar) || nextChar == '(' || nextChar == ';';
}

// Validate data type string is valid
bool isValidDataType(const string& typeStr) {
    string upper = toUpper(typeStr);
    return (upper.find("INT") != string::npos ||
            upper.find("VARCHAR") != string::npos ||
            upper.find("FLOAT") != string::npos ||
            upper.find("DOUBLE") != string::npos ||
            upper.find("BOOL") != string::npos ||
            upper == "STRING" ||
            upper == "TEXT");
}

// Parse condition from WHERE clause with AND/OR support
Condition parseCondition(const string& wherePart) {
    string wherePartTrimmed = trim(wherePart);
    string wherePartUpper = toUpper(wherePartTrimmed);
    
    // Check for OR first (lower precedence)
    size_t orPos = wherePartUpper.find(" OR ");
    if (orPos != string::npos) {
        Condition c;
        c.logicalOp = LogicalOperator::OR;
        
        string leftPart = trim(wherePart.substr(0, orPos));
        string rightPart = trim(wherePart.substr(orPos + 4));
        
        c.left = make_unique<Condition>(parseCondition(leftPart));
        c.right = make_unique<Condition>(parseCondition(rightPart));
        
        return c;
    }
    
    // Check for AND (higher precedence)
    size_t andPos = wherePartUpper.find(" AND ");
    if (andPos != string::npos) {
        Condition c;
        c.logicalOp = LogicalOperator::AND;
        
        string leftPart = trim(wherePart.substr(0, andPos));
        string rightPart = trim(wherePart.substr(andPos + 5));
        
        c.left = make_unique<Condition>(parseCondition(leftPart));
        c.right = make_unique<Condition>(parseCondition(rightPart));
        
        return c;
    }
    
    // Parse simple condition (column op value)
    Condition c;
    c.logicalOp = LogicalOperator::NONE;
    
    string opStr = "="; // Default
    size_t opPos = string::npos;
    
    // Check for two-character operators first
    opPos = wherePart.find("!=");
    if (opPos != string::npos) {
        opStr = "!=";
    } else {
        opPos = wherePart.find("<>");
        if (opPos != string::npos) {
            opStr = "<>";
        } else {
            opPos = wherePart.find(">=");
            if (opPos != string::npos) {
                opStr = ">=";
            } else {
                opPos = wherePart.find("<=");
                if (opPos != string::npos) {
                    opStr = "<=";
                }
            }
        }
    }
    
    // If no two-character operator found, check for single-character operators
    if (opPos == string::npos) {
        opPos = wherePart.find('=');
        if (opPos != string::npos) {
            opStr = "=";
        }
    }
    if (opPos == string::npos) {
        opPos = wherePart.find('>');
        if (opPos != string::npos) opStr = ">";
    }
    if (opPos == string::npos) {
        opPos = wherePart.find('<');
        if (opPos != string::npos) opStr = "<";
    }

    if (opPos != string::npos) {
        c.column = trim(wherePart.substr(0, opPos));
        c.op = opStr;
        string valStr = trim(wherePart.substr(opPos + opStr.length()));
        DataType inferredType = inferDataType(valStr);
        c.value = Value(inferredType, stripQuotes(valStr));
    }
    return c;
}

Query* Parser::parse(const string& sqlText) {
    string upperQuery = toUpper(trim(sqlText));

    if (upperQuery.find("SELECT") == 0) {
        // Validate SELECT has proper spacing
        if (!hasProperSpacing(upperQuery, "SELECT", 0)) {
            return nullptr;
        }
        
        SelectQuery* q = new SelectQuery();
        size_t fromPos = upperQuery.find("FROM");
        if (fromPos == string::npos) { delete q; return nullptr; }

        // Validate FROM has proper spacing
        if (!hasProperSpacing(upperQuery, "FROM", fromPos)) {
            delete q;
            return nullptr;
        }

        string columnsPart = trim(sqlText.substr(6, fromPos - 6));
        
        // Validate that columns part is not empty
        if (columnsPart.empty()) {
            delete q;
            return nullptr; // Invalid: SELECT without columns
        }
        
        if (columnsPart == "*") {
            q->columns.push_back("*");
        } else {
            // Parse columns and aggregate functions
            auto colList = split(columnsPart, ',');
            for (const auto& col : colList) {
                string colTrimmed = trim(col);
                string colUpper = toUpper(colTrimmed);
                
                // Check if it's an aggregate function
                bool isAggregate = false;
                vector<string> aggFuncs = {"SUM", "COUNT", "AVG", "MIN", "MAX"};
                
                for (const auto& aggFunc : aggFuncs) {
                    if (colUpper.find(aggFunc) == 0) {
                        size_t openParen = colTrimmed.find('(');
                        if (openParen != string::npos) {
                            string between = trim(colTrimmed.substr(aggFunc.length(), openParen - aggFunc.length()));
                            if (between.empty()) {
                                isAggregate = true;
                                
                                // Extract function name
                                string funcName = aggFunc;
                                
                                // Extract column name inside parentheses
                                size_t closeParen = colTrimmed.find(')');
                                if (closeParen != string::npos) {
                                    string innerCol = trim(colTrimmed.substr(openParen + 1, closeParen - openParen - 1));
                                    
                                    AggregateFunction agg;
                                    agg.function = funcName;
                                    agg.column = innerCol; // Can be "*" for COUNT(*)
                                    agg.alias = colTrimmed; // Store original expression as alias
                                    
                                    q->aggregates.push_back(agg);
                                }
                                break;
                            }
                        }
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
        size_t fromEnd = string::npos;
        vector<size_t> positions = {wherePos, joinPos, groupByPos, orderByPos};
        for (size_t pos : positions) {
            if (pos != string::npos && (fromEnd == string::npos || pos < fromEnd)) {
                fromEnd = pos;
            }
        }
        
        if (fromEnd == string::npos) {
            fromEnd = sqlText.length();
        }
        
        // Extract table name and handle alias
        string tablePart = trim(sqlText.substr(fromPos + 4, fromEnd - fromPos - 4));
        
        // Create a map to store table aliases for resolving references
        map<string, string> tableAliases; // alias -> actual table name
        
        // Parse table name and alias (e.g., "ahmed a" -> table: "ahmed", alias: "a")
        auto tableParts = split(tablePart, ' ');
        if (tableParts.size() >= 2) {
            q->tableName = tableParts[0];
            q->tableAlias = tableParts[1];
            tableAliases[tableParts[1]] = tableParts[0]; // alias -> table
        } else {
            q->tableName = tablePart;
            q->tableAlias = tablePart; // No separate alias
            tableAliases[tablePart] = tablePart; // table can reference itself
        }
        
        // Store table aliases in query for later use
        q->tableAliases = tableAliases;

        // Parse JOIN clauses
        size_t currentPos = fromEnd;
        while (joinPos != string::npos && joinPos >= currentPos) {
            JoinClause join;
            // Determine join type
            size_t innerPos = upperQuery.rfind("INNER", joinPos);
            size_t leftPos = upperQuery.rfind("LEFT", joinPos);
            size_t rightPos = upperQuery.rfind("RIGHT", joinPos);
            
            if (innerPos != string::npos && innerPos < joinPos && innerPos > currentPos) {
                join.joinType = "INNER";
            } else if (leftPos != string::npos && leftPos < joinPos && leftPos > currentPos) {
                join.joinType = "LEFT";
            } else if (rightPos != string::npos && rightPos < joinPos && rightPos > currentPos) {
                join.joinType = "RIGHT";
            } else {
                join.joinType = "INNER"; // Default
            }
            
            // Find ON clause
            size_t onPos = upperQuery.find("ON", joinPos);
            if (onPos != string::npos) {
                // Extract joined table name (between JOIN and ON, excluding INNER/LEFT/RIGHT)
                string joinTablePart = trim(sqlText.substr(joinPos + 4, onPos - joinPos - 4));
                
                // Remove INNER/LEFT/RIGHT from table name if present
                string joinTablePartUpper = toUpper(joinTablePart);
                if (joinTablePartUpper.find("INNER") == 0) {
                    joinTablePart = trim(joinTablePart.substr(5));
                } else if (joinTablePartUpper.find("LEFT") == 0) {
                    joinTablePart = trim(joinTablePart.substr(4));
                } else if (joinTablePartUpper.find("RIGHT") == 0) {
                    joinTablePart = trim(joinTablePart.substr(5));
                }
                
                // Parse table name and alias (e.g., "ali l" -> table: "ali", alias: "l")
                auto joinTableParts = split(joinTablePart, ' ');
                if (joinTableParts.size() >= 2) {
                    join.tableName = joinTableParts[0];
                    tableAliases[joinTableParts[1]] = joinTableParts[0]; // alias -> table
                    q->tableAliases[joinTableParts[1]] = joinTableParts[0]; // Update query's alias map
                } else {
                    join.tableName = joinTablePart;
                    tableAliases[joinTablePart] = joinTablePart; // table can reference itself
                    q->tableAliases[joinTablePart] = joinTablePart; // Update query's alias map
                }
                
                // Find next clause to determine end of ON
                size_t onEnd = string::npos;
                size_t nextJoin = upperQuery.find("JOIN", onPos);
                vector<size_t> nextPositions = {wherePos, nextJoin, groupByPos, orderByPos};
                for (size_t pos : nextPositions) {
                    if (pos != string::npos && pos > onPos && (onEnd == string::npos || pos < onEnd)) {
                        onEnd = pos;
                    }
                }
                if (onEnd == string::npos) onEnd = sqlText.length();
                
                // Parse ON condition (e.g., "a.id = l.id")
                string onPart = trim(sqlText.substr(onPos + 2, onEnd - onPos - 2));
                
                // Find the equals sign
                size_t eqPos = onPart.find('=');
                if (eqPos != string::npos) {
                    string leftSide = trim(onPart.substr(0, eqPos));
                    string rightSide = trim(onPart.substr(eqPos + 1));
                    
                    // Parse left side (could be table.column or alias.column)
                    size_t dotPos = leftSide.find('.');
                    if (dotPos != string::npos) {
                        join.leftColumn = trim(leftSide.substr(dotPos + 1));
                    } else {
                        join.leftColumn = leftSide;
                    }
                    
                    // Parse right side (could be table.column or alias.column)
                    dotPos = rightSide.find('.');
                    if (dotPos != string::npos) {
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
        if (wherePos != string::npos) {
            size_t whereEnd = string::npos;
            vector<size_t> nextPositions = {groupByPos, orderByPos};
            for (size_t pos : nextPositions) {
                if (pos != string::npos && (whereEnd == string::npos || pos < whereEnd)) {
                    whereEnd = pos;
                }
            }
            if (whereEnd == string::npos) whereEnd = sqlText.length();
            
            string wherePart = trim(sqlText.substr(wherePos + 5, whereEnd - wherePos - 5));
            q->where = parseCondition(wherePart);
        }

        // Parse GROUP BY
        if (groupByPos != string::npos) {
            size_t groupByEnd = orderByPos != string::npos ? orderByPos : sqlText.length();
            string groupByPart = trim(sqlText.substr(groupByPos + 8, groupByEnd - groupByPos - 8));
            q->groupBy = split(groupByPart, ',');
        }

        // Parse ORDER BY
        if (orderByPos != string::npos) {
            string orderByPart = trim(sqlText.substr(orderByPos + 8));
            auto orderItems = split(orderByPart, ',');
            for (const auto& item : orderItems) {
                SortRule rule;
                string itemUpper = toUpper(item);
                if (itemUpper.find("DESC") != string::npos) {
                    rule.ascending = false;
                    rule.column = trim(item.substr(0, item.find_last_of(' ')));
                } else {
                    rule.ascending = true;
                    // Remove ASC if present
                    if (itemUpper.find("ASC") != string::npos) {
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
        // Validate INSERT has proper spacing
        if (!hasProperSpacing(upperQuery, "INSERT", 0)) {
            return nullptr;
        }
        
        InsertQuery* q = new InsertQuery();
        size_t intoPos = upperQuery.find("INTO");
        size_t valuesPos = upperQuery.find("VALUES");
        if (intoPos == string::npos || valuesPos == string::npos) { delete q; return nullptr; }

        // Validate INTO has proper spacing
        if (!hasProperSpacing(upperQuery, "INTO", intoPos)) {
            delete q;
            return nullptr;
        }

        // Extract table name and optionally specified columns
        string tablePart = trim(sqlText.substr(intoPos + 4, valuesPos - intoPos - 4));
        size_t parenPos = tablePart.find('(');
        
        if (parenPos != string::npos) {
            // INSERT INTO table(col1, col2) VALUES(...)
            q->tableName = trim(tablePart.substr(0, parenPos));
            size_t closeParenPos = tablePart.find(')');
            if (closeParenPos == string::npos) { delete q; return nullptr; }
            string colsPart = tablePart.substr(parenPos + 1, closeParenPos - parenPos - 1);
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
        if (openParen == string::npos || closeParen == string::npos) { delete q; return nullptr; }

        string valuesPart = sqlText.substr(openParen + 1, closeParen - openParen - 1);
        auto valueStrs = split(valuesPart, ',');
        for (const auto& vs : valueStrs) {
            string trimmedVal = trim(vs);
            DataType inferredType = inferDataType(trimmedVal);
            q->values.values.emplace_back(inferredType, stripQuotes(trimmedVal));
        }

        return q;
    } else if (upperQuery.find("UPDATE") == 0) {
        // Validate UPDATE has proper spacing
        if (!hasProperSpacing(upperQuery, "UPDATE", 0)) {
            return nullptr;
        }
        
        UpdateQuery* q = new UpdateQuery();
        size_t setPos = upperQuery.find("SET");
        if (setPos == string::npos) { delete q; return nullptr; }

        // Extract table name and handle alias
        string tablePart = trim(sqlText.substr(6, setPos - 6));
        auto tableParts = split(tablePart, ' ');
        if (tableParts.size() >= 2) {
            q->tableName = tableParts[0];
            q->tableAlias = tableParts[1];
        } else {
            q->tableName = tablePart;
            q->tableAlias = tablePart; // No separate alias
        }

        size_t wherePos = upperQuery.find("WHERE");
        string setPart;
        if (wherePos != string::npos) {
            setPart = trim(sqlText.substr(setPos + 3, wherePos - setPos - 3));
            string wherePart = trim(sqlText.substr(wherePos + 5));
            q->where = parseCondition(wherePart);
        } else {
            setPart = trim(sqlText.substr(setPos + 3));
        }

        // Parse SET clause: support multiple column=value pairs separated by commas
        auto assignments = split(setPart, ',');
        for (const auto& assignment : assignments) {
            size_t eqPos = assignment.find('=');
            if (eqPos != string::npos) {
                string col = trim(assignment.substr(0, eqPos));
                string valStr = trim(assignment.substr(eqPos + 1));
                DataType inferredType = inferDataType(valStr);
                q->newValues[col] = Value(inferredType, stripQuotes(valStr));
            }
        }

        return q;
    } else if (upperQuery.find("DELETE") == 0) {
        // Validate DELETE has proper spacing
        if (!hasProperSpacing(upperQuery, "DELETE", 0)) {
            return nullptr;
        }
        
        DeleteQuery* q = new DeleteQuery();
        size_t fromPos = upperQuery.find("FROM");
        if (fromPos == string::npos) { delete q; return nullptr; }

        size_t wherePos = upperQuery.find("WHERE");
        string tablePart;
        if (wherePos != string::npos) {
            tablePart = trim(sqlText.substr(fromPos + 4, wherePos - fromPos - 4));
            string wherePart = trim(sqlText.substr(wherePos + 5));
            q->where = parseCondition(wherePart);
        } else {
            tablePart = trim(sqlText.substr(fromPos + 4));
        }

        // Extract table name and handle alias
        auto tableParts = split(tablePart, ' ');
        if (tableParts.size() >= 2) {
            q->tableName = tableParts[0];
            q->tableAlias = tableParts[1];
        } else {
            q->tableName = tablePart;
            q->tableAlias = tablePart; // No separate alias
        }

        return q;
    } else if (upperQuery.find("CREATE") == 0 && upperQuery.find("TABLE") != string::npos) {
        // Validate CREATE has proper spacing
        if (!hasProperSpacing(upperQuery, "CREATE", 0)) {
            return nullptr;
        }
        
        CreateTableQuery* q = new CreateTableQuery();
        size_t tablePos = upperQuery.find("TABLE");
        
        // Validate TABLE has proper spacing
        if (!hasProperSpacing(upperQuery, "TABLE", tablePos)) {
            delete q;
            return nullptr;
        }
        
        size_t openParen = sqlText.find('(', tablePos);
        size_t closeParen = sqlText.rfind(')');
        
        if (openParen == string::npos || closeParen == string::npos) {
            delete q;
            return nullptr;
        }

        // Extract table name and validate proper spacing
        string tableNamePart = sqlText.substr(tablePos + 5, openParen - tablePos - 5);
        q->tableName = trim(tableNamePart);
        
        // Validate table name is valid identifier
        if (!isValidIdentifier(q->tableName)) {
            delete q;
            return nullptr;
        }
        
        string colsDef = sqlText.substr(openParen + 1, closeParen - openParen - 1);

        // Parse columns: column_name TYPE [PRIMARY KEY] [REFERENCES table(column)], ...
        auto defs = split(colsDef, ',');
        for (auto& def : defs) {
            string defUpper = toUpper(def);
            auto parts = split(trim(def), ' ');
            if (parts.size() >= 2) {
                Column col;
                col.name = parts[0];
                
                // Validate column name is valid identifier
                if (!isValidIdentifier(col.name)) {
                    delete q;
                    return nullptr;
                }
                
                // Parse data type and validate it
                string typeStr = toUpper(parts[1]);
                if (!isValidDataType(typeStr)) {
                    delete q; // Invalid data type
                    return nullptr;
                }
                
                DataType type = DataType::STRING;
                if (typeStr.find("INT") != string::npos) type = DataType::INTEGER;
                else if (typeStr.find("VARCHAR") != string::npos) type = DataType::VARCHAR;
                else if (typeStr.find("FLOAT") != string::npos || typeStr.find("DOUBLE") != string::npos) type = DataType::FLOAT;
                else if (typeStr.find("BOOL") != string::npos) type = DataType::BOOLEAN;
                col.type = type;
                
                // Check for PRIMARY KEY
                if (defUpper.find("PRIMARY") != string::npos && defUpper.find("KEY") != string::npos) {
                    col.isPrimaryKey = true;
                }
                
                // Check for UNIQUE constraint
                if (defUpper.find("UNIQUE") != string::npos) {
                    col.isUnique = true;
                }
                
                // Check for FOREIGN KEY (REFERENCES table(column))
                size_t refPos = defUpper.find("REFERENCES");
                if (refPos != string::npos) {
                    col.isForeignKey = true;
                    string refPart = trim(def.substr(refPos + 10));
                    size_t parenPos = refPart.find('(');
                    if (parenPos != string::npos) {
                        col.foreignTable = trim(refPart.substr(0, parenPos));
                        size_t closeParenPos = refPart.find(')');
                        if (closeParenPos != string::npos) {
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
    } else if (upperQuery.find("DROP") == 0 && upperQuery.find("TABLE") != string::npos) {
        // Validate DROP has proper spacing
        if (!hasProperSpacing(upperQuery, "DROP", 0)) {
            return nullptr;
        }
        
        DropTableQuery* q = new DropTableQuery();
        size_t tablePos = upperQuery.find("TABLE");
        if (tablePos == string::npos) {
            delete q;
            return nullptr;
        }

        // Validate TABLE has proper spacing
        if (!hasProperSpacing(upperQuery, "TABLE", tablePos)) {
            delete q;
            return nullptr;
        }

        // Check for IF EXISTS
        size_t ifExistsPos = upperQuery.find("IF EXISTS", tablePos);
        string tablesPart;
        
        if (ifExistsPos != string::npos && ifExistsPos > tablePos) {
            q->ifExists = true;
            tablesPart = trim(sqlText.substr(ifExistsPos + 9)); // After "IF EXISTS"
        } else {
            tablesPart = trim(sqlText.substr(tablePos + 5)); // After "TABLE"
        }
        
        if (tablesPart.empty()) {
            delete q;
            return nullptr;
        }

        // Parse multiple table names (comma-separated)
        auto tableNames = split(tablesPart, ',');
        for (const auto& tableName : tableNames) {
            string trimmedName = trim(tableName);
            if (!trimmedName.empty()) {
                q->tableNames.push_back(trimmedName);
            }
        }
        
        if (q->tableNames.empty()) {
            delete q;
            return nullptr;
        }

        return q;
    }

    return nullptr;
}
