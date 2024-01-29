#include "enum.h"

double calcularDiferenca(int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut);


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
    
    printf("hora de entrada:");
    scanf("%d", &nova.horaIn);
    printf("minuto de entrada:");
    scanf("%d", &nova.minutoIn);
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


double calcularValorTotal(int numero, int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut){
    Quarto quarto;
    FILE *file = fopen("quarto.bin", "rb");
    double total;
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
    FILE *fileQuarto = fopen("quarto.bin", "r+b");
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
    FILE *fileQuarto = fopen("quarto.bin", "r+b");
    int numero;
    printf("digite o numero do quarto para realizar o checkOut: ");
    scanf("%d", &numero);
    int encontrado = 0;
    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numero) {
            encontrado = 1;
            mudarStatusQuarto(fileQuarto, reserva.numeroQuarto, Livre);
            RegistraHoraSaida(file, reserva.numeroQuarto);
            printf("Check-out realizado com sucesso.\n");
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
    fclose(file);
    fopen("reserva.bin", "r+b");
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
            printf("Pagamento  no valor de %.2f realizado com sucesso.\n", reserva.valorTotal);
            fseek(file, -sizeof(Reserva), SEEK_CUR);
            fwrite(&reserva, sizeof(Reserva), 1, file);
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
        fclose(file);
        file = fopen("reserva.bin", "ab+");
}

void mudarStatusQuarto(FILE *file, int numeroQuarto, enum Status status) {
    Quarto quarto;
    int encontrado = 0;

    rewind(file);

    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        int indice=0;
        if (quarto.numero == numeroQuarto) {
            encontrado = 1;
            quarto.status = status;
            fseek(file, indice * sizeof(Quarto), SEEK_SET);
            fwrite(&quarto, sizeof(Quarto), 1, file);
            break;
        }
        indice++;
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
        printf("data de entrada: %d/%d/%d\n as %d:%d", reserva.diaIn,reserva.mesIn, reserva.anoIn, reserva.horaIn, reserva.minutoIn);
        if (reserva.diaOut!=0)
        {
        printf("data de saida: %d/%d/%d as %d:%d\n", reserva.diaOut,reserva.mesOut, reserva.anoOut, reserva.horaOut, reserva.horaOut);
        }
        
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

double calcularDiferenca(int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut) {
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
   double dias = diferenca / 86400;
   
   return dias;
}


int menuReserva() {
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
        printf("7 - Realizar Check-Out\n");
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
            case 7:{
                realizarCheckOut(file);
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
    int i = 0;
    rewind(file);  // Posiciona o ponteiro no início do arquivo

    printf("Lista de Quartos:\n");

    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        printf("[%d] - Indice\n", i);
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
            i++;
        printf("\n");
    }
}
int apagarQuartoPorNumero(FILE *file, int numero) {
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
    int indice;
    fclose(file);
    file = fopen("quarto.bin", "r+b");
    listarQuartos(file);

    printf("Digite o indice do quarto que deseja atualizar: ");
    scanf("%d", &indice);

    Quarto quarto;

    // Posiciona o ponteiro de arquivo no registro desejado
    fseek(file, indice * sizeof(Quarto), SEEK_SET);
    
    // Lê as informações do quarto do arquivo
    fread(&quarto, sizeof(Quarto), 1, file);

    printf("Número: %d\n", quarto.numero);
    printf("Preço: %.2f\n", quarto.preco);

    // Restante da impressão ...

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

    // Posiciona o ponteiro de arquivo no início do registro a ser atualizado
    fseek(file, indice * sizeof(Quarto), SEEK_SET);

    // Escreve as informações atualizadas de volta no arquivo
    fwrite(&quarto, sizeof(Quarto), 1, file);
    fclose(file);
    file = fopen("quarto.bin", "ab+");
    printf("Status atualizado com sucesso.\n");
}

    
 
int menuQuarto() {
    FILE *file = fopen("quarto.bin", "a+b");  // Abre ou cria o arquivo para leitura e escrita binária
    
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

    fclose(file); 

    return 0;
}



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
    //fread(&aux, sizeof(Cliente), 1, file);   
    int encontrado = 0;
    
             encontrado = 1; 

    if (encontrado) {
        // printf("Cliente encontrado:\n");
        // printf("Nome: %s\n", aux.nome);
        // printf("CPF: %s\n", aux.cpf);
        // printf("RG: %s\n", aux.rg);
        // printf("Telefone: %s\n", aux.telefone);
        // printf("Endereço: %s\n", aux.endereco);
        // printf("E-mail: %s\n", aux.email);
         Cliente cliente;
        // Menu para escolher o que deseja atualizar4
        printf("\n modifique o usuario:\n");
        
        
                printf("Digite o novo nome: \n");
                scanf("%s", cliente.nome);
         
                printf("digite o novo Cpf: \n");
                scanf("%s", cliente.cpf);

                printf("Digite o novo RG: \n");
                scanf("%s", cliente.rg);
         
                printf("Digite o novo telefone: \n");
                scanf("%s", cliente.telefone);
         
                printf("Digite o novo endereço: \n");
                scanf("%s", cliente.endereco);
         
                printf("Digite o novo e-mail: \n");
                scanf("%s", cliente.email);
                
         
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

int menuCliente() {
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

int main() {
    

    int opcao;

    do {
        printf("Menu:\n");
        printf("1 - Opcoes de Quarto\n");
        printf("2 - LOpcoes do Cliente\n");
        printf("3 - Opcoes de reserva\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: 
               menuQuarto();
               break;
            case 2:
               menuCliente();
                break;
            case 3: 
                menuReserva();
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);
 
    return 0;
}