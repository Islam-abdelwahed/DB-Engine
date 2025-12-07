// src/QueryExecutor.cpp
#include "QueryExecutor.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"


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

    // TODO: Handle groupBy, orderBy, column projection

    // Call the result callback if set
    resultTable(table->getColumns(), selected);

    output("(" + std::to_string(selected.size()) + " row(s) selected)");
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
