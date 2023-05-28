#include <iostream>
#include "postgres.hpp"

Postgres::Postgres() : connection(nullptr) {}

Postgres::~Postgres() {
    disconnect();
}

bool Postgres::connect(const std::string& host, const std::string& port,
                          const std::string& dbname, const std::string& user,
                          const std::string& password) {
    std::string connectionString =
        "host=" + host + " port=" + port + " dbname=" + dbname + " user=" +
        user + " password=" + password;

    try {
        connection = new pqxx::connection(connectionString);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return false;
    }
}

void Postgres::disconnect() {
    if (connection != nullptr) {
        connection->disconnect();
        delete connection;
        connection = nullptr;
    }
}

bool Postgres::createTable(const std::string& tableName, const std::string& columns) {
    try {
        pqxx::work txn(*connection);
        std::string query = "CREATE TABLE " + tableName + " (" + columns + ")";
        txn.exec(query);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Table creation failed: " << e.what() << std::endl;
        return false;
    }
}

bool Postgres::insertData(const std::string& tableName, const std::string& columns,
                            const std::string& values) {
    try {
        pqxx::work txn(*connection);
        std::string query = "INSERT INTO " + tableName + " (" + columns + ") VALUES (" + values + ")";
        txn.exec(query);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Insertion failed: " << e.what() << std::endl;
        return false;
    }
}

bool Postgres::dropTable(const std::string& tableName) {
    try {
        pqxx::work txn(*connection);
        std::string query = "DROP TABLE IF EXISTS " + tableName;
        txn.exec(query);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Table drop failed: " << e.what() << std::endl;
        return false;
    }
}

bool Postgres::updateData(const std::string& tableName, const std::string& setClause,
                            const std::string& whereClause) {
    try {
        pqxx::work txn(*connection);
        std::string query = "UPDATE " + tableName + " SET " + setClause + " WHERE " + whereClause;
        txn.exec(query);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Update failed: " << e.what() << std::endl;
        return false;
    }
}

pqxx::result Postgres::selectData(const std::string& tableName, const std::string& columns,
                                    const std::string& whereClause) {
    try {
        pqxx::work txn(*connection);
        std::string query = "SELECT " + columns + " FROM " + tableName + " WHERE " + whereClause;
        pqxx::result result = txn.exec(query);
        txn.commit();
        return result;
    } catch (const std::exception& e) {
        std::cerr << "Select failed: " << e.what() << std::endl;
        return pqxx::result();
    }
}