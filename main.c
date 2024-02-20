#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void limparTela() {
    system("cls");
}

// Definição das structs
typedef struct {
    int id;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

// Funções para operações de estoque
void cadastrarProduto(FILE *arquivo) {
    Produto produto;
    printf("Digite o ID do produto: ");
    scanf("%d", &produto.id);
    printf("Digite o nome do produto: ");
    getchar(); // Limpa o buffer do teclado
    fgets(produto.nome, sizeof(produto.nome), stdin); // Permite espaços no nome
    produto.nome[strcspn(produto.nome, "\n")] = '\0'; // Remove a nova linha do nome
    printf("Digite o preco do produto: ");
    scanf("%f", &produto.preco);
    printf("Digite a quantidade do produto: ");
    scanf("%d", &produto.quantidade);
    fwrite(&produto, sizeof(Produto), 1, arquivo);
    limparTela();
}

void listarProdutos(FILE *arquivo) {
    Produto produto;
    rewind(arquivo);
    limparTela();
    printf("\nLista de Produtos:\n");

    // Verificar se o arquivo está vazio
    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    rewind(arquivo);

    while(fread(&produto, sizeof(Produto), 1, arquivo)) {
        printf("ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
    }
}

void buscarProduto(FILE *arquivo, int buscarPorId, int id, char *nome) {
    Produto produto;
    rewind(arquivo);
    int encontrado = 0;

    // Iterar sobre o arquivo procurando pelo produto com o ID ou nome fornecido
    while(fread(&produto, sizeof(Produto), 1, arquivo)) {
        if ((buscarPorId && produto.id == id) || (!buscarPorId && strcmp(produto.nome, nome) == 0)) {
            printf("Produto encontrado:\n");
            printf("ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
            encontrado = 1;
        }
    }

    // Se o produto não for encontrado, exibir mensagem de não encontrado
    if (!encontrado) {
        if (buscarPorId)
            printf("Produto com ID %d não encontrado.\n", id);
        else
            printf("Produto com nome '%s' não encontrado.\n", nome);
    }
}

void excluirProduto(FILE *arquivo, int id) {
    FILE *tempArquivo;
    Produto produto;

    // Abre um arquivo temporário para escrita
    tempArquivo = fopen("temp.dat", "wb");
    if (tempArquivo == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return;
    }

    rewind(arquivo);
    while(fread(&produto, sizeof(Produto), 1, arquivo)) {
        if (produto.id != id) {
            // Escreve o produto no arquivo temporário se o ID não corresponder ao ID a ser excluído
            fwrite(&produto, sizeof(Produto), 1, tempArquivo);
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
    if (arquivo == NULL) {
        printf("Erro ao reabrir o arquivo.\n");
        exit(1);
    }
}

void menuGP(FILE *arquivo) { //menu gerenciar programa
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1 - Criar produto\n");
        printf("2 - Listar todos produtos\n");
        printf("3 - Buscar produto específico\n");
        printf("4 - Excluir produto\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                limparTela();
                cadastrarProduto(arquivo);
                break;
            case 2:
                limparTela();
                listarProdutos(arquivo);
                break;
            case 3:
                printf("Escolha como deseja buscar o produto:\n");
                printf("1 - Por ID\n");
                printf("2 - Por nome\n");
                printf("Opção: ");
                int escolha;
                scanf("%d", &escolha);
                if (escolha == 1) {
                    printf("Digite o ID do produto a ser buscado: ");
                    int id;
                    scanf("%d", &id);
                    buscarProduto(arquivo, 1, id, "");
                } else if (escolha == 2) {
                    printf("Digite o nome do produto a ser buscado: ");
                    char nome[50];
                    scanf("%s", nome);
                    buscarProduto(arquivo, 0, 0, nome);
                } else {
                    printf("Opção inválida.\n");
                }
                break;
            case 4:
                printf("Digite o ID do produto a ser excluído: ");
                int idExcluir;
                scanf("%d", &idExcluir);
                excluirProduto(arquivo, idExcluir);
                printf("Produto excluído com sucesso.\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while(opcao != 0);
}

// Função principal
int main() {
    FILE *produtosArquivo;
    produtosArquivo = fopen("produtos.dat", "ab+"); // Abre o arquivo para leitura e escrita binária, cria se não existir

    if (produtosArquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1 - Gerenciar produtos\n "); // falta apenas tratamento de erros
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                limparTela();
                menuGP(produtosArquivo);
                break;
            case 2:
                break;
            case 0:
                printf("Finalizando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while(opcao != 0);

    fclose(produtosArquivo);
    return 0;
}
