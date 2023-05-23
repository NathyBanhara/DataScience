#ifndef DATABASE_HPP

#define DATABASE_HPP

#include <iostream>
#include <pqxx/pqxx>
#include "graph.hpp"

using namespace std;

class Database
{
    public:

        pqxx::connection conn;
        Graph graph;
    
     //   int connectDatabase();
        void dropTable(pqxx::connection conn, string name);
        void createTable(pqxx::connection conn, string name, Graph graph);
        void selectTable(pqxx::connection conn, string name);
        void insertTable(pqxx::connection conn, string name);
        void updateTable(pqxx::connection conn, string name);
        void disconnectDatabase();
};

#endif