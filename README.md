# DB-Engine

A lightweight, SQL-based database management system built with C++ and Qt, featuring a graphical user interface for executing SQL queries and managing relational databases.

![Version](https://img.shields.io/badge/version-0.1-blue)
![C++](https://img.shields.io/badge/C++-17-blue)
![Qt](https://img.shields.io/badge/Qt-5%2F6-green)
![License](https://img.shields.io/badge/license-MIT-green)

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [SQL Syntax](#sql-syntax)
- [Project Structure](#project-structure)
- [Examples](#examples)
- [Contributing](#contributing)

## Features

### Core SQL Operations
- **DDL (Data Definition Language)**
  - `CREATE TABLE` - Create tables with column definitions
  - `DROP TABLE` - Remove tables from the database
  
- **DML (Data Manipulation Language)**
  - `INSERT` - Add rows to tables (full or partial row insertion)
  - `SELECT` - Query data with filtering, sorting, and aggregation
  - `UPDATE` - Modify existing records
  - `DELETE` - Remove records from tables

### Advanced Query Features
- **JOIN Operations**
  - `INNER JOIN` - Returns matching rows from both tables
  - `LEFT JOIN` - Returns all rows from left table with matching right table rows
  - `RIGHT JOIN` - Returns all rows from right table with matching left table rows
  - Support for multiple joins in a single query
  
- **Table Aliases**
  - Use aliases in SELECT, UPDATE, DELETE queries
  - Example: `SELECT * FROM users u WHERE u.age > 18`
  
- **Aggregate Functions**
  - `COUNT()` - Count rows
  - `SUM()` - Calculate sum
  - `AVG()` - Calculate average
  - `MIN()` - Find minimum value
  - `MAX()` - Find maximum value
  
- **Advanced Clauses**
  - `WHERE` - Filter rows with conditions (supports AND/OR operators)
  - `GROUP BY` - Group results by columns
  - `ORDER BY` - Sort results (ASC/DESC)
  - Compound conditions with logical operators

### Data Features
- **Data Types**
  - `INT` / `INTEGER` - Whole numbers
  - `FLOAT` / `DOUBLE` - Floating-point numbers
  - `VARCHAR` / `STRING` / `TEXT` - Text data
  - `BOOLEAN` / `BOOL` - Boolean values
  - `DATE` - Date values
  
- **Constraints**
  - Primary Key
  - Unique constraints
  - Foreign Key relationships
  - NULL value handling

- **Storage**
  - CSV-based persistent storage
  - Automatic data loading on startup
  - Manual and automatic save operations

### GUI Features
- **SQL Editor**
  - Multi-query execution support
  - Syntax-aware query parsing
  - Real-time query feedback
  
- **Results Display**
  - Table view for query results
  - Scrollable output for large datasets
  - Color-coded output messages
  
- **Database Explorer**
  - Tree view of all tables
  - Quick table inspection
  - Visual database structure

## Architecture

The DB-Engine follows a modular architecture with clear separation of concerns:

```
┌─────────────────┐
│   GUI Layer     │  (MainWindow, Qt Widgets)
│  (mainwindow)   │
└────────┬────────┘
         │
┌────────▼────────┐
│  Query Layer    │  (Parser, QueryExecutor)
│  - Parser       │  - Parses SQL text into Query objects
│  - Executor     │  - Executes Query objects on Database
└────────┬────────┘
         │
┌────────▼────────┐
│  Data Layer     │  (Database, Table)
│  - Database     │  - Manages collection of tables
│  - Table        │  - Manages rows and columns
│  - Row/Column   │  - Data structures
│  - Value        │  - Type-safe value container
└────────┬────────┘
         │
┌────────▼────────┐
│ Storage Layer   │  (CSV Files)
│   data/*.csv    │
└─────────────────┘
```

### Key Components

- **Parser**: Converts SQL text into structured Query objects
- **QueryExecutor**: Executes Query objects and manages database operations
- **Database**: Container for all tables with load/save functionality
- **Table**: Manages rows, columns, and constraints for a single table
- **Condition**: Evaluates WHERE clause conditions (supports nested conditions)
- **Value**: Type-safe container for database values with NULL support

## Requirements

- **C++ Compiler**: Supporting C++17 or later
- **CMake**: Version 3.16 or higher
- **Qt Framework**: Qt 5 or Qt 6
  - Qt Widgets module
- **Operating System**: Windows, Linux, or macOS

## Installation

### Building from Source

1. **Clone the repository**
   ```bash
   git clone https://github.com/Islam-abdelwahed/DB-Engine.git
   cd DB-Engine
   ```

2. **Install Qt**
   - Download and install Qt from [qt.io](https://www.qt.io/download)
   - Ensure Qt is in your PATH

3. **Configure with CMake**
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

4. **Build the project**
   ```bash
   cmake --build .
   ```

5. **Run the application**
   ```bash
   ./DB-engine
   ```

### Using Qt Creator (Recommended)

1. Open Qt Creator
2. Select "Open Project"
3. Navigate to and open `CMakeLists.txt`
4. Configure the project with your Qt kit
5. Build and run

## Usage

### Starting the Application

1. Launch DB-Engine
2. The application automatically loads existing tables from the `data/` directory
3. Use the SQL editor to write and execute queries
4. View results in the table view or output panel

### Basic Workflow

1. **Create a Table**
   ```sql
   CREATE TABLE employees (
       id INT PRIMARY KEY,
       name VARCHAR,
       age INT,
       department VARCHAR
   );
   ```

2. **Insert Data**
   ```sql
   INSERT INTO employees VALUES (1, 'John Doe', 30, 'IT');
   INSERT INTO employees (id, name) VALUES (2, 'Jane Smith');
   ```

3. **Query Data**
   ```sql
   SELECT * FROM employees WHERE age > 25;
   SELECT name, department FROM employees ORDER BY name ASC;
   ```

4. **Update Records**
   ```sql
   UPDATE employees SET age = 31 WHERE name = 'John Doe';
   UPDATE employees e SET e.department = 'HR' WHERE e.id = 2;
   ```

5. **Delete Records**
   ```sql
   DELETE FROM employees WHERE age < 25;
   DELETE FROM employees e WHERE e.department = 'IT';
   ```

## SQL Syntax

### CREATE TABLE
```sql
CREATE TABLE table_name (
    column1 datatype [PRIMARY KEY] [UNIQUE],
    column2 datatype [FOREIGN KEY REFERENCES other_table(column)],
    ...
);
```

### INSERT
```sql
-- Full row insertion
INSERT INTO table_name VALUES (val1, val2, val3, ...);

-- Partial row insertion
INSERT INTO table_name (col1, col2) VALUES (val1, val2);
```

### SELECT
```sql
SELECT column1, column2, ...
FROM table_name [alias]
[INNER|LEFT|RIGHT] JOIN other_table [alias] ON condition
WHERE condition [AND|OR condition]
GROUP BY column1, column2, ...
ORDER BY column1 [ASC|DESC], column2 [ASC|DESC];
```

### Aggregate Functions
```sql
SELECT COUNT(*), AVG(age), SUM(salary), MIN(age), MAX(salary)
FROM employees
GROUP BY department;
```

### UPDATE
```sql
UPDATE table_name [alias]
SET column1 = value1, column2 = value2, ...
WHERE condition;
```

### DELETE
```sql
DELETE FROM table_name [alias]
WHERE condition;
```

### DROP TABLE
```sql
DROP TABLE table_name;
```

### JOIN Examples
```sql
-- INNER JOIN
SELECT * FROM users u 
INNER JOIN orders o ON u.id = o.user_id;

-- LEFT JOIN
SELECT * FROM users u 
LEFT JOIN orders o ON u.id = o.user_id;

-- RIGHT JOIN
SELECT * FROM users u 
RIGHT JOIN orders o ON u.id = o.user_id;

-- Multiple JOINs
SELECT * FROM users u
INNER JOIN orders o ON u.id = o.user_id
INNER JOIN products p ON o.product_id = p.id;
```

## Project Structure

```
DB-Engine/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── main.cpp                    # Application entry point
├── mainwindow.cpp/h/ui         # GUI implementation
├── resources.qrc               # Qt resources (images, icons)
│
├── Core Components:
│   ├── Database.cpp/h          # Database container and management
│   ├── Table.cpp/h             # Table operations and storage
│   ├── Parser.cpp/h            # SQL parser
│   ├── QueryExecutor.cpp/h    # Query execution engine
│   └── Condition.cpp/h         # WHERE clause evaluation
│
├── Data Structures:
│   ├── Query.h                 # Base query class
│   ├── SelectQuery.h           # SELECT query structure
│   ├── InsertQuery.h           # INSERT query structure
│   ├── UpdateQuery.h           # UPDATE query structure
│   ├── DeleteQuery.h           # DELETE query structure
│   ├── CreateTableQuery.h      # CREATE TABLE structure
│   ├── DropTableQuery.h        # DROP TABLE structure
│   ├── Column.h                # Column definition
│   ├── Row.h                   # Row data structure
│   ├── Value.h                 # Type-safe value container
│   └── SortRule.h              # ORDER BY rule
│
├── Test Files:
│   ├── test_joins.sql          # JOIN operation tests
│   └── test_aliases.sql        # Table alias tests
│
└── data/                       # CSV storage directory
    ├── *.csv                   # Table data files
    └── ...
```

## Examples

### Example 1: Employee Database

```sql
-- Create employees table
CREATE TABLE employees (
    id INT PRIMARY KEY,
    name VARCHAR,
    salary INT,
    department VARCHAR
);

-- Insert sample data
INSERT INTO employees VALUES (1, 'John Doe', 50000, 'IT');
INSERT INTO employees VALUES (2, 'Jane Smith', 60000, 'HR');
INSERT INTO employees VALUES (3, 'Bob Johnson', 55000, 'IT');

-- Query with aggregation
SELECT department, AVG(salary) AS avg_salary, COUNT(*) AS employee_count
FROM employees
GROUP BY department
ORDER BY avg_salary DESC;

-- Update with alias
UPDATE employees e 
SET e.salary = 65000 
WHERE e.department = 'IT' AND e.salary < 60000;

-- Complex WHERE condition
SELECT * FROM employees e
WHERE e.department = 'IT' OR (e.salary > 55000 AND e.department = 'HR');
```

### Example 2: Multi-table JOIN

```sql
-- Create tables
CREATE TABLE users (id INT, name VARCHAR, age INT);
CREATE TABLE orders (id INT, user_id INT, product VARCHAR, amount INT);

-- Insert data
INSERT INTO users VALUES (1, 'Alice', 25);
INSERT INTO users VALUES (2, 'Bob', 30);
INSERT INTO orders VALUES (1, 1, 'Laptop', 1000);
INSERT INTO orders VALUES (2, 1, 'Mouse', 25);
INSERT INTO orders VALUES (3, 2, 'Keyboard', 75);

-- LEFT JOIN to show all users and their orders
SELECT u.name, o.product, o.amount
FROM users u
LEFT JOIN orders o ON u.id = o.user_id
ORDER BY u.name;

-- Aggregate with JOIN
SELECT u.name, COUNT(*) AS order_count, SUM(o.amount) AS total_spent
FROM users u
INNER JOIN orders o ON u.id = o.user_id
GROUP BY u.name;
```

## Features in Detail

### NULL Value Handling
- NULL values are properly handled in all operations
- NULL comparisons in WHERE clauses follow SQL standards (NULL != NULL)
- Support for NULL in INSERT and UPDATE operations

### Constraint Validation
- Primary key uniqueness enforced
- Foreign key relationships validated
- Unique constraints checked on INSERT and UPDATE

### Error Handling
- Detailed error messages for syntax errors
- Type mismatch detection
- Column and table existence validation
- Foreign key constraint violations

### Performance Considerations
- In-memory operations for fast query execution
- Efficient CSV loading and saving
- Indexed column lookups for better performance

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

### Development Guidelines
1. Follow C++17 standards
2. Maintain consistent code style
3. Add tests for new features
4. Update documentation as needed

## Future Enhancements

Potential features for future versions:
- [ ] Transaction support (BEGIN, COMMIT, ROLLBACK)
- [ ] View creation and management
- [ ] Index creation for performance optimization
- [ ] HAVING clause for filtered aggregations
- [ ] Subquery support
- [ ] DISTINCT keyword implementation
- [ ] More SQL functions (STRING functions, DATE functions)
- [ ] Import/Export to other formats (JSON, XML)
- [ ] Query history and favorites
- [ ] Auto-complete in SQL editor

## License

This project is open source. See the LICENSE file for details.

## Authors

- Islam Abdelwahed - [@Islam-abdelwahed](https://github.com/Islam-abdelwahed)

## Acknowledgments

Built with Qt Framework and inspired by traditional RDBMS systems.

---

For questions, issues, or feature requests, please open an issue on GitHub.
