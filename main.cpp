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
#include "operations.hpp"

using json = nlohmann::json;
using namespace std;

//pqxx::connection conn("dbname=science user=postgres password=postgres hostaddr=127.0.0.1 port=5432");


int main()
{
    Operations operations = Operations();

    operations.inicializeParser();
    operations.inicializeGraph();
    operations.inicializePostgres();
    operations.inicializeGraphLog();

    operations.free();

    return 0;
}