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
void mudarStatusQuarto(FILE *file, int numeroQuarto, enum Status status);
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


