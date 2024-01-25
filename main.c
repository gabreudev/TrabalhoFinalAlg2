#include<stdio.h>
#include "quarto.h"

int main() {
    FILE *file = fopen("quarto.dat", "ab+");  // Abre ou cria o arquivo para leitura e escrita binária

    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;

    do {
        printf("Menu:\n");
        printf("1 - Cadastrar Quarto\n");
        printf("2 - Listar Quartos\n");
        printf("3 - Apagar Quarto por Número\n");
        printf("4 - Atualizar Status do Quarto\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Quarto novo = obterNovoQuarto();
                cadastrarQuarto(file, novo);
                break;
            }
            case 2:
                listarQuartos(file);
                break;
            case 3: {
                int numero;
                printf("Digite o número do quarto que deseja apagar: ");
                scanf("%d", &numero);

                if (apagarQuartoPorNumero(file, numero)) {
                    printf("Quarto apagado com sucesso.\n");
                } else {
                    printf("Quarto não encontrado.\n");
                }
                break;
            }
            case 4:
                atualizarStatusQuarto(file);
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    fclose(file);  // Fecha o arquivo

    return 0;
}

