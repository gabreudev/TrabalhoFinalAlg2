#include <stdio.h>
#include <stdlib.h>
#include "enum.h"

int verificarNumero(FILE *file, int numero) {
    Quarto quarto;
    rewind(file);

    while (fread(&quarto, sizeof(Quarto), 1, file) == 1) {
        if(quarto.numero == numero) {
            printf("O número do Quarto informado consta no nosso registro de Quartos!\n");
            return 1;  // Número de quarto encontrado
        }
    }

    return 0;  // Número de quarto não encontrado
}

int verificarCpf(FILE *file, const char *cpf) {
    Cliente cliente;
    rewind(file);

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            printf("O CPF informado consta no nosso registro de clientes e está no nome de %s!\n", cliente.nome);
            return 1;  // CPF encontrado
        }
    }

    return 0;  // CPF não encontrado
}

void realizarReserva(FILE *file, Reserva reserva) {
    fwrite(&reserva, sizeof(Reserva), 1, file);
    printf("Reserva realizada com sucesso.\n");
}

void obterNovaReserva(FILE *file) {
    Reserva nova;
    FILE *fileCliente = fopen("cliente.bin", "rb");
    FILE *fileQuarto = fopen("quarto.bin", "rb");

    if (!fileCliente || !fileQuarto) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    do {
        printf("Digite o número do quarto para a reserva: ");
        scanf("%d", &nova.numeroQuarto);
    } while (!verificarNumero(fileQuarto, nova.numeroQuarto));

    do {
        printf("Digite o CPF do cliente: ");
        scanf("%s", nova.cpfCliente);
    } while (!verificarCpf(fileCliente, nova.cpfCliente));

    // Informações padrão
    nova.horaIn = 0;
    nova.minutoIn = 0;
    nova.diaIn = 0;
    nova.mesIn = 0;
    nova.anoIn = 0;
    nova.horaOut = 0;
    nova.minutoOut = 0;
    nova.diaOut = 0;
    nova.mesOut = 0;
    nova.anoOut = 0;

    // Status e valor padrão
    nova.statusPagamento = Pendente;
    nova.valorTotal = 0.0;

    realizarReserva(file, nova);

    fclose(fileCliente);
    fclose(fileQuarto);
}


int excluirReserva(FILE *file, int numeroQuarto) {
    FILE *tempFile = fopen("temp.dat", "wb");

    if (!tempFile) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return 0;
    }

    Reserva reserva;

    rewind(file);

    int encontrado = 0;

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numeroQuarto) {
            printf("Reserva excluída com sucesso.\n");
            encontrado = 1;
        } else {
            fwrite(&reserva, sizeof(Reserva), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("reserva.bin");
    rename("temp.bin", "reserva.bin");

    return encontrado;
}

void realizarCheckIn(FILE *file, int numeroQuarto) {
    Reserva reserva;
    int encontrado = 0;

    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numeroQuarto) {
            encontrado = 1;
            // Lógica para realizar o check-in, se necessário
            printf("Check-in realizado com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
}

void realizarPagamento(FILE *file, int numeroQuarto) {
    Reserva reserva;
    int encontrado = 0;

    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numeroQuarto) {
            encontrado = 1;
            // Lógica para realizar o pagamento, se necessário
            reserva.statusPagamento = Pago;
            printf("Pagamento realizado com sucesso.\n");
            fseek(file, -sizeof(Reserva), SEEK_CUR);
            fwrite(&reserva, sizeof(Reserva), 1, file);
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
}

void consultarReservas(FILE *file) {
    Reserva reserva;

    rewind(file);

    printf("Lista de Reservas:\n");

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        printf("Número do Quarto: %d\n", reserva.numeroQuarto);
        printf("CPF do Cliente: %s\n", reserva.cpfCliente);
        // Exibir outras informações conforme necessário
        printf("Status do Pagamento: ");
    if (reserva.statusPagamento == Pendente) {
        printf("Pendente\n");
    } else {
        printf("Pago\n");
    }
        printf("\n");
    }
}

float calcularValoresRecebidos(FILE *file) {
    Reserva reserva;
    float valoresRecebidos = 0.0;

    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.statusPagamento == Pago) {
            valoresRecebidos += reserva.valorTotal;
        }
    }

    return valoresRecebidos;
}

int main() {
    FILE *file = fopen("reserva.bin", "ab+");

    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;

    do {
        printf("Menu:\n");
        printf("1 - Realizar Reserva\n");
        printf("2 - Excluir Reserva\n");
        printf("3 - Realizar Check-In\n");
        printf("4 - Realizar Pagamento\n");
        printf("5 - Consultar Reservas\n");
        printf("6 - Consultar Valores Recebidos\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Lógica para preencher os detalhes da reserva
                obterNovaReserva(file);
                break;
            }
            case 2: {
                int numeroQuarto;
                printf("Digite o número do quarto da reserva a ser excluída: ");
                scanf("%d", &numeroQuarto);
                if (excluirReserva(file, numeroQuarto)) {
                    printf("Reserva excluída com sucesso.\n");
                } else {
                    printf("Reserva não encontrada.\n");
                }
                break;
            }
            case 3: {
                int numeroQuartoCheckIn;
                printf("Digite o número do quarto para realizar o check-in: ");
                scanf("%d", &numeroQuartoCheckIn);
                realizarCheckIn(file, numeroQuartoCheckIn);
                break;
            }
            case 4: {
                int numeroQuartoPagamento;
                printf("Digite o número do quarto para realizar o pagamento: ");
                scanf("%d", &numeroQuartoPagamento);
                realizarPagamento(file, numeroQuartoPagamento);
                break;
            }
            case 5:
                consultarReservas(file);
                break;
            case 6: {
                float valoresRecebidos = calcularValoresRecebidos(file);
                printf("Valores Recebidos: %.2f\n", valoresRecebidos);
                break;
            }
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    fclose(file);

    return 0;
}