#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

Cliente obterNovoCliente() {
    Cliente novo;

    printf("Digite o CPF do cliente: ");
    scanf("%s", novo.cpf);

    printf("Digite o RG do cliente: ");
    scanf("%s", novo.rg);

    printf("Digite o telefone do cliente: ");
    scanf("%s", novo.telefone);

    printf("Digite o endereço do cliente: ");
    scanf("%s", novo.endereco);

    printf("Digite o e-mail do cliente: ");
    scanf("%s", novo.email);

    printf("Digite o nome do cliente: ");
    scanf("%s", novo.nome);
    // Lê a linha inteira, incluindo espaços
    return novo;
}


void cadastrarCliente(FILE *file, Cliente cliente) {
    fwrite(&cliente, sizeof(Cliente), 1, file);
}

void listarClientes(FILE *file) {
    Cliente cliente;
    int indice=0;
    rewind(file);  // Posiciona o ponteiro no início do arquivo

    printf("Lista de Clientes:\n");

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        printf("indice: [%d]\n", indice);
        printf("Nome: %s\n", cliente.nome);
        printf("CPF: %s\n", cliente.cpf);
        printf("RG: %s\n", cliente.rg);
        printf("Telefone: %s\n", cliente.telefone);
        printf("Endereço: %s\n", cliente.endereco);
        printf("E-mail: %s\n", cliente.email);
        printf("\n");
        indice++;
    }
}

int apagarClientePorCpf(FILE *file, char cpf[]) {
    FILE *tempFile = fopen("temp.bin", "wb");  // Arquivo temporário para reescrever os clientes excluindo o desejado

    if (!tempFile) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return 0;
    }

    Cliente cliente;

    rewind(file);  // Posiciona o ponteiro no início do arquivo

    int encontrado = 0;

    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            printf("Cliente encontrado e apagado.\n");
            encontrado = 1;
        } else {
            fwrite(&cliente, sizeof(Cliente), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("cliente.bin");       // Remove o arquivo original
    rename("temp.bin", "cliente.bin");  // Renomeia o arquivo temporário para o original

    return encontrado;
}

void atualizarCliente(FILE *file) {
    
    listarClientes(file);
    fclose(file);
    file = fopen("cliente.bin", "r+b");
    char cpf[15];
    
    int indice;
    printf("Digite o indice do cliente que deseja atualizar: ");
    scanf("%d", &indice);
    fseek(file, indice*sizeof(Cliente), SEEK_SET);
    Cliente cliente;
    int encontrado = 0;
    
             encontrado = 1;
       

    if (encontrado) {
        printf("Cliente encontrado:\n");
        printf("Nome: %s\n", cliente.nome);
        printf("CPF: %s\n", cliente.cpf);
        printf("RG: %s\n", cliente.rg);
        printf("Telefone: %s\n", cliente.telefone);
        printf("Endereço: %s\n", cliente.endereco);
        printf("E-mail: %s\n", cliente.email);

        // Menu para escolher o que deseja atualizar
        printf("\nEscolha o que deseja atualizar:\n");
        printf("1 - Nome\n");
        printf("2 - RG\n");
        printf("3 - Telefone\n");
        printf("4 - Endereço\n");
        printf("5 - E-mail\n");

        int opcaoAtualizacao;
        scanf("%d", &opcaoAtualizacao);

        switch (opcaoAtualizacao) {
            case 1:
                printf("Digite o novo nome: ");
                scanf("%s", cliente.nome);
                break;
            case 2:
                printf("Digite o novo RG: ");
                scanf("%s", cliente.rg);
                break;
            case 3:
                printf("Digite o novo telefone: ");
                scanf("%s", cliente.telefone);
                break;
            case 4:
                printf("Digite o novo endereço: ");
                scanf("%s", cliente.endereco);
                break;
            case 5:
                printf("Digite o novo e-mail: ");
                scanf("%s", cliente.email);
                break;
            default:
                printf("Opção inválida. Nenhuma alteração realizada.\n");
        }

        // Atualizar o registro no arquivo
          // Volta para a posição antes da leitura do último registro
        fwrite(&cliente, sizeof(Cliente), 1, file);
        printf("Informação atualizada com sucesso.\n");

        fclose(file);
        file = fopen("cliente.bin", "ab+");
    
    } else {
        printf("Cliente não encontrado.\n");
    }
}

int main() {
    FILE *file = fopen("cliente.bin", "ab+");  // Abre ou cria o arquivo para leitura e escrita binária

    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        
    }

    int opcao;

    do {
        printf("Menu:\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Listar Clientes\n");
        printf("3 - Apagar Cliente por CPF\n");
        printf("4 - Atualizar Informações do Cliente\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Cliente novo = obterNovoCliente();
                cadastrarCliente(file, novo);
                break;
            }
            case 2:
                listarClientes(file);
                break;
            case 3: {
                char cpf[15];
                printf("Digite o CPF do cliente que deseja apagar: ");
                scanf("%s", cpf);

                if (apagarClientePorCpf(file, cpf)) {
                    printf("Cliente apagado com sucesso.\n");
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            }
            case 4:
                atualizarCliente(file);
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
