#include <iostream>
#include <sqlite3.h>
#include <string>
#include <iomanip>

using namespace std;

// Função para abrir o banco de dados
sqlite3* abrirBanco() {
    sqlite3 *db;
    int rc = sqlite3_open("cripto.db", &db);
    if (rc) {
        cerr << "Erro ao abrir o banco de dados: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

// Função para criar a tabela (caso não exista)
void criarTabela(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS ativos ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "moeda TEXT NOT NULL,"
                      "preco REAL NOT NULL,"
                      "quantidade REAL NOT NULL,"
                      "valor_calculado REAL NOT NULL,"
                      "data TEXT NOT NULL);";
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Erro ao criar tabela: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Função para inserir um ativo
void inserirAtivo(sqlite3 *db) {
    string moeda, data;
    double preco, quantidade, valor;

    cout << "Digite a moeda: ";
    getline(cin, moeda);
    cout << "Digite o preço: ";
    cin >> preco;
    cout << "Digite a quantidade: ";
    cin >> quantidade;
    cin.ignore();
    cout << "Digite a data (YYYY-MM-DD): ";
    getline(cin, data);

    valor = preco * quantidade;

    string sql = "INSERT INTO ativos (moeda, preco, quantidade, valor_calculado, data) VALUES ('" + 
                 moeda + "', " + to_string(preco) + ", " + to_string(quantidade) + ", " + 
                 to_string(valor) + ", '" + data + "');";
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Erro ao inserir ativo: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Ativo adicionado com sucesso!" << endl;
    }
}

// Função para listar carteira ordenando por data mais recente
void listarCarteira(sqlite3 *db) {
    cout << "\nCarteira de Ativos:\n";
    const char *sql = "SELECT moeda, preco, quantidade, valor_calculado, data FROM ativos ORDER BY data DESC;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            cout << "Moeda: " << sqlite3_column_text(stmt, 0)
                 << " | Preço: " << sqlite3_column_double(stmt, 1)
                 << " | Quantidade: " << sqlite3_column_double(stmt, 2)
                 << " | Valor Total: " << sqlite3_column_double(stmt, 3)
                 << " | Data: " << sqlite3_column_text(stmt, 4) << endl;
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Erro ao listar carteira." << endl;
    }
}

// Função para verificar valorização ou desvalorização
void verificarValorizacao(sqlite3 *db) {
    string moeda;
    cout << "Digite a moeda para verificar a valorização/desvalorização: ";
    getline(cin, moeda);

    string sql = "SELECT preco FROM ativos WHERE moeda = '" + moeda + "' ORDER BY data DESC LIMIT 2;";
    sqlite3_stmt *stmt;
    double precos[2] = {0, 0};
    int index = 0;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            precos[index++] = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
        
        if (index < 2) {
            cout << "Não há dados suficientes para comparação." << endl;
        } else {
            double diferenca = precos[0] - precos[1];
            cout << "Último preço: " << fixed << setprecision(6) << precos[0] << endl;
            cout << "Preço anterior: " << precos[1] << endl;
            cout << "Variação: " << ((diferenca / precos[1]) * 100) << "%" << (diferenca >= 0 ? " (Valorizou)" : " (Desvalorizou)") << endl;
        }
    } else {
        cerr << "Erro ao consultar banco de dados." << endl;
    }
}

int main() {
    sqlite3 *db = abrirBanco();
    if (!db) return 1;

    criarTabela(db);

    int opcao;
    do {
        cout << "\nMenu:\n1. Adicionar Ativo\n2. Verificar Valorização\n3. Listar Carteira\n0. Sair\nEscolha: ";
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
            case 1:
                inserirAtivo(db);
                break;
            case 2:
                verificarValorizacao(db);
                break;
            case 3:
                listarCarteira(db);
                break;
            case 0:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opção inválida!\n";
        }
    } while (opcao != 0);

    sqlite3_close(db);
    return 0;
}
