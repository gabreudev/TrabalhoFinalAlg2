#include <stdio.h>
#include "enum.h"

double calcularDiferenca(int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut);
int verificarNumero(FILE *file, int numero);
int verificarCpf(FILE *file, const char *cpf);
void realizarReserva(FILE *file, Reserva reserva);
void obterNovaReserva(FILE *file);
int excluirReserva(FILE *file, int numeroQuarto);
double calcularValorTotal(int numero, int diaIn, int mesIn, int anoIn, int diaOut, int mesOut, int anoOut);
void realizarCheckIn(FILE *file);
void realizarCheckOut(FILE *file);
void realizarPagamento(FILE *file, int numeroQuarto);
void mudarStatusQuarto(FILE *file, int numeroQuarto, enum Status status);
void RegistraHoraSaida(FILE *file, int numeroQuarto);
void consultarReservas(FILE *file);
float calcularValoresRecebidos(FILE *file);
