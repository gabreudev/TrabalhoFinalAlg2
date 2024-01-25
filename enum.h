#include <stdio.h>

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
    float valorTotal;
    enum StatusPagamento statusPagamento;
} Reserva;