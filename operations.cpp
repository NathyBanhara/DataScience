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

void Operations::inicializeParser()
{
    std::ifstream text("database.json");
    json::sax_parse(text, &this->sec);
}

void Operations::inicializeGraph()
{
    this->graph = Graph();

	while (this->sec.queue.front()->name != "end_object")
		this->graph.getAtt(this->sec.queue.front()->name,
         this->sec.queue.front()->type, " ", this->sec.queue);
}

void Operations::inicializePostgres()
{
    if (this->postgres.connect("127.0.0.1", "5432", "science", "postgres", "postgres"))
    {
        this->postgres.dropTable("data_science_ii");
        insertTable();
    }
    else
    {
        cout << "Não foi possível conectar com o banco" << "\n";
    }
}

void Operations::insertTable()
{
    string columns = "";
        
    bool isFirstCol = 1;

    for (auto x : this->graph.listNodes)
    {
        if (isFirstCol)
        {
            if (x.second->name == "id") columns += "id SERIAL PRIMARY KEY";
            else columns += x.second->name + " " + x.second->next->type;
            isFirstCol = 0;
        }
        else
        {
            if (x.second->name == "id") columns += ", id SERIAL PRIMARY KEY";
            else columns += ", " + x.second->name + " " + x.second->next->type;
        }
    }

    this->postgres.createTable("data_science_ii", columns);

    columns = "";
    std::string values = "";
    int firstCol = 1;
    int firstVal = 1;

    for (auto x : this->graph.listNodes)
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
        for (auto x : this->graph.listNodes)
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

        this->postgres.insertData("data_science_ii", columns, values);

        firstVal = 1;
        values = "";
    }
}

void Operations::inicializeGraphLog()
{
    this->gLog = GraphLog();

    this->gLog.getAtt();

    printingTransactionType();

    printf("\n");

    printingREDO();

    printf("\n");

    printingUNDO();
}

void Operations::printingTransactionType()
{
    for (auto x : this->gLog.listNodes)
    {
        if (x.second->commited)
            cout << x.first << " realizou REDO" << "\n";
        else cout << x.first << " realizou UNDO" << "\n";
    }
}

void Operations::printingREDO()
{
    for (auto x : this->gLog.operations) //REDO
    {
        if (this->gLog.listNodes[x]->commited)
        {
            string where = "id = '" + to_string(this->gLog.listNodes[x]->first->id) + "'";
            pqxx::result result = this->postgres.selectData("data_science_ii",
            this->gLog.listNodes[x]->first->column, where);

            for (const auto& row : result) {
                for (const auto& field : row) {
                    if (field.c_str() != this->gLog.listNodes[x]->first->newValue)
                    {
                        string set = this->gLog.listNodes[x]->first->column + " = " +
                         this->gLog.listNodes[x]->first->newValue;
                        this->postgres.updateData("data_science_ii", set, where);
                        cout << this->gLog.listNodes[x]->first->column 
                        + " atualizado para " + this->gLog.listNodes[x]->first->newValue + " -> REDO" + "\n";
                    }
                }
            }
            this->gLog.removeREDO(x);
        }
    }
}

void Operations::printingUNDO()
{
    for (int i = this->gLog.operations.size()-1; i >= 0; i--)
    {
        if (!this->gLog.listNodes[this->gLog.operations[i]]->commited)
        {
            string where = "id = " + to_string(this->gLog.listNodes[this->gLog.operations[i]]->last->id);
            pqxx::result result = this->postgres.selectData("data_science_ii",
            this->gLog.listNodes[this->gLog.operations[i]]->last->column, where);

            for (const auto& row : result) {
                for (const auto& field : row) {
                    if (field.c_str() != this->gLog.listNodes[this->gLog.operations[i]]->last->oldValue)
                    {
                        string set = this->gLog.listNodes[this->gLog.operations[i]]->last->column + " = " 
                        + this->gLog.listNodes[this->gLog.operations[i]]->last->oldValue;
                        this->postgres.updateData("data_science_ii", set, where);
                        cout << this->gLog.listNodes[this->gLog.operations[i]]->last->column
                         + " atualizado para " + this->gLog.listNodes[this->gLog.operations[i]]->last->oldValue 
                         + " -> UNDO" + "\n";
                    }
                }
            }
            this->gLog.removeUNDO(this->gLog.operations[i]);
        }
    }
}

void Operations::free()
{
    this->postgres.disconnect();
    this->graph.freeGraph();
    this->gLog.freeGraph();
}