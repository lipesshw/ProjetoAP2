#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <ctype.h>

#define VERMELHO     "\x1b[31m" //cor vermelha
#define VERDE   "\x1b[32m"
#define AZUL    "\x1b[34m" // cor azul
#define AMARELO  "\x1b[33m" // cor amarela
#define CIANO    "\x1b[36m" // cor ciano
#define MAGENTA  "\x1b[35m" // cor magenta
#define RESET   "\x1b[0m"  //resetar parar cor branca


int vendasRealizadas = 0;
int produtosExcluidos = 0;
int produtosVendidos = 0; // Declarando a variável globalmente
float totalVendido = 0;

// limpa tela
void limparTela()
{
    system("cls");
}

// Definição das structs
typedef struct
{
    int id;
    char nome[50];
    float preco;
    int quantidade;
} Produto;




// CADASTRO DEE PRODUTOS  /  INTERFACE: OK
void cadastrarProduto(FILE *arquivo)
{
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Produto novoProduto;
    Produto produtoExistente;
    int idValido = 0;
    int nomeValido = 0;
    int precoValido = 0;
    int quantValido = 0;
    limparTela();



    char idString[50]; // String para armazenar a entrada do ID

    do
    {
        printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|\t                  CRIAR PRODUTO                         |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|---------------------------------------------------------------|\n");
        printf("\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t         > Digite o ID do produto: ");
        scanf("%s", idString); // Lê a entrada como uma string

        // Verifica se todos os caracteres da string são dígitos
        int i;
        idValido = 1;
        for (i = 0; idString[i] != '\0'; i++)
        {
            if (!isdigit(idString[i]))
            {
                limparTela();
            printf("\n\t\t\t\t\t\t\t\t\t\t +-------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t |                                                             |\n");
            printf("\t\t\t\t\t\t\t\t\t\t |     "VERMELHO"O ID do produto deve conter apenas números positivos."VERDE"   |\n");
            printf("\t\t\t\t\t\t\t\t\t\t |                     "VERMELHO"Tente novamente..."VERDE"                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t |                                                             |\n");
            printf("\t\t\t\t\t\t\t\t\t\t +-------------------------------------------------------------+\n");
                idValido = 0;
                break;
            }
        }

        // Se todos os caracteres forem dígitos, converte a string para um número inteiro
        if (idValido)
        {
            novoProduto.id = atoi(idString);

            // Verifica se o ID do produto já existe
            rewind(arquivo);
            while (fread(&produtoExistente, sizeof(Produto), 1, arquivo))
            {
                if (produtoExistente.id == novoProduto.id)
                {
            printf("\n\t\t\t\t\t\t\t\t\t\t    +------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t    |                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t    |          "VERMELHO"Produto com o mesmo ID já existe."VERDE"           |\n");
            printf("\t\t\t\t\t\t\t\t\t\t    |                  "VERMELHO"Tente novamente..."VERDE"                  |\n");
            printf("\t\t\t\t\t\t\t\t\t\t    |                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t    +------------------------------------------------------+\n");
                    idValido = 0;
                    break;
                }
            }
        }
    } while (!idValido);





    getchar();

    // laço para solicitar um nome válido
do
{
        printf("\t\t\t\t\t\t\t\t\t\t\t         > Digite o NOME do produto: ");
    fgets(novoProduto.nome, sizeof(novoProduto.nome), stdin);
    novoProduto.nome[strcspn(novoProduto.nome, "\n")] = '\0'; // Remover a nova linha do nome

    if (strlen(novoProduto.nome) == 0)
    {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t              O nome do produto não pode estar vazio.        \n"VERDE);
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                         Tente novamente!        \n"VERDE);
            printf("\n");
            sleep(2);
        nomeValido = 0;
    }
    else
    {
        nomeValido = 1;

        // Verifica se o nome contém apenas caracteres
        for (int i = 0; novoProduto.nome[i] != '\0'; i++)
        {
            if (!isalpha(novoProduto.nome[i]) && novoProduto.nome[i] != ' ')
            {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t          O nome do produto deve conter apenas caracteres.        \n"VERDE);
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                          Tente novamente!        \n"VERDE);
            printf("\n");
            sleep(2);
                nomeValido = 0;
                break;
            }
        }

        if (nomeValido)
        {
            // Converter o nome para letras minúsculas (ou maiúsculas)
            for (int i = 0; novoProduto.nome[i]; i++)
            {
                novoProduto.nome[i] = tolower(novoProduto.nome[i]);
            }

            // Verifica se o nome do produto já existe
            rewind(arquivo);
            while (fread(&produtoExistente, sizeof(Produto), 1, arquivo))
            {
                // Converter o nome existente para letras minúsculas (ou maiúsculas) para comparação
                char nomeExistenteLower[50];
                strcpy(nomeExistenteLower, produtoExistente.nome);
                for (int i = 0; nomeExistenteLower[i]; i++)
                {
                    nomeExistenteLower[i] = tolower(nomeExistenteLower[i]);
                }

                if (strcmp(nomeExistenteLower, novoProduto.nome) == 0)
                {

            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                Produto com o mesmo nome já existe.        \n"VERDE);
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                        Tente novamente...              \n"VERDE );
            printf("\n");
            sleep(2);
                    nomeValido = 0;
                    break;
                }
            }
        }
    }
} while (!nomeValido);


// laço para solicitar um preço válid


    char precoString[50]; // String para armazenar a entrada do preço
 do {
        printf("\t\t\t\t\t\t\t\t\t\t\t         > Digite o PREÇO do produto: ");
        scanf("%s", precoString); // Lê a entrada como uma string

        char *ptr;
        novoProduto.preco = strtof(precoString, &ptr);

        // Verifica se a conversão foi bem sucedida e se o resto da string é vazio (sem caracteres adicionais)
        if (*ptr == '\0' && ptr != precoString) {
            precoValido = 1;
        } else {printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t              O Preço do produto deve ser um número válido.        \n"VERDE);
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t        Exemplo: se o preço for R$ 1,250,00 adicione como 1250,00              \n"VERDE );
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t          separe apenas as ultimas duas casas usando VIRGULA.              \n"VERDE );
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                         Tente novamente...              \n"VERDE );
            printf("\n");
            precoValido = 0;
        }

        if (novoProduto.preco <= 0) {

            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t              O Preço do produto deve ser um número válido.        \n"VERDE);
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t        Exemplo: se o preço for R$ 1,250,00 adicione como 1250,00              \n"VERDE );
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t          separe apenas as ultimas duas casas usando VIRGULA.              \n"VERDE );
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                         Tente novamente...              \n"VERDE );
            printf("\n");
            precoValido = 0;
        }
    } while (!precoValido);


    // laço para solicitar uma quantidade válida
    char quantString[50];
        do {
        printf("\t\t\t\t\t\t\t\t\t\t\t         > Digite a quantidade do produto: ");
        scanf("%s", quantString); // lê a entrada da quantidade como string
        int i;
        quantValido = 1;
        for (i = 0; quantString[i] != '\0'; i++) {
            if (!isdigit(quantString[i])) {
                  printf("\n");
                  printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t     A quantidade do produto deve ser apenas números positivos.        \n"VERDE);
                  printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                          Tente novamente!              \n"VERDE);
                  printf("\n");
                quantValido = 0;
                break;
            }
        }

        // Se todos os caracteres forem dígitos, converte a string para um número inteiro
        if (quantValido) {
            novoProduto.quantidade = atoi(quantString);

            // faz a verficaçaõ para saber se a quantidade é maior que zero
            if (novoProduto.quantidade <= 0) {
                printf("\n");
                printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t          A quantidade do produto deve ser maior que zero.              \n"VERDE);
                printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                          Tente novamente!             \n"VERDE);
                printf("\n");
                quantValido = 0;
            }
        }
    } while (!quantValido);




    // Adicionar o novo produto ao arquivo
    fseek(arquivo, 0, SEEK_END);
    fwrite(&novoProduto, sizeof(Produto), 1, arquivo);

    limparTela();
    printf("\n\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
    printf("\t\t\t\t\t\t\t\t\t\t|           Produto cadastrado com sucesso!            |\n");
    printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
    printf("\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
    sleep(2);
    limparTela();

}


// INTERFACE: OK
void buscarProduto(FILE *arquivo, int buscarPorId, int id, char *nome)
{
    Produto produto;
    rewind(arquivo);
    int encontrado = 0;
    limparTela();

    // Iterar sobre o arquivo procurando pelo produto com o ID ou nome fornecido
    while(fread(&produto, sizeof(Produto), 1, arquivo))
    {
        if ((buscarPorId && produto.id == id) || (!buscarPorId && strcmp(produto.nome, nome) == 0))
        {
            printf("\n\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                PRODUTO ENCONTRADO                       |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|----------------------------------------------------------------|\n");
            printf("\t\t\t\t\t\t\t\t\t\t|"AZUL"    ID: " VERDE "%-4d " RESET AZUL"Nome: " VERDE "%-10s " RESET AZUL "Preço: " VERDE "%-9.2f " RESET AZUL "Quantidade: " VERDE "%-4d |\n"RESET VERDE, produto.id, produto.nome, produto.preco, produto.quantidade);
            printf("\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            encontrado = 1;
        }
    }

    // Se o produto não for encontrado, exibir mensagem de não encontrado
    if (!encontrado)
    {
        if (buscarPorId) {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t\t        Produto com ID %d não encontrado.        \n",id, VERDE);
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                        Tente novamente...              \n"VERDE );
            printf("\n");
            sleep(2);
        } else {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t\t       Produto com NOME %s não encontrado.        \n",nome, VERDE);
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                        Tente novamente...              \n"VERDE );
            printf("\n");
            sleep(2);
    }

    }
}




void excluirProduto(FILE *arquivo, int id) {
    char entrada[50];
    int valido = 0;

    while (!valido) {
        printf("\n");
        fgets(entrada, sizeof(entrada), stdin);

        // Remover o caractere de nova linha, se houver
        entrada[strcspn(entrada, "\n")] = '\0';

        // Verificar se a entrada contém apenas dígitos
        valido = 1;
        for (int i = 0; i < strlen(entrada); i++) {
            if (!isdigit(entrada[i])) {
                valido = 0;
                break;
            }
        }

        if (!valido) {
            printf("\n");
        printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                    O ID do produto é inválido.        \n");
        printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t         O ID do produto deve ser um número inteiro.\n");
        printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                         Tente novamente!\n"VERDE);
        printf("\n");
            printf("\t\t\t\t\t\t\t\t\t\t\t  > Digite o ID do produto a ser excluído: ");
        } else {
            id <= atoi(entrada);
        }
    }

    FILE *tempArquivo;
    tempArquivo = fopen("temp.dat", "wb");
    if (tempArquivo == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return;
    }

    Produto produto;
    int algumProdutoExcluido = 0; // Variável para verificar se algum produto foi excluído

    rewind(arquivo);
    int encontrado = 0; // Variável para verificar se o produto foi encontrado
    while (fread(&produto, sizeof(Produto), 1, arquivo)) {
        if (produto.id != id) {
            fwrite(&produto, sizeof(Produto), 1, tempArquivo);
        } else {
            encontrado = 1; // Marcar que o produto foi encontrado
            algumProdutoExcluido = 1; // Marcar que algum produto foi excluído
        }
    }

    // Verificar se o produto foi encontrado
    if (!encontrado)
    {
            printf("\n\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                  EXCLUIR PRODUTO                        |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|----------------------------------------------------------------|\n");
            printf("\t\t\t\t\t\t\t\t\t\t|"VERMELHO"                Produto com ID %-3d não encontrado               "VERDE"|\n", id);
            printf("\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            sleep(2);
            limparTela();
    }
    else
    {
            printf("\n\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                  EXCLUIR PRODUTO                        |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|----------------------------------------------------------------|\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                 Produto %d excluido com sucesso!                |\n", id);
            printf("\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            sleep(2);
            limparTela();
    }

    // Se algum produto foi excluído, incrementa a variável produtosExcluidos
    if (algumProdutoExcluido) {
        produtosExcluidos++;
    }

    // Fechar ambos os arquivos
    fclose(arquivo);
    fclose(tempArquivo);

    // Remove o arquivo original
    remove("produtos.dat");

    // Renomeia o arquivo temporário para o nome original
    rename("temp.dat", "produtos.dat");

    // Reabre o arquivo principal para atualizar a variável arquivo
    arquivo = fopen("produtos.dat", "rb+");
    if (arquivo == NULL)
    {
        printf("Erro ao reabrir o arquivo.\n");
        exit(1);
    }
}




// recursiva: valor vendido
float Venda(int quant, float preco)
{
    if (quant == 0)
    {
        return 0;
    }
    else
    {
        return preco + Venda(quant - 1, preco);
    }
}





int contemApenasDigitos(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

// Função para criar uma nova venda
void criarNovaVenda(FILE *arquivo) {
    limparTela();
    int id;
    char input[50];
        printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|\t                REALIZAR VENDA                          |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
        printf("\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t   > Digite o ID do produto a ser vendido: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remover o caractere de nova linha
    while (!contemApenasDigitos(input) || sscanf(input, "%d", &id) != 1 || id <= 0) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remover o caractere de nova linha
        printf("\n");
        printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t           O ID do produto deve ser um número positivo.\n"VERDE);
        printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                       Tente novamente!\n"VERDE);
        printf("\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t   > Digite o ID do produto a ser vendido: ");

    }

    Produto produto;
    int encontrado = 0;

    FILE *tempArquivo;
    tempArquivo = fopen("temp.dat", "wb"); // Arquivo temporário

    rewind(arquivo);
    while(fread(&produto, sizeof(Produto), 1, arquivo)) {
        if (produto.id == id) {
                limparTela();
        printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|\t                REALIZAR VENDA                          |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|---------------------------------------------------------------|\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     PRODUTO     |   %-10s                                |\n", produto.nome);
        printf("\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
            encontrado = 1;
            int quantidadeVendida;
                printf("\t\t\t\t\t\t\t\t\t\t\t    > Digite a quantidade a ser vendido: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0; // Remover o caractere de nova linha
            while (!contemApenasDigitos(input) || sscanf(input, "%d", &quantidadeVendida) != 1 || quantidadeVendida <= 0 || quantidadeVendida > produto.quantidade) {
                printf("\n");
                printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t         Quantidade inválida ou insuficiente em estoque.        \n"VERDE);
                printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                         Tente novamente!        \n"VERDE);
                printf("\n");
                printf("\t\t\t\t\t\t\t\t\t\t\t    > Digite a quantidade a ser vendido: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0; // Remover o caractere de nova linha

            }


            produto.quantidade -= quantidadeVendida;
            float valorVenda = Venda(quantidadeVendida, produto.preco);

                        printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                 RESUMO DA VENDA                        |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|---------------------------------------------------------------|\n");
            printf("\t\t\t\t\t\t\t\t\t\t|     PRODUTO        |   %-10s                             |\n", produto.nome);
            printf("\t\t\t\t\t\t\t\t\t\t|     QUANTIDADE     |   %-10d                             |\n", quantidadeVendida);
            printf("\t\t\t\t\t\t\t\t\t\t|                    |                                          |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|     PREÇO TOTAL    |  R$ %-10.2f                           |\n", valorVenda);
            printf("\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
            totalVendido += valorVenda;

            // Atualizar a contagem de produtos vendidos
            produtosVendidos += quantidadeVendida;

            sleep(2);

            // Escrever o produto atualizado no arquivo temporário
            fwrite(&produto, sizeof(Produto), 1, tempArquivo);
        } else {
            // Escrever outros produtos no arquivo temporário sem modificação
            fwrite(&produto, sizeof(Produto), 1, tempArquivo);
        }
    }

    // Fechar ambos os arquivos
    fclose(arquivo);
    fclose(tempArquivo);

    // Remover o arquivo original
    remove("produtos.dat");

    // Renomear o arquivo temporário para o nome original
    rename("temp.dat", "produtos.dat");

    // Reabrir o arquivo principal para atualizar a variável arquivo
    arquivo = fopen("produtos.dat", "rb+");
    if (arquivo == NULL) {
        printf("Erro ao reabrir o arquivo.\n");
        exit(1);
    }

    if (!encontrado) {
                limparTela();
                printf("\n");
                printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                  Produto não foi encontrado.        \n"VERDE);
                printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t                       Tente novamente!        \n"VERDE);
                printf("\n");
                sleep(2);
                limparTela();
    } else {
        printf("\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t     Venda finalizada com sucesso!");
        vendasRealizadas++;
    }
}










// Lista produtos INTERFACE: OK!!
void listarProdutos(FILE *arquivo)
{
    Produto produto;
    rewind(arquivo);
    limparTela();

            printf("\n\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                  |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                  LISTA DE PRODUTOS                        |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                  |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|------------------------------------------------------------------|");


    // Verificar se o arquivo está vazio
    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0)
    {
            printf("\n\t\t\t\t\t\t\t\t\t\t|                     Nenhum produto cadastrado.                   |\n");
            printf("\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------------------+\n");
            sleep(2);
            limparTela();
        return;
    }
    rewind(arquivo);

    while(fread(&produto, sizeof(Produto), 1, arquivo))
    {
            printf("\n\t\t\t\t\t\t\t\t\t\t|"AZUL"    ID: " VERDE "%-4d " RESET AZUL"Nome: " VERDE "%-10s " RESET AZUL "Preço: " VERDE "%-9.2f " RESET AZUL "Quantidade: "VERDE "%-4d   |", produto.id, produto.nome, produto.preco, produto.quantidade);

    }
printf("\n\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------------------+\n");
    char res;
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t Você deseja realizar uma nova venda? [s/n]: ");
    scanf(" %c", &res);
    if(res == 's' || res == 'S')
    {
        criarNovaVenda(arquivo);
    }
}


//adiciona estoque


void adicionarEstoque(FILE *arquivo)
{
    limparTela();
    int id;
    char idString[50];
            printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                 ADICIONAR ESTOQUE                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+");
            printf("\n");
            printf("\t\t\t\t\t\t\t\t\t\t\t> Digite o ID do produto para adicionar estoque: ");
    scanf("%s", idString);

    // Verificar se todos os caracteres da string são dígitos
    int i;
    int idValido = 1;
    for (i = 0; idString[i] != '\0'; i++)
    {
        if (!isdigit(idString[i]))
        {
            idValido = 0;
            break;
        }
    }

    // Se a string contém apenas dígitos, convertemos para um número inteiro
    if (idValido)
    {
        id = atoi(idString);
    }
    else
    {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t\t        ID inválido. Tente novamente!\n"VERDE);
            printf("\n");
        return;
    }

    // Verificar se o ID é positivo
    if (id < 0)
    {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t\t        ID do produto inválido. Digite um número inteiro positivo.!\n",VERDE);
            printf("\n");
        printf("\n");
        return;
    }

    FILE *tempArquivo;
    tempArquivo = fopen("temp.dat", "wb"); // Arquivo temporário

    if (tempArquivo == NULL)
    {
        printf("Erro ao abrir o arquivo temporário.\n");
        return;
    }

    Produto produto;
    int encontrado = 0;

    // Voltar para o início do arquivo
    rewind(arquivo);

    while (fread(&produto, sizeof(Produto), 1, arquivo))
    {
        if (produto.id == id)
        {
            encontrado = 1;
            int quantidadeAdicionar;
            char quantidadeString[50];
            printf("\t\t\t\t\t\t\t\t\t\t        > Digite a quantidade a ser adicionada ao estoque: ");
            scanf("%s", quantidadeString);

            // Verificar se todos os caracteres da string são dígitos
            int i;
            int quantidadeValida = 1;
            for (i = 0; quantidadeString[i] != '\0'; i++)
            {
                if (!isdigit(quantidadeString[i]))
                {
                    quantidadeValida = 0;
                    break;
                }
            }

            // Se a string contém apenas dígitos, convertemos para um número inteiro
            if (quantidadeValida)
            {
                quantidadeAdicionar = atoi(quantidadeString);
            }
            else
            {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t\tQuantidade inválida. Digite um número inteiro positivo!\n"VERDE);
            printf("\n");
                return;
            }

            // Verificar se a quantidade é positiva
            if (quantidadeAdicionar <= 0)
            {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t\tQuantidade inválida. Digite um número inteiro positivo!\n"VERDE);
            printf("\n");
                return;
            }

            // Atualizar a quantidade do produto
            produto.quantidade += quantidadeAdicionar;
            printf("\n");
        printf(VERDE"\t\t\t\t\t\t\t\t\t\t\t        Estoque atualizado com sucesso.        \n");
            printf("\n");
        }

        // Escrever o produto no arquivo temporário
        fwrite(&produto, sizeof(Produto), 1, tempArquivo);
    }

    // Se o produto não foi encontrado, mantemos o arquivo inalterado
    if (!encontrado)
    {
            printf("\n");
            printf(VERMELHO"\t\t\t\t\t\t\t\t\t\t\t             Produto não encontrado!\n"VERDE);
            printf("\n");

        // Copiar todos os produtos do arquivo original para o arquivo temporário
        rewind(arquivo);
        while (fread(&produto, sizeof(Produto), 1, arquivo))
        {
            fwrite(&produto, sizeof(Produto), 1, tempArquivo);
        }
    }

    // Fechar ambos os arquivos
    fclose(arquivo);
    fclose(tempArquivo);

    // Remover o arquivo original
    remove("produtos.dat");

    // Renomear o arquivo temporário para o nome original
    rename("temp.dat", "produtos.dat");

    // Reabrir o arquivo principal para atualizar a variável arquivo
    arquivo = fopen("produtos.dat", "rb+");
    if (arquivo == NULL)
    {
        printf("Erro ao reabrir o arquivo.\n");
        exit(1);
    }
}






//INTERFACE: OK
void ListaSemEstoque(FILE *arquivo) {
    Produto produto;
    rewind(arquivo);
            printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t            LISTAR PRODUTOS SEM ESTOQUE                 |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|---------------------------------------------------------------|");

    // Verificar se o arquivo está vazio
    fseek(arquivo, 0, SEEK_END);
    rewind(arquivo);

    int encontrado = 0; // Variável para indicar se foi encontrado produto sem estoque

    while (fread(&produto, sizeof(Produto), 1, arquivo)) {
        if (produto.quantidade == 0) {
            printf("\n\t\t\t\t\t\t\t\t\t\t|"AZUL"    ID: " VERDE "%-4d " RESET AZUL"Nome: " VERDE "%-10s " RESET AZUL "Preço: " VERDE "%-9.2f " RESET AZUL "Quantidade: "VERDE "%d   |", produto.id, produto.nome, produto.preco, produto.quantidade);

printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");

            encontrado = 1; // Produto sem estoque encontrado
    char res;
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t\t\tVocê adicionar estoque em algum produto? [s/n]: ");
    scanf(" %c", &res);
    if(res == 's' || res == 'S')
    {
        adicionarEstoque(arquivo);
        break;
    }

    }
    }


    if (!encontrado || produto.quantidade > 1) {
            printf("\n\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|          "VERMELHO"Não foi encontrado nenhum produto sem estoque."VERDE"       |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                      "VERMELHO" Tente novamente..."VERDE"                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                               |");
            printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
            sleep(3);
            limparTela();
    }


}




//gera relatorio INTERFACE: OK
void gerarRelatorio(FILE *arquivo)
{
    FILE *relatorioArquivo;
    relatorioArquivo = fopen("relatorio.txt", "w"); // Abre o arquivo de relatório para escrita

    if (relatorioArquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de relatório.\n");
        return;
    }

    int totalVendas = vendasRealizadas; // Total de vendas realizadas
    float totalVendasR = 0;             // Total em R$ das vendas realizadas
    int produtosAdicionados = 0;        // Total de produtos adicionados

    Produto produto;

    // Contagem do número de produtos adicionados
    rewind(arquivo);
    while (fread(&produto, sizeof(Produto), 1, arquivo))
    {
        produtosAdicionados++;
    }

    // Escrevendo os detalhes no relatório
    fprintf(relatorioArquivo, "Relatório:\n");
    fprintf(relatorioArquivo, "Número de vendas realizadas: %d\n", totalVendas);
    fprintf(relatorioArquivo, "Número de produtos adicionados: %d\n", produtosAdicionados);
    fprintf(relatorioArquivo, "Número de produtos excluídos: %d\n", produtosExcluidos);
    fprintf(relatorioArquivo, "Número de produtos vendidos: %d\n", produtosVendidos);

    // Escrevendo detalhes sobre os produtos adicionados
    rewind(arquivo);
    fprintf(relatorioArquivo, "\nDetalhes dos produtos existentes:\n");
    fprintf(relatorioArquivo, "-------------------------------\n");
    while (fread(&produto, sizeof(Produto), 1, arquivo))
    {
        fprintf(relatorioArquivo, "ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
    }

    fprintf(relatorioArquivo, "\nTotal em R$ das vendas realizadas: %.2f\n", totalVendido);
    fclose(relatorioArquivo);
    sleep(1);
        printf("\n\t\t\t\t\t\t\t\t\t\t +-----------------------------------------------------+\n");
        printf("\t\t\t\t\t\t\t\t\t\t |                                                     |\n");
        printf("\t\t\t\t\t\t\t\t\t\t |\t      Relatório gerado com sucesso!            |\n");
        printf("\t\t\t\t\t\t\t\t\t\t |                                                     |\n");
        printf("\t\t\t\t\t\t\t\t\t\t +-----------------------------------------------------+\n");
    sleep(2);
    limparTela();
}




//menu
void menuGP(FILE *arquivo)   //menu gerenciar programa
{
    int opcao;
    char input[100]; // Buffer para entrada do usuário

    limparTela();
    do
    {
        printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|\t                GERENCIAR PRODUTOS                      |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|---------------------------------------------------------------|\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 1     |       Criar produto                        |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 2     |       Adicionar estoque de um produto      |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 3     |       Excluir um produto                   |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 4    |        Listar todos produtos                |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 5    |        Listar produtos sem estoque          |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 6    |        Buscar produto específico            |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 0    |        Voltar ao menu principal             |");
        printf("\n\t\t\t\t\t\t\t\t\t\t+---------------------------------------------------------------+\n");
        printf("\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t           > Escolha uma opção: ");
        scanf("%s", input);
    // Verifica se todos os caracteres da entrada são dígitos
    int i;
    int isNumber = 1;
    for (i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            isNumber = 0;
            break;
        }
    }

    if (!isNumber) {
        limparTela();
        printf("\n\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|   "VERMELHO"A entrada deve ser um número. Tente novamente."VERDE"     |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                    "VERMELHO"Retornando..."VERDE"                     |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
        printf("\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
        sleep(2);
        limparTela();
        continue; // Volta para o início do loop
    }

    opcao = atoi(input); // Converte a entrada para inteiro



        switch(opcao)
        {
        case 1:
            limparTela();
            cadastrarProduto(arquivo);
            break;
        case 2:
            adicionarEstoque(arquivo);
            break;
        case 3:
            limparTela();
            printf("\n\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                  EXCLUIR PRODUTO                        |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|----------------------------------------------------------------|\n");
            printf("\t\t\t\t\t\t\t\t\t\t\t  > Digite o ID do produto a ser excluído: ");
            int idExcluir;
            scanf("%d", &idExcluir);
            excluirProduto(arquivo, idExcluir);
            break;
        case 4:
            limparTela();
            listarProdutos(arquivo);
            break;
        case 5:
            limparTela();
            ListaSemEstoque(arquivo);
            break;
        case 6:
            limparTela();
            printf("\n\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|\t                   BUSCAR PRODUTO                        |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|----------------------------------------------------------------|\n");
            printf("\t\t\t\t\t\t\t\t\t\t|               Escolha como deseja buscar o produto:            |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|        Digite 1     |      Para realizar busca pelo ID         |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|        Digite 2     |      Para realizar busca pelo NOME       |\n");
            printf("\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
            printf("\n");
            printf("\t\t\t\t\t\t\t\t\t\t\t           > Escolha uma opção: ");
            int escolha;
            scanf("%d", &escolha);
            if (escolha == 1)
            {
                limparTela();
                printf("\n\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|\t                   BUSCAR PRODUTO                        |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|----------------------------------------------------------------|\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|            Você escolheu realizar uma busca por id.            |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
                printf("\n");
                printf("\t\t\t\t\t\t\t\t\t\t\t   > Digite o ID do produto a ser buscado: ");
                int id;
                scanf("%d", &id);
                buscarProduto(arquivo, 1, id, "");
            }
            else if (escolha == 2)
            {
                limparTela();
                printf("\n\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|\t                   BUSCAR PRODUTO                        |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|----------------------------------------------------------------|\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|            Você escolheu realizar uma busca por nome.          |\n");
                printf("\t\t\t\t\t\t\t\t\t\t|                                                                |\n");
                printf("\t\t\t\t\t\t\t\t\t\t+----------------------------------------------------------------+\n");
                printf("\n");
                printf("\t\t\t\t\t\t\t\t\t\t\t > Digite o NOME do produto a ser buscado: ");
                char nome[50];
                scanf("%s", nome);
                buscarProduto(arquivo, 0, 0, nome);
            }
            else
            {
            limparTela();
            printf("\n\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|          "VERMELHO"Opção inválida, tente novamente."VERDE"            |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                     "VERMELHO"Retornando..."VERDE"                    |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
            sleep(2);
            limparTela();
            }
            break;
        case 0:
            limparTela();
            break;
        default:
            limparTela();
            printf("\n\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|          "VERMELHO"Opção inválida, tente novamente."VERDE"            |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                     "VERMELHO"Retornando..."VERDE"                    |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
            sleep(2);
            limparTela();
            break;
        }
    }
    while(opcao != 0);
}
// Função principal
int main()
{

    system("color 02");
    setlocale(LC_ALL, "");
    FILE *produtosArquivo;
    produtosArquivo = fopen("produtos.dat", "ab+"); // Abre o arquivo para leitura e escrita binária, cria se não existir

    if (produtosArquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;
    do
    {
        printf("\n");
        printf("\t\t\t\t\t\t\t\t                                                     ##                       ###\n");
        printf("\t\t\t\t\t\t\t\t                                                     ##                        ##\n");
        printf("\t\t\t\t\t\t\t\t       ####    ##  ##   ######             #####    #####    ####     ####     ##  ##\n");
        printf("\t\t\t\t\t\t\t\t      ##  ##   #######   ##  ##  ######   ##         ##     ##  ##   ##  ##    ## ##\n");
        printf("\t\t\t\t\t\t\t\t      ##       ## # ##   ##  ##            #####     ##     ##  ##   ##        ####\n");
        printf("\t\t\t\t\t\t\t\t      ##  ##   ##   ##   #####                 ##    ## ##  ##  ##   ##  ##    ## ##\n");
        printf("\t\t\t\t\t\t\t\t       ####    ##   ##   ##               ######      ###    ####     ####     ##  ##\n");
        printf("\t\t\t\t\t\t\t\t                        ####\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\t  v1.0\n");
        printf("\n\t\t\t\t\t\t\t\t\t\t+-------------------------------------------------------+\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                       |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|\t              MENU PRINCIPAL                    |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                                                       |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|-------------------------------------------------------|\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 1     |        Gerenciar produtos          |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 2     |        Realizar nova venda         |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 3     |        Gerar relatório             |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|                  |                                    |\n");
        printf("\t\t\t\t\t\t\t\t\t\t|     Digite 0     |        Finalizar programa          |\n");
        printf("\t\t\t\t\t\t\t\t\t\t+-------------------------------------------------------+\n");
        printf("\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t       > Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao)
        {
        case 1:
            menuGP(produtosArquivo);
            break;
        case 2:
            criarNovaVenda(produtosArquivo);
            break;
        case 3:
            gerarRelatorio(produtosArquivo);
            break;
        case 0:
            printf("\t\t\t\t\t\t\t\t\t\t\t           Finalizando... ");
            sleep(1);
            break;
        default:
            limparTela();
            printf("\n\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|          "VERMELHO"Opção inválida, tente novamente."VERDE"            |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                        "VERMELHO"Retornando..."VERDE"                    |\n");
            printf("\t\t\t\t\t\t\t\t\t\t|                                                      |\n");
            printf("\t\t\t\t\t\t\t\t\t\t+------------------------------------------------------+\n");
            sleep(2);
            limparTela();
        }
    }
    while(opcao != 0);

    fclose(produtosArquivo); // Fecha o arquivo
    return 0;
}
