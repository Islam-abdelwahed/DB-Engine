#pragma once
#include <QMainWindow>
#include "Database.h"
#include "Parser.h"
#include "QueryExecutor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExecute_triggered();
        void on_actionSave_triggered();
    // void on_clearOutputButton_clicked();
    // void on_inputReturnPressed();

private:
    Ui::MainWindow *ui;

    // Your clean engine — no Qt inside!
    Database database;
    Parser parser;
    QueryExecutor executor;

    void executeSQL(const QString& sql);
    void printOutput(const QString& text,const bool focus);
    void populateResultsTable(const std::vector<Column>& cols, const std::vector<Row>& rows);
    void printError(const QString& error);
    // void handleCreateTable(const std::string& query);
    // void handleDropTable(const std::string& query);
    void updateExplorerTree() ;
};
