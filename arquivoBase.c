#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

int vendasRealizadas = 0;
int produtosExcluidos = 0;

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

// Funções para operações de estoque

//Cadastro
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
    do
    {
        printf("Digite o ID do produto: ");
        scanf("%d", &novoProduto.id);
        if (novoProduto.id < 0)
        {
            printf("O ID do produto não pode ser negativo ou inválido. Tente novamente!\n");
            while (getchar() != '\n');
        }
        else
        {
            // Verifica se o ID do produto já existe
            rewind(arquivo);
            idValido = 1;
            while (fread(&produtoExistente, sizeof(Produto), 1, arquivo))
            {
                if (produtoExistente.id == novoProduto.id)
                {
                    printf("Produto com o mesmo ID já existe. Tente novamente!\n");
                    idValido = 0;
                    break;
                }
            }
        }
    }
    while (novoProduto.id < 0 || idValido == 0);

    getchar();

    // laço para solicitar um nome válido
    do
    {
        printf("Digite o nome do produto: ");
        fgets(novoProduto.nome, sizeof(novoProduto.nome), stdin);
        novoProduto.nome[strcspn(novoProduto.nome, "\n")] = '\0'; // Remover a nova linha do nome

        if (strlen(novoProduto.nome) == 0)
        {
            printf("O nome do produto não pode estar vazio. Tente novamente!\n");
        }
        else
        {
            // Verifica se o nome do produto já existe
            rewind(arquivo);
            nomeValido = 1;
            while (fread(&produtoExistente, sizeof(Produto), 1, arquivo))
            {
                if (strcmp(produtoExistente.nome, novoProduto.nome) == 0)
                {
                    printf("Produto com o mesmo nome já existe. Tente novamente!\n");
                    nomeValido = 0;
                    break;
                }
            }
        }
    }
    while (nomeValido == 0);

    // laço para solicitar um preço válido
    do
    {
        printf("Digite o preço do produto: ");
        scanf("%f", &novoProduto.preco);
        if ( novoProduto.preco < 0)
        {
            printf("O preço do produto deve ser um número válido e não negativo. Tente novamente!\n");
            while (getchar() != '\n'); // Limpar o buffer do teclado
        }
        else
        {
            precoValido = 1;
        }
    }
    while (precoValido == 0);

    // laço para solicitar uma quantidade válida
    do
    {
        printf("Digite a quantidade do produto: ");
        scanf("%d", &novoProduto.quantidade);
        if (novoProduto.quantidade < 0)
        {
            printf("A quantidade do produto deve ser um número inteiro válido e não negativo. Tente novamente!\n");
            while (getchar() != '\n'); // Limpar o buffer do teclado
        }
        else
        {
            quantValido = 1;
        }
    }
    while (quantValido == 0);

    // Adicionar o novo produto ao arquivo
    fseek(arquivo, 0, SEEK_END);
    fwrite(&novoProduto, sizeof(Produto), 1, arquivo);

    printf("Produto cadastrado com sucesso!\n");

}

//busca
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
            printf("Produto encontrado:\n");
            printf("ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
            encontrado = 1;
        }
    }

    // Se o produto não for encontrado, exibir mensagem de não encontrado
    if (!encontrado)
    {
        if (buscarPorId)
            printf("Produto com ID %d não encontrado.\n", id);
        else
            printf("Produto com nome '%s' não encontrado.\n", nome);
    }
}

// Excluir produto
void excluirProduto(FILE *arquivo, int id)

{
    while (id < 0)
    {
        printf("O ID do produto é inválido. Tente novamente!\n");
        printf("Digite o ID do produto a ser excluído: ");
        scanf("%d", &id);
    }

    FILE *tempArquivo;
    Produto produto;

    // Abre um arquivo temporário para escrita
    tempArquivo = fopen("temp.dat", "wb");
    if (tempArquivo == NULL)
    {
        printf("Erro ao abrir o arquivo temporário.\n");
        return;
    }

    rewind(arquivo);
    while(fread(&produto, sizeof(Produto), 1, arquivo))
    {
        if (produto.id != id)
        {
            fwrite(&produto, sizeof(Produto), 1, tempArquivo);
            produtosExcluidos++;
        }
    }

    // Fecha ambos os arquivos
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
    if(quant==1)
    {
        return preco;
    }
    else
    {
        return preco+Venda( quant-1,preco);
    }
}

//cria nova venda
void criarNovaVenda(FILE *arquivo)
{

    limparTela();
    int id;
    printf("Digite o ID do produto vendido: ");
    scanf("%d", &id);

    while (id < 0)
    {
        printf("O Produto não foi encontrado. Tente novamente!\n");
        printf("Digite o ID do produto vendido: ");
        scanf("%d", &id);
    }

    Produto produto;
    int encontrado = 0;

    FILE *tempArquivo;
    tempArquivo = fopen("temp.dat", "wb"); // Arquivo temporário

    rewind(arquivo);
    while(fread(&produto, sizeof(Produto), 1, arquivo))
    {
        if (produto.id == id)
        {
            encontrado = 1;
            int quantidadeVendida;
            printf("Digite a quantidade vendida: ");
            scanf("%d", &quantidadeVendida);

            while (quantidadeVendida <= 0)
            {
                printf("Quantidade inválida de produtos. Tente novamente!\n");
                printf("Digite a quantidade vendida: ");
                scanf("%d", &quantidadeVendida);
            }

            if (quantidadeVendida <= 0)
            {
                printf("Quantidade inválida de produtos.\n");
                fclose(tempArquivo);
                remove("temp.dat");
                return;
            }

            if (quantidadeVendida > produto.quantidade)
            {
                printf("Quantidade insuficiente em estoque.\n");
                fclose(tempArquivo);
                remove("temp.dat");
                return;
            }

            produto.quantidade -= quantidadeVendida;
            printf("\n \tForam vendidos: %.2f em produtos \n", Venda(quantidadeVendida,produto.preco));

            sleep(2);

            // Escrever o produto atualizado no arquivo temporário
            fwrite(&produto, sizeof(Produto), 1, tempArquivo);
        }
        else
        {
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
    if (arquivo == NULL)
    {
        printf("Erro ao reabrir o arquivo.\n");
        exit(1);
    }

    if (!encontrado)
        printf("Produto não foi encontrado. Tente novamente!\n");
    else
        printf("Venda realizada com sucesso.\n");
        vendasRealizadas++;
}



// Lista produtos
void listarProdutos(FILE *arquivo)
{
    Produto produto;
    rewind(arquivo);
    limparTela();
    printf("\nLista de Produtos:\n");

    // Verificar se o arquivo está vazio
    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0)
    {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    rewind(arquivo);

    while(fread(&produto, sizeof(Produto), 1, arquivo))
    {
        printf("\t ID: %d  || Nome: %s || Preço: %.2f  || Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
    }

    char res;
    printf("\n Você deseja realizar uma nova venda? [s/n]:");
    scanf("%s", &res);
    if(res == 's' || res == 'S')
    {
        criarNovaVenda(arquivo);
    }
}

// Lista produtos sem estoque
void ListaSemEstoque(FILE *arquivo)
{
    Produto produto;
    rewind(arquivo);
    printf("\n \t Produtos sem estoque\n");

// Verificar se o arquivo está vazio
    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0)
    {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    rewind(arquivo);
    while(fread(&produto, sizeof(Produto), 1, arquivo))
    {
        if(produto.quantidade==0)
        {

            printf("\t ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
        }
    }
}

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
    float totalVendasR = 0; // Total em R$ das vendas realizadas
    int produtosAdicionados = 0; // Total de produtos adicionados
    int produtosVendidos = 0; // Total de produtos vendidos

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
    fprintf(relatorioArquivo, "\nDetalhes dos produtos adicionados:\n");
    fprintf(relatorioArquivo, "-------------------------------\n");
    while (fread(&produto, sizeof(Produto), 1, arquivo))
    {
        fprintf(relatorioArquivo, "ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
    }

    // Calcular o total em R$ das vendas realizadas
    rewind(arquivo);
    totalVendasR = 0;
    while (fread(&produto, sizeof(Produto), 1, arquivo))
    {
        totalVendasR += (produto.preco * (5 - produto.quantidade)); // Corrigido para calcular corretamente o total em R$
        produtosVendidos += (5 - produto.quantidade); // Corrigido para contar corretamente o número de produtos vendidos
    }

    fprintf(relatorioArquivo, "\n\nTotal em R$ das vendas realizadas: %.2f\n", totalVendasR);
    fclose(relatorioArquivo);
    printf("Relatório gerado com sucesso.\n");
}


//menu
void menuGP(FILE *arquivo)   //menu gerenciar programa
{
    int opcao;
    limparTela();
    do
    {
        printf("\n Menu:\n");
        printf("1 - Criar produto\n");
        printf("2 - Adicionar estoque de um produto\n");
        printf("3 - Excluir produto\n");
        printf("\n");
        printf("4 - Listar todos produtos\n");
        printf("5 - Listar produtos sem estoque\n");
        printf("6 - Buscar produto específico\n");
        printf("\n");
        printf("\n");
        printf("0 - Voltar para menu principal\n\n");
        printf(" Escolha uma opção: \n");
        scanf("%d", &opcao);

        switch(opcao)
        {
        case 1:
            limparTela();
            cadastrarProduto(arquivo);
            break;
        case 2:
            break;
        case 3:
            printf("Digite o ID do produto a ser excluído: ");
            int idExcluir;
            scanf("%d", &idExcluir);
            excluirProduto(arquivo, idExcluir);
            printf("Produto excluído com sucesso.\n");
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
            printf("Escolha como deseja buscar o produto:\n");
            printf("1 - Por ID\n");
            printf("2 - Por nome\n");
            printf("Opção: ");
            int escolha;
            scanf("%d", &escolha);
            if (escolha == 1)
            {
                printf("Digite o ID do produto a ser buscado: ");
                int id;
                scanf("%d", &id);
                buscarProduto(arquivo, 1, id, "");
            }
            else if (escolha == 2)
            {
                printf("Digite o nome do produto a ser buscado: ");
                char nome[50];
                scanf("%s", nome);
                buscarProduto(arquivo, 0, 0, nome);
            }
            else
            {
                printf("Opção inválida.\n");
            }
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opção inválida.\n");
        }
    }
    while(opcao != 0);
}

// Função principal
int main()
{

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
        printf("\n   Menu:\n");
        printf("1 - Gerenciar produtos\n"); // falta apenas tratamento de erros
        printf("2 - Realizar nova venda\n"); // falta apenas tratamento de erros
        printf("3 - Gerar relatório\n");
        printf("0 - Sair\n\n");
        printf(" Escolha uma opção: \n");
        scanf("%d", &opcao);

        switch (opcao)
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
            printf("Finalizando...\n");
            break;
        default:
            printf("\t Opção inválida.\n");
        }
    } while (opcao != 0);

    fclose(produtosArquivo);
    return 0;
}
