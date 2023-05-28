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

    Graph g = Graph();

	while (q.front()->name != "end_object")
		g.getAtt(q.front()->name, q.front()->type, " ", q);

    Postgres postgres;

    if (postgres.connect("127.0.0.1", "5432", "science", "postgres", "postgres"))
    {
        postgres.dropTable("data_science_ii");
    
        string columns = "";
        
        bool firstCol = 1;

        for (auto x : g.listNodes)
        {
            if (firstCol)
            {
                if (x.second->name == "id") columns += "id SERIAL PRIMARY KEY";
                else if (x.second->next->type == "string") columns += x.second->name + " " + "varchar(50)";
                else columns += x.second->name + " " + x.second->next->type;
                firstCol = 0;
            }
            else
            {
                if (x.second->name == "id") columns += ", id SERIAL PRIMARY KEY";
                else if (x.second->next->type == "string") columns += ", " + x.second->name + " " + "varchar(50)";
                else columns += ", " + x.second->name + " " + x.second->next->type;
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

    bool valuesFound = false;

    while (true)
    {
        for (auto x : g.listNodes)
        {
            Edge *aux = x.second->next;
            while (aux != NULL)
            {
                if (!aux->read)
                {
                    if (firstVal)
                    {
                        values += "'" + aux->value + "'";
                        firstVal = 0;
                    }
                    else values += ", '" + aux->value + "'";
                    aux->read = true;
                    valuesFound = true;
                    break;
                }
                aux = aux->next;
            }
        }
        if (!valuesFound) break;
        valuesFound = false;

        postgres.insertData("data_science_ii", columns, values);

        firstVal = 1;
        values = "";
    }

    GraphLog gLog = GraphLog();

    gLog.getAtt();

    for (auto x : gLog.listNodes)
    {
        if (x.second->commited)
            cout << x.first << " realizou REDO" << "\n";
        else cout << x.first << " realizou UNDO" << "\n";
    }

    printf("\n");

    for (auto x : gLog.operations) //REDO
    {
        if (gLog.listNodes[x]->commited)
        {
            string where = "id = '" + to_string(gLog.listNodes[x]->first->id) + "'";
            pqxx::result result = postgres.selectData("data_science_ii",
            gLog.listNodes[x]->first->column, where);

            for (const auto& row : result) {
                for (const auto& field : row) {
                    if (field.c_str() != gLog.listNodes[x]->first->newValue)
                    {
                        string set = gLog.listNodes[x]->first->column + " = " + gLog.listNodes[x]->first->newValue;
                            postgres.updateData("data_science_ii", set, where);
                        cout << gLog.listNodes[x]->first->column 
                        + " updated to " + gLog.listNodes[x]->first->newValue + " -> REDO" + "\n";
                    }
                }
            }
            gLog.removeREDO(x);
        }
    }

    for (int i = gLog.operations.size()-1; i >= 0; i--)
    {
        if (!gLog.listNodes[gLog.operations[i]]->commited)
        {
            string where = "id = " + to_string(gLog.listNodes[gLog.operations[i]]->last->id);
            pqxx::result result = postgres.selectData("data_science_ii",
            gLog.listNodes[gLog.operations[i]]->last->column, where);

            for (const auto& row : result) {
                for (const auto& field : row) {
                    if (field.c_str() != gLog.listNodes[gLog.operations[i]]->last->oldValue)
                    {
                        string set = gLog.listNodes[gLog.operations[i]]->last->column + " = " 
                        + gLog.listNodes[gLog.operations[i]]->last->oldValue;
                        postgres.updateData("data_science_ii", set, where);
                        cout << gLog.listNodes[gLog.operations[i]]->last->column + " updated to "
                         + gLog.listNodes[gLog.operations[i]]->last->oldValue + " -> UNDO" + "\n";
                    }
                }
            }
            gLog.removeUNDO(gLog.operations[i]);
        }
    }

    postgres.disconnect();

    return 0;
}