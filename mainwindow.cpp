#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include <QStandardItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load database on startup
    database = Database("data");
    database.loadAllTables();

    // updateExplorerTree();

    // printOutput("Welcome to SQL Studio!\n");
    // printOutput("Database loaded: " + QString::number(database.getTableNames().size()) + " tables.\n\n");

    // Set callbacks for executor
    executor.setOutputCallback([this](const std::string& s) {
        printOutput(QString::fromStdString(s));
    });
    executor.setErrorCallback([this](const std::string& s) {
        printError(QString::fromStdString(s));
    });
    executor.setResultTableCallback([this](const std::vector<Column>& cols, const std::vector<Row>& rows) {
        populateResultsTable(cols, rows);
    });

    // // Connect actions
    connect(ui->actionExecute, &QAction::triggered, this, &MainWindow::on_actionExecute_triggered);
    // Assuming there's an actionSave for saving queries or something, connect if needed
}

MainWindow::~MainWindow() {
    database.saveAllTables();
    delete ui;
}

void MainWindow::on_actionExecute_triggered() {
    QString sql = ui->queryText->toPlainText();
    executeSQL(sql);
    // ui->queryEditor->clear();
}

void MainWindow::executeSQL(const QString& sql) {
    std::string query = sql.trimmed().toStdString();
    if (query.empty()) return;

    printOutput("<span style='color: #4A90E2;'><b>SQL&gt;</b> " + sql.toHtmlEscaped() + "</span>");

    QString upperQuery = QString::fromStdString(query).toUpper();

    try {
        if (upperQuery.startsWith("CREATE TABLE") == 0) {
            // handleCreateTable(query);
        } else if (upperQuery.startsWith("DROP TABLE") == 0) {
            // handleDropTable(query);
        } else {
            Query* q = parser.parse(query);
            if (q) {
                executor.execute(q, database);
                delete q;
            } else {
                printError("Syntax error or unsupported query.");
                printOutput("<span style='color: #4A90E2;'><b>SQL&gt;</b> error </span>");
            }
        }
    } catch (const std::exception& e) {
        printError("Exception: " + QString(e.what()));
    }

    printOutput(""); // newline
    // updateExplorerTree(); // Refresh explorer after changes
}

void MainWindow::updateExplorerTree() {
    auto tableNames = database.getTableNames();

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"Tables"}); // optional header

    for (const auto& name : tableNames) {
        QStandardItem *item = new QStandardItem(QString::fromStdString(name));
        model->appendRow(item);
    }

    ui->tables_tree->setModel(model);
}


// void MainWindow::handleCreateTable(const std::string& query) {
//     size_t tablePos = toUpper(query).find("TABLE");
//     if (tablePos == std::string::npos) return;

//     size_t openParen = query.find('(', tablePos);
//     size_t closeParen = query.rfind(')');
//     if (openParen == std::string::npos || closeParen == std::string::npos) {
//         printError("Invalid CREATE TABLE syntax.");
//         return;
//     }

//     std::string tableName = trim(query.substr(tablePos + 5, openParen - tablePos - 5));
//     std::string colsDef = query.substr(openParen + 1, closeParen - openParen - 1);

//     std::vector<Column> columns;
//     auto defs = split(colsDef, ',');
//     for (auto& def : defs) {
//         auto parts = split(trim(def), ' ');
//         if (parts.size() >= 2) {
//             DataType type = DataType::STRING;
//             std::string typeStr = toUpper(parts[1]);
//             if (typeStr.find("INT") != std::string::npos) type = DataType::INTEGER;
//             else if (typeStr.find("FLOAT") != std::string::npos) type = DataType::FLOAT;
//             else if (typeStr.find("BOOL") != std::string::npos) type = DataType::BOOLEAN;
//             columns.emplace_back(parts[0], type);
//         }
//     }

//     if (columns.empty()) {
//         printError("No columns defined.");
//         return;
//     }

//     database.createTable(tableName, columns);
//     printOutput("<span style='color: green;'>Table '" + QString::fromStdString(tableName) + "' created successfully.</span>");
// }

// void MainWindow::handleDropTable(const std::string& query) {
//     size_t tablePos = toupper(query).find("TABLE");
//     if (tablePos == std::string::npos) return;

//     std::string tableName = trim(query.substr(tablePos + 5));
//     if (database.getTable(tableName)) {
//         database.dropTable(tableName);
//         printOutput("<span style='color: orange;'>Table '" + QString::fromStdString(tableName) + "' dropped.</span>");
//     } else {
//         printError("Table not found: " + QString::fromStdString(tableName));
//     }
// }

void MainWindow::printOutput(const QString& text) {
    ui->outputText->append(text);
    QScrollBar *bar = ui->outputText->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void MainWindow::printError(const QString& error) {
    ui->errorText->append("<span style='color: red;'><b>Error:</b> " + error.toHtmlEscaped() + "</span>");
    QScrollBar *bar = ui->errorText->verticalScrollBar();
    ui->tabWidget->setCurrentWidget(ui->error_tab);
    ui->errorText->setFocus();
    bar->setValue(bar->maximum());
}

void MainWindow::populateResultsTable(const std::vector<Column>& cols, const std::vector<Row>& rows) {
    // Clear previous content
    ui->resultText->clear();
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
    ui->resultText->setHtml(html);
    // Scroll to top
    ui->tabWidget->setCurrentWidget(ui->result_tab);
    ui->resultText->setFocus();
    QScrollBar *bar = ui->resultText->verticalScrollBar();
    bar->setValue(0);

    printOutput("(" + QString::number(rows.size()) + " row(s) selected)");
}


