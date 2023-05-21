#include <pqxx/pqxx>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "json.hpp"
#include "queue.hpp"
#include "graph.hpp"
#include "sax_event_consumer.hpp"

using json = nlohmann::json;

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

    // Cria uma conexão com o banco de dados
    pqxx::connection conn("dbname=science user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

    // Verifica se a conexão foi estabelecida com sucesso
    if (conn.is_open()) {
        std::cout << "Conexão bem-sucedida!" << std::endl;
    } else {
        std::cout << "Erro ao conectar ao banco de dados." << std::endl;
        return 1;
    }

    // Fecha a conexão com o banco de dados
    conn.disconnect();

    return 0;
}