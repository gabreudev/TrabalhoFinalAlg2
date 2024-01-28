#include <stdio.h>
#include "Quarto.h"

Quarto obterNovoQuarto() {
    Quarto novo;

    printf("Digite o número do quarto: ");
    scanf("%d", &novo.numero);
    printf("Digite o preço da diária: ");
    scanf("%f", &novo.preco);
    printf("Digite o tipo do quarto (1 - Simples, 2 - Duplo, 3 - Suite): ");
    scanf("%d", &novo.tipoQuarto);
    
    switch (novo.tipoQuarto) {
        case 1:
            novo.tipoQuarto = Simples;
            break;
        case 2:
            novo.tipoQuarto = Duplo;
            break;
        case 3:
            novo.tipoQuarto = Suite;
            break;
        default:
            printf("Tipo de quarto inválido. Usando Desconhecido.\n");
            novo.tipoQuarto = -1;  // Valor inválido
    }

    return novo;
}

void cadastrarQuarto(FILE *file, Quarto quarto) {
    fwrite(&quarto, sizeof(Quarto), 1, file);
}

void listarQuartos(FILE *file) {
    Quarto quarto;

    rewind(file);  // Posiciona o ponteiro no início do arquivo

    printf("Lista de Quartos:\n");

    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        printf("Número: %d\n", quarto.numero);
        printf("Preço: %.2f\n", quarto.preco);

        switch (quarto.tipoQuarto) {
            case Simples:
                printf("Tipo: Simples\n");
                break;
            case Duplo:
                printf("Tipo: Duplo\n");
                break;
            case Suite:
                printf("Tipo: Suite\n");
                break;
            default:
                printf("Tipo: Desconhecido\n");
        }

        printf("Status: ");
        switch (quarto.status) {
            case Livre:
                printf("Livre\n");
                break;
            case Ocupado:
                printf("Ocupado\n");
                break;
            case Reservado:
                printf("Reservado\n");
                break;
            default:
                printf("Desconhecido\n");
        }

        printf("\n");
    }
}int apagarQuartoPorNumero(FILE *file, int numero) {
    FILE *tempFile = fopen("temp.bin", "wb");

    if (!tempFile) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return 0;
    }

    Quarto quarto;
    long posicaoAtual;

    rewind(file);

    int encontrado = 0;

    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        posicaoAtual = ftell(file);  // Salva a posição atual do ponteiro de arquivo

        if (quarto.numero == numero) {
            printf("Quarto encontrado e apagado.\n");
            encontrado = 1;
        } else {
            fwrite(&quarto, sizeof(Quarto), 1, tempFile);
        }

        fseek(file, posicaoAtual, SEEK_SET);  // Reposiciona o ponteiro após a leitura
    }

    fclose(file);
    fclose(tempFile);

    remove("quarto.bin");
    rename("temp.bin", "quarto.bin");

    return encontrado;
}


void atualizarStatusQuarto(FILE *file) {
    int numero;
    printf("Digite o número do quarto que deseja atualizar: ");
    scanf("%d", &numero);

    Quarto quarto;
    int encontrado = 0;

    rewind(file);  // Posiciona o ponteiro no início do arquivo

    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        if (quarto.numero == numero) {
            encontrado = 1;
            break;
        }
    }

    if (encontrado) {
        printf("Quarto encontrado:\n");
        printf("Número: %d\n", quarto.numero);
        printf("Preço: %.2f\n", quarto.preco);

        switch (quarto.tipoQuarto) {
            case Simples:
                printf("Tipo: Simples\n");
                break;
            case Duplo:
                printf("Tipo: Duplo\n");
                break;
            case Suite:
                printf("Tipo: Suite\n");
                break;
            default:
                printf("Tipo: Desconhecido\n");
        }

        printf("Status atual: ");
        switch (quarto.status) {
            case Livre:
                printf("Livre\n");
                break;
            case Ocupado:
                printf("Ocupado\n");
                break;
            case Reservado:
                printf("Reservado\n");
                break;
            default:
                printf("Desconhecido\n");
        }

        // Menu para escolher o novo status
        printf("\nEscolha o novo status:\n");
        printf("1 - Livre\n");
        printf("2 - Ocupado\n");
        printf("3 - Reservado\n");

        int opcaoStatus;
        scanf("%d", &opcaoStatus);

        switch (opcaoStatus) {
            case 1:
                quarto.status = Livre;
                break;
            case 2:
                quarto.status = Ocupado;
                break;
            case 3:
                quarto.status = Reservado;
                break;
            default:
                printf("Opção inválida. O status permanecerá inalterado.\n");
        }

        // Atualizar o registro no arquivo
        fseek(file, -sizeof(Quarto), SEEK_CUR);  // Volta para a posição antes da leitura do último registro
        fwrite(&quarto, sizeof(Quarto), 1, file);
        printf("Status atualizado com sucesso.\n");
    } else {
        printf("Quarto não encontrado.\n");
    }
}
    
 
int main() {
    FILE *file = fopen("quarto.bin", "ab+");  // Abre ou cria o arquivo para leitura e escrita binária

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

