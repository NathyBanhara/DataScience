#include <iostream>
#include <iostream>
#include <pqxx/pqxx>
#include "graph.hpp"

using namespace std;

int connectDatabase()
{
    pqxx::connection conn("dbname=science user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

    // Verifica se a conexão foi estabelecida com sucesso
    if (conn.is_open()) {
        std::cout << "Conexão bem-sucedida!" << std::endl;
        return 1;
    } else {
        std::cout << "Erro ao conectar ao banco de dados." << std::endl;
        return 0;
    }
}

/*// Executa uma consulta no banco de dados
    pqxx::work txn(conn);
    pqxx::result result = txn.exec("SELECT * FROM data_science_II");
    txn.commit();

    // Itera sobre os resultados
    for (const auto& row : result) {
        std::cout << "Valor da coluna 1: " << row[0].as<int>() << std::endl;
        std::cout << "Valor da coluna 2: " << row[1].as<int>() << std::endl;
        std::cout << "Valor da coluna 3: " << row[2].as<int>() << std::endl;
    }
    */
/*
// Cria uma conexão com o banco de dados
    pqxx::connection conn("dbname=science user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

    // Verifica se a conexão foi estabelecida com sucesso
    if (conn.is_open()) {
        std::cout << "Conexão bem-sucedida!" << std::endl;
    } else {
        std::cout << "Erro ao conectar ao banco de dados." << std::endl;
        return 1;
    }

    try {
        // Cria um objeto de comando para executar a query de criação da tabela
        pqxx::work transaction(conn);
        std::string createQuery = "CREATE TABLE data_science_II (id SERIAL PRIMARY KEY, a INT, b INT)";
        transaction.exec(createQuery);
        transaction.commit();

        std::cout << "Tabela criada com sucesso!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao criar tabela: " << e.what() << std::endl;
        return 1;
    }


    // Fecha a conexão com o banco de dados
    conn.disconnect();*/