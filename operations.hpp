#ifndef OPERATIONS_HPP

#define OPERATIONS_HPP

#include <pqxx/pqxx>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "json.hpp"
#include "queue.hpp"
#include "graph.hpp"
#include "graphLog.hpp"
#include "sax_event_consumer.hpp"
#include "postgres.hpp"

using json = nlohmann::json;
using namespace std;

class Operations
{
    private:
        sax_event_consumer sec;
        Graph graph;
        Postgres postgres;
        GraphLog gLog;
    public:
        void inicializeParser();
        void inicializeGraph();
        void inicializePostgres();
        void insertTable();
        void inicializeGraphLog();
        void printingTransactionType();
        void printingREDO();
        void printingUNDO();
        void free();
};

#endif