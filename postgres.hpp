#ifndef POSTGRES_HPP
#define POSTGRES_HPP

#include <pqxx/pqxx>

class Postgres {
public:
    Postgres();
    ~Postgres();

    bool connect(const std::string& host, const std::string& port,
                 const std::string& dbname, const std::string& user,
                 const std::string& password);

    void disconnect();

    bool createTable(const std::string& tableName, const std::string& columns);

    bool insertData(const std::string& tableName, const std::string& columns,
                    const std::string& values);

    bool dropTable(const std::string& tableName);

    bool updateData(const std::string& tableName, const std::string& setClause,
                    const std::string& whereClause);

    pqxx::result selectData(const std::string& tableName, const std::string& columns,
                                    const std::string& whereClause);

private:
    pqxx::connection* connection;
};

#endif
