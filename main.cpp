#include <pqxx/pqxx>
#include <iostream>

using namespace std;

int main()
{
    // Cria uma conexão com o banco de dados
    pqxx::connection conn("dbname=science user=postgres password=postgres hostaddr=127.0.0.1 port=5432");

    // Verifica se a conexão foi estabelecida com sucesso
    if (conn.is_open()) {
        std::cout << "Conexão bem-sucedida!" << std::endl;
    } else {
        std::cout << "Erro ao conectar ao banco de dados." << std::endl;
        return 1;
    }

    // Executa uma consulta no banco de dados
    pqxx::work txn(conn);
    pqxx::result result = txn.exec("SELECT * FROM data_science_II");
    txn.commit();

    // Itera sobre os resultados
    for (const auto& row : result) {
        std::cout << "Valor da coluna 1: " << row[0].as<int>() << std::endl;
        std::cout << "Valor da coluna 2: " << row[1].as<int>() << std::endl;
        std::cout << "Valor da coluna 3: " << row[2].as<int>() << std::endl;
    }

    // Fecha a conexão com o banco de dados
    conn.disconnect();

    return 0;
}