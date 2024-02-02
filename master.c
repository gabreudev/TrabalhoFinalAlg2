#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////
enum TipoQuarto {
    Simples,
    Duplo,
    Suite
};

enum Status {
    Livre,
    Ocupado,
    Reservado
};

enum StatusPagamento {
    Pendente,
    Pago
};

typedef struct {
    int numero;
    float preco;
    enum TipoQuarto tipoQuarto;
    enum Status status;
} Quarto;

typedef struct {
    char nome[30];
    char cpf[12];
    char rg[20];
    char telefone[15];
    char endereco[15];
    char email[20];
} Cliente;

typedef struct{
    int codigo;
    int numeroQuarto;
    char cpfCliente[20];
    int horaIn;
    int minutoIn;
    int diaIn;
    int mesIn;
    int anoIn;
    int horaOut;
    int minutoOut;
    int diaOut;
    int mesOut;
    int anoOut;
    double valorTotal;
    enum StatusPagamento statusPagamento;
} Reserva;
//////////////
void atualizarCliente(FILE *file);

int apagarClientePorCpf(FILE *file, char cpf[]);

Cliente obterNovoCliente();

void cadastrarCliente(FILE *file, Cliente cliente);

void listarClientes(FILE *file);

int menuCliente();

////////////////////

double calcularDiferenca(int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut);
int verificarNumero(FILE *file, int numero);
int verificarCpf(FILE *file, const char *cpf);
void realizarReserva(FILE *file, Reserva reserva);
void obterNovaReserva(FILE *file);
double calcularValorTotal(int numero, int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut);
void realizarCheckIn(FILE *file);
void realizarCheckOut(FILE *file);
void realizarPagamento(FILE *file, int numeroQuarto);
void RegistraHoraSaida(FILE *file, int numeroQuarto);
void consultarReservas(FILE *file);
double calcularValoresRecebidos(FILE *file);
void menuReserva();
/////////////////////


Quarto obterNovoQuarto();

void cadastrarQuarto(FILE *file, Quarto quarto);

void listarQuartos(FILE *file);

int apagarQuartoPorNumero(FILE *file, int numero);

void atualizarStatusQuarto(FILE *file);

int menuQuarto();

double calcularDiferenca(int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut);

int gerarCodigo() {

    srand(time(NULL));

    int codigo = rand() % 90000 + 10000;  

    return codigo;
}

int verificarNumero(FILE *file, int numero) {
    Quarto quarto;
    rewind(file);

    while (fread(&quarto, sizeof(Quarto), 1, file) == 1) {
        if(quarto.numero == numero) {
            printf("O número do Quarto informado consta no nosso registro de Quartos!\n");
            
            
            return 1;  
        }
    }
    printf("nenhum quarto está cadastrado com o numeor %d em nosso sistema. Tente novamente.\n", numero);
    return 0;  
}

int verificarCpf(FILE *file, const char *cpf) {
    Cliente cliente;
    rewind(file);

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            printf("O CPF informado consta no nosso registro de clientes e está no nome de %s!\n", cliente.nome);
            return 1; 
        }
    }

    return 0; 
}

void realizarReserva(FILE *file, Reserva reserva) {
    fwrite(&reserva, sizeof(Reserva), 1, file);
    printf("Reserva realizada com sucesso.\n");
    //main();
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
    printf("hora de entrada:");
    scanf("%d", &nova.horaIn);
    printf("minuto de entrada:");
    scanf("%d", &nova.minutoIn);
    printf("dia previsto de saida:");
    scanf("%d", &nova.diaOut);
    printf("mes previsto de saida:");
    scanf("%d", &nova.mesOut);
    printf("ano previsto de saida:");
    scanf("%d", &nova.anoOut);
    printf("hora prevista de saida:");
    scanf("%d", &nova.horaOut);
    printf("hora prevista de saida:");
    scanf("%d", &nova.minutoOut);
    
    nova.codigo= gerarCodigo();

    nova.statusPagamento = Pendente;
    nova.valorTotal = 0.0;
    mudarStatusQuart(fileQuarto, nova.numeroQuarto, 2);
    realizarReserva(file, nova);

    fclose(fileCliente);
    fclose(fileQuarto);
    fclose(file);
    main();
}
double calcularValorTotal(int numero, int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut) {
    FILE *file = fopen("quarto.bin", "rb");
    Quarto quarto;

    double total = 0.0;
    double precoDiaria = 0.0;

    rewind(file);

    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        if (quarto.numero == numero) {
            precoDiaria = quarto.preco;
            break;
        }
    }

    fclose(file);

    if (precoDiaria == 0.0) {
        printf("Quarto não encontrado.\n");
        return total;
    }

    double diferenca = //calcularDiferenca(diaIn, mesIn, anoIn, diaOut, mesOut, anoOut);
    total = precoDiaria * diferenca;

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
            mudarStatusQuart(fileQuarto, reserva.numeroQuarto, 1);
            
            printf("Check-in realizado com sucesso.\n agora o quarto esta oficialmente ocupado\n");
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

    int numero;
    listarQuartos;
    printf("digite o numero do quarto para realizar o check-out: ");
    scanf("%d", &numero);
    int encontrado = 0;
    FILE *fileQuarto = fopen("quarto.bin", "r+b");
    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.numeroQuarto == numero) {
            encontrado = 1;
            realizarPagamento(file, reserva.numeroQuarto);
            mudarStatusQuart(fileQuarto, reserva.numeroQuarto, 1);
            printf("Checkout realizado com sucesso. O quarto agora esta Livre\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva não encontrada.\n");
    }
    fclose(fileQuarto);
   // main();
}
void realizarPagamento(FILE *file, int numeroQuarto) {
    Reserva reserva;
    fclose(file);
    
    FILE *fileQuarto = fopen("quarto.bin", "r+b");
    file = fopen("reserva.bin", "r+b");
    int encontrado = 0;
    int codigo;
    
    consultarReservas(file);
    printf("\ndigite o codigo da reserva: ");
    scanf("%d", &codigo);
    
    
    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.codigo == codigo) {
            encontrado = 1;
            mudarStatusQuart(fileQuarto, reserva.numeroQuarto, 0);
            reserva.statusPagamento = Pago;
            reserva.valorTotal = calcularValorTotal(reserva.numeroQuarto, reserva.diaIn, reserva.mesIn, reserva.anoIn, reserva.diaOut, reserva.mesOut, reserva.anoOut);
            printf("Pagamento  no valor de %.2f realizado com sucesso.\n", reserva.valorTotal);
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
void mudarStatusQuart(FILE *file, int numero, int novoStatus) {
    rewind(file);

    Quarto quarto;
    int encontrado = 0;

    while (fread(&quarto, sizeof(Quarto), 1, file) == 1) {
        if (quarto.numero == numero) {
            encontrado = 1;
            quarto.status = novoStatus;

            fseek(file, -sizeof(Quarto), SEEK_CUR);
            fwrite(&quarto, sizeof(Quarto), 1, file);
            printf("Status atualizado com sucesso.\n");

            break;
        }
    }

    if (!encontrado) {
        printf("Quarto não encontrado.\n");
    }
}

void RegistraHoraSaida(FILE *file, int numeroQuarto) {
    Reserva reserva;
    int encontrado = 0;

    rewind(file);

    while (fread(&reserva, sizeof(Quarto), 1, file)) {
        if (reserva.numeroQuarto == numeroQuarto) {
            encontrado = 1;
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
        printf("codigo: %d\n", reserva.codigo);
        printf("Número do Quarto: %d\n", reserva.numeroQuarto);
        printf("CPF do Cliente: %s\n", reserva.cpfCliente);
        printf("data de entrada: %d/%d/%d\n as %d:%d\n", reserva.diaIn,reserva.mesIn, reserva.anoIn, reserva.horaIn, reserva.minutoIn);
        printf("data de saida: %d/%d/%d as %d:%d\n", reserva.diaOut,reserva.mesOut, reserva.anoOut, reserva.horaOut, reserva.horaOut);
        
        
        printf("Status do Pagamento: ");

    if (reserva.statusPagamento == Pendente) {
        printf("Pendente\n");
    } else {
        printf("Pago\n");
    }
        printf("\n");
    }
}

double calcularValoresRecebidos(FILE *file) {
    Reserva reserva;
    double valoresRecebidos = 0.0;
    fseek(file, sizeof(Reserva), SEEK_SET);
    rewind(file);

    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (reserva.statusPagamento == Pago) {
            valoresRecebidos += reserva.valorTotal;
        }
    }

    return valoresRecebidos;
}


double calcularDiferenca(int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut) {
   struct tm datain = {0};
   struct tm dataOut = {0}; 
   
   datain.tm_year =  anoIn - 1900; 
   datain.tm_mon = mesIn; 
   datain.tm_mday = diaIn; 
   dataOut.tm_year = anoOut - 1900;
   dataOut.tm_mon = mesOut;
   dataOut.tm_mday = diaOut;
   
   time_t segundos1 = mktime(&datain);
   time_t segundos2 = mktime(&dataOut);
   
   double diferenca = difftime(segundos2, segundos1);
   
   double dias = diferenca / 86400;
   
   return dias;
}
int excluirReserva(FILE *file, int codigo) {
    fclose(file);
    FILE *File = fopen("reserva.bin", "r+b");
    FILE *tempFile = fopen("temp.bin", "wb");

    if (!tempFile) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return 0;
    }

    Reserva reserva;

    rewind(file);

    int encontrado = 0;

    while (fread(&reserva, sizeof(reserva), 1, file)) {
        if (reserva.codigo == codigo) {
            printf("Reserva de código %d encontrada e apagada.\n", reserva.codigo);
            encontrado = 1;
        } else {
            printf("Copiando a reserva de código %d\n", reserva.codigo);
            fwrite(&reserva, sizeof(Reserva), 1, tempFile);
        }
    }

    fclose(File);
    fclose(tempFile);

    remove("reserva.bin");
    rename("temp.bin", "reserva.bin");

    return encontrado;
}

void menuReserva() {
    FILE *file = fopen("reserva.bin", "ab+");

    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;

    do {
        printf("=-=-=Menu de Reservas=-=-=\n");
        printf("1 - Realizar Reserva\n");
        printf("2 - Excluir Reserva\n");
        printf("3 - Realizar Check-In\n");
        printf("4 - Consultar Reservas\n");  
        printf("5 - Consultar Valores Recebidos\n");
        printf("6 - Realizar Check-Out\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                obterNovaReserva(file);
                break;
            }
            case 2: {
                int codigo;
                printf("Digite o código da reserva a ser excluída: ");
                scanf("%d", &codigo);
                if (excluirReserva(file, codigo)) {
                    printf("Reserva excluída com sucesso.\n");
                    main();
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
                consultarReservas(file);
                break;
            }
            case 5: {
                double valoresRecebidos = calcularValoresRecebidos(file);
                printf("Valores Recebidos: %.2f\n", valoresRecebidos);
                break;
            }
            case 6: {
                realizarCheckOut(file);
                break;
            }
            case 0:
                printf("Voltando..\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    fclose(file);

    main();
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
            novo.tipoQuarto = -1; 

    }

    return novo;
}

void cadastrarQuarto(FILE *file, Quarto quarto) {
    fwrite(&quarto, sizeof(Quarto), 1, file);
}

void listarQuartos(FILE *file) {
    Quarto quarto;
    int i = 0;
    rewind(file);  

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
    fclose(file);
    FILE *File = fopen("quarto.bin", "r+b");
    FILE *tempFile = fopen("temp.bin", "wb");

    if (!tempFile) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return 0;
    }

     Quarto quarto;
    

    rewind(file);

    int encontrado = 0;
    int quant=0;
    while (fread(&quarto, sizeof(Quarto), 1, file)) {
    

    if (quarto.numero == numero) {
        printf("Quarto %d encontrado e apagado.\n", quarto.numero);
        encontrado = 1;
    } else {
        printf("copiando o quarto %d\n", quarto.numero);
        fwrite(&quarto, sizeof(Quarto), 1, tempFile);
    }

    }


    fclose(File);
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

    
    fseek(file, indice * sizeof(Quarto), SEEK_SET);
    
    fread(&quarto, sizeof(Quarto), 1, file);

    printf("Número: %d\n", quarto.numero);
    printf("Preço: %.2f\n", quarto.preco);


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

    fseek(file, indice * sizeof(Quarto), SEEK_SET);

    fwrite(&quarto, sizeof(Quarto), 1, file);
    fclose(file);
    file = fopen("quarto.bin", "ab+");
    printf("Status atualizado com sucesso.\n");
}

    
 
int menuQuarto() {
    FILE *file = fopen("quarto.bin", "a+b");  
    
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;

    do {
        printf("=-=-=Menu de Quarto=-=-=\n");
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
                    main();
                } else {
                    printf("Quarto não encontrado.\n");
                }
                break;
            }
            case 4:
                atualizarStatusQuarto(file);
                break;
            case 0:
                printf("Voltando..\n");
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
    getchar(); 
    fgets(novo.endereco, sizeof(novo.endereco), stdin);
    novo.endereco[strcspn(novo.endereco, "\n")] = '\0'; 

    printf("Digite o e-mail do cliente: ");
    scanf("%s", novo.email);

    printf("Digite o nome do cliente: ");
    getchar(); 
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; 

    return novo;
}


void cadastrarCliente(FILE *file, Cliente cliente) {
    fwrite(&cliente, sizeof(Cliente), 1, file);
}

void listarClientes(FILE *file) {
    Cliente cliente;
    int indice=0;
    rewind(file);  

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
    fclose(file);
FILE *File = fopen("cliente.bin", "r+b");
FILE *tempFile = fopen("temp.bin", "wb");

if (!tempFile) {
    printf("Erro ao abrir o arquivo temporário.\n");
    return 0;
}

Cliente cliente;

rewind(File); 

int encontrado = 0;

while (fread(&cliente, sizeof(cliente), 1, File)) {

    if (strcmp(cliente.cpf, cpf) == 0) {
        printf("Usuário do CPF %s encontrado e apagado.\n", cliente.cpf);
        encontrado = 1;
    } else {
        printf("Copiando o cliente %s\n", cliente.nome);
        fwrite(&cliente, sizeof(Cliente), 1, tempFile);
    }

}

fclose(File);
fclose(tempFile);

remove("cliente.bin");  
rename("temp.bin", "cliente.bin");

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
    int encontrado = 0;
             encontrado = 1; 
    if (encontrado) {
         Cliente cliente;
        printf("\n modifique o usuario:\n");
        
        printf("Digite o CPF do cliente: ");
        scanf("%s", cliente.cpf);

        printf("Digite o RG do cliente: ");
        scanf("%s", cliente.rg);

        printf("Digite o novo telefone do cliente: ");
        scanf("%s", cliente.telefone);

        printf("Digite o novo endereço do cliente: ");
        getchar(); 
        fgets(cliente.endereco, sizeof(cliente.endereco), stdin);
        cliente.endereco[strcspn(cliente.endereco, "\n")] = '\0'; 

        printf("Digite o novo e-mail do cliente: ");
        scanf("%s", cliente.email);

        printf("Digite o novo nome do cliente: ");
        getchar(); 
        fgets(cliente.nome, sizeof(cliente.nome), stdin);
        cliente.nome[strcspn(cliente.nome, "\n")] = '\0'; 

        fwrite(&cliente, sizeof(Cliente), 1, file);
        printf("Informação atualizada com sucesso.\n");

        fclose(file);
        file = fopen("cliente.bin", "ab+");
    
    } else {
        printf("Cliente não encontrado.\n");
    }
}

int menuCliente() {
    FILE *file = fopen("cliente.bin", "ab+");  
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        
    }

    int opcao;

    do {
        printf("=-=-=Menu Cliente=-=-=\n");
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
                main();
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
                    main();
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            }
            case 4:
                atualizarCliente(file);
                break;
            case 0:
                printf("Voltando.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    fclose(file);  
    return 0;
    
}

int main() {
    

    int opcao;

    do {
        printf("=-=-=Menu principal=-=-=\n");
        printf("1 - Opcoes de Quarto\n");
        printf("2 - Opcoes do Cliente\n");
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