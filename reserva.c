#include <stdio.h>
#include <stdlib.h>
#include "enum.h"

int verificarNumero(FILE *file, int numero) {
    Quarto quarto;
    rewind(file);

    while (fread(&quarto, sizeof(Quarto), 1, file) == 1) {
        if(quarto.numero == numero) {
            printf("O número do Quarto informado consta no nosso registro de Quartos!\n");
            if (quarto.status==Ocupado)
            {
                printf("o quarto escolhido está indisponivel no momento!");
                return 0;
            }
            
            return 1;  // Número de quarto encontrado
        }
    }
    printf("nenhum quarto está cadastrado com o numeor %d em nosso sistema. Tente novamente.\n", numero);
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

    printf("digite a data de entrada no quarto: \n");
    printf("dia de entrada:");
    scanf("%d", &nova.diaIn);
    printf("mes de entrada:");
    scanf("%d", &nova.mesIn);
    printf("ano de entrada:");
    scanf("%d", &nova.anoIn);
    printf("dia de saida:");
    scanf("%d", &nova.diaOut);
    printf("mes de saida:");
    scanf("%d", &nova.mesOut);
    printf("ano de saida:");
    scanf("%d", &nova.anoOut);
    printf("hora de entrada:");
    scanf("%d", &nova.horaIn);
    printf("minuto de entrada:");
    scanf("%d", &nova.anoOut);
    nova.minutoIn = 0;
    nova.horaOut = 0;
    nova.minutoOut = 0;

    // Status e valor padrão
    nova.statusPagamento = Pendente;
    nova.valorTotal = 0.0;
    mudarStatusQuarto(fileQuarto, nova.numeroQuarto, Reservado);
    realizarReserva(file, nova);

    fclose(fileCliente);
    fclose(fileQuarto);
    fclose(file);
}

int excluirReserva(FILE *file, int numeroQuarto) {
    FILE *tempFile = fopen("temp.bin", "wb");  // Corrigido para "temp.bin"
    char cpf[20];
    printf("Digite o número do CPF do cliente: ");
    scanf("%s", cpf);
    if (!tempFile) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return 0;
    }

    Reserva reserva;

    rewind(file);

    int encontrado = 0;

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numeroQuarto && strcmp(reserva.cpfCliente, cpf)==0) {
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


float calcularValorTotal(int numero, int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut){
    Quarto quarto;
    FILE *file = fopen("quarto.bin", "rb");
    float total;
    int encontrado = 0;
    rewind(file);
    total = quarto.preco * calcularDiferenca(diaIn, mesIn, anoIn, diaOut, mesOut, anoOut);
    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        if (quarto.numero == numero) {
            encontrado = 1;
            
            printf("Check-in realizado com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
    return total;
}
void realizarCheckIn(FILE *file) {
    Reserva reserva;
    int numero;
    printf("digite o numero do quarto para realizar o checkIn: ");
    scanf("%d", &numero);
    int encontrado = 0;
    FILE *fileQuarto = fopen("quarto.bin", "rb");
    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numero) {
            encontrado = 1;
            mudarStatusQuarto(fileQuarto, reserva.numeroQuarto, Ocupado);
            // Lógica para realizar o check-in, se necessário
            printf("Check-in realizado com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
    fclose(fileQuarto);
}
void realizarCheckOut(FILE *file) {
    Reserva reserva;
    FILE *fileQuarto = fopen("quarto.bin", "wb");
    int numero;
    printf("digite o numero do quarto para realizar o checkOut: ");
    int encontrado = 0;
    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numero) {
            encontrado = 1;
            mudarStatusQuarto(fileQuarto, reserva.numeroQuarto, Livre);
            RegistraHoraSaida(file, reserva.numeroQuarto);
            printf("Check-in realizado com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
    fclose(fileQuarto);
}
void realizarPagamento(FILE *file, int numeroQuarto) {
    Reserva reserva;
    int encontrado = 0;
    char cpf[20];
    do
    {
    printf("digite o cpf do cliente: ");
    scanf("%s", &cpf); 
    } while (!verificarCpf);
    
    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numeroQuarto && strcmp(reserva.cpfCliente, cpf) == 0) {
            encontrado = 1;
            reserva.statusPagamento = Pago;
            reserva.valorTotal = calcularValorTotal(reserva.numeroQuarto, reserva.diaIn, reserva.mesIn, reserva.anoIn, reserva.diaOut, reserva.mesOut, reserva.anoOut);
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

void mudarStatusQuarto(FILE *file, int numeroQuarto, enum Status Status) {
    Quarto quarto;
    int encontrado = 0;

    rewind(file);

    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        if (quarto.numero == numeroQuarto) {
            encontrado = 1;
            // Lógica para realizar o pagamento, se necessário
            quarto.status = Status;
            fseek(file, -sizeof(Quarto), SEEK_CUR);
            fwrite(&quarto, sizeof(Quarto), 1, file);
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
}
void RegistraHoraSaida(FILE *file, int numeroQuarto) {
    Reserva reserva;
    int encontrado = 0;

    rewind(file);

    while (fread(&reserva, sizeof(Quarto), 1, file)) {
        if (reserva.numeroQuarto == numeroQuarto) {
            encontrado = 1;
            // Lógica para realizar o pagamento, se necessário
            printf("digite a hora de saida: ");
            scanf("%d", &reserva.horaOut);
            printf("digite o minuto de saida: ");
            scanf("%d", &reserva.minutoOut);
            fseek(file, -sizeof(Quarto), SEEK_CUR);
            fwrite(&reserva, sizeof(Quarto), 1, file);
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
#include <stdio.h>
#include <time.h>

int calcularDiferenca(int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut) {
   struct tm datain = {0}; // data inicial
   struct tm dataOut = {0}; // data final
   
   // preenche as datas
   datain.tm_year =  anoIn - 1900; // ano - 1900
   datain.tm_mon = mesIn; // mês (0-11)
   datain.tm_mday = diaIn; // dia
   dataOut.tm_year = anoOut - 1900;
   dataOut.tm_mon = mesOut;
   dataOut.tm_mday = diaOut;
   
   // converte as datas para segundos
   time_t segundos1 = mktime(&datain);
   time_t segundos2 = mktime(&dataOut);
   
   // calcula a diferença em segundos
   double diferenca = difftime(segundos2, segundos1);
   
   // converte a diferença em dias
   int dias = diferenca / 86400;
   
   return dias;
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
                realizarCheckIn(file);
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