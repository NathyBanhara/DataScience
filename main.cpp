#include <pqxx/pqxx>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "json.hpp"
#include "queue.hpp"
#include "graph.hpp"
#include "sax_event_consumer.hpp"
#include "postgres.hpp"

using json = nlohmann::json;
using namespace std;

//pqxx::connection conn("dbname=science user=postgres password=postgres hostaddr=127.0.0.1 port=5432");


int main()
{
    std::ifstream text("database.json");

    // create a SAX event consumer object
    sax_event_consumer sec;

    // parse JSON
    bool resultJson = json::sax_parse(text, &sec);
    
    // output the result of sax_parse
    std::cout << "\nresult: " << std::boolalpha << resultJson << "\n";

    printf("\n");

    Queue q = sec.queue;

    q.printQueue();

    printf("\n");

    Graph g = Graph();

	while (q.front()->name != "end_object")
		g.getAtt(q.front()->name, q.front()->type, " ", q);

    g.printGraph();

    Postgres postgres;

    if (postgres.connect("127.0.0.1", "5432", "science", "postgres", "postgres"))
    {
        postgres.dropTable("data_science_ii");
    
        string columns = "id SERIAL PRIMARY KEY";

        for (auto x : g.listNodes)
        {
            if (x.second->name != "id")
            {
                columns += ", " + x.second->name + " ";
                if (x.second->next->type == "string") columns += "varchar(50)";
                else columns += x.second->next->type;
            }
        }

        postgres.createTable("data_science_ii", columns);
    }

    std::string columns = "";
    std::string values = "";
    int firstCol = 1;
    int firstVal = 1;

    for (auto x : g.listNodes)
    {
        if (firstCol)
        {
            columns += x.second->name;
            firstCol = 0; 
        }
        else
        {
            columns += ", " + x.second->name;
        }
    }

    for (auto x : g.listNodes)
    {
        Edge *aux = x.second->next;
        while (aux != NULL)
        {
            if (x.second->next->read == 0)
            {
                if (firstVal)
                {
                    values += "'" + x.second->next->value + "'";
                    firstVal = 0;
                }
                else values += ", '" + x.second->next->value + "'";
                break;
            }
        }

        firstVal = 1;

        std::string tableName = "data_science_ii";
        //std::string columns = "column1, column2";
        std::string values = "'value1', 'value2'";
        if (postgresql.insertData(tableName, columns, values)) {
            std::cout << "Dados inseridos com sucesso!" << std::endl;
        } else {
            std::cout << "Falha ao inserir dados." << std::endl;
        }
    }
    postgres.disconnect();

    return 0;
}