Este projeto é uma aplicação em C++ que utiliza SQLite3 para armazenar e analisar históricos de preços de criptomoedas. 
Ele permite registrar ativos, listar entradas anteriores e verificar valorização/desvalorização com base nos registros mais recentes.

Funcionalidades
Adicionar Ativos: Registre moeda, preço, quantidade e data.
Listar Carteira: Exibe todos os ativos ordenados pela data mais recente.
Verificar Valorização: Compara o último preço registrado com o penúltimo para indicar valorização ou desvalorização.

Requisitos
C++ (compilador compatível com C++11 ou superior)
SQLite3 (biblioteca de banco de dados)

Instalação e Execução
git clone git@github.com:chmodr644/cpp-crypto-analisys.git
cd cpp-crypto-analisys
Compile o código:
g++ main.cpp -o app -lsqlite3
Execute a aplicação:
./app


Como Usar
Após executar o programa, um menu interativo será exibido:
Adicionar Ativo → Insira moeda, preço, quantidade e data.
Verificar Valorização → Digite a moeda desejada e veja a comparação.
Listar Carteira → Exibe todas as transações ordenadas da mais recente para a mais antiga.
Sair → Finaliza o programa.


Futuramente faeri no qtcreator.
