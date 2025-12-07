// include/QueryExecutor.h
#pragma once
#include "Query.h"
#include "Database.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"
#include <functional>

using OutputCallback = std::function<void(const std::string&)>;
using ErrorCallback = std::function<void(const std::string&)>;
using ResultTableCallback = std::function<void(const std::vector<Column>&, const std::vector<Row>&)>;

class QueryExecutor {
public:
    void setOutputCallback(OutputCallback cb) { output = cb; }
    void setErrorCallback(ErrorCallback cb) { error = cb; }
    void setResultTableCallback(ResultTableCallback cb) { resultTable = cb; }

    void execute(Query* q, Database& db);

private:
    void executeSelect(SelectQuery* q, Database& db);
    void executeInsert(InsertQuery* q, Database& db);
    void executeUpdate(UpdateQuery* q, Database& db);
    void executeDelete(DeleteQuery* q, Database& db);

    OutputCallback output = [](const std::string& s) {};
    ErrorCallback error = [](const std::string& s) {};
    ResultTableCallback resultTable = [](const std::vector<Column>&, const std::vector<Row>&) {};
};
