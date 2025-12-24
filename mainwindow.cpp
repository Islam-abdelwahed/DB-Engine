#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include <QStandardItem>

using namespace std;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->showMaximized();
    QMenu* viewMenu = ui->menuView;
    viewMenu->addAction(ui->dockWidget->toggleViewAction());
    viewMenu->addAction(ui->dockWidget_2->toggleViewAction());

    // Load database on startup
    database = Database("data");
    database.loadAllTables();

    updateExplorerTree();

    printOutput("Welcome to SQL Studio!\n");
    printOutput("Database loaded: " + QString::number(database.getTableNames().size()) + " tables.\n\n");

    // Set callbacks for executor
    executor.setOutputCallback([this](const string& s) {
        printOutput(QString::fromStdString(s));
    });
    executor.setErrorCallback([this](const string& s) {
        printError(QString::fromStdString(s));
    });
    executor.setResultTableCallback([this](const vector<Column>& cols, const vector<Row>& rows) {
        populateResultsTable(cols, rows);
    });
    executor.setTreeRefreshCallback([this](){
        ui->tables_tree->update();
    });
    }

MainWindow::~MainWindow() {
    database.saveAllTables();
    delete ui;
}

void MainWindow::on_actionExecute_triggered() {
    ui->resultText->clear();
    QString sql = ui->queryText->toPlainText();
    executeSQL(sql);
}

void MainWindow::on_actionSave_triggered() {
    database.saveAllTables();

}

void MainWindow::executeSQL(const QString& sql) {
    string input = sql.trimmed().toStdString();
    if (input.empty()) return;
    printOutput("<span style='color: #4A90E2;'><b>SQL&gt;</b> " + sql.toHtmlEscaped() + "</span>");
    // Split multiple queries by semicolon
    vector<string> queries;
    string currentQuery;
    for (char c : input) {
        if (c == ';') {
            string trimmed = currentQuery;
            // Trim whitespace
            size_t start = trimmed.find_first_not_of(" \t\n\r");
            if (start != string::npos) {
                size_t end = trimmed.find_last_not_of(" \t\n\r");
                trimmed = trimmed.substr(start, end - start + 1);
                if (!trimmed.empty()) {
                    queries.push_back(trimmed);
                }
            }
            currentQuery.clear();
        } else {
            currentQuery += c;
        }
    }
    // Add last query if no trailing semicolon
    if (!currentQuery.empty()) {
        string trimmed = currentQuery;
        size_t start = trimmed.find_first_not_of(" \t\n\r");
        if (start != string::npos) {
            size_t end = trimmed.find_last_not_of(" \t\n\r");
            trimmed = trimmed.substr(start, end - start + 1);
            if (!trimmed.empty()) {
                queries.push_back(trimmed);
            }
        }
    }

    // Execute each query
    for (const auto& query : queries) {
        printOutput("<span style='color: #4A90E2;'><b>SQL&gt;</b> " + QString::fromStdString(query).toHtmlEscaped() + "</span>");

        try {
            Query* q = parser.parse(query);
            if (q) {
                executor.execute(q, database);
                delete q;
            } else {
                printError("Syntax error or unsupported query.");
            }
        } catch (const exception& e) {
            printError("Exception: " + QString(e.what()));
        }

        printOutput(""); // newline
    }
    updateExplorerTree(); // Refresh explorer after changes
}

void MainWindow::updateExplorerTree() {
    auto model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"Tables"});
    QStandardItem *rootItem = new QStandardItem("Database");
    rootItem->setEditable(false);
    for (const auto& name : database.getTableNames()) {
        QStandardItem* table = new QStandardItem(QString::fromStdString(name));
        for (const auto& col : (database.getTable(name)->getColumns()))
        {
            QStandardItem* column = new QStandardItem(QString::fromStdString(col.name));
            column->setIcon(QIcon(":icons/column.png"));
            table->appendRow(column);
        }
        table->setIcon(QIcon(":icons/table.png"));
        rootItem->appendRow(table);
    }
    model->appendRow(rootItem);
    ui->tables_tree->setModel(model);
    // Enable tree decorations
    ui->tables_tree->setRootIsDecorated(true);
    ui->tables_tree->setItemsExpandable(true);
    ui->tables_tree->setExpandsOnDoubleClick(true);
    // Expand only the root item
    QModelIndex rootIndex = model->indexFromItem(rootItem);
    ui->tables_tree->expand(rootIndex);
}

void MainWindow::printOutput(const QString& text) {
    ui->outputText->append(text);
    QScrollBar *bar = ui->outputText->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void MainWindow::printError(const QString& error) {
    ui->errorText->append("<span style='color: red;'><b>Error:</b> " + error.toHtmlEscaped() + "</span>");
    QScrollBar *bar = ui->errorText->verticalScrollBar();
    ui->bottomTabs->setCurrentWidget(ui->error_tab);
    // ui->errorText->setFocus();
    bar->setValue(bar->maximum());
}

void MainWindow::populateResultsTable(const vector<Column>& cols, const vector<Row>& rows) {
    // Clear previous content

    ui->bottomTabs->setCurrentWidget(ui->result_tab);
    ui->resultText->setFocus();
    if (cols.empty()) {
        printOutput("(0 column(s) returned)");
        return;
    }

    QString html = R"(
        <style>
            table {
                border-collapse: collapse;
                width: 100%;
                font-family: Arial, sans-serif;
            }
            th, td {
                border: 1px solid #dddddd;
                text-align: left;
                padding: 8px;
            }
            th {
                background-color: #f2f2f2;
                font-weight: bold;
            }
            tr:nth-child(even) {
                background-color: #f9f9f9;
            }
        </style>
        <table>
            <tr>
    )";

    // -------------------------------
    // Add header row
    // -------------------------------
    for (const auto& col : cols) {
        html += "<th>" +
                QString::fromStdString(col.name).toHtmlEscaped() +
                "</th>";
    }

    html += "</tr>";

    // -------------------------------
    // Add data rows
    // -------------------------------
    for (const auto& row : rows) {
        html += "<tr>";

        for (const auto& val : row.values) {
            html += "<td>" +
                    QString::fromStdString(val.data).toHtmlEscaped() +
                    "</td>";
        }

        html += "</tr>";
    }

    html += "</table>";
    // Set the HTML content
    ui->resultText->append(html);
    // Scroll to top
    QScrollBar *bar = ui->resultText->verticalScrollBar();
    bar->setValue(0);

    printOutput("(" + QString::number(rows.size()) + " row(s) selected)");
}


