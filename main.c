#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição das structs
typedef struct {
    int id;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct {
    int id;
    char nome[50];
    char endereco[100];
} Cliente;

typedef struct {
    int id;
    int idCliente;
    int idProduto;
    int quantidade;
} Pedido;


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
}

void listarProdutos(FILE *arquivo) {
    Produto produto;
    rewind(arquivo);
    printf("\nLista de Produtos:\n");
    while(fread(&produto, sizeof(Produto), 1, arquivo)) {
        printf("ID: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", produto.id, produto.nome, produto.preco, produto.quantidade);
    }
}


void menuGP() {
    FILE *produtosArquivo;
    produtosArquivo = fopen("produtos.dat", "ab+"); // Abre o arquivo para leitura e escrita binária, cria se não existir

    if (produtosArquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
    }

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
                cadastrarProduto(produtosArquivo);
                break;
            case 2:
                listarProdutos(produtosArquivo);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while(opcao != 0);

    fclose(produtosArquivo);

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
        printf("1 - Gerenciar produtos\n");
        printf("2 - Gerenciar pedidos\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
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
