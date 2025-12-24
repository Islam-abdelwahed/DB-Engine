// include/QueryExecutor.h
#pragma once
#include "Query.h"
#include "Database.h"
#include "SelectQuery.h"
#include "InsertQuery.h"
#include "UpdateQuery.h"
#include "DeleteQuery.h"
#include "CreateTableQuery.h"
#include "DropTableQuery.h"
#include <functional>
using namespace std;

using namespace std;

using OutputCallback = function<void(const string&)>;
using ErrorCallback = function<void(const string&)>;
using ResultTableCallback = function<void(const vector<Column>&, const vector<Row>&)>;
using TreeRefreshCallback = function<void()>;

class QueryExecutor {
public:
    void setOutputCallback(OutputCallback cb) { output = cb; }
    void setErrorCallback(ErrorCallback cb) { error = cb; }
    void setResultTableCallback(ResultTableCallback cb) { resultTable = cb; }
    void setTreeRefreshCallback(TreeRefreshCallback cb){tree =cb;};
    void execute(Query* q, Database& db);

private:
    void executeSelect(SelectQuery* q, Database& db);
    void executeInsert(InsertQuery* q, Database& db);
    void executeUpdate(UpdateQuery* q, Database& db);
    void executeDelete(DeleteQuery* q, Database& db);
    void executeCreateTable(CreateTableQuery* q, Database& db);
    void executeDropTable(DropTableQuery* q, Database& db);

    OutputCallback output = [](const string& s) {};
    ErrorCallback error = [](const string& s) {};
    TreeRefreshCallback tree = []() {};
    ResultTableCallback resultTable = [](const vector<Column>&, const vector<Row>&) {};
};
